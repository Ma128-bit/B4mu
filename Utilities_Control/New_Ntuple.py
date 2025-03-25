import sys, os, subprocess
import time
start = time.time()

import argparse
from scipy.constants import c as speed_of_light

print("Done imports p1!")

os.environ['PATH'] += ':/lustrehome/mbuonsante/miniconda3/envs/root_env/bin'
from ROOT import RDataFrame, gROOT, EnableImplicitMT, gInterpreter, TH1F, TString, std, TFile
print("Done imports p2!")

gROOT.SetBatch(True)
EnableImplicitMT()

gInterpreter.Declare("""
    int redef_isMC(unsigned int slot, const ROOT::RDF::RSampleInfo &id){
        if(id.Contains("MC")) return 1;
        if(id.Contains("Data")) return 0;
        else return -1;
    }
    TString add_ID(unsigned int slot, const ROOT::RDF::RSampleInfo &id){
        if(id.Contains("MC_B2mu2K_2022")) return "MC_B2mu2K_2022";
        if(id.Contains("MC_B2mu2K_2023")) return "MC_B2mu2K_2023";
        if(id.Contains("MC_B2mu2K_2024")) return "MC_B2mu2K_2024";
        if(id.Contains("MC_B2muKpi_2022")) return "MC_B2muKpi_2022";
        if(id.Contains("MC_B2muKpi_2023")) return "MC_B2muKpi_2023";
        if(id.Contains("MC_B2muKpi_2024")) return "MC_B2muKpi_2024";
                     
        if(id.Contains("Data_B2mu2K_2022")) return "Data_B2mu2K_2022";
        if(id.Contains("Data_B2mu2K_2023")) return "Data_B2mu2K_2023";
        if(id.Contains("Data_B2mu2K_2024")) return "Data_B2mu2K_2024";
        else return "None";
    }
                     
    double new_tau2D(double QuadrupletVtx_x, double QuadrupletVtx_y, double BS_x, double BS_y, double Quadruplet_Pt){

        double ct = 5.366*TMath::Sqrt((QuadrupletVtx_x-BS_x)*(QuadrupletVtx_x-BS_x) + (QuadrupletVtx_y-BS_y)*(QuadrupletVtx_y-BS_y))/(Quadruplet_Pt);
        return ct;
    }

    double add_lumiW(unsigned int slot, const ROOT::RDF::RSampleInfo &id){
        if(id.Contains("MC_B2mu2K_2022")) return 142986.0/322522.0; //Normalization to the number of events in data
        if(id.Contains("MC_B2mu2K_2023")) return 123801.0/353676.0; //Normalization to the number of events in data
        if(id.Contains("MC_B2mu2K_2024")) return 419844.0/1225395.0; //Normalization to the number of events in data
        if(id.Contains("MC_B2muKpi_2022")) return 1;
        if(id.Contains("MC_B2muKpi_2023")) return 1;

        if(id.Contains("Data_B2mu2K_2022")) return 1;
        if(id.Contains("Data_B2mu2K_2023")) return 1;
        if(id.Contains("Data_B2mu2K_2024")) return 1;
        else return 1;
    }

    double weight_to_new_ctau(double old_ctau, double new_ctau, double ct){
        /*
        Returns an event weight based on the ratio of the normalised lifetime distributions.
        old_ctau: ctau used for the sample production
        new_ctau: target ctau
        ct      : per-event lifetime
        */
        double weight = 1;
        if(ct>0){
            weight = old_ctau / new_ctau * TMath::Exp((1.0 / old_ctau - 1.0 / new_ctau) * ct);
        }
        return weight;
    }

    struct add_new_ctau{
        double old_ctau, new_ctau;
        add_new_ctau(double oldct, double newct) : old_ctau(oldct), new_ctau(newct)  {}
        float operator()(const TString& ID, const double cts, const double ctc) {
            if (ID.Contains("MC_B2mu2K")) {return weight_to_new_ctau(old_ctau, new_ctau, ctc);}
            else if (ID.Contains("Data") || ID.Contains("Bd202")) {return 1;}
            else return 1;
        }
    };

    struct PV_WeightsComputer{
        std::vector<TString> name;
        std::vector<TH1F*> histo;
        bool flag;
        PV_WeightsComputer(std::vector<TString>& s, const std::vector<TH1F*>& histograms, bool f): name(s), histo(histograms), flag(f) {}
        float operator()(const TString& ID, const uint nPileUpInt) {
            auto it = std::find(name.begin(), name.end(), ID);
            if (it != name.end()) {
                int indx = std::distance(name.begin(), it);
                int nP = histo[indx]->GetXaxis()->FindBin(nPileUpInt);
                //std::cout<<name[indx]<<std::endl;
                if (!flag) { return histo[indx]->GetBinContent(nP);}
                else { return histo[indx]->GetBinError(nP);}
            } else {
                if (!flag) { return 1;}
                else { return 0;}
            }
        }
    };

""")

from ROOT import add_new_ctau, PV_WeightsComputer

branches = [
    "isMC", "lumi", "run", "evt", "nPileUpInt", "PVCollection_Size", 
    "Mu1_Pt", "Mu2_Pt", "Mu3_Pt", "Mu4_Pt","Mu1_Eta", "Mu2_Eta", "Mu3_Eta", "Mu4_Eta",
    "Mu1_Phi", "Mu2_Phi", "Mu3_Phi", "Mu4_Phi", "Quadruplet_Mass", "FlightDistBS_SV_Significance",
    "QuadrupletVtx_Chi2", "Quadruplet_Pt", "Quadruplet_Eta", "Quadruplet_Phi", "mu1_pfreliso03",
    "mu2_pfreliso03", "mu1_bs_dxy_sig", "mu2_bs_dxy_sig", "mu3_bs_dxy_sig", "mu4_bs_dxy_sig", 
    "vtx_prob", "vtx_ref_prob", "Cos3d_PV_SV", "Cos3d_BS_SV", "Cos2d_PV_SV", "Cos2d_BS_SV", "Gen_ct_signal", "Gen_ct_control",
    "RefittedSV_Mass", "RefittedSV_Mass_err", "MVASoft1", "MVASoft2", "Ditrk_mass", "Dimu_mass", "new_ct"
]
cuts={
    "Jpsi": [[75,60], [110,85], [140,110]],
    "phi":   [[7,7], [8.5,8.5], [10,10]],
    "Kstar":   [[30,30], [50,35], [55,44]]
}
resonances = {
    "Jpsi": 3.0969,
    "phi": 1.019455,
    "Kstar": 0.7
}

def load_df(index, treename, Files):
    files = Files[index]
    frame = RDataFrame(treename, files, branches)
    return frame

def check_type():
    parser = argparse.ArgumentParser(description="Set B2mu2K202X or B2muKpi202X")
    parser.add_argument("--type", type=str, help="B2mu2K202X or B2muKpi202X")
    parser.add_argument("--label", type=str, help="")
    args = parser.parse_args()
    type = args.type
    label = args.label
    if "2024" in type:
        print("WARNING: 2024 data must be under cmssw14, just make shure to have them in the right folder")
    if "B2mu2K" in type:
        return "B2mu2K", int(type.replace("B2mu2K", "")), label
    elif "B2muKpi" in type:
        return "B2muKpi", int(type.replace("B2muKpi", "")), label
    else:
        print("ERROR: choose --type between B4mu and control")
        sys.exit()

def two_mu_cuts(df, cuts, resonances, B2mu2X):
    if B2mu2X=="B2mu2K":
        res = "phi"
    else:
        res = "Kstar"
    sel_out = "("
    for j in range(3):
        sel = "(("
        sel += f"{resonances[res]}-Ditrk_mass>-{cuts[res][j][1]/1000} && ({resonances[res]}-Ditrk_mass)<{cuts[res][j][0]/1000} && {resonances['Jpsi']}-Dimu_mass>-{cuts['Jpsi'][j][1]/1000} && ({resonances['Jpsi']}-Dimu_mass)<{cuts['Jpsi'][j][0]/1000}"
        sel += f") && category=={j})"
        sel_out += sel
        if j!=2:
            sel_out += " || "
    sel_out += ")"

    df = df.Filter(sel_out)
    return df


if __name__ == "__main__":
    B2mu2X, year, label = check_type()

    pos = "/lustrehome/mbuonsante/B_4mu/CMSSW_13_0_13/src/Analysis/FinalFiles_"+B2mu2X+"_"+label+"/"
    pos_24 = "/lustrehome/mbuonsante/B_4mu/CMSSW_14_0_18_patch1/src/Analysis/FinalFiles_"+B2mu2X+"_"+label+"/"
    Files = {
        "B2mu2K2022": [pos+"Analyzed_Data_B2mu2K_2022.root", pos+"Analyzed_MC_B2mu2K_2022.root"],
        "B2mu2K2023": [pos+"Analyzed_Data_B2mu2K_2023.root", pos+"Analyzed_MC_B2mu2K_2023.root"],
        "B2mu2K2024": [pos_24+"Analyzed_Data_B2mu2K_2024.root", pos_24+"Analyzed_MC_B2mu2K_2024.root"],
        "B2muKpi2022": [pos+"Analyzed_Data_B2muKpi_2022.root"],
        "B2muKpi2023": [pos+"Analyzed_Data_B2muKpi_2023.root"],
        "B2muKpi2024": [pos_24+"Analyzed_Data_B2muKpi_2024.root"]
    }
    print("Starting!")
    start_2 = time.time()
    df = load_df(B2mu2X + str(year),  "FinalTree", Files)
    df = df.Define("year", f"{year}")
    df = df.Redefine("nPileUpInt", "(unsigned int)nPileUpInt")
    df = df.Redefine("run", "(unsigned int)run")
    df = df.Redefine("lumi", "(unsigned int)lumi")
    df = df.DefinePerSample("ID", "add_ID(rdfslot_, rdfsampleinfo_)")
    df = df.DefinePerSample("wnevt", "add_lumiW(rdfslot_, rdfsampleinfo_)")
    df = df.DefinePerSample("isMC2", "redef_isMC(rdfslot_, rdfsampleinfo_)")
    df = df.Redefine("isMC", "isMC2")

    # Bs LifeTime reweithg: taken from Rebecca: https://gitlab.cern.ch/regartner/b4mu-analysis/-/blob/master/data_MC_correction/bs_lifetime_reweighting.py
    ctau_actual = 4.4129450e-2  # from EvtGen  # in cm -> tau = 1.47e-12
    ctau_pdg = 1.527e-12 * speed_of_light * 100.0  # in cm

    #df = df.Define("ctau_weight_central", add_new_ctau(ctau_actual, ctau_pdg), ["ID", "Gen_ct_signal", "Gen_ct_control"])
    df = df.Define("ctau_weight_central", add_new_ctau(ctau_actual, ctau_pdg), ["ID", "new_ct", "new_ct"])

    #FixME start
    h_vectors = std.vector(TH1F)()
    h_name = std.vector(TString)()
    histo_file = TFile.Open("PileUp/ratio_histo_"+str(year)+"_"+label+".root")
    for name in ["B2mu2K_"]:
        h_vectors.push_back(histo_file.Get("pileUp_ratio_" + name + str(year)))
        h_name.push_back("MC_"+name+str(year))
    
    df = df.Define("weight_pileUp", PV_WeightsComputer(h_name, h_vectors, False), ["ID", "nPileUpInt"])
    #FixME end
    
    #Define eta category
    branches.append("category")
    branches.append("eta_category")
    branches.append("RefittedSV_Mass_reso")
    branches.append("RefittedSV_Mass_eq")
    df = df.Define("RefittedSV_Mass_eq", "RefittedSV_Mass-Dimu_mass+3.0969")
    df = df.Define("RefittedSV_Mass_reso", "sqrt(RefittedSV_Mass_err)")
    df = df.Define("category", "RefittedSV_Mass_reso < 0.027 ? 0 : RefittedSV_Mass_reso < 0.038 ? 1 : 2")
    df = df.Define("eta_category", "abs(Quadruplet_Eta) < 0.8 ? 0 : (abs(Quadruplet_Eta) < 1.2 ? 1 : 2)")

    if not os.path.exists("ROOTFiles_"+label):
        subprocess.run(["mkdir", "ROOTFiles_"+label])

    df = df.Define("new_ct2D", "new_tau2D(QuadrupletVtx_x, QuadrupletVtx_y, BS_x, BS_y, Quadruplet_Pt)")
    df = two_mu_cuts(df, cuts, resonances, B2mu2X)
    b_weights = ["ID", "year", "weight_pileUp", "ctau_weight_central","weight","wnevt","new_ct2D"]
    df=df.Define("weight", "wnevt*weight_pileUp*ctau_weight_central")
    df.Snapshot("FinalTree", "ROOTFiles_"+label+"/All"+B2mu2X+str(year)+".root", branches+b_weights)
