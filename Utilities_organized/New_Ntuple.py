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
    #include "cpp_library.h"
""")

from ROOT import SF_WeightsComputer, PV_WeightsComputer, add_index, add_new_ctau, loadInfo

branches = [
    "isMC", "lumi", "run", "evt", "nPileUpInt", "PVCollection_Size", "isGlobal", "isPF", "isMedium", 
    "Mu1_Pt", "Mu2_Pt", "Mu3_Pt", "Mu4_Pt","Mu1_Eta", "Mu2_Eta", "Mu3_Eta", "Mu4_Eta",
    "Mu1_Phi", "Mu2_Phi", "Mu3_Phi", "Mu4_Phi", "Quadruplet_Mass", "FlightDistBS_SV_Significance",
    "QuadrupletVtx_Chi2", "Quadruplet_Pt", "Quadruplet_Eta", "Quadruplet_Phi", "mu1_pfreliso03",
    "mu2_pfreliso03", "mu3_pfreliso03", "mu4_pfreliso03", "mu1_bs_dxy_sig", "mu2_bs_dxy_sig",
    "mu3_bs_dxy_sig", "mu4_bs_dxy_sig", "vtx_prob", "vtx_ref_prob", "Cos3d_PV_SV", "Cos3d_BS_SV", "Cos2d_PV_SV", 
    "Cos2d_BS_SV", "dR_max", "isJPsiPhi", "OS1v1_mass", "OS2v1_mass", "OS1v2_mass", "OS2v2_mass", 
    "OS1v1_mass_err", "OS2v1_mass_err", "OS1v2_mass_err", "OS2v2_mass_err", "Quadruplet_Mass_eq",
    "RefittedSV_Mass", "RefittedSV_Mass_err","MVASoft1", "MVASoft2", "MVASoft3", "MVASoft4",
    "OS1v1_Chi2", "OS2v1_Chi2", "OS1v2_Chi2", "OS2v2_Chi2", "Gen_ct_signal", "Gen_ct_control", "new_ct"
]

cuts={
    "Jpsi": [[75,60], [110,85], [140,110]],
    "phi":   [[30,30], [50,35], [55,44]],
    "psi2s": [[80,70], [90,80], [110,90]],
    "omega": [[40,35], [45,40], [60,45]]
}

def load_df(isB4mu, year, treename, Files):
    if isB4mu == True:
        files = Files["B4mu"+str(year)]
        br = branches
    else:
        files = Files["control"+str(year)]
        br = branches
    frame = RDataFrame(treename, files, br)
    return frame

def check_type():
    parser = argparse.ArgumentParser(description="Set B4mu202X or control202X")
    parser.add_argument("--type", type=str, help="B4mu202X or control202X")
    parser.add_argument("--label", type=str, help="")
    args = parser.parse_args()
    type = args.type
    label = args.label
    if "2024" in type:
        print("WARNING: 2024 data must be under cmssw14, just make shure to have them in the right folder")
    if "B4mu" in type:
        return True, int(type.replace("B4mu", "")), label
    elif "control" in type:
        return False, int(type.replace("control", "")), label
    else:
        print("ERROR: choose --type between B4mu and control")
        sys.exit()

def two_mu_cuts(df, cuts):
    sel_out = "("
    for j in range(3):
        sel = "(("
        for i in range(1,3):
            sel += f'((1.019461-OS1v{i}_mass)>-{cuts["phi"][j][1]/1000} && (1.019461-OS1v{i}_mass)<{cuts["phi"][j][0]/1000} && (3.0969-OS2v{i}_mass)<{cuts["Jpsi"][j][0]/1000} && (3.0969-OS2v{i}_mass)>-{cuts["Jpsi"][j][1]/1000}) ||'
            sel += f'((1.019461-OS2v{i}_mass)>-{cuts["phi"][j][1]/1000} && (1.019461-OS2v{i}_mass)<{cuts["phi"][j][0]/1000} && (3.0969-OS1v{i}_mass)<{cuts["Jpsi"][j][0]/1000} && (3.0969-OS1v{i}_mass)>-{cuts["Jpsi"][j][1]/1000})'
            if i==1:
                sel += "||"
        sel += f") && category=={j})"
        sel_out += sel
        if j!=2:
            sel_out += " || "
    sel_out += ")"

    df = df.Filter(sel_out)
    return df

def two_mu_vetos(df, resoname, cuts):
    resonances = {
        "Jpsi": 3.0969,
        "phi": 1.019455,
        "psi2s": 3.686097, 
        "omega": 0.78265
    }
    sel = ["","",""]
    i = 0
    for cut_c in cuts[resoname]:
        for ind in range(1,3):
            for jnd in range(1,3):
                sel[i] += f"((OS{ind}v{jnd}_mass-{resonances[resoname]})<-{cut_c[0]/1000} || (OS{ind}v{jnd}_mass-{resonances[resoname]})>{cut_c[1]/1000})"
                if (not(ind*jnd==4)):
                    sel[i] += " && "
        i+=1
    for i in range(3):
        sel[i] = "(("+sel[i]+") && eta_category=="+str(i)+")"
    sel2 = "(" + sel[0] + "||" + sel[1] + "||" +sel[2] + ")"
    
    df = df.Define(resoname+"cut", sel2)
    return df

if __name__ == "__main__":
    isB4mu, year, label = check_type()
    loadInfo("config/config_"+label+".txt")

    pos = "/lustrehome/mbuonsante/B_4mu/CMSSW_13_0_13/src/Analysis/FinalFiles_B4mu_"+label+"/"
    pos_24 = "/lustrehome/mbuonsante/B_4mu/CMSSW_14_0_18_patch1/src/Analysis/FinalFiles_B4mu_"+label+"/"
    Files = {
        "B4mu2022": [pos+"Analyzed_Data_B4mu_2022.root", pos+"Analyzed_MC_Bs_4mu_2022.root", pos+"Analyzed_MC_Bd_4mu_2022.root"],
        "B4mu2023": [pos+"Analyzed_Data_B4mu_2023.root", pos+"Analyzed_MC_Bs_4mu_2023.root", pos+"Analyzed_MC_Bd_4mu_2023.root"],
        "B4mu2024": [pos_24+"Analyzed_Data_B4mu_2024.root", pos_24+"Analyzed_MC_Bs_4mu_2024.root", pos_24+"Analyzed_MC_Bd_4mu_2024.root"],
        "control2022": [pos+"Analyzed_Data_B4mu_2022.root", pos+"Analyzed_MC_BsJPsiPhi_2022.root"],
        "control2023": [pos+"Analyzed_Data_B4mu_2023.root", pos+"Analyzed_MC_BsJPsiPhi_2023.root"],
        "control2024": [pos_24+"Analyzed_Data_B4mu_2024.root", pos_24+"Analyzed_MC_BsJPsiPhi_2024.root"],
    }

    print("Starting!")
    start_2 = time.time()
    df = load_df(isB4mu, year,  "FinalTree", Files)
    df = df.Define("year", add_index(year))
    df = df.Redefine("nPileUpInt", "(unsigned int)nPileUpInt")
    df = df.Redefine("run", "(unsigned int)run")
    df = df.Redefine("evt", "(unsigned int)evt")
    df = df.Redefine("lumi", "(unsigned int)lumi")
    df = df.DefinePerSample("ID", "add_ID(rdfslot_, rdfsampleinfo_)")
    df = df.DefinePerSample("isMC2", "redef_isMC(rdfslot_, rdfsampleinfo_)")
    df = df.Redefine("isMC", "isMC2")
    df = df.DefinePerSample("weight", "add_weight(rdfslot_, rdfsampleinfo_)")
    df = df.DefinePerSample("weight_err", "add_weight_err(rdfslot_, rdfsampleinfo_)")
    df = df.DefinePerSample("w_mc", "add_wDMC(rdfslot_, rdfsampleinfo_)")

    # Bs LifeTime reweithg: taken from Rebecca: https://gitlab.cern.ch/regartner/b4mu-analysis/-/blob/master/data_MC_correction/bs_lifetime_reweighting.py
    ctau_actual = 4.4129450e-02  # from EvtGen  # in cm -> tau = 1.47e-12
    ctau_pdg = 1.527e-12 * speed_of_light * 100.0  # in cm 
    ctau_heavy = (1.622) * 10 ** (-12) * speed_of_light * 100.0
    ctau_light = (1.429) * 10 ** (-12) * speed_of_light * 100.0

    df = df.Define("ctau_weight_central", add_new_ctau(ctau_actual, ctau_pdg), ["ID", "new_ct", "new_ct"])
    df = df.Define("ctau_weight_heavy", add_new_ctau(ctau_heavy, ctau_pdg), ["ID", "new_ct", "new_ct"])
    df = df.Define("ctau_weight_light", add_new_ctau(ctau_light, ctau_pdg), ["ID", "new_ct", "new_ct"])
    
    h_vectors = std.vector(TH1F)()
    h_name = std.vector(TString)()
    histo_file = TFile.Open("PileUp/ratio_histo_"+str(year)+"_"+label+".root")
    for name in ["Bd", "Bs", "BsJPsiPhi"]:
        if name!= "BsJPsiPhi":
            n = "signal_"
        else:
            n = "control_"
        h_vectors.push_back(histo_file.Get("pileUp_ratio_" + n + str(year)))
        h_name.push_back(name+str(year))
    
    df = df.Define("weight_pileUp", PV_WeightsComputer(h_name, h_vectors, False), ["ID", "nPileUpInt"])
    df = df.Define("weight_pileUp_err", PV_WeightsComputer(h_name, h_vectors, True), ["ID", "nPileUpInt"])    

    if not os.path.exists("ROOTFiles_"+label):
        subprocess.run(["mkdir", "ROOTFiles_"+label])

    #Define eta category
    branches.append("category")
    branches.append("eta_category")
    branches.append("RefittedSV_Mass_reso")
    df = df.Define("RefittedSV_Mass_reso", "sqrt(RefittedSV_Mass_err)")
    df = df.Define("category", "RefittedSV_Mass_reso < 0.027 ? 0 : RefittedSV_Mass_reso < 0.038 ? 1 : 2")
    df = df.Define("eta_category", "abs(Quadruplet_Eta) < 0.8 ? 0 : (abs(Quadruplet_Eta) < 1.2 ? 1 : 2)")

    #df = df.Define("new_ct", "new_tau(QuadrupletVtx_x, QuadrupletVtx_y, QuadrupletVtx_z, RefittedPV_x, RefittedPV_y, RefittedPV_z, Quadruplet_Pt, Quadruplet_Eta, Quadruplet_Phi)")

    if isB4mu==True:
        #Filters for omega and phi:
        df = two_mu_vetos(df, "Jpsi", cuts)
        df = two_mu_vetos(df, "phi", cuts)
        df = two_mu_vetos(df, "omega", cuts)
        df = two_mu_vetos(df, "psi2s", cuts)
        
        b_weights = ["ID", "year", "weight", "weight_err", "weight_pileUp", "weight_pileUp_err", "ctau_weight_central", "ctau_weight_heavy", "ctau_weight_light", "Jpsicut", "phicut", "omegacut", "psi2scut", "bdt_weight", "w_mc"]
        
        #df = df.Define("signal_weight", "weight * weight_pileUp * ctau_weight_central")
        df = df.Define("bdt_weight", "w_mc * weight_pileUp * ctau_weight_central")
        df = df.Filter("Jpsicut==1 && phicut==1 && omegacut==1 && psi2scut==1 && FlightDistBS_SV_Significance>0")
        df.Snapshot("FinalTree", "ROOTFiles_"+label+"/AllData"+str(year)+".root", branches+b_weights)
    else:
        df = two_mu_cuts(df, cuts)
        df = df.Define("NewMassEqation","NewMassEqation(OS1v1_mass, OS2v1_mass, OS1v2_mass, OS2v2_mass, category, RefittedSV_Mass)")
        b_weights = ["ID", "year", "weight", "weight_err", "weight_pileUp", "weight_pileUp_err", "ctau_weight_central", "NewMassEqation"]
        #df = df.Define("control_weight", "weight * weight_pileUp * ctau_weight_central")
        df.Snapshot("FinalTree", "ROOTFiles_"+label+"/AllControl"+str(year)+".root", branches+b_weights)
    
    print("Performed ",df.GetNRuns()," loops")
    end = time.time()
    print('Partial execution time ', end-start_2)
    print('Total execution time ', end-start)


