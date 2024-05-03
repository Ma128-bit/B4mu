from ROOT import RDataFrame, gROOT, EnableImplicitMT, gInterpreter, gDirectory, TChain, gPad
from ROOT import RooRealVar, RooExponential, RooJohnson, RooAddPdf, RooArgList, RooFit, kFALSE, RooDataHist, RooArgSet, kRed, kGreen, kDashed, TCanvas, RooCategory, RooSimultaneous
print("Import Done!")

gROOT.SetBatch(True)
EnableImplicitMT()

gInterpreter.Declare("""
    double Mass_eqKK(double Quadruplet_Mass, double Dimu_mass, double Ditrk_mass){
        double out = Quadruplet_Mass - Dimu_mass - Ditrk_mass + 1.019 + 3.0969;
        return out;
    }
    double Mass_eqKpi(double Quadruplet_Mass, double Dimu_mass, double Ditrk_mass_Kpi){
        double out = Quadruplet_Mass - Dimu_mass - Ditrk_mass_Kpi + 0.892 + 3.0969;
        return out;
    }
    vector<double> B0KpiMass(double Mu1pt, double Mu1eta, double Mu1phi, double Mu2pt, double Mu2eta, double Mu2phi, double Trk3pt, double Trk3eta, double Trk3phi, double Trk4pt, double Trk4eta, double Trk4phi){
        vector<double> out;
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
        if( abs(K1.M()-0.892) < abs(K2.M()-0.892)) {B0 = M1 + M2 + T31 + T41; out.push_back(K1.M());}
        else {B0 = M1 + M2 + T32 + T42; out.push_back(K2.M());}
        out.push_back(B0.M());
        return out;
    }
    struct flat0D_double{
    int i;
    flat0D_double(int ii) : i(ii)  {}
    double operator()(vector<double> branch) {
        if(i<branch.size()) return branch[i];
        else return -99;
        }
    };
""")

from ROOT import flat0D_double

if __name__ == "__main__":
    treename = "FinalTree"
    #file = "../Analysis/FinalFiles_B2mu2K/Analyzed_Data_B2mu2K_2022.root"
    file = "../Analysis/FinalFiles_B2mu2K/Analyzed_MC_B2mu2K_2022.root"
    rdf = RDataFrame(treename, file)
    print("Load RDF Done!")
    rdf = rdf.Define("B0Kpi","B0KpiMass(Mu1_Pt, Mu1_Eta, Mu1_Phi, Mu2_Pt, Mu2_Eta, Mu2_Phi, Mu3_Pt, Mu3_Eta, Mu3_Phi, Mu4_Pt, Mu4_Eta, Mu4_Phi)") 
    rdf = rdf.Define("Ditrk_mass_Kpi", flat0D_double(0), ["B0Kpi"])
    rdf = rdf.Define("B0KpiMass", flat0D_double(1), ["B0Kpi"])
    
    rdf = rdf.Define("Quadruplet_Mass_KKeq", "Mass_eqKK(Quadruplet_Mass, Dimu_mass, Ditrk_mass)")
    rdf = rdf.Define("Quadruplet_Mass_Kpieq", "Mass_eqKpi(B0KpiMass, Dimu_mass, Ditrk_mass_Kpi)")
    
    rdf = rdf.Filter("abs(Ditrk_mass-1.01945)<0.007 && abs(Dimu_mass-3.0969)<0.1 && vtx_prob>0")
    rdf.Snapshot("FinalTree", "temp.root")
    del rdf
    
    chain = TChain("FinalTree")
    chain.Add("temp.root")
    chain.Draw("Quadruplet_Mass_KKeq>>hBs(100, 5.25, 5.5)")
    chain.Draw("Quadruplet_Mass_Kpieq>>hB0(100, 5.25, 5.5)")
    hBs = gDirectory.Get("hBs") 
    hB0 = gDirectory.Get("hB0")    
    print("Histos Done!")
    
    x = RooRealVar("x", "x", 5.25, 5.5)
    x.setBins(100);
    """
    sample = RooCategory("sample","sample")
    sample.defineType("Bs")
    sample.defineType("B0")
    combData = RooDataHist("combData","combined data",x, RooFit.Index(sample), RooFit.Import("Bs",hBs), RooFit.Import("B0",hB0))

    # Main Model
    mu = RooRealVar("mu", "mu", 5.366, 5.2, 5.5)
    lambd = RooRealVar("lambd", "lambd", 0, 10)
    gamma = RooRealVar("gamma", "gamma", -10, 10)
    delta = RooRealVar("delta", "delta", 0, 20)
    signal_Bs = RooJohnson("signal_Bs", "signal_Bs", x, mu, lambd, gamma, delta )

    mu2 = RooRealVar("mu2", "mu2", 5.366, 5.2, 5.5)
    lambd2 = RooRealVar("lambd2", "lambd2", 0, 10)
    gamma2 = RooRealVar("gamma2", "gamma2", -10, 10)
    delta2 = RooRealVar("delta2", "delta2", 0, 20)
    signal_Bd = RooJohnson("signal_Bd", "signal_Bd", x, mu2, lambd2, gamma2, delta2 )

    c1 = RooRealVar("c1", "c1", -0.2, -10, 10)
    bkg_Bs = RooExponential("bkg_Bs", "bkg_Bs", x, c1)

    nsig = RooRealVar("nsig", "Numero di segnali", 100000, 50000, 1000000)
    nsig2 = RooRealVar("nsig", "Numero di segnali", 3000, 1000, 10000)
    nbkg = RooRealVar("nbkg", "Numero di background",70000, 10000, 100000)
    model = RooAddPdf("model", "Signal + Background", RooArgList(signal_Bs, signal_Bd, bkg_Bs), RooArgList(nsig, nsig2, nbkg))
    
    # Reflected Model
    muR = RooRealVar("muR", "muR", 5.366, 5.2, 5.5)
    lambdR = RooRealVar("lambdR", "lambdR", 0, 10)
    gammaR = RooRealVar("gammaR", "gammaR", -10, 10)
    deltaR = RooRealVar("deltaR", "deltaR", 0, 20)
    Rsignal_Bs = RooJohnson("Rsignal_Bs", "Rsignal_Bs", x, muR, lambdR, gammaR, deltaR )

    mu2R = RooRealVar("mu2R", "mu2R", 5.366, 5.2, 5.5)
    lambd2R = RooRealVar("lambd2R", "lambd2R", 0, 10)
    gamma2R = RooRealVar("gamma2R", "gamma2R", -10, 10)
    delta2R = RooRealVar("delta2R", "delta2R", 0, 20)
    Rsignal_Bd = RooJohnson("Rsignal_Bd", "Rsignal_Bd", x, mu2R, lambd2R, gamma2R, delta2R )

    Rc1 = RooRealVar("Rc1", "Rc1", -0.2, -10, 10)
    Rbkg_Bs = RooExponential("Rbkg_Bs", "Rbkg_Bs", x, Rc1)

    Rmodel = RooAddPdf("Rmodel", "RSignal + RBackground", RooArgList(Rsignal_Bs, Rsignal_Bd, Rbkg_Bs), RooArgList(nsig, nsig2, nbkg))

    simPdf = RooSimultaneous("simPdf","simultaneous pdf", sample)
 
    simPdf.addPdf(model,"Bs")
    simPdf.addPdf(Rmodel,"B0")

    simPdf.fitTo(combData)

    frame1 = x.frame(RooFit.Title("Bs sample"))
    combData.plotOn(frame1,RooFit.Cut("sample==sample::Bs"))
    simPdf.plotOn(frame1,RooFit.Slice(sample,"Bs"),RooFit.ProjWData(sample,combData))
    simPdf.plotOn(frame1,RooFit.Slice(sample,"Bs"),RooFit.Components(bkg_Bs),RooFit.ProjWData(sample,combData),RooFit.LineStyle(kDashed))
    simPdf.plotOn(frame1,RooFit.Slice(sample,"Bs"),RooFit.Components(signal_Bs),RooFit.ProjWData(sample,combData), RooFit.LineColor(kGreen), RooFit.LineStyle(kDashed))
    simPdf.plotOn(frame1,RooFit.Slice(sample,"Bs"),RooFit.Components(signal_Bd),RooFit.ProjWData(sample,combData), RooFit.LineColor(kRed), RooFit.LineStyle(kDashed))

    frame2 = x.frame(RooFit.Title("B0 sample"))
    combData.plotOn(frame2, RooFit.Cut("sample==sample::B0"))
    simPdf.plotOn(frame2, RooFit.Slice(sample,"B0"), RooFit.ProjWData(sample,combData))
    simPdf.plotOn(frame2, RooFit.Slice(sample,"B0"), RooFit.Components(Rbkg_Bs), RooFit.ProjWData(sample,combData), RooFit.LineStyle(kDashed))
    simPdf.plotOn(frame2, RooFit.Slice(sample,"B0"), RooFit.Components(Rsignal_Bs), RooFit.ProjWData(sample,combData), RooFit.LineColor(kGreen), RooFit.LineStyle(kDashed))
    simPdf.plotOn(frame2, RooFit.Slice(sample,"B0"), RooFit.Components(Rsignal_Bd), RooFit.ProjWData(sample,combData), RooFit.LineColor(kRed), RooFit.LineStyle(kDashed))
 
    canvas = TCanvas("canvas", "Fit Result", 1200, 600)
    canvas.Divide(2)
    canvas.cd(1)
    gPad.SetLeftMargin(0.15)
    frame1.GetYaxis().SetTitleOffset(1.4)
    frame1.Draw()
    canvas.cd(2)
    gPad.SetLeftMargin(0.15)
    frame2.GetYaxis().SetTitleOffset(1.4)
    frame2.Draw()
    canvas.SaveAs("test.png")
    
    """
    data = RooDataHist("data", hBs.GetTitle(), RooArgSet(x), RooFit.Import(hBs))
    
    mu = RooRealVar("mu", "mu", 5.366, 5.3, 5.45)
    lambd = RooRealVar("lambd", "lambd", 0, 10)
    gamma = RooRealVar("gamma", "gamma", -10, 10)
    delta = RooRealVar("delta", "delta", 0, 20)
    signal_Bs = RooJohnson("signal_Bs", "signal_Bs", x, mu, lambd, gamma, delta )

    c1 = RooRealVar("c1", "c1", -0.2, -10, 10)
    bkg_Bs = RooExponential("bkg_Bs", "bkg_Bs", x, c1)
    
    nsig = RooRealVar("nsig", "Numero di segnali", 750000, 100000, 1250000)
    nbkg = RooRealVar("nbkg", "Numero di background",10000, 1000, 100000)

    model = RooAddPdf("model", "Signal + Background", RooArgList(signal_Bs, bkg_Bs), RooArgList(nsig, nbkg))

    result = model.fitTo(data)
    
    frame = x.frame()
    data.plotOn(frame)
    model.plotOn(frame, RooFit.Components(signal_Bs), RooFit.LineStyle(kDashed), RooFit.LineColor(kRed))
    model.paramOn(frame, RooFit.Parameters(RooArgSet(nsig, nbkg, mu, lambd, gamma, delta, c1)), RooFit.Layout(0.5,0.9,0.9))
    model.plotOn(frame, RooFit.Components(bkg_Bs), RooFit.LineStyle(kDashed), RooFit.LineColor(kGreen))
    model.plotOn(frame)
    
    
    canvas = TCanvas("canvas", "Fit Result", 900, 600)
    frame.Draw();
    canvas.SaveAs("test.png")

    
  
  

