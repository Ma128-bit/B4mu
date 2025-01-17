from ROOT import gROOT, TH1F, TChain, gDirectory, RooFit, kFALSE, TLine, kRed, kDashed, gPad, TLegend
gROOT.SetBatch(True)
import sys, os, subprocess, argparse
import cmsstyle as CMS

import pandas as pd


var = ["vtx_prob", "mu1_pfreliso03", "mu2_pfreliso03", "FlightDistBS_SV_Significance", "mu1_bs_dxy_sig", "mu2_bs_dxy_sig", "mu3_bs_dxy_sig", "mu4_bs_dxy_sig", "Cos2d_BS_SV", "Quadruplet_Eta","Quadruplet_Pt", "RefittedSV_Mass", "RefittedSV_Mass_eq", "bdt"]

binning_dict = {
    "vtx_prob": "(50,0.01,1.0)",
    "mu1_pfreliso03": "(50,0,10)",
    "mu2_pfreliso03": "(50,0,10)",
    "FlightDistBS_SV_Significance": "(50,3,400)",
    "mu1_bs_dxy_sig": "(50,-100,100)",
    "mu2_bs_dxy_sig": "(50,-100,100)",
    "mu3_bs_dxy_sig": "(50,-75,75)",
    "mu4_bs_dxy_sig": "(50,-75,75)",
    "Cos2d_BS_SV": "(50,0.95,1)",
    "Quadruplet_Eta": "(50,-2.5,2.5)",
    "RefittedSV_Mass_eq": "(50,5.2,5.6)",
    "RefittedSV_Mass": "(50,5.2,5.6)",
    "Quadruplet_Pt": "(50,10,100)",
    "bdt": "(50,0,1)"
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
    "Cos2d_BS_SV": True,
    "Quadruplet_Eta": False,
    "RefittedSV_Mass_eq": False,
    "RefittedSV_Mass": False,
    "Quadruplet_Pt": False,
    "bdt": True
}

lumi={
    "2022": 34.6,
    "2023": 27.8,
    "2022+23": 62.4
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
        data.Draw(varname + ">>hdata_sig" + s+ binning, "nsigBs_sw*(isMC==0 && RefittedSV_Mass_eq>5.2 && RefittedSV_Mass_eq<5.7)")
        hdata_sig = TH1F(gDirectory.Get("hdata_sig" + s))
        data.Draw(varname + ">>hMC_sig" + s + binning, "nsigBs_sw*weight*(isMC>0)")
        hMC_sig = TH1F(gDirectory.Get("hMC_sig" + s))

        # Rescaling
        hMC_sig.Scale(1 / hMC_sig.Integral(1,int(numbers[0])))
        hdata_sig.Scale(1 / hdata_sig.Integral(1,int(numbers[0])))

        CMS.SetExtraText("Preliminary")
        CMS.SetLumi(f"{lumi[year]}")
        CMS.SetEnergy(13.6)
        if logy:
            dicanvas = CMS.cmsDiCanvas("", numbers[1], numbers[2], 0.0001, max(hdata_sig.GetMaximum(),hMC_sig.GetMaximum())*5, -0.1, 2.1, varname, f"a.u.", "ratio data/MC", square=CMS.kSquare, iPos=11, extraSpace=0, scaleLumi=None)
        else:
            if varname!="Quadruplet_Eta":
                dicanvas = CMS.cmsDiCanvas("", numbers[1], numbers[2], 0, max(hdata_sig.GetMaximum(),hMC_sig.GetMaximum())*1.2, -0.1, 2.1, varname, f"a.u.", "ratio data/MC", square=CMS.kSquare, iPos=11, extraSpace=0, scaleLumi=None)
            else:
                dicanvas = CMS.cmsDiCanvas("", numbers[1], numbers[2], 0, max(hdata_sig.GetMaximum(),hMC_sig.GetMaximum())*1.5, -0.1, 2.1, varname, f"a.u.", "ratio data/MC", square=CMS.kSquare, iPos=11, extraSpace=0, scaleLumi=None)

        dicanvas.SetCanvasSize(1200,1300)
        dicanvas.cd(1)
        if logy:
            gPad.SetLogy()
        hMC_sig.SetLineColor(4)
        hMC_sig.SetFillColor(4)
        hMC_sig.SetLineWidth(2)
        hMC_sig.SetFillStyle(3004)
        hMC_sig.Draw("Hsame")
        hdata_sig.SetLineColor(1)
        hdata_sig.SetLineWidth(2)
        hdata_sig.Draw("samePE1")

        legend = TLegend(0.61, 0.7, 0.9, 0.9)
        legend.AddEntry(hdata_sig, "sWeighted Data", "lep") 
        legend.AddEntry(hMC_sig, "MC B^{0}_{s} J/#psi(#mu#mu)#phi(KK)", "f")  
        legend.SetBorderSize(0)       
        legend.SetFillStyle(0)    
        legend.Draw("same")

        dicanvas.cd(2)
        line1 = TLine(numbers[1], 1, numbers[2], 1) 
        line2 = TLine(numbers[1], 0, numbers[2], 0) 
        line3 = TLine(numbers[1], 2, numbers[2], 2) 
        line1.SetLineColor(kRed)
        line1.SetLineStyle(kDashed)
        line1.SetLineWidth(2)
        line2.SetLineColor(1)
        line2.SetLineStyle(kDashed)
        line2.SetLineWidth(2)
        line3.SetLineColor(1)
        line3.SetLineWidth(2)
        line3.SetLineStyle(kDashed)
        h_x_ratio = hdata_sig.Clone()
        h_x_ratio.Sumw2()
        h_x_ratio.Divide(hMC_sig)
        h_x_ratio.SetLineColor(1)
        h_x_ratio.SetLineWidth(2)
        h_x_ratio.Draw("samePE1")
        line1.Draw("same")

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
