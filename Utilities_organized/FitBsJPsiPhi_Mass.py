import ROOT
from ROOT import RooFit, RooRealVar, RooDataHist, RooExponential, RooAddPdf, RooPlot
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
    gauss_pdf.paramOn(frame2, RooFit.Parameters(ROOT.RooArgSet(mu, gamm, lambd, delta)), RooFit.Layout(0.6,0.9,0.9))
    c =  ROOT.TCanvas("canvas2", "Fit Result2", 900, 600)
    c.cd()
    frame2.Draw()
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
    # Applicare lo stile CMS
    CMS.SetExtraText("Preliminary")
    CMS.SetLumi("2022+2023+2024, 171.5", unit="fb")
    CMS.SetEnergy(13.6, unit='TeV')
    canv = CMS.cmsCanvas("",  5.05, 5.7, 0, 1.2*h1.GetMaximum() , "m_{J/#psi#phi}(GeV)", 'Entries', square=CMS.kSquare, extraSpace=0.01, iPos=0)
    canv.SetCanvasSize(1000,700)
    frame.Draw("same")
    
    legend = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)  
    #legend.SetBorderSize(0)  # Rimuove il bordo
    #legend.SetTextSize(0.03)  # Dimensione del testo

    legend.AddEntry(data, "Data", "p")
    legend.AddEntry(signal_curve, "Total", "l")  # "l" significa che l'oggetto è una linea
    legend.AddEntry(signal_curve, "B_{s} /rightarow J/#psi(#mu#mu)#phi(#mu#mu)", "l")  # "l" significa che l'oggetto è una linea
    legend.AddEntry(bkg_curve, "Background", "l")

    legend.Draw("same")

    CMS.SaveCanvas(canv, f"{dir_path}/Fit_BsJPsiPhi_{year}.png")
    #"""
    file.Close()
    del file 
    return nsig.getVal(), nsig.getError()

if __name__=="__main__":
    FitBsJPsiPhi_Mass("_bdt", "09_12_24", f"bdt>0.48")
    """
    cut = []
    nBs = []
    nBS_err = []
    nBS_ratio = []
    for i in range(25):
        val, err = FitBsJPsiPhi_Mass("_bdt", "09_12_24", f"bdt>{i/25}")
        nBs.append(val)
        nBS_err.append(err)
        nBS_ratio.append(val/err if err!=0 else 0)
        cut.append(i/25)
        del val
        del err

    hep.style.use("CMS")
    plt.figure(figsize=(8, 6))
    hep.cms.label("Preliminary", data=True, lumi=171.5, com=13.6, fontsize=18)
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