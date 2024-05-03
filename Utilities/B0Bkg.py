from ROOT import RDataFrame, gROOT, EnableImplicitMT, gInterpreter, gDirectory
from ROOT import RooRealVar, RooExponential, RooJohnson, RooAddPdf, RooArgList, RooFit, kFALSE, RooDataHist, RooArgSet, kRed, kGreen, kDashed, TCanvas, RooCategory
print("Import Done!")

gROOT.SetBatch(True)
EnableImplicitMT()

gInterpreter.Declare("""
    double B0KpiMass(double Mu1pt, double Mu1eta, double Mu1phi, double Mu2pt, double Mu2eta, double Mu2phi, double Trk3pt, double Trk3eta, double Trk3phi, double Trk4pt, double Trk4eta, double Trk4phi){
        TLorentzVector M1, M2, T31, T41, T32, T42;
        M1.SetPtEtaPhiM(Mu1pt, Mu1eta, Mu1phi, 0.10566);
        M2.SetPtEtaPhiM(Mu2pt, Mu2eta, Mu2phi, 0.10566);
        T31.SetPtEtaPhiM(Trk3pt, Trk3eta, Trk3phi, 0.493677);
        T41.SetPtEtaPhiM(Trk4pt, Trk4eta, Trk4phi, 0.139570);
        T32.SetPtEtaPhiM(Trk3pt, Trk3eta, Trk3phi, 0.139570);
        T42.SetPtEtaPhiM(Trk4pt, Trk4eta, Trk4phi, 0.493677);  
        TLorentzVector K1, K2;
        K1 = T31 + T41;
        K2 = T32 + T42;
        TLorentzVector B0;
        if( abs(K1.M()-0.892) < abs(K2.M()-0.892)) B0 = M1 + M2 + T31 + T41;
        else B0 = M1 + M2 + T32 + T42;
        return B0.M();
    }
""")


if __name__ == "__main__":
    treename = "FinalTree"
    file = "../Analysis/FinalFiles_B2mu2K/Analyzed_Data_B2mu2K_2022.root"
    rdf = RDataFrame(treename, file)
    print("Load RDF Done!")
    rdf = rdf.Define("B0KpiMass","B0KpiMass(Mu1_Pt, Mu1_Eta, Mu1_Phi, Mu2_Pt, Mu2_Eta, Mu2_Phi, Mu3_Pt, Mu3_Eta, Mu3_Phi, Mu4_Pt, Mu4_Eta, Mu4_Phi)") 

    rdf = rdf.Filter("abs(Ditrk_mass-1.01945)<0.007 && abs(Dimu_mass-3.0969)<0.1 && vtx_prob>0")
    hBs = rdf.Histo1D(("Quadruplet_Mass", "Quadruplet_Mass", 100, 5.25, 5.5), "Quadruplet_Mass")
    hB0 = rdf.Histo1D(("B0KpiMass", "B0KpiMass", 100, 5.25, 5.5), "B0KpiMass")
    print("Histos Done!")

    x = RooRealVar("x", "x", 5.25, 5.5)
    x.setBins(100);
    
    sample = RooCategory("sample","sample")
    sample.defineType("Bs")
    sample.defineType("B0")
    #combData = RooDataHist("combData","combined data",x, RooFit.Index(sample), Import("Bs",hBs), Import("B0",hB0))

    data = RooDataHist("data", hBs.GetTitle(), RooArgSet(x), RooFit.Import(hBs))
    
    mu = RooRealVar("mu", "mu", (up+down)/2, down, up)
    lambd = RooRealVar("lambd", "lambd", 0.005, 0.001, 0.02)
    gamma = RooRealVar("gamma", "gamma", 0.005, 0.001, 0.02)
    delta = RooRealVar("delta", "delta", 0.005, 0.001, 0.02)
    signal_Bs = RooJohnson("signal_Bs", "signal_Bs", x, mu, lambd, gamma, delta )

    c1 = RooRealVar("c1", "c1", -0.2, -10, 10)
    bkg_Bs = RooExponential("bkg_Bs", "bkg_Bs", x, c1)
    
    nsig = RooRealVar("nsig", "Numero di segnali", 200000, 10000, 400000)
    nbkg = RooRealVar("nbkg", "Numero di background",400000, 100000, 1000000)

    model = RooAddPdf("model", "Signal + Background", RooArgList(signal_Bs, bkg_Bs), RooArgList(nsig, nbkg))

    result = model.fitTo(data, Save(true))
    
    frame = x.frame()
    data.plotOn(frame)
    model.plotOn(frame, Components(signal_Bs), LineStyle(kDashed), LineColor(kRed))
    model.paramOn(frame, Parameters(RooArgSet(nsig, nbkg, mu, lambd, gamma, delta, c1)), Layout(0.5,0.9,0.9))
    model.plotOn(frame, Components(bkg_Bs), LineStyle(kDashed), LineColor(kGreen))
    model.plotOn(frame)
    
    
    canvas = TCanvas("canvas", "Fit Result", 900, 600)
    frame.Draw();
    canvas.SaveAs("test.png")
  
  

