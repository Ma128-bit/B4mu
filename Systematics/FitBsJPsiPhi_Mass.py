import ROOT
from ROOT import RooFit, RooRealVar, RooDataHist, RooExponential, RooAddPdf, RooPlot
import os
import cmsstyle as CMS

if __name__=="__main__":
    year=""
    label="02_12_24"
    file_path = f"../Utilities_Control/ROOTFiles_{label}/AllB2mu2K{year}_sPlot_bdt.root"
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

    h1 = ROOT.TH1F("h1", "Quadruplet Mass", 80, 5.05, 5.7)
    tree.Draw("RefittedSV_Mass >> h1", "nsigBs_sw*(isMC==0 && RefittedSV_Mass>5.05 && RefittedSV_Mass<5.7)")

    h2 = ROOT.TH1F("h2", "Quadruplet Mass mc", 80, 5.05, 5.7)
    tree.Draw("RefittedSV_Mass >> h2", "nsigBs_sw*weight*(isMC>0)")

    x = RooRealVar("RefittedSV_Mass_eq", "Quadruplet Mass", 5.05, 5.7)
    x.setRange("R1", 5.05, 5.25)
    x.setRange("R2", 5.55, 5.7)
    x.setRange("RT", 5.05, 5.7)
    x.setRange("RS", 5.25, 5.55)
    x.setBins(80)

    data = RooDataHist("data", h1.GetTitle(), ROOT.RooArgSet(x), RooFit.Import(h1, False))
    mc = RooDataHist("mc", h2.GetTitle(), ROOT.RooArgSet(x), RooFit.Import(h2, False))

    gamma = RooRealVar("#Gamma", "Gamma", -0.9, -10, 10)
    exp_bkg = RooExponential("exp_bkg", "Exponential Background", x, gamma)
    exp_bkg.fitTo(data, RooFit.Range("R1,R2"))

    mu = RooRealVar("mu", "mu", 5.36, 4.50, 6.0)
    lambd = RooRealVar("lambd", "lambd", 0.02, 0.001, 1.5)
    gamm = RooRealVar("gamm", "gamm", 0.14, 0.01, 1.5)
    delta = RooRealVar("delta", "delta", 1.45, 0.1, 10)
    gauss_pdf = ROOT.RooJohnson("signal_Bs", "Signal Bs", x, mu, lambd, gamm, delta)

    nsig = RooRealVar("nsig", "Number of signals", 12000, 0., 2000000)

    model = ROOT.RooExtendPdf("model", "sig", gauss_pdf, nsig)

    model.fitTo(mc, RooFit.Save(True), RooFit.Range("RS"))
    mcmu = mu.getVal()

    model.fitTo(data, RooFit.Save(True), RooFit.Range("RS"))
    datamu = mu.getVal()

    print("Mu \nData: ",datamu, " MC:", mcmu)
    file.Close()
