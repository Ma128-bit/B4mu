#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooPlot.h>
#include <RooGaussian.h>
#include <RooExponential.h>
#include <RooAddPdf.h>
#include <RooFitResult.h>
#include <filesystem>

using namespace RooFit;
//Dimu_mass>>h1(100,2.8,3.5)
//Ditrk_mass>>h2(100,1.01,1.06)

void Fit2mu2K(TString dataFile="../Analysis/FinalFiles_B2mu2K/Analyzed_Data_B2mu2K_2022.root", TString var="Ditrk_mass", float down=1.002, float up=1.05) {
    // Aprire il file root contenente l'albero
    TFile *file = new TFile(dataFile);
    if (!file || file->IsZombie()) {
        std::cerr << "Errore nell'apertura del file" << std::endl;
        return;
    }

    // Ottenere l'albero dal file
    TTree *tree = (TTree*)file->Get("FinalTree");
    if (!tree) {
        std::cerr << "Errore nell'apertura dell'albero" << std::endl;
        file->Close();
        return;
    }
    TString s;
    s.Form(">>h1(100,%f,%f)", down, up);
    tree->Draw(var+s, "abs(Quadruplet_Mass-5.2)<0.4");
    TH1F *h1 = (TH1F*)gDirectory->Get("h1");
      
    RooRealVar x(var, var, down, up);
    x.setBins(100);
    
    RooDataHist data("data", h1->GetTitle(), RooArgSet(x), Import(*h1, kFALSE));
    x.setRange("R1", 1.002, 1.01);
    x.setRange("R2", 1.01, 1.03);
    x.setRange("R3", 1.03, 1.05);
    
    // Creare il fondo
    RooRealVar c1("c1", "c1", -0.2, -10, 10);
    RooRealVar c2("c2", "c2", -0.2, -10, 10);
    RooRealVar c3("c3", "c3", -0.2, -10, 10);
    
    RooChebychev pol_bkg("pol_bkg", "pol_bkg", x, RooArgList(c1,c2));
    pol_bkg.fitTo(data,Range("R1,R3"));
    
    // Creare la gaussiana
    RooRealVar mean("mean", "Media gaussiana", (up+down)/2, down, up);
    RooRealVar sigma("sigma", "Deviazione standard gaussiana", 0.005, 0.001, 0.02);
    RooRealVar width("width", "width", 0.005, 0.001, 0.02);
    //RooVoigtian voigt_pdf("voigt_pdf", "Signal Gaussian PDF", x, mean, width, sigma);
    RooGaussian voigt_pdf("voigt_pdf", "Signal Gaussian PDF", x, mean, sigma);

    RooRealVar mean2("mean2", "Media gaussiana 2", (up+down)/2, down, up);
    RooRealVar sigma2("sigma2", "Deviazione standard gaussiana 2", 0.005, 0.001, 0.02);
    RooGaussian voigt_pdf2("voigt_pdf2", "Signal Gaussian PDF 2", x, mean2, sigma2);
    
    // Creare il modello di fit combinando fondo e gaussiana
    RooRealVar nsig("nsig", "Numero di segnali", 200000, 50000, 400000);
    RooRealVar nsig2("nsi2g", "Numero di segnali2", 200000, 50000, 400000);
    RooRealVar nbkg("nbkg", "Numero di background",2000000, 1700000, 2400000);

    RooAddPdf model("model", "Signal + Background", RooArgList(voigt_pdf, voigt_pdf2, pol_bkg), RooArgList(nsig, nsig2, nbkg));

    RooFitResult *result = model.fitTo(data, Save(true));
    
    RooPlot *frame = x.frame();
    data.plotOn(frame);
    model.plotOn(frame, Components(voigt_pdf, voigt_pdf2), LineStyle(kDashed), LineColor(kRed));
    model.plotOn(frame, Components(voigt_pdf), LineStyle(kDashed), LineColor(kRed+2));
    model.plotOn(frame, Components(voigt_pdf2), LineStyle(kDashed), LineColor(kRed+2));
    model.paramOn(frame, Parameters(RooArgSet(nsig, nsig2, nbkg, mean, mean2, sigma, sigma2)), Layout(0.5,0.9,0.9));
    model.plotOn(frame, Components(pol_bkg), LineStyle(kDashed), LineColor(kGreen));
    model.plotOn(frame);
    
    TCanvas *canvas = new TCanvas("canvas", "Fit Result", 900, 600);
    frame->Draw();
    //canvas->SaveAs("Fit_results/Fit_BsJPsiPhi.png");

    // Chiudere il file
    //file->Close();
}

void Fit2muKpi(TString dataFile="../Analysis/FinalFiles_B2muKpi/Analyzed_Data_B2muKpi_2022.root", TString var="Ditrk_mass", float down=0.82, float up=1.05) {
    // Aprire il file root contenente l'albero
    TFile *file = new TFile(dataFile);
    if (!file || file->IsZombie()) {
        std::cerr << "Errore nell'apertura del file" << std::endl;
        return;
    }

    // Ottenere l'albero dal file
    TTree *tree = (TTree*)file->Get("FinalTree");
    if (!tree) {
        std::cerr << "Errore nell'apertura dell'albero" << std::endl;
        file->Close();
        return;
    }
    TString s;
    s.Form(">>h1(100,%f,%f)", down, up);
    tree->Draw(var+s, "abs(Quadruplet_Mass-5.2)<0.4");
    TH1F *h1 = (TH1F*)gDirectory->Get("h1");
      
    RooRealVar x(var, var, down, up);
    x.setBins(100);
    
    RooDataHist data("data", h1->GetTitle(), RooArgSet(x), Import(*h1, kFALSE));
    x.setRange("R1", 0.82, 0.83);
    x.setRange("R2", 1.01, 1.03);
    x.setRange("R3", 0.93, 1.05);
    
    // Creare il fondo
    RooRealVar c1("c1", "c1", -0.2, -10, 10);
    RooRealVar c2("c2", "c2", -0.2, -10, 10);
    RooRealVar c3("c3", "c3", -0.2, -10, 10);
    
    RooChebychev pol_bkg("pol_bkg", "pol_bkg", x, RooArgList(c1,c2,c3));
    pol_bkg.fitTo(data,Range("R1,R3"));
    
    // Creare la gaussiana
    RooRealVar mean("mean", "Media gaussiana", 0.89, 0.88, 0.9);
    RooRealVar sigma("sigma", "Deviazione standard gaussiana", 0.05, 0.002, 0.02);
    RooRealVar width("width", "width", 0.005, 0.001, 0.02);
    //RooVoigtian voigt_pdf("voigt_pdf", "Signal Gaussian PDF", x, mean, width, sigma);
    RooGaussian voigt_pdf("voigt_pdf", "Signal Gaussian PDF", x, mean, sigma);

    RooRealVar mean2("mean2", "Media gaussiana 2", 0.89, 0.88, 0.9);
    RooRealVar sigma2("sigma2", "Deviazione standard gaussiana 2", 0.05, 0.005, 0.1);
    RooGaussian voigt_pdf2("voigt_pdf2", "Signal Gaussian PDF 2", x, mean2, sigma2);
    
    // Creare il modello di fit combinando fondo e gaussiana
    RooRealVar nsig("nsig", "Numero di segnali", 2500000, 100000, 5000000);
    RooRealVar nsig2("nsi2g", "Numero di segnali2", 800000, 100000, 2000000);
    RooRealVar nbkg("nbkg", "Numero di background",12000000, 7000000, 15000000);

    RooAddPdf model("model", "Signal + Background", RooArgList(voigt_pdf, pol_bkg), RooArgList(nsig, nbkg));

    RooFitResult *result = model.fitTo(data, Save(true));
    
    RooPlot *frame = x.frame();
    data.plotOn(frame);
    model.plotOn(frame, Components(voigt_pdf), LineStyle(kDashed), LineColor(kRed));
    model.paramOn(frame, Parameters(RooArgSet(nsig, nbkg, mean, sigma)), Layout(0.5,0.9,0.9));
    model.plotOn(frame, Components(pol_bkg), LineStyle(kDashed), LineColor(kGreen));
    model.plotOn(frame);
    
    TCanvas *canvas = new TCanvas("canvas", "Fit Result", 900, 600);
    frame->Draw();
    //canvas->SaveAs("Fit_results/Fit_BsJPsiPhi.png");

    // Chiudere il file
    //file->Close();
}

void Fit2muKpiM(TString dataFile="../Analysis/FinalFiles_B2muKpi/Analyzed_Data_B2muKpi_2022.root", TString var="Quadruplet_Mass", float down=4.9, float up=5.6) {
    // Aprire il file root contenente l'albero
    TFile *file = new TFile(dataFile);
    if (!file || file->IsZombie()) {
        std::cerr << "Errore nell'apertura del file" << std::endl;
        return;
    }

    // Ottenere l'albero dal file
    TTree *tree = (TTree*)file->Get("FinalTree");
    if (!tree) {
        std::cerr << "Errore nell'apertura dell'albero" << std::endl;
        file->Close();
        return;
    }
    TString s;
    s.Form(">>h1(80,%f,%f)", down, up);
    tree->Draw(var+s, "abs(Ditrk_mass-0.892)<0.075 && abs(Dimu_mass-3.0969)<0.09");
    TH1F *h1 = (TH1F*)gDirectory->Get("h1");
      
    RooRealVar x(var, var, down, up);
    x.setBins(80);
    
    RooDataHist data("data", h1->GetTitle(), RooArgSet(x), Import(*h1, kFALSE));
    x.setRange("R1", 4.9, 5.1);
    x.setRange("R2", 5.1, 5.4);
    x.setRange("R3", 5.4, 5.6);
    
    // Creare il fondo
    RooRealVar c1("c1", "c1", -0.2, -10, 10);
    RooRealVar c2("c2", "c2", -0.2, -10, 10);
    RooRealVar c3("c3", "c3", -0.2, -10, 10);
    
    //RooExponential pol_bkg("pol_bkg", "pol_bkg", x, c1);
    RooChebychev pol_bkg("pol_bkg", "pol_bkg", x, RooArgList(c1,c2));
    pol_bkg.fitTo(data,Range("R1,R3"));
    
    // Creare la gaussiana
    RooRealVar mean("mean", "Media gaussiana", (up+down)/2, down, up);
    RooRealVar sigma("#sigma_{1}", "Deviazione standard gaussiana", 0.02, 0.001, 0.3);
    RooGaussian voigt_pdf("voigt_pdf", "Signal Gaussian PDF", x, mean, sigma);

    // Creare la gaussiana
    RooRealVar mean2("mean2", "Media gaussiana2", (up+down)/2, down, up);
    RooRealVar sigma2("#sigma_{2}", "Deviazione standard gaussiana2", 0.05, 0.001, 0.3);
    RooGaussian voigt_pdf2("voigt_pdf2", "Signal Gaussian PDF2", x, mean, sigma2);
    
    // Creare il modello di fit combinando fondo e gaussiana
    RooRealVar nsig("nsig_G1", "Numero di segnali", 0.25e+07, 1e+05, 1e+07);
    RooRealVar nsig2("nsig_G2", "Numero di segnali2", 0.25e+07, 1e+05, 1e+07);
    RooRealVar nbkg("nbkg", "Numero di background",0.5e+07, 1e+05, 1e+07);

    RooAddPdf model("model", "Signal + Background", RooArgList(voigt_pdf, voigt_pdf2, pol_bkg), RooArgList(nsig, nsig2, nbkg));

    RooFitResult *result = model.fitTo(data, Save(true));
    
    RooPlot *frame = x.frame();
    data.plotOn(frame);
    model.plotOn(frame, Components(voigt_pdf, voigt_pdf2), LineStyle(kDashed), LineColor(kRed));
    model.paramOn(frame, Parameters(RooArgSet(nsig, nsig2, nbkg, mean, sigma, sigma2, c1, c2)), Layout(0.1,0.4,0.9));
    model.plotOn(frame, Components(pol_bkg), LineStyle(kDashed), LineColor(kGreen));
    model.plotOn(frame);
    
    TCanvas *canvas = new TCanvas("canvas", "Fit Result", 900, 600);
    frame->Draw();
    //canvas->SaveAs("Fit_results/Fit_BsJPsiPhi.png");

    // Chiudere il file
    //file->Close();
}

void Fit2mu2KM(TString dataFile="../Analysis/FinalFiles_B2mu2K/Analyzed_Data_B2mu2K_2022.root", TString var="Quadruplet_Mass", float down=4.9, float up=5.6) {
    // Aprire il file root contenente l'albero
    TFile *file = new TFile(dataFile);
    if (!file || file->IsZombie()) {
        std::cerr << "Errore nell'apertura del file" << std::endl;
        return;
    }

    // Ottenere l'albero dal file
    TTree *tree = (TTree*)file->Get("FinalTree");
    if (!tree) {
        std::cerr << "Errore nell'apertura dell'albero" << std::endl;
        file->Close();
        return;
    }
    TString s;
    s.Form(">>h1(80,%f,%f)", down, up);
    tree->Draw(var+s, "abs(Ditrk_mass-1.01945)<0.0135 && abs(Dimu_mass-3.0969)<0.09");
    TH1F *h1 = (TH1F*)gDirectory->Get("h1");
      
    RooRealVar x(var, var, down, up);
    x.setBins(80);
    
    RooDataHist data("data", h1->GetTitle(), RooArgSet(x), Import(*h1, kFALSE));
    x.setRange("R1", 4.9, 5.2);
    x.setRange("R2", 5.2, 5.4);
    x.setRange("R3", 5.5, 5.6);
    
    // Creare il fondo
    RooRealVar c1("c1", "c1", -0.2, -10, 10);
    RooRealVar c2("c2", "c2", -0.2, -10, 10);
    RooRealVar c3("c3", "c3", -0.2, -10, 10);
    
    //RooExponential pol_bkg("pol_bkg", "pol_bkg", x, c1);
    RooChebychev pol_bkg("pol_bkg", "pol_bkg", x, RooArgList(c1,c2));
    pol_bkg.fitTo(data,Range("R1,R3"));
    
    // Creare la gaussiana
    RooRealVar mean("mean", "Media gaussiana", (up+down)/2, down, up);
    RooRealVar sigma("#sigma_{1}", "Deviazione standard gaussiana", 0.02, 0.001, 0.2);
    RooGaussian voigt_pdf("voigt_pdf", "Signal Gaussian PDF", x, mean, sigma);

    // Creare la gaussiana
    RooRealVar mean2("mean2", "Media gaussiana2", (up+down)/2, down, up);
    RooRealVar sigma2("#sigma_{2}", "Deviazione standard gaussiana2", 0.05, 0.001, 0.2);
    RooGaussian voigt_pdf2("voigt_pdf2", "Signal Gaussian PDF2", x, mean, sigma2);
    
    // Creare il modello di fit combinando fondo e gaussiana
    RooRealVar nsig("nsig_G1", "Numero di segnali", 0.25e+06, 1e+04, 1e+06);
    RooRealVar nsig2("nsig_G2", "Numero di segnali2", 0.25e+06, 1e+04, 1e+06);
    RooRealVar nbkg("nbkg", "Numero di background",0.6e+06, 1e+04, 1e+06);

    RooAddPdf model("model", "Signal + Background", RooArgList(voigt_pdf, voigt_pdf2, pol_bkg), RooArgList(nsig, nsig2, nbkg));

    RooFitResult *result = model.fitTo(data, Save(true));
    
    RooPlot *frame = x.frame();
    data.plotOn(frame);
    model.plotOn(frame, Components(voigt_pdf, voigt_pdf2), LineStyle(kDashed), LineColor(kRed));
    model.paramOn(frame, Parameters(RooArgSet(nsig, nsig2, nbkg, mean, sigma, sigma2, c1, c2)), Layout(0.1,0.4,0.9));
    model.plotOn(frame, Components(pol_bkg), LineStyle(kDashed), LineColor(kGreen));
    model.plotOn(frame);
    
    TCanvas *canvas = new TCanvas("canvas", "Fit Result", 900, 600);
    frame->Draw();
    //canvas->SaveAs("Fit_results/Fit_BsJPsiPhi.png");

    // Chiudere il file
    //file->Close();
}
