import sys, os, subprocess, json
import time
start = time.time()

import pickle
import argparse
from tqdm import tqdm
from ROOT import RDataFrame, gROOT, EnableImplicitMT, gInterpreter, TH1F, TString, std, TFile
from scipy.constants import c as speed_of_light

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
        if(id.Contains("MC_B2muKpi_2022")) return "MC_B2muKpi_2022";
        if(id.Contains("MC_B2muKpi_2023")) return "MC_B2muKpi_2023";

        if(id.Contains("Data_B2mu2K_2022")) return "Data_B2mu2K_2022";
        if(id.Contains("Data_B2mu2K_2023")) return "Data_B2mu2K_2022";
        else return "None";
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
    "vtx_prob", "Cos3d_PV_SV", "Cos3d_BS_SV", "Cos2d_PV_SV", "Cos2d_BS_SV", "Gen_ct_signal", "Gen_ct_control",
    "RefittedSV_Mass", "RefittedSV_Mass_err", "MVASoft1", "MVASoft2", "Ditrk_mass", "Dimu_mass"
]
cuts={
    "Jpsi": [[75,60], [110,85], [140,110]],
    "phi":   [[30,30], [50,35], [55,44]],
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
    loadInfo("config/config_"+label+".txt")

    pos = "/lustrehome/mbuonsante/B_4mu/CMSSW_13_0_13/src/Analysis/FinalFiles_"+B2mu2X+"_"+label+"/"
    Files = {
        "B2muKK2022": [pos+"Analyzed_Data_B2mu2K_2022.root", pos+"Analyzed_MC_B2mu2K_2022.root"],
        "B2muKK2023": [pos+"Analyzed_Data_B2mu2K_2023.root", pos+"Analyzed_MC_B2mu2K_2023.root"],
        "B2muKpi2022": [pos+"Analyzed_Data_B2muKpi_2022.root"],
        "B2muKpi2023": [pos+"Analyzed_Data_B2muKpi_2022.root"]
    }
    print("Starting!")
    start_2 = time.time()
    df = load_df(B2mu2X + str(year),  "FinalTree", Files)
    df = df.Define("year", f"{year}")
    df = df.DefinePerSample("isMC2", "redef_isMC(rdfslot_, rdfsampleinfo_)")
    df = df.Redefine("isMC", "isMC2")

    # Bs LifeTime reweithg: taken from Rebecca: https://gitlab.cern.ch/regartner/b4mu-analysis/-/blob/master/data_MC_correction/bs_lifetime_reweighting.py
    ctau_actual = 4.4129450e-01  # from EvtGen  # in mm -> tau = 1.47e-12
    ctau_pdg = 1.527e-12 * speed_of_light * 1000.0  # in mm ===> 457 mm

    df = df.Define("ctau_weight_central", add_new_ctau(ctau_actual, ctau_pdg), ["ID", "Gen_ct_signal", "Gen_ct_control"])

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
    df = df.Define("RefittedSV_Mass_reso", "sqrt(RefittedSV_Mass_err)")
    df = df.Define("category", "RefittedSV_Mass_reso < 0.027 ? 0 : RefittedSV_Mass_reso < 0.038 ? 1 : 2")
    df = df.Define("eta_category", "abs(Quadruplet_Eta) < 0.8 ? 0 : (abs(Quadruplet_Eta) < 1.2 ? 1 : 2)")

    if not os.path.exists("ROOTFiles_"+label):
        subprocess.run(["mkdir", "ROOTFiles_"+label])

    df = two_mu_cuts(df, cuts, resonances, B2mu2X)
    b_weights = ["ID", "year", "weight_pileUp", "ctau_weight_central","weight"]
    df=df.Define("weight", "weight_pileUp*ctau_weight_central")
    df.Snapshot("FinalTree", "ROOTFiles_"+label+"/All"+B2mu2X+str(year)+".root", branches+b_weights)
