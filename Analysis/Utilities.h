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

double TreeFin_CosAngle(double QuadrupletVtx_x, double QuadrupletVtx_y, double QuadrupletVtx_z, double RefittedPV_x, double RefittedPV_y, double RefittedPV_z, double Quadruplet_Pt, double Quadruplet_Eta,double Quadruplet_Phi, double FlightDistPVSV){
    // Computes the angle between the momentum vector of the 4mu quadruplet (b) and the vector from the primary vertex (a)
    double a_x = QuadrupletVtx_x - RefittedPV_x;
    double a_y = QuadrupletVtx_y - RefittedPV_y;
    double a_z = QuadrupletVtx_z - RefittedPV_z;
    TVector3 b;
    b.SetPtEtaPhi(Quadruplet_Pt, Quadruplet_Eta, Quadruplet_Phi);
    double b_x = b.Px();
    double b_y = b.Py();
    double b_z = b.Pz();
    double a_mod = abs(FlightDistPVSV);
    double b_mod = abs(b.Mag());
    double cos_ang = ((a_x*b_x)+(a_y*b_y)+(a_z*b_z))/(a_mod*b_mod);
    return cos_ang;
}

double dR_Max(double Quadruplet_Eta, double Quadruplet_Phi, double Mu1_Eta, double Mu1_Phi, double Mu2_Eta, double Mu2_Phi, double Mu3_Eta, double Mu3_Phi, double Mu4_Eta, double Mu4_Phi){
    double dr_mu1 = deltaR(Quadruplet_Eta, Mu1_Eta, Quadruplet_Phi, Mu1_Phi);
    double dr_mu2 = deltaR(Quadruplet_Eta, Mu2_Eta, Quadruplet_Phi, Mu2_Phi);
    double dr_mu3 = deltaR(Quadruplet_Eta, Mu3_Eta, Quadruplet_Phi, Mu3_Phi);
    double dr_mu4 = deltaR(Quadruplet_Eta, Mu4_Eta, Quadruplet_Phi, Mu4_Phi);

    double dr_max = dr_mu1;
    if (dr_mu2>dr_max) dr_max = dr_mu2;
    if (dr_mu3>dr_max) dr_max = dr_mu3;
    if (dr_mu4>dr_max) dr_max = dr_mu4;

    return dr_max;
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

vector<int> get_2index(ROOT::VecOps::RVec<float> MuonPt, double pt1, double pt2){
    vector<int> index;
    int i=0;
    auto i1 = std::find(MuonPt.begin(), MuonPt.end(), pt1);
    auto i2 = std::find(MuonPt.begin(), MuonPt.end(), pt2);
    if (i1 != MuonPt.end() && i2 != MuonPt.end()) {
        index.push_back(std::distance(MuonPt.begin(), i1));
        index.push_back(std::distance(MuonPt.begin(), i2));
    }
    else{
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

int GenMatching(ROOT::VecOps::RVec<float> MuonPt, ROOT::VecOps::RVec<float> MuonEta, ROOT::VecOps::RVec<float> MuonPhi, double Mu1_Pt, double Mu2_Pt, double Mu3_Pt, double Mu4_Pt,  ROOT::VecOps::RVec<double> GenParticle_Pt, ROOT::VecOps::RVec<double> GenParticle_Pt_v2, ROOT::VecOps::RVec<double> GenParticle_Eta_v2, ROOT::VecOps::RVec<double> GenParticle_Phi_v2,  ROOT::VecOps::RVec<int> GenParticle_PdgId, ROOT::VecOps::RVec<int> GenParticle_MotherPdgId, ROOT::VecOps::RVec<int> GenParticle_GrandMotherPdgId){
    vector<int> index = get_4index(MuonPt, Mu1_Pt, Mu2_Pt, Mu3_Pt, Mu4_Pt);
    vector<double> pt, eta, phi;
    for(int h=0; h<index.size(); h++){
        double pt_temp=MuonPt.at(index.at(h));
        double eta_temp=MuonEta.at(index.at(h));
        double phi_temp=MuonPhi.at(index.at(h));
        pt.push_back(pt_temp);
        eta.push_back(eta_temp);
        phi.push_back(phi_temp);
    }
    vector<double> Genpt, Geneta, Genphi;
    for(int j=0; j<GenParticle_Pt_v2.size(); j++){ 
        Genpt.push_back(GenParticle_Pt_v2.at(j));
        Geneta.push_back(GenParticle_Eta_v2.at(j));
        Genphi.push_back(GenParticle_Phi_v2.at(j));
    }
    if(Genpt.size() != 4) cout<<"Genpt.size() != 4"<<endl;
    int Gen_matching = 0;
    for(int p=0; p<pt.size();p++){
        //cout<<"Genpt: ";
        //for(int kk=0; kk<Genpt.size(); kk++) {cout<<Genpt[kk]<<" ";}
        //cout<<endl;
        vector<double> dR_temp, dpt_temp, dRpt_temp;
        for(int w=0; w<Genpt.size();w++){
            double dphi = abs(phi.at(p) - Genphi.at(w));
            double deta = abs(eta.at(p) - Geneta.at(w));
            if(dphi > double(M_PI)) dphi -= double(2*M_PI);
            double dR = TMath::Sqrt(dphi*dphi + deta*deta);
            double dpt = abs(pt.at(p) - Genpt.at(w))/pt.at(p);
            double dRpt = TMath::Sqrt(dphi*dphi + deta*deta + dpt*dpt);
            dR_temp.push_back(dR);
            dpt_temp.push_back(dpt);
            dRpt_temp.push_back(dRpt);
        }
        auto dRpt_min_p = std::min_element(dRpt_temp.begin(), dRpt_temp.end());
        int dRpt_minID = std::distance(dRpt_temp.begin(), dRpt_min_p);
        double dRpt_min = *dRpt_min_p;
        double dpt_min = dpt_temp[dRpt_minID];
        double dR_min = dR_temp[dRpt_minID];
        if(dR_min<0.03 && dpt_min<0.08){
            Gen_matching++;
            Genpt.erase(Genpt.begin() + dRpt_minID);
            Geneta.erase(Geneta.begin() + dRpt_minID);
            Genphi.erase(Genphi.begin() + dRpt_minID);
        }
    }
    if(Gen_matching<4) return 99;
    else return 1;
}

int GenMatching2mu(ROOT::VecOps::RVec<float> MuonPt, ROOT::VecOps::RVec<float> MuonEta, ROOT::VecOps::RVec<float> MuonPhi, double Mu1_Pt, double Mu2_Pt,  ROOT::VecOps::RVec<double> GenParticle_Pt, ROOT::VecOps::RVec<double> GenParticle_Pt_v2, ROOT::VecOps::RVec<double> GenParticle_Eta_v2, ROOT::VecOps::RVec<double> GenParticle_Phi_v2,  ROOT::VecOps::RVec<int> GenParticle_PdgId, ROOT::VecOps::RVec<int> GenParticle_MotherPdgId, ROOT::VecOps::RVec<int> GenParticle_GrandMotherPdgId){
    vector<int> index = get_2index(MuonPt, Mu1_Pt, Mu2_Pt);
    vector<double> pt, eta, phi;
    for(int h=0; h<index.size(); h++){
        double pt_temp=MuonPt.at(index.at(h));
        double eta_temp=MuonEta.at(index.at(h));
        double phi_temp=MuonPhi.at(index.at(h));
        pt.push_back(pt_temp);
        eta.push_back(eta_temp);
        phi.push_back(phi_temp);
    }
    vector<double> Genpt, Geneta, Genphi;
    for(int j=0; j<GenParticle_Pt_v2.size(); j++){ 
        Genpt.push_back(GenParticle_Pt_v2.at(j));
        Geneta.push_back(GenParticle_Eta_v2.at(j));
        Genphi.push_back(GenParticle_Phi_v2.at(j));
    }
    if(Genpt.size() != 2) cout<<"Genpt.size() != 2"<<endl;
    int Gen_matching = 0;
    for(int p=0; p<pt.size();p++){
        //cout<<"Genpt: ";
        //for(int kk=0; kk<Genpt.size(); kk++) {cout<<Genpt[kk]<<" ";}
        //cout<<endl;
        vector<double> dR_temp, dpt_temp, dRpt_temp;
        for(int w=0; w<Genpt.size();w++){
            double dphi = abs(phi.at(p) - Genphi.at(w));
            double deta = abs(eta.at(p) - Geneta.at(w));
            if(dphi > double(M_PI)) dphi -= double(2*M_PI);
            double dR = TMath::Sqrt(dphi*dphi + deta*deta);
            double dpt = abs(pt.at(p) - Genpt.at(w))/pt.at(p);
            double dRpt = TMath::Sqrt(dphi*dphi + deta*deta + dpt*dpt);
            dR_temp.push_back(dR);
            dpt_temp.push_back(dpt);
            dRpt_temp.push_back(dRpt);
        }
        auto dRpt_min_p = std::min_element(dRpt_temp.begin(), dRpt_temp.end());
        int dRpt_minID = std::distance(dRpt_temp.begin(), dRpt_min_p);
        double dRpt_min = *dRpt_min_p;
        double dpt_min = dpt_temp[dRpt_minID];
        double dR_min = dR_temp[dRpt_minID];
        if(dR_min<0.03 && dpt_min<0.08){
            Gen_matching++;
            Genpt.erase(Genpt.begin() + dRpt_minID);
            Geneta.erase(Geneta.begin() + dRpt_minID);
            Genphi.erase(Genphi.begin() + dRpt_minID);
        }
    }
    if(Gen_matching!=2) return 99;
    else return 1;
}


vector<int> B4mu_QuadSel(int isMC, int evt, ROOT::VecOps::RVec<float> MuonPt, ROOT::VecOps::RVec<float> MuonEta, ROOT::VecOps::RVec<float> MuonPhi, ROOT::VecOps::RVec<double> RefTrack1_Pt, ROOT::VecOps::RVec<double> Mu1_Pt, ROOT::VecOps::RVec<double> Mu2_Pt, ROOT::VecOps::RVec<double> Mu3_Pt, ROOT::VecOps::RVec<double> Mu4_Pt, ROOT::VecOps::RVec<int> NGoodQuadruplets, ROOT::VecOps::RVec<double> QuadrupletVtx_Chi2, ROOT::VecOps::RVec<double> Quadruplet_Mass, ROOT::VecOps::RVec<double> Muon_isGlobal, ROOT::VecOps::RVec<double> Muon_isPF, ROOT::VecOps::RVec<double> Muon_isLoose, ROOT::VecOps::RVec<double> Muon_isMedium, ROOT::VecOps::RVec<double> Muon_isTight, ROOT::VecOps::RVec<double> Muon_isSoft, ROOT::VecOps::RVec<double> MuonPt_HLT, ROOT::VecOps::RVec<double> MuonEta_HLT, ROOT::VecOps::RVec<double> MuonPhi_HLT,  ROOT::VecOps::RVec<double> FlightDistBS_SV_Significance, ROOT::VecOps::RVec<double> Muon_vz, ROOT::VecOps::RVec<double> GenParticle_Pt, ROOT::VecOps::RVec<double> GenParticle_Pt_v2, ROOT::VecOps::RVec<double> GenParticle_Eta_v2, ROOT::VecOps::RVec<double> GenParticle_Phi_v2,  ROOT::VecOps::RVec<int> GenParticle_PdgId, ROOT::VecOps::RVec<int> GenParticle_MotherPdgId, ROOT::VecOps::RVec<int> GenParticle_GrandMotherPdgId){
    vector<int> quad_indx;
    int exit_code = -1;
    
    for (int j=0; j<QuadrupletVtx_Chi2.size(); j++){
        //Cut1 "strange" events
        if(Mu1_Pt.at(j)==-99 || Mu2_Pt.at(j) == -99 || Mu3_Pt.at(j) == -99 || Mu4_Pt.at(j) == -99 || RefTrack1_Pt.at(j) == -99){ continue;}

        if(exit_code<0) exit_code=0;
        
        vector<int> index = get_4index(MuonPt, Mu1_Pt.at(j), Mu2_Pt.at(j), Mu3_Pt.at(j), Mu4_Pt.at(j));
        if(index.at(0)==-1){ cout<<"Error in index\n"; continue; }
        
        if(exit_code<1) exit_code=1;
        
        //Cut2 FlightDistBS_SV_Significance, dR and dz
        //if(FlightDistBS_SV_Significance.at(j) < 2.25 ) continue;
        
        //Cut2 CMS muon system acceptance
        bool acceptanceCUT = true;
        for(int c=0; c<index.size(); c++){
            if ( abs(MuonEta.at(index.at(c))) < 1.2 && MuonPt.at(index.at(c))<3.5 ) acceptanceCUT=false;
            if ( abs(MuonEta.at(index.at(c))) > 1.2 && MuonPt.at(index.at(c))<2 ) acceptanceCUT=false;
            if ( abs(MuonEta.at(index.at(c))) > 2.4) acceptanceCUT=false;
        }
        if(acceptanceCUT==false) continue;
        if(exit_code<2) exit_code=2;
        
        //if( !(isPairDeltaRGood(MuonEta, MuonPhi, index, 1)) ) continue;
        double vz1 = Muon_vz.at(index.at(0));
        double vz2 = Muon_vz.at(index.at(1));
        double vz3 = Muon_vz.at(index.at(2));
        double vz4 = Muon_vz.at(index.at(3));
        //if( !(isPairDeltaZGood(vz1, vz2, vz3, vz4, 1) )) continue;
        
        //Cut3 invariant mass
        if(!(Quadruplet_Mass.at(j)>4.0 && Quadruplet_Mass.at(j)<7.0)) continue;
        if(exit_code<3) exit_code=3;
        
        //Cut4 isGlobal and isPF
        int isGlobal=0;
        int isMedium=0;
        int isPF=0;
        int isSoft=0;
        for(int k=0; k<index.size(); k++){
            isGlobal = isGlobal + Muon_isGlobal.at(index.at(k));
            isMedium = isMedium + Muon_isMedium.at(index.at(k));
            isSoft = isSoft + Muon_isSoft.at(index.at(k));
            isPF = isPF + Muon_isPF.at(index.at(k));
        }
        //if(!(isMedium==4)) continue;
        if(!(isMedium==4 && isGlobal==4)) continue;
        if(exit_code<4) exit_code=4;
        
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
                //if(dR<0.03 && dpt<0.1){
                if(dR<0.1){
                    HLT_matching++;
                    phi.erase(phi.begin() + p);
                    eta.erase(eta.begin() + p);
                    pt.erase(pt.begin() + p);
                    break;
                }
            }
        }
        if(HLT_matching<2) continue;
        if(exit_code<5 ) exit_code=5;
        
        //CUT 6: Gen Matching only MC
        //if(isMC>0){
            //int genmatch = GenMatching(MuonPt, MuonEta, MuonPhi, Mu1_Pt.at(j), Mu2_Pt.at(j), Mu3_Pt.at(j), Mu4_Pt.at(j), GenParticle_Pt, GenParticle_Pt_v2, GenParticle_Eta_v2, GenParticle_Phi_v2, GenParticle_PdgId, GenParticle_MotherPdgId, GenParticle_GrandMotherPdgId);
            //if(genmatch!=1) continue;
        //}
        //if(exit_code<6) exit_code=6;
        quad_indx.push_back(j);
    }
    //cout<<evt<<", "<<exit_code<<endl;
    
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

vector<int> B2muKpi_CombSel( ROOT::VecOps::RVec<double> Mu3_Pt, ROOT::VecOps::RVec<double> Mu4_Pt, ROOT::VecOps::RVec<double> Mu3_Eta, ROOT::VecOps::RVec<double> Mu4_Eta, ROOT::VecOps::RVec<double> Mu3_Phi, ROOT::VecOps::RVec<double> Mu4_Phi,  ROOT::VecOps::RVec<double> QuadrupletVtx_Chi2){
    vector<int> quad_indx(QuadrupletVtx_Chi2.size(), 0);
    
    for (int j=0; j<QuadrupletVtx_Chi2.size(); j++){
        if(Mu3_Pt.at(j) == -99 || Mu4_Pt.at(j) == -99){ continue;}
        TLorentzVector mu3, mu4, mutot, mu32, mu42, mutot2;
        mu3.SetPtEtaPhiM(Mu3_Pt.at(j), Mu3_Eta.at(j), Mu3_Phi.at(j), 0.493677);
        mu4.SetPtEtaPhiM(Mu4_Pt.at(j), Mu4_Eta.at(j), Mu4_Phi.at(j), 0.139570);
        mutot = mu3 + mu4;
        double ditrkmass1 = mutot.M();
        double ditrkmass2 = -9999;
        int w_out;
        for (int w=j+1; w<QuadrupletVtx_Chi2.size(); w++){
            if(Mu3_Pt.at(w) == -99 || Mu4_Pt.at(w) == -99){ continue;}
            if(!((Mu3_Eta.at(j) == Mu4_Eta.at(w)) && (Mu4_Eta.at(j) == Mu3_Eta.at(w)))){ continue;}
            mu32.SetPtEtaPhiM(Mu3_Pt.at(w), Mu3_Eta.at(w), Mu3_Phi.at(w), 0.493677);
            mu42.SetPtEtaPhiM(Mu4_Pt.at(w), Mu4_Eta.at(w), Mu4_Phi.at(w), 0.139570);
            mutot2 = mu32 + mu42;
            ditrkmass2 = mutot2.M();
            w_out=w;
            break;
        }
        if(abs(ditrkmass1-0.892) < abs(ditrkmass2-0.892)) quad_indx[j]=1;
        else if(abs(ditrkmass1-ditrkmass2)<0.1) {quad_indx[j]=1; quad_indx[w_out]=1;}
        else quad_indx[w_out]=1;
    }
    return quad_indx;
}

vector<int> B2mu2K_CombSel( ROOT::VecOps::RVec<double> Mu3_Pt, ROOT::VecOps::RVec<double> Mu4_Pt, ROOT::VecOps::RVec<double> Mu3_Eta, ROOT::VecOps::RVec<double> Mu3_Phi, ROOT::VecOps::RVec<double> Mu4_Phi, ROOT::VecOps::RVec<double> Mu4_Eta, ROOT::VecOps::RVec<double> QuadrupletVtx_Chi2){
    vector<int> quad_indx(QuadrupletVtx_Chi2.size(), 1);
    return quad_indx;
}

vector<int> B2muX_QuadSel(vector<int> indexPreSel, int isMC, int evt, ROOT::VecOps::RVec<float> MuonPt, ROOT::VecOps::RVec<float> MuonEta, ROOT::VecOps::RVec<float> MuonPhi, ROOT::VecOps::RVec<double> RefTrack1_Pt, ROOT::VecOps::RVec<double> Mu1_Pt, ROOT::VecOps::RVec<double> Mu2_Pt, ROOT::VecOps::RVec<double> Mu3_Pt, ROOT::VecOps::RVec<double> Mu4_Pt, ROOT::VecOps::RVec<double> Mu3_Eta, ROOT::VecOps::RVec<double> Mu4_Eta, ROOT::VecOps::RVec<int> NGoodQuadruplets, ROOT::VecOps::RVec<double> QuadrupletVtx_Chi2, ROOT::VecOps::RVec<double> Quadruplet_Mass, ROOT::VecOps::RVec<double> Muon_isGlobal, ROOT::VecOps::RVec<double> Muon_isPF, ROOT::VecOps::RVec<double> Muon_isLoose, ROOT::VecOps::RVec<double> Muon_isMedium, ROOT::VecOps::RVec<double> Muon_isTight, ROOT::VecOps::RVec<double> Muon_isSoft, ROOT::VecOps::RVec<double> MuonPt_HLT, ROOT::VecOps::RVec<double> MuonEta_HLT, ROOT::VecOps::RVec<double> MuonPhi_HLT,  ROOT::VecOps::RVec<double> FlightDistBS_SV_Significance, ROOT::VecOps::RVec<double> Muon_vz, ROOT::VecOps::RVec<double> GenParticle_Pt, ROOT::VecOps::RVec<double> GenParticle_Pt_v2, ROOT::VecOps::RVec<double> GenParticle_Eta_v2, ROOT::VecOps::RVec<double> GenParticle_Phi_v2,  ROOT::VecOps::RVec<int> GenParticle_PdgId, ROOT::VecOps::RVec<int> GenParticle_MotherPdgId, ROOT::VecOps::RVec<int> GenParticle_GrandMotherPdgId){
    vector<int> quad_indx;
    int exit_code = -1;
    
    for (int j=0; j<QuadrupletVtx_Chi2.size(); j++){
        //Cut1 "strange" events
        if(Mu1_Pt.at(j)==-99 || Mu2_Pt.at(j) == -99 || Mu3_Pt.at(j) == -99 || Mu4_Pt.at(j) == -99 || RefTrack1_Pt.at(j) == -99){ continue;}
        if(indexPreSel.at(j)==0){ continue;}
        
        if(exit_code<0) exit_code=0;
        
        vector<int> index = get_2index(MuonPt, Mu1_Pt.at(j), Mu2_Pt.at(j));
        if(index.at(0)==-1){ cout<<"Error in index\n"; continue; }
        
        if(exit_code<1) exit_code=1;
        
        //Cut2 FlightDistBS_SV_Significance, dR and dz
        //if(FlightDistBS_SV_Significance.at(j) < 2.25 ) continue;
        
        //Cut2 CMS muon system acceptance
        bool acceptanceCUT = true;
        for(int c=0; c<index.size(); c++){
            if ( abs(MuonEta.at(index.at(c))) < 1.2 && MuonPt.at(index.at(c))<3.5 ) acceptanceCUT=false;
            if ( abs(MuonEta.at(index.at(c))) > 1.2 && MuonPt.at(index.at(c))<2 ) acceptanceCUT=false;
            if ( abs(MuonEta.at(index.at(c))) > 2.4) acceptanceCUT=false;
        }
        // Tracks acceptance:
        if ( abs(Mu3_Eta.at(j)) > 2.4 ||  Mu3_Pt.at(j)<1.2 ) acceptanceCUT=false;
        if ( abs(Mu4_Eta.at(j)) > 2.4 ||  Mu4_Pt.at(j)<1.2 ) acceptanceCUT=false;
        
        if(acceptanceCUT==false) continue;
        if(exit_code<2) exit_code=2;
        
        //if( !(isPairDeltaRGood(MuonEta, MuonPhi, index, 1)) ) continue;
        //double vz1 = Muon_vz.at(index.at(0));
        //double vz2 = Muon_vz.at(index.at(1));
        //if( !(isPairDeltaZGood(vz1, vz2, vz3, vz4, 1) )) continue;
        
        //Cut3 invariant mass
        if(!(Quadruplet_Mass.at(j)>4.0 && Quadruplet_Mass.at(j)<7.0)) continue;
        if(exit_code<3) exit_code=3;
        
        //Cut4 isGlobal and isPF
        int isGlobal=0;
        int isMedium=0;
        int isPF=0;
        int isSoft=0;
        for(int k=0; k<index.size(); k++){
            isGlobal = isGlobal + Muon_isGlobal.at(index.at(k));
            isMedium = isMedium + Muon_isMedium.at(index.at(k));
            isSoft = isSoft + Muon_isSoft.at(index.at(k));
            isPF = isPF + Muon_isPF.at(index.at(k));
        }
        //if(!(isMedium==4)) continue;
        if(!(isMedium==2 && isGlobal==2)) continue;
        if(exit_code<4) exit_code=4;
        
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
                //if(dR<0.03 && dpt<0.1){
                if(dR<0.1){
                    HLT_matching++;
                    phi.erase(phi.begin() + p);
                    eta.erase(eta.begin() + p);
                    pt.erase(pt.begin() + p);
                    break;
                }
            }
        }
        if(HLT_matching<2) continue;
        if(exit_code<5 ) exit_code=5;
        
        //CUT 6: Gen Matching only MC
        //if(isMC>0){
            //int genmatch = GenMatching2mu(MuonPt, MuonEta, MuonPhi, Mu1_Pt.at(j), Mu2_Pt.at(j), GenParticle_Pt, GenParticle_Pt_v2, GenParticle_Eta_v2, GenParticle_Phi_v2, GenParticle_PdgId, GenParticle_MotherPdgId, GenParticle_GrandMotherPdgId);
            //if(genmatch!=1) continue;
        //}
        //if(exit_code<6) exit_code=6;
        quad_indx.push_back(j);
    }
    //cout<<evt<<", "<<exit_code<<endl;
    
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
                  //return a.first > b.first;
              });

    for (size_t i = 0; i < v_union.size(); ++i) {
        chi2[i] = v_union[i].first;
        quad_indx[i] = v_union[i].second;
    }

    return quad_indx;
}

struct flat0D_int{
    int i;
    flat0D_int(int ii) : i(ii)  {}
    int operator()(vector<int> branch) {
        if(i<branch.size()) return branch[i];
        else return -99;
    }
};

struct flat0D_double{
    int i;
    flat0D_double(int ii) : i(ii)  {}
    double operator()(vector<double> branch) {
        if(i<branch.size()) return branch[i];
        else return -99;
    }
};

struct flat1D_double{
    int i;
    flat1D_double(int ii) : i(ii)  {}
    std::vector<double> operator()(std::vector<std::vector<double>> branch) {
        return branch.at(i);
    }
};

struct flat1D_int{
    int i;
    flat1D_int(int ii) : i(ii)  {}
    std::vector<int> operator()(std::vector<std::vector<int>> branch) {
        return branch.at(i);
    }
};

struct add_int{
    int i;
    add_int(int ii) : i(ii)  {}
    int operator()() {
        return i;
    }
};

struct add_double{
    double i;
    add_double(double ii) : i(ii)  {}
    double operator()() {
        return i;
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

struct flat2D{
    int i;
    int j;
    flat2D(int ii, int jj) : i(ii), j(jj)  {}
    double operator()(std::pair<std::vector<double>, std::vector<double>> branch) {
        if(i==0){
            return (branch.first)[j];
        }
        if(i==1){
            return (branch.second)[j];
        }
        else return -1;
    }
};

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

double Mass(int mu_index1, int mu_index, ROOT::VecOps::RVec<float> MuonPt, ROOT::VecOps::RVec<float> MuonEta, ROOT::VecOps::RVec<float> MuonPhi, ROOT::VecOps::RVec<double> MuonEnergy){
    float pt1 = MuonPt.at(mu_index1);
    float pt2 = MuonPt.at(mu_index);
    float eta1 = MuonEta.at(mu_index1);
    float eta2 = MuonEta.at(mu_index);
    float phi1 = MuonPhi.at(mu_index1);
    float phi2 = MuonPhi.at(mu_index);
    double en1 = MuonEnergy.at(mu_index1);
    double en2 = MuonEnergy.at(mu_index);
    TLorentzVector mu1, mu2, mutot;
    mu1.SetPtEtaPhiE(pt1, eta1, phi1, en1);
    mu2.SetPtEtaPhiE(pt2, eta2, phi2, en2);
    mutot = mu1 + mu2;
    return mutot.M();
}

std::vector<double> DimuondR(std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> Dimuon_index, ROOT::VecOps::RVec<float> MuonEta, ROOT::VecOps::RVec<float> MuonPhi){
    
    std::vector<std::vector<int>> final_pairs1 = Dimuon_index.first;
    std::vector<std::vector<int>> final_pairs2 = Dimuon_index.second;
    
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

    std::vector<double> dR;
    dR.push_back(dr_1);
    dR.push_back(dr_2);
    return(dR);
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

std::vector<double> DimuonMassfinal(double Dimu_OS1_1, double Dimu_OS1_2, double Dimu_OS2_1, double Dimu_OS2_2){
    double Dimu_OS1_min = std::min(Dimu_OS1_1, Dimu_OS1_2);
    double Dimu_OS1_max = std::max(Dimu_OS1_1, Dimu_OS1_2);
    double Dimu_OS2_min = std::min(Dimu_OS2_1, Dimu_OS2_2);
    double Dimu_OS2_max = std::max(Dimu_OS2_1, Dimu_OS2_2);

    double massjpsi = 3.0969;
    double massphi = 1.019445;
    std::vector<double> mass_max_min;
    
    double diff1 = std::abs(Dimu_OS1_max-massjpsi) + std::abs(Dimu_OS1_min-massphi);
    double diff2 = std::abs(Dimu_OS2_max-massjpsi) + std::abs(Dimu_OS2_min-massphi);
    if(diff1<diff2){
        mass_max_min.push_back(Dimu_OS1_max);
        mass_max_min.push_back(Dimu_OS1_min);
        }
    else{
        mass_max_min.push_back(Dimu_OS2_max);
        mass_max_min.push_back(Dimu_OS2_min);
        }
    return mass_max_min;
}

double BsJPsiPhiMass(double Dimu_OS_max, double Dimu_OS_min, double Quadruplet_Mass){
    double massjpsi = 3.0969;
    double massphi = 1.019445;
    return Quadruplet_Mass + massjpsi + massphi - Dimu_OS_max - Dimu_OS_min;
}

int BsJPsiPhi(double Dimu_OS_max, double Dimu_OS_min){
    double sigma_phi = 0.015;
    double sigma_jpsi = 0.035;
    double massjpsi = 3.0969;
    double massphi = 1.019445;
    if (std::abs(massphi-Dimu_OS_min)<3*sigma_phi && std::abs(massjpsi-Dimu_OS_max)<3*sigma_jpsi) return 1;
    else return 0;
}

struct TwoObjMassFit{
    double m1;
    double m2;
    TwoObjMassFit(double mm1, double mm2) : m1(mm1), m2(mm2)  {}
    double operator()(double pt1, double pt2, double eta1, double eta2, double phi1, double phi2) {
        TLorentzVector mu1, mu2, mutot;
        mu1.SetPtEtaPhiM(pt1, eta1, phi1, m1);
        mu2.SetPtEtaPhiM(pt2, eta2, phi2, m2);
        mutot=mu1+mu2;
        return mutot.M();
    }
};

struct FourObjMassFit{
    double m1;
    double m2;
    double m3;
    double m4;
    FourObjMassFit(double mm1, double mm2, double mm3, double mm4) : m1(mm1), m2(mm2), m3(mm3), m4(mm4)  {}
    double operator()(double pt1, double pt2, double pt3, double pt4, double eta1, double eta2, double eta3, double eta4, double phi1, double phi2, double phi3, double phi4) {
        TLorentzVector mu1, mu2, mu3, mu4, mutot;
        mu1.SetPtEtaPhiM(pt1, eta1, phi1, m1);
        mu2.SetPtEtaPhiM(pt2, eta2, phi1, m2);
        mu3.SetPtEtaPhiM(pt3, eta3, phi3, m3);
        mu4.SetPtEtaPhiM(pt4, eta4, phi4, m4);
        mutot=mu1+mu2+mu3+mu4;
        return mutot.M();
    }
};

vector<double> DiMassB2mu2K(double pt1, double pt2, double pt3, double pt4, double eta3, double eta4, double phi3, double phi4, ROOT::VecOps::RVec<float> MuonPt, ROOT::VecOps::RVec<float> MuonEta, ROOT::VecOps::RVec<float> MuonPhi, ROOT::VecOps::RVec<double> MuonEnergy){
    vector<int> index = get_2index(MuonPt, pt1, pt2);
    double dimumass = Mass(index[0], index[1], MuonPt, MuonEta, MuonPhi, MuonEnergy);
    TLorentzVector mu3, mu4, mutot;
    mu3.SetPtEtaPhiM(pt3, eta3, phi3, 0.493677);
    mu4.SetPtEtaPhiM(pt4, eta4, phi4, 0.493677);
    mutot = mu3 + mu4;
    double ditrkmass = mutot.M();
    vector<double> masses;
    masses.push_back(dimumass);
    masses.push_back(ditrkmass);
    return masses;
}

vector<double> DiMassB2muKpi(double pt1, double pt2, double pt3, double pt4, double eta3, double eta4, double phi3, double phi4, ROOT::VecOps::RVec<float> MuonPt, ROOT::VecOps::RVec<float> MuonEta, ROOT::VecOps::RVec<float> MuonPhi, ROOT::VecOps::RVec<double> MuonEnergy){
    vector<int> index = get_2index(MuonPt, pt1, pt2);
    double dimumass = Mass(index[0], index[1], MuonPt, MuonEta, MuonPhi, MuonEnergy);
    TLorentzVector mu3, mu4, mutot;
    mu3.SetPtEtaPhiM(pt3, eta3, phi3, 0.493677);
    mu4.SetPtEtaPhiM(pt4, eta4, phi4, 0.139570);
    mutot = mu3 + mu4;
    double ditrkmass = mutot.M();
    vector<double> masses;
    masses.push_back(dimumass);
    masses.push_back(ditrkmass);
    return masses;
}

double NoRefitMassB4mu(ROOT::VecOps::RVec<float> MuonPt, double pt1, double pt2, double pt3, double pt4, double eta3, double eta4, double phi3, double phi4, ROOT::VecOps::RVec<float> MuonEta, ROOT::VecOps::RVec<float> MuonPhi, ROOT::VecOps::RVec<double> MuonEnergy){
    vector<int> index = get_4index(MuonPt, pt1, pt2, pt3, pt4);
    vector<double> eta, phi, en;
    for(int i=0; i<index.size();i++){
        eta.push_back(MuonEta.at(index.at(i)));
        phi.push_back(MuonPhi.at(index.at(i)));
        en.push_back(MuonEnergy.at(index.at(i)));
    }
    TLorentzVector mu1, mu2, mu3, mu4, mutot;
    mu1.SetPtEtaPhiE(pt1, eta[0], phi[0], en[0]);
    mu2.SetPtEtaPhiE(pt2, eta[1], phi[1], en[1]);
    mu3.SetPtEtaPhiE(pt3, eta[2], phi[2], en[2]);
    mu4.SetPtEtaPhiE(pt4, eta[3], phi[3], en[3]);
    mutot = mu1 + mu2 + mu3 + mu4;
    return mutot.M();
}

double NoRefitMassB2mu2K(ROOT::VecOps::RVec<float> MuonPt, double pt1, double pt2, double pt3, double pt4, double eta3, double eta4, double phi3, double phi4,  ROOT::VecOps::RVec<float> MuonEta, ROOT::VecOps::RVec<float> MuonPhi, ROOT::VecOps::RVec<double> MuonEnergy){
    vector<int> index = get_2index(MuonPt, pt1, pt2);
    vector<double> eta, phi, en;
    for(int i=0; i<index.size();i++){
        eta.push_back(MuonEta.at(index.at(i)));
        phi.push_back(MuonPhi.at(index.at(i)));
        en.push_back(MuonEnergy.at(index.at(i)));
    }
    TLorentzVector mu1, mu2, mu3, mu4, mutot;
    mu1.SetPtEtaPhiE(pt1, eta[0], phi[0], en[0]);
    mu2.SetPtEtaPhiE(pt2, eta[1], phi[1], en[1]);
    mu3.SetPtEtaPhiM(pt3, eta3, phi3, 0.493677);
    mu4.SetPtEtaPhiM(pt4, eta4, phi4, 0.493677);
    mutot = mu1 + mu2 + mu3 + mu4;
    return mutot.M();
}

double NoRefitMassB2muKpi(ROOT::VecOps::RVec<float> MuonPt, double pt1, double pt2, double pt3, double pt4, double eta3, double eta4, double phi3, double phi4, ROOT::VecOps::RVec<float> MuonEta, ROOT::VecOps::RVec<float> MuonPhi, ROOT::VecOps::RVec<double> MuonEnergy){
    vector<int> index = get_2index(MuonPt, pt1, pt2);
    vector<double> eta, phi, en;
    for(int i=0; i<index.size();i++){
        eta.push_back(MuonEta.at(index.at(i)));
        phi.push_back(MuonPhi.at(index.at(i)));
        en.push_back(MuonEnergy.at(index.at(i)));
    }
    TLorentzVector mu1, mu2, mu3, mu4, mutot;
    mu1.SetPtEtaPhiE(pt1, eta[0], phi[0], en[0]);
    mu2.SetPtEtaPhiE(pt2, eta[1], phi[1], en[1]);
    mu3.SetPtEtaPhiM(pt3, eta3, phi3, 0.493677);
    mu4.SetPtEtaPhiM(pt4, eta4, phi4, 0.139570);
    mutot = mu1 + mu2 + mu3 + mu4;
    return mutot.M();
}

vector<vector<double>> GenMatching_v2(ROOT::VecOps::RVec<float> MuonPt, ROOT::VecOps::RVec<float> MuonEta, ROOT::VecOps::RVec<float> MuonPhi, double Mu1_Pt, double Mu2_Pt, double Mu3_Pt, double Mu4_Pt,  ROOT::VecOps::RVec<double> GenParticle_Pt, ROOT::VecOps::RVec<double> GenParticle_Pt_v2, ROOT::VecOps::RVec<double> GenParticle_Eta_v2, ROOT::VecOps::RVec<double> GenParticle_Phi_v2,  ROOT::VecOps::RVec<int> GenParticle_PdgId, ROOT::VecOps::RVec<int> GenParticle_MotherPdgId, ROOT::VecOps::RVec<int> GenParticle_GrandMotherPdgId){
    vector<int> index = get_4index(MuonPt, Mu1_Pt, Mu2_Pt, Mu3_Pt, Mu4_Pt);
    vector<double> pt, eta, phi;
    for(int h=0; h<index.size(); h++){
        double pt_temp=MuonPt.at(index.at(h));
        double eta_temp=MuonEta.at(index.at(h));
        double phi_temp=MuonPhi.at(index.at(h));
        pt.push_back(pt_temp);
        eta.push_back(eta_temp);
        phi.push_back(phi_temp);
    }
    vector<double> Genpt, Geneta, Genphi;
    for(int j=0; j<GenParticle_Pt_v2.size(); j++){ 
        Genpt.push_back(GenParticle_Pt_v2.at(j));
        Geneta.push_back(GenParticle_Eta_v2.at(j));
        Genphi.push_back(GenParticle_Phi_v2.at(j));
    }
    if(Genpt.size() != 4) cout<<"Genpt.size() != 4"<<endl;
    int Gen_matching = 0;
    vector<int> index_gen;
    for(int p=0; p<pt.size();p++){
        vector<double> dR_temp, dpt_temp, dRpt_temp;
        for(int w=0; w<Genpt.size();w++){
            double dphi = abs(phi.at(p) - Genphi.at(w));
            double deta = abs(eta.at(p) - Geneta.at(w));
            if(dphi > double(M_PI)) dphi -= double(2*M_PI);
            double dR = TMath::Sqrt(dphi*dphi + deta*deta);
            double dpt = abs(pt.at(p) - Genpt.at(w))/pt.at(p);
            double dRpt = TMath::Sqrt(dphi*dphi + deta*deta + dpt*dpt);
            dR_temp.push_back(dR);
            dpt_temp.push_back(dpt);
            dRpt_temp.push_back(dRpt);
        }
        auto dRpt_min_p = std::min_element(dRpt_temp.begin(), dRpt_temp.end());
        int dRpt_minID = std::distance(dRpt_temp.begin(), dRpt_min_p);
        double dRpt_min = *dRpt_min_p;
        double dpt_min = dpt_temp[dRpt_minID];
        double dR_min = dR_temp[dRpt_minID];
        if(dR_min<0.03 && dpt_min<0.08){
            index_gen.push_back(dRpt_minID);
            Gen_matching++;
            Genpt.erase(Genpt.begin() + dRpt_minID);
            Geneta.erase(Geneta.begin() + dRpt_minID);
            Genphi.erase(Genphi.begin() + dRpt_minID);
        }
        else{
            index_gen.push_back(-1);
        }
    }
    vector<vector<double>> out;
    vector<double> out_pt;
    vector<double> out_eta;
    vector<double> out_phi;
    for(int ee=0; ee<index_gen.size();ee++){
        if(index_gen[ee] != -1){
            out_pt.push_back(GenParticle_Pt_v2.at(index_gen[ee]));
            out_eta.push_back(GenParticle_Eta_v2.at(index_gen[ee]));
            out_phi.push_back(GenParticle_Phi_v2.at(index_gen[ee]));
        }
        else{
            out_pt.push_back(-1);
            out_eta.push_back(-1);
            out_phi.push_back(-1);
        }
    }
    out.push_back(out_pt);
    out.push_back(out_eta);
    out.push_back(out_phi);
    return out;
}

/*
int BsJPsiPhi(double m1, double m2, double chi1, double chi2){
    std::vector<double> mass = {m1, m2};
    std::sort(mass.begin(), mass.end());
    double sigma_phi = 0.015;
    double sigma_jpsi = 0.035;
    double mass_phi = 1.019;
    double mass_jpsi = 3.096;
    for(int i=2;i<16;i++){
        if (std::abs(mass_phi-mass[0])<3*sigma_phi && std::abs(mass_jpsi-mass[1])<3*sigma_jpsi && chi1>-1 && chi2>-1 && chi1<10*i && chi2<10*i) return i;
    }
    if (std::abs(mass_phi-mass[0])<3*sigma_phi && std::abs(mass_jpsi-mass[1])<3*sigma_jpsi && chi1>-1 && chi2>-1) return 1;
    else return 0;
}
*/
