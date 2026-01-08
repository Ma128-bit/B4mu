import ROOT
from ROOT import RooFit, RooRealVar, RooDataHist, RooExponential, RooAddPdf, RooPlot, gROOT, RooArgList
gROOT.SetBatch(True)
import os
import cmsstyle as CMS
import matplotlib.pyplot as plt
import mplhep as hep
import numpy as np
import random

def fit_with_random_starts(pdf, data, x, n_trials=5, fit_range="RS"):
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
    
    result = pdf.fitTo(data,
                           RooFit.Save(True),
                           RooFit.Range(fit_range),
                           RooFit.Strategy(2),
                           RooFit.Extended(False),
                           RooFit.Verbose(False),
                           RooFit.PrintLevel(-1),
                           RooFit.NumCPU(4),
                           RooFit.Hesse(True),
                           RooFit.Minimizer("Minuit2", "migrad"))
    
    frametmp = x.frame()
    data.plotOn(frametmp)
    pdf.plotOn(frametmp, RooFit.LineStyle(ROOT.kDashed), RooFit.LineColor(ROOT.kRed))
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
                           RooFit.Range(fit_range),
                           RooFit.Strategy(2),
                           RooFit.Extended(False),
                           RooFit.Verbose(False),
                           RooFit.PrintLevel(-1),
                           RooFit.NumCPU(4),
                           RooFit.Hesse(True),
                           RooFit.Minimizer("Minuit2", "migrad"))
        
        at_limit = any(p.getVal() <= p.getMin() or p.getVal() >= p.getMax() for p in param_list)
        if at_limit:
            continue  # scarta questo fit

        frametmp = x.frame()
        data.plotOn(frametmp)
        pdf.plotOn(frametmp, RooFit.LineStyle(ROOT.kDashed), RooFit.LineColor(ROOT.kRed))
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
                             RooFit.Range(fit_range),
                             RooFit.Strategy(2),
                             RooFit.Extended(False),
                             RooFit.Verbose(False),
                             RooFit.PrintLevel(-1),
                             RooFit.NumCPU(4),
                             RooFit.Hesse(True),
                             RooFit.Minimizer("Minuit2", "migrad"))
    if best_chi2 == float('inf'):
        return False
    return True

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
    
    h2 = ROOT.TH1F("h2", "Quadruplet Mass", 160, 5.05, 5.7)
    tree.Draw("NewMassEqation >> h2", "weight_pileUp*ctau_weight_central*bdt_reweight_0*bdt_reweight_1*bdt_reweight_2*(isMC!=0 && "+bdt_sel+")")

    # Definire la variabile di massa
    x = RooRealVar("Quadruplet_Mass_eq", "Quadruplet Mass", 5.05, 5.7)
    x.setRange("R1", 5.05, 5.25)
    x.setRange("R2", 5.55, 5.7)
    x.setRange("RT", 5.05, 5.7)
    x.setRange("RS", 5.29, 5.45)
    #x.setBins(80)

    # Creare RooDataHist dal TH1F
    data = RooDataHist("data", h1.GetTitle(), ROOT.RooArgSet(x), RooFit.Import(h1, False))

    MC = RooDataHist("MC", "MC", ROOT.RooArgSet(x), RooFit.Import(h2, False))

    # Definire il modello di background esponenziale
    gamma = RooRealVar("#Gamma", "Gamma", -0.9, -10, 10)
    exp_bkg = RooExponential("exp_bkg", "Exponential Background", x, gamma)
    exp_bkg.fitTo(data, RooFit.Range("R1,R2"))

    # Definire il modello di segnale
    mu = RooRealVar("mu", "mu", 5.366, 5.20, 5.5)
    lambd = RooRealVar("lambd", "lambd", 0.02, 0.005, 0.1)
    gamm = RooRealVar("gamm", "gamm",  0.19, -10.5, 10.5)
    delta = RooRealVar("delta", "delta", 1.364, 0.001, 10)
    johnson = ROOT.RooJohnson("johnson", "johnson", x, mu, lambd, gamm, delta)
    frac = RooRealVar("frac", "frac", 0.20, 0.01, 0.99)
    gauss = ROOT.RooGaussian("gauss", "gauss", x, mu, lambd)

    gauss_pdf = ROOT.RooAddPdf("signal_Bs", "Signal Bs", RooArgList(gauss, johnson), RooArgList(frac))

    """
    gauss_pdf.fitTo(MC,
                             RooFit.Save(True),
                             RooFit.Range("RS"),
                             RooFit.Strategy(2),
                             RooFit.Extended(False),
                             RooFit.Verbose(False),
                             RooFit.PrintLevel(-1),
                             RooFit.NumCPU(4),
                             RooFit.Hesse(True),
                             RooFit.Minimizer("Minuit2", "migrad"))
    """
    if not fit_with_random_starts(gauss_pdf, MC, x, n_trials=20):
        print("NO BEST FIT")
        return None, None
    
    vars = [p for p in gauss_pdf.getParameters(MC)]

    print("=== RooRealVar values ===")
    for v in vars:
        print(f"{v.GetName():10s} = {v.getVal():.6f}  "
            f"[{v.getMin():.3f}, {v.getMax():.3f}]")

    frame2 = x.frame()
    MC.plotOn(frame2)
    gauss_pdf.plotOn(frame2, RooFit.LineStyle(ROOT.kDashed), RooFit.LineColor(ROOT.kRed))
    #Pull:
    pull_hist_mc = frame2.pullHist()
    pull_frame_mc = x.frame()
    pull_frame_mc.addPlotable(pull_hist_mc, "P")

    # Calcola il chi²/ndf (ROOT calcola ndf internamente)
    params = gauss_pdf.getParameters(MC)
    nFitParams = sum(1 for p in params if not p.isConstant())
    ndf = frame2.GetNbinsX() - nFitParams
    chi2 = frame2.chiSquare(nFitParams)

    leg = ROOT.TLegend(0.6, 0.6, 0.9, 0.85)
    leg.SetTextSize(0.05)
    leg.SetBorderSize(0)  # Rimuove il bordo
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
    c = CMS.cmsDiCanvas("",  5.23, 5.5, 0, 1.2*h2.GetMaximum(), -10, 10, "m_{J/#psi#phi}(GeV)", 'Entries', "Pull", square=CMS.kSquare, extraSpace=0.02, iPos=11)
    c.SetCanvasSize(1000,750)
    c.cd(1)
    frame2.Draw("same")
    leg.Draw("same")

    # Crea un box di testo
    chi2_labelMC = ROOT.TPaveText(0.65, 0.4, 0.95, 0.55, "NDC")  # NDC = Normalized Device Coordinates
    chi2_labelMC.SetFillColor(0)
    chi2_labelMC.SetTextAlign(12)
    chi2_labelMC.SetTextFont(42)
    chi2_labelMC.SetTextSize(0.05)
    chi2_labelMC.AddText(f"#chi^{{2}} = {ndf*chi2:.2f}")
    chi2_labelMC.AddText(f"ndf = {ndf:.2f}")
    chi2_labelMC.AddText(f"#chi^{{2}}/ndf = {chi2:.2f}")
    chi2_labelMC.Draw("same")

    c.cd(2)  # Lower pad
    zero_line = ROOT.TLine(5.23, 0, 5.5, 0)
    zero_line.SetLineStyle(ROOT.kDashed)
    zero_line.SetLineColor(ROOT.kRed)
    zero_line.SetLineWidth(2)
    zero_line.Draw("same")
    pull_frame_mc.Draw("P same")
    c.SaveAs("BsJPsiPhi_MassFit_"+label+"/Fit_MC_"+year+".png")
    c.Delete()
    del c

    # Definire i coefficienti di segnale e fondo
    nsig = RooRealVar("nsig", "Number of signals", 60, 30, 1000)
    nbkg = RooRealVar("nbkg", "Number of backgrounds", h1.GetEntries(), 1, 2 * h1.GetEntries())

    #delta.setConstant(ROOT.kTRUE)
    mu.setRange(mu.getVal() * 0.998, mu.getVal() * 1.002) #0.2%
    delta.setRange(delta.getVal() * 0.93, delta.getVal() * 1.07) #7%
    gamm.setConstant(ROOT.kTRUE) 
    lambd.setConstant(ROOT.kTRUE) 
    frac.setConstant(ROOT.kTRUE) 
    #mu.setConstant(ROOT.kTRUE) 

    model = RooAddPdf("model", "Signal + Background", ROOT.RooArgList(gauss_pdf, exp_bkg), ROOT.RooArgList(nsig, nbkg))

    # Eseguire il fit
    model.fitTo(data, RooFit.Save(True), RooFit.Range("RT"))
    
    # Creare il frame per il fit
    frame = x.frame()
    data.plotOn(frame)

    model.plotOn(frame, RooFit.LineWidth(4))

    #Pull:
    pull_hist = frame.pullHist()
    pull_frame = x.frame()
    pull_frame.addPlotable(pull_hist, "P")

    # Calcola il chi²/ndf (ROOT calcola ndf internamente)
    params = model.getParameters(data)
    nFitParams = sum(1 for p in params if not p.isConstant())
    ndf = 30 - nFitParams
    chi2 = frame.chiSquare(nFitParams)


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

    print("*****", h1.GetMaximum())
    CMS.SetExtraText("Preliminary")
    CMS.SetLumi("2022+2023+2024, 170.7", unit="fb")
    CMS.SetEnergy(13.6, unit='TeV')
    # Applicare lo stile CMS
    canv = CMS.cmsDiCanvas("",  5.1, 5.7, 0, 1.2*h1.GetMaximum() , -5, 5, "m_{J/#psi#phi}(GeV)", 'Entries', "Pull", square=CMS.kSquare, extraSpace=0.02, iPos=11)
    canv.cd(1)  # Upper pad
    canv.SetCanvasSize(1000,750)
    frame.Draw("same")
    
    # Crea un box di testo
    chi2_label = ROOT.TPaveText(0.6, 0.4, 0.8, 0.55, "NDC")  # NDC = Normalized Device Coordinates
    chi2_label.SetFillColor(0)
    chi2_label.SetTextAlign(12)
    chi2_label.SetTextFont(42)
    chi2_label.SetTextSize(0.05)
    chi2_label.AddText(f"#chi^{{2}} = {ndf*chi2:.2f}")
    chi2_label.AddText(f"ndf = {ndf:.2f}")
    chi2_label.AddText(f"#chi^{{2}}/ndf = {chi2:.2f}")
    chi2_label.Draw("same")

    legend = ROOT.TLegend(0.6, 0.6, 0.9, 0.85)
    legend.SetTextSize(0.05)
    legend.SetBorderSize(0)  # Rimuove il bordo
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
    
    canv.cd(2)  # Lower pad
    pull_frame.Draw("P same")
    zero_line2 = ROOT.TLine(5.1, 0, 5.7, 0)
    zero_line2.SetLineStyle(ROOT.kDashed)
    zero_line2.SetLineColor(ROOT.kRed)
    zero_line2.SetLineWidth(2)
    zero_line2.Draw("same")

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
    FitBsJPsiPhi_Mass("_rw_bdt_v0", "20_01_25", f"bdt>0.48")
    #FitBsJPsiPhi_Mass("_rw_bdt", "20_01_25", f"Quadruplet_Mass_eq>0")
    # 2022 nsig      = 65.1684      +/-  8.65423   (limited)
    # 2023 nsig      = 42.6421      +/-  7.46291   (limited)
    # 2024 nsig      = 139.717      +/-  13.4737   (limited)
    # All nsig      = 257.681      +/-  18.0491   (limited)
    """
    cut = []
    nBs = []
    nBS_err = []
    nBS_ratio = []
    bins=25
    for i in range(bins):
        val, err = FitBsJPsiPhi_Mass("_rw_bdt_v0", "20_01_25", f"bdt>{i/bins}")
        if val is not None:
            nBs.append(val)
            nBS_err.append(err)
            nBS_ratio.append(val/err if err!=0 else 0)
            cut.append(i/bins)
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
    
    
    