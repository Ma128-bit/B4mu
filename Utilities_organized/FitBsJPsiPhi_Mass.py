import ROOT
from ROOT import RooFit, RooRealVar, RooDataHist, RooExponential, RooAddPdf, RooPlot, gROOT
gROOT.SetBatch(True)
import os
import cmsstyle as CMS
import matplotlib.pyplot as plt
import mplhep as hep
import numpy as np

def FitBsJPsiPhi_Mass(year="2022", label="", bdt_sel="bdt>0"):
    # Aprire il file ROOT contenente l'albero
    file_path = f"ROOTFiles_{label}/AllControl{year}.root"
    file = ROOT.TFile.Open(file_path)
    if not file or file.IsZombie():
        print("Error opening the file")
        return

    # Ottenere l'albero dal file
    tree = file.Get("FinalTree")
    if not tree:
        print("Error opening Tree")
        file.Close()
        return

    # Creare la directory
    label_str = str(label)
    dir_path = f"BsJPsiPhi_MassFit_{label_str}"
    os.makedirs(dir_path, exist_ok=True)

    # Disegnare l'istogramma dal tree
    h1 = ROOT.TH1F("h1", "Quadruplet Mass", 30, 5.05, 5.7)
    tree.Draw("NewMassEqation >> h1", "isMC==0 && "+bdt_sel)
    
    h2 = ROOT.TH1F("h2", "Quadruplet Mass", 80, 5.05, 5.7)
    tree.Draw("NewMassEqation >> h2", "isMC!=0 && "+bdt_sel)

    # Definire la variabile di massa
    x = RooRealVar("Quadruplet_Mass_eq", "Quadruplet Mass", 5.05, 5.7)
    x.setRange("R1", 5.05, 5.25)
    x.setRange("R2", 5.55, 5.7)
    x.setRange("RT", 5.05, 5.7)
    x.setRange("RS", 5.275, 5.45)
    #x.setBins(80)

    # Creare RooDataHist dal TH1F
    data = RooDataHist("data", h1.GetTitle(), ROOT.RooArgSet(x), RooFit.Import(h1, False))

    MC = RooDataHist("MC", "MC", ROOT.RooArgSet(x), RooFit.Import(h2, False))

    # Definire il modello di background esponenziale
    gamma = RooRealVar("#Gamma", "Gamma", -0.9, -10, 10)
    exp_bkg = RooExponential("exp_bkg", "Exponential Background", x, gamma)
    exp_bkg.fitTo(data, RooFit.Range("R1,R2"))

    # Definire il modello di segnale
    mu = RooRealVar("mu", "mu", 5.36, 4.50, 6.0)
    lambd = RooRealVar("lambd", "lambd", 0.02, 0.001, 1.5)
    gamm = RooRealVar("gamm", "gamm", 0.14, 0.01, 1.5)
    delta = RooRealVar("delta", "delta", 1.45, 0.1, 10)
    gauss_pdf = ROOT.RooJohnson("signal_Bs", "Signal Bs", x, mu, lambd, gamm, delta)
    gauss_pdf.fitTo(MC, RooFit.Save(True), RooFit.Range("RS"))

    frame2 = x.frame()
    MC.plotOn(frame2)
    gauss_pdf.plotOn(frame2, RooFit.LineStyle(ROOT.kDashed), RooFit.LineColor(ROOT.kRed))
    leg = ROOT.TLegend(0.65, 0.65, 0.95, 0.9)
    leg.SetTextSize(0.03)
    dummy = ROOT.TGraphErrors(1)  # Create a dummy graph
    dummy.SetMarkerStyle(20)  # Circle marker
    dummy.SetMarkerSize(1.0)  # Size of the markers
    dummy.SetLineColor(ROOT.kBlack)  # Color of the line
    dummy.SetMarkerColor(ROOT.kBlack)  # Color of the marker
    dummy.SetPoint(0, 1, 1)  # Dummy point for display
    dummy.SetPointError(0, 0.1, 0.2)  # Error with caps
    leg.AddEntry(dummy, "MC B^{0}_{s} #rightarrow J/#psi(#mu#mu)#phi(#mu#mu)", "ep")  
    pdf = ROOT.TGraph()
    pdf.SetLineColor(ROOT.kRed)
    pdf.SetLineWidth(3)
    pdf.SetLineStyle(2)
    leg.AddEntry(pdf, "Fit Function", "l")

    CMS.SetExtraText("Preliminary Simulation")
    CMS.SetLumi("2022+2023+2024, 170.7", unit="fb")
    CMS.SetEnergy(13.6, unit='TeV')
    #gauss_pdf.paramOn(frame2, RooFit.Parameters(ROOT.RooArgSet(mu, gamm, lambd, delta)), RooFit.Layout(0.6,0.9,0.9))
    c = CMS.cmsCanvas("",  5.1, 5.7, 0, 1.2*h2.GetMaximum() , "m_{J/#psi#phi}(GeV)", 'Entries', square=CMS.kSquare, extraSpace=0.02, iPos=11)
    c.SetCanvasSize(1000,750)
    c.cd()
    frame2.Draw("same")
    leg.Draw("same")
    c.SaveAs("BsJPsiPhi_MassFit_"+label+"/Fit_MC_"+year+".png")
    c.Delete()
    del c

    # Definire i coefficienti di segnale e fondo
    nsig = RooRealVar("nsig", "Number of signals", 60, 30, 1000)
    nbkg = RooRealVar("nbkg", "Number of backgrounds", h1.GetEntries(), 1, 2 * h1.GetEntries())

    # Combinare segnale e background
    lambd.setConstant(ROOT.kTRUE)
    delta.setConstant(ROOT.kTRUE) 
    gamm.setConstant(ROOT.kTRUE) 
    #mu.setConstant(ROOT.kTRUE) 
    model = RooAddPdf("model", "Signal + Background", ROOT.RooArgList(gauss_pdf, exp_bkg), ROOT.RooArgList(nsig, nbkg))

    # Eseguire il fit
    model.fitTo(data, RooFit.Save(True), RooFit.Range("RT"))
    #"""
    # Creare il frame per il fit
    frame = x.frame()
    data.plotOn(frame)
    signal_curve = model.plotOn(frame, 
                            RooFit.Components("signal_Bs"), 
                            RooFit.LineStyle(ROOT.kDashed), 
                            RooFit.LineColor(ROOT.kRed), 
                            RooFit.LineWidth(4))  # Aumenta la larghezza della linea a 3

    bkg_curve = model.plotOn(frame, 
                             RooFit.Components("exp_bkg"), 
                             RooFit.LineStyle(ROOT.kDashed), 
                             RooFit.LineColor(ROOT.kGreen), 
                             RooFit.LineWidth(3))  # Aumenta la larghezza della linea a 3
    
    model.plotOn(frame, RooFit.LineWidth(4))

    print("*****", h1.GetMaximum())
    CMS.SetExtraText("Preliminary")
    CMS.SetLumi("2022+2023+2024, 170.7", unit="fb")
    CMS.SetEnergy(13.6, unit='TeV')
    # Applicare lo stile CMS
    canv = CMS.cmsCanvas("",  5.1, 5.7, 0, 1.2*h1.GetMaximum() , "m_{J/#psi#phi}(GeV)", 'Entries', square=CMS.kSquare, extraSpace=0.02, iPos=11)
    canv.SetCanvasSize(1000,750)
    frame.Draw("same")
    
    legend = ROOT.TLegend(0.65, 0.65, 0.95, 0.9)
    legend.SetTextSize(0.03)
    #legend.SetBorderSize(0)  # Rimuove il bordo
    #legend.SetTextSize(0.03)  # Dimensione del testo
    dummy_graph = ROOT.TGraphErrors(1)  # Create a dummy graph
    dummy_graph.SetMarkerStyle(20)  # Circle marker
    dummy_graph.SetMarkerSize(1.0)  # Size of the markers
    dummy_graph.SetLineColor(ROOT.kBlack)  # Color of the line
    dummy_graph.SetMarkerColor(ROOT.kBlack)  # Color of the marker
    dummy_graph.SetPoint(0, 1, 1)  # Dummy point for display
    dummy_graph.SetPointError(0, 0.1, 0.2)  # Error with caps
    legend.AddEntry(dummy_graph, "Data", "ep")  

    sum_pdf = ROOT.TGraph()
    sum_pdf.SetLineColor(ROOT.kBlue)
    sum_pdf.SetLineWidth(3)     
    legend.AddEntry(sum_pdf, "Total", "l")

    sig_pdf = ROOT.TGraph()
    sig_pdf.SetLineColor(ROOT.kRed)
    sig_pdf.SetLineWidth(3)
    sig_pdf.SetLineStyle(2)
    legend.AddEntry(sig_pdf, "B^{0}_{s} #rightarrow J/#psi(#mu#mu)#phi(#mu#mu)", "l")

    bkg_pdf = ROOT.TGraph()
    bkg_pdf.SetLineColor(ROOT.kGreen)
    bkg_pdf.SetLineWidth(3)
    bkg_pdf.SetLineStyle(2)
    legend.AddEntry(bkg_pdf, "Background", "l")

    legend.Draw("same")

    CMS.SaveCanvas(canv, f"{dir_path}/Fit_BsJPsiPhi_{year}"+f"_{bdt_sel}.png")
    #"""
    file.Close()
    del file 
    return nsig.getVal(), nsig.getError()

if __name__=="__main__":
    #FitBsJPsiPhi_Mass("_rw_bdt", "20_01_25", f"year==2022")
    #nsig      = 60.3143      +/-  7.83868   (limited)
    #nsig      = 45.1128      +/-  6.94377   (limited)
    #nsig      = 137.573      +/-  12.3842   (limited)

    #nsig      = 65.1684      +/-  8.65423   (limited)
    #nsig      = 42.6421      +/-  7.46291   (limited)
    #nsig      = 139.717      +/-  13.4737   (limited)
    #FitBsJPsiPhi_Mass("_rw_bdt", "20_01_25", f"bdt>0.48")
    FitBsJPsiPhi_Mass("_rw_bdt", "20_01_25", f"Quadruplet_Mass_eq>0")
    # 2022 nsig      = 65.1684      +/-  8.65423   (limited)
    # 2023 nsig      = 42.6421      +/-  7.46291   (limited)
    # 2024 nsig      = 139.717      +/-  13.4737   (limited)
    # All nsig      = 257.681      +/-  18.0491   (limited)
    """
    cut = []
    nBs = []
    nBS_err = []
    nBS_ratio = []
    for i in range(25):
        val, err = FitBsJPsiPhi_Mass("_rw_bdt", "20_01_25", f"bdt>{i/25}")
        nBs.append(val)
        nBS_err.append(err)
        nBS_ratio.append(val/err if err!=0 else 0)
        cut.append(i/25)
        del val
        del err

    hep.style.use("CMS")
    plt.figure(figsize=(8, 6))
    hep.cms.label("Preliminary", data=True, lumi=170.7, com=13.6, fontsize=18)
    max_y = np.max(nBS_ratio)
    max_x = cut[np.argmax(nBS_ratio)]
    print(max_x)
    print(nBs[np.argmax(nBS_ratio)])
    print(nBS_err[np.argmax(nBS_ratio)])
    plt.scatter(cut, nBS_ratio, color='blue', s=50)
    plt.scatter(max_x, max_y, color='red', label=f'Max ({max_x}, {max_y})', s=100, edgecolor='red')
    plt.xlabel('BDT cut')
    plt.ylabel(r'$S/\sigma$')
    plt.tight_layout()
    plt.savefig("scatter_plot.png", dpi=300)
    """
    
    