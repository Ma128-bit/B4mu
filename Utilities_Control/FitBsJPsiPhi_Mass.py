from ROOT import RooFit, RooRealVar, RooDataHist, RooExponential, RooAddPdf, TGraph, TH1F, gStyle
from ROOT import RooArgSet, TFile, RooJohnson, RooArgList, kDashed, kRed, kGreen, kBlack, TLegend, TGraphErrors, kBlue
import os
import cmsstyle as CMS

def FitBsJPsiPhi_Mass(year="2022", label=""):
    # Aprire il file ROOT contenente l'albero
    file_path = f"ROOTFiles_{label}/AllB2mu2K{year}.root"
    file = TFile.Open(file_path)
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
    h1 = TH1F("h1", "Quadruplet Mass", 80, 5.2, 5.7)
    tree.Draw("RefittedSV_Mass_eq >> h1", "isMC==0")

    # Definire la variabile di massa
    x = RooRealVar("RefittedSV_Mass_eq", "Quadruplet Mass", 5.2, 5.7)
    x.setRange("R1", 5.2, 5.25)
    x.setRange("R2", 5.55, 5.7)
    x.setRange("RT", 5.2, 5.7)
    x.setBins(80)

    # Creare RooDataHist dal TH1F
    data = RooDataHist("data", h1.GetTitle(), RooArgSet(x), RooFit.Import(h1, False))

    # Definire il modello di background esponenziale
    gamma = RooRealVar("#Gamma", "Gamma", -0.9, -10, 10)
    exp_bkg = RooExponential("exp_bkg", "Exponential Background", x, gamma)
    exp_bkg.fitTo(data, RooFit.Range("R1,R2"))

    # Definire il modello di segnale
    mu = RooRealVar("mu", "mu", 5.36, 4.50, 6.0)
    lambd = RooRealVar("lambd", "lambd", 0.02, 0.001, 1.5)
    gamm = RooRealVar("gamm", "gamm", 0.14, 0.01, 1.5)
    delta = RooRealVar("delta", "delta", 1.45, 0.1, 10)
    gauss_pdf = RooJohnson("signal_Bs", "Signal Bs", x, mu, lambd, gamm, delta)

    # Definire i coefficienti di segnale e fondo
    nsig = RooRealVar("nsig", "Number of signals", 12000, 0., 2000000)
    nbkg = RooRealVar("nbkg", "Number of backgrounds",  10000, 1., 1000000);

    # Combinare segnale e background
    model = RooAddPdf("model", "Signal + Background", RooArgList(gauss_pdf, exp_bkg), RooArgList(nsig, nbkg))

    # Eseguire il fit
    result = model.fitTo(data, RooFit.Save(True), RooFit.Range("RT"))

    # Creare il frame per il fit
    frame = x.frame()
    data.plotOn(frame)
    signal_curve = model.plotOn(frame, 
                            RooFit.Components("signal_Bs"), 
                            RooFit.LineStyle(kDashed), 
                            RooFit.LineColor(kRed), 
                            RooFit.LineWidth(4))  # Aumenta la larghezza della linea a 3

    bkg_curve = model.plotOn(frame, 
                             RooFit.Components("exp_bkg"), 
                             RooFit.LineStyle(kDashed), 
                             RooFit.LineColor(kGreen), 
                             RooFit.LineWidth(3))  # Aumenta la larghezza della linea a 3
    
    model.plotOn(frame, RooFit.LineWidth(4))

    print("*****", h1.GetMaximum())
    # Applicare lo stile CMS
    CMS.SetExtraText("Preliminary")
    CMS.SetLumi("2022+2023, 62.4", unit="fb")
    CMS.SetEnergy(13.6, unit='TeV')
    canv = CMS.cmsCanvas("",  5.2, 5.7, 0, 1.2*h1.GetMaximum() , "m_{J/#psi#phi}(GeV)", 'Entries', square=CMS.kSquare, extraSpace=0.05, iPos=11, yTitOffset=1.5)
    canv.SetCanvasSize(1000,800)
    frame.Draw("same")

    legend = TLegend(0.65, 0.65, 0.95, 0.9)
    legend.SetTextSize(0.03)
    #legend.SetBorderSize(0)  # Rimuove il bordo
    #legend.SetTextSize(0.03)  # Dimensione del testo
    dummy_graph = TGraphErrors(1)  # Create a dummy graph
    dummy_graph.SetMarkerStyle(20)  # Circle marker
    dummy_graph.SetMarkerSize(1.0)  # Size of the markers
    dummy_graph.SetLineColor(kBlack)  # Color of the line
    dummy_graph.SetMarkerColor(kBlack)  # Color of the marker
    dummy_graph.SetPoint(0, 1, 1)  # Dummy point for display
    dummy_graph.SetPointError(0, 0.1, 0.2)  # Error with caps
    legend.AddEntry(dummy_graph, "Data", "ep")  

    sum_pdf = TGraph()
    sum_pdf.SetLineColor(kBlue)
    sum_pdf.SetLineWidth(3)     
    legend.AddEntry(sum_pdf, "Total", "l")

    sig_pdf = TGraph()
    sig_pdf.SetLineColor(kRed)
    sig_pdf.SetLineWidth(3)
    sig_pdf.SetLineStyle(2)
    legend.AddEntry(sig_pdf, "B_{s} #rightarrow J/#psi(#mu#mu)#phi(KK)", "l")

    bkg_pdf = TGraph()
    bkg_pdf.SetLineColor(kGreen)
    bkg_pdf.SetLineWidth(3)
    bkg_pdf.SetLineStyle(2)
    legend.AddEntry(bkg_pdf, "Background", "l")

    legend.Draw("same")
    
    CMS.SaveCanvas(canv, f"{dir_path}/Fit_BsJPsiPhi_{year}.pdf")
    file.Close()

if __name__=="__main__":
    #FitBsJPsiPhi_Mass("_sPlot_rw_bdt", "24_01_25")
    FitBsJPsiPhi_Mass("2022", "24_01_25")
    
