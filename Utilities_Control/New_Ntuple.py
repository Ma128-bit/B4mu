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

branches = [
    "isMC", "lumi", "run", "evt", "nPileUpInt", "PVCollection_Size", 
    "Mu1_Pt", "Mu2_Pt", "Mu3_Pt", "Mu4_Pt","Mu1_Eta", "Mu2_Eta", "Mu3_Eta", "Mu4_Eta",
    "Mu1_Phi", "Mu2_Phi", "Mu3_Phi", "Mu4_Phi", "Quadruplet_Mass", "FlightDistBS_SV_Significance",
    "QuadrupletVtx_Chi2", "Quadruplet_Pt", "Quadruplet_Eta", "Quadruplet_Phi", "mu1_pfreliso03",
    "mu2_pfreliso03", "mu1_bs_dxy_sig", "mu2_bs_dxy_sig", "mu3_bs_dxy_sig", "mu4_bs_dxy_sig", 
    "vtx_prob", "Cos3d_PV_SV", "Cos3d_BS_SV", "Cos2d_PV_SV", "Cos2d_BS_SV",
    "RefittedSV_Mass", "RefittedSV_Mass_err", "MVASoft1", "MVASoft2", "Ditrk_mass", "Dimu_mass"
]
cuts={
    "Jpsi": [[75,60], [110,85], [140,110]],
    "phi":   [[30,30], [50,35], [55,44]]
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
    parser = argparse.ArgumentParser(description="Set B2muKK202X or B2muKpi202X")
    parser.add_argument("--type", type=str, help="B2muKK202X or B2muKpi202X")
    parser.add_argument("--label", type=str, help="")
    args = parser.parse_args()
    type = args.type
    label = args.label
    if "B2muKK" in type:
        return 1, int(type.replace("B2muKK", "")), label
    elif "B2muKpi" in type:
        return 0, int(type.replace("B2muKpi", "")), label
    else:
        print("ERROR: choose --type between B4mu and control")
        sys.exit()