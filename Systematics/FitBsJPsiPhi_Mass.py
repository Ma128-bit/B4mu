import ROOT
from ROOT import RooFit, RooRealVar, RooDataHist, RooExponential, RooAddPdf, RooPlot
import os
import cmsstyle as CMS

def fit(x, framein, d, color= ROOT.kRed, isMC=False):
    frame = x.frame()
    d.plotOn(frame, RooFit.MarkerColor(ROOT.kBlue), RooFit.MarkerStyle(20), RooFit.Name(f"Data {isMC}"))
    gamma = RooRealVar("#Gamma", "Gamma", -0.9, -10, 10)
    exp_bkg = RooExponential("exp_bkg", "Exponential Background", x, gamma)
    exp_bkg.fitTo(data, RooFit.Range("R1,R2"))

    mu = RooRealVar("mu", "mu", 5.36, 4.50, 6.0)
    lambd = RooRealVar("lambd", "lambd", 0.02, 0.001, 1.5)
    gamm = RooRealVar("gamm", "gamm", 0.0001, 0.0000001, 0.05)
    delta = RooRealVar("delta", "delta", 1.45, 0.1, 10)
    
    
    
    #gauss_pdf = ROOT.RooBifurGauss("signal_Bs", "Signal Bs", x, mu, lambd, gamm)
    gauss_pdf = ROOT.RooJohnson("signal_Bs", "Signal Bs", x, mu, lambd, gamm, delta)
    #gauss_pdf = ROOT.RooCrystalBall("signal_Bs", "Signal Bs", x, mu, lambd, gamm, delta)
    #
    nsig = RooRealVar("nsig", "Number of signals", 12000, 0., 2000000)
    nbkg = RooRealVar("nbkg", "Number of background", 5000, 0., 2000000)

    if isMC==True:
        nbkg.setVal(0)  
        nbkg.setConstant(True)

    # Modello totale: segnale + fondo
    model = ROOT.RooAddPdf("model", "sig + bkg", ROOT.RooArgList(gauss_pdf, exp_bkg), ROOT.RooArgList(nsig, nbkg))

    #model = ROOT.RooExtendPdf("model", "sig", gauss_pdf, nsig)
    model.fitTo(d, RooFit.Save(True), RooFit.Range("RS"))
    model.plotOn(frame, RooFit.LineColor(color), RooFit.LineStyle(ROOT.kDashed), RooFit.Name(f"Fit {isMC}"))
    model.plotOn(frame, RooFit.Components("exp_bkg"), RooFit.LineColor(ROOT.kGreen), RooFit.LineStyle(ROOT.kDashed), RooFit.Name(f"Fit {isMC}"))

    canvas = ROOT.TCanvas("canvas", "Fit Results", 800, 600)
    frame.Draw()
    canvas.SaveAs(f"{dir_path}/fit_comparison_{isMC}.png")

    nsig_val = nsig.getVal()
    
    nsig.setVal(1)
    nbkg.setVal(0)
    #mu.setVal(5.366)
    #delta.setVal(1.47)
    #lambd.setVal(0.025)
    model.plotOn(framein, RooFit.Components("signal_Bs"), RooFit.LineColor(color), RooFit.LineStyle(ROOT.kDashed), RooFit.Name(f"Fit {isMC}"))
    
    del canvas
    del frame
    return mu.getVal(), lambd.getVal(), gamm.getVal(), delta.getVal(), nsig_val

if __name__=="__main__":
    year=""
    label="20_01_25"
    file_path = f"../Utilities_Control/ROOTFiles_{label}/AllB2mu2K{year}_sPlot_rw_bdt.root"
    file = ROOT.TFile.Open(file_path)
    if not file or file.IsZombie():
        print("Error opening the file")
        exit()

    # Ottenere l'albero dal file
    tree = file.Get("FinalTree")
    if not tree:
        print("Error opening Tree")
        file.Close()
        exit()

    label_str = str(label)
    dir_path = f"BsJPsiPhi_MassFit_{label_str}"
    os.makedirs(dir_path, exist_ok=True)

    h1 = ROOT.TH1F("h1", "Quadruplet Mass", 120, 5.25, 5.5)
    tree.Draw("RefittedSV_Mass_eq >> h1", "(isMC==0 && RefittedSV_Mass>5.2 && RefittedSV_Mass<5.55)")

    h2 = ROOT.TH1F("h2", "Quadruplet Mass mc", 120, 5.25, 5.5)
    tree.Draw("RefittedSV_Mass_eq >> h2", "weight*(isMC>0)")

    x = RooRealVar("RefittedSV_Mass_eq", "Quadruplet Mass", 5.25, 5.5)
    x.setRange("R1", 5.25, 5.25)
    x.setRange("R2", 5.55, 5.5)
    x.setRange("RT", 5.15, 5.5)
    x.setRange("RS", 5.25, 5.5)
    x.setBins(120)
    frame = x.frame()

    data = RooDataHist("data", h1.GetTitle(), ROOT.RooArgSet(x), RooFit.Import(h1, False))
    mc = RooDataHist("mc", h2.GetTitle(), ROOT.RooArgSet(x), RooFit.Import(h2, False))

    datamu, datasig, datagamm, datadelt, _ = fit(x, frame, data, color= ROOT.kBlue)
    mcmu, mcsig, mcgamm, mcdelt, _ = fit(x, frame, mc, color= ROOT.kRed,isMC=True)

    canvas = ROOT.TCanvas("canvas", "Fit Results", 800, 600)
    frame.Draw()
    legend = ROOT.TLegend(0.6, 0.7, 0.88, 0.88)
    legend.AddEntry("Data", "Data", "p")
    legend.AddEntry("Fit", "Fit", "l")
    legend.Draw()
    canvas.SaveAs(f"{dir_path}/mc_comparison.png")

    print("Mu \nData: ",datamu, " MC:", mcmu)
    print("Lambda \nData: ",datasig, " MC:", mcsig)    
    print("Gamma \nData: ",datagamm, " MC:", mcgamm)    
    print("Delta \nData: ",datadelt, " MC:", mcdelt)    

    # Creazione del canvas e salvataggio dell'immagine
    

    file.Close()
