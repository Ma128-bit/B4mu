#include "RooRealVar.h"
#include "RooStats/SPlot.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooExponential.h"
#include "RooChebychev.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooAddition.h"
#include "RooProduct.h"
#include "RooAbsPdf.h"
#include "RooFitResult.h"
#include "RooWorkspace.h"
#include "RooConstVar.h"
    
#include "TFile.h"
    
#include "TCanvas.h"
#include "TLegend.h"

#include <iomanip>

using namespace RooFit;
using namespace RooStats;

void AddModel(RooWorkspace &ws){
    RooRealVar xMass("RefittedSV_Mass_eq", "M_{inv}", 5.2, 5.7, "GeV");
    std::cout << "make Bs model" << std::endl;
    RooRealVar mu("mu", "mu", 5.366, 5.2, 5.7, "GeV");
    RooRealVar lambd("lambd", "lambd", 0.02, 0.001, 1.5);
    RooRealVar gamm("gamm", "gamm", 0.14, 0.01, 1.5);
    RooRealVar delta("delta", "delta", 1.45, 0.1, 10);
    RooJohnson mBsModel("mDsModel", "Ds Model", xMass, mu, lambd, gamm, delta);

    std::cout << "make bkg model" << std::endl;
    RooRealVar lambda("lambda", "lambda of Exponential", -0.01, -10, 10);
    RooExponential bkgModel("bkgModel", "Exponential", xMass, lambda);

    // --------------------------------------
    // combined model
    RooRealVar nsigBs("nsigBs", "fitted yield for Bs", 300000, 0., 2000000);
    RooRealVar nbkg("nbkg", "fitted yield for bkg", 10000, 0., 1000000);

    // now make the combined models
    std::cout << "make full model" << std::endl;
    RooAddPdf massModel("massModel", "invariant mass model", RooArgList(mBsModel, bkgModel), RooArgList(nsigBs, nbkg));
    
    std::cout << "import model" << std::endl;

    ws.import(xMass);
    ws.import(massModel, RecycleConflictNodes());
}

void AddMC_Model(RooWorkspace &ws){
    RooRealVar xMass("RefittedSV_Mass_eq", "M_{inv}", 5.2, 5.7, "GeV");
    std::cout << "make Bs model" << std::endl;
    RooRealVar mu("mu", "mu", 5.366, 5.2, 5.7, "GeV");
    RooRealVar lambd("lambd", "lambd", 0.02, 0.001, 1.5);
    RooRealVar gamm("gamm", "gamm", 0.14, 0.01, 1.5);
    RooRealVar delta("delta", "delta", 1.45, 0.1, 10);
    RooJohnson mBsModel("mDsModel", "Ds Model", xMass, mu, lambd, gamm, delta);

    std::cout << "make bkg model" << std::endl;
    RooRealVar lambda("lambda", "lambda of Exponential", -0.01, -10, 10);
    RooExponential bkgModel("bkgModel", "Exponential", xMass, lambda);

    // --------------------------------------
    // combined model
    RooRealVar nsigBs("nsigBs", "fitted yield for Bs", 300000, 0., 2000000);
    RooRealVar nbkg("nbkg", "fitted yield for bkg", 10000, 0., 1000000);

    // now make the combined models
    std::cout << "make full model" << std::endl;
    RooAddPdf massModel("massModel", "invariant mass model", RooArgList(mBsModel, bkgModel), RooArgList(nsigBs, nbkg));
    
    std::cout << "import model" << std::endl;
    ws.import(xMass);
    ws.import(massModel, RecycleConflictNodes());
}
void AddData(RooWorkspace &ws, TString name_file = "AllB2mu2K2022.root", TString tree_name = "FinalTree", TString selMC = "isMC==0"){
    TFile *file = new TFile("./ROOTFiles_07_01_25/"+name_file);
    TTree *tree = (TTree*)file->Get(tree_name);
    RooAbsPdf *model = ws.pdf("model");
    RooRealVar *xMass = ws.var("RefittedSV_Mass_eq");
    RooArgSet variables;
    for (const auto& branch : *tree->GetListOfBranches()) {
        TString branchName = branch->GetName();
        if (!branchName.Contains("__") && branchName!="") { // Assicurati di non includere le variabili aggiuntive (es: numero di eventi)
            RooRealVar* var = new RooRealVar(branchName.Data(), branchName.Data(), -9999999999999999999999999999., 9999999999999999999999999999.);
            variables.add(*var);
        }
    }
    variables.add(*xMass);
    //TString selez = selMC + " && (Ptmu3 > 1.2 && ((Ptmu1>3.5 && Etamu1<1.2) || (Ptmu1>2.0 && Etamu1>=1.2 && Etamu1<=2.4)) && ((Ptmu2>3.5 && Etamu2<1.2) || (Ptmu2>2.0 && Etamu2>=1.2 && Etamu2<=2.4)))";
    TString selez = selMC;
    RooDataSet dataset("dataset", "dataset", variables, Import(*tree),Cut(selez));//, WeightVar("control_weight"));

    ws.import(dataset, Rename("data"));
}

void DoSPlot(RooWorkspace &ws){
    std::cout << "Calculate sWeights" << std::endl;

    // get what we need out of the workspace to do the fit
    RooAbsPdf *massModel = ws.pdf("massModel");
    RooRealVar *nsigBs = ws.var("nsigBs");
    RooRealVar *nbkg = ws.var("nbkg");
    RooDataSet& data = static_cast<RooDataSet&>(*ws.data("data"));
    massModel->fitTo(data, Save(true), PrintLevel(-1));

    RooRealVar* mass = ws.var("RefittedSV_Mass_eq");
    RooPlot* frame = mass->frame(Title("Fit dei dati con il modello"));
    data.plotOn(frame, MarkerStyle(kFullCircle), MarkerColor(kBlack));
    massModel->plotOn(frame, LineColor(kBlue), LineWidth(2));
    TCanvas* c = new TCanvas("c", "Fit", 800, 600);
    frame->Draw();
    c->SaveAs("fit_result.png");

    RooMsgService::instance().setSilentMode(true);

    std::cout << "\n\n------------------------------------------\nThe dataset before creating sWeights:\n";
    data.Print();

    RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR);

    //RooStats::SPlot sData{"sData", "An SPlot", data, massModel, RooArgList(*nsigDs, *nsigDp, *nbkg)};
    RooStats::SPlot sData{"sData", "An SPlot", data, massModel, RooArgList(*nsigBs, *nbkg)};

    std::cout << "\n\nThe dataset after creating sWeights:\n";
    data.Print();

    // Check that our weights have the desired properties

    std::cout << "\n\n------------------------------------------\n\nCheck SWeights:" << std::endl;

    std::cout << std::endl << "Yield of Ds is\t" << nsigBs->getVal() << ".  From sWeights it is " << sData.GetYieldFromSWeight("nsigBs") << std::endl;
    
    //std::cout << "Yield of Dp is\t" << nsigDp->getVal() << ".  From sWeights it is " << sData.GetYieldFromSWeight("nsigDp") << std::endl << std::endl;

    std::cout << "Yield of bkg is\t" << nbkg->getVal() << ".  From sWeights it is " << sData.GetYieldFromSWeight("nbkg") << std::endl << std::endl;

    for (Int_t i = 0; i < 10; i++) {
        std::cout << "Bs Weight for event " << i << std::right << std::setw(12) << sData.GetSWeight(i, "nsigBs") << "  Bp Weight"
                << std::setw(12) << sData.GetSWeight(i, "nbkg") << "  Total Weight" << std::setw(12) << sData.GetSumOfEventSWeight(i)
                << std::endl;
    }

    std::cout << std::endl;

    // import this new dataset with sWeights
    std::cout << "import new dataset with sWeights" << std::endl;
    ws.import(data, Rename("dataWithSWeights"));

    RooMsgService::instance().setGlobalKillBelow(RooFit::INFO);
}

void DoSPlotMC(RooWorkspace &ws){
    std::cout << "Calculate sWeights" << std::endl;

    // get what we need out of the workspace to do the fit
    RooAbsPdf *massModel = ws.pdf("massModel");
    RooRealVar *nsigBs = ws.var("nsigBs");
    RooRealVar *nbkg = ws.var("nbkg");
    RooDataSet& data = static_cast<RooDataSet&>(*ws.data("data"));
    massModel->fitTo(data, Save(true), PrintLevel(-1));

    RooMsgService::instance().setSilentMode(true);

    std::cout << "\n\n------------------------------------------\nThe dataset before creating sWeights:\n";
    data.Print();

    RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR);

    RooStats::SPlot sData{"sData", "An SPlot", data, massModel, RooArgList(*nsigBs, *nbkg)};

    std::cout << "\n\nThe dataset after creating sWeights:\n";
    data.Print();

    // Check that our weights have the desired properties

    std::cout << "\n\n------------------------------------------\n\nCheck SWeights:" << std::endl;

    std::cout << std::endl << "Yield of Ds is\t" << nsigBs->getVal() << ".  From sWeights it is " << sData.GetYieldFromSWeight("nsigBs") << std::endl;
    
    std::cout << "Yield of bkg is\t" << nbkg->getVal() << ".  From sWeights it is " << sData.GetYieldFromSWeight("nbkg") << std::endl << std::endl;

    for (Int_t i = 0; i < 10; i++) {
        std::cout << "Bs Weight for event " << i << std::right << std::setw(12) << sData.GetSWeight(i, "nsigBs") << "  nbkg Weight"
                << std::setw(12) << sData.GetSWeight(i, "nbkg") << "  Total Weight" << std::setw(12) << sData.GetSumOfEventSWeight(i)
                << std::endl;
    }

    std::cout << std::endl;

    // import this new dataset with sWeights
    std::cout << "import new dataset with sWeights" << std::endl;
    ws.import(data, Rename("dataWithSWeights"));

    RooMsgService::instance().setGlobalKillBelow(RooFit::INFO);
}

void MakePlots(RooWorkspace &ws)
{
    std::cout << "make plots" << std::endl;

    TCanvas *cdata = new TCanvas("sPlot", "sPlot demo");
    cdata->Divide(1, 2);
    
    RooRealVar *xMass = ws.var("RefittedSV_Mass_eq");
    auto& data = static_cast<RooDataSet&>(*ws.data("dataWithSWeights"));

    // create weighted data sets
    RooDataSet dataBs_w{data.GetName(), data.GetTitle(), &data, *data.get(), nullptr, "nsigBs_sw"};
    RooDataSet databkg{data.GetName(), data.GetTitle(), &data, *data.get(), nullptr, "nbkg_sw"};

    cdata->cd(1);
    RooPlot *frame = xMass->frame(Title("Invaraint Mass after sPlot"));
    dataBs_w.plotOn(frame, DataError(RooAbsData::SumW2));
    frame->Draw();
    
    cdata->cd(2);
    RooPlot *frame3 = xMass->frame(Title("Invaraint Mass after sPlot"));
    databkg.plotOn(frame3, DataError(RooAbsData::SumW2));
    frame3->Draw();

    cdata->Draw();
    cdata->SaveAs("SPlot.png");
}

void BsJPsiPhi_sPlot(TString name_file = "AllB2mu2K2022", TString tree_name = "FinalTree", int isMC = 0){
    RooWorkspace wspace{"myWS"};
    TString selMC ="";
    if(isMC == 0) selMC = selMC + Form("isMC==%d", isMC); 
    else selMC = selMC + "isMC!=0";
    std::cout<<selMC<<std::endl;
    if(isMC == 0) AddModel(wspace);
    else AddMC_Model(wspace);
    AddData(wspace, name_file+".root", tree_name, selMC);
    if(isMC == 0) DoSPlot(wspace);
    else DoSPlotMC(wspace);
    MakePlots(wspace);
    const TTree *tree = wspace.data("dataWithSWeights")->GetClonedTree();

    std::vector<std::string>  branchNames;
    TFile *file = new TFile(name_file+"_sPlot_MC_"+Form("%d", isMC)+".root", "RECREATE");
    tree->Write(tree_name, TObject::kOverwrite);
    for (const auto& branch : *const_cast<TTree*>(tree)->GetListOfBranches()) {
        std::string branchName = branch->GetName();
        if (branchName!="nsigDp_sw" && branchName!="L_nsigDs") {
            branchNames.push_back(branchName);
        }
    }
    file->Close();
    /*
    ROOT::EnableImplicitMT(); // Tell ROOT you want to go parallel
    ROOT::RDataFrame d(tree_name, name_file+"_sPlot_MC_"+Form("%d", isMC)+".root"); 
    d.Snapshot(tree_name, name_file+"_sPlot_MC_"+Form("%d", isMC)+"_v2.root", branchNames);
    */
}
