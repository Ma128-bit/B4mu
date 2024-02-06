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

void Fit_BsJPsiPhi() {
    std::string folderName = "Fit_results";
    if (!std::filesystem::exists(folderName)) {
        // Se la cartella non esiste, crea una nuova cartella
        if (std::filesystem::create_directory(folderName)) {
            std::cout << "Cartella creata con successo.\n";
        } else {
            std::cerr << "Errore durante la creazione della cartella.\n";
        }
    }
    // Aprire il file root contenente l'albero
    TFile *file = new TFile("../Analysis/FinalFiles/Analyzed_Data_All.root");
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
    
    tree->Draw("Quadruplet_Mass>>h1(36,5.05,5.65)","(BsJPsiPhi_sel_OS1>0) || (BsJPsiPhi_sel_OS2>0)");
    TH1F *h1 = (TH1F*)gDirectory->Get("h1");
    
    RooRealVar x("Quadruplet_Mass", "Quadruplet_Mass", 5.05, 5.65);
    x.setRange("R1", 5.05, 5.25);
    x.setRange("R2", 5.55, 5.65);
    x.setRange("RT", 5.05, 5.65);
    x.setBins(24);
    
    RooDataHist data("data", h1->GetTitle(), RooArgSet(x), Import(*h1, kFALSE));
    
    
    // Creare il fondo
    RooRealVar gamma("#Gamma", "Gamma", -0.2, -10, 10);
    RooExponential exp_bkg("exp_bkg", "exp_bkg", x, gamma);
    exp_bkg.fitTo(data,Range("R1,R2"));

    // Creare la gaussiana
    RooRealVar mean("mean", "Media gaussiana", 5.367, 5.33, 5.40);
    RooRealVar sigma("sigma", "Deviazione standard gaussiana", 0.01, 0.005, 0.2);
    RooGaussian gauss_pdf("gauss_pdf", "Signal Gaussian PDF", x, mean, sigma);
    
    // Creare il modello di fit combinando fondo e gaussiana
    RooRealVar nsig("nsig", "Numero di segnali", 140, 10, 300);
    RooRealVar nbkg("nbkg", "Numero di background", 320, 40, 500);

    RooAddPdf model("model", "Signal + Background", RooArgList(gauss_pdf, exp_bkg), RooArgList(nsig, nbkg));

    RooFitResult *result = model.fitTo(data, Save(true), Range("RT"));
    
    RooPlot *frame = x.frame();
    data.plotOn(frame);
    model.plotOn(frame, Components(gauss_pdf), LineStyle(kDashed), LineColor(kRed));
    model.paramOn(frame, Parameters(RooArgSet(nsig, nbkg, mean, sigma, gamma)), Layout(0.1,0.6,0.9));
    model.plotOn(frame, Components(exp_bkg), LineStyle(kDashed), LineColor(kGreen));
    model.plotOn(frame);
    
    TCanvas *canvas = new TCanvas("canvas", "Fit Result", 900, 600);
    frame->Draw();
    //canvas->SaveAs("Fit_results/Fit_BsJPsiPhi.png");

    // Chiudere il file
    //file->Close();
}
