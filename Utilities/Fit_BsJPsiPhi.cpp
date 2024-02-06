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

using namespace RooFit;

void Fit_BsJPsiPhi() {
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

    // Creare una variabile RooRealVar dal ramo dell'albero
    RooRealVar x("Quadruplet_Mass", "Quadruplet_Mass", 5.12, 5.58);
    x.setRange("R1", 5.2, 5.25);
    x.setRange("R2", 5.45, 5.5);
    x.setRange("RT", 5.2, 5.5);
    
    RooRealVar BsJPsiPhi_sel_OS1("BsJPsiPhi_sel_OS1", "BsJPsiPhi_sel_OS1", 0, 2);
    RooRealVar BsJPsiPhi_sel_OS2("BsJPsiPhi_sel_OS2", "BsJPsiPhi_sel_OS2", 0, 2);

    // Creare un dataset RooDataSet dalla variabile x
    RooDataSet data("data", "Dataset", RooArgSet(x, BsJPsiPhi_sel_OS1, BsJPsiPhi_sel_OS2),
                        Import(*tree), Cut("(BsJPsiPhi_sel_OS1>0) || (BsJPsiPhi_sel_OS2>0)"));

    // Creare il fondo 
    RooRealVar gamma("#Gamma", "Gamma", -1, -2.0, -1e-2);
    RooExponential exp_bkg("exp_bkg", "exp_bkg", x, gamma);
    exp_bkg.fitTo(data,Range("R1,R2"));

    // Creare la gaussiana
    RooRealVar mean("mean", "Media gaussiana", 5.367, 5.33, 5.40);
    RooRealVar sigma("sigma", "Deviazione standard gaussiana", 0.01, 0.001, 0.2);
    RooGaussian gauss_pdf("gauss_pdf", "Signal Gaussian PDF", x, mean, sigma);

    // Creare il modello di fit combinando fondo e gaussiana
    RooRealVar nsig("nsig", "Numero di segnali", 140, 10, 300);
    RooRealVar nbkg("nbkg", "Numero di background", 200, 50000);
    
    RooAddPdf model("model", "Signal + Background", RooArgList(gauss_pdf, exp_bkg), RooArgList(nsig, nbkg));

    // Eseguire il fit
    RooFitResult *result = model.fitTo(data, Save(true), Range("RT"));

    // Visualizzare i risultati del fit
    result->Print();

    // Creare un frame RooPlot per visualizzare il risultato del fit
    RooPlot *frame = x.frame();
    data.plotOn(frame);
    //data.plotOn(frame);
    model.plotOn(frame);

    // Aggiungere la visualizzazione della gaussiana
    model.plotOn(frame, Components(gauss_pdf), LineStyle(kDashed), LineColor(kRed));

    // Aggiungere la visualizzazione dell'esponenziale
    model.plotOn(frame, Components(exp_bkg), LineStyle(kDashed), LineColor(kGreen));

    model.paramOn(frame, Parameters(RooArgSet(nsig,nbkg, mean, sigma, gamma )), Layout(0.2,0.6,0.5));

    // Creare un canvas per visualizzare il risultato del fit
    TCanvas *canvas = new TCanvas("canvas", "Fit Result", 800, 600);
    frame->Draw();
    //canvas->SaveAs("Fit_BsJPsiPhi.png");

    // Chiudere il file
    //file->Close();
}

