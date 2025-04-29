from ROOT import gROOT, TH1F, TChain, gDirectory, RooFit, kFALSE, TLine, kRed, kDashed, gPad, TLegend
gROOT.SetBatch(True)
import sys, os, subprocess, argparse
import cmsstyle as CMS

#var = ["vtx_prob", "mu1_pfreliso03", "mu2_pfreliso03", "FlightDistBS_SV_Significance", "mu1_bs_dxy_sig", "mu2_bs_dxy_sig", "mu3_bs_dxy_sig", "mu4_bs_dxy_sig", "Cos2d_BS_SV", "Quadruplet_Eta","Quadruplet_Pt", "RefittedSV_Mass_eq", "Mu1_Eta", "Mu1_Pt", "RefittedSV_Mass_reso"]
var = ["PVCollection_Size"]

binning_dict = {
    "vtx_prob": "(50,0.01,1.0)",
    "mu1_pfreliso03": "(50,0,10)",
    "mu2_pfreliso03": "(50,0,10)",
    "MVASoft1": "(50,0.2,0.8)",
    "MVASoft2": "(50,0.2,0.8)",
    "FlightDistBS_SV_Significance": "(50,0,400)",
    "RefittedSV_Mass_reso": "(50,0.01,0.08)",
    "mu1_bs_dxy_sig": "(50,-120,150)",
    "mu2_bs_dxy_sig": "(50,-120,150)",
    "mu3_bs_dxy_sig": "(50,-75,75)",
    "mu4_bs_dxy_sig": "(50,-75,75)",
    "Cos2d_BS_SV": "(50,0.95,1)",
    "Quadruplet_Eta": "(50,-2.5,2.5)",
    "RefittedSV_Mass_eq": "(50,5.25,5.5)",
    "RefittedSV_Mass": "(50,5.2,5.6)",
    "Quadruplet_Pt": "(50,10,100)",
    "bdt": "(50,0,1)",
    "100*new_ct/2.998": "(50,0,14)",
    "Mu1_Eta": "(50,-2.5,2.5)",
    "Mu1_Pt": "(50,4, 50)",
    "PVCollection_Size": "(70,0,70)",
    "category": "(10,-1,5)"
}

log_dict = {
    "vtx_prob": False,
    "mu1_pfreliso03": True,
    "mu2_pfreliso03": True,
    "MVASoft1": False,
    "MVASoft2": False,
    "FlightDistBS_SV_Significance": True,
    "RefittedSV_Mass_reso": False,
    "mu1_bs_dxy_sig": True,
    "mu2_bs_dxy_sig": True,
    "mu3_bs_dxy_sig": True,
    "mu4_bs_dxy_sig": True,
    "Cos2d_BS_SV": True,
    "Quadruplet_Eta": False,
    "RefittedSV_Mass_eq": False,
    "RefittedSV_Mass": False,
    "Quadruplet_Pt": False,
    "bdt": True,
    "100*new_ct/2.998": True,
    "Mu1_Eta": False,
    "Mu1_Pt": False,
    "PVCollection_Size": False,
    "category": False
}

x_name = {
    "vtx_prob": "Vertex Probability",
    "mu1_pfreliso03": "#mu_{1} PF relative iso.",
    "mu2_pfreliso03": "#mu_{2} PF relative iso.",
    "MVASoft1": "",
    "MVASoft2": "",
    "FlightDistBS_SV_Significance": "L_{xy}^{sig}",
    "RefittedSV_Mass_reso": "",
    "mu1_bs_dxy_sig": "#mu_{1} dxy_{sig}",
    "mu2_bs_dxy_sig": "#mu_{2} dxy_{sig}",
    "mu3_bs_dxy_sig": "#mu_{3} dxy_{sig}",
    "mu4_bs_dxy_sig": "#mu_{4} dxy_{sig}",
    "Cos2d_BS_SV": "cos(#theta)",
    "Quadruplet_Eta": "B |#eta|",
    "RefittedSV_Mass_eq": "4#mu mass", 
    "RefittedSV_Mass": "4#mu mass", 
    "Quadruplet_Pt": "B p_{T}",
    "bdt": "BDT score",
    "100*new_ct/2.998": "",
    "Mu1_Eta": "#mu_{1} |#eta|",
    "Mu1_Pt": "#mu_{1} p_{T}",
    "PVCollection_Size": "N. PV",
    "category": "category"
}

lumi={
    "2022": 34.6,
    "2023": 27.7,
    "2024": 108.4,
    "2022+2023": 62.4,
    "2022+2023+2024": 170.7
}

def control_plots(file_name, year, reweight):
    if not os.path.exists(f"Control_Plots_{year}"):
        subprocess.run(["mkdir", f"Control_Plots_{year}"])
    
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
        if reweight:
            if "2024" in year:
                data.Draw(varname + ">>hMC_sig_wrw" + s + binning, "nsigBs_sw*bdt_reweight_0*bdt_reweight_1*bdt_reweight_2*weight*(isMC>0)")
            else:
                data.Draw(varname + ">>hMC_sig_wrw" + s + binning, "nsigBs_sw*bdt_reweight_0*bdt_reweight_1*weight*(isMC>0)")
            hMC_sig_wrw = TH1F(gDirectory.Get("hMC_sig_wrw" + s))

        # Rescaling
        hMC_sig.Scale(1 / hMC_sig.Integral(1,int(numbers[0])))
        if reweight:
            hMC_sig_wrw.Scale(1 / hMC_sig_wrw.Integral(1,int(numbers[0])))
        hdata_sig.Scale(1 / hdata_sig.Integral(1,int(numbers[0])))

        
        CMS.SetLumi(f"{year}, {lumi[year]}")
        CMS.SetEnergy(13.6)
        if logy:
            dicanvas = CMS.cmsDiCanvas("", numbers[1], numbers[2], 0.0001, max(hdata_sig.GetMaximum(),hMC_sig.GetMaximum())*5, -0.1, 2.1, x_name[varname], f"a.u.", "ratio data/MC", square=CMS.kSquare, iPos=11, extraSpace=0, scaleLumi=None)
        else:
            if varname!="Quadruplet_Eta":
                dicanvas = CMS.cmsDiCanvas("", numbers[1], numbers[2], 0, max(hdata_sig.GetMaximum(),hMC_sig.GetMaximum())*1.5, -0.1, 2.1, x_name[varname], f"a.u.", "ratio data/MC", square=CMS.kSquare, iPos=11, extraSpace=0, scaleLumi=None)
            else:
                dicanvas = CMS.cmsDiCanvas("", numbers[1], numbers[2], 0, max(hdata_sig.GetMaximum(),hMC_sig.GetMaximum())*1.5, -0.1, 2.1, x_name[varname], f"a.u.", "ratio data/MC", square=CMS.kSquare, iPos=11, extraSpace=0, scaleLumi=None)

        dicanvas.SetCanvasSize(1200,900)
        dicanvas.cd(1)
        if logy:
            gPad.SetLogy()
        hMC_sig.SetLineColor(4)
        hMC_sig.SetFillColor(4)
        hMC_sig.SetLineWidth(2)
        hMC_sig.SetFillStyle(3004)
        if reweight:
            hMC_sig_wrw.SetLineColor(4)
            hMC_sig_wrw.SetFillColor(4)
            hMC_sig_wrw.SetLineWidth(2)
            hMC_sig_wrw.SetFillStyle(3004)
            hMC_sig.SetFillColor(0) 
            hMC_sig.SetLineWidth(2)
            hMC_sig.SetLineColor(17)
        hMC_sig.Draw("Hsame")
        if reweight:
            hMC_sig_wrw.Draw("Hsame")
        hdata_sig.SetLineColor(1)
        hdata_sig.SetLineWidth(2)
        hdata_sig.Draw("samePE1")

        if reweight:
            legend = TLegend(0.55, 0.63, 0.9, 0.9)
        else:
            legend = TLegend(0.60, 0.7, 0.9, 0.9)
        legend.AddEntry(hdata_sig, "sWeighted Data", "lep") 
        legend.AddEntry(hMC_sig, "MC B^{0}_{s} #rightarrow J/#psi(#mu#mu)#phi(KK)", "f")  
        if reweight:
            legend.AddEntry(hMC_sig_wrw, "reWeighted MC B^{0}_{s} #rightarrow J/#psi(#mu#mu)#phi(KK)", "f")  
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
        if reweight:
            h_x_ratio2 = hdata_sig.Clone()
            h_x_ratio2.Sumw2()
            h_x_ratio2.Divide(hMC_sig_wrw)
            h_x_ratio2.SetLineColor(1)
            h_x_ratio2.SetLineWidth(2)
            h_x_ratio.SetLineColor(17)
            h_x_ratio.SetMarkerColor(17)
            h_x_ratio.SetLineWidth(2)
        h_x_ratio.Draw("samePE1")
        if reweight:
            h_x_ratio2.Draw("samePE1")
        line1.Draw("same")

        dicanvas.Update()
        varname = varname.replace("*", "_")
        varname = varname.replace("/", "_")
        dicanvas.SaveAs(f"Control_Plots_{year}/" + varname + "_" + year + "_SPlot" + ("_rw" if reweight else "") + ".pdf")
        dicanvas.Clear()

        h_x_ratio.Delete();
        hdata_sig.Delete();
        hMC_sig.Delete(); 

if __name__ == "__main__": 
    parser = argparse.ArgumentParser(description="--plots for control plots")
    parser.add_argument("--file", type=str, help="file name")
    parser.add_argument("--year", type=str, help="year (2022 or 2023)")
    parser.add_argument("--reweight", action="store_true", help="apply reweighting")
    args = parser.parse_args()
    file = args.file
    year = args.year
    reweight = args.reweight
    if reweight:
        var.append("bdt")
    control_plots(file, year, reweight)
