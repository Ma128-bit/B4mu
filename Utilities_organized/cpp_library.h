#include <iostream>
#include <ROOT/RDataFrame.hxx>
#include <ROOT/RVec.hxx>
#include <ROOT/RDF/RInterface.hxx>
#include <TFile.h>
#include <TMath.h>
#include <TTree.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TString.h>
#include <TH2F.h>

double N0_Bd22 = 6015075;
double N0_Bs22 = 5726775;
double N0_BsJPsiPhi22 = 6092393;

double N0_Bd23 = 5974440;
double N0_Bs23 = 5859938;
double N0_BsJPsiPhi23 = 6114171;

double N_Bd22 = 141697;
double N_Bs22 = 132403;
double N_BsJPsiPhi22 = 1118915;

double N_Bd23 = 136391;
double N_Bs23 = 130347;
double N_BsJPsiPhi23 = 1085347;

double ANAeff_Bd22 = N_Bd22/N0_Bd22;
double ANAeff_Bs22 = N_Bs22/N0_Bs22;
double ANAeff_BsJPsiPhi22 = N_BsJPsiPhi22/N0_BsJPsiPhi22;

double ANAeff_Bd23 = N_Bd23/N0_Bd23;
double ANAeff_Bs23 = N_Bs23/N0_Bs23;
double ANAeff_BsJPsiPhi23 = N_BsJPsiPhi23/N0_BsJPsiPhi23;

double lumi22_preEE = 7.9;
double lumi22_postEE = 26.5;
double lumi23_preBPix = 18.3;
double lumi23_postBPix = 9.4;

double GENeff_Bd22 = (0.063851845*lumi22_preEE + 0.064040337*lumi22_postEE)/(lumi22_preEE+lumi22_postEE);
double GENeff_Bs22 = (0.019179115*lumi22_preEE + 0.019273361*lumi22_postEE)/(lumi22_preEE+lumi22_postEE);
double GENeff_BsJPsiPhi22 = (0.002334313*lumi22_preEE + 0.002348375*lumi22_postEE)/(lumi22_preEE+lumi22_postEE);

double GENeff_Bd23 = (0.064558692*lumi23_preBPix + 0.064323076*lumi23_postBPix)/(lumi23_preBPix+lumi23_postBPix);
double GENeff_Bs23 = (0.018755007*lumi23_preBPix + 0.018755007*lumi23_postBPix)/(lumi23_preBPix+lumi23_postBPix);
double GENeff_BsJPsiPhi23 = (0.002306189*lumi23_preBPix + 0.002376499*lumi23_postBPix)/(lumi23_preBPix+lumi23_postBPix);

double BdBR = 1.0;
double BsBR = 1.0;
double BsJPsiPhiBR = 1.738e-8;

double NData_BsJPsiPhi = 159;
double NData_err_BsJPsiPhi = 14;

double NData22_BsJPsiPhi = NData_BsJPsiPhi*(lumi22_preEE+lumi22_postEE)/(lumi22_preEE+lumi22_postEE+lumi23_preBPix+lumi23_postBPix);
double NData22_err_BsJPsiPhi = NData_err_BsJPsiPhi*(lumi22_preEE+lumi22_postEE)/(lumi22_preEE+lumi22_postEE+lumi23_preBPix+lumi23_postBPix);

double NData23_BsJPsiPhi = NData_BsJPsiPhi*(lumi23_preBPix+lumi23_postBPix)/(lumi22_preEE+lumi22_postEE+lumi23_preBPix+lumi23_postBPix);
double NData23_err_BsJPsiPhi = NData_err_BsJPsiPhi*(lumi23_preBPix+lumi23_postBPix)/(lumi22_preEE+lumi22_postEE+lumi23_preBPix+lumi23_postBPix);

struct add_index{
    int i;
    add_index(int ii) : i(ii)  {}
    int operator()() {
        return i;
    }
};


double weight_to_new_ctau(double old_ctau, double new_ctau, double ct){
    /*
    Returns an event weight based on the ratio of the normalised lifetime distributions.
    old_ctau: ctau used for the sample production
    new_ctau: target ctau
    ct      : per-event lifetime
    */
    double weight = old_ctau / new_ctau * TMath::Exp((1.0 / old_ctau - 1.0 / new_ctau) * ct);
    return weight;
}

struct add_new_ctau{
    double old_ctau, new_ctau;
    add_new_ctau(double oldct, double newct) : old_ctau(oldct), new_ctau(newct)  {}
    float operator()(const TString& ID, const double cts, const double ctc) {
        if (ID.Contains("Bs202") || ID.Contains("Bd202")) {return weight_to_new_ctau(old_ctau, new_ctau, cts);} 
        else if (ID.Contains("BsJPsiPhi202")) {return weight_to_new_ctau(old_ctau, new_ctau, ctc);}
        else if (ID.Contains("Data")) {return 1;}
        else return 1;
    }
};

TString add_ID(unsigned int slot, const ROOT::RDF::RSampleInfo &id){
    //std::cout<<"id: "<<id.AsString()<<std::endl;
    if(id.Contains("Analyzed_MC_Bs_4mu_2022")) return "Bs2022";
    if(id.Contains("Analyzed_MC_Bs_4mu_2023")) return "Bs2023";
    if(id.Contains("Analyzed_MC_Bd_4mu_2022")) return "Bd2022";
    if(id.Contains("Analyzed_MC_Bd_4mu_2023")) return "Bd2023";
    if(id.Contains("Analyzed_MC_BsJPsiPhi_2022")) return "BsJPsiPhi2022";
    if(id.Contains("Analyzed_MC_BsJPsiPhi_2022")) return "BsJPsiPhi2023";
    
    if(id.Contains("Analyzed_Data_B4mu_2022")) return "Data22";
    if(id.Contains("Analyzed_Data_B4mu_2023")) return "Data23";
    else return "None";
}

int redef_isMC(unsigned int slot, const ROOT::RDF::RSampleInfo &id){
    //std::cout<<"id: "<<id.AsString()<<std::endl;
    if(id.Contains("Analyzed_MC_Bs_4mu_2022")) return 1;
    if(id.Contains("Analyzed_MC_Bs_4mu_2023")) return 1;
    if(id.Contains("Analyzed_MC_Bd_4mu_2022")) return 2;
    if(id.Contains("Analyzed_MC_Bd_4mu_2023")) return 2;
    if(id.Contains("Analyzed_MC_BsJPsiPhi_2022")) return 3;
    if(id.Contains("Analyzed_MC_BsJPsiPhi_2022")) return 3;
    
    if(id.Contains("Analyzed_Data_B4mu_2022")) return 0;
    if(id.Contains("Analyzed_Data_B4mu_2023")) return 0;
    else return -1;
}


double add_weight(unsigned int slot, const ROOT::RDF::RSampleInfo &id){
    //cout<<((BsBR*GENeff_Bs22*ANAeff_Bs22)/(BsJPsiPhiBR*GENeff_BsJPsiPhi22*ANAeff_BsJPsiPhi22))*NData22_BsJPsiPhi<<endl;
    //cout<<((BsBR*GENeff_Bs23*ANAeff_Bs23)/(BsJPsiPhiBR*GENeff_BsJPsiPhi23*ANAeff_BsJPsiPhi23))*NData23_BsJPsiPhi<<endl;
    if(id.Contains("Analyzed_MC_Bs_4mu_2022")) return ((BsBR*GENeff_Bs22*ANAeff_Bs22)/(BsJPsiPhiBR*GENeff_BsJPsiPhi22*ANAeff_BsJPsiPhi22))*NData22_BsJPsiPhi/N_Bs22;
    if(id.Contains("Analyzed_MC_Bs_4mu_2023")) return ((BsBR*GENeff_Bs23*ANAeff_Bs23)/(BsJPsiPhiBR*GENeff_BsJPsiPhi23*ANAeff_BsJPsiPhi23))*NData23_BsJPsiPhi/N_Bs23;
    if(id.Contains("Analyzed_MC_Bd_4mu_2022")) return ((BdBR*GENeff_Bd22*ANAeff_Bd22)/(BsJPsiPhiBR*GENeff_BsJPsiPhi22*ANAeff_BsJPsiPhi22))*NData22_BsJPsiPhi/N_Bd22;
    if(id.Contains("Analyzed_MC_Bd_4mu_2023")) return ((BdBR*GENeff_Bd23*ANAeff_Bd23)/(BsJPsiPhiBR*GENeff_BsJPsiPhi23*ANAeff_BsJPsiPhi23))*NData23_BsJPsiPhi/N_Bd23;
    if(id.Contains("Analyzed_MC_BsJPsiPhi_2022")) return 1;
    if(id.Contains("Analyzed_MC_BsJPsiPhi_2023")) return 1;
        
    if(id.Contains("Analyzed_Data_B4mu_")) return 1;
    else return -1;
}

double add_weight_err(unsigned int slot, const ROOT::RDF::RSampleInfo &id){
    //cout<<((BsBR*GENeff_Bs22*ANAeff_Bs22)/(BsJPsiPhiBR*GENeff_BsJPsiPhi22*ANAeff_BsJPsiPhi22))*NData22_err_BsJPsiPhi<<endl;
    //cout<<((BsBR*GENeff_Bs23*ANAeff_Bs23)/(BsJPsiPhiBR*GENeff_BsJPsiPhi23*ANAeff_BsJPsiPhi23))*NData23_err_BsJPsiPhi<<endl;
    if(id.Contains("Analyzed_MC_Bs_4mu_2022")) return ((BsBR*GENeff_Bs22*ANAeff_Bs22)/(BsJPsiPhiBR*GENeff_BsJPsiPhi22*ANAeff_BsJPsiPhi22))*NData22_err_BsJPsiPhi/N_Bs22;
    if(id.Contains("Analyzed_MC_Bs_4mu_2023")) return ((BsBR*GENeff_Bs23*ANAeff_Bs23)/(BsJPsiPhiBR*GENeff_BsJPsiPhi23*ANAeff_BsJPsiPhi23))*NData23_err_BsJPsiPhi/N_Bs23;
    if(id.Contains("Analyzed_MC_Bd_4mu_2022")) return ((BdBR*GENeff_Bd22*ANAeff_Bd22)/(BsJPsiPhiBR*GENeff_BsJPsiPhi22*ANAeff_BsJPsiPhi22))*NData22_err_BsJPsiPhi/N_Bd22;
    if(id.Contains("Analyzed_MC_Bd_4mu_2023")) return ((BdBR*GENeff_Bd23*ANAeff_Bd23)/(BsJPsiPhiBR*GENeff_BsJPsiPhi23*ANAeff_BsJPsiPhi23))*NData23_err_BsJPsiPhi/N_Bd23;
    if(id.Contains("Analyzed_MC_BsJPsiPhi_2022")) return 1;
    if(id.Contains("Analyzed_MC_BsJPsiPhi_2023")) return 1;
        
    if(id.Contains("Analyzed_Data_B4mu_")) return 1;
    else return -1;
}

double get_MuonSF(const TString& ID, const double pt, const double eta, TH2F* SF_pre, TH2F* SF_post){
    TH2F* SF = nullptr;
    if (ID.Contains("preE")) { SF = SF_pre;} 
    else if (ID.Contains("postE")) {SF = SF_post;}
    else if (ID.Contains("Data")) {return 1;}
    else return 1;
    if(pt>30) return 1;
    int ipt = SF->GetYaxis()->FindBin(pt);
    int ieta = SF->GetXaxis()->FindBin(std::abs(eta));
    return SF->GetBinContent(ieta, ipt);
}
double get_MuonSF_err(const TString& ID, const double pt, const double eta, TH2F* SF_pre, TH2F* SF_post){
    TH2F* SF = nullptr;
    if (ID.Contains("preE")) { SF = SF_pre;} 
    else if (ID.Contains("postE")) {SF = SF_post;}
    else if (ID.Contains("Data")) {return 0;}
    else return 0;
    if(pt>30) return 0;
    int ipt = SF->GetYaxis()->FindBin(pt);
    int ieta = SF->GetXaxis()->FindBin(std::abs(eta));
    return SF->GetBinError(ieta, ipt);
}
struct SF_WeightsComputer{
    TH2F *h2D_1;
    TH2F *h2D_2;
    bool flag;
    SF_WeightsComputer(TH2F *h1, TH2F *h2, bool f) : h2D_1(h1), h2D_2(h2), flag(f)  {}
    float operator()(const TString& ID, const double pt, const double eta) {
        if (!flag) return get_MuonSF(ID, pt, eta, h2D_1, h2D_2);
        else return get_MuonSF_err(ID, pt, eta, h2D_1, h2D_2);
    }
};
struct PV_WeightsComputer{
    std::vector<TString> name;
    std::vector<TH1F*> histo;
    bool flag;
    PV_WeightsComputer(std::vector<TString>& s, const std::vector<TH1F*>& histograms, bool f): name(s), histo(histograms), flag(f) {}
    float operator()(const TString& ID, const uint nPileUpInt) {
        auto it = std::find(name.begin(), name.end(), ID);
        if (it != name.end()) {
            int indx = std::distance(name.begin(), it);
            int nP = histo[indx]->GetXaxis()->FindBin(nPileUpInt);
            //std::cout<<name[indx]<<std::endl;
            if (!flag) { return histo[indx]->GetBinContent(nP);}
            else { return histo[indx]->GetBinError(nP);}
        } else {
            if (!flag) { return 1;}
            else { return 0;}
        }
    }
};