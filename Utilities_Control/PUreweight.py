from ROOT import TChain, gROOT, gDirectory, TFile, TCanvas, TH1F, kRed, kBlue, TLegend, kGreen
gROOT.SetBatch(True)
import math, os, sys, subprocess, argparse
"""
subprocess.run(["mkdir", "PileUp"])
"""
# PU_MC202*.root are on my eos

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="")
    parser.add_argument("--year", type=str, required=True, help="2022 or 2023")
    parser.add_argument("--label", type=str, required=True, help="")
    args = parser.parse_args()
    year = args.year
    label = args.label
    
    file = TFile.Open("PileUp/PU_MC"+year+".root")
    hist_Data = file.Get("pileup")
    n_bins = hist_Data.GetNbinsX()
    x_min = hist_Data.GetXaxis().GetXmin()
    x_max = hist_Data.GetXaxis().GetXmax()
    
    chain1 = TChain("FinalTree")
    
    if year == "2024":
        chain1.Add("../../../CMSSW_14_0_18_patch1/src/Analysis/FinalFiles_B2mu2K_"+label+"/Analyzed_MC_B2mu2K_"+year+".root")
    else:
        chain1.Add("../Analysis/FinalFiles_B2mu2K_"+label+"/Analyzed_MC_B2mu2K_"+year+".root")

    #chain2 = TChain("FinalTree")
    #chain2.Add("../Analysis/FinalFiles_B4mu_"+label+"/Analyzed_MC_BsJPsiPhi_"+year+".root")

    print(f"nPileUpInt>>h_MC({n_bins},{x_min},{x_max})")
    
    chain1.Draw(f"nPileUpInt>>h_MC({n_bins},{x_min},{x_max})")
    hist_MC= gDirectory.Get("h_MC") 

    #chain2.Draw(f"nPileUpInt>>h_MC2({n_bins},{x_min},{x_max})")
    #hist_MC2= gDirectory.Get("h_MC2") 

    hist_Data.Scale(1/hist_Data.Integral())
    hist_MC.Scale(1/hist_MC.Integral())
    #hist_MC2.Scale(1/hist_MC2.Integral())

    hist_ratio_signal = TH1F("pileUp_ratio_B2mu2K_"+year,"pileUp_ratio_B2mu2K_"+year, n_bins,x_min,x_max)
    #hist_ratio_control = TH1F("pileUp_ratio_control_"+year,"pileUp_ratio_control_"+year, n_bins,x_min,x_max)
    hist_ratio_signal.Divide(hist_Data, hist_MC)
    #hist_ratio_control.Divide(hist_Data, hist_MC2)

    c = TCanvas()
    c.cd()
    hist_Data.Draw("Histo")
    hist_MC.Draw("Histo same")
    hist_Data.SetLineColor(kRed)    
    hist_MC.SetLineColor(kBlue)  
    hist_MC_rw = hist_MC.Clone("hist_MC_rw")  # Clona hist1 in un nuovo istogramma
    hist_MC_rw.Multiply(hist_ratio_signal)
    #hist_MC_rw.Scale(1/hist_MC_rw.Integral())
    hist_MC_rw.SetLineColor(kGreen)  
    hist_MC_rw.Draw("Histo same")
    legend = TLegend(0.7, 0.7, 0.9, 0.9) 
    legend.AddEntry(hist_Data, "Data", "l")  
    legend.AddEntry(hist_MC, "MC", "l") 
    legend.AddEntry(hist_MC_rw, "MC rw", "l") 
    legend.Draw()
    #hist_MC2.Draw("Histo same")
    c.SaveAs("PileUp/PUDist"+year+"_"+label+".png")
    
    fout = TFile.Open("PileUp/ratio_histo_"+year+"_"+label+".root","recreate")
    hist_ratio_signal.Write()
    #hist_ratio_control.Write()
    fout.Close()
    
    #c2 = TCanvas()
    #c2.cd()
    #hist_ratio_signal.Draw("Histo")
    #hist_ratio_control.SetLineColor(kRed)
    #hist_ratio_control.Draw("Histo same")
    #c2.SaveAs("pipp2.png")
