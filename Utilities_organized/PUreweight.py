from ROOT import TChain, gROOT, gDirectory, TFile, TCanvas, TH1F, kRed
gROOT.SetBatch(True)
import math, os, sys, subprocess, argparse
"""
subprocess.run(["mkdir", "PileUp"])
subprocess.run(['wget', '-O', 'PileUp/PU_MC2022.root', 'https://cms-service-dqmdc.web.cern.ch/CAF/certification/Collisions22/PileUp/BCDEFG/pileupHistogram-Cert_Collisions2022_355100_362760_GoldenJson-13p6TeV-69200ub-99bins.root'])

subprocess.run(['wget', '-O', 'PileUp/PU_MC2023.root', 'https://cms-service-dqmdc.web.cern.ch/CAF/certification/Collisions23/PileUp/BCD/pileupHistogram-Cert_Collisions2023_366442_370790_GoldenJson-13p6TeV-69200ub-99bins.root'])
"""

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
    chain1.Add("../Analysis/FinalFiles_B4mu_"+label+"/Analyzed_MC_Bd_4mu_"+year+".root")
    chain1.Add("../Analysis/FinalFiles_B4mu_"+label+"/Analyzed_MC_Bs_4mu_"+year+".root")

    chain2 = TChain("FinalTree")
    chain2.Add("../Analysis/FinalFiles_B4mu_"+label+"/Analyzed_MC_BsJPsiPhi_"+year+".root")

    print(f"nPileUpInt>>h_MC({n_bins},{x_min},{x_max})")
    
    chain1.Draw(f"nPileUpInt>>h_MC({n_bins},{x_min},{x_max})")
    hist_MC= gDirectory.Get("h_MC") 

    chain2.Draw(f"nPileUpInt>>h_MC2({n_bins},{x_min},{x_max})")
    hist_MC2= gDirectory.Get("h_MC2") 

    hist_Data.Scale(1/hist_Data.Integral())
    hist_MC.Scale(1/hist_MC.Integral())
    hist_MC2.Scale(1/hist_MC2.Integral())

    #c = TCanvas()
    #c.cd()
    #hist_Data.Draw("Histo")
    #hist_MC.Draw("Histo same")
    #hist_MC2.Draw("Histo same")
    #c.SaveAs("pipp.png")

    hist_ratio_signal = TH1F("pileUp_ratio_signal_"+year,"pileUp_ratio_signal_"+year, n_bins,x_min,x_max)
    hist_ratio_control = TH1F("pileUp_ratio_control_"+year,"pileUp_ratio_control_"+year, n_bins,x_min,x_max)
    hist_ratio_signal.Divide(hist_Data, hist_MC)
    hist_ratio_control.Divide(hist_Data, hist_MC2)
    
    fout = TFile.Open("PileUp/ratio_histo_"+year+".root","recreate")
    hist_ratio_signal.Write()
    hist_ratio_control.Write()
    fout.Close()
    
    #c2 = TCanvas()
    #c2.cd()
    #hist_ratio_signal.Draw("Histo")
    #hist_ratio_control.SetLineColor(kRed)
    #hist_ratio_control.Draw("Histo same")
    #c2.SaveAs("pipp2.png")
