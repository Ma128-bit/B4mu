from ROOT import TFile, gROOT, gDirectory
gROOT.SetBatch(True)
import matplotlib.pyplot as plt
import numpy as np
import math, os, draw_utilities

class ROOTDrawer(draw_utilities.ROOTDrawer):
    pass

def scan_with_cut(tree, hist1, hist2, cut, dir):
    in_events_hist1 = hist1.Integral(0, hist1.GetNbinsX() + 1)
    in_events_hist2 = hist2.Integral(0, hist2.GetNbinsX() + 1)
    if dir == 'R':
        passed_events_hist1 = hist1.Integral(hist1.FindBin(cut), hist1.GetNbinsX() + 1)
        passed_events_hist2 = hist2.Integral(hist2.FindBin(cut), hist2.GetNbinsX() + 1)
    if dir == 'L':
        passed_events_hist1 = hist1.Integral(0, hist1.FindBin(cut))
        passed_events_hist2 = hist2.Integral(0, hist2.FindBin(cut))
    return [passed_events_hist1/in_events_hist1, passed_events_hist2/in_events_hist2]
    
var_dict = {
    "FlightDistBS_SV_Significance": ["(BsJPsiPhi_sel_OS1>0 || BsJPsiPhi_sel_OS2>0)","(60,0,30)",'R',0, 6, 0.5],
    "QuadrupletVtx_Chi2": ["(BsJPsiPhi_sel_OS1>0 || BsJPsiPhi_sel_OS2>0)","(200,0,200)",'L', 3, 200, 1],
    "Dimu_OS1_1_chi2": ["(BsJPsiPhi_sel_OS1>0 || BsJPsiPhi_sel_OS2>0)","(120,0,60)",'L', 1, 60, 0.5],
    "Dimu_OS1_2_chi2": ["(BsJPsiPhi_sel_OS1>0 || BsJPsiPhi_sel_OS2>0)","(120,0,60)",'L', 1, 60, 0.5],
    "Dimu_OS2_1_chi2": ["(BsJPsiPhi_sel_OS1>0 || BsJPsiPhi_sel_OS2>0)","(120,0,60)",'L', 1, 60, 0.5],
    "Dimu_OS2_2_chi2": ["(BsJPsiPhi_sel_OS1>0 || BsJPsiPhi_sel_OS2>0)","(120,0,60)",'L', 1, 60, 0.5]
}

if __name__ == "__main__":
    file = TFile("../Analysis/FinalFiles/Analyzed_Data_2022All.root", "READ")
    tree = file.Get("FinalTree")
    out_dir="AMS_plot"
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)
    for var in var_dict:
        tree.Draw(var+">>hm"+var_dict[var][1], var_dict[var][0]+" && isMC==1")
        hm = gDirectory.Get("hm")
        tree.Draw(var+">>hd"+var_dict[var][1], var_dict[var][0]+" && isMC==0 && abs(Quadruplet_Mass-5.366)>0.15")
        hd = gDirectory.Get("hd")

        hm.Scale(hd.Integral(0, hd.GetNbinsX()+1)/hm.Integral(0, hm.GetNbinsX()+1))

        ratio = []
        AMS = []
        cuts = []
        scan_v = np.arange(var_dict[var][3], var_dict[var][4], var_dict[var][5])

        for cut in scan_v:
            out = scan_with_cut(tree, hm, hd, cut, var_dict[var][2])
            ratio.append(out[0]/out[1])
            cuts.append(cut)
            AMS.append(math.sqrt(2*((out[0]+out[1])*math.log(1+out[0]/out[1]) - out[0])))
            
        cutx = cuts[AMS.index(max(AMS))]
        plt.figure(figsize=(8, 4))
        plt.plot(scan_v, AMS, label='AMS Curve', color='blue', linestyle='-', linewidth=1)
        plt.axvline(x=cutx, color='red', linestyle='--', label='Best Cut')
        plt.xlabel('cut')
        plt.ylabel('AMS')
        plt.title(f"{var} - cut {cutx}")
        plt.legend()
        plt.savefig(out_dir+"/"+var+"_AMS.png")
            
        canvas = ROOTDrawer(SetLogY=True)
        canvas.HaddTH1(hm, Color=4, SetXName=var, SetYName="a.u.", Fill=True, label="Signal MC")
        canvas.HaddTH1(hd, Color=2, Fill=True, FillStyle=3005, DrawOpt="h same", label="Data Sidebands")
        canvas.DefTLine(Color=1, Orientation=0, X=cutx,  label="Cut")
        canvas.MakeLegend()
        canvas.Save(out_dir+"/"+var+"_histo.png", era=2022, extra="Preliminary")
        canvas.Delete()
