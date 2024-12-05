from ROOT import gROOT, TH1F, TChain, gDirectory, RooFit, kFALSE
gROOT.SetBatch(True)
import sys, os, subprocess, argparse
import cmsstyle as CMS

import pandas as pd


var = ["vtx_prob", "mu1_pfreliso03", "mu2_pfreliso03", "FlightDistBS_SV_Significance", "mu1_bs_dxy_sig", "mu2_bs_dxy_sig", "mu3_bs_dxy_sig", "mu4_bs_dxy_sig", "Cos2d_PV_SV", "Quadruplet_Eta","Quadruplet_Pt"]

binning_dict = {
    "vtx_prob": "(50,0.0,1.0)",
    "mu1_pfreliso03": "(50,0,10)",
    "mu2_pfreliso03": "(50,0,10)",
    "FlightDistBS_SV_Significance": "(50,0,400)",
    "mu1_bs_dxy_sig": "(50,-50,50)",
    "mu2_bs_dxy_sig": "(50,-50,50)",
    "mu3_bs_dxy_sig": "(50,-50,50)",
    "mu4_bs_dxy_sig": "(50,-50,50)",
    "Cos2d_PV_SV": "(50,0.95,1)",
    "Quadruplet_Eta": "(50,-2.5,2.5)",
    "Quadruplet_Pt": "(50,10,100)"
}

log_dict = {
    "vtx_prob": False,
    "mu1_pfreliso03": True,
    "mu2_pfreliso03": True,
    "FlightDistBS_SV_Significance": True,
    "mu1_bs_dxy_sig": True,
    "mu2_bs_dxy_sig": True,
    "mu3_bs_dxy_sig": True,
    "mu4_bs_dxy_sig": True,
    "Cos2d_PV_SV": True,
    "Quadruplet_Eta": False,
    "Quadruplet_Pt": False

}


def control_plots(file_name, year):
    if not os.path.exists("Control_Plots"):
        subprocess.run(["mkdir", "Control_Plots"])
    
    # Data ALL
    data = TChain("FinalTree")
    data.Add(file_name)
    
    for k in range(len(var)):
        varname = var[k]
        logy = log_dict[varname]
        s = str(k)
        binning = binning_dict[varname]
        numbers = binning.strip("()").split(",")
        numbers = [float(x) if x.isdigit() else float(x) for x in numbers]

        legend_label = "sWeighted"
        data.Draw(varname + ">>hdata_sig" + s+ binning, "nsigBs_sw*(isMC==0)")
        hdata_sig = TH1F(gDirectory.Get("hdata_sig" + s))
        data.Draw(varname + ">>hMC_sig" + s + binning, "weight*nsigBs_sw*(isMC>0)")
        hMC_sig = TH1F(gDirectory.Get("hMC_sig" + s))
            
        # Rescaling
        hMC_sig.Scale(1 / hMC_sig.Integral())
        hdata_sig.Scale(1 / hdata_sig.Integral())

        CMS.SetExtraText("Preliminary")
        CMS.SetLumi("34.6")
        CMS.SetEnergy(13.6)
        dicanvas = CMS.cmsDiCanvas("", numbers[1], numbers[2], 0, max(hdata_sig.GetMaximum(),hMC_sig.GetMaximum())*1.2, -6, 6, 'm(#mu^{+}#mu^{-}#K^{+}#K^{-}) [GeV/c^{2}]', f"a.u.", "ratio data/MC", square=CMS.kSquare, iPos=11, extraSpace=0, scaleLumi=None)
        dicanvas.SetCanvasSize(1200,1300)
        dicanvas.cd(1)
        hMC_sig.SetLineColor(4)
        hMC_sig.SetFillColor(4)
        hMC_sig.SetFillStyle(3004)
        hMC_sig.Draw("same")
        hdata_sig.SetLineColor(1)
        hdata_sig.Draw("samePE1")

        dicanvas.cd(2)
        h_x_ratio = hdata_sig.Clone()
        h_x_ratio.Sumw2()
        h_x_ratio.Divide(hMC_sig)
        h_x_ratio.SetLineColor(1)
        h_x_ratio.Draw()
        h_x_ratio.Draw("samePE1")
        dicanvas.Update()
        dicanvas.SaveAs("Control_Plots/" + varname + "_"+year+"_SPlot"+".png")
        dicanvas.Clear()

        h_x_ratio.Delete();
        hdata_sig.Delete();
        hMC_sig.Delete(); 

if __name__ == "__main__": 
    parser = argparse.ArgumentParser(description="--plots for control plots")
    parser.add_argument("--file", type=str, help="file name")
    parser.add_argument("--year", type=str, help="year (2022 or 2023)")
    args = parser.parse_args()
    file = args.file
    year = args.year
    control_plots(file, year)
