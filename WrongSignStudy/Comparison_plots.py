from ROOT import gROOT, TH1F, TChain, gDirectory, RooFit, kFALSE, TLine, kRed, kDashed, gPad, TLegend
gROOT.SetBatch(True)
import sys, os, subprocess, argparse
import cmsstyle as CMS

var = ["vtx_prob", "mu1_pfreliso03", "mu2_pfreliso03", "FlightDistBS_SV_Significance", "mu1_bs_dxy_sig", "mu2_bs_dxy_sig", "mu3_bs_dxy_sig", "mu4_bs_dxy_sig", "Cos2d_BS_SV", "Quadruplet_Eta","Quadruplet_Pt", "Mu1_Eta", "Mu1_Pt", "RefittedSV_Mass_reso", "bdt", "bdt_cv"]

binning_dict = {
    "vtx_prob": "(50,0.01,1.0)",
    "vtx_prob_2mu": "(50,0.,1.0)",
    "vtx_prob_2K": "(50,0.,1.0)",
    "mu1_pfreliso03": "(50,0,10)",
    "mu2_pfreliso03": "(50,0,10)",
    "MVASoft1": "(50,0.2,0.8)",
    "MVASoft2": "(50,0.2,0.8)",
    "FlightDistBS_SV_Significance": "(50,0,100)",
    "RefittedSV_Mass_reso": "(50,0.01,0.08)",
    "mu1_bs_dxy_sig": "(50,-50,50)",
    "mu2_bs_dxy_sig": "(50,-50,50)",
    "mu3_bs_dxy_sig": "(50,-50,50)",
    "mu4_bs_dxy_sig": "(50,-50,50)",
    "Cos2d_BS_SV": "(50,0.95,1)",
    "Quadruplet_Eta": "(50,-2.5,2.5)",
    "RefittedSV_Mass_eq": "(50,5.25,5.5)",
    "RefittedSV_Mass": "(50,5.2,5.6)",
    "Quadruplet_Pt": "(50,10,100)",
    "bdt": "(50,0.,1)",
    "bdt_cv": "(50,0.,1)",
    "100*new_ct/2.998": "(50,0,14)",
    "Mu1_Eta": "(50,-2.5,2.5)",
    "Mu1_Pt": "(50,4, 50)",
    "Mu2_Eta": "(50,-2.5,2.5)",
    "Mu2_Pt": "(50,4, 50)",
    "Mu3_Eta": "(50,-2.5,2.5)",
    "Mu3_Pt": "(50,4, 50)",
    "Mu4_Eta": "(50,-2.5,2.5)",
    "Mu4_Pt": "(50,4, 50)",
    "PVCollection_Size": "(70,0,70)",
    "category": "(10,-1,5)"
}

log_dict = {
    "vtx_prob": False,
    "vtx_prob_2mu": False,
    "vtx_prob_2K": False,
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
    "bdt_cv": True,
    "100*new_ct/2.998": True,
    "Mu1_Eta": False,
    "Mu1_Pt": False,
    "Mu2_Eta": False,
    "Mu2_Pt": False,
    "Mu3_Eta": False,
    "Mu3_Pt": False,
    "Mu4_Eta": False,
    "Mu4_Pt": False,
    "PVCollection_Size": False,
    "category": False
}

x_name = {
    "vtx_prob": "Vertex Probability",
    "vtx_prob_2mu": "Vertex Probability 2mu",
    "vtx_prob_2K": "Vertex Probability 2K",
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
    "bdt_cv": "BDT score",
    "100*new_ct/2.998": "",
    "Mu1_Eta": "#mu_{1} |#eta|",
    "Mu1_Pt": "#mu_{1} p_{T}",
    "Mu2_Eta": "#mu_{2} |#eta|",
    "Mu2_Pt": "#mu_{2} p_{T}",
    "Mu3_Eta": "K_{1} |#eta|",
    "Mu3_Pt": "K_{1} p_{T}",
    "Mu4_Eta": "K_{2} |#eta|",
    "Mu4_Pt": "K_{2} p_{T}",
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

def Comparison_plots(rs, ws, year="2022+2023+2024"):
    if not os.path.exists(f"Comparison_Plots_{year}"):
        subprocess.run(["mkdir", f"Comparison_Plots_{year}"])
    
    for k in range(len(var)):
        varname = var[k]
        logy = log_dict[varname]
        s = str(k)
        binning = binning_dict[varname]
        numbers = binning.strip("()").split(",")
        numbers = [float(x) if x.isdigit() else float(x) for x in numbers]

        legend_label = "sWeighted"
        rs.Draw(varname + ">>hrs" + s+ binning, "(isMC==0 && (RefittedSV_Mass>5.566 || RefittedSV_Mass<5.079))")
        hrs = TH1F(gDirectory.Get("hrs" + s))
        ws.Draw(varname + ">>hws" + s + binning, "weight*(isMC==0)")
        hws = TH1F(gDirectory.Get("hws" + s))

        # Rescaling
        hrs.Scale(1 / hrs.Integral(1,int(numbers[0])))
        hws.Scale(1 / hws.Integral(1,int(numbers[0])))

        
        CMS.SetLumi(f"{year}, {lumi[year]}")
        CMS.SetEnergy(13.6)
        if logy:
            dicanvas = CMS.cmsDiCanvas("", numbers[1], numbers[2], 0.0001, max(hws.GetMaximum(),hrs.GetMaximum())*5, -0.1, 2.1, x_name[varname], f"a.u.", "ratio data/MC", square=CMS.kSquare, iPos=11, extraSpace=0, scaleLumi=None)
        else:
            if varname!="Quadruplet_Eta":
                dicanvas = CMS.cmsDiCanvas("", numbers[1], numbers[2], 0, max(hws.GetMaximum(),hrs.GetMaximum())*1.5, -0.1, 2.1, x_name[varname], f"a.u.", "ratio data/MC", square=CMS.kSquare, iPos=11, extraSpace=0, scaleLumi=None)
            else:
                dicanvas = CMS.cmsDiCanvas("", numbers[1], numbers[2], 0, max(hws.GetMaximum(),hrs.GetMaximum())*1.5, -0.1, 2.1, x_name[varname], f"a.u.", "ratio data/MC", square=CMS.kSquare, iPos=11, extraSpace=0, scaleLumi=None)

        dicanvas.SetCanvasSize(1200,900)
        dicanvas.cd(1)
        if logy:
            gPad.SetLogy()
        hrs.SetLineColor(4)
        hrs.SetFillColor(4)
        hrs.SetLineWidth(2)
        hrs.SetFillStyle(3004)
        hrs.Draw("Hsame")
        hws.SetLineColor(1)
        hws.SetLineWidth(2)
        hws.Draw("samePE1")

        legend = TLegend(0.60, 0.7, 0.9, 0.9)
        legend.AddEntry(hws, "WS Loose ID", "lep") 
        legend.AddEntry(hrs, "RS Sidebands", "f")  
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

        h_x_ratio = hws.Clone()
        h_x_ratio.Sumw2()
        h_x_ratio.Divide(hrs)
        h_x_ratio.SetLineColor(1)
        h_x_ratio.SetLineWidth(2)
        h_x_ratio.Draw("samePE1")
        line1.Draw("same")

        dicanvas.Update()
        varname = varname.replace("*", "_")
        varname = varname.replace("/", "_")
        dicanvas.SaveAs(f"Comparison_Plots_{year}/" + varname + "_" + year + ".png")
        dicanvas.Clear()

        h_x_ratio.Delete();
        hws.Delete();
        hrs.Delete(); 

if __name__ == "__main__": 
    rs_MC = "/lustrehome/mbuonsante/B_4mu/CMSSW_13_0_13/src/Utilities_organized/ROOTFiles_20_01_25/AllData_rw_bdt_v0.root"
    ws_MC = "/lustrehome/mbuonsante/B_4mu/CMSSW_13_0_13/src/Analysis/FinalFiles_B4mu_09_12_25/AllData_rw_bdt.root"
    #rs_MC = "/lustrehome/mbuonsante/B_4mu/CMSSW_13_0_13/src/WrongSignStudy/BDT_results/_20251216-112832/TrainedDataset_20251216-112832.root"
    #ws_MC = "/lustrehome/mbuonsante/B_4mu/CMSSW_13_0_13/src/WrongSignStudy/BDT_results/_20251216-112832/TrainedDataset_20251216-112832.root"

    parser = argparse.ArgumentParser(description="--plots for Comparison plots")
    
    rs = TChain("FinalTree")
    rs.Add(rs_MC)

    ws = TChain("FinalTree")
    ws.Add(ws_MC)

    Comparison_plots(rs, ws)
