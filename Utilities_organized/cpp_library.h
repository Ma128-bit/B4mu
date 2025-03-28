#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
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

std::map<std::string, std::string> readConfigFile(const std::string& filename) {
    std::map<std::string, std::string> config;
    std::ifstream file(filename);
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string key, value;

            if (std::getline(iss, key, '=') && std::getline(iss, value)) {
                config[key] = value;
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

    return config;
}

std::string config_file;

double N0_Bd22, N0_Bs22, N0_BsJPsiPhi22, N0_Bd23, N0_Bs23, N0_BsJPsiPhi23, N0_Bd24, N0_Bs24, N0_BsJPsiPhi24; 
double N_Bd22, N_Bs22, N_BsJPsiPhi22, N_Bd23, N_Bs23, N_BsJPsiPhi23, N_Bd24, N_Bs24, N_BsJPsiPhi24;

double ANAeff_Bd22, ANAeff_Bs22, ANAeff_BsJPsiPhi22, ANAeff_Bd23, ANAeff_Bs23, ANAeff_BsJPsiPhi23, ANAeff_Bd24, ANAeff_Bs24, ANAeff_BsJPsiPhi24;

double lumi22_preEE, lumi22_postEE, lumi23_preBPix, lumi23_postBPix, lumi24;
double GENeff_Bd22, GENeff_Bs22, GENeff_BsJPsiPhi22, GENeff_Bd23, GENeff_Bs23, GENeff_BsJPsiPhi23, GENeff_Bd24, GENeff_Bs24, GENeff_BsJPsiPhi24;

double BdBR = 1.0, BsBR = 1.0, BsJPsiPhiBR = 1.7668e-8; // old: 1.738e-8 --> new: 1.7668e-8
double NData_BsJPsiPhi, NData_err_BsJPsiPhi, NData22_BsJPsiPhi, NData22_err_BsJPsiPhi, NData23_BsJPsiPhi, NData23_err_BsJPsiPhi, NData24_BsJPsiPhi, NData24_err_BsJPsiPhi;

double NSidebans, NSignal;

double fs_fd_ratio = 0.222;
double fs_fd_ratio_unc = 0.009;

void loadInfo(const std::string& inputString){
    config_file = inputString;
    std::map<std::string, std::string> config = readConfigFile(config_file); 
    N0_Bd22 = std::stod(config["N0_Bd22"]);
    N0_Bs22 = std::stod(config["N0_Bs22"]);
    N0_BsJPsiPhi22 = std::stod(config["N0_BsJPsiPhi22"]);

    N0_Bd23 = std::stod(config["N0_Bd23"]);
    N0_Bs23 = std::stod(config["N0_Bs23"]);
    N0_BsJPsiPhi23 = std::stod(config["N0_BsJPsiPhi23"]);

    N0_Bd24 = std::stod(config["N0_Bd24"]);
    N0_Bs24 = std::stod(config["N0_Bs24"]);
    N0_BsJPsiPhi24 = std::stod(config["N0_BsJPsiPhi24"]);

    N_Bd22 = std::stod(config["N_Bd22"]);
    N_Bs22 = std::stod(config["N_Bs22"]);
    N_BsJPsiPhi22 = std::stod(config["N_BsJPsiPhi22"]);

    N_Bd23 = std::stod(config["N_Bd23"]);
    N_Bs23 = std::stod(config["N_Bs23"]);
    N_BsJPsiPhi23 = std::stod(config["N_BsJPsiPhi23"]);

    N_Bd24 = std::stod(config["N_Bd24"]);
    N_Bs24 = std::stod(config["N_Bs24"]);
    N_BsJPsiPhi24 = std::stod(config["N_BsJPsiPhi24"]);

    ANAeff_Bd22 = N_Bd22/N0_Bd22;
    ANAeff_Bs22 = N_Bs22/N0_Bs22;
    ANAeff_BsJPsiPhi22 = N_BsJPsiPhi22/N0_BsJPsiPhi22;

    ANAeff_Bd23 = N_Bd23/N0_Bd23;
    ANAeff_Bs23 = N_Bs23/N0_Bs23;
    ANAeff_BsJPsiPhi23 = N_BsJPsiPhi23/N0_BsJPsiPhi23;

    ANAeff_Bd24 = N_Bd24/N0_Bd24;
    ANAeff_Bs24 = N_Bs24/N0_Bs24;
    ANAeff_BsJPsiPhi24 = N_BsJPsiPhi24/N0_BsJPsiPhi24;

    //cout<<"ANAeff: Bd22: "<<ANAeff_Bd22<<" Bs22: "<<ANAeff_Bs22<<" BsJPsiPhi22: "<<ANAeff_BsJPsiPhi22<<endl;
    //cout<<"ANAeff: Bd23: "<<ANAeff_Bd23<<" Bs23: "<<ANAeff_Bs23<<" BsJPsiPhi23: "<<ANAeff_BsJPsiPhi23<<endl;

    lumi22_preEE = std::stod(config["lumi22_preEE"]);
    lumi22_postEE = std::stod(config["lumi22_postEE"]);
    lumi23_preBPix = std::stod(config["lumi23_preBPix"]);
    lumi23_postBPix = std::stod(config["lumi23_postBPix"]);
    lumi24 = std::stod(config["lumi24"]);

    GENeff_Bd22 = (0.063851845*lumi22_preEE + 0.064040337*lumi22_postEE)/(lumi22_preEE+lumi22_postEE);
    GENeff_Bs22 = (0.019179115*lumi22_preEE + 0.019273361*lumi22_postEE)/(lumi22_preEE+lumi22_postEE);
    GENeff_BsJPsiPhi22 = (0.002334313*lumi22_preEE + 0.002348375*lumi22_postEE)/(lumi22_preEE+lumi22_postEE);

    GENeff_Bd23 = (0.064558692*lumi23_preBPix + 0.064323076*lumi23_postBPix)/(lumi23_preBPix+lumi23_postBPix);
    GENeff_Bs23 = (0.018755007*lumi23_preBPix + 0.018755007*lumi23_postBPix)/(lumi23_preBPix+lumi23_postBPix);
    GENeff_BsJPsiPhi23 = (0.002306189*lumi23_preBPix + 0.002376499*lumi23_postBPix)/(lumi23_preBPix+lumi23_postBPix);

    GENeff_Bd24 = 0.0655984919886899;
    GENeff_Bs24 = 0.0163353367236862;
    GENeff_BsJPsiPhi24 = 0.00269996625042187;

    //From PYTHIA to real GEN eff (see AN, Sec 6)
    GENeff_Bd22 = GENeff_Bd22/0.676; 
    GENeff_Bs22 = GENeff_Bs22/0.189;
    GENeff_BsJPsiPhi22 = GENeff_BsJPsiPhi22/0.189;
    GENeff_Bd23 = GENeff_Bd23/0.676; 
    GENeff_Bs23 = GENeff_Bs23/0.189;
    GENeff_BsJPsiPhi23 = GENeff_BsJPsiPhi23/0.189;
    GENeff_Bd24 = GENeff_Bd24/0.676; 
    GENeff_Bs24 = GENeff_Bs24/0.189;
    GENeff_BsJPsiPhi24 = GENeff_BsJPsiPhi24/0.189;

    //cout<<"GENeff: Bd22: "<<GENeff_Bd22<<" Bs22: "<<GENeff_Bs22<<" BsJPsiPhi22: "<<GENeff_BsJPsiPhi22<<endl;
    //cout<<"GENeff: Bd23: "<<GENeff_Bd23<<" Bs23: "<<GENeff_Bs23<<" BsJPsiPhi23: "<<GENeff_BsJPsiPhi23<<endl;

    NData_BsJPsiPhi = 1;
    NData_err_BsJPsiPhi = 1;

    NData22_BsJPsiPhi = NData_BsJPsiPhi*(lumi22_preEE+lumi22_postEE)/(lumi22_preEE+lumi22_postEE+lumi23_preBPix+lumi23_postBPix+lumi24);
    NData22_err_BsJPsiPhi = NData_err_BsJPsiPhi*(lumi22_preEE+lumi22_postEE)/(lumi22_preEE+lumi22_postEE+lumi23_preBPix+lumi23_postBPix+lumi24);

    NData23_BsJPsiPhi = NData_BsJPsiPhi*(lumi23_preBPix+lumi23_postBPix)/(lumi22_preEE+lumi22_postEE+lumi23_preBPix+lumi23_postBPix+lumi24);
    NData23_err_BsJPsiPhi = NData_err_BsJPsiPhi*(lumi23_preBPix+lumi23_postBPix)/(lumi22_preEE+lumi22_postEE+lumi23_preBPix+lumi23_postBPix+lumi24);

    NData24_BsJPsiPhi = NData_BsJPsiPhi*(lumi24)/(lumi22_preEE+lumi22_postEE+lumi23_preBPix+lumi23_postBPix+lumi24);
    NData24_err_BsJPsiPhi = NData_err_BsJPsiPhi*(lumi24)/(lumi22_preEE+lumi22_postEE+lumi23_preBPix+lumi23_postBPix+lumi24);

    NSidebans = std::stod(config["NSidebans"]);
    NSignal = std::stod(config["NSignal"]);
}

struct add_index{
    int i;
    add_index(int ii) : i(ii)  {}
    int operator()() {
        return i;
    }
};

double add_wDMC(unsigned int slot, const ROOT::RDF::RSampleInfo &id){//weigth Number ot Data sidebands / Number of MC signal --> to have the seme number of events
    double out = NSidebans/NSignal;
    if(id.Contains("Analyzed_MC_Bs_4mu_202")) return out;
    if(id.Contains("Analyzed_MC_Bd_4mu_202")) return out;
    if(id.Contains("Analyzed_MC_BsJPsiPhi_202")) return 1;    
    if(id.Contains("Analyzed_Data_B4mu_202")) return 1;
    else return -1;
}

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
        if (ID.Contains("Bs202")) {return weight_to_new_ctau(old_ctau, new_ctau, cts);} 
        else if (ID.Contains("BsJPsiPhi202")) {return weight_to_new_ctau(old_ctau, new_ctau, ctc);}
        else if (ID.Contains("Data") || ID.Contains("Bd202")) {return 1;}
        else return 1;
    }
};

TString add_ID(unsigned int slot, const ROOT::RDF::RSampleInfo &id){
    //std::cout<<"N0_Bd22: "<<N0_Bd22<<std::endl;
    //std::cout<<"id: "<<id.AsString()<<std::endl;
    if(id.Contains("Analyzed_MC_Bs_4mu_2022")) return "Bs2022";
    if(id.Contains("Analyzed_MC_Bs_4mu_2023")) return "Bs2023";
    if(id.Contains("Analyzed_MC_Bs_4mu_2024")) return "Bs2024";

    if(id.Contains("Analyzed_MC_Bd_4mu_2022")) return "Bd2022";
    if(id.Contains("Analyzed_MC_Bd_4mu_2023")) return "Bd2023";
    if(id.Contains("Analyzed_MC_Bd_4mu_2024")) return "Bd2024";

    if(id.Contains("Analyzed_MC_BsJPsiPhi_2022")) return "BsJPsiPhi2022";
    if(id.Contains("Analyzed_MC_BsJPsiPhi_2023")) return "BsJPsiPhi2023";
    if(id.Contains("Analyzed_MC_BsJPsiPhi_2024")) return "BsJPsiPhi2024";
    
    if(id.Contains("Analyzed_Data_B4mu_2022")) return "Data22";
    if(id.Contains("Analyzed_Data_B4mu_2023")) return "Data23";
    if(id.Contains("Analyzed_Data_B4mu_2024")) return "Data24";
    else return "None";
}

int redef_isMC(unsigned int slot, const ROOT::RDF::RSampleInfo &id){
    //std::cout<<"id: "<<id.AsString()<<std::endl;
    if(id.Contains("Analyzed_MC_Bs_4mu_2022")) return 1;
    if(id.Contains("Analyzed_MC_Bs_4mu_2023")) return 1;
    if(id.Contains("Analyzed_MC_Bs_4mu_2024")) return 1;

    if(id.Contains("Analyzed_MC_Bd_4mu_2022")) return 2;
    if(id.Contains("Analyzed_MC_Bd_4mu_2023")) return 2;
    if(id.Contains("Analyzed_MC_Bd_4mu_2024")) return 2;

    if(id.Contains("Analyzed_MC_BsJPsiPhi_2022")) return 3;
    if(id.Contains("Analyzed_MC_BsJPsiPhi_2023")) return 3;
    if(id.Contains("Analyzed_MC_BsJPsiPhi_2024")) return 3;

    if(id.Contains("Analyzed_Data_B4mu_2022")) return 0;
    if(id.Contains("Analyzed_Data_B4mu_2023")) return 0;
    if(id.Contains("Analyzed_Data_B4mu_2024")) return 0;
    else return -1;
}

double add_weight(unsigned int slot, const ROOT::RDF::RSampleInfo &id){
    //cout<<((GENeff_Bs22*ANAeff_Bs22)/(GENeff_BsJPsiPhi22*ANAeff_BsJPsiPhi22))<<endl;
    //cout<<((BsBR*GENeff_Bs23*ANAeff_Bs23)/(BsJPsiPhiBR*GENeff_BsJPsiPhi23*ANAeff_BsJPsiPhi23))*NData23_BsJPsiPhi<<endl;
    if(id.Contains("Analyzed_MC_Bs_4mu_2022")) return ((BsBR*GENeff_Bs22*ANAeff_Bs22)/(BsJPsiPhiBR*GENeff_BsJPsiPhi22*ANAeff_BsJPsiPhi22))*NData22_BsJPsiPhi/(1.0);
    if(id.Contains("Analyzed_MC_Bs_4mu_2023")) return ((BsBR*GENeff_Bs23*ANAeff_Bs23)/(BsJPsiPhiBR*GENeff_BsJPsiPhi23*ANAeff_BsJPsiPhi23))*NData23_BsJPsiPhi/(1.0);
    if(id.Contains("Analyzed_MC_Bs_4mu_2024")) return ((BsBR*GENeff_Bs24*ANAeff_Bs24)/(BsJPsiPhiBR*GENeff_BsJPsiPhi24*ANAeff_BsJPsiPhi24))*NData24_BsJPsiPhi/(1.0);
    
    if(id.Contains("Analyzed_MC_Bd_4mu_2022")) return ((BdBR*GENeff_Bd22*ANAeff_Bd22)/(BsJPsiPhiBR*GENeff_BsJPsiPhi22*ANAeff_BsJPsiPhi22))*(NData22_BsJPsiPhi/(1.0))*(1/fs_fd_ratio);
    if(id.Contains("Analyzed_MC_Bd_4mu_2023")) return ((BdBR*GENeff_Bd23*ANAeff_Bd23)/(BsJPsiPhiBR*GENeff_BsJPsiPhi23*ANAeff_BsJPsiPhi23))*(NData23_BsJPsiPhi/(1.0))*(1/fs_fd_ratio);
    if(id.Contains("Analyzed_MC_Bd_4mu_2024")) return ((BdBR*GENeff_Bd24*ANAeff_Bd24)/(BsJPsiPhiBR*GENeff_BsJPsiPhi24*ANAeff_BsJPsiPhi24))*(NData24_BsJPsiPhi/(1.0))*(1/fs_fd_ratio);

    if(id.Contains("Analyzed_MC_BsJPsiPhi_2022")) return 1;
    if(id.Contains("Analyzed_MC_BsJPsiPhi_2023")) return 1;
    if(id.Contains("Analyzed_MC_BsJPsiPhi_2024")) return 1;
        
    if(id.Contains("Analyzed_Data_B4mu_")) return 1;
    else return -1;
}

double add_weight_err(unsigned int slot, const ROOT::RDF::RSampleInfo &id){
    //cout<<((BsBR*GENeff_Bs22*ANAeff_Bs22)/(BsJPsiPhiBR*GENeff_BsJPsiPhi22*ANAeff_BsJPsiPhi22))*NData22_err_BsJPsiPhi<<endl;
    //cout<<((BsBR*GENeff_Bs23*ANAeff_Bs23)/(BsJPsiPhiBR*GENeff_BsJPsiPhi23*ANAeff_BsJPsiPhi23))*NData23_err_BsJPsiPhi<<endl;
    if(id.Contains("Analyzed_MC_Bs_4mu_2022")) return ((BsBR*GENeff_Bs22*ANAeff_Bs22)/(BsJPsiPhiBR*GENeff_BsJPsiPhi22*ANAeff_BsJPsiPhi22))*NData22_err_BsJPsiPhi/(1.0);
    if(id.Contains("Analyzed_MC_Bs_4mu_2023")) return ((BsBR*GENeff_Bs23*ANAeff_Bs23)/(BsJPsiPhiBR*GENeff_BsJPsiPhi23*ANAeff_BsJPsiPhi23))*NData23_err_BsJPsiPhi/(1.0);
    if(id.Contains("Analyzed_MC_Bs_4mu_2024")) return ((BsBR*GENeff_Bs24*ANAeff_Bs24)/(BsJPsiPhiBR*GENeff_BsJPsiPhi24*ANAeff_BsJPsiPhi24))*NData24_err_BsJPsiPhi/(1.0);

    if(id.Contains("Analyzed_MC_Bd_4mu_2022")) return ((BdBR*GENeff_Bd22*ANAeff_Bd22)/(BsJPsiPhiBR*GENeff_BsJPsiPhi22*ANAeff_BsJPsiPhi22))*(NData22_err_BsJPsiPhi/(1.0))*(1/fs_fd_ratio);
    if(id.Contains("Analyzed_MC_Bd_4mu_2023")) return ((BdBR*GENeff_Bd23*ANAeff_Bd23)/(BsJPsiPhiBR*GENeff_BsJPsiPhi23*ANAeff_BsJPsiPhi23))*(NData23_err_BsJPsiPhi/(1.0))*(1/fs_fd_ratio);
    if(id.Contains("Analyzed_MC_Bd_4mu_2024")) return ((BdBR*GENeff_Bd24*ANAeff_Bd24)/(BsJPsiPhiBR*GENeff_BsJPsiPhi24*ANAeff_BsJPsiPhi24))*(NData24_err_BsJPsiPhi/(1.0))*(1/fs_fd_ratio);

    if(id.Contains("Analyzed_MC_BsJPsiPhi_2022")) return 1;
    if(id.Contains("Analyzed_MC_BsJPsiPhi_2023")) return 1;
    if(id.Contains("Analyzed_MC_BsJPsiPhi_2024")) return 1;
        
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

double NewMassEqation(double OS1v1_mass, double OS2v1_mass, double OS1v2_mass, double OS2v2_mass, int category, double mass){
    std::map<std::string, std::vector<std::vector<double>>> cuts = {
        {"Jpsi", {{75., 60.}, {110., 85.}, {140., 110.}}},
        {"phi", {{30., 30.}, {50., 35.}, {55., 44.}}}
    };
    double diff1, diff2, diff1v2, diff2v2;

    if (((1.019461-OS1v1_mass)>-(cuts["phi"][category][1]/1000)) && ((1.019461-OS1v1_mass)<(cuts["phi"][category][0]/1000)) && ((3.0969-OS2v1_mass)<(cuts["Jpsi"][category][0]/1000)) && ((3.0969-OS2v1_mass)>-(cuts["Jpsi"][category][1]/1000))){
        diff1 = 1.019461-OS1v1_mass + 3.0969-OS2v1_mass;
    }
    else {diff1= 99.;}
    
    if (((1.019461-OS2v1_mass)>-(cuts["phi"][category][1]/1000)) && ((1.019461-OS2v1_mass)<(cuts["phi"][category][0]/1000)) && ((3.0969-OS1v1_mass)<(cuts["Jpsi"][category][0]/1000)) && ((3.0969-OS1v1_mass)>-(cuts["Jpsi"][category][1]/1000))) {
        diff2 = 1.019461-OS2v1_mass + 3.0969-OS1v1_mass;
    }
    else {diff2= 99.;}

    if (((1.019461-OS1v2_mass)>-(cuts["phi"][category][1]/1000)) && ((1.019461-OS1v2_mass)<(cuts["phi"][category][0]/1000)) && ((3.0969-OS2v2_mass)<(cuts["Jpsi"][category][0]/1000)) && ((3.0969-OS2v2_mass)>-(cuts["Jpsi"][category][1]/1000))){
        diff1v2 = 1.019461-OS1v2_mass + 3.0969-OS2v2_mass;
    }
    else {diff1v2= 99.;}
    
    if (((1.019461-OS2v2_mass)>-(cuts["phi"][category][1]/1000)) && ((1.019461-OS2v2_mass)<(cuts["phi"][category][0]/1000)) && ((3.0969-OS1v2_mass)<(cuts["Jpsi"][category][0]/1000)) && ((3.0969-OS1v2_mass)>-(cuts["Jpsi"][category][1]/1000))) {
        diff2v2 = 1.019461-OS2v2_mass + 3.0969-OS1v2_mass;
    }
    else {diff2v2= 99.;}

    double min_ = 120;
    if(diff1<min_) { min_ = diff1;}
    if(diff2<min_) { min_ = diff2;}
    if(diff1v2<min_) { min_ = diff1v2;}
    if(diff2v2<min_) { min_ = diff2v2;}

    if(min_!=99) { return mass + min_;}
    else {return -99;}
}
