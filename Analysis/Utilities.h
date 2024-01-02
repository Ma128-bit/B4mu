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

int best_quadruplet(ROOT::VecOps::RVec<float> MuonPt, ROOT::VecOps::RVec<float> MuonEta, ROOT::VecOps::RVec<float> MuonPhi, ROOT::VecOps::RVec<double> Mu1_Pt, ROOT::VecOps::RVec<double> Mu2_Pt, ROOT::VecOps::RVec<double> Mu3_Pt, ROOT::VecOps::RVec<double> Mu4_Pt, ROOT::VecOps::RVec<int> NGoodQuadruplets, ROOT::VecOps::RVec<double> QuadrupletVtx_Chi2, ROOT::VecOps::RVec<double> Quadruplet_Mass, ROOT::VecOps::RVec<double>Muon_isGlobal, ROOT::VecOps::RVec<double>Muon_isPF, ROOT::VecOps::RVec<double> FlightDistBS_SV_Significance, ROOT::VecOps::RVec<double> Muon_vz){
    
    vector<int> quad_indx;
    
    for (int j=0; j<QuadrupletVtx_Chi2.size(); j++){
        //Cut1 "strange" events
        if(Mu1_Pt.at(j)==-99 || Mu2_Pt.at(j) == -99 || Mu3_Pt.at(j) == -99 || Mu4_Pt.at(j) == -99){
            continue;
        }
        
        //Cut2 FlightDistBS_SV_Significance, dR and dz
        //if(FlightDistBS_SV_Significance.at(j) < 2 ) continue;
        vector<int> index = get_4index(MuonPt, Mu1_Pt.at(j), Mu2_Pt.at(j), Mu3_Pt.at(j), Mu4_Pt.at(j));
        if(index.at(0)==-1){
            cout<<"Error in index\n";
            continue;
        }
        
        //if( !(isPairDeltaRGood(MuonEta, MuonPhi, index, 1)) ) continue;
        double vz1 = Muon_vz.at(index.at(0));
        double vz2 = Muon_vz.at(index.at(1));
        double vz3 = Muon_vz.at(index.at(2));
        double vz4 = Muon_vz.at(index.at(3));
        //if( !(isPairDeltaZGood(vz1, vz2, vz3, vz4, 1) )) continue;

        //Cut3 isGlobal and isPF
        int isGlobal=0;
        int isPF=0;
        for(int k=0; k<index.size(); k++){
            isGlobal = isGlobal + Muon_isGlobal.at(index.at(k));
            isPF = isPF + Muon_isPF.at(index.at(k));
        }
        //if(isGlobal<2 || isPF<2) continue;
        
        //Cut4 invariant mass
        if(!(Quadruplet_Mass.at(j)>4.4 && Quadruplet_Mass.at(j)<6.4)) continue;
        
        //Cut5 HLT Trigger Matching
        //Not yet implemented
        
        quad_indx.push_back(j);
    }
    
    if(quad_indx.size()==0) return -99;
    
    double best_chi2=15000000000;
    double best_i=-1;
    
    for(int l=0; l<quad_indx.size(); l++){
        double temp_i=quad_indx.at(l);
        double temp_chi2 = QuadrupletVtx_Chi2.at(temp_i);
        if(temp_chi2 < best_chi2){
            best_chi2=temp_chi2;
            best_i=temp_i;
        }
    }
    return best_i;
}

double flattening(ROOT::VecOps::RVec<double> var, double Quadruplet_index){
    double value = -99;
    try {
        value = var.at(Quadruplet_index);
    } catch (const std::out_of_range& e) {
        std::cout << "Not valid index " << std::endl;
        return -99;
    }
    return value;
}

std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> Dimuon(double Mu1_Pt, double Mu2_Pt, double Mu3_Pt, double Mu4_Pt, ROOT::VecOps::RVec<float> MuonPt, ROOT::VecOps::RVec<float> MuonEta, ROOT::VecOps::RVec<float> MuonPhi, ROOT::VecOps::RVec<double> MuonCharge){
    vector<int> index = get_4index(MuonPt, Mu1_Pt, Mu2_Pt, Mu3_Pt, Mu4_Pt);
    //if(index.at(0)==-1) return 0;

    vector<double> charge;
    for(int i=0; i<index.size();i++){
        charge.push_back(MuonCharge.at(index.at(i)));
    }
    std::vector<std::pair<int, double>> index_charge;
    for (int j = 0; j < std::min(index.size(), charge.size()); ++j) {
        index_charge.push_back(std::make_pair(index[j], charge[j]));
    }
    std::vector<std::vector<int>> final_pairs;
    
    std::pair<int, double> first_pair = index_charge[0];
    int pos = first_pair.first;
    double ch = first_pair.second;
    std::vector<int> index_copy = index;
    auto it = std::find(index_copy.begin(), index_copy.end(), pos);
    index_copy.erase(it);
    int pos2 = -1;
    double ch2 = 0.0;
    for (auto it2 = index_charge.begin()+1; it2 != index_charge.end(); ++it2) {
        ch2 = it2->second;
        if(ch2+ch == 0){
            pos2 = it2->first;
            std::vector<int> pos_21 = index_copy;
            auto it3 = std::find(pos_21.begin(), pos_21.end(), pos2);
            pos_21.erase(it3);
            std::vector<int> pos_12 ={pos, pos2};
            //if(MuonCharge.at(pos_21.at(0))==MuonCharge.at(pos_21.at(1))) cout<<"ERRORRRRRRR!!!"<<endl;
            final_pairs.push_back(pos_12);
            final_pairs.push_back(pos_21);
        }
    }
    std::vector<std::vector<int>> final_pairs1 = {final_pairs[0], final_pairs[1]};
    std::vector<std::vector<int>> final_pairs2 = {final_pairs[2], final_pairs[3]};
    //cout<<MuonPt.at(final_pairs1[0][1])<<" "<<MuonPt.at(final_pairs2[0][1])<<endl;
    float eta1, eta2, phi1, phi2;
    eta1 = MuonEta.at(final_pairs1[0][0]);
    eta2 = MuonEta.at(final_pairs1[0][1]);
    phi1 = MuonPhi.at(final_pairs1[0][0]);
    phi2 = MuonPhi.at(final_pairs1[0][1]);
    double dr_1A = deltaR(eta1, eta2, phi1, phi2);
    eta1 = MuonEta.at(final_pairs1[1][0]);
    eta2 = MuonEta.at(final_pairs1[1][1]);
    phi1 = MuonPhi.at(final_pairs1[1][0]);
    phi2 = MuonPhi.at(final_pairs1[1][1]);
    double dr_1B = deltaR(eta1, eta2, phi1, phi2);
    double dr_1 = dr_1A + dr_1B;
    
    eta1 = MuonEta.at(final_pairs2[0][0]);
    eta2 = MuonEta.at(final_pairs2[0][1]);
    phi1 = MuonPhi.at(final_pairs2[0][0]);
    phi2 = MuonPhi.at(final_pairs2[0][1]);
    double dr_2A = deltaR(eta1, eta2, phi1, phi2);
    eta1 = MuonEta.at(final_pairs2[1][0]);
    eta2 = MuonEta.at(final_pairs2[1][1]);
    phi1 = MuonPhi.at(final_pairs2[1][0]);
    phi2 = MuonPhi.at(final_pairs2[1][1]);
    double dr_2B = deltaR(eta1, eta2, phi1, phi2);
    double dr_2 = dr_2A + dr_2B;
    
    std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> output_index;
    
    if(dr_1<dr_2){
        output_index.first = final_pairs1;
        output_index.second = final_pairs2;
    }
    else{
        output_index.first = final_pairs2;
        output_index.second = final_pairs1;
    }
    return output_index;
}

double Mass(int mu_index1, int mu_index2, ROOT::VecOps::RVec<float> MuonPt, ROOT::VecOps::RVec<float> MuonEta, ROOT::VecOps::RVec<float> MuonPhi, ROOT::VecOps::RVec<double> MuonEnergy){
    float pt1 = MuonPt.at(mu_index1);
    float pt2 = MuonPt.at(mu_index2);
    float eta1 = MuonEta.at(mu_index1);
    float eta2 = MuonEta.at(mu_index2);
    float phi1 = MuonPhi.at(mu_index1);
    float phi2 = MuonPhi.at(mu_index2);
    double en1 = MuonEnergy.at(mu_index1);
    double en2 = MuonEnergy.at(mu_index2);
    TLorentzVector mu1, mu2, mutot;
    mu1.SetPtEtaPhiE(pt1, eta1, phi1, en1);
    mu2.SetPtEtaPhiE(pt2, eta2, phi2, en2);
    mutot = mu1 + mu2;
    return mutot.M();
}

std::pair<std::vector<double>, std::vector<double>> DimuonMass(std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> Dimuon_index, ROOT::VecOps::RVec<float> MuonPt, ROOT::VecOps::RVec<float> MuonEta, ROOT::VecOps::RVec<float> MuonPhi, ROOT::VecOps::RVec<double> MuonEnergy){
    
    std::vector<double> m1;
    std::vector<double> m2;
    
    for(int i=0;i<2;i++){
        m1.push_back(Mass((Dimuon_index.first)[i][0], (Dimuon_index.first)[i][1], MuonPt, MuonEta, MuonPhi, MuonEnergy));
        m2.push_back(Mass((Dimuon_index.second)[i][0], (Dimuon_index.second)[i][1], MuonPt, MuonEta, MuonPhi, MuonEnergy));
    }
    
    return (std::make_pair(m1, m2));
}


double FindDimuChi2(std::vector<int> vec, double Vtx12_Chi2, double Vtx13_Chi2, double Vtx14_Chi2, double Vtx23_Chi2, double Vtx24_Chi2, double Vtx34_Chi2){
    std::vector<int> vec_copy = vec;
    if(vec_copy[0]>vec_copy[1]){
        int temp = vec_copy[1];
        vec_copy[1]= vec_copy[0];
        vec_copy[0] = temp;
    }
    switch (vec_copy[0]+1) {
        case 1:
            switch (vec_copy[1]+1) {
                case 2:
                    return Vtx12_Chi2;
                case 3:
                    return Vtx13_Chi2;
                case 4:
                    return Vtx14_Chi2;
                default:
                    std::cout << "Error!!" << std::endl;
                    return -1;
            }
        case 2:
            switch (vec_copy[1]+1) {
                case 3:
                    return Vtx23_Chi2;
                case 4:
                    return Vtx24_Chi2;
                default:
                    std::cout << "Error!!" << std::endl;
                    return -1;
            }

        case 3:
            switch (vec_copy[1]+1) {
                case 4:
                    return Vtx34_Chi2;
                default:
                    std::cout << "Error!!" << std::endl;
                    return -1;
            }

        default:
            std::cout << "Error!!" << std::endl;
            return -1;
    }
}

std::pair<std::vector<double>, std::vector<double>> DimuonChi2(std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> Dimuon_index, double Mu1_Pt, double Mu2_Pt, double Mu3_Pt, double Mu4_Pt, ROOT::VecOps::RVec<float> MuonPt, double Vtx12_Chi2, double Vtx13_Chi2, double Vtx14_Chi2, double Vtx23_Chi2, double Vtx24_Chi2, double Vtx34_Chi2){
    
    vector<int> index = get_4index(MuonPt, Mu1_Pt, Mu2_Pt, Mu3_Pt, Mu4_Pt);
    
    std::vector<double> chi1;
    std::vector<double> chi2;
    
    for(int i=0;i<2;i++){
        std::vector<int> i1;
        std::vector<int> i2;
        auto it0 = std::find(index.begin(), index.end(), Dimuon_index.first[i][0]);
        auto it1 = std::find(index.begin(), index.end(), Dimuon_index.first[i][1]);
        
        if (it0 != index.end() && it1 != index.end()) {
            i1.push_back(std::distance(index.begin(), it0));
            i1.push_back(std::distance(index.begin(), it1));
            
        }
        chi1.push_back(FindDimuChi2(i1, Vtx12_Chi2, Vtx13_Chi2, Vtx14_Chi2, Vtx23_Chi2, Vtx24_Chi2, Vtx34_Chi2));
        
        auto it2 = std::find(index.begin(), index.end(), Dimuon_index.second[i][0]);
        auto it3 = std::find(index.begin(), index.end(), Dimuon_index.second[i][1]);
        
        if (it3 != index.end() && it2 != index.end()) {
            i2.push_back(std::distance(index.begin(), it2));
            i2.push_back(std::distance(index.begin(), it3));
        }
        chi2.push_back(FindDimuChi2(i2, Vtx12_Chi2, Vtx13_Chi2, Vtx14_Chi2, Vtx23_Chi2, Vtx24_Chi2, Vtx34_Chi2));
        
    }
    
    return (std::make_pair(chi1, chi2));
}

struct flat2D{
    int i;
    int j;
    flat2D(int ii, int jj) : i(ii), j(jj)  {}
    float operator()(std::pair<std::vector<double>, std::vector<double>> branch) {
        if(i==0){
            return (branch.first)[j];
        }
        if(i==1){
            return (branch.second)[j];
        }
        else return -1;
    }
};
