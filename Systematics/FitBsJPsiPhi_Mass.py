import ROOT
from ROOT import RooFit, RooRealVar, RooDataHist, RooExponential, RooAddPdf, RooPlot, RooGenericPdf, RooArgList, RooJohnson, RooGaussian
import os
import cmsstyle as CMS

if __name__=="__main__":
    year=""
    label="18_11_25"
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

    h1 = ROOT.TH1F("h1", "Quadruplet Mass", 180, 5.24, 5.6)
    tree.Draw("Quadruplet_Mass >> h1", "(isMC==0 && vtx_prob>0.01)")

    h2 = ROOT.TH1F("h2", "Quadruplet Mass mc", 180, 5.24, 5.6)
    tree.Draw("Quadruplet_Mass >> h2", "weight_pileUp*ctau_weight_central*bdt_reweight_0*bdt_reweight_1*bdt_reweight_2*(isMC>0 && vtx_prob>0.01)")

    x = ROOT.RooRealVar("x", "Quadruplet Mass", 5.24, 5.6)
    x.setRange("R1", 5.24, 5.25)
    x.setRange("R2", 5.55, 5.6)
    x.setRange("RS", 5.24, 5.5)

    # ----------------------------------------------------------------
    # 3) RooDataHist (usa RooArgSet per sicurezza)
    # ----------------------------------------------------------------
    data = ROOT.RooDataHist("data", h1.GetTitle(), ROOT.RooArgSet(x), RooFit.Import(h1, False))
    mc   = ROOT.RooDataHist("mc",   h2.GetTitle(), ROOT.RooArgSet(x), RooFit.Import(h2, False))

    # ----------------------------------------------------------------
    # 4) Categoria + combined dataset (RooArgSet!)
    # ----------------------------------------------------------------
    sample = ROOT.RooCategory("sample","sample")
    sample.defineType("A")
    sample.defineType("B")

    combData = ROOT.RooDataHist(
        "combData", "combined data",
        ROOT.RooArgSet(x),          # <- IMPORTANT: RooArgSet
        RooFit.Index(sample),
        RooFit.Import("A", data),
        RooFit.Import("B", mc)
    )

    # ----------------------------------------------------------------
    # 5) MODELLO CANALE A (data) - background + signal (gauss + johnson)
    # ----------------------------------------------------------------
    # background exponential (fit preliminare su data nelle sidebands opzionale)
    gamma = ROOT.RooRealVar("gamma_bkg", "Gamma bkg", -1.9, -10, 10)
    exp_bkg = ROOT.RooExponential("exp_bkg_A", "Exponential Background A", x, gamma)
    # (opzionale) fit preliminare del fondo nelle sideband:
    # exp_bkg.fitTo(data, RooFit.Range("R1,R2"))

    # signal: gauss + johnson
    muA    = ROOT.RooRealVar("muA", "muA", 5.366, 5.20, 5.5)
    lamA   = ROOT.RooRealVar("lambdaA", "lambdaA", 0.047, 0.001, 0.2)
    gammA  = ROOT.RooRealVar("gammA", "gammA", 0.1, -1.5, 1.5)
    deltaA = ROOT.RooRealVar("deltaA", "deltaA", 1.5, 0.01, 10)

    gaussA   = ROOT.RooGaussian("gaussA", "gaussA", x, muA, lamA)
    johnsonA = ROOT.RooJohnson("johnsonA", "johnsonA", x, muA, lamA, gammA, deltaA)

    # fraction of gaussian inside signal shape (0..1)
    frac_sig_shape = ROOT.RooRealVar("frac_sig_shape", "frac gaussian in signal", 0.15, 0.0, 1.0)

    sigShapeA = ROOT.RooAddPdf("sigShapeA", "signal shape A", ROOT.RooArgList(gaussA, johnsonA), ROOT.RooArgList(frac_sig_shape))

    # fraction of signal vs background in channel A (0..1)
    fracA = ROOT.RooRealVar("fracA", "frac signal in A", 0.6, 0.0, 1.0)

    modelA = ROOT.RooAddPdf("modelA", "sig + bkg (A)", ROOT.RooArgList(sigShapeA, exp_bkg), ROOT.RooArgList(fracA))


    # ----------------------------------------------------------------
    # 6) MODELLO CANALE B (MC) - solo signal shape (gaussB + johnsonB)
    #    uso lo stesso 'frac_sig_shape' per condividere la frazione interna (se desideri)
    # ----------------------------------------------------------------
    muB    = ROOT.RooRealVar("muB", "muB", 5.366, 5.20, 5.5)
    lamB   = ROOT.RooRealVar("lambdaB", "lambdaB", 0.047, 0.001, 0.2)
    gammB  = ROOT.RooRealVar("gammB", "gammB", 0.1, -1.5, 1.5)
    deltaB = ROOT.RooRealVar("deltaB", "deltaB", 1.5, 0.01, 10)

    gaussB   = ROOT.RooGaussian("gaussB", "gaussB", x, muB, lamB)
    johnsonB = ROOT.RooJohnson("johnsonB", "johnsonB", x, muB, lamB, gammB, deltaB)

    # qui decido di riutilizzare la stessa frac_sig_shape (condiviso) per il shape interno
    sigShapeB = ROOT.RooAddPdf("sigShapeB", "signal shape B", ROOT.RooArgList(gaussB, johnsonB), ROOT.RooArgList(frac_sig_shape))

    # se B è solo segnale, modelB è semplicemente sigShapeB (no fondo)
    # ma RooSimultaneous si aspetta pdf per ciascun canale: uso sigShapeB come modelB
    modelB = sigShapeB  # semplice alias

    # ----------------------------------------------------------------
    # 7) Simultaneous
    # ----------------------------------------------------------------
    simPdf = ROOT.RooSimultaneous("simPdf","simPdf", sample)
    simPdf.addPdf(modelA, "A")
    simPdf.addPdf(modelB, "B")

    # ----------------------------------------------------------------
    # 8) Fit: NON-EXTENDED (usa frazioni, evita mismatch extended)
    # ----------------------------------------------------------------
    result = simPdf.fitTo(
        combData,
        RooFit.Save(),
        RooFit.PrintLevel(1),
        RooFit.Offset(True)   # opzionale, migliora stabilità
    )

    result.Print("v")

    c = ROOT.TCanvas("c","Simultaneous fit",1200,800)
    c.Divide(2,2)

    # A
    c.cd(1)
    frameA = x.frame(RooFit.Title("Data"))
    combData.plotOn(frameA, RooFit.Cut("sample==sample::A"))
    simPdf.plotOn(frameA,
              RooFit.Slice(sample,"A"),
              RooFit.ProjWData(sample, combData),
              RooFit.Components("exp_bkg_A"),
              RooFit.LineStyle(ROOT.kDashed),
              RooFit.LineColor(ROOT.kGreen))
    simPdf.plotOn(frameA,
              RooFit.Slice(sample,"A"),
              RooFit.ProjWData(sample, combData),
              RooFit.Components("gaussA"),
              RooFit.LineStyle(ROOT.kDashed),
              RooFit.LineColor(ROOT.kYellow+3))
    simPdf.plotOn(frameA,
              RooFit.Slice(sample,"A"),
              RooFit.ProjWData(sample, combData),
              RooFit.Components("johnsonA"),
              RooFit.LineStyle(ROOT.kDashed),
              RooFit.LineColor(ROOT.kGray))

    simPdf.plotOn(frameA,
                RooFit.Slice(sample,"A"),
                RooFit.ProjWData(sample, combData))
    pullA = frameA.pullHist()

    frameA.Draw()

    # ============================
    # PULL A
    # ============================
    c.cd(3)

    framePullA = x.frame(RooFit.Title("Pull (Data)"))
    framePullA.addPlotable(pullA, "P")

    framePullA.GetYaxis().SetTitle("(Data - Fit) / #sigma")
    framePullA.GetYaxis().SetRangeUser(-5,5)
    framePullA.Draw()

    line = ROOT.TLine(x.getMin(), 0, x.getMax(), 0)
    line.SetLineStyle(ROOT.kDashed)
    line.Draw()

    # B
    c.cd(2)
    frameB = x.frame(RooFit.Title("MC"))
    combData.plotOn(frameB, RooFit.Cut("sample==sample::B"))
    simPdf.plotOn(frameB,
                RooFit.Slice(sample,"B"),
                RooFit.ProjWData(sample, combData))
    pullB = frameB.pullHist()
    simPdf.plotOn(frameB,
              RooFit.Slice(sample,"B"),
              RooFit.ProjWData(sample, combData),
              RooFit.Components("gaussB"),
              RooFit.LineStyle(ROOT.kDashed),
              RooFit.LineColor(ROOT.kYellow+3))
    simPdf.plotOn(frameB,
              RooFit.Slice(sample,"B"),
              RooFit.ProjWData(sample, combData),
              RooFit.Components("johnsonB"),
              RooFit.LineStyle(ROOT.kDashed),
              RooFit.LineColor(ROOT.kGray))

    frameB.Draw()

    # ============================
    # PULL B
    # ============================
    c.cd(4)

    framePullB = x.frame(RooFit.Title("Pull (MC)"))
    framePullB.addPlotable(pullB, "P")

    framePullB.GetYaxis().SetTitle("(MC - Fit) / #sigma")
    framePullB.GetYaxis().SetRangeUser(-5,5)
    framePullB.Draw()

    lineB = ROOT.TLine(x.getMin(), 0, x.getMax(), 0)
    lineB.SetLineStyle(ROOT.kDashed)
    lineB.Draw()

    # Aggiorna canvas
    c.Update()

    # -----------------------
    # Salvataggio su file
    # -----------------------
    outname = "simultaneous_fit"

    c.SaveAs(outname + ".png")

    print(f"Canvas salvata come {outname}.png ")

    # -----------------------------
    # Canvas confronto segnale
    # -----------------------------
    c2 = ROOT.TCanvas("c2","Signal Shapes Comparison",800,600)

    frameSig = x.frame(RooFit.Title("Signal Shapes Comparison"))

    #deltaB.setVal(deltaA.getVal())
    gammB.setVal(gammA.getVal())
    muB.setVal(muA.getVal())

    # Plot sigShapeA (canale A) - colore blu
    sigShapeA.plotOn(frameSig,
                    RooFit.LineColor(ROOT.kBlue),
                    RooFit.Normalization(1.0, ROOT.RooAbsReal.Relative),  # normalizza area a 1
                    RooFit.LineWidth(2),
                    RooFit.Name("sigA"))

    # Plot sigShapeB (canale B) - colore rossa
    sigShapeB.plotOn(frameSig,
                    RooFit.LineColor(ROOT.kRed),
                    RooFit.Normalization(1.0, ROOT.RooAbsReal.Relative),  # normalizza area a 1
                    RooFit.LineWidth(2),
                    RooFit.LineStyle(ROOT.kDashed),
                    RooFit.Name("sigB"))

    # Disegna frame
    frameSig.Draw()

    # -----------------------------
    # Legenda
    # -----------------------------
    leg = ROOT.TLegend(0.65,0.7,0.9,0.9)  # posizione: x1,y1,x2,y2
    leg.AddEntry(frameSig.findObject("sigA"), "data", "l")
    leg.AddEntry(frameSig.findObject("sigB"), "MC", "l")
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.Draw()

    # Aggiorna canvas e salva
    c2.Update()
    c2.SaveAs("SignalShapesComparison.png")
    print("Canvas confronto salvata come SignalShapesComparison.png")