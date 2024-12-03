import ROOT
from ROOT import RooFit, RooRealVar, RooDataHist, RooExponential, RooAddPdf, RooPlot
import os
import cmsstyle as CMS

def FitBsJPsiPhi_Mass(year="2022", label=""):
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
    h1 = ROOT.TH1F("h1", "Quadruplet Mass", 80, 4.8, 6.1)
    tree.Draw("Quadruplet_Mass_eq >> h1", "isMC==0")

    # Definire la variabile di massa
    x = RooRealVar("Quadruplet_Mass_eq", "Quadruplet Mass", 4.8, 6.1)
    x.setRange("R1", 5.0, 5.25)
    x.setRange("R2", 5.55, 5.8)
    x.setRange("RT", 4.8, 6.1)
    x.setBins(80)

    # Creare RooDataHist dal TH1F
    data = RooDataHist("data", h1.GetTitle(), ROOT.RooArgSet(x), RooFit.Import(h1, False))

    # Definire il modello di background esponenziale
    gamma = RooRealVar("#Gamma", "Gamma", -0.9, -10, 10)
    exp_bkg = RooExponential("exp_bkg", "Exponential Background", x, gamma)
    exp_bkg.fitTo(data, RooFit.Range("R1,R2"))

    # Definire il modello di segnale
    mu = RooRealVar("mu", "mu", 5.46, 5.1, 5.80)
    lambd = RooRealVar("lambd", "lambd", 1.0, 10.5)
    gamm = RooRealVar("gamm", "gamm", 1.0, 10.5)
    delta = RooRealVar("delta", "delta", 175, 100, 600)
    gauss_pdf = ROOT.RooJohnson("signal_Bs", "Signal Bs", x, mu, lambd, gamm, delta)

    # Definire i coefficienti di segnale e fondo
    nsig = RooRealVar("nsig", "Number of signals", 60, 30, 1000)
    nbkg = RooRealVar("nbkg", "Number of backgrounds", h1.GetEntries(), 1, 2 * h1.GetEntries())

    # Combinare segnale e background
    model = RooAddPdf("model", "Signal + Background", ROOT.RooArgList(gauss_pdf, exp_bkg), ROOT.RooArgList(nsig, nbkg))

    # Eseguire il fit
    result = model.fitTo(data, RooFit.Save(True), RooFit.Range("RT"))

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
    CMS.SetLumi("2022+2023, 62.4", unit="fb")
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
    file.Close()

if __name__=="__main__":
    FitBsJPsiPhi_Mass("", "05_10_24")