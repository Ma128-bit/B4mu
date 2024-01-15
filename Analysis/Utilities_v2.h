#include <iostream>
#include <vector>
#include <algorithm>
#include <ROOT/RDataFrame.hxx>
#include <ROOT/RVec.hxx>
#include <ROOT/RDF/RInterface.hxx>
#include <TFile.h>
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

double deltaR(float eta1, float eta2, float phi1, float phi2){
    auto dp = std::abs(phi1 - phi2);
    auto deta = std::abs(eta1 - eta2);
    if (dp > float(M_PI))
        dp -= float(2 * M_PI);
    Double_t n = TMath::Sqrt(dp*dp + deta*deta);
    return n;
}

Bool_t isPairDeltaRGood(ROOT::VecOps::RVec<float> MuonEta, ROOT::VecOps::RVec<float> MuonPhi, vector<int> index, double DeltaRmax){
    // The function returns 'true' if all of the 3 possible pairs of muons have dR<DeltaRmax
    Double_t dR12 = deltaR(MuonEta.at(index.at(0)), MuonEta.at(index.at(1)), MuonPhi.at(index.at(0)), MuonPhi.at(index.at(1)));
    Double_t dR13 = deltaR(MuonEta.at(index.at(0)), MuonEta.at(index.at(2)), MuonPhi.at(index.at(0)), MuonPhi.at(index.at(2)));
    Double_t dR14 = deltaR(MuonEta.at(index.at(0)), MuonEta.at(index.at(3)), MuonPhi.at(index.at(0)), MuonPhi.at(index.at(3)));
    Double_t dR23 = deltaR(MuonEta.at(index.at(1)), MuonEta.at(index.at(2)), MuonPhi.at(index.at(1)), MuonPhi.at(index.at(2)));
    Double_t dR24 = deltaR(MuonEta.at(index.at(1)), MuonEta.at(index.at(3)), MuonPhi.at(index.at(1)), MuonPhi.at(index.at(3)));
    Double_t dR34 = deltaR(MuonEta.at(index.at(2)), MuonEta.at(index.at(3)), MuonPhi.at(index.at(2)), MuonPhi.at(index.at(3)));
    
    if (dR12<DeltaRmax && dR13<DeltaRmax && dR14<DeltaRmax && dR23<DeltaRmax && dR24<DeltaRmax && dR34<DeltaRmax) return true;
    else return false;
}

Bool_t isPairDeltaZGood(double vz1, double vz2, double vz3, double vz4, double DeltaZmax){
    double dZ12 = TMath::Abs(vz2 - vz1);
    double dZ13 = TMath::Abs(vz3 - vz1);
    double dZ14 = TMath::Abs(vz4 - vz1);
    double dZ23 = TMath::Abs(vz3 - vz2);
    double dZ24 = TMath::Abs(vz4 - vz2);
    double dZ34 = TMath::Abs(vz4 - vz3);
    
    if (dZ12<DeltaZmax && dZ13<DeltaZmax && dZ14<DeltaZmax && dZ23<DeltaZmax && dZ24<DeltaZmax && dZ34<DeltaZmax) return true;
    else return false;
}


vector<int> get_4index(ROOT::VecOps::RVec<float> MuonPt, double pt1, double pt2, double pt3, double pt4){
    vector<int> index;
    int i=0;
    auto i1 = std::find(MuonPt.begin(), MuonPt.end(), pt1);
    auto i2 = std::find(MuonPt.begin(), MuonPt.end(), pt2);
    auto i3 = std::find(MuonPt.begin(), MuonPt.end(), pt3);
    auto i4 = std::find(MuonPt.begin(), MuonPt.end(), pt4);
    if (i1 != MuonPt.end() && i2 != MuonPt.end() && i3 != MuonPt.end() && i4 != MuonPt.end()) {
        index.push_back(std::distance(MuonPt.begin(), i1));
        index.push_back(std::distance(MuonPt.begin(), i2));
        index.push_back(std::distance(MuonPt.begin(), i3));
        index.push_back(std::distance(MuonPt.begin(), i4));
    }
    else{
        index.push_back(-1);
        index.push_back(-1);
        index.push_back(-1);
        index.push_back(-1);
    }
    return index;
}
std::vector<std::vector<int>> get_stat(int quad_indx, ROOT::VecOps::RVec<float> MuonPt, ROOT::VecOps::RVec<float> MuonEta, ROOT::VecOps::RVec<float> MuonPhi, ROOT::VecOps::RVec<double> Mu1_Pt, ROOT::VecOps::RVec<double> Mu2_Pt, ROOT::VecOps::RVec<double> Mu3_Pt, ROOT::VecOps::RVec<double> Mu4_Pt, ROOT::VecOps::RVec<int> NGoodQuadruplets, ROOT::VecOps::RVec<double> QuadrupletVtx_Chi2, ROOT::VecOps::RVec<double> Quadruplet_Mass, ROOT::VecOps::RVec<double> Muon_isGlobal, ROOT::VecOps::RVec<double> Muon_isPF, ROOT::VecOps::RVec<double> Muon_isLoose, ROOT::VecOps::RVec<double> Muon_isMedium, ROOT::VecOps::RVec<double> Muon_isTight, ROOT::VecOps::RVec<double> Muon_isSoft, ROOT::VecOps::RVec<double> Muon_isTrackerMuon, ROOT::VecOps::RVec<double> MuonPt_HLT, ROOT::VecOps::RVec<double> MuonEta_HLT, ROOT::VecOps::RVec<double> MuonPhi_HLT,  ROOT::VecOps::RVec<double> FlightDistBS_SV_Significance, ROOT::VecOps::RVec<double> Muon_vz){    
    std::vector<std::vector<int>> out;
    
    std::vector<int> isGlobal={0,0,0,0};
    std::vector<int> isPF={0,0,0,0};
    std::vector<int> isLoose={0,0,0,0};
    std::vector<int> isMedium={0,0,0,0};
    std::vector<int> isTight={0,0,0,0};
    std::vector<int> isSoft={0,0,0,0};
    std::vector<int> isTracker={0,0,0,0};
    
    vector<int> index = get_4index(MuonPt, Mu1_Pt.at(quad_indx), Mu2_Pt.at(quad_indx), Mu3_Pt.at(quad_indx), Mu4_Pt.at(quad_indx));
    for(int k=0; k<index.size(); k++){
        isGlobal[k] = Muon_isGlobal.at(index.at(k));
        isPF[k] = Muon_isPF.at(index.at(k));
        isTracker[k] = Muon_isTrackerMuon.at(index.at(k));
        isLoose[k] = Muon_isLoose.at(index.at(k));
        isMedium[k] = Muon_isMedium.at(index.at(k));
        isTight[k] = Muon_isTight.at(index.at(k));
        isSoft[k] = Muon_isSoft.at(index.at(k));
    }
    out.push_back(isGlobal);
    out.push_back(isPF);
    out.push_back(isLoose);
    out.push_back(isMedium);
    out.push_back(isTight);
    out.push_back(isSoft);
    out.push_back(isTracker);
    return out;
}
struct flat3D{
    int i;
    flat3D(int ii) : i(ii)  {}
    std::vector<int> operator()(std::vector<std::vector<int>> branch) {
        return branch.at(i);
    }
};

vector<int> best_quadruplets(ROOT::VecOps::RVec<float> MuonPt, ROOT::VecOps::RVec<float> MuonEta, ROOT::VecOps::RVec<float> MuonPhi, ROOT::VecOps::RVec<double> Mu1_Pt, ROOT::VecOps::RVec<double> Mu2_Pt, ROOT::VecOps::RVec<double> Mu3_Pt, ROOT::VecOps::RVec<double> Mu4_Pt, ROOT::VecOps::RVec<int> NGoodQuadruplets, ROOT::VecOps::RVec<double> QuadrupletVtx_Chi2, ROOT::VecOps::RVec<double> Quadruplet_Mass, ROOT::VecOps::RVec<double> Muon_isGlobal, ROOT::VecOps::RVec<double> Muon_isPF, ROOT::VecOps::RVec<double> Muon_isLoose, ROOT::VecOps::RVec<double> Muon_isMedium, ROOT::VecOps::RVec<double> Muon_isTight, ROOT::VecOps::RVec<double> Muon_isSoft, ROOT::VecOps::RVec<double> MuonPt_HLT, ROOT::VecOps::RVec<double> MuonEta_HLT, ROOT::VecOps::RVec<double> MuonPhi_HLT,  ROOT::VecOps::RVec<double> FlightDistBS_SV_Significance, ROOT::VecOps::RVec<double> Muon_vz){
    int cont1=0, cont2=0, cont3=0, cont4=0;
    vector<int> quad_indx;
    for (int j=0; j<QuadrupletVtx_Chi2.size(); j++){
        //Cut1 "strange" events
        if(Mu1_Pt.at(j)==-99 || Mu2_Pt.at(j) == -99 || Mu3_Pt.at(j) == -99 || Mu4_Pt.at(j) == -99){ continue;}
        
        //Cut2 FlightDistBS_SV_Significance, dR and dz
        //if(FlightDistBS_SV_Significance.at(j) < 2 ) continue;
        vector<int> index = get_4index(MuonPt, Mu1_Pt.at(j), Mu2_Pt.at(j), Mu3_Pt.at(j), Mu4_Pt.at(j));
        if(index.at(0)==-1){ cout<<"Error in index\n"; continue; }
        cont1++;
        
        //if( !(isPairDeltaRGood(MuonEta, MuonPhi, index, 1)) ) continue;
        double vz1 = Muon_vz.at(index.at(0));
        double vz2 = Muon_vz.at(index.at(1));
        double vz3 = Muon_vz.at(index.at(2));
        double vz4 = Muon_vz.at(index.at(3));
        //if( !(isPairDeltaZGood(vz1, vz2, vz3, vz4, 1) )) continue;
        
        //Cut3 invariant mass
        if(!(Quadruplet_Mass.at(j)>5.05 && Quadruplet_Mass.at(j)<5.65)) continue;
        cont2++;
        
        //Cut4 isGlobal and isPF
        int isGlobal=0;
        int isPF=0;
        for(int k=0; k<index.size(); k++){
            isGlobal = isGlobal + Muon_isGlobal.at(index.at(k));
            isPF = isPF + Muon_isPF.at(index.at(k));
        }
        //if(isGlobal<4 || isPF<4) continue;
        cont3++;

        
        //Cut5 HLT Trigger Matching
        vector<double> pt_HLT, eta_HLT, phi_HLT;
        vector<float> pt, eta, phi;
        for(int h=0; h<index.size(); h++){
            float pt_temp=MuonPt.at(index.at(h));
            float eta_temp=MuonEta.at(index.at(h));
            float phi_temp=MuonPhi.at(index.at(h));
            pt.push_back(pt_temp);
            eta.push_back(eta_temp);
            phi.push_back(phi_temp);
        }        
        int HLT_matching = 0;
        for(int w=0; w<MuonPt_HLT.size();w++){
            for(int p=0; p<pt.size();p++){
                double dphi = abs(phi.at(p) - MuonPhi_HLT.at(w));
                double deta = abs(eta.at(p) - MuonEta_HLT.at(w));
                if(dphi > double(M_PI)) dphi -= double(2*M_PI);
                double dR = TMath::Sqrt(dphi*dphi + deta*deta);
                double dpt = abs(pt.at(p) - MuonPt_HLT.at(w))/pt.at(p);
                if(dR<0.03 && dpt<0.1){
                    HLT_matching++;
                    phi.erase(phi.begin() + p);
                    eta.erase(eta.begin() + p);
                    pt.erase(pt.begin() + p);
                    break;
                }
            }
        }
        
        if(HLT_matching<2) continue;
        
        cont4++;
        
        quad_indx.push_back(j);
    }
    if(quad_indx.size()==0) {quad_indx.push_back(-99); return quad_indx;}

    vector<double> chi2;
    for(int l=0; l<quad_indx.size(); l++){
        double temp_i=quad_indx.at(l);
        double temp_chi2 = QuadrupletVtx_Chi2.at(temp_i);
        chi2.push_back(temp_chi2);
    }

    std::vector<std::pair<double, int>> v_union;
    for (size_t i = 0; i < quad_indx.size(); ++i) {
        v_union.push_back(std::make_pair(chi2[i], quad_indx[i]));
    }
    std::sort(v_union.begin(), v_union.end(), 
              [](const std::pair<double, int>& a, const std::pair<double, int>& b) {
                  return a.first < b.first;
              });

    for (size_t i = 0; i < v_union.size(); ++i) {
        chi2[i] = v_union[i].first;
        quad_indx[i] = v_union[i].second;
    }
    return quad_indx;
}

struct flat_index{
    int i;
    flat_index(int ii) : i(ii)  {}
    double operator()(vector<int> branch) {
        if(i<branch.size()) return branch[i];
        else return -99;
    }
};

double flattening(ROOT::VecOps::RVec<double> var, int Quadruplet_index){
    double value = -99;
    try {
        value = var.at(Quadruplet_index);
    } catch (const std::out_of_range& e) {
        std::cout << "Not valid index " << std::endl;
        return -99;
    }
    return value;
}
