from ROOT import gROOT, TH1F, TFile, gDirectory, RooFit, kFALSE, TLine, kRed, kDashed, gPad, TLegend
gROOT.SetBatch(True)
import cmsstyle as CMS

# Open the ROOT files
file1 = TFile.Open("../Utilities_Control/ROOTFiles_24_01_25/AllB2mu2K_sPlot_rw_bdt.root")
file2 = TFile.Open("../Utilities_organized/ROOTFiles_20_01_25/AllData_rw_bdt.root")
file3 = TFile.Open("../Utilities_organized/ROOTFiles_20_01_25/AllControl_rw_bdt.root")

# Get the TTrees from the files
tree1 = file1.Get("FinalTree")
tree2 = file2.Get("FinalTree")
tree3 = file3.Get("FinalTree")

# Fill the histograms
tree1.Draw("bdt>>MC_cont(50, 0, 1)", "nsigBs_sw*weight*bdt_reweight_0*bdt_reweight_1*bdt_reweight_2*(isMC>0)")
tree2.Draw("bdt_cv>>MC_sig(50, 0, 1)", "weight_pileUp*ctau_weight_central*bdt_reweight_0*bdt_reweight_1*bdt_reweight_2*(isMC>0 && vtx_prob>0.01)")
tree3.Draw("bdt>>MC_norm(50, 0, 1)", "weight_pileUp*ctau_weight_central*bdt_reweight_0*bdt_reweight_1*bdt_reweight_2*(isMC>0 && vtx_prob>0.01)")

MC_cont = TH1F(gDirectory.Get("MC_cont"))
MC_sig = TH1F(gDirectory.Get("MC_sig"))
MC_norm = TH1F(gDirectory.Get("MC_norm"))

MC_cont.Scale(1 / MC_cont.Integral(1,int(50)))
MC_sig.Scale(1 / MC_sig.Integral(1,int(50)))
MC_norm.Scale(1 / MC_norm.Integral(1,int(50)))

CMS.SetExtraText("Simulation Preliminary")
CMS.SetLumi(f"2022+2023, {62.4}")
CMS.SetEnergy(13.6)
dicanvas = CMS.cmsDiCanvas("", 0, 1, 0.0001, max(MC_cont.GetMaximum(),MC_sig.GetMaximum())*5, -0.1, 2.1, "bdt score", f"a.u.", "Signal/Control", square=CMS.kSquare, iPos=11, extraSpace=0, scaleLumi=None)
dicanvas.SetCanvasSize(1200,800)
dicanvas.cd(1)
gPad.SetLogy()

MC_sig.SetLineColor(9)       # Blu scuro
MC_sig.SetFillColor(9)
MC_sig.SetLineWidth(2)
MC_sig.SetFillStyle(3012)    # Diagonal hatch

MC_cont.SetLineColor(8)      # Verde scuro
MC_cont.SetFillColor(8)
MC_cont.SetLineWidth(2)
MC_cont.SetFillStyle(3013)   # Dotted hatch

MC_norm.SetLineColor(6)      # Viola chiaro
MC_norm.SetFillColor(6)
MC_norm.SetLineWidth(2)
MC_norm.SetFillStyle(3014)   # Cross hatch


MC_cont.Draw("Hsame")
MC_norm.Draw("Hsame")
MC_sig.Draw("Hsame")

legend = TLegend(0.71, 0.65, 0.9, 0.85)
legend.AddEntry(MC_cont, r"#it{B}_{s}^{0} #rightarrow J/#psi(#mu#mu) #phi(KK)", "f")
legend.AddEntry(MC_norm, r"#it{B}_{s}^{0} #rightarrow J/#psi(#mu#mu) #phi(#mu#mu)", "f")
legend.AddEntry(MC_sig, r"#it{B}_{(s)}^{0} #rightarrow 4#mu", "f")
legend.Draw("same")

dicanvas.cd(2)
line1 = TLine(0, 1, 1, 1) 
line2 = TLine(0, 0, 1, 0) 
line3 = TLine(0, 2, 1, 2) 
line1.SetLineColor(kRed)
line1.SetLineStyle(kDashed)
line1.SetLineWidth(2)
line2.SetLineColor(1)
line2.SetLineStyle(kDashed)
line2.SetLineWidth(2)
line3.SetLineColor(1)
line3.SetLineWidth(2)
line3.SetLineStyle(kDashed)

h_x_ratio = MC_sig.Clone()
h_x_ratio.Sumw2()
h_x_ratio.Divide(MC_cont)
h_x_ratio.SetLineColor(1)
h_x_ratio.SetLineWidth(2)

h_x_ratio.Draw("samePE1")
line1.Draw("same")

dicanvas.SaveAs("mc_ratio.pdf")
dicanvas.SaveAs("mc_ratio.png")

# Close the input files
file1.Close()
file2.Close()