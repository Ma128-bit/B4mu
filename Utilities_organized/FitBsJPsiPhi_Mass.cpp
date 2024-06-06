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
namespace fs = std::filesystem;

void FitBsJPsiPhi_Mass(TString year="2022") {
    // Aprire il file root contenente l'albero
    TFile *file = new TFile("ROOTFiles/AllControl"+year+".root");
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

    fs::path dir_path = "BsJPsiPhi_MassFit";

    // Creare la directory
    try {
        if (fs::create_directory(dir_path)) {
            std::cout << "Diretory creata con successo: " << dir_path << std::endl;
        } else {
            std::cout << "La directory esiste già o non è stato possibile crearla." << std::endl;
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Errore: " << e.what() << std::endl;
    }
    
    //tree->Draw("Quadruplet_Mass_eq>>h1(52,5.0, 5.9)","isMC==0 && (isMedium[0]+isMedium[1]+isMedium[2]+isMedium[3]==4)");
    tree->Draw("Quadruplet_Mass_eq>>h1(52,5.0, 5.9)","isMC==0");
    TH1F *h1 = (TH1F*)gDirectory->Get("h1");
    
    RooRealVar x("Quadruplet_Mass_eq", "Quadruplet_Mass_eq", 5.0, 5.9);
    x.setRange("R1", 5.0, 5.25);
    x.setRange("R2", 5.55, 5.9);
    x.setRange("RT", 5.0, 5.9);
    x.setBins(52);
    
    RooDataHist data("data", h1->GetTitle(), RooArgSet(x), Import(*h1, kFALSE));
    
    
    // Creare il fondo
    RooRealVar gamma("#Gamma", "Gamma", -0.9, -10, 10);
    RooExponential exp_bkg("exp_bkg", "exp_bkg", x, gamma);
    exp_bkg.fitTo(data,Range("R1,R2"));

    RooRealVar mu("mu", "mu", 5.46, 5.1, 5.80);
    RooRealVar lambd("lambd", "lambd", 3.6, 2.0, 4.5);
    RooRealVar gamm("gamm", "gamm", 4.5, 3.0, 6.0);
    RooRealVar delta("delta", "delta", 175, 100, 300);
    RooJohnson gauss_pdf("signal_Bs", "signal_Bs", x, mu, lambd, gamm, delta);
         
    // Creare la gaussiana
    //RooRealVar mean("mean", "Media gaussiana", 5.367, 5.33, 5.40);
    //RooRealVar sigma("sigma", "Deviazione standard gaussiana", 0.02, 0.01, 0.06);
    //RooGaussian gauss_pdf("gauss_pdf", "Signal Gaussian PDF", x, mean, sigma);
        
    // Creare il modello di fit combinando fondo e gaussiana
    RooRealVar nsig("nsig", "Numero di segnali", 60, 40, 1000);
    RooRealVar nbkg("nbkg", "Numero di background", h1->GetEntries(), 40, 2*h1->GetEntries());
    

    RooAddPdf model("model", "Signal + Background", RooArgList(gauss_pdf,  exp_bkg), RooArgList(nsig, nbkg));

    RooFitResult *result = model.fitTo(data, Save(true), Range("RT"));
    
    RooPlot *frame = x.frame();
    data.plotOn(frame);
    model.plotOn(frame, Components(gauss_pdf), LineStyle(kDashed), LineColor(kRed));
    //model.paramOn(frame, Parameters(RooArgSet(nsig, nbkg, mean, sigma, gamma)), Layout(0.6,0.9,0.9));
    model.paramOn(frame, Parameters(RooArgSet(nsig, nbkg, mu, lambd, gamm ,delta , gamma)), Layout(0.6,0.9,0.9));
    model.plotOn(frame, Components(exp_bkg), LineStyle(kDashed), LineColor(kGreen));
    model.plotOn(frame);
    
    TCanvas *canvas = new TCanvas("canvas", "Fit Result", 900, 600);
    frame->Draw();
    canvas->SaveAs("BsJPsiPhi_MassFit/Fit_BsJPsiPhi_"+year+".png");

    canvas->Clear();
    canvas->Delete();
    // Chiudere il file
    file->Close();
}
