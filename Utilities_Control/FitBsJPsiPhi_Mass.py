from ROOT import RooFit, RooRealVar, RooDataHist, RooExponential, RooAddPdf, TGraph, TH1F, gStyle, RooGenericPdf, TPaveText, RooGaussian
from ROOT import RooArgSet, TFile, RooJohnson, RooArgList, kDashed, kRed, kGreen, kBlack, TLegend, TGraphErrors, kBlue, TLine, kYellow, kGray
import os
import cmsstyle as CMS
import random

def fit_with_random_starts(pdf, data, x, n_trials=5):
    """
    Esegue fit multipli di un PDF RooFit con parametri iniziali randomici.
    Restituisce il miglior RooFitResult e i parametri ottimizzati.
    
    Args:
        pdf: RooAbsPdf, il PDF da fitare
        data: RooAbsData, il dataset
        x: RooRealVar, variabile osservabile
        n_trials: numero di fit da provare
        fit_range: intervallo del fit (stringa)
    """
    best_chi2 = float('inf')
    best_result = None
    best_params = {}

    # Otteniamo i parametri liberi del pdf
    params = pdf.getParameters(data)
    
    # Creiamo una lista di RooRealVar
    param_list = [p for p in params if p.isConstant() == False]
    param_list = [p for p in param_list if (p.GetName() != "nbkg" and p.GetName() != "nsig")]
    
    result = pdf.fitTo(data,
                           RooFit.Save(True),
                           RooFit.Strategy(2),
                           RooFit.Extended(True),
                           RooFit.Verbose(False),
                           RooFit.PrintLevel(-1),
                           RooFit.NumCPU(4),
                           RooFit.Hesse(True),
                           RooFit.Minimizer("Minuit2", "migrad"))
    
    frametmp = x.frame()
    data.plotOn(frametmp)
    pdf.plotOn(frametmp, RooFit.LineWidth(4))
    chi2 = frametmp.chiSquare()

    if chi2 < best_chi2:
        best_chi2 = chi2
        best_result = result
        best_params = {p.GetName(): p.getVal() for p in param_list}

    for i in range(n_trials):
        # Imposta valori iniziali casuali
        for p in param_list:
            low, high = p.getMin(), p.getMax()
            val = random.uniform(low, high)
            p.setVal(val)
        
        # Fit
        result = pdf.fitTo(data,
                           RooFit.Save(True),
                           RooFit.Strategy(2),
                           RooFit.Extended(True),
                           RooFit.Verbose(False),
                           RooFit.PrintLevel(-1),
                           RooFit.NumCPU(4),
                           RooFit.Hesse(True),
                           RooFit.Minimizer("Minuit2", "migrad"))
        
        EPS = 1e-3
        at_limit = any(
            p.getVal() <= p.getMin() + EPS or
            p.getVal() >= p.getMax() - EPS
            for p in param_list
        )        
        if at_limit:
            continue  # scarta questo fit

        # Calcoliamo un "chi2 proxy"
        frametmp = x.frame()
        data.plotOn(frametmp)
        pdf.plotOn(frametmp, RooFit.LineWidth(4))
        chi2 = frametmp.chiSquare()
        if chi2 < best_chi2:
            best_chi2 = chi2
            best_result = result
            best_params = {p.GetName(): p.getVal() for p in param_list}

    if best_params == {}:
        print("NO OPTIMAL PARAMETERS")
        return False
    # Imposta i parametri migliori al pdf
    print("OPTIMAL PARAMETERS:")
    for p in param_list:
        print(p.GetName(),":",best_params[p.GetName()])
        p.setVal(best_params[p.GetName()])
    
    # Rifit finale con parametri ottimizzati
    final_result = pdf.fitTo(data,
                             RooFit.Save(True),
                             RooFit.Strategy(2),
                             RooFit.Extended(True),
                             RooFit.Verbose(False),
                             RooFit.PrintLevel(-1),
                             RooFit.NumCPU(4),
                             RooFit.Hesse(True),
                             RooFit.Minimizer("Minuit2", "migrad"))
    if best_chi2 == float('inf'):
        return False
    return True

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
    h1 = TH1F("h1", "Quadruplet Mass", 120, 5.25, 5.65)
    tree.Draw("RefittedSV_Mass_eq >> h1", "isMC==0 && vtx_prob>0.01")

    # Definire la variabile di massa
    x = RooRealVar("RefittedSV_Mass_eq", "Quadruplet Mass", 5.25, 5.65)
    x.setRange("R1", 5.25, 5.25)
    x.setRange("R2", 5.55, 5.65)
    x.setRange("RT", 5.25, 5.65)
    x.setBins(120)

    # Creare RooDataHist dal TH1F
    data = RooDataHist("data", h1.GetTitle(), RooArgSet(x), RooFit.Import(h1, False))

    # Definire il modello di background esponenziale
    gamma = RooRealVar("#Gamma", "Gamma", -0.9, -10, 10)
    exp_bkg = RooExponential("exp_bkg", "Exponential Background", x, gamma)
    exp_bkg.fitTo(data, RooFit.Range("R1,R2"))

    # Definire il modello di segnale
    mu = RooRealVar("mu", "mu", 5.366, 5.20, 5.5)
    lambd = RooRealVar("lambd", "lambd", 0.021, 0.005, 0.1)
    gamm = RooRealVar("gamm", "gamm",  -0.112, -10.5, 10.5)
    delta = RooRealVar("delta", "delta", 1.18, 0.01, 10)
    johnson = RooJohnson("johnson", "johnson", x, mu, lambd, gamm, delta)
    frac = RooRealVar("frac", "frac", 0.16, 0.1, 0.3)
    gauss = RooGaussian("gauss", "gauss", x, mu, lambd)

    gauss_pdf = RooAddPdf("signal_Bs", "Signal Bs", RooArgList(gauss, johnson), RooArgList(frac))

    # Definire i coefficienti di segnale e fondo
    nsig = RooRealVar("nsig", "Number of signals", 170000, 0., 2000000)
    nbkg = RooRealVar("nbkg", "Number of backgrounds",  100000, 1., 1000000);

    # Combinare segnale e background
    model = RooAddPdf("model", "Signal + Background", RooArgList(gauss_pdf, exp_bkg), RooArgList(nsig, nbkg))

    # Eseguire il fit
    if not fit_with_random_starts(model, data, x, n_trials=50):
        print("NO BEST FIT")
        return None

    #result = model.fitTo(data, RooFit.Save(True), RooFit.Range("RT"))

    # Creare il frame per il fit
    frame = x.frame()
    data.plotOn(frame)

    """
    model.plotOn(frame, 
                        RooFit.Components("johnson"), 
                        RooFit.LineStyle(kDashed), 
                        RooFit.LineColor(kYellow), 
                        RooFit.LineWidth(4))  # Aumenta la larghezza della linea a 3

    model.plotOn(frame, 
                        RooFit.Components("studentT"), 
                        RooFit.LineStyle(kDashed), 
                        RooFit.LineColor(kGray), 
                        RooFit.LineWidth(4))  # Aumenta la larghezza della linea a 3
    """
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

    pull_hist = frame.pullHist()
    pull_frame = x.frame()
    pull_frame.addPlotable(pull_hist, "P")

    params = gauss_pdf.getParameters(data)
    nFitParams = sum(1 for p in params if not p.isConstant())
    ndf = frame.GetNbinsX() - nFitParams
    chi2 = frame.chiSquare()

    print("*****", h1.GetMaximum())
    # Applicare lo stile CMS
    CMS.SetExtraText("Preliminary")
    CMS.SetLumi("2022+2023+2024, 170.7", unit="fb")
    CMS.SetEnergy(13.6, unit='TeV')
    #canv = CMS.cmsCanvas("",  5.2, 5.7, 0, 1.2*h1.GetMaximum() , "m_{J/#psi#phi}(GeV)", 'Entries', square=CMS.kSquare, extraSpace=0.05, iPos=11, yTitOffset=1.5)
    canv = CMS.cmsDiCanvas("",  5.25, 5.65, 0, 1.2*h1.GetMaximum() , -5, 5, "m_{J/#psi#phi}(GeV)", 'Entries', "Pull", square=CMS.kSquare, extraSpace=0.05, iPos=11)
    canv.cd(1)
    canv.SetCanvasSize(1000,750)
    frame.Draw("same")

    chi2_label = TPaveText(0.6, 0.4, 0.9, 0.55, "NDC")  # NDC = Normalized Device Coordinates
    chi2_label.SetFillColor(0)
    chi2_label.SetTextAlign(12)
    chi2_label.SetTextFont(42)
    chi2_label.SetTextSize(0.05)
    chi2_label.AddText(f"#chi^{{2}} = {ndf*chi2:.2f}")
    chi2_label.AddText(f"ndf = {ndf:.2f}")
    chi2_label.AddText(f"#chi^{{2}}/ndf = {chi2:.2f}")
    chi2_label.Draw("same")

    legend = TLegend(0.6, 0.6, 0.9, 0.85)
    legend.SetTextSize(0.05)
    #legend.SetBorderSize(0)  # Rimuove il bordo
    #legend.SetTextSize(0.03)  # Dimensione del testo
    dummy_graph = TGraphErrors()  # Create a dummy graph
    dummy_graph.SetPoint(0, 0.0, 0.0)
    dummy_graph.SetPointError(0, 1.0, 1.0)
    dummy_graph.SetMarkerStyle(20)  # Circle marker
    dummy_graph.SetMarkerSize(1.0)  # Size of the markers
    dummy_graph.SetLineColor(kBlack)  # Color of the line
    dummy_graph.SetMarkerColor(kBlack)  # Color of the marker
    dummy_graph.SetPoint(0, 1, 1)  # Dummy point for display
    dummy_graph.SetPointError(0, 0.1, 0.2)  # Error with caps
    legend.AddEntry(dummy_graph, "Data", "PE")  

    sum_pdf = TGraph()
    sum_pdf.SetLineColor(kBlue)
    sum_pdf.SetLineWidth(3)     
    legend.AddEntry(sum_pdf, "Total", "l")

    sig_pdf = TGraph()
    sig_pdf.SetLineColor(kRed)
    sig_pdf.SetLineWidth(3)
    sig_pdf.SetLineStyle(2)
    legend.AddEntry(sig_pdf, "B_{s}^{0} #rightarrow J/#psi(#mu#mu)#phi(KK)", "l")

    bkg_pdf = TGraph()
    bkg_pdf.SetLineColor(kGreen)
    bkg_pdf.SetLineWidth(3)
    bkg_pdf.SetLineStyle(2)
    legend.AddEntry(bkg_pdf, "Background", "l")

    legend.Draw("same")

    canv.cd(2)  # Lower pad
    pull_frame.Draw("P same")
    zero_line2 = TLine(5.25, 0, 5.65, 0)
    zero_line2.SetLineStyle(kDashed)
    zero_line2.SetLineColor(kRed)
    zero_line2.SetLineWidth(2)
    zero_line2.Draw("same")
    
    CMS.SaveCanvas(canv, f"{dir_path}/Fit_BsJPsiPhi_{year}.pdf")
    file.Close()

if __name__=="__main__":
    FitBsJPsiPhi_Mass("_sPlot_rw_bdt", "18_11_25")
    #FitBsJPsiPhi_Mass("2022", "18_11_25")
    
