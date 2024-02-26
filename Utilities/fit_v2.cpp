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

void fit_v2() {
    // Aprire il file root contenente l'albero
    TFile *file = new TFile("../Analysis/FinalFiles/Analyzed_Data_2022_v2.root");
    if (!file || file->IsZombie()) {
        std::cerr << "Errore nell'apertura del file" << std::endl;
        return;
    }

    // Ottenere l'albero dal file
    TTree *tree = (TTree*)file->Get("OutputTree");
    if (!tree) {
        std::cerr << "Errore nell'apertura dell'albero" << std::endl;
        file->Close();
        return;
    }
    
    //tree->Draw("Quadruplet_Mass>>h1(24,5.05,5.65)","((BsJPsiPhi_sel_OS1>0 && Dimu_OS1_dR>0.17 && Dimu_OS1_dR<1.08) || (BsJPsiPhi_sel_OS2>0 && Dimu_OS2_dR>0.17 && Dimu_OS2_dR<1.08)) && FlightDistBS_SV_Significance>2.25 ");
    tree->Draw("Quadruplet_Mass_eq>>h1(52,5.0, 6.0)","(isJPsiPhi==1)");
    TH1F *h1 = (TH1F*)gDirectory->Get("h1");
    
    RooRealVar x("Quadruplet_Mass_eq", "Quadruplet_Mass_eq", 5.0, 6.0);
    x.setRange("R1", 5.0, 5.25);
    x.setRange("R2", 5.55, 6.0);
    x.setRange("RT", 5.0, 6.0);
    x.setBins(52);
    
    RooDataHist data("data", h1->GetTitle(), RooArgSet(x), Import(*h1, kFALSE));
    
    
    // Creare il fondo
    RooRealVar gamma("#Gamma", "Gamma", -0.2, -10, 10);
    RooExponential exp_bkg("exp_bkg", "exp_bkg", x, gamma);
    exp_bkg.fitTo(data,Range("R1,R2"));

    // Creare la gaussiana
    RooRealVar mean("mean", "Media gaussiana", 5.367, 5.33, 5.40);
    RooRealVar sigma("sigma", "Deviazione standard gaussiana", 0.01, 0.001, 0.04);
    RooGaussian gauss_pdf("gauss_pdf", "Signal Gaussian PDF", x, mean, sigma);
    
    // Creare la gaussiana N2
    RooRealVar sigma2("sigma2", "Deviazione standard gaussiana 2", 0.02, 0.001, 0.04);
    RooGaussian gauss_pdf2("gauss_pdf2", "Signal Gaussian PDF", x, mean, sigma2);
    
    // Creare il modello di fit combinando fondo e gaussiana
    RooRealVar nsig("nsig", "Numero di segnali", 40, 10, 1000);
    RooRealVar nsig2("nsig2", "Numero di segnali 2", 20, 10, 1000);
    RooRealVar nbkg("nbkg", "Numero di background", 320, 40, 2000);
    

    RooAddPdf model("model", "Signal + Background", RooArgList(gauss_pdf, gauss_pdf2,  exp_bkg), RooArgList(nsig, nsig2, nbkg));

    RooFitResult *result = model.fitTo(data, Save(true), Range("RT"));
    
    RooPlot *frame = x.frame();
    data.plotOn(frame);
    model.plotOn(frame, Components(gauss_pdf), LineStyle(kDashed), LineColor(kRed));
    model.paramOn(frame, Parameters(RooArgSet(nsig, nsig2, nbkg, mean, sigma, sigma2, gamma)), Layout(0.6,0.9,0.9));
    model.plotOn(frame, Components(exp_bkg), LineStyle(kDashed), LineColor(kGreen));
    model.plotOn(frame);
    
    TCanvas *canvas = new TCanvas("canvas", "Fit Result", 900, 600);
    frame->Draw();
    //canvas->SaveAs("Fit_results/Fit_BsJPsiPhi.png");

    // Chiudere il file
    //file->Close();
}
