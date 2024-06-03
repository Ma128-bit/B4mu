// -*- C++ -*-
// Package:    MiniAna2017/MiniAnaB2Mu2K
// Class:      MiniAnaB2Mu2K
//
/* class MiniAnaB2Mu2K MiniAnaB2Mu2K.cc MiniAna2017/MiniAnaB2Mu2K/plugins/MiniAnaB2Mu2K.cc
 
 Description: [one line class summary]
 
 Implementation:
 [Notes on implementation]
 */
//
// Original Author:  venditti
//         Created:  Tue, 18 Dec 2018 09:30:06 GMT
//
//


// system include files
#include <memory>
#include <algorithm>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include <DataFormats/MuonReco/interface/MuonFwd.h>
#include <DataFormats/MuonReco/interface/Muon.h>
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "TFile.h"
#include "TH1.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TH2.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "Math/ProbFunc.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "RecoBTag/SecondaryVertex/interface/SecondaryVertex.h"
#include "RecoVertex/AdaptiveVertexFit/interface/AdaptiveVertexFitter.h"

#include "RecoVertex/KinematicFit/interface/KinematicParticleVertexFitter.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParticleFactoryFromTransientTrack.h"
#include "RecoVertex/KinematicFit/interface/MassKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleFitter.h"
#include "RecoVertex/KinematicFitPrimitives/interface/MultiTrackKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/KinematicConstrainedVertexFitter.h"
#include "RecoVertex/KinematicFit/interface/TwoTrackMassKinematicConstraint.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/TransientTrackKinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicTree.h"

#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackFromFTSFactory.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/PatternTools/interface/ClosestApproachInRPhi.h"

#include "DataFormats/Candidate/interface/VertexCompositeCandidateFwd.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"

#include "RecoVertex/VertexPrimitives/interface/ConvertToFromReco.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"

#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/PatCandidates/interface/TriggerFilter.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/Candidate/interface/OverlapChecker.h"
#include "TrackingTools/IPTools/interface/IPTools.h"

#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/L1TGlobal/interface/GlobalAlgBlk.h"
#include "L1Trigger/L1TGlobal/interface/L1TGlobalUtil.h"
#include "DataFormats/L1Trigger/interface/Muon.h"
#include "CondFormats/DataRecord/interface/L1TUtmTriggerMenuRcd.h"
#include "CondFormats/L1TObjects/interface/L1TUtmTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1TGlobalPrescalesVetosRcd.h"
#include "CondFormats/L1TObjects/interface/L1TGlobalPrescalesVetos.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#include "DataFormats/TrackReco/interface/TrackToTrackMap.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
OverlapChecker overlap;
////
class MiniAnaB2Mu2K : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
public:
    explicit MiniAnaB2Mu2K(const edm::ParameterSet&);
    ~MiniAnaB2Mu2K();
    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
    float dR(float eta1, float eta2, float phi1, float phi2);
    float dRtriggerMatch(pat::Muon m, vector<pat::TriggerObjectStandAlone> triggerObjects);
    float dRtriggerMatchTrk(reco::Track Trk, std::vector<pat::TriggerObjectStandAlone> triggerObjects);
    
    
private:
    virtual void beginJob() override;
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;
    edm::EDGetTokenT<edm::View<pat::Muon> > muons_;
    edm::EDGetTokenT<edm::View<reco::Vertex> > vertex_;
    edm::EDGetTokenT<edm::View<pat::PackedCandidate> > trackToken_;
    edm::EDGetTokenT<edm::View<reco::CompositeCandidate> > Cand2Mu2Tracks_;
    edm::EDGetTokenT<edm::View<reco::GenParticle> > genParticles_;
    edm::EDGetTokenT<edm::View<pat::PackedGenParticle> > genParticlesn2_;
    edm::EDGetTokenT<std::vector<PileupSummaryInfo> > puToken_ ;
    edm::EDGetTokenT<edm::TriggerResults> triggerToken_;
    edm::EDGetTokenT<BXVector<l1t::Muon> > l1muonsToken_;
    edm::EDGetTokenT<reco::BeamSpot> token_BeamSpot;
    //edm::EDGetTokenT<edm::View<pat::Photon> > photons_;
    edm::EDGetTokenT<std::vector<pat::TriggerObjectStandAlone> > triggerObjects_;
    edm::EDGetToken algToken_;
    edm::EDGetToken algTok_;
    edm::EDGetTokenT<std::vector<pat::PackedCandidate> >srcCands_;
    edm::ESGetToken<TransientTrackBuilder, TransientTrackRecord> theTransientTrackBuilder_;
    bool isMc;
    bool is2K;
    bool isAna;
    HLTConfigProvider hltConfig;
    
    edm::Service<TFileService> fs;
    l1t::L1TGlobalUtil* gtUtil_;
    TH1F *hEvents;
    TH1F *hEventsAfterGoodCand;
    TH1F *  hEventsAfterMu1ID;
    TH1F *  hEventsAfterMu2ID;
    TH1F *  hEventsAfterMu3ID;
    TH1F *  hEventsAfterMu4ID;
    TH1F *  hEventsAfterBMass;
    
    edm::InputTag algInputTag_;
    const edm::InputTag algTag_, extTag_;
    
    //tree
    TTree*      tree_;
    std::vector<float>  MuonPt, MuonEta, MuonPhi;
    std::vector<double> MuonEnergy,  MuonCharge;
    
    std::vector<double> PhotonPt, PhotonEt, PhotonEnergy, PhotonEta, PhotonPhi;
    
    std::vector<int> GenParticle_PdgId, GenParticle_MotherPdgId, GenParticle_GrandMotherPdgId;
    std::vector<double> GenParticle_Pt, GenParticle_Eta,    GenParticle_Phi, GenParticle_vx, GenParticle_vy, GenParticle_vz, GenParticle_MotherPt, GenParticle_MotherEta, GenParticle_MotherPhi;

    std::vector<double> GenParticle_Pt_v2, GenParticle_Eta_v2, GenParticle_Phi_v2, GenParticle_PdgId_v2;

    std::vector<double> GenParticle_Pt_trk, GenParticle_Eta_trk, GenParticle_Phi_trk, GenParticle_PdgId_trk;

    //Vtx position
    std::vector<double>  Muon_vx,  Muon_vy,  Muon_vz;
    
    //MuonID
    std::vector<double>  Muon_isGlobal,  Muon_isTracker,  Muon_isSoft, Muon_isMVA, Muon_isMVASoft,  Muon_isLoose, Muon_isMedium, Muon_isTight,  Muon_isPF,  Muon_isRPCMuon,  Muon_isStandAloneMuon,  Muon_isTrackerMuon,  Muon_isCaloMuon,  Muon_isQualityValid,  Muon_isTimeValid,  Muon_isIsolationValid,  Muon_numberOfMatchedStations,  Muon_numberOfMatches;
    
    //MuonTime
    std::vector<double>  Muon_timeAtIpInOut, Muon_timeAtIpInOutErr;
    
    //Muon inner + outer track
    std::vector<double>  Muon_GLnormChi2, Muon_GLhitPattern_numberOfValidMuonHits,  Muon_trackerLayersWithMeasurement,  Muon_Numberofvalidpixelhits,  Muon_outerTrack_p,  Muon_outerTrack_eta, Muon_outerTrack_phi,  Muon_outerTrack_normalizedChi2,  Muon_outerTrack_muonStationsWithValidHits,  Muon_innerTrack_p,  Muon_innerTrack_eta,  Muon_innerTrack_phi,  Muon_innerTrack_normalizedChi2,  Muon_QInnerOuter;
    
    std::vector<double>   Muon_combinedQuality_updatedSta,  Muon_combinedQuality_trkKink,  Muon_combinedQuality_glbKink,  Muon_combinedQuality_trkRelChi2,  Muon_combinedQuality_staRelChi2,  Muon_combinedQuality_chi2LocalPosition,  Muon_combinedQuality_chi2LocalMomentum,  Muon_combinedQuality_localDistance,  Muon_combinedQuality_globalDeltaEtaPhi,  Muon_combinedQuality_tightMatch,  Muon_combinedQuality_glbTrackProbability,  Muon_calEnergy_em,  Muon_calEnergy_emS9,  Muon_calEnergy_emS25,  Muon_calEnergy_had,  Muon_calEnergy_hadS9,  Muon_segmentCompatibility,  Muon_caloCompatibility,  Muon_ptErrOverPt, Muon_BestTrackPt,  Muon_BestTrackPtErr, Muon_BestTrackEta,  Muon_BestTrackEtaErr,  Muon_BestTrackPhi,  Muon_BestTrackPhiErr;
    
    std::vector<int>  Muon_simPdgId, Muon_simMotherPdgId, Muon_simFlavour,  Muon_simType, Muon_simBX, Muon_simHeaviestMotherFlavour;
    std::vector<double> Mu1_Pt, Mu1_Eta, Mu1_Phi, Mu2_Pt, Mu2_Eta, Mu2_Phi, Mu3_Pt, Mu3_Eta, Mu3_Phi, Mu3_Charge, Mu4_Pt, Mu4_Eta, Mu4_Phi, Mu4_Charge, GenMatchMu1_SimPt, GenMatchMu2_SimPt, GenMatchMu3_SimPt, GenMatchMu4_SimPt, GenMatchMu1_SimEta, GenMatchMu2_SimEta, GenMatchMu3_SimEta, GenMatchMu4_SimEta, GenMatchMu1_SimPhi, GenMatchMu2_SimPhi, GenMatchMu3_SimPhi, GenMatchMu4_SimPhi, GenMatchMu1_Pt, GenMatchMu2_Pt, GenMatchMu3_Pt, GenMatchMu4_Pt, GenMatchMu1_Eta, GenMatchMu2_Eta, GenMatchMu3_Eta, GenMatchMu4_Eta, GenMatchMu1_Phi, GenMatchMu2_Phi, GenMatchMu3_Phi, GenMatchMu4_Phi;
    std::vector<double> mu1_pfreliso03, mu2_pfreliso03, mu3_pfreliso03, mu4_pfreliso03, vtx_prob, vtx_ref_prob;

    std::vector<double> RefTrack1_Pt, RefTrack1_Eta, RefTrack1_Phi, RefTrack1_QuadrupletIndex;
    std::vector<double> RefTrack2_Pt, RefTrack2_Eta, RefTrack2_Phi, RefTrack2_QuadrupletIndex;
    std::vector<double> RefTrack3_Pt, RefTrack3_Eta, RefTrack3_Phi, RefTrack3_QuadrupletIndex;
    std::vector<double> RefTrack4_Pt, RefTrack4_Eta, RefTrack4_Phi, RefTrack4_QuadrupletIndex;
    
    std::vector<double> RefittedSV_Chi2, RefittedSV_nDOF, RefittedSV_Mass, RefittedSV_Mass_err;
    
    std::vector<double> IsoTrackMu1_Pt, IsoTrackMu1_Eta, IsoTrackMu1_Phi;
    std::vector<double> IsoTrackMu2_Pt, IsoTrackMu2_Eta, IsoTrackMu2_Phi;
    std::vector<double> IsoTrackMu3_Pt, IsoTrackMu3_Eta, IsoTrackMu3_Phi;
    std::vector<double> IsoTrackMu4_Pt, IsoTrackMu4_Eta, IsoTrackMu4_Phi;
    
    std::vector<float> Mu1_dRtriggerMatch, Mu2_dRtriggerMatch, Mu3_dRtriggerMatch, Mu4_dRtriggerMatch;
    
    std::vector<double> Muon_emEt03, Muon_hadEt03, Muon_nJets03, Muon_nTracks03, Muon_sumPt03, Muon_emEt05,    Muon_hadEt05, Muon_nJets05, Muon_nTracks05, Muon_sumPt05,Muon_hadVetoEt03,Muon_emVetoEt03,    Muon_trackerVetoPt03,    Muon_hadVetoEt05,    Muon_emVetoEt05,    Muon_trackerVetoPt05;
    
    std::vector<double>     Quadruplet_mindca_iso, Quadruplet_relativeiso, Quadruplet_relativeiso2;
    
    std::vector<int>  Mu1_QuadrupletIndex,  Mu2_QuadrupletIndex,  Mu3_QuadrupletIndex, Mu4_QuadrupletIndex;
    std::vector<int>  Mu1_NTracks03iso,  Mu2_NTracks03iso,  Mu3_NTracks03iso, Mu4_NTracks03iso;
    
    int QuadrupletCollectionSize, PVCollection_Size, MuonCollectionSize;
    int PhotonCollectionSize;
    std::vector<double>  QuadrupletVtx_x,  QuadrupletVtx_y,  QuadrupletVtx_z,  QuadrupletVtx_Chi2,  QuadrupletVtx_NDOF,  Quadruplet_Mass,  Quadruplet_Pt,  Quadruplet_Eta,  Quadruplet_Phi, Quadruplet_Charge;
    std::vector<std::vector<double>>  QuadrupletVtx_cov;
    
    std::vector<double> dxy_mu1, dxy_mu2, dxy_mu3, dxy_mu4, dxyErr_mu1, dxyErr_mu2, dxyErr_mu3, dxyErr_mu4;
    std::vector<double> mu1_bs_dxy, mu2_bs_dxy, mu3_bs_dxy, mu4_bs_dxy, mu1_bs_dxy_err, mu2_bs_dxy_err, mu3_bs_dxy_err, mu4_bs_dxy_err, mu1_bs_dxy_sig, mu2_bs_dxy_sig, mu3_bs_dxy_sig, mu4_bs_dxy_sig;

    std::vector<double>  RefittedPV_x;
    std::vector<double>  RefittedPV_y;
    std::vector<double>  RefittedPV_z;
    std::vector<std::vector<double>>  RefittedPV_cov;
    std::vector<double>  RefittedPV_NTracks;
    std::vector<int>     RefittedPV_isValid;
    std::vector<double>  RefittedPV_Chi2, RefittedPV_nDOF;
    std::vector<double>  PV_bis_Chi2, PV_bis_nDOF;
    
    std::vector<double>  FlightDistPVSV;
    std::vector<double>  FlightDistPVSV_Err;
    std::vector<double>  FlightDistPVSV_Significance;
    std::vector<double>  FlightDistPVSV_chi2;
    
    std::vector<double> PV_x,  PV_y,  PV_z,  PV_NTracks;
    std::vector<double> BS_x,  BS_y,  BS_z;
    std::vector<double> Vtx12_x, Vtx23_x, Vtx13_x, Vtx14_x, Vtx24_x, Vtx34_x, Vtx12_y, Vtx23_y, Vtx13_y, Vtx14_y, Vtx24_y, Vtx34_y, Vtx12_z, Vtx23_z, Vtx13_z, Vtx14_z, Vtx24_z, Vtx34_z, Vtx12_Chi2, Vtx23_Chi2, Vtx13_Chi2, Vtx14_Chi2, Vtx24_Chi2, Vtx34_Chi2, Vtx12_nDOF, Vtx23_nDOF, Vtx13_nDOF, Vtx14_nDOF, Vtx24_nDOF, Vtx34_nDOF;
    std::vector<double> Vtx12_mass, Vtx23_mass, Vtx13_mass, Vtx14_mass, Vtx24_mass, Vtx34_mass, Vtx12_mass_err, Vtx23_mass_err, Vtx13_mass_err, Vtx14_mass_err, Vtx24_mass_err, Vtx34_mass_err;

    std::vector<int> NGoodQuadruplets;
    uint  evt, run, lumi, puN;
    std::vector<string>  Trigger_l1name;
    std::vector<int> Trigger_l1Initialdecision, Trigger_l1Finaldecision;
    std::vector<double> Trigger_l1prescale;
    
    std::vector<string>  Trigger_hltname;
    std::vector<int> Trigger_hltdecision;
    
    std::vector<double> MuonPt_HLT,  MuonEta_HLT,  MuonPhi_HLT;
    
    std::vector<double>  Muon_innerTrack_highPurity,  Muon_innerTrack_ValidFraction, Muon_Numberofvalidtrackerhits, Muon_validMuonHitComb, Muon_IP2D_BS,  Muon_IP3D_BS,  Muon_IP2D_PV,  Muon_IP3D_PV, Muon_SoftMVA_Val;
    std::vector<double>  DistXY_PVSV,  DistXY_significance_PVSV;
    std::vector<double>  Quadruplet_IsoMu1, Quadruplet_IsoMu2, Quadruplet_IsoMu3, Quadruplet_IsoMu4;
    std::vector<double>  FlightDistBS_SV,  FlightDistBS_SV_Err,  FlightDistBS_SV_Significance;
    
    std::vector<double>  Mu1_IsGlobal, Mu2_IsGlobal, Mu3_IsGlobal, Mu4_IsGlobal, Mu1_IsPF, Mu2_IsPF, Mu3_IsPF, Mu4_IsPF;
    
    std::vector<double> L1Muon_Pt, L1Muon_Eta, L1Muon_Phi, L1Muon_EtaAtVtx, L1Muon_PhiAtVtx, L1Muon_BX, L1Muon_Quality, L1Muon_Charge, L1Muon_ChargeValid, L1Muon_TfMuonIndex, L1Muon_rank, L1Muon_isoSum;
    
};



MiniAnaB2Mu2K::MiniAnaB2Mu2K(const edm::ParameterSet& iConfig){
    edm::InputTag algInputTag_;
    isMc = iConfig.getUntrackedParameter<bool>("isMcLabel");
    is2K = iConfig.getUntrackedParameter<bool>("is2KLabel");
    isAna = iConfig.getUntrackedParameter<bool>("isAnaLabel");
    muons_ = consumes<edm::View<pat::Muon> >  (iConfig.getParameter<edm::InputTag>("muonLabel"));
    //photons_ = consumes<edm::View<pat::Photon> >  (iConfig.getParameter<edm::InputTag>("photonLabel"));
    vertex_ = consumes<edm::View<reco::Vertex> > (iConfig.getParameter<edm::InputTag>("VertexLabel"));
    trackToken_ = consumes<edm::View<pat::PackedCandidate> > (iConfig.getParameter<edm::InputTag>("TracksLabel"));
    srcCands_ = consumes<std::vector<pat::PackedCandidate> >(edm::InputTag("packedPFCandidates"));
    genParticles_ = consumes<edm::View<reco::GenParticle>  > (iConfig.getParameter<edm::InputTag>("genParticleLabel"));
    genParticlesn2_ = consumes<edm::View<pat::PackedGenParticle>  > (iConfig.getParameter<edm::InputTag>("genParticleLabeln2"));
    Cand2Mu2Tracks_ = consumes<edm::View<reco::CompositeCandidate> > (iConfig.getParameter<edm::InputTag>("Cand2Mu2TracksLabel"));
    puToken_ =   consumes<std::vector<PileupSummaryInfo> >(iConfig.getParameter<edm::InputTag>("pileupSummary"));
    triggerToken_ = consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("triggerResults"));
    triggerObjects_ = consumes<std::vector<pat::TriggerObjectStandAlone> >(iConfig.getParameter<edm::InputTag>("objects"));
    algToken_ = consumes<BXVector<GlobalAlgBlk>>(iConfig.getParameter<edm::InputTag>("AlgInputTag"));
    algTok_ = consumes<BXVector<GlobalAlgBlk>>(iConfig.getParameter<edm::InputTag>("algInputTag"));
    gtUtil_ = new l1t::L1TGlobalUtil(iConfig, consumesCollector(), *this, algTag_, extTag_, l1t::UseEventSetupIn::Event);
    //
    token_BeamSpot = consumes<reco::BeamSpot>(edm::InputTag("offlineBeamSpot"));
    l1muonsToken_ = consumes<BXVector<l1t::Muon>>(edm::InputTag("gmtStage2Digis", "Muon"  , "RECO"));
    theTransientTrackBuilder_ = esConsumes<TransientTrackBuilder, TransientTrackRecord>(edm::ESInputTag("", "TransientTrackBuilder"));
}

MiniAnaB2Mu2K::~MiniAnaB2Mu2K(){
    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)
}


float MiniAnaB2Mu2K::dR(float eta1, float eta2, float phi1, float phi2){
    float dphi=(phi1-phi2);
    float deta=(eta1-eta2);
    float deltaR= TMath::Sqrt(dphi*dphi + deta*deta);
    return deltaR;
}

float MiniAnaB2Mu2K::dRtriggerMatch(pat::Muon m, vector<pat::TriggerObjectStandAlone> triggerObjects) {
    //float MiniAnaB2Mu2K::dRtriggerMatch(pat::Muon m, trigger::TriggerObjectCollection triggerObjects) {
    float dRmin = 1.;
    for (unsigned int i = 0 ; i < triggerObjects.size() ; i++) {
        float deltaR = sqrt( reco::deltaR2(triggerObjects[i].eta(), triggerObjects[i].phi(), m.eta(), m.phi()));
        //float deltaR = sqrt( pow(triggerObjects[i].eta() - m.eta(), 2) + pow(acos(cos(triggerObjects[i].phi() - m.phi())), 2));
        if (deltaR < dRmin) dRmin = deltaR;
    }
    return dRmin;
}

float MiniAnaB2Mu2K::dRtriggerMatchTrk(reco::Track Trk, vector<pat::TriggerObjectStandAlone> triggerObjects) {
    float dRmin = 1.;
    for (unsigned int i = 0 ; i < triggerObjects.size() ; i++) {
        float deltaR = sqrt( reco::deltaR2(triggerObjects[i].eta(), triggerObjects[i].phi(), Trk.eta(), Trk.phi()));
        //float deltaR = sqrt( pow(triggerObjects[i].eta() - m.eta(), 2) + pow(acos(cos(triggerObjects[i].phi() - m.phi())), 2));
        if (deltaR < dRmin) dRmin = deltaR;
    }
    return dRmin;
}

double PFreliso03(pat::Muon imu){
    return (imu.pfIsolationR03().sumChargedHadronPt + std::max(imu.pfIsolationR03().sumNeutralHadronEt + imu.pfIsolationR03().sumPhotonEt - 0.5 * imu.pfIsolationR03().sumPUPt, 0.0)) / imu.pt();
}

bool isGoodTrack(const reco::Track &track) {
    if(track.pt()>1){
        if(std::fabs(track.eta())<2.5){
            if(track.hitPattern().trackerLayersWithMeasurement()>5){
                if(track.hitPattern().pixelLayersWithMeasurement()>1) return true;
            }
        }
    }
    return false;
}

typedef std::map<const reco::Track*, reco::TransientTrack> TransientTrackMap;
// auxiliary function to exclude tracks associated to tau lepton decay "leg"
// from primary event vertex refit
bool tracksMatchByDeltaR(const reco::Track* trk1, const reco::Track* trk2)
{
    //cout<<" pv_t eta="<<trk1->eta()<<" sv_t eta="<<trk2->eta()<<" deltaR(tk1, tk2)="<<reco::deltaR(*trk1, *trk2)<<endl;
    if ( reco::deltaR(*trk1, *trk2) < 1.e-2 && trk1->charge() == trk2->charge() ) return true;
    else return false;
}

bool tracksMatchByDeltaR2(const reco::TransientTrack trk1, const reco::Track* trk2)
{
    //cout<<" ++++++++ tracksMatchByDeltaR2 sv_t pt="<<trk2->pt()<<" eta="<<trk2->eta()<<" phi="<<trk2->phi()<<endl;
    //cout<<" ++++++++ tracksMatchByDeltaR2 pv_t pt="<<trk1.track().pt()<<" eta="<<trk1.track().eta()<<" phi="<<trk1.track().phi()<<" deltaR(tk1, tk2)="<<reco::deltaR(trk1.track(), *trk2)<<endl;
    if ( reco::deltaR(trk1.track(), *trk2) < 1.e-2 && trk1.track().charge() == trk2->charge() ) return true;
    else return false;
}

void removeTracks(TransientTrackMap& pvTracks_toRefit, const std::vector<reco::Track*> svTracks)
{
    //  cout<<"Size PV Trk: "<<(pvTracks_toRefit->first).size()<<endl;
    for ( std::vector<reco::Track*>::const_iterator svTrack = svTracks.begin(); svTrack != svTracks.end(); ++svTrack ){
        for ( TransientTrackMap::iterator pvTrack = pvTracks_toRefit.begin(); pvTrack != pvTracks_toRefit.end(); ++pvTrack ) {
            //cout<<"Eta PV Trk:"<<pvTrack->first->eta()<<endl;
            if ( tracksMatchByDeltaR(pvTrack->first, *svTrack) ) {
                pvTracks_toRefit.erase(pvTrack);
                break;
            }
        }
    }
}

void removeTracks2(std::vector<reco::Track*> pvTracks, const std::vector<reco::Track*> svTracks)
{
    for ( std::vector<reco::Track*>::const_iterator svTrack = svTracks.begin(); svTrack != svTracks.end(); ++svTrack ){
        for ( std::vector<reco::Track*>::const_iterator pvTrack = pvTracks.begin(); pvTrack != pvTracks.end(); ++pvTrack ){
            if ( tracksMatchByDeltaR(*pvTrack, *svTrack) ) {
                pvTracks.erase(pvTrack);
                break;
            }
        }
    }
}


void removeTracks3(vector<reco::TransientTrack> &pvTracks, const std::vector<reco::Track*> svTracks)
{
    //cout<<" ++++removeTracks3: tracks associated to PV = "<<pvTracks.size()<<endl;
    int svtrack_cout = 0;
    for ( std::vector<reco::Track*>::const_iterator svTrack = svTracks.begin(); svTrack != svTracks.end(); ++svTrack ){
        ++svtrack_cout;
        //cout<<" ++++removeTracks3: looking for matching with svTrack "<<svtrack_cout<<endl;
        for(uint f=0;f<pvTracks.size(); f++){
            if ( tracksMatchByDeltaR2(pvTracks.at(f), *svTrack) ) {
                //cout<<"     track to be erased position: "<<f<<" pt="<<pvTracks.at(f).track().pt()<<" eta="<<pvTracks.at(f).track().eta()<<" phi="<<pvTracks.at(f).track().phi()<<endl;
                pvTracks.erase(pvTracks.begin()+f);
                break;
            }
        }
    }
}

void MiniAnaB2Mu2K::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;
    using namespace std;
    using namespace reco;
    using std::vector;
    
    edm::Handle< edm::View<reco::Vertex> >vertices;
    iEvent.getByToken(vertex_, vertices);
    if (vertices->empty()) return; // skip the event if no PV found
    const reco::Vertex &PV = vertices->front();
    
    edm::Handle< edm::View<pat::Muon> > muons;
    iEvent.getByToken(muons_, muons);
    
    edm::Handle<edm::View<reco::CompositeCandidate> > Cand2Mu2Tracks;
    iEvent.getByToken(Cand2Mu2Tracks_, Cand2Mu2Tracks);
    
    edm::Handle< edm::View<reco::GenParticle> > genParticles;
    iEvent.getByToken(genParticles_, genParticles);

    edm::Handle< edm::View<pat::PackedGenParticle> > genParticlesn2;
    iEvent.getByToken(genParticlesn2_, genParticlesn2);

    edm::Handle<edm::View<pat::PackedCandidate> > trackCollection;
    iEvent.getByToken(trackToken_,trackCollection);
    
    Handle<TriggerResults> triggerResults;
    iEvent.getByToken(triggerToken_, triggerResults);
        
    //edm::Handle< edm::View<pat::Photon> > photons;
    //iEvent.getByToken(photons_, photons);
    
    Handle<std::vector<pat::TriggerObjectStandAlone> > triggerObjects;
    iEvent.getByToken(triggerObjects_, triggerObjects);
    
    Handle<BXVector<GlobalAlgBlk>> alg;
    iEvent.getByToken(algToken_,alg);
    
    Handle<BXVector<l1t::Muon> > gmuons;
    iEvent.getByToken(l1muonsToken_, gmuons);
    
    edm::ESHandle<TransientTrackBuilder> theTransientTrackBuilder = iSetup.getHandle(theTransientTrackBuilder_);
    
    edm::Handle<std::vector<pat::PackedCandidate> > PFCands;
    iEvent.getByToken(srcCands_,PFCands);
    
    reco::BeamSpot beamSpot;
    edm::Handle<reco::BeamSpot> beamSpotHandle;
    iEvent.getByToken(token_BeamSpot, beamSpotHandle);
    double x_bs = 0.0;
    double y_bs = 0.0;
    double z_bs = 0.0;
    if ( beamSpotHandle.isValid() ) {
        beamSpot = *beamSpotHandle;
        x_bs = beamSpot.x0();
        y_bs = beamSpot.y0();
        z_bs = beamSpot.z0();
    } else {
        cout << "No beam spot available from EventSetup \n" << endl;
    }
    
    for (int ibx = gmuons->getFirstBX(); ibx <= gmuons->getLastBX(); ++ibx) {
        for (auto itr = gmuons->begin(ibx); itr != gmuons->end(ibx); ++itr) {
            L1Muon_Pt.push_back(itr->pt());
            L1Muon_Eta.push_back(itr->eta());
            L1Muon_Phi.push_back(itr->phi());
            L1Muon_EtaAtVtx.push_back(itr->etaAtVtx());
            L1Muon_PhiAtVtx.push_back(itr->phiAtVtx());
            L1Muon_BX.push_back(ibx);
            L1Muon_Quality.push_back(itr->hwQual());
            L1Muon_Charge.push_back(itr->charge());
            L1Muon_ChargeValid.push_back(itr->hwChargeValid());
            L1Muon_TfMuonIndex.push_back(itr->tfMuonIndex());
            L1Muon_rank.push_back(itr->hwRank());
            L1Muon_isoSum.push_back(itr->hwIso());
            cout << "Muon : "
            << " BX=" << ibx << " ipt=" << itr->pt() << " eta=" << itr->eta()
            << " phi=" << itr->phi() << " hweta="<< itr->hwEta()<<" charge="<<itr->charge()<< "rank="<<itr->hwRank()<<std::endl;
        }
    }
    
    hEvents->Fill(1);
    
    ///////////////Fill Trigger Vars, L1 and HLT///////////////
    
    cout << "I'm here L1" << endl;
    gtUtil_->retrieveL1(iEvent, iSetup, algTok_);
    cout << "I'm here L1 bis" << endl;
    const vector<pair<string, bool> > initialDecisions = gtUtil_->decisionsInitial();
    const vector<pair<string, bool> > finalDecisions = gtUtil_->decisionsFinal();
    const vector<pair<string, double> > PSValues = gtUtil_->prescales();
    
    if(initialDecisions.size() != finalDecisions.size())
        cout << "L1 initial and final decisions have different size!" << endl;
    if (!iEvent.isRealData())
    {
        //cout<<"sto qua is MC"<<endl;
        for (size_t i_l1t = 0; i_l1t < initialDecisions.size(); i_l1t++)
        {
            string l1tName = (initialDecisions.at(i_l1t)).first;
            //cout<<"l1 name="<<l1tName<<endl;
            if(l1tName.find("DoubleMu") != string::npos || l1tName.find("TripleMu") != string::npos ||  l1tName.find("SingleMu")!= string::npos ){
                //cout<<"l1 name="<<l1tName<<endl;
                Trigger_l1name.push_back( l1tName );
                Trigger_l1Initialdecision.push_back( initialDecisions.at(i_l1t).second );
                Trigger_l1Finaldecision.push_back( finalDecisions.at(i_l1t).second );
                Trigger_l1prescale.push_back( 1 );
            }
        }
    }
    else
    {
        //ESHandle<L1TGlobalPrescalesVetos> psAndVetos;
        //auto psRcd = iSetup.tryToGet<L1TGlobalPrescalesVetosRcd>();
        //if(psRcd) psRcd->get(psAndVetos);
        //int columnN= gtUtil_->prescaleColumn();
        for (size_t i_l1t = 0; i_l1t < initialDecisions.size(); i_l1t++) {
            string l1tName = (initialDecisions.at(i_l1t)).first;
            if(l1tName.find("DoubleMu") != string::npos || l1tName.find("TripleMu") != string::npos ||  l1tName.find("SingleMu")!= string::npos){
                //cout<<"L1Seed="<<l1tName<<" decision="<<initialDecisions.at(i_l1t).second<<" prescale="<<(psAndVetos->prescale_table_)[columnN][i_l1t]<<endl;
                Trigger_l1name.push_back( l1tName );
                Trigger_l1Initialdecision.push_back( initialDecisions.at(i_l1t).second );
                Trigger_l1Finaldecision.push_back( finalDecisions.at(i_l1t).second );
                Trigger_l1prescale.push_back( PSValues.at(i_l1t).second );
            }
        }
    }
    
    
    const TriggerNames &triggerNames = iEvent.triggerNames( *triggerResults );
    for (size_t i_hlt = 0; i_hlt != triggerResults->size(); ++i_hlt){
        string hltName = triggerNames.triggerName(i_hlt);
        
        if(hltName.find("HLT_DoubleMu") != string::npos || hltName.find("HLT_DoubleMu3") != string::npos  || hltName.find("HLT_Mu8_IP") != string::npos || (hltName.find("HLT_Mu7_IP") != string::npos) || (hltName.find("HLT_Mu9_IP") != string::npos) || (hltName.find("HLT_Mu12_IP") != string::npos)  ){
            //if(hltName.find("HLT_DoubleMu") != string::npos  || hltName.find("HLT_Mu8_IP") != string::npos || (hltName.find("HLT_Mu7_IP") != string::npos) || (hltName.find("HLT_Mu9_IP") != string::npos) || (hltName.find("HLT_Mu12_IP") != string::npos)  ){
            //cout<<" HLTPath="<<hltName<<" isPassed="<<triggerResults->accept(i_hlt )<<endl;
            Trigger_hltname.push_back(hltName);
            Trigger_hltdecision.push_back(triggerResults->accept(i_hlt ));
        }
    }
    
    vector<pat::TriggerObjectStandAlone> TriggerObj_B4Mu;
    
    for (pat::TriggerObjectStandAlone obj : *triggerObjects) { // note: not "const &" since we want to call unpackPathNames
        obj.unpackPathNames(triggerNames);
        
        for (unsigned h = 0; h < obj.filterLabels().size(); ++h) {
            if(obj.filterLabels()[h]=="hltDisplacedmumuFilterDoubleMu43LowMass"){
                TriggerObj_B4Mu.push_back(obj);
            }
        }//loop on filterLabels
    }
    
    for(uint t=0; t<TriggerObj_B4Mu.size();t++){
        MuonPt_HLT.push_back(TriggerObj_B4Mu.at(t).pt());
        MuonEta_HLT.push_back(TriggerObj_B4Mu.at(t).eta());
        MuonPhi_HLT.push_back(TriggerObj_B4Mu.at(t).phi());
    }
        
    //

    ///////////////Fill Genparticles ///////////////
    if(isMc){
        uint j=0;
        uint ngenP=genParticlesn2->size();
        
        for(edm::View<pat::PackedGenParticle>::const_iterator gp=genParticlesn2->begin(); gp!=genParticlesn2->end(), j<ngenP; ++gp , ++j){
            if( (fabs(gp->pdgId())==13 || fabs(gp->pdgId())==321 || fabs(gp->pdgId())==211) && (fabs(gp->mother(0)->pdgId())==333 || fabs(gp->mother(0)->pdgId())==313 || fabs(gp->mother(0)->pdgId())==443) ){ //mu K+ pi
                GenParticle_PdgId.push_back(gp->pdgId());
                GenParticle_Pt.push_back(gp->pt());
                GenParticle_Eta.push_back(gp->eta());
                GenParticle_Phi.push_back(gp->phi());
                GenParticle_vx.push_back(gp->vx());
                GenParticle_vy.push_back(gp->vy());
                GenParticle_vz.push_back(gp->vz());
                if (gp->numberOfMothers()) {
                    GenParticle_MotherPdgId.push_back(gp->mother(0)->pdgId());
                    GenParticle_MotherPt.push_back(gp->mother(0)->pt());
                    GenParticle_MotherEta.push_back(gp->mother(0)->eta());
                    GenParticle_MotherPhi.push_back(gp->mother(0)->phi());
                    if(gp->mother(0)->mother(0)) {
                        GenParticle_GrandMotherPdgId.push_back(gp->mother(0)->mother(0)->pdgId());
                    }else{
                        GenParticle_GrandMotherPdgId.push_back(-99);
                    }
                }else{
                    GenParticle_MotherPdgId.push_back(-99);
                    GenParticle_MotherPt.push_back(-99);
                    GenParticle_MotherEta.push_back(-99);
                    GenParticle_MotherPhi.push_back(-99);
                }
            } //pdgId
        }
    } //End GenParticles

    ///////////////Fill Genparticles_v2 ///////////////
    if(isMc){
        uint j=0;
        uint ngenP=genParticles->size();
     
        for(edm::View<reco::GenParticle>::const_iterator gp=genParticles->begin(); gp!=genParticles->end(), j<ngenP; ++gp , ++j){
            if( fabs(gp->pdgId())==531  || fabs(gp->pdgId())==533 || fabs(gp->pdgId())==511) { // B0s B0*s B0        
                int number_good_GrandDaughters_mu=0;
                int number_good_GrandDaughters_KK=0;
                int number_good_GrandDaughters_pi=0;
                int number_good_GrandDaughters_K=0;
                int number_phi=0;
                int number_Kstar=0;
                int number_jpsi=0;
                if (gp->numberOfDaughters() > 0) {
                    for (uint k = 0; k < gp->numberOfDaughters(); ++k) {
                        const reco::GenParticle* daughter = dynamic_cast<const reco::GenParticle*>(gp->daughter(k));
                        //const reco::Candidate* daughter = gp->daughter(k);
                        if (fabs(daughter->pdgId())==333) number_phi++;
                        if (fabs(daughter->pdgId())==443) number_jpsi++;
                        if (fabs(daughter->pdgId())==313) number_Kstar++;
                        if (fabs(daughter->pdgId())==443){
                            if (daughter->numberOfDaughters() > 0 ) {
                                for (uint l = 0; l < daughter->numberOfDaughters(); ++l) {
                                    const reco::Candidate* granddaughter = daughter->daughter(l);
                                    if (fabs(granddaughter->pdgId())==13) number_good_GrandDaughters_mu++;
                                }
                            }
                        }
                        if (fabs(daughter->pdgId())==333){
                            if (daughter->numberOfDaughters() > 0 ) {
                                for (uint l = 0; l < daughter->numberOfDaughters(); ++l) {
                                    const reco::Candidate* granddaughter = daughter->daughter(l);
                                    if (fabs(granddaughter->pdgId())==321) number_good_GrandDaughters_KK++;
                                }
                            }
                        } 
                        if (fabs(daughter->pdgId())==313){
                            for(uint jj=0; jj<GenParticle_PdgId.size(); jj++){
                                if( (fabs(GenParticle_PdgId.at(jj))==321 || fabs(GenParticle_PdgId.at(jj))==211) && (fabs(GenParticle_MotherPdgId.at(jj))==313) && (GenParticle_MotherEta.at(jj) == daughter->eta()) && (GenParticle_MotherPhi.at(jj) == daughter->phi())){
                                    if(fabs(GenParticle_PdgId.at(jj)) == 321) number_good_GrandDaughters_K++;
                                    if(fabs(GenParticle_PdgId.at(jj)) == 211) number_good_GrandDaughters_pi++;
                                }
                            }
                        }
                    }
                }
                if(number_good_GrandDaughters_KK==2 && number_good_GrandDaughters_mu==2 && number_phi==1 && number_jpsi==1 && is2K==true){
                    for (uint k = 0; k < gp->numberOfDaughters(); ++k) {
                        const reco::GenParticle* daughter = dynamic_cast<const reco::GenParticle*>(gp->daughter(k));
                        if (fabs(daughter->pdgId())==443){
                            for (uint l = 0; l < daughter->numberOfDaughters(); ++l) {
                                const reco::Candidate* granddaughter = daughter->daughter(l);
                                if (fabs(granddaughter->pdgId())==13){
                                    GenParticle_Pt_v2.push_back(granddaughter->pt());
                                    GenParticle_Eta_v2.push_back(granddaughter->eta());
                                    GenParticle_Phi_v2.push_back(granddaughter->phi());
                                    GenParticle_PdgId_v2.push_back(granddaughter->pdgId());
                                }
                            }
                        }
                        if (fabs(daughter->pdgId())==333){
                            for (uint l = 0; l < daughter->numberOfDaughters(); ++l) {
                                const reco::Candidate* granddaughter = daughter->daughter(l);
                                if (fabs(granddaughter->pdgId())==321){
                                    GenParticle_Pt_trk.push_back(granddaughter->pt());
                                    GenParticle_Eta_trk.push_back(granddaughter->eta());
                                    GenParticle_Phi_trk.push_back(granddaughter->phi());
                                    GenParticle_PdgId_trk.push_back(granddaughter->pdgId());
                                }
                            }
                        }
                    }
                }
                if(number_good_GrandDaughters_pi==1 && number_good_GrandDaughters_K==1 && number_good_GrandDaughters_mu==2 && number_Kstar==1 && number_jpsi==1 && is2K==false){
                //if(number_good_GrandDaughters_mu==2 && number_Kstar==1 && number_jpsi==1 && is2K==false){
                    for (uint k = 0; k < gp->numberOfDaughters(); ++k) {
                        const reco::GenParticle* daughter = dynamic_cast<const reco::GenParticle*>(gp->daughter(k));
                        if (fabs(daughter->pdgId())==443){
                            for (uint l = 0; l < daughter->numberOfDaughters(); ++l) {
                                const reco::Candidate* granddaughter = daughter->daughter(l);
                                if (fabs(granddaughter->pdgId())==13){
                                    GenParticle_Pt_v2.push_back(granddaughter->pt());
                                    GenParticle_Eta_v2.push_back(granddaughter->eta());
                                    GenParticle_Phi_v2.push_back(granddaughter->phi());
                                    GenParticle_PdgId_v2.push_back(granddaughter->pdgId());
                                }
                            }
                        }
                        if (fabs(daughter->pdgId())==313){
                            for(uint jj=0; jj<GenParticle_PdgId.size(); jj++){
                                if( (fabs(GenParticle_PdgId.at(jj))==321 || fabs(GenParticle_PdgId.at(jj))==211) && (fabs(GenParticle_MotherPdgId.at(jj))==313) && (GenParticle_MotherEta.at(jj) == daughter->eta()) && (GenParticle_MotherPhi.at(jj) == daughter->phi())){
                                    GenParticle_Pt_trk.push_back(GenParticle_Pt.at(jj));
                                    GenParticle_Eta_trk.push_back(GenParticle_Eta.at(jj));
                                    GenParticle_Phi_trk.push_back(GenParticle_Phi.at(jj));
                                    GenParticle_PdgId_trk.push_back(GenParticle_PdgId.at(jj));
                                }
                            }
                        }
                    }
                }
                if(number_good_GrandDaughters_mu>2) cout<<"number_good_GrandDaughters>2"<<endl;
            }
        }
    } //End GenParticles_v2
    
    //Primary Vtx
    PVCollection_Size = vertices->size();
    cout<<" PV size ="<<vertices->size()<<endl;
    
    //ParticleFlow Candidates
    uint kk=0;
    std::vector<uint> VtxIdV;
    std::vector<uint> SelectedCandIdx;
    vector<pat::PackedCandidate> MyPFCands;
    cout<<"    Number of PFCands="<<PFCands->size()<<endl;
    for (std::vector<pat::PackedCandidate>::const_iterator cand = PFCands->begin(); cand != PFCands->end(), kk!= PFCands->size(); ++cand, ++kk) {
        
        if (cand->charge()==0 || cand->vertexRef().isNull() ) continue;
        //if (!(cand->hasTrackDetails()) ) continue;
        
        int key = cand->vertexRef().key();
        int quality = cand->pvAssociationQuality();
        if(cand->fromPV(cand->vertexRef().key())<2) continue;
        if(cand->fromPV(cand->vertexRef().key())==2 && quality!=pat::PackedCandidate::UsedInFitLoose) continue;
        
        if ( !(cand->bestTrack()) ) continue;
        //cout<<kk<<" vtx ref key="<<key<<" cand pt="<<cand->pt()<<" vtx x="<<cand->vertexRef()->x()<<endl;
        VtxIdV.push_back(key);
        SelectedCandIdx.push_back(kk);
        MyPFCands.push_back(*cand);
    } // loop PFCandidates
    
    sort( VtxIdV.begin(), VtxIdV.end() );
    VtxIdV.erase( unique(VtxIdV.begin(), VtxIdV.end() ), VtxIdV.end() );
    
    typedef vector<double> MyPt;
    vector<MyPt> AssoPtToVtx;
    vector<pat::PackedCandidate> SelPFCands;
    vector<vector<pat::PackedCandidate>> AssoCandToVtx;
    
    //loop on primavy vertices associated to good PF candidates after sorting and cleaning the list VtxIdV
    for(uint i=0; i<VtxIdV.size(); i++){
        vector<double> tmp_pt;
        vector<pat::PackedCandidate> tmp_cand;
        //cout<<i<<"----------Stored Vtx id="<<VtxIdV.at(i)<<"---------------"<<endl;
        for (vector<pat::PackedCandidate>::const_iterator myc = MyPFCands.begin(); myc != MyPFCands.end(); ++myc) {
            //cout<<"cand vtx="<<myc->vertexRef().key()<<" cand pt="<<myc->pt()<<endl;
            if(myc->vertexRef().key()==VtxIdV.at(i)) {
                tmp_pt.push_back(myc->pt());
                tmp_cand.push_back(*myc);
            }
        }
        AssoPtToVtx.push_back(tmp_pt);
        AssoCandToVtx.push_back(tmp_cand);
    }//loop VtxIdV
    //cout<<" AssoCandToVtx.size()="<<AssoCandToVtx.size()<<endl;
    
    vector<vector<reco::TransientTrack>> transTracksAssoToVtx;
    
    for(uint i=0; i<AssoCandToVtx.size(); i++){
        std::auto_ptr<reco::TrackCollection> newTrackCollection = std::auto_ptr<reco::TrackCollection>(new TrackCollection);
        std::vector<reco::TransientTrack> transTracks;
        
        //cout<<i<<" vertex ID="<<VtxIdV.at(i)<<" with trk ass. n="<<AssoCandToVtx.at(i).size()<<endl;
        for(vector<pat::PackedCandidate>::const_iterator c = AssoCandToVtx.at(i).begin(); c != AssoCandToVtx.at(i).end(); ++c) {
            newTrackCollection->push_back(*(c->bestTrack()));
        }
        //cout<<i<<" vertex ID="<<VtxIdV.at(i)<<" newTrackCollection size="<<newTrackCollection->size()<<endl;
        for (std::vector<reco::Track>::const_iterator iter = newTrackCollection->begin(); iter != newTrackCollection->end(); ++iter){
            reco::TransientTrack tt = theTransientTrackBuilder->build(*iter);
            transTracks.push_back(tt);
        }
        transTracksAssoToVtx.push_back(transTracks);
    }//loop AssoCandToVtx
    
    
    
    //Quadruplets  Loop
    vector<int> Mu1C, Mu2C, Mu3C, Mu4C, BMass;
    
    //cout<<"Number Of Quadruplets="<<Cand2Mu2Tracks->size()<<endl;
    std::vector<int> NQuad;
    if(isAna){
        QuadrupletCollectionSize = Cand2Mu2Tracks->size() ;
        int QuadrupletIndex =-99; uint trIn=0;
        for(edm::View<reco::CompositeCandidate>::const_iterator B_It=Cand2Mu2Tracks->begin(); B_It!=Cand2Mu2Tracks->end(), trIn<Cand2Mu2Tracks->size(); ++B_It, ++trIn){
            //cout<<"----------------"<<trIn<<"----------------"<<endl;
            const Candidate * c1 = B_It->daughter(0)->masterClone().get();
            const pat::Muon *mu1 = dynamic_cast<const pat::Muon *>(c1);
            
            const Candidate * c2 = B_It->daughter(1)->masterClone().get();
            const pat::Muon *mu2 = dynamic_cast<const pat::Muon *>(c2);
            
            const Candidate * c3 = B_It->daughter(2)->masterClone().get();
            const reco::Track *Track3 = c3->bestTrack();
            
            const Candidate * c4 = B_It->daughter(3)->masterClone().get();
            const reco::Track *Track4 = c4->bestTrack();

            const reco::TransientTrack transientTrack3=theTransientTrackBuilder->build( Track3 );
            const reco::TransientTrack transientTrack4=theTransientTrackBuilder->build( Track4 );
            if( !(transientTrack3.isValid()) ) continue;
            if( !(transientTrack4.isValid()) ) continue;

            //check overlap among the legs of the triplet
            double dR_13 = sqrt( reco::deltaR2(mu1->eta(), mu1->phi(), c3->eta(), c3->phi()) );
            double dR_23 = sqrt( reco::deltaR2(mu2->eta(), mu2->phi(), c3->eta(), c3->phi()) );
            double dR_14 = sqrt( reco::deltaR2(mu1->eta(), mu1->phi(), c4->eta(), c4->phi()) );
            double dR_24 = sqrt( reco::deltaR2(mu2->eta(), mu2->phi(), c4->eta(), c4->phi()) );
            if(dR_13<0.01 || dR_23<0.01 || dR_14<0.01 || dR_24<0.01) continue;
            if(!(fabs(c1->eta()- c3->eta())>  1.e-6)) continue;
            if(!(fabs(c2->eta()- c3->eta())>  1.e-6)) continue;
            if(!(fabs(c1->eta()- c4->eta())>  1.e-6)) continue;
            if(!(fabs(c2->eta()- c4->eta())>  1.e-6)) continue;
            if(!(B_It->vertexChi2()>0)) continue;

            /////////////////VertexFit///////////////////////////////////
            TrackRef trk1, trk2;
            trk1 = mu1->innerTrack();
            trk2 = mu2->innerTrack();
            const reco::TransientTrack transientTrack1=theTransientTrackBuilder->build( trk1 );
            const reco::TransientTrack transientTrack2=theTransientTrackBuilder->build( trk2 );
            reco::Track Track1 =transientTrack1.track();
            reco::Track Track2 =transientTrack2.track();
            reco::Track Track_3 =transientTrack3.track();
            reco::Track Track_4 =transientTrack4.track();
            reco::Track* TrackRef1=&Track1;
            reco::Track* TrackRef2=&Track2;
            reco::Track* TrackRef3=&Track_3;
            reco::Track* TrackRef4=&Track_4;
            vector<reco::Track*> SVTrackRef;
            SVTrackRef.push_back(TrackRef1);
            SVTrackRef.push_back(TrackRef2);
            SVTrackRef.push_back(TrackRef3);
            SVTrackRef.push_back(TrackRef4);
            //cout<<" track ref vector= "<<SVTrackRef.size()<<endl;
            
            reco::Vertex QuadrupletVtx = reco::Vertex(B_It->vertex(), B_It->vertexCovariance(), B_It->vertexChi2(), B_It->vertexNdof(), B_It->numberOfDaughters() );
            double dphi_pv = -1.0;
            uint primaryvertex_index=0;
            TLorentzVector FourCandidate;
            uint selVtxId = 0;
            
            FourCandidate.SetPtEtaPhiM(B_It->pt(), B_It->eta(), B_It->phi(), B_It->mass());
            //cout<<"B pt="<<B_It->pt()<<" eta="<<B_It->eta()<<" phi="<<B_It->phi()<<" mass="<<B_It->mass()<<endl;
            
            //loop on privary vertices
            //cout<<"Among "<<VtxIdV.size()<<" vertices we select the closest, i.e. maximum Cosdphi_3D"<<endl;
            if(VtxIdV.size()>0 && vertices->size()>0) {
                for(uint VtxIt=0; VtxIt<vertices->size(); VtxIt++ ){
                    //cout << "vertex collection n." << VtxIt << " has size "<< VtxIdV.size() << endl;
                    for(uint k=0; k<VtxIdV.size(); k++){
                        //cout << "   VtxIdV.at("<<k<<") = " << VtxIdV.at(k) << endl;
                        if(VtxIdV[k]==VtxIt){
                            //cout<<"     Vtx id="<<VtxIt<<" x="<<(*vertices)[VtxIt].x()<<" y="<<(*vertices)[VtxIt].y()<<" z="<<(*vertices)[VtxIt].z()<<endl;
                            TVector3 Dv3D_reco(QuadrupletVtx.x() - (*vertices)[VtxIt].x(), QuadrupletVtx.y() - (*vertices)[VtxIt].y(), QuadrupletVtx.z() - (*vertices)[VtxIt].z());
                            double Cosdphi_3D = Dv3D_reco.Dot(FourCandidate.Vect())/(Dv3D_reco.Mag()*FourCandidate.Vect().Mag());
                            //cout<<"     cosDPhi3D="<<Cosdphi_3D<<endl;
                            if(Cosdphi_3D>dphi_pv){
                                dphi_pv = Cosdphi_3D;
                                primaryvertex_index=VtxIt;
                                selVtxId=k;
                            }
                        }
                    }
                }
                //cout<<"Max Cosdphi_3D= "<<dphi_pv<<" selVtxId="<<selVtxId<<" primaryvertex_index="<<primaryvertex_index<<endl;
                //cout<<"Closest PV index before refit: "<<primaryvertex_index<<" x="<<(*vertices)[primaryvertex_index].x()<<" y="<<(*vertices)[primaryvertex_index].y()<<" z="<<(*vertices)[primaryvertex_index].z()<<endl;
                
                std::vector<reco::TransientTrack> pvTracks_original;
                TransientTrackMap pvTrackMap_refit;
                
                // PV before removing SV tracks
                TransientVertex PVertex_bis;
                bool PVertex_bis_fit;
                if(transTracksAssoToVtx.at(selVtxId).size()>1){
                    PVertex_bis_fit = true;
                    KalmanVertexFitter PV_fitter_bis (true);
                    PVertex_bis = PV_fitter_bis.vertex(transTracksAssoToVtx.at(selVtxId));
                }
                else PVertex_bis_fit = false;
                
                vector<reco::TransientTrack> transTracksAssoToVtx_copy;
                for(std::vector<reco::TransientTrack>::const_iterator transTrack_it = transTracksAssoToVtx.at(selVtxId).begin(); transTrack_it != transTracksAssoToVtx.at(selVtxId).end(); ++transTrack_it){
                    transTracksAssoToVtx_copy.push_back(*transTrack_it);
                }
                
                //cout << "--> Starting PV refit" << endl;
                removeTracks3(transTracksAssoToVtx_copy, SVTrackRef);
                
                if(transTracksAssoToVtx_copy.size()>1){
                    RefittedPV_NTracks.push_back(transTracksAssoToVtx_copy.size());
                    //cout << "transTracksAssoToVtx_copy.size() after: " << transTracksAssoToVtx_copy.size() << endl;
                    
                    KalmanVertexFitter PV_fitter (true);
                    TransientVertex PVertex = PV_fitter.vertex(transTracksAssoToVtx_copy);
                    RefittedPV_isValid.push_back(PVertex.isValid());
                    RefittedPV_Chi2.push_back(PVertex.totalChiSquared());
                    RefittedPV_nDOF.push_back(PVertex.degreesOfFreedom());
                    
                    if(PVertex_bis_fit && PVertex_bis.isValid()){
                        PV_bis_Chi2.push_back(PVertex_bis.totalChiSquared());
                        PV_bis_nDOF.push_back(PVertex_bis.degreesOfFreedom());
                    }else{
                        //cout << "No Valid PV_bis" << endl;
                        PV_bis_Chi2.push_back(-99);
                        PV_bis_nDOF.push_back(-99);
                    }
                    
                    //cout<<"Valid Vtx1="<<PVertex.isValid()<<endl;
                    if(PVertex.isValid() && B_It->vertexChi2() >0 ){
                        NQuad.push_back(1);
                        QuadrupletIndex=trIn;
                        if((mu1->isGlobalMuon()) && (mu1->isPFMuon())) {
                            Mu1C.push_back(1);
                            if((mu2->isGlobalMuon()) && (mu2->isPFMuon())){
                                Mu2C.push_back(1);
                                if( (B_It->mass()>4.9) &&  (B_It->mass()<5.7) ){
                                    BMass.push_back(1);
                                }
                            }
                        }
                        mu1_pfreliso03.push_back(PFreliso03(*mu1));
                        mu2_pfreliso03.push_back(PFreliso03(*mu2));
                        mu3_pfreliso03.push_back(0);
                        mu4_pfreliso03.push_back(0);

                        Mu1_Pt.push_back(mu1->pt());
                        Mu1_Eta.push_back(mu1->eta());
                        Mu1_Phi.push_back(mu1->phi());
                        Mu1_QuadrupletIndex.push_back(QuadrupletIndex);
                        
                        Mu2_Pt.push_back(mu2->pt());
                        Mu2_Eta.push_back(mu2->eta());
                        Mu2_Phi.push_back(mu2->phi());
                        Mu2_QuadrupletIndex.push_back(QuadrupletIndex);
                        
                        Mu3_Pt.push_back(Track3->pt());
                        Mu3_Eta.push_back(Track3->eta());
                        Mu3_Phi.push_back(Track3->phi());
                        Mu3_Charge.push_back(Track3->charge());
                        Mu3_QuadrupletIndex.push_back(QuadrupletIndex);
                        
                        Mu4_Pt.push_back(Track4->pt());
                        Mu4_Eta.push_back(Track4->eta());
                        Mu4_Phi.push_back(Track4->phi());
                        Mu4_Charge.push_back(Track4->charge());
                        Mu4_QuadrupletIndex.push_back(QuadrupletIndex);
                        
                        Mu1_IsGlobal.push_back(mu1->isGlobalMuon());
                        Mu2_IsGlobal.push_back(mu2->isGlobalMuon());
                        Mu3_IsGlobal.push_back(0);
                        Mu4_IsGlobal.push_back(0);
                        
                        Mu1_IsPF.push_back(mu1->isPFMuon());
                        Mu2_IsPF.push_back(mu2->isPFMuon());
                        Mu3_IsPF.push_back(0);
                        Mu4_IsPF.push_back(0);
                        //cout<<"Reco mu1 pt="<<mu1->pt()<<" mu2 pt="<<mu2->pt()<<" mu3 pt="<<mu3->pt()<<" mu4 pt="<<mu4->pt()<<endl;
                        
                        //Refitted vars related to SV
                        std::vector<reco::TransientTrack> Ttracks;
                        Ttracks.push_back(transientTrack1);
                        Ttracks.push_back(transientTrack2);
                        Ttracks.push_back(transientTrack3);
                        Ttracks.push_back(transientTrack4);

                        /* Remove KalmanVertexFitter, include Kinematic fitter
                        KalmanVertexFitter SVfitter (true);
                        TransientVertex SVertex_ref = SVfitter.vertex(Ttracks);
                        vector < TransientTrack > ttrks = SVertex_ref.refittedTracks();
                        //cout<<"ttrks.size() :"<<ttrks.size()<<endl;
                        */
                        float chi = 0.;
                        float ndf = 0.;
                        KinematicParticleFactoryFromTransientTrack pFactory;
                        ParticleMass JPsi_mass = 3.096916;
                        ParticleMass muon_mass = 0.1056583;
                        ParticleMass kaon_mass = 0.493677;
                        ParticleMass pion_mass = 0.139570;
                        float muon_sigma = muon_mass*1.e-6;
                        float kaon_sigma = kaon_mass*1.e-6;
                        float pion_sigma = pion_mass*1.e-6;
                        vector<RefCountedKinematicParticle> ParticlesList;
                        ParticlesList.push_back(pFactory.particle(transientTrack1,muon_mass,chi,ndf,muon_sigma));
                        ParticlesList.push_back(pFactory.particle(transientTrack2,muon_mass,chi,ndf,muon_sigma));
                        ParticlesList.push_back(pFactory.particle(transientTrack3,kaon_mass,chi,ndf,kaon_sigma));
                        if(is2K==true) ParticlesList.push_back(pFactory.particle(transientTrack4,kaon_mass,chi,ndf,kaon_sigma));
                        else ParticlesList.push_back(pFactory.particle(transientTrack4,pion_mass,chi,ndf,pion_sigma));
                        //MultiTrackKinematicConstraint *  j_psi_c = new  TwoTrackMassKinematicConstraint(JPsi_mass);
                        KinematicConstrainedVertexFitter kcvFitter;
                        //RefCountedKinematicTree SVertex_ref = kcvFitter.fit(ParticlesList, j_psi_c);
                        RefCountedKinematicTree SVertex_ref = kcvFitter.fit(ParticlesList);

                        if(SVertex_ref->isValid()){
                            SVertex_ref->movePointerToTheTop();
                            RefCountedKinematicParticle bCandMC = SVertex_ref->currentParticle();
                            RefCountedKinematicVertex bDecayVertexMC = SVertex_ref->currentDecayVertex();
                            if(bDecayVertexMC->vertexIsValid()){
                                RefittedSV_Mass.push_back(bCandMC->currentState().mass());
                                RefittedSV_Mass_err.push_back(bCandMC->currentState().kinematicParametersError().matrix().At(6,6));
                                RefittedSV_Chi2.push_back(bDecayVertexMC->chiSquared());
                                RefittedSV_nDOF.push_back((int)bDecayVertexMC->degreesOfFreedom());
                             
                                vtx_ref_prob.push_back(TMath::Prob(bDecayVertexMC->chiSquared(),(int)bDecayVertexMC->degreesOfFreedom()));
    
                                SVertex_ref->movePointerToTheFirstChild();
                                RefCountedKinematicParticle mu1CandMC = SVertex_ref->currentParticle();
    
                                SVertex_ref->movePointerToTheNextChild();
                                RefCountedKinematicParticle mu2CandMC = SVertex_ref->currentParticle();
    
                                SVertex_ref->movePointerToTheNextChild();
                                RefCountedKinematicParticle T1CandMC = SVertex_ref->currentParticle();
    
                                SVertex_ref->movePointerToTheNextChild();
                                RefCountedKinematicParticle T2CandMC = SVertex_ref->currentParticle();
    
                                KinematicParameters psiMu1KP = mu1CandMC->currentState().kinematicParameters();
                                KinematicParameters psiMu2KP = mu2CandMC->currentState().kinematicParameters();		   
    
                                KinematicParameters trk1KP = T1CandMC->currentState().kinematicParameters();
                                KinematicParameters trk2KP = T2CandMC->currentState().kinematicParameters();
                             
    
                                TLorentzVector LV1, LV2, LV3, LV4;
                                LV1.SetPxPyPzE(psiMu1KP.momentum().x(), psiMu1KP.momentum().y(), psiMu1KP.momentum().z(), psiMu1KP.energy());
                                LV2.SetPxPyPzE(psiMu2KP.momentum().x(), psiMu2KP.momentum().y(), psiMu2KP.momentum().z(), psiMu2KP.energy());
                                LV3.SetPxPyPzE(trk1KP.momentum().x(), trk1KP.momentum().y(), trk1KP.momentum().z(), trk1KP.energy());
                                LV4.SetPxPyPzE(trk2KP.momentum().x(), trk2KP.momentum().y(), trk2KP.momentum().z(), trk2KP.energy());
                                
                                
                                RefTrack1_Pt.push_back(LV1.Pt()); RefTrack1_Eta.push_back(LV1.Eta()); RefTrack1_Phi.push_back(LV1.Phi()); RefTrack1_QuadrupletIndex.push_back(QuadrupletIndex);
                                RefTrack2_Pt.push_back(LV2.Pt()); RefTrack2_Eta.push_back(LV2.Eta()); RefTrack2_Phi.push_back(LV2.Phi()); RefTrack2_QuadrupletIndex.push_back(QuadrupletIndex);
                                RefTrack3_Pt.push_back(LV3.Pt()); RefTrack3_Eta.push_back(LV3.Eta()); RefTrack3_Phi.push_back(LV3.Phi()); RefTrack3_QuadrupletIndex.push_back(QuadrupletIndex);
                                RefTrack4_Pt.push_back(LV4.Pt()); RefTrack4_Eta.push_back(LV4.Eta()); RefTrack4_Phi.push_back(LV4.Phi()); RefTrack4_QuadrupletIndex.push_back(QuadrupletIndex);
                            } else {
                                RefTrack1_Pt.push_back(-99); RefTrack1_Eta.push_back(-99); RefTrack1_Phi.push_back(-99); RefTrack1_QuadrupletIndex.push_back(QuadrupletIndex);
                                RefTrack2_Pt.push_back(-99); RefTrack2_Eta.push_back(-99); RefTrack2_Phi.push_back(-99); RefTrack2_QuadrupletIndex.push_back(QuadrupletIndex);
                                RefTrack3_Pt.push_back(-99); RefTrack3_Eta.push_back(-99); RefTrack3_Phi.push_back(-99); RefTrack3_QuadrupletIndex.push_back(QuadrupletIndex);
                                RefTrack4_Pt.push_back(-99); RefTrack4_Eta.push_back(-99); RefTrack4_Phi.push_back(-99); RefTrack4_QuadrupletIndex.push_back(QuadrupletIndex);
                                
                                RefittedSV_Chi2.push_back(-99);
                                RefittedSV_nDOF.push_back(-99);
                                vtx_ref_prob.push_back(-99);
                                RefittedSV_Mass.push_back(-99);
                                RefittedSV_Mass_err.push_back(-99);
                            }
                        } else {
                            RefTrack1_Pt.push_back(-99); RefTrack1_Eta.push_back(-99); RefTrack1_Phi.push_back(-99); RefTrack1_QuadrupletIndex.push_back(QuadrupletIndex);
                            RefTrack2_Pt.push_back(-99); RefTrack2_Eta.push_back(-99); RefTrack2_Phi.push_back(-99); RefTrack2_QuadrupletIndex.push_back(QuadrupletIndex);
                            RefTrack3_Pt.push_back(-99); RefTrack3_Eta.push_back(-99); RefTrack3_Phi.push_back(-99); RefTrack3_QuadrupletIndex.push_back(QuadrupletIndex);
                            RefTrack4_Pt.push_back(-99); RefTrack4_Eta.push_back(-99); RefTrack4_Phi.push_back(-99); RefTrack4_QuadrupletIndex.push_back(QuadrupletIndex);
                            
                            RefittedSV_Chi2.push_back(-99);
                            RefittedSV_nDOF.push_back(-99);
                            vtx_ref_prob.push_back(-99);
                            RefittedSV_Mass.push_back(-99);
                            RefittedSV_Mass_err.push_back(-99);
                        }
                        ///////////////Check Trigger Matching///////////////
                        float dR1 = 999., dR2 = 999., dR3 = 999., dR4 = 999.;
                        
                        dR1 = MiniAnaB2Mu2K::dRtriggerMatch(*mu1, TriggerObj_B4Mu);
                        dR2 = MiniAnaB2Mu2K::dRtriggerMatch(*mu2, TriggerObj_B4Mu);
                        dR3 = MiniAnaB2Mu2K::dRtriggerMatchTrk(*Track3, TriggerObj_B4Mu);
                        dR4 = MiniAnaB2Mu2K::dRtriggerMatchTrk(*Track4, TriggerObj_B4Mu);
                        //cout<<"Trigger Matching: dR1="<<dR1<<" dR2="<<dR2<<" dR3="<<dR3<<" dR4="<<dR4<<endl;
                        Mu1_dRtriggerMatch.push_back(dR1);
                        Mu2_dRtriggerMatch.push_back(dR2);
                        Mu3_dRtriggerMatch.push_back(dR3);
                        Mu4_dRtriggerMatch.push_back(dR4);
                        
                        ///////////////Check GEN matching and Fill SimInfo///////////////
                        GenMatchMu1_SimPt.push_back(-99);
                        GenMatchMu2_SimPt.push_back(-99);
                        GenMatchMu3_SimPt.push_back(-99);
                        GenMatchMu4_SimPt.push_back(-99);
                                
                        GenMatchMu1_SimEta.push_back(-99);
                        GenMatchMu2_SimEta.push_back(-99);
                        GenMatchMu3_SimEta.push_back(-99);
                        GenMatchMu4_SimEta.push_back(-99);
                                
                        GenMatchMu1_SimPhi.push_back(-99);
                        GenMatchMu2_SimPhi.push_back(-99);
                        GenMatchMu3_SimPhi.push_back(-99);
                        GenMatchMu4_SimPhi.push_back(-99);
                                
                        GenMatchMu1_Pt.push_back(-99);
                        GenMatchMu2_Pt.push_back(-99);
                        GenMatchMu3_Pt.push_back(-99);
                        GenMatchMu4_Pt.push_back(-99);
                                
                        GenMatchMu1_Eta.push_back(-99);
                        GenMatchMu2_Eta.push_back(-99);
                        GenMatchMu3_Eta.push_back(-99);
                        GenMatchMu4_Eta.push_back(-99);
                                
                        GenMatchMu1_Phi.push_back(-99);
                        GenMatchMu2_Phi.push_back(-99);
                        GenMatchMu3_Phi.push_back(-99);
                        GenMatchMu4_Phi.push_back(-99);

                        /*
                        if(isMc){
                            int isMatch_phi=0, isMatch_jpsi=0;
                            if( (mu1->simType() == reco::MatchedMuonFromLightFlavour) && (fabs(mu1->simMotherPdgId()) == 333) ){
                                isMatch_phi++;
                            }
                            if( (mu1->simType() == reco::MatchedMuonFromHeavyFlavour) && (fabs(mu1->simMotherPdgId()) == 443) ){
                                isMatch_jpsi++;
                            }
                            if( (mu2->simType() == reco::MatchedMuonFromLightFlavour) && (fabs(mu2->simMotherPdgId()) == 333) ){
                                isMatch_phi++;
                            }
                            if( (mu2->simType() == reco::MatchedMuonFromHeavyFlavour) && (fabs(mu2->simMotherPdgId()) == 443) ){
                                isMatch_jpsi++;
                            }
                            if( (mu3->simType() == reco::MatchedMuonFromLightFlavour) && (fabs(mu3->simMotherPdgId()) == 333) ){
                                isMatch_phi++;
                            }
                            if( (mu3->simType() == reco::MatchedMuonFromHeavyFlavour) && (fabs(mu3->simMotherPdgId()) == 443) ){
                                isMatch_jpsi++;
                            }
                            if( (mu4->simType() == reco::MatchedMuonFromLightFlavour) && (fabs(mu4->simMotherPdgId()) == 333) ){
                                isMatch_phi++;
                            }
                            if( (mu4->simType() == reco::MatchedMuonFromHeavyFlavour) && (fabs(mu4->simMotherPdgId()) == 443) ){
                                isMatch_jpsi++;
                            }
                            //cout<<"mu4->simHeaviestMotherFlavour(): "<<mu4->simHeaviestMotherFlavour()<<endl;
                            //cout<<"isMatch_jpsi: "<<isMatch_jpsi<<" -- isMatch_phi: "<<isMatch_phi<<endl;
                            // cout<<QuadrupletIndex<<"Quadruplet Mass:"<<B_It->mass()<<" pt="<<B_It->pt()<<" vtx.x="<<B_It->vx()<<" vtx x="<<QuadrupletVtx.x()<<" chi2="<<B_It->vertexChi2()<<" ndof="<<B_It->vertexNdof()<<endl;
                            // cout<<QuadrupletIndex<<"--Muon 1 pt="<<mu1->pt()<<" Muon2 pt="<<mu2->pt()<<" Mu3 pt="<<mu3->pt()<<" "<<endl;
                            if( isMatch_phi==2 && isMatch_jpsi==2) {
                                cout<<" Matched Quadruplets mass="<<B_It->mass()<<endl;
                                GenMatchMu1_SimPt.push_back(mu1->simPt());
                                GenMatchMu2_SimPt.push_back(mu2->simPt());
                                GenMatchMu3_SimPt.push_back(mu3->simPt());
                                GenMatchMu4_SimPt.push_back(mu4->simPt());
                                
                                GenMatchMu1_SimEta.push_back(mu1->simEta());
                                GenMatchMu2_SimEta.push_back(mu2->simEta());
                                GenMatchMu3_SimEta.push_back(mu3->simEta());
                                GenMatchMu4_SimEta.push_back(mu4->simEta());
                                
                                GenMatchMu1_SimPhi.push_back(mu1->simPhi());
                                GenMatchMu2_SimPhi.push_back(mu2->simPhi());
                                GenMatchMu3_SimPhi.push_back(mu3->simPhi());
                                GenMatchMu4_SimPhi.push_back(mu4->simPhi());
                                
                                GenMatchMu1_Pt.push_back(mu1->pt());
                                GenMatchMu2_Pt.push_back(mu2->pt());
                                GenMatchMu3_Pt.push_back(mu3->pt());
                                GenMatchMu4_Pt.push_back(mu4->pt());
                                
                                GenMatchMu1_Eta.push_back(mu1->eta());
                                GenMatchMu2_Eta.push_back(mu2->eta());
                                GenMatchMu3_Eta.push_back(mu3->eta());
                                GenMatchMu4_Eta.push_back(mu4->eta());
                                
                                GenMatchMu1_Phi.push_back(mu1->phi());
                                GenMatchMu2_Phi.push_back(mu2->phi());
                                GenMatchMu3_Phi.push_back(mu3->phi());
                                GenMatchMu4_Phi.push_back(mu4->phi());
                            }
                            else{
                                GenMatchMu1_SimPt.push_back(-99);
                                GenMatchMu2_SimPt.push_back(-99);
                                GenMatchMu3_SimPt.push_back(-99);
                                GenMatchMu4_SimPt.push_back(-99);
                                
                                GenMatchMu1_SimEta.push_back(-99);
                                GenMatchMu2_SimEta.push_back(-99);
                                GenMatchMu3_SimEta.push_back(-99);
                                GenMatchMu4_SimEta.push_back(-99);
                                
                                GenMatchMu1_SimPhi.push_back(-99);
                                GenMatchMu2_SimPhi.push_back(-99);
                                GenMatchMu3_SimPhi.push_back(-99);
                                GenMatchMu4_SimPhi.push_back(-99);
                                
                                GenMatchMu1_Pt.push_back(-99);
                                GenMatchMu2_Pt.push_back(-99);
                                GenMatchMu3_Pt.push_back(-99);
                                GenMatchMu4_Pt.push_back(-99);
                                
                                GenMatchMu1_Eta.push_back(-99);
                                GenMatchMu2_Eta.push_back(-99);
                                GenMatchMu3_Eta.push_back(-99);
                                GenMatchMu4_Eta.push_back(-99);
                                
                                GenMatchMu1_Phi.push_back(-99);
                                GenMatchMu2_Phi.push_back(-99);
                                GenMatchMu3_Phi.push_back(-99);
                                GenMatchMu4_Phi.push_back(-99);
                            }
                            //GenVtx vars to be added
                        } //if(isMC)
                        */
                        ///////////////Fill Quadruplet Vars///////////////
                        QuadrupletVtx_x.push_back(B_It->vx());
                        QuadrupletVtx_y.push_back(B_It->vy());
                        QuadrupletVtx_z.push_back(B_It->vz());

                        vtx_prob.push_back(1. - ROOT::Math::chisquared_cdf(B_It->vertexChi2(), (int)B_It->vertexNdof()));
                        QuadrupletVtx_Chi2.push_back(B_It->vertexChi2());
                        QuadrupletVtx_NDOF.push_back(B_It->vertexNdof());
                        
                        Quadruplet_Mass.push_back(B_It->mass());
                        Quadruplet_Pt.push_back(B_It->pt());
                        Quadruplet_Eta.push_back(B_It->eta());
                        Quadruplet_Phi.push_back(B_It->phi());
                        Quadruplet_Charge.push_back(B_It->charge());
                        
                        //////////////Dimu vertices////////////////////
                        // std::vector<reco::TransientTrack> SVTracks12_Vtx, SVTracks23_Vtx, SVTracks13_Vtx, SVTracks14_Vtx, SVTracks24_Vtx, SVTracks34_Vtx;
                        
                        vector<RefCountedKinematicParticle> SVTracks12_Vtx, SVTracks23_Vtx, SVTracks13_Vtx, SVTracks14_Vtx, SVTracks24_Vtx, SVTracks34_Vtx;
                        chi = 0; ndf = 0;
                        SVTracks12_Vtx.push_back(pFactory.particle(transientTrack1,muon_mass,chi,ndf,muon_sigma));
                        SVTracks12_Vtx.push_back(pFactory.particle(transientTrack2,muon_mass,chi,ndf,muon_sigma));
                        SVTracks23_Vtx.push_back(pFactory.particle(transientTrack2,muon_mass,chi,ndf,muon_sigma));
                        SVTracks23_Vtx.push_back(pFactory.particle(transientTrack3,kaon_mass,chi,ndf,kaon_sigma));
                        SVTracks13_Vtx.push_back(pFactory.particle(transientTrack1,muon_mass,chi,ndf,muon_sigma));
                        SVTracks13_Vtx.push_back(pFactory.particle(transientTrack3,kaon_mass,chi,ndf,kaon_sigma));
                        
                        SVTracks14_Vtx.push_back(pFactory.particle(transientTrack1,muon_mass,chi,ndf,muon_sigma));
                        if(is2K==true) SVTracks14_Vtx.push_back(pFactory.particle(transientTrack4,kaon_mass,chi,ndf,kaon_sigma));
                        else SVTracks14_Vtx.push_back(pFactory.particle(transientTrack4,pion_mass,chi,ndf,pion_sigma));
                        
                        SVTracks24_Vtx.push_back(pFactory.particle(transientTrack2,muon_mass,chi,ndf,muon_sigma));
                        if(is2K==true) SVTracks24_Vtx.push_back(pFactory.particle(transientTrack4,kaon_mass,chi,ndf,kaon_sigma));
                        else SVTracks24_Vtx.push_back(pFactory.particle(transientTrack4,pion_mass,chi,ndf,pion_sigma));

                        SVTracks34_Vtx.push_back(pFactory.particle(transientTrack3,kaon_mass,chi,ndf,kaon_sigma));
                        if(is2K==true) SVTracks34_Vtx.push_back(pFactory.particle(transientTrack4,kaon_mass,chi,ndf,kaon_sigma));
                        else SVTracks34_Vtx.push_back(pFactory.particle(transientTrack4,pion_mass,chi,ndf,pion_sigma));

                        ////DiMu12////
                        KinematicConstrainedVertexFitter DiMu12_fitter;
                        RefCountedKinematicTree DiMu12Vtx = DiMu12_fitter.fit(SVTracks12_Vtx);
                        if(DiMu12Vtx->isValid()){
                            DiMu12Vtx->movePointerToTheTop();
                            Vtx12_mass.push_back(DiMu12Vtx->currentParticle()->currentState().mass());
                            Vtx12_mass_err.push_back(DiMu12Vtx->currentParticle()->currentState().kinematicParametersError().matrix().At(6,6));

                            Vtx12_Chi2.push_back(DiMu12Vtx->currentDecayVertex()->chiSquared());
                            Vtx12_nDOF.push_back(DiMu12Vtx->currentDecayVertex()->degreesOfFreedom());
                         
                            Vtx12_x.push_back(DiMu12Vtx->currentDecayVertex()->position().x());
                            Vtx12_y.push_back(DiMu12Vtx->currentDecayVertex()->position().y());
                            Vtx12_z.push_back(DiMu12Vtx->currentDecayVertex()->position().z());
                        }else{
                            Vtx12_mass.push_back(-99);
                            Vtx12_mass_err.push_back(-99);
                            Vtx12_Chi2.push_back(-99);
                            Vtx12_nDOF.push_back(-99);
                            Vtx12_x.push_back(-99);
                            Vtx12_y.push_back(-99);
                            Vtx12_z.push_back(-99);
                        }
                        ////DiMu23///
                        KinematicConstrainedVertexFitter DiMu23_fitter;
                        RefCountedKinematicTree DiMu23Vtx = DiMu23_fitter.fit(SVTracks23_Vtx);
                        if(DiMu23Vtx->isValid()){
                            DiMu23Vtx->movePointerToTheTop();
                            Vtx23_mass.push_back(DiMu23Vtx->currentParticle()->currentState().mass());
                            Vtx23_mass_err.push_back(DiMu23Vtx->currentParticle()->currentState().kinematicParametersError().matrix().At(6,6));
                            
                            Vtx23_Chi2.push_back(DiMu23Vtx->currentDecayVertex()->chiSquared());
                            Vtx23_nDOF.push_back((int)DiMu23Vtx->currentDecayVertex()->degreesOfFreedom());
                         
                            Vtx23_x.push_back(DiMu23Vtx->currentDecayVertex()->position().x());
                            Vtx23_y.push_back(DiMu23Vtx->currentDecayVertex()->position().y());
                            Vtx23_z.push_back(DiMu23Vtx->currentDecayVertex()->position().z());
                        }else{
                            Vtx23_mass.push_back(-99);
                            Vtx23_mass_err.push_back(-99);
                            Vtx23_Chi2.push_back(-99);
                            Vtx23_nDOF.push_back(-99);
                            Vtx23_x.push_back(-99);
                            Vtx23_y.push_back(-99);
                            Vtx23_z.push_back(-99);
                        }
                        ////DiMu13///
                        KinematicConstrainedVertexFitter DiMu13_fitter;
                        RefCountedKinematicTree DiMu13Vtx = DiMu13_fitter.fit(SVTracks13_Vtx);
                        if(DiMu13Vtx->isValid()){
                            DiMu13Vtx->movePointerToTheTop();
                            Vtx13_mass.push_back(DiMu13Vtx->currentParticle()->currentState().mass());
                            Vtx13_mass_err.push_back(DiMu13Vtx->currentParticle()->currentState().kinematicParametersError().matrix().At(6,6));

                            Vtx13_Chi2.push_back(DiMu13Vtx->currentDecayVertex()->chiSquared());
                            Vtx13_nDOF.push_back((int)DiMu13Vtx->currentDecayVertex()->degreesOfFreedom());

                            Vtx13_x.push_back(DiMu13Vtx->currentDecayVertex()->position().x());
                            Vtx13_y.push_back(DiMu13Vtx->currentDecayVertex()->position().y());
                            Vtx13_z.push_back(DiMu13Vtx->currentDecayVertex()->position().z());
                        }else{
                            Vtx13_mass.push_back(-99);
                            Vtx13_mass_err.push_back(-99);
                            Vtx13_Chi2.push_back(-99);
                            Vtx13_nDOF.push_back(-99);
                            Vtx13_x.push_back(-99);
                            Vtx13_y.push_back(-99);
                            Vtx13_z.push_back(-99);
                        }
                        ////DiMu14///
                        KinematicConstrainedVertexFitter DiMu14_fitter;
                        RefCountedKinematicTree DiMu14Vtx = DiMu14_fitter.fit(SVTracks14_Vtx);
                        if (DiMu14Vtx->isValid()) {
                            DiMu14Vtx->movePointerToTheTop();
                            Vtx14_mass.push_back(DiMu14Vtx->currentParticle()->currentState().mass());
                            Vtx14_mass_err.push_back(DiMu14Vtx->currentParticle()->currentState().kinematicParametersError().matrix().At(6,6));
                         
                            Vtx14_Chi2.push_back(DiMu14Vtx->currentDecayVertex()->chiSquared());
                            Vtx14_nDOF.push_back((int)DiMu14Vtx->currentDecayVertex()->degreesOfFreedom());

                            Vtx14_x.push_back(DiMu14Vtx->currentDecayVertex()->position().x());
                            Vtx14_y.push_back(DiMu14Vtx->currentDecayVertex()->position().y());
                            Vtx14_z.push_back(DiMu14Vtx->currentDecayVertex()->position().z());
                        } else {
                            Vtx14_mass.push_back(-99);
                            Vtx14_mass_err.push_back(-99);
                            Vtx14_Chi2.push_back(-99);
                            Vtx14_nDOF.push_back(-99);
                            Vtx14_x.push_back(-99);
                            Vtx14_y.push_back(-99);
                            Vtx14_z.push_back(-99);
                        }
                        
                        ////DiMu24///
                        KinematicConstrainedVertexFitter DiMu24_fitter;
                        RefCountedKinematicTree DiMu24Vtx = DiMu24_fitter.fit(SVTracks24_Vtx);
                        if (DiMu24Vtx->isValid()) {
                            DiMu24Vtx->movePointerToTheTop();
                            Vtx24_mass.push_back(DiMu24Vtx->currentParticle()->currentState().mass());
                            Vtx24_mass_err.push_back(DiMu24Vtx->currentParticle()->currentState().kinematicParametersError().matrix().At(6,6));

                            Vtx24_Chi2.push_back(DiMu24Vtx->currentDecayVertex()->chiSquared());
                            Vtx24_nDOF.push_back((int)DiMu24Vtx->currentDecayVertex()->degreesOfFreedom());
                         
                            Vtx24_x.push_back(DiMu24Vtx->currentDecayVertex()->position().x());
                            Vtx24_y.push_back(DiMu24Vtx->currentDecayVertex()->position().y());
                            Vtx24_z.push_back(DiMu24Vtx->currentDecayVertex()->position().z());
                        } else {
                            Vtx24_mass.push_back(-99);
                            Vtx24_mass_err.push_back(-99);
                            Vtx24_Chi2.push_back(-99);
                            Vtx24_nDOF.push_back(-99);
                            Vtx24_x.push_back(-99);
                            Vtx24_y.push_back(-99);
                            Vtx24_z.push_back(-99);
                        }
                        
                        ////DiMu34///
                        KinematicConstrainedVertexFitter DiMu34_fitter;
                        RefCountedKinematicTree DiMu34Vtx = DiMu34_fitter.fit(SVTracks34_Vtx);
                        if (DiMu34Vtx->isValid()) {
                            DiMu34Vtx->movePointerToTheTop();
                            Vtx34_mass.push_back(DiMu34Vtx->currentParticle()->currentState().mass());
                            Vtx34_mass_err.push_back(DiMu34Vtx->currentParticle()->currentState().kinematicParametersError().matrix().At(6,6));
                         
                            Vtx34_Chi2.push_back(DiMu34Vtx->currentDecayVertex()->chiSquared());
                            Vtx34_nDOF.push_back((int)DiMu34Vtx->currentDecayVertex()->degreesOfFreedom());
                         
                            Vtx34_x.push_back(DiMu34Vtx->currentDecayVertex()->position().x());
                            Vtx34_y.push_back(DiMu34Vtx->currentDecayVertex()->position().y());
                            Vtx34_z.push_back(DiMu34Vtx->currentDecayVertex()->position().z());
                        } else {
                            Vtx34_mass.push_back(-99);
                            Vtx34_mass_err.push_back(-99);
                            Vtx34_Chi2.push_back(-99);
                            Vtx34_nDOF.push_back(-99);
                            Vtx34_x.push_back(-99);
                            Vtx34_y.push_back(-99);
                            Vtx34_z.push_back(-99);
                        }
                        
                        /////////////////Defining ISO VAR related to the Quadruplet//////////////////////
                        TLorentzVector LV1=TLorentzVector( mu1->px(), mu1->py(), mu1->pz(), mu1->energy() );
                        TLorentzVector LV2=TLorentzVector( mu2->px(), mu2->py(), mu2->pz(), mu2->energy() );
                        TLorentzVector LV3=TLorentzVector( c3->px(), c3->py(), c3->pz(), c3->energy() );
                        TLorentzVector LV4=TLorentzVector( c4->px(), c4->py(), c4->pz(), c4->energy() );
                        TLorentzVector LV_B;
                        LV_B = LV1 + LV2 + LV3 + LV4;
                        //cout<<QuadrupletIndex<<" B_CandMass "<<B_It->mass()<<" B_Pt="<<B_It->pt()<<endl;
                        //cout<<QuadrupletIndex<<" B_VectMass "<<LV_B.M()<<" B_Pt="<<LV_B.Pt()<<endl;
                        
                        int nTracks03_mu1=0, nTracks03_mu2=0, nTracks03_mu3=0, nTracks03_mu4=0;
                        double mindist=9999;
                        double sumPtTrack1=0, sumPtTrack2=0, sumPtTrack3=0, sumPtTrack4=0, maxSumPtTracks=0;
                        
                        math::XYZPoint SVertexPoint = math::XYZPoint(QuadrupletVtx.x(), QuadrupletVtx.y(), QuadrupletVtx.z());
                        uint kk=0;
                        
                        //////////////Loop on PF Candidates////////////////////
                        for (std::vector<pat::PackedCandidate>::const_iterator cand = PFCands->begin(); cand != PFCands->end(), kk!= PFCands->size(); ++cand, ++kk) {
                            if(  (cand->pt()>1) && (fabs(cand->eta())<2.5) && (cand->trackerLayersWithMeasurement()>5) && (cand->pixelLayersWithMeasurement()>1) ){
                                
                                double dR1 = sqrt( reco::deltaR2(Track1.eta(), Track1.phi(), cand->eta(), cand->phi()) );
                                double dR2 = sqrt( reco::deltaR2(Track2.eta(), Track2.phi(), cand->eta(), cand->phi()) );
                                double dR3 = sqrt( reco::deltaR2(Track_3.eta(), Track_3.phi(), cand->eta(), cand->phi()) );
                                double dR4 = sqrt( reco::deltaR2(Track_4.eta(), Track_4.phi(), cand->eta(), cand->phi()) );
                                //cout<<"Skip muon track"<<endl;
                                if (dR1 < 0.01 || dR2 < 0.01 || dR3 < 0.01 || dR4 < 0.01) continue;
                                
                                double dz = abs(cand->dz(SVertexPoint));
                                double dxy = abs(cand->dxy(SVertexPoint));
                                double dca_fv = sqrt(dz*dz+dxy*dxy);
                                if(dca_fv<mindist && dca_fv>0) {
                                    mindist = dca_fv;
                                }
                                //for eack track having pt>1, excluded the muon tracks,
                                //for each muon in the Quadruplet, if deltaR<0.3 and the DCA is smaller than 1 mm
                                //the pt of the track is added -> I will take the largest total pt from the Four muons
                                if (dca_fv < 0.1) {
                                    if (dR1<0.3) {
                                        sumPtTrack1+=cand->pt();
                                        nTracks03_mu1++;
                                    }
                                    if (dR2<0.3) {
                                        sumPtTrack2+=cand->pt();
                                        nTracks03_mu2++;
                                    }
                                    if (dR3<0.3) {
                                        sumPtTrack3+=cand->pt();
                                        nTracks03_mu3++;
                                    }
                                    if (dR4<0.3) {
                                        sumPtTrack4+=cand->pt();
                                        nTracks03_mu4++;
                                    }
                                }
                            }
                        }//loop on tracks
                        
                        Quadruplet_mindca_iso.push_back(mindist);
                        maxSumPtTracks = std::max(sumPtTrack1, std::max(sumPtTrack2, std::max(sumPtTrack3,sumPtTrack4)));
                        //cout<<QuadrupletIndex<<" BMass "<<B_It->mass()<<" SumPt Tracks in cone="<<maxSumPtTracks<<" TauPt="<<B_It->pt()<<endl;
                        double relativeiso = maxSumPtTracks/LV_B.Pt();
                        Quadruplet_relativeiso.push_back(relativeiso);
                        
                        Mu1_NTracks03iso.push_back(nTracks03_mu1);
                        Mu2_NTracks03iso.push_back(nTracks03_mu2);
                        Mu3_NTracks03iso.push_back(nTracks03_mu3);
                        Mu4_NTracks03iso.push_back(nTracks03_mu4);
                        
                        double sumPtTrackRel1=0, sumPtTrackRel2=0, sumPtTrackRel3=0, sumPtTrackRel4=0, maxSumPtRelTracks =0;
                        sumPtTrackRel1=sumPtTrack1/LV1.Pt();
                        sumPtTrackRel2=sumPtTrack2/LV2.Pt();
                        sumPtTrackRel3=sumPtTrack3/LV3.Pt();
                        sumPtTrackRel4=sumPtTrack4/LV3.Pt();
                        maxSumPtRelTracks = std::max(sumPtTrackRel1, std::max(sumPtTrackRel2,std::max(sumPtTrackRel3,sumPtTrackRel4)));
                        Quadruplet_relativeiso2.push_back(maxSumPtRelTracks);
                        Quadruplet_IsoMu1.push_back(sumPtTrack1);
                        Quadruplet_IsoMu2.push_back(sumPtTrack2);
                        Quadruplet_IsoMu3.push_back(sumPtTrack3);
                        Quadruplet_IsoMu4.push_back(sumPtTrack4);
                        
                        /////////////////Defining variables related to PV and SV positions and errors//////////////////////
                        GlobalPoint PVertexPos  (PVertex.position());
                        GlobalPoint SVertexPos  (QuadrupletVtx.x(), QuadrupletVtx.y(), QuadrupletVtx.z());
                        
                        
                        VertexState PVstate(PVertex.position(),PVertex.positionError());
                        //cout<<"PV position="<<PVertex.position()<<endl;
                        //cout<<"PV covariance matrix: \n"<<PVertex.positionError().matrix()<<endl;
                        //cout<<"SV position="<<QuadrupletVtx.position()<<endl;
                        //cout<<"SV covariance matrix: \n"<<QuadrupletVtx.error()<<endl;
                        
                        VertexDistance3D vertTool;
                        double distance = vertTool.distance(PVstate, QuadrupletVtx).value();
                        double dist_err = vertTool.distance(PVstate, QuadrupletVtx).error();
                        double dist_sign =vertTool.distance(PVstate, QuadrupletVtx).significance();
                        
                        VertexDistanceXY vdistXY;
                        Measurement1D distXY = vdistXY.distance(QuadrupletVtx, PVertex);
                        DistXY_PVSV.push_back(distXY.value());
                        DistXY_significance_PVSV.push_back(distXY.significance());
                        
                        PV_x.push_back( (*vertices)[primaryvertex_index].x());
                        PV_y.push_back( (*vertices)[primaryvertex_index].y());
                        PV_z.push_back( (*vertices)[primaryvertex_index].z());
                        PV_NTracks.push_back(pvTracks_original.size());
                        
                        std::vector<double> PV_cov;
                        for (int i = 0; i < 3; i++) {
                            for (int j = i; j < 3; j++) {
                                PV_cov.push_back(PVertex.positionError().matrix()[i][j]);
                            }
                        }
                        std::vector<double> SV_cov;
                        for (int i = 0; i < 3; i++) {
                            for (int j = i; j < 3; j++) {
                                SV_cov.push_back(QuadrupletVtx.error()[i][j]);
                            }
                        }
                        //cout<<"PV covariance matrix: \n"<<PV_cov<<endl;
                        RefittedPV_x.push_back(PVertexPos.x());
                        RefittedPV_y.push_back(PVertexPos.y());
                        RefittedPV_z.push_back(PVertexPos.z());
                        RefittedPV_cov.push_back(PV_cov);
                        //RefittedPV_NTracks.push_back(pvTracks_refit.size());
                        //RefittedPV_Chi2.push_back(PVertex.);
                        
                        QuadrupletVtx_cov.push_back(SV_cov);
                        
                        VertexState BSstate(beamSpot);
                        VertexDistanceXY vertTool2D;
                        double BSdistance2D = vertTool2D.distance(BSstate, QuadrupletVtx).value();
                        double BSdist_err2D = vertTool2D.distance(BSstate, QuadrupletVtx).error();
                        double BSdist_sign2D =vertTool2D.distance(BSstate, QuadrupletVtx).significance();
                        
                        FlightDistPVSV.push_back(distance);
                        FlightDistPVSV_Err.push_back(dist_err);
                        FlightDistPVSV_Significance.push_back(dist_sign);
                        //FlightDistPVSV_chi2.push_back(chi2);
                        
                        FlightDistBS_SV.push_back(BSdistance2D);
                        FlightDistBS_SV_Err.push_back(BSdist_err2D);
                        FlightDistBS_SV_Significance.push_back(BSdist_sign2D);
                        
                        //Beam spot coordinates
                        BS_x.push_back(x_bs);
                        BS_y.push_back(y_bs);
                        BS_z.push_back(z_bs);
                        
                        //Add dxy info
                        GlobalVector dir1(mu1->px(), mu1->py(),  mu1->pz()); //To compute sign of IP
                        GlobalVector dir2(mu2->px(), mu2->py(),  mu2->pz()); //To compute sign of IP
                        GlobalVector dir3(Track3->px(), Track3->py(),  Track3->pz()); //To compute sign of IP
                        GlobalVector dir4(Track4->px(), Track4->py(),  Track4->pz()); //To compute sign of IP
                        std::pair<bool,Measurement1D> signed_IP2D_mu1 = IPTools::signedTransverseImpactParameter(transientTrack1, dir1, PVertex);
                        std::pair<bool,Measurement1D> signed_IP2D_mu2 = IPTools::signedTransverseImpactParameter(transientTrack2, dir2, PVertex);
                        std::pair<bool,Measurement1D> signed_IP2D_mu3 = IPTools::signedTransverseImpactParameter(transientTrack3, dir3, PVertex);
                        std::pair<bool,Measurement1D> signed_IP2D_mu4 = IPTools::signedTransverseImpactParameter(transientTrack4, dir4, PVertex);
                        //cout<<" IP mu1="<<signed_IP2D_mu1.second.value()<<" IP mu2="<<signed_IP2D_mu2.second.value()<<" IP mu3="<<signed_IP2D_mu3.second.value()<<endl;
                        dxy_mu1.push_back(signed_IP2D_mu1.second.value());
                        dxy_mu2.push_back(signed_IP2D_mu2.second.value());
                        dxy_mu3.push_back(signed_IP2D_mu3.second.value());
                        dxy_mu4.push_back(signed_IP2D_mu4.second.value());
                        dxyErr_mu1.push_back(signed_IP2D_mu1.second.error());
                        dxyErr_mu2.push_back(signed_IP2D_mu2.second.error());
                        dxyErr_mu3.push_back(signed_IP2D_mu3.second.error());
                        dxyErr_mu4.push_back(signed_IP2D_mu4.second.error());

                        //Add dxy info wrt BS
                        reco::Vertex BS_vertex = reco::Vertex(beamSpot.position(), beamSpot.rotatedCovariance3D(), 0., 0., 0);
                        std::pair<bool,Measurement1D> signed_IP2D_mu1_BS = IPTools::signedTransverseImpactParameter(transientTrack1, dir1, BS_vertex);
                        std::pair<bool,Measurement1D> signed_IP2D_mu2_BS = IPTools::signedTransverseImpactParameter(transientTrack2, dir2, BS_vertex);
                        std::pair<bool,Measurement1D> signed_IP2D_mu3_BS = IPTools::signedTransverseImpactParameter(transientTrack3, dir3, BS_vertex);
                        std::pair<bool,Measurement1D> signed_IP2D_mu4_BS = IPTools::signedTransverseImpactParameter(transientTrack4, dir4, BS_vertex);
                        mu1_bs_dxy.push_back(signed_IP2D_mu1_BS.second.value());
                        mu2_bs_dxy.push_back(signed_IP2D_mu2_BS.second.value());
                        mu3_bs_dxy.push_back(signed_IP2D_mu3_BS.second.value());
                        mu4_bs_dxy.push_back(signed_IP2D_mu4_BS.second.value());
                        mu1_bs_dxy_err.push_back(signed_IP2D_mu1_BS.second.error());
                        mu2_bs_dxy_err.push_back(signed_IP2D_mu2_BS.second.error());
                        mu3_bs_dxy_err.push_back(signed_IP2D_mu3_BS.second.error());
                        mu4_bs_dxy_err.push_back(signed_IP2D_mu4_BS.second.error());
                        mu1_bs_dxy_sig.push_back(signed_IP2D_mu1_BS.second.value()/signed_IP2D_mu1_BS.second.error());
                        mu2_bs_dxy_sig.push_back(signed_IP2D_mu2_BS.second.value()/signed_IP2D_mu2_BS.second.error());
                        mu3_bs_dxy_sig.push_back(signed_IP2D_mu3_BS.second.value()/signed_IP2D_mu3_BS.second.error());
                        mu4_bs_dxy_sig.push_back(signed_IP2D_mu4_BS.second.value()/signed_IP2D_mu4_BS.second.error());   
                        
                        IsoTrackMu1_Pt.push_back(-99); IsoTrackMu1_Eta.push_back(-99); IsoTrackMu1_Phi.push_back(-99);
                        IsoTrackMu2_Pt.push_back(-99); IsoTrackMu2_Eta.push_back(-99); IsoTrackMu2_Phi.push_back(-99);
                        IsoTrackMu3_Pt.push_back(-99); IsoTrackMu3_Eta.push_back(-99); IsoTrackMu3_Phi.push_back(-99);
                        IsoTrackMu4_Pt.push_back(-99); IsoTrackMu4_Eta.push_back(-99); IsoTrackMu4_Phi.push_back(-99);
                        
                        
                    }else{ //!(PVertex.isValid() && B_It->vertexChi2() >0)
                        mu1_pfreliso03.push_back(-99);
                        mu2_pfreliso03.push_back(-99);
                        mu3_pfreliso03.push_back(-99);
                        mu4_pfreliso03.push_back(-99);
                        Mu1_Pt.push_back(-99);
                        Mu1_Eta.push_back(-99);
                        Mu1_Phi.push_back(-99);
                        Mu1_QuadrupletIndex.push_back(-99);
                        
                        Mu2_Pt.push_back(-99);
                        Mu2_Eta.push_back(-99);
                        Mu2_Phi.push_back(-99);
                        Mu2_QuadrupletIndex.push_back(-99);
                        
                        Mu3_Pt.push_back(-99);
                        Mu3_Eta.push_back(-99);
                        Mu3_Phi.push_back(-99);
                        Mu3_Charge.push_back(-99);
                        Mu3_QuadrupletIndex.push_back(-99);
                        
                        Mu4_Pt.push_back(-99);
                        Mu4_Eta.push_back(-99);
                        Mu4_Phi.push_back(-99);
                        Mu4_Charge.push_back(-99);
                        Mu4_QuadrupletIndex.push_back(-99);
                        
                        QuadrupletVtx_x.push_back(-99);
                        QuadrupletVtx_y.push_back(-99);
                        QuadrupletVtx_z.push_back(-99);
                        vtx_prob.push_back(-99);
                        QuadrupletVtx_Chi2.push_back(-99);
                        QuadrupletVtx_NDOF.push_back(-99);
                        
                        Quadruplet_Mass.push_back(-99);
                        Quadruplet_Pt.push_back(-99);
                        Quadruplet_Eta.push_back(-99);
                        Quadruplet_Phi.push_back(-99);
                        Quadruplet_Charge.push_back(-99);
                        
                        Quadruplet_mindca_iso.push_back(-99);
                        Quadruplet_relativeiso.push_back(-99);
                        
                        Mu1_NTracks03iso.push_back(-99);
                        Mu2_NTracks03iso.push_back(-99);
                        Mu3_NTracks03iso.push_back(-99);
                        Mu4_NTracks03iso.push_back(-99);
                        
                        PV_x.push_back(-99);
                        PV_y.push_back(-99);
                        PV_z.push_back(-99);
                        BS_x.push_back(-99);
                        BS_y.push_back(-99);
                        BS_z.push_back(-99);
                        PV_NTracks.push_back(-99);
                        RefittedPV_x.push_back(-99);
                        RefittedPV_y.push_back(-99);
                        RefittedPV_z.push_back(-99);
                        std::vector<double>cov; for(int i=0; i<6; i++) cov.push_back(-99);
                        RefittedPV_cov.push_back(cov);
                        QuadrupletVtx_cov.push_back(cov);
                        //RefittedPV_Chi2.push_back(PVertex.);
                                 
                        Vtx12_Chi2.push_back(-99);
                        Vtx12_nDOF.push_back(-99);
                        Vtx12_x.push_back(-99);
                        Vtx12_y.push_back(-99);
                        Vtx12_z.push_back(-99);
                        Vtx12_mass.push_back(-99);
                        Vtx12_mass_err.push_back(-99);
                                 
                        Vtx23_Chi2.push_back(-99);
                        Vtx23_nDOF.push_back(-99);
                        Vtx23_x.push_back(-99);
                        Vtx23_y.push_back(-99);
                        Vtx23_z.push_back(-99);
                        Vtx23_mass.push_back(-99);
                        Vtx23_mass_err.push_back(-99);
                                 
                        Vtx13_Chi2.push_back(-99);
                        Vtx13_nDOF.push_back(-99);
                        Vtx13_x.push_back(-99);
                        Vtx13_y.push_back(-99);
                        Vtx13_z.push_back(-99);
                        Vtx13_mass.push_back(-99);
                        Vtx13_mass_err.push_back(-99);

                        Vtx14_Chi2.push_back(-99);
                        Vtx14_nDOF.push_back(-99);
                        Vtx14_x.push_back(-99);
                        Vtx14_y.push_back(-99);
                        Vtx14_z.push_back(-99);
                        Vtx14_mass.push_back(-99);
                        Vtx14_mass_err.push_back(-99);

                        Vtx24_Chi2.push_back(-99);
                        Vtx24_nDOF.push_back(-99);
                        Vtx24_x.push_back(-99);
                        Vtx24_y.push_back(-99);
                        Vtx24_z.push_back(-99);
                        Vtx24_mass.push_back(-99);
                        Vtx24_mass_err.push_back(-99);

                        Vtx34_Chi2.push_back(-99);
                        Vtx34_nDOF.push_back(-99);
                        Vtx34_x.push_back(-99);
                        Vtx34_y.push_back(-99);
                        Vtx34_z.push_back(-99);
                        Vtx34_mass.push_back(-99);
                        Vtx34_mass_err.push_back(-99);

                        RefTrack1_Pt.push_back(-99); RefTrack1_Eta.push_back(-99); RefTrack1_Phi.push_back(-99); RefTrack1_QuadrupletIndex.push_back(-99);
                        RefTrack2_Pt.push_back(-99); RefTrack2_Eta.push_back(-99); RefTrack2_Phi.push_back(-99); RefTrack2_QuadrupletIndex.push_back(-99);
                        RefTrack3_Pt.push_back(-99); RefTrack3_Eta.push_back(-99); RefTrack3_Phi.push_back(-99); RefTrack3_QuadrupletIndex.push_back(-99);
                        RefTrack4_Pt.push_back(-99); RefTrack4_Eta.push_back(-99); RefTrack4_Phi.push_back(-99); RefTrack4_QuadrupletIndex.push_back(-99);
                        RefittedSV_Chi2.push_back(-99);
                        RefittedSV_nDOF.push_back(-99);
                        RefittedSV_Mass.push_back(-99);
                        RefittedSV_Mass_err.push_back(-99);
                        vtx_ref_prob.push_back(-99);

                        IsoTrackMu1_Pt.push_back(-99); IsoTrackMu1_Eta.push_back(-99); IsoTrackMu1_Phi.push_back(-99);
                        IsoTrackMu2_Pt.push_back(-99); IsoTrackMu2_Eta.push_back(-99); IsoTrackMu2_Phi.push_back(-99);
                        IsoTrackMu3_Pt.push_back(-99); IsoTrackMu3_Eta.push_back(-99); IsoTrackMu3_Phi.push_back(-99);
                        IsoTrackMu4_Pt.push_back(-99); IsoTrackMu4_Eta.push_back(-99); IsoTrackMu4_Phi.push_back(-99);
                        
                        Mu1_dRtriggerMatch.push_back(-99);
                        Mu2_dRtriggerMatch.push_back(-99);
                        Mu3_dRtriggerMatch.push_back(-99);
                        Mu4_dRtriggerMatch.push_back(-99);
                                                                                                
                        FlightDistPVSV.push_back(-99);
                        FlightDistPVSV_Err.push_back(-99);
                        FlightDistPVSV_Significance.push_back(-99);
                        FlightDistPVSV_chi2.push_back(-99);
                        dxy_mu1.push_back(-99);
                        dxy_mu2.push_back(-99);
                        dxy_mu3.push_back(-99);
                        dxy_mu4.push_back(-99);
                        
                        dxyErr_mu1.push_back(-99);
                        dxyErr_mu2.push_back(-99);
                        dxyErr_mu3.push_back(-99);
                        dxyErr_mu4.push_back(-99);

                        mu1_bs_dxy.push_back(-99);
                        mu2_bs_dxy.push_back(-99);
                        mu3_bs_dxy.push_back(-99);
                        mu4_bs_dxy.push_back(-99);
                        mu1_bs_dxy_err.push_back(-99);
                        mu2_bs_dxy_err.push_back(-99);
                        mu3_bs_dxy_err.push_back(-99);
                        mu4_bs_dxy_err.push_back(-99);
                        mu1_bs_dxy_sig.push_back(-99);
                        mu2_bs_dxy_sig.push_back(-99);
                        mu3_bs_dxy_sig.push_back(-99);
                        mu4_bs_dxy_sig.push_back(-99);

                        GenMatchMu1_SimPt.push_back(-99);
                        GenMatchMu2_SimPt.push_back(-99);
                        GenMatchMu3_SimPt.push_back(-99);
                        GenMatchMu4_SimPt.push_back(-99);
                        
                        GenMatchMu1_SimEta.push_back(-99);
                        GenMatchMu2_SimEta.push_back(-99);
                        GenMatchMu3_SimEta.push_back(-99);
                        GenMatchMu4_SimEta.push_back(-99);
                        
                        GenMatchMu1_SimPhi.push_back(-99);
                        GenMatchMu2_SimPhi.push_back(-99);
                        GenMatchMu3_SimPhi.push_back(-99);
                        GenMatchMu4_SimPhi.push_back(-99);
                        
                        GenMatchMu1_Pt.push_back(-99);
                        GenMatchMu2_Pt.push_back(-99);
                        GenMatchMu3_Pt.push_back(-99);
                        GenMatchMu4_Pt.push_back(-99);
                        
                        GenMatchMu1_Eta.push_back(-99);
                        GenMatchMu2_Eta.push_back(-99);
                        GenMatchMu3_Eta.push_back(-99);
                        GenMatchMu4_Eta.push_back(-99);
                        
                        GenMatchMu1_Phi.push_back(-99);
                        GenMatchMu2_Phi.push_back(-99);
                        GenMatchMu3_Phi.push_back(-99);
                        GenMatchMu4_Phi.push_back(-99);
                        
                        Quadruplet_relativeiso2.push_back(-99);
                        
                        DistXY_PVSV.push_back(-99);
                        DistXY_significance_PVSV.push_back(-99);
                        Quadruplet_IsoMu1.push_back(-99);
                        Quadruplet_IsoMu2.push_back(-99);
                        Quadruplet_IsoMu3.push_back(-99);
                        Quadruplet_IsoMu4.push_back(-99);
                        
                        FlightDistBS_SV.push_back(-99);
                        FlightDistBS_SV_Err.push_back(-99);
                        FlightDistBS_SV_Significance.push_back(-99);
                        
                        Mu1_IsGlobal.push_back(-99);
                        Mu2_IsGlobal.push_back(-99);
                        Mu3_IsGlobal.push_back(-99);
                        Mu4_IsGlobal.push_back(-99);
                        
                        Mu1_IsPF.push_back(-99);
                        Mu2_IsPF.push_back(-99);
                        Mu3_IsPF.push_back(-99);
                        Mu4_IsPF.push_back(-99);
                        
                    }//!(PVertex.isValid() && B_It->vertexChi2() >0)
                }//transTracksAssoToVtx_copy.size()>1
            }//(VtxIdV.size()>0 && vertices->size()>0)
        }//loop Cand2Mu2Tracks
    }//isAna
    
    NGoodQuadruplets.push_back(NQuad.size());
    if(NQuad.size()>0) hEventsAfterGoodCand->Fill(1);
    if(Mu1C.size()>0) hEventsAfterMu1ID->Fill(1);
    if(Mu2C.size()>0)  hEventsAfterMu2ID->Fill(1);
    if(Mu3C.size()>0)  hEventsAfterMu3ID->Fill(1);
    if(Mu4C.size()>0)  hEventsAfterMu4ID->Fill(1);
    if(BMass.size()>0)  hEventsAfterBMass->Fill(1);
    
    //////Fill info photons//////
    //PhotonCollectionSize = photons->size();
    //cout<<" PhotonCollectionSize="<<PhotonCollectionSize<<endl;
    //uint j=0;
    //for(edm::View<pat::Photon>::const_iterator pho=photons->begin(); pho!=photons->end(), j<photons->size(); ++pho, ++j){
        //Basic Kinematics
        //cout<<"   PhotonEnergy="<<pho->energy()<<endl;
        //PhotonPt.push_back(pho->pt());
        //PhotonEt.push_back(pho->et());
        //PhotonEnergy.push_back(pho->energy());
        //PhotonEta.push_back(pho->eta());
        //PhotonPhi.push_back(pho->phi());
    //}
    
    //////Fill recoMu//////
    std::vector<int> MuFilter;
    vector<pat::Muon>    MyMu, MyMu2, SyncMu;
    //double AllMuPt =0;
    MuonCollectionSize = muons->size();
    uint k=0;
    for(edm::View<pat::Muon>::const_iterator mu=muons->begin(); mu!=muons->end(), k<muons->size(); ++mu, ++k){
        
        MuFilter.push_back(1);
        MyMu.push_back(*mu);
        
        //Basic Kinematics
        MuonPt.push_back(mu->pt());
        MuonEta.push_back(mu->eta());
        MuonPhi.push_back(mu->phi());
        MuonEnergy.push_back(mu->energy());
        MuonCharge.push_back(mu->charge());
        
        Muon_simPdgId.push_back(mu->simPdgId());
        Muon_simMotherPdgId.push_back(mu->simMotherPdgId());
        Muon_simFlavour.push_back(mu->simFlavour());
        Muon_simHeaviestMotherFlavour.push_back(mu->simHeaviestMotherFlavour());
        Muon_simType.push_back(mu->simType());
        Muon_simBX.push_back(mu->simBX());
        
        //Vtx position
        Muon_vx.push_back(mu->vx());
        Muon_vy.push_back(mu->vy());
        Muon_vz.push_back(mu->vz());
        Muon_IP2D_BS.push_back(pat::Muon::BS2D);
        Muon_IP3D_BS.push_back(pat::Muon::BS3D);
        Muon_IP2D_PV.push_back(pat::Muon::PV2D);
        Muon_IP3D_PV.push_back(pat::Muon::PV3D);
        
        //MuonID
        Muon_isGlobal.push_back(mu->isGlobalMuon());
        Muon_isSoft.push_back(mu->isSoftMuon(PV));
        Muon_isMVA.push_back(mu->mvaIDValue());
        Muon_isMVASoft.push_back(mu->softMvaValue());
        Muon_isLoose.push_back(mu->isLooseMuon());
        Muon_isMedium.push_back(mu->isMediumMuon());
        Muon_isTight.push_back(mu->isTightMuon(PV));
        Muon_isPF.push_back(mu->isPFMuon());
        Muon_isRPCMuon.push_back(mu->isRPCMuon());
        Muon_isStandAloneMuon.push_back(mu->isStandAloneMuon());
        Muon_isTrackerMuon.push_back(mu->isTrackerMuon());
        Muon_isCaloMuon.push_back(mu->isCaloMuon());
        Muon_isQualityValid.push_back(mu->isQualityValid());
        Muon_isTimeValid.push_back(mu->isTimeValid());
        Muon_isIsolationValid.push_back(mu->isIsolationValid());
        Muon_numberOfMatchedStations.push_back(mu->numberOfMatchedStations());
        Muon_numberOfMatches.push_back(mu->numberOfMatches(reco::Muon::SegmentArbitration));
        Muon_SoftMVA_Val.push_back(mu->softMvaValue());
        
        Muon_timeAtIpInOut.push_back(mu->time().timeAtIpInOut);
        Muon_timeAtIpInOutErr.push_back(mu->time().timeAtIpInOutErr);
        
        std::vector<int> fvDThits{0, 0, 0, 0};
        std::vector<int> fvRPChits{0, 0, 0, 0};
        std::vector<int> fvCSChits{0, 0, 0, 0};
        
        float kVMuonHitComb = 0;
        if(mu->isGlobalMuon()) {
            reco::TrackRef gTrack = mu->globalTrack();
            const reco::HitPattern& gMpattern = gTrack->hitPattern();
            for(int i=0; i<gMpattern.numberOfAllHits(reco::HitPattern::TRACK_HITS); i++) {
                uint32_t hit = gMpattern.getHitPattern(reco::HitPattern::TRACK_HITS, i);
                if(!gMpattern.validHitFilter(hit)) continue;
                
                int muStation0 = gMpattern.getMuonStation(hit) - 1;
                if(muStation0 >= 0 && muStation0 < 4) {
                    if(gMpattern.muonDTHitFilter(hit)) fvDThits[muStation0]++;
                    if(gMpattern.muonRPCHitFilter(hit)) fvRPChits[muStation0]++;
                    if(gMpattern.muonCSCHitFilter(hit)) fvCSChits[muStation0]++;
                }
            }
            
            for (unsigned int station = 0; station < 4; ++station) {
                kVMuonHitComb += (fvDThits[station]) / 2.;
                kVMuonHitComb += fvRPChits[station];
                if(fvCSChits[station] > 6) kVMuonHitComb += 6;
                else kVMuonHitComb += fvCSChits[station];
            }
            Muon_validMuonHitComb.push_back(kVMuonHitComb);
        }else{
            Muon_validMuonHitComb.push_back(-99);
        }
        
        if (mu->isGlobalMuon()) {
            Muon_GLnormChi2.push_back(mu->globalTrack()->normalizedChi2());
            Muon_GLhitPattern_numberOfValidMuonHits.push_back(mu->globalTrack()->hitPattern().numberOfValidMuonHits());
        }else{
            Muon_GLnormChi2.push_back(-999);
            Muon_GLhitPattern_numberOfValidMuonHits.push_back(-999);
        }
        
        if (mu->innerTrack().isNonnull()){
            Muon_trackerLayersWithMeasurement.push_back(mu->innerTrack()->hitPattern().trackerLayersWithMeasurement());
            bool ishighq = mu->innerTrack()->quality(reco::Track::highPurity);
            Muon_innerTrack_highPurity.push_back(ishighq);
            Muon_Numberofvalidpixelhits.push_back(mu->innerTrack()->hitPattern().numberOfValidPixelHits());
            Muon_innerTrack_ValidFraction.push_back( mu->innerTrack()->validFraction() );
            Muon_Numberofvalidtrackerhits.push_back(mu->innerTrack()->hitPattern().numberOfValidTrackerHits());
            Muon_innerTrack_p.push_back(mu->innerTrack()->p());
            Muon_innerTrack_eta.push_back(mu->innerTrack()->eta());
            Muon_innerTrack_phi.push_back(mu->innerTrack()->phi());
            Muon_innerTrack_normalizedChi2.push_back(mu->innerTrack()->normalizedChi2());
        }else{
            Muon_innerTrack_ValidFraction.push_back( -99);
            Muon_innerTrack_highPurity.push_back( -99);
            Muon_trackerLayersWithMeasurement.push_back(-999);
            Muon_Numberofvalidpixelhits.push_back(-999);
            Muon_Numberofvalidtrackerhits.push_back(-999);
            Muon_trackerLayersWithMeasurement.push_back(-999);
            Muon_innerTrack_p.push_back(-999);
            Muon_innerTrack_eta.push_back(-999);
            Muon_innerTrack_phi.push_back(-999);
            Muon_innerTrack_normalizedChi2.push_back(-999);
        }
        if (mu->outerTrack().isNonnull()){
            Muon_outerTrack_p.push_back(mu->outerTrack()->p());
            Muon_outerTrack_eta.push_back(mu->outerTrack()->eta());
            Muon_outerTrack_phi.push_back(mu->outerTrack()->phi());
            Muon_outerTrack_normalizedChi2.push_back(mu->outerTrack()->normalizedChi2());
            Muon_outerTrack_muonStationsWithValidHits.push_back(mu->outerTrack()->hitPattern().muonStationsWithValidHits());
        }else{
            Muon_outerTrack_p.push_back(-999);
            Muon_outerTrack_eta.push_back(-999);
            Muon_outerTrack_phi.push_back(-999);
            Muon_outerTrack_normalizedChi2.push_back(-999);
            Muon_outerTrack_muonStationsWithValidHits.push_back(-999);
        }
        if (mu->innerTrack().isNonnull() && mu->outerTrack().isNonnull()){
            Muon_QInnerOuter.push_back(mu->outerTrack()->charge()*mu->innerTrack()->charge());
        }else{
            Muon_QInnerOuter.push_back(-999);
        }
        
        Muon_combinedQuality_updatedSta.push_back(mu->combinedQuality().updatedSta);
        Muon_combinedQuality_trkKink.push_back(mu->combinedQuality().trkKink);
        Muon_combinedQuality_glbKink.push_back(mu->combinedQuality().glbKink);
        Muon_combinedQuality_trkRelChi2.push_back(mu->combinedQuality().trkRelChi2);
        Muon_combinedQuality_staRelChi2.push_back(mu->combinedQuality().staRelChi2);
        Muon_combinedQuality_chi2LocalPosition.push_back(mu->combinedQuality().chi2LocalPosition);
        Muon_combinedQuality_chi2LocalMomentum.push_back(mu->combinedQuality().chi2LocalMomentum);
        Muon_combinedQuality_localDistance.push_back(mu->combinedQuality().localDistance);
        Muon_combinedQuality_globalDeltaEtaPhi.push_back(mu->combinedQuality().globalDeltaEtaPhi);
        Muon_combinedQuality_tightMatch.push_back(mu->combinedQuality().tightMatch);
        Muon_combinedQuality_glbTrackProbability.push_back(mu->combinedQuality().glbTrackProbability);
        
        Muon_calEnergy_em.push_back(mu->calEnergy().em);
        Muon_calEnergy_emS9.push_back(mu->calEnergy().emS9);
        Muon_calEnergy_emS25.push_back(mu->calEnergy().emS25);
        Muon_calEnergy_had.push_back(mu->calEnergy().had);
        Muon_calEnergy_hadS9.push_back(mu->calEnergy().hadS9);
        
        Muon_segmentCompatibility.push_back(muon::segmentCompatibility(*mu));
        Muon_caloCompatibility.push_back(muon::caloCompatibility(*mu));
        
        Muon_ptErrOverPt.push_back( (mu->muonBestTrack()->ptError()/mu->muonBestTrack()->pt()) );
        Muon_BestTrackPt.push_back( mu->muonBestTrack()->pt() );
        Muon_BestTrackPtErr.push_back( mu->muonBestTrack()->ptError() );
        
        Muon_BestTrackEta.push_back( mu->muonBestTrack()->eta() );
        Muon_BestTrackEtaErr.push_back( mu->muonBestTrack()->etaError() );
        
        Muon_BestTrackPhi.push_back( mu->muonBestTrack()->phi() );
        Muon_BestTrackPhiErr.push_back( mu->muonBestTrack()->phiError() );
        
        const reco::MuonIsolation Iso03 = mu->isolationR03();
        const reco::MuonIsolation Iso05 = mu->isolationR05();
        if (mu->isIsolationValid()) {
            Muon_emEt03.push_back(Iso03.emEt);
            Muon_hadEt03.push_back(Iso03.hadEt);
            Muon_nJets03.push_back(Iso03.nJets);
            Muon_nTracks03.push_back(Iso03.nTracks);
            Muon_sumPt03.push_back(Iso03.sumPt);
            Muon_emVetoEt03.push_back(Iso03.emVetoEt);
            Muon_hadVetoEt03.push_back(Iso03.hadVetoEt);
            Muon_trackerVetoPt03.push_back(Iso03.trackerVetoPt);
            
            Muon_emEt05.push_back(Iso05.emEt);
            Muon_hadEt05.push_back(Iso05.hadEt);
            Muon_nJets05.push_back(Iso05.nJets);
            Muon_nTracks05.push_back(Iso05.nTracks);
            Muon_sumPt05.push_back(Iso05.sumPt);
            Muon_hadVetoEt05.push_back(Iso05.hadVetoEt);
            Muon_trackerVetoPt05.push_back(Iso05.trackerVetoPt);
            Muon_emVetoEt05.push_back(Iso05.emVetoEt);
            
        } else { // if isolation is not valid use -1 as default
            Muon_emEt03.push_back(-1);
            Muon_hadEt03.push_back(-1);
            Muon_nJets03.push_back(-1);
            Muon_nTracks03.push_back(-1);
            Muon_sumPt03.push_back(-1);
            
            Muon_hadVetoEt03.push_back(-1);
            Muon_emVetoEt03.push_back(-1);
            Muon_trackerVetoPt03.push_back(-1);
            
            Muon_emEt05.push_back(-1);
            Muon_hadEt05.push_back(-1);
            Muon_nJets05.push_back(-1);
            Muon_nTracks05.push_back(-1);
            Muon_sumPt05.push_back(-1);
            
            Muon_emVetoEt05.push_back(-1);
            Muon_trackerVetoPt05.push_back(-1);
            Muon_hadVetoEt05.push_back(-1);
        }
    }
    
    if (!iEvent.isRealData())
    {
        Handle<vector<PileupSummaryInfo> >  PupInfo;
        iEvent.getByToken(puToken_, PupInfo);
        puN = PupInfo->begin()->getTrueNumInteractions();
    }
    
    ///////SyncTree
    
    evt  = iEvent.id().event();
    run  = iEvent.id().run();
    lumi = iEvent.luminosityBlock();
    
    tree_->Fill();
    
    run= -999;
    evt= -999;
    lumi= -999;
    puN= -999;
    
    GenParticle_PdgId.clear();
    GenParticle_Pt.clear();
    GenParticle_Eta.clear();
    GenParticle_Phi.clear();
    GenParticle_MotherPt.clear();
    GenParticle_MotherEta.clear();                             
    GenParticle_MotherPhi.clear();
                                 
    GenParticle_Pt_v2.clear();
    GenParticle_Eta_v2.clear();
    GenParticle_Phi_v2.clear();
    GenParticle_PdgId_v2.clear();
    GenParticle_Pt_trk.clear();
    GenParticle_Eta_trk.clear();
    GenParticle_Phi_trk.clear();
    GenParticle_PdgId_trk.clear();
    GenParticle_MotherPdgId.clear();
    GenParticle_GrandMotherPdgId.clear();
    GenParticle_vx.clear();
    GenParticle_vy.clear();
    GenParticle_vz.clear();
    PhotonCollectionSize=0;
    PhotonPt.clear();
    PhotonEt.clear();
    PhotonEnergy.clear();
    PhotonEta.clear();
    PhotonPhi.clear();
    
    MuonCollectionSize=0;
    MuonPt.clear();
    MuonEta.clear();
    MuonPhi.clear();
    
    Muon_simPdgId.clear();
    Muon_simMotherPdgId.clear();
    Muon_simFlavour.clear();
    Muon_simHeaviestMotherFlavour.clear();
    Muon_simType.clear();
    Muon_simBX.clear();
    MuonEnergy.clear();
    MuonCharge.clear();
    
    /////Vtx position
    Muon_vx.clear();
    Muon_vy.clear();
    Muon_vz.clear();
    
    /////MuonID
    Muon_isGlobal.clear();
    Muon_isSoft.clear();
    Muon_isMVA.clear();
    Muon_isMVASoft.clear();
    Muon_isLoose.clear();
    Muon_isMedium.clear();
    Muon_isTight.clear();
    Muon_isPF.clear();
    Muon_isRPCMuon.clear();
    Muon_isStandAloneMuon.clear();
    Muon_isTrackerMuon.clear();
    Muon_isCaloMuon.clear();
    Muon_isQualityValid.clear();
    Muon_isTimeValid.clear();
    Muon_isIsolationValid.clear();
    Muon_numberOfMatchedStations.clear();
    Muon_numberOfMatches.clear();
    Muon_SoftMVA_Val.clear();
    /////MuonTime
    Muon_timeAtIpInOut.clear();
    Muon_timeAtIpInOutErr.clear();
    
    /////Muon inner + outer track
    Muon_GLnormChi2.clear();
    Muon_GLhitPattern_numberOfValidMuonHits.clear();
    
    Muon_trackerLayersWithMeasurement.clear();
    Muon_Numberofvalidpixelhits.clear();
    
    Muon_innerTrack_highPurity.clear();
    Muon_innerTrack_ValidFraction.clear();
    Muon_Numberofvalidtrackerhits.clear();
    Muon_validMuonHitComb.clear();
    Muon_IP2D_BS.clear();
    Muon_IP3D_BS.clear();
    Muon_IP2D_PV.clear();
    Muon_IP3D_PV.clear();
    
    Muon_outerTrack_p.clear();
    Muon_outerTrack_eta.clear();
    Muon_outerTrack_phi.clear();
    Muon_outerTrack_normalizedChi2.clear();
    Muon_outerTrack_muonStationsWithValidHits.clear();
    Muon_innerTrack_p.clear();
    Muon_innerTrack_eta.clear();
    Muon_innerTrack_phi.clear();
    Muon_innerTrack_normalizedChi2.clear();
    Muon_QInnerOuter.clear();
    
    Muon_combinedQuality_updatedSta.clear();
    Muon_combinedQuality_trkKink.clear();
    Muon_combinedQuality_glbKink.clear();
    Muon_combinedQuality_trkRelChi2.clear();
    Muon_combinedQuality_staRelChi2.clear();
    Muon_combinedQuality_chi2LocalPosition.clear();
    Muon_combinedQuality_chi2LocalMomentum.clear();
    Muon_combinedQuality_localDistance.clear();
    Muon_combinedQuality_globalDeltaEtaPhi.clear();
    Muon_combinedQuality_tightMatch.clear();
    Muon_combinedQuality_glbTrackProbability.clear();
    
    Muon_calEnergy_em.clear();
    Muon_calEnergy_emS9.clear();
    Muon_calEnergy_emS25.clear();
    Muon_calEnergy_had.clear();
    Muon_calEnergy_hadS9.clear();
    
    Muon_segmentCompatibility.clear();
    Muon_caloCompatibility.clear();
    
    Muon_ptErrOverPt.clear();
    Muon_BestTrackPt.clear();
    Muon_BestTrackPtErr.clear();
    
    Muon_BestTrackEta.clear();
    Muon_BestTrackEtaErr.clear();
    
    Muon_BestTrackPhi.clear();
    Muon_BestTrackPhiErr.clear();
    
    Muon_emEt03.clear();
    Muon_hadEt03.clear();
    Muon_nJets03.clear();
    Muon_nTracks03.clear();
    Muon_sumPt03.clear();
    Muon_hadVetoEt03.clear();
    Muon_emVetoEt03.clear();
    Muon_trackerVetoPt03.clear();
    
    Muon_emEt05.clear();
    Muon_hadEt05.clear();
    Muon_nJets05.clear();
    Muon_nTracks05.clear();
    Muon_sumPt05.clear();
    Muon_hadVetoEt05.clear();
    Muon_emVetoEt05.clear();
    Muon_trackerVetoPt05.clear();
    
    Quadruplet_mindca_iso.clear();
    Quadruplet_relativeiso.clear();
    
    PV_x.clear();
    PV_y.clear();
    PV_z.clear();
    PV_NTracks.clear();
    
    BS_x.clear();
    BS_y.clear();
    BS_z.clear();
    
    Vtx12_x.clear();
    Vtx23_x.clear();
    Vtx13_x.clear();
    Vtx12_y.clear();
    Vtx23_y.clear();
    Vtx13_y.clear();
    Vtx12_z.clear();
    Vtx23_z.clear();
    Vtx13_z.clear();
    Vtx12_Chi2.clear();
    Vtx23_Chi2.clear();
    Vtx13_Chi2.clear();
    Vtx12_nDOF.clear();
    Vtx23_nDOF.clear();
    Vtx13_nDOF.clear();
    Vtx14_x.clear();
    Vtx24_x.clear();
    Vtx34_x.clear();
    Vtx14_y.clear();
    Vtx24_y.clear();
    Vtx34_y.clear();
    Vtx14_z.clear();
    Vtx24_z.clear();
    Vtx34_z.clear();
    Vtx14_Chi2.clear();
    Vtx24_Chi2.clear();
    Vtx34_Chi2.clear();
    Vtx14_nDOF.clear();
    Vtx24_nDOF.clear();
    Vtx34_nDOF.clear();

    Vtx12_mass.clear();
    Vtx23_mass.clear();
    Vtx13_mass.clear();
    Vtx14_mass.clear();
    Vtx24_mass.clear();
    Vtx34_mass.clear();
    Vtx12_mass_err.clear();
    Vtx23_mass_err.clear();
    Vtx13_mass_err.clear();
    Vtx14_mass_err.clear();
    Vtx24_mass_err.clear();
    Vtx34_mass_err.clear();

    Mu1_Pt.clear();
    Mu1_Eta.clear();
    Mu1_Phi.clear();
    Mu1_NTracks03iso.clear();
    Mu1_dRtriggerMatch.clear();
    
    Mu2_Pt.clear();
    Mu2_Eta.clear();
    Mu2_Phi.clear();
    Mu2_NTracks03iso.clear();
    Mu2_dRtriggerMatch.clear();
    
    Mu3_Pt.clear();
    Mu3_Eta.clear();
    Mu3_Phi.clear();
    Mu3_Charge.clear();
    Mu3_NTracks03iso.clear();
    Mu3_dRtriggerMatch.clear();
    
    Mu4_Pt.clear();
    Mu4_Eta.clear();
    Mu4_Phi.clear();
    Mu4_Charge.clear();
    Mu4_NTracks03iso.clear();
    Mu4_dRtriggerMatch.clear();

    mu1_pfreliso03.clear();
    mu2_pfreliso03.clear();
    mu3_pfreliso03.clear();
    mu4_pfreliso03.clear();
    vtx_ref_prob.clear();

    RefTrack1_Pt.clear();
    RefTrack1_Eta.clear();
    RefTrack1_Phi.clear();
    RefTrack1_QuadrupletIndex.clear();
    
    RefTrack2_Pt.clear();
    RefTrack2_Eta.clear();
    RefTrack2_Phi.clear();
    RefTrack2_QuadrupletIndex.clear();
    
    RefTrack3_Pt.clear();
    RefTrack3_Eta.clear();
    RefTrack3_Phi.clear();
    RefTrack3_QuadrupletIndex.clear();
    
    RefTrack4_Pt.clear();
    RefTrack4_Eta.clear();
    RefTrack4_Phi.clear();
    RefTrack4_QuadrupletIndex.clear();
    
    RefittedSV_Chi2.clear();
    RefittedSV_nDOF.clear();
    RefittedSV_Mass.clear();
    RefittedSV_Mass_err.clear();
    
    IsoTrackMu1_Pt.clear();
    IsoTrackMu1_Eta.clear();
    IsoTrackMu1_Phi.clear();
    
    IsoTrackMu2_Pt.clear();
    IsoTrackMu2_Eta.clear();
    IsoTrackMu2_Phi.clear();
    
    IsoTrackMu3_Pt.clear();
    IsoTrackMu3_Eta.clear();
    IsoTrackMu3_Phi.clear();
    
    IsoTrackMu4_Pt.clear();
    IsoTrackMu4_Eta.clear();
    IsoTrackMu4_Phi.clear();
        
    Mu1_QuadrupletIndex.clear();
    Mu2_QuadrupletIndex.clear();
    Mu3_QuadrupletIndex.clear();
    Mu4_QuadrupletIndex.clear();
    
    GenMatchMu1_SimPhi.clear();
    GenMatchMu2_SimPhi.clear();
    GenMatchMu3_SimPhi.clear();
    GenMatchMu4_SimPhi.clear();
    
    GenMatchMu1_SimPt.clear();
    GenMatchMu2_SimPt.clear();
    GenMatchMu3_SimPt.clear();
    GenMatchMu4_SimPt.clear();
    
    GenMatchMu1_SimEta.clear();
    GenMatchMu2_SimEta.clear();
    GenMatchMu3_SimEta.clear();
    GenMatchMu4_SimEta.clear();
    
    GenMatchMu1_Pt.clear();
    GenMatchMu2_Pt.clear();
    GenMatchMu3_Pt.clear();
    GenMatchMu4_Pt.clear();
    
    GenMatchMu1_Eta.clear();
    GenMatchMu2_Eta.clear();
    GenMatchMu3_Eta.clear();
    GenMatchMu4_Eta.clear();
    
    GenMatchMu1_Phi.clear();
    GenMatchMu2_Phi.clear();
    GenMatchMu3_Phi.clear();
    GenMatchMu4_Phi.clear();
    
    QuadrupletCollectionSize = -99;
    
    QuadrupletVtx_x.clear();
    QuadrupletVtx_y.clear();
    QuadrupletVtx_z.clear();
                                 
    vtx_prob.clear();
    QuadrupletVtx_Chi2.clear();
    QuadrupletVtx_NDOF.clear();
    QuadrupletVtx_cov.clear();
    
    Quadruplet_Mass.clear();
    Quadruplet_Pt.clear();
    Quadruplet_Eta.clear();
    Quadruplet_Phi.clear();
    Quadruplet_Charge.clear();
    
    dxy_mu1.clear();
    dxy_mu2.clear();
    dxy_mu3.clear();
    dxy_mu4.clear();
    dxyErr_mu1.clear();
    dxyErr_mu2.clear();
    dxyErr_mu3.clear();
    dxyErr_mu4.clear();

    mu1_bs_dxy.clear();
    mu2_bs_dxy.clear();
    mu3_bs_dxy.clear();
    mu4_bs_dxy.clear();
    mu1_bs_dxy_err.clear();
    mu2_bs_dxy_err.clear();
    mu3_bs_dxy_err.clear();
    mu4_bs_dxy_err.clear();
    mu1_bs_dxy_sig.clear();
    mu2_bs_dxy_sig.clear();
    mu3_bs_dxy_sig.clear();
    mu4_bs_dxy_sig.clear();
                                 
    RefittedPV_x.clear();
    RefittedPV_y.clear();
    RefittedPV_z.clear();
    RefittedPV_cov.clear();
    RefittedPV_NTracks.clear();
    RefittedPV_isValid.clear();
    //RefittedPV_Chi2.push_back(PVertex.);
    RefittedPV_Chi2.clear();
    RefittedPV_nDOF.clear();
    PV_bis_Chi2.clear();
    PV_bis_nDOF.clear();
    
    FlightDistPVSV.clear();
    FlightDistPVSV_Err.clear();
    FlightDistPVSV_Significance.clear();
    FlightDistPVSV_chi2.clear();
    PVCollection_Size =0;
    
    Trigger_l1name.clear();
    Trigger_l1Initialdecision.clear();
    Trigger_l1Finaldecision.clear();
    Trigger_l1prescale.clear();
    
    Trigger_hltname.clear();
    Trigger_hltdecision.clear();
    NGoodQuadruplets.clear();
    Quadruplet_relativeiso2.clear();
    
    MuonPt_HLT.clear();
    MuonEta_HLT.clear();
    MuonPhi_HLT.clear();
            
    DistXY_PVSV.clear();
    DistXY_significance_PVSV.clear();
    Quadruplet_IsoMu1.clear();
    Quadruplet_IsoMu2.clear();
    Quadruplet_IsoMu3.clear();
    FlightDistBS_SV.clear();
    FlightDistBS_SV_Err.clear();
    FlightDistBS_SV_Significance.clear();
    
    Mu1_IsGlobal.clear(); Mu2_IsGlobal.clear(); Mu3_IsGlobal.clear(); Mu4_IsGlobal.clear();
    
    Mu1_IsPF.clear(); Mu2_IsPF.clear(); Mu3_IsPF.clear(); Mu4_IsPF.clear();
    
    L1Muon_Pt.clear(); L1Muon_Eta.clear(); L1Muon_Phi.clear(); L1Muon_EtaAtVtx.clear(); L1Muon_PhiAtVtx.clear(); L1Muon_BX.clear(); L1Muon_Quality.clear(); L1Muon_ChargeValid.clear(); L1Muon_Charge.clear(); L1Muon_TfMuonIndex.clear(); L1Muon_rank.clear(); L1Muon_isoSum.clear();
}

// ------------ method called once each job just before starting event loop  ------------
void MiniAnaB2Mu2K::beginJob() {
    
    hEvents = fs->make<TH1F>("hEvents","hEvents",10,0,10);
    hEventsAfterGoodCand = fs->make<TH1F>("hEventsAfterGoodCand","hEventsAfterGoodCand",10,0,10);
    
    //      if(NQuad.size()>0) hEventsAfterGoodCand->Fill(1);
    hEventsAfterMu1ID = fs->make<TH1F>("hEventsAfterMu1ID","hEventsAfterMu1ID",10,0,10);
    hEventsAfterMu2ID = fs->make<TH1F>("hEventsAfterMu2ID","hEventsAfterMu2ID",10,0,10);
    hEventsAfterMu3ID = fs->make<TH1F>("hEventsAfterMu3ID","hEventsAfterMu3ID",10,0,10);
    hEventsAfterMu4ID = fs->make<TH1F>("hEventsAfterMu4ID","hEventsAfterMu4ID",10,0,10);
    hEventsAfterBMass = fs->make<TH1F>("hEventsAfterBMass","hEventsAfterBMass",10,0,10);
    
    tree_ = fs->make<TTree>("ntuple","LFVTau ntuple");
    tree_->Branch("evt", &evt);
    tree_->Branch("run", &run);
    tree_->Branch("lumi", &lumi);
    tree_->Branch("NGoodQuadruplets", &NGoodQuadruplets);
    tree_->Branch("nPileUpInt", &puN);
    
    tree_->Branch("Trigger_l1name", &Trigger_l1name);
    tree_->Branch("Trigger_l1Initialdecision",&Trigger_l1Initialdecision);
    tree_->Branch("Trigger_l1Finaldecision",&Trigger_l1Finaldecision);
    tree_->Branch("Trigger_l1prescale",&Trigger_l1prescale);
    
    tree_->Branch("Trigger_hltname",&Trigger_hltname);
    tree_->Branch("Trigger_hltdecision",&Trigger_hltdecision);
    
    tree_->Branch("GenParticle_PdgId", &GenParticle_PdgId);
    tree_->Branch("GenParticle_Pt", &GenParticle_Pt);
    tree_->Branch("GenParticle_Eta", &GenParticle_Eta);
    tree_->Branch("GenParticle_Phi", &GenParticle_Phi);
    tree_->Branch("GenParticle_MotherPt", &GenParticle_MotherPt);
    tree_->Branch("GenParticle_MotherEta", &GenParticle_MotherEta);
    tree_->Branch("GenParticle_MotherPhi", &GenParticle_MotherPhi);
                                 
    tree_->Branch("GenParticle_Pt_v2", &GenParticle_Pt_v2);
    tree_->Branch("GenParticle_Eta_v2", &GenParticle_Eta_v2);
    tree_->Branch("GenParticle_Phi_v2", &GenParticle_Phi_v2);
    tree_->Branch("GenParticle_PdgId_v2", &GenParticle_PdgId_v2);

    tree_->Branch("GenParticle_Pt_trk", &GenParticle_Pt_trk);
    tree_->Branch("GenParticle_Eta_trk", &GenParticle_Eta_trk);
    tree_->Branch("GenParticle_Phi_trk", &GenParticle_Phi_trk);
    tree_->Branch("GenParticle_PdgId_trk", &GenParticle_PdgId_trk);

    tree_->Branch("GenParticle_MotherPdgId", &GenParticle_MotherPdgId);
    tree_->Branch("GenParticle_GrandMotherPdgId", &GenParticle_GrandMotherPdgId);
    tree_->Branch("GenParticle_vx", &GenParticle_vx);
    tree_->Branch("GenParticle_vy", &GenParticle_vy);
    tree_->Branch("GenParticle_vz", &GenParticle_vz);
    
    tree_->Branch("PhotonCollectionSize",&PhotonCollectionSize);
    tree_->Branch("PhotonPt",&PhotonPt);
    tree_->Branch("PhotonEt",&PhotonEt);
    tree_->Branch("PhotonEnergy",&PhotonEnergy);
    tree_->Branch("PhotonEta",&PhotonEta);
    tree_->Branch("PhotonPhi",&PhotonPhi);
    
    tree_->Branch("MuonCollectionSize",&MuonCollectionSize);
    tree_->Branch("MuonPt",&MuonPt);
    tree_->Branch("MuonEnergy", &MuonEnergy);
    tree_->Branch("MuonCharge", &MuonCharge);
    tree_->Branch("MuonEta",&MuonEta);
    tree_->Branch("MuonPhi",&MuonPhi);
    tree_->Branch("Muon_simPdgId", &Muon_simPdgId);
    tree_->Branch("Muon_simMotherPdgId", &Muon_simMotherPdgId);
    tree_->Branch("Muon_simFlavour", &Muon_simFlavour);
    tree_->Branch("Muon_simHeaviestMotherFlavour",&Muon_simHeaviestMotherFlavour);
    tree_->Branch("Muon_simType", &Muon_simType);
    tree_->Branch("Muon_simBX", &Muon_simBX);
    
    /////Vtx position
    tree_->Branch("Muon_vx", &Muon_vx);
    tree_->Branch("Muon_vy", &Muon_vy);
    tree_->Branch("Muon_vz", &Muon_vz);
    
    /////MuonID
    tree_->Branch("Muon_isGlobal", &Muon_isGlobal);
    tree_->Branch("Muon_isSoft", &Muon_isSoft);
    tree_->Branch("Muon_isMVA", &Muon_isMVA);
    tree_->Branch("Muon_isMVASoft", &Muon_isMVASoft);
    tree_->Branch("Muon_isLoose", &Muon_isLoose);
    tree_->Branch("Muon_isMedium", &Muon_isMedium);
    tree_->Branch("Muon_isTight", &Muon_isTight);
    tree_->Branch("Muon_isPF", &Muon_isPF);
    tree_->Branch("Muon_isRPCMuon", &Muon_isRPCMuon);
    tree_->Branch("Muon_isStandAloneMuon", &Muon_isStandAloneMuon);
    tree_->Branch("Muon_isTrackerMuon", &Muon_isTrackerMuon);
    tree_->Branch("Muon_isCaloMuon", &Muon_isCaloMuon);
    tree_->Branch("Muon_isQualityValid", &Muon_isQualityValid);
    tree_->Branch("Muon_isTimeValid", &Muon_isTimeValid);
    tree_->Branch("Muon_isIsolationValid", &Muon_isIsolationValid);
    tree_->Branch("Muon_numberOfMatchedStations", &Muon_numberOfMatchedStations);
    tree_->Branch("Muon_numberOfMatches", &Muon_numberOfMatches);
    tree_->Branch("Muon_SoftMVA_Val", &Muon_SoftMVA_Val);
    
    tree_->Branch("Muon_timeAtIpInOut",&Muon_timeAtIpInOut);
    tree_->Branch("Muon_timeAtIpInOutErr",&Muon_timeAtIpInOutErr);
    //////Muon inner + outer track
    tree_->Branch("Muon_GLnormChi2", &Muon_GLnormChi2);
    tree_->Branch("Muon_GLhitPattern_numberOfValidMuonHits", &Muon_GLhitPattern_numberOfValidMuonHits);
    
    tree_->Branch("Muon_trackerLayersWithMeasurement", &Muon_trackerLayersWithMeasurement);
    tree_->Branch("Muon_Numberofvalidpixelhits", &Muon_Numberofvalidpixelhits);
    
    tree_->Branch("Muon_outerTrack_p", &Muon_outerTrack_p);
    tree_->Branch("Muon_outerTrack_eta", &Muon_outerTrack_eta);
    tree_->Branch("Muon_outerTrack_phi", &Muon_outerTrack_phi);
    tree_->Branch("Muon_outerTrack_normalizedChi2", &Muon_outerTrack_normalizedChi2);
    tree_->Branch("Muon_outerTrack_muonStationsWithValidHits", &Muon_outerTrack_muonStationsWithValidHits);
    tree_->Branch("Muon_innerTrack_p", &Muon_innerTrack_p);
    tree_->Branch("Muon_innerTrack_eta", &Muon_innerTrack_eta);
    
    tree_->Branch("Muon_innerTrack_phi", &Muon_innerTrack_phi);
    tree_->Branch("Muon_innerTrack_normalizedChi2", &Muon_innerTrack_normalizedChi2);
    tree_->Branch("Muon_QInnerOuter", &Muon_QInnerOuter);
    
    tree_->Branch("Muon_combinedQuality_updatedSta", &Muon_combinedQuality_updatedSta);
    tree_->Branch("Muon_combinedQuality_trkKink", &Muon_combinedQuality_trkKink);
    tree_->Branch("Muon_combinedQuality_glbKink", &Muon_combinedQuality_glbKink);
    tree_->Branch("Muon_combinedQuality_trkRelChi2", &Muon_combinedQuality_trkRelChi2);
    tree_->Branch("Muon_combinedQuality_staRelChi2", &Muon_combinedQuality_staRelChi2);
    tree_->Branch("Muon_combinedQuality_chi2LocalPosition", &Muon_combinedQuality_chi2LocalPosition);
    tree_->Branch("Muon_combinedQuality_chi2LocalMomentum", &Muon_combinedQuality_chi2LocalMomentum);
    tree_->Branch("Muon_combinedQuality_localDistance", &Muon_combinedQuality_localDistance);
    tree_->Branch("Muon_combinedQuality_globalDeltaEtaPhi", &Muon_combinedQuality_globalDeltaEtaPhi);
    tree_->Branch("Muon_combinedQuality_tightMatch", &Muon_combinedQuality_tightMatch);
    tree_->Branch("Muon_combinedQuality_glbTrackProbability", &Muon_combinedQuality_glbTrackProbability);
    
    tree_->Branch("Muon_calEnergy_em", &Muon_calEnergy_em);
    tree_->Branch("Muon_calEnergy_emS9", &Muon_calEnergy_emS9);
    tree_->Branch("Muon_calEnergy_emS25", &Muon_calEnergy_emS25);
    tree_->Branch("Muon_calEnergy_had", &Muon_calEnergy_had);
    tree_->Branch("Muon_calEnergy_hadS9", &Muon_calEnergy_hadS9);
    
    tree_->Branch("Muon_segmentCompatibility", &Muon_segmentCompatibility);
    tree_->Branch("Muon_caloCompatibility", &Muon_caloCompatibility);
    
    tree_->Branch("Muon_ptErrOverPt", &Muon_ptErrOverPt);
    tree_->Branch("Muon_BestTrackPt", &Muon_BestTrackPt);
    tree_->Branch("Muon_BestTrackPtErr", &Muon_BestTrackPtErr);
    tree_->Branch("Muon_BestTrackEta", &Muon_BestTrackEta);
    tree_->Branch("Muon_BestTrackEtaErr", &Muon_BestTrackEtaErr);
    tree_->Branch("Muon_BestTrackPhi", &Muon_BestTrackPhi);
    tree_->Branch("Muon_BestTrackPhiErr", &Muon_BestTrackPhiErr);
    
    tree_->Branch("Muon_emEt03", &Muon_emEt03);
    tree_->Branch("Muon_hadEt03", &Muon_hadEt03);
    tree_->Branch("Muon_nJets03", &Muon_nJets03);
    tree_->Branch("Muon_nTracks03", &Muon_nTracks03);
    tree_->Branch("Muon_sumPt03", &Muon_sumPt03);
    tree_->Branch("Muon_hadVetoEt03", &Muon_hadVetoEt03);
    tree_->Branch("Muon_emVetoEt03", &Muon_emVetoEt03);
    tree_->Branch("Muon_trackerVetoPt03", &Muon_trackerVetoPt03);
    
    tree_->Branch("Muon_emEt05", &Muon_emEt05);
    tree_->Branch("Muon_hadEt05", &Muon_hadEt05);
    tree_->Branch("Muon_nJets05", &Muon_nJets05);
    tree_->Branch("Muon_nTracks05", &Muon_nTracks05);
    tree_->Branch("Muon_sumPt05", &Muon_sumPt05);
    tree_->Branch("Muon_hadVetoEt05", &Muon_hadVetoEt05);
    tree_->Branch("Muon_emVetoEt05", &Muon_emVetoEt05);
    tree_->Branch("Muon_trackerVetoPt05", &Muon_trackerVetoPt05);
    
    tree_->Branch("Muon_innerTrack_highPurity", &Muon_innerTrack_highPurity);
    tree_->Branch("Muon_innerTrack_ValidFraction", &Muon_innerTrack_ValidFraction);
    tree_->Branch("Muon_Numberofvalidtrackerhits", &Muon_Numberofvalidtrackerhits);
    tree_->Branch("Muon_validMuonHitComb", &Muon_validMuonHitComb);
    tree_->Branch("Muon_IP2D_BS", &Muon_IP2D_BS);
    tree_->Branch("Muon_IP3D_BS", &Muon_IP3D_BS);
    tree_->Branch("Muon_IP2D_PV", &Muon_IP2D_PV);
    tree_->Branch("Muon_IP3D_PV", &Muon_IP3D_PV);
    
    tree_->Branch("PVCollection_Size", &PVCollection_Size);
    tree_->Branch("PV_x", &PV_x);
    tree_->Branch("PV_y", &PV_y);
    tree_->Branch("PV_z", &PV_z);
    tree_->Branch("PV_NTracks", &PV_NTracks);
    
    tree_->Branch("BS_x", &BS_x);
    tree_->Branch("BS_y", &BS_y);
    tree_->Branch("BS_z", &BS_z);
    
    tree_->Branch("Vtx12_x", &Vtx12_x);
    tree_->Branch("Vtx23_x", &Vtx23_x);
    tree_->Branch("Vtx13_x", &Vtx13_x);
    tree_->Branch("Vtx12_y", &Vtx12_y);
    tree_->Branch("Vtx23_y", &Vtx23_y);
    tree_->Branch("Vtx13_y", &Vtx13_y);
    tree_->Branch("Vtx12_z", &Vtx12_z);
    tree_->Branch("Vtx23_z", &Vtx23_z);
    tree_->Branch("Vtx13_z", &Vtx13_z);
    tree_->Branch("Vtx12_Chi2", &Vtx12_Chi2);
    tree_->Branch("Vtx23_Chi2", &Vtx23_Chi2);
    tree_->Branch("Vtx13_Chi2", &Vtx13_Chi2);
    tree_->Branch("Vtx12_nDOF", &Vtx12_nDOF);
    tree_->Branch("Vtx23_nDOF", &Vtx23_nDOF);
    tree_->Branch("Vtx13_nDOF", &Vtx13_nDOF);
    tree_->Branch("Vtx14_x", &Vtx14_x);
    tree_->Branch("Vtx24_x", &Vtx24_x);
    tree_->Branch("Vtx34_x", &Vtx34_x);
    tree_->Branch("Vtx14_y", &Vtx14_y);
    tree_->Branch("Vtx24_y", &Vtx24_y);
    tree_->Branch("Vtx34_y", &Vtx34_y);
    tree_->Branch("Vtx14_z", &Vtx14_z);
    tree_->Branch("Vtx24_z", &Vtx24_z);
    tree_->Branch("Vtx34_z", &Vtx34_z);
    tree_->Branch("Vtx14_Chi2", &Vtx14_Chi2);
    tree_->Branch("Vtx24_Chi2", &Vtx24_Chi2);
    tree_->Branch("Vtx34_Chi2", &Vtx34_Chi2);
    tree_->Branch("Vtx14_nDOF", &Vtx14_nDOF);
    tree_->Branch("Vtx24_nDOF", &Vtx24_nDOF);
    tree_->Branch("Vtx34_nDOF", &Vtx34_nDOF);
    
    tree_->Branch("Vtx12_mass", &Vtx12_mass);
    tree_->Branch("Vtx23_mass", &Vtx23_mass);
    tree_->Branch("Vtx13_mass", &Vtx13_mass);
    tree_->Branch("Vtx14_mass", &Vtx14_mass);
    tree_->Branch("Vtx24_mass", &Vtx24_mass);
    tree_->Branch("Vtx34_mass", &Vtx34_mass);
    tree_->Branch("Vtx12_mass_err", &Vtx12_mass_err);
    tree_->Branch("Vtx23_mass_err", &Vtx23_mass_err);
    tree_->Branch("Vtx13_mass_err", &Vtx13_mass_err);
    tree_->Branch("Vtx14_mass_err", &Vtx14_mass_err);
    tree_->Branch("Vtx24_mass_err", &Vtx24_mass_err);
    tree_->Branch("Vtx34_mass_err", &Vtx34_mass_err);

    tree_->Branch("QuadrupletCollectionSize", &QuadrupletCollectionSize);
    tree_->Branch("Mu1_Pt",&Mu1_Pt);
    tree_->Branch("Mu1_Eta", &Mu1_Eta);
    tree_->Branch("Mu1_Phi", &Mu1_Phi);
    tree_->Branch("Mu1_NTracks03iso", &Mu1_NTracks03iso);
    tree_->Branch("Mu1_QuadrupletIndex", &Mu1_QuadrupletIndex);
    tree_->Branch("Mu1_dRtriggerMatch", &Mu1_dRtriggerMatch);
        
    tree_->Branch("Mu2_Pt", &Mu2_Pt);
    tree_->Branch("Mu2_Eta", &Mu2_Eta);
    tree_->Branch("Mu2_Phi", &Mu2_Phi);
    tree_->Branch("Mu2_NTracks03iso", &Mu2_NTracks03iso);
    tree_->Branch("Mu2_dRtriggerMatch", &Mu2_dRtriggerMatch);
    tree_->Branch("Mu2_QuadrupletIndex", &Mu2_QuadrupletIndex);
    
    tree_->Branch("Mu3_Pt", &Mu3_Pt);
    tree_->Branch("Mu3_Eta",&Mu3_Eta);
    tree_->Branch("Mu3_Phi", &Mu3_Phi);
    tree_->Branch("Mu3_Charge", &Mu3_Charge);
    tree_->Branch("Mu3_NTracks03iso", &Mu3_NTracks03iso);
    tree_->Branch("Mu3_dRtriggerMatch", &Mu3_dRtriggerMatch);
    tree_->Branch("Mu3_QuadrupletIndex", &Mu3_QuadrupletIndex);
    
    tree_->Branch("Mu4_Pt", &Mu4_Pt);
    tree_->Branch("Mu4_Eta",&Mu4_Eta);
    tree_->Branch("Mu4_Phi", &Mu4_Phi);
    tree_->Branch("Mu4_Charge", &Mu4_Charge);
    tree_->Branch("Mu4_NTracks03iso", &Mu4_NTracks03iso);
    tree_->Branch("Mu4_dRtriggerMatch", &Mu4_dRtriggerMatch);
    tree_->Branch("Mu4_QuadrupletIndex", &Mu4_QuadrupletIndex);
    
    tree_->Branch("Mu1_IsGlobal", &Mu1_IsGlobal);
    tree_->Branch("Mu2_IsGlobal", &Mu2_IsGlobal);
    tree_->Branch("Mu3_IsGlobal", &Mu3_IsGlobal);
    tree_->Branch("Mu4_IsGlobal", &Mu4_IsGlobal);
    
    tree_->Branch("Mu1_IsPF", &Mu1_IsPF);
    tree_->Branch("Mu2_IsPF", &Mu2_IsPF);
    tree_->Branch("Mu3_IsPF", &Mu3_IsPF);
    tree_->Branch("Mu4_IsPF", &Mu4_IsPF);
    
    tree_->Branch("dxy_mu1", &dxy_mu1);
    tree_->Branch("dxy_mu2", &dxy_mu2);
    tree_->Branch("dxy_mu3", &dxy_mu3);
    tree_->Branch("dxy_mu4", &dxy_mu4);
    
    tree_->Branch("dxyErr_mu1", &dxyErr_mu1);
    tree_->Branch("dxyErr_mu2", &dxyErr_mu2);
    tree_->Branch("dxyErr_mu3", &dxyErr_mu3);
    tree_->Branch("dxyErr_mu4", &dxyErr_mu4);

    tree_->Branch("mu1_bs_dxy", &mu1_bs_dxy);
    tree_->Branch("mu2_bs_dxy", &mu2_bs_dxy);
    tree_->Branch("mu3_bs_dxy", &mu3_bs_dxy);
    tree_->Branch("mu4_bs_dxy", &mu4_bs_dxy);
    tree_->Branch("mu1_bs_dxy_err", &mu1_bs_dxy_err);
    tree_->Branch("mu2_bs_dxy_err", &mu2_bs_dxy_err);
    tree_->Branch("mu3_bs_dxy_err", &mu3_bs_dxy_err);
    tree_->Branch("mu4_bs_dxy_err", &mu4_bs_dxy_err);
    tree_->Branch("mu1_bs_dxy_sig", &mu1_bs_dxy_sig);
    tree_->Branch("mu2_bs_dxy_sig", &mu2_bs_dxy_sig);
    tree_->Branch("mu3_bs_dxy_sig", &mu3_bs_dxy_sig);
    tree_->Branch("mu4_bs_dxy_sig", &mu4_bs_dxy_sig);
                                 
    tree_->Branch("RefTrack1_Pt",           &RefTrack1_Pt);
    tree_->Branch("RefTrack1_Eta",          &RefTrack1_Eta);
    tree_->Branch("RefTrack1_Phi",          &RefTrack1_Phi);
    tree_->Branch("RefTrack1_QuadrupletIndex", &RefTrack1_QuadrupletIndex);
    tree_->Branch("RefTrack2_Pt",           &RefTrack2_Pt);
    tree_->Branch("RefTrack2_Eta",          &RefTrack2_Eta);
    tree_->Branch("RefTrack2_Phi",          &RefTrack2_Phi);
    tree_->Branch("RefTrack2_QuadrupletIndex", &RefTrack2_QuadrupletIndex);
    tree_->Branch("RefTrack3_Pt",           &RefTrack3_Pt);
    tree_->Branch("RefTrack3_Eta",          &RefTrack3_Eta);
    tree_->Branch("RefTrack3_Phi",          &RefTrack3_Phi);
    tree_->Branch("RefTrack3_QuadrupletIndex", &RefTrack3_QuadrupletIndex);
    tree_->Branch("RefTrack4_Pt",           &RefTrack4_Pt);
    tree_->Branch("RefTrack4_Eta",          &RefTrack4_Eta);
    tree_->Branch("RefTrack4_Phi",          &RefTrack4_Phi);
    tree_->Branch("RefTrack4_QuadrupletIndex", &RefTrack4_QuadrupletIndex);
    
    tree_->Branch("RefittedSV_Chi2", &RefittedSV_Chi2);
    tree_->Branch("RefittedSV_nDOF", &RefittedSV_nDOF);
    tree_->Branch("RefittedSV_Mass", &RefittedSV_Mass);
    tree_->Branch("RefittedSV_Mass_err", &RefittedSV_Mass_err);
    
    tree_->Branch("IsoTrackMu1_Pt",         &IsoTrackMu1_Pt);
    tree_->Branch("IsoTrackMu1_Eta",        &IsoTrackMu1_Eta);
    tree_->Branch("IsoTrackMu1_Phi",        &IsoTrackMu1_Phi);
    tree_->Branch("IsoTrackMu2_Pt",         &IsoTrackMu2_Pt);
    tree_->Branch("IsoTrackMu2_Eta",        &IsoTrackMu2_Eta);
    tree_->Branch("IsoTrackMu2_Phi",        &IsoTrackMu2_Phi);
    tree_->Branch("IsoTrackMu3_Pt",         &IsoTrackMu3_Pt);
    tree_->Branch("IsoTrackMu3_Eta",        &IsoTrackMu3_Eta);
    tree_->Branch("IsoTrackMu3_Phi",        &IsoTrackMu3_Phi);
    tree_->Branch("IsoTrackMu4_Pt",         &IsoTrackMu4_Pt);
    tree_->Branch("IsoTrackMu4_Eta",        &IsoTrackMu4_Eta);
    tree_->Branch("IsoTrackMu4_Phi",        &IsoTrackMu4_Phi);
    
    tree_->Branch("mu1_pfreliso03", &mu1_pfreliso03);
    tree_->Branch("mu2_pfreliso03", &mu2_pfreliso03);
    tree_->Branch("mu3_pfreliso03", &mu3_pfreliso03);
    tree_->Branch("mu4_pfreliso03", &mu4_pfreliso03);
    tree_->Branch("vtx_prob", &vtx_prob);
    tree_->Branch("vtx_ref_prob", &vtx_ref_prob);

    tree_->Branch("GenMatchMu1_SimPt", &GenMatchMu1_SimPt);
    tree_->Branch("GenMatchMu2_SimPt", &GenMatchMu2_SimPt);
    tree_->Branch("GenMatchMu3_SimPt", &GenMatchMu3_SimPt);
    tree_->Branch("GenMatchMu4_SimPt", &GenMatchMu4_SimPt);
    
    tree_->Branch("GenMatchMu1_SimEta", &GenMatchMu1_SimEta);
    tree_->Branch("GenMatchMu2_SimEta", &GenMatchMu2_SimEta);
    tree_->Branch("GenMatchMu3_SimEta", &GenMatchMu3_SimEta);
    tree_->Branch("GenMatchMu4_SimEta", &GenMatchMu4_SimEta);
    
    tree_->Branch("GenMatchMu1_SimPhi", &GenMatchMu1_SimPhi);
    tree_->Branch("GenMatchMu2_SimPhi", &GenMatchMu2_SimPhi);
    tree_->Branch("GenMatchMu3_SimPhi", &GenMatchMu3_SimPhi);
    tree_->Branch("GenMatchMu4_SimPhi", &GenMatchMu4_SimPhi);
    
    tree_->Branch("GenMatchMu1_Pt", &GenMatchMu1_Pt);
    tree_->Branch("GenMatchMu2_Pt", &GenMatchMu2_Pt);
    tree_->Branch("GenMatchMu3_Pt", &GenMatchMu3_Pt);
    tree_->Branch("GenMatchMu4_Pt", &GenMatchMu4_Pt);
    
    tree_->Branch("GenMatchMu1_Eta", &GenMatchMu1_Eta);
    tree_->Branch("GenMatchMu2_Eta", &GenMatchMu2_Eta);
    tree_->Branch("GenMatchMu3_Eta", &GenMatchMu3_Eta);
    tree_->Branch("GenMatchMu4_Eta", &GenMatchMu4_Eta);
    
    tree_->Branch("GenMatchMu1_Phi", &GenMatchMu1_Phi);
    tree_->Branch("GenMatchMu2_Phi", &GenMatchMu2_Phi);
    tree_->Branch("GenMatchMu3_Phi", &GenMatchMu3_Phi);
    tree_->Branch("GenMatchMu4_Phi", &GenMatchMu4_Phi);
    
    tree_->Branch("QuadrupletVtx_x", &QuadrupletVtx_x);
    tree_->Branch("QuadrupletVtx_y", &QuadrupletVtx_y);
    tree_->Branch("QuadrupletVtx_z", &QuadrupletVtx_z);
    
    tree_->Branch("QuadrupletVtx_Chi2", &QuadrupletVtx_Chi2);
    tree_->Branch("QuadrupletVtx_NDOF", &QuadrupletVtx_NDOF);
    tree_->Branch("QuadrupletVtx_cov", &QuadrupletVtx_cov);
    
    tree_->Branch("Quadruplet_Mass", &Quadruplet_Mass);
    tree_->Branch("Quadruplet_Pt", &Quadruplet_Pt);
    tree_->Branch("Quadruplet_Eta", &Quadruplet_Eta);
    tree_->Branch("Quadruplet_Phi", &Quadruplet_Phi);
    tree_->Branch("Quadruplet_Charge", &Quadruplet_Charge);
    
    tree_->Branch("Quadruplet_mindca_iso", &Quadruplet_mindca_iso);
    tree_->Branch("Quadruplet_relativeiso", &Quadruplet_relativeiso);
    tree_->Branch("Quadruplet_relativeiso2", &Quadruplet_relativeiso2);
    tree_->Branch("RefittedPV_x", &RefittedPV_x);
    tree_->Branch("RefittedPV_y", &RefittedPV_y);
    tree_->Branch("RefittedPV_z", &RefittedPV_z);
    tree_->Branch("RefittedPV_cov", &RefittedPV_cov);
    tree_->Branch("RefittedPV_NTracks", &RefittedPV_NTracks);
    tree_->Branch("RefittedPV_isValid", &RefittedPV_isValid);
    //RefittedPV_Chi2.push_back(PVertex.);
    tree_->Branch("RefittedPV_Chi2", &RefittedPV_Chi2);
    tree_->Branch("RefittedPV_nDOF", &RefittedPV_nDOF);
    tree_->Branch("PV_bis_Chi2", &PV_bis_Chi2);
    tree_->Branch("PV_bis_nDOF", &PV_bis_nDOF);
    
    tree_->Branch("FlightDistPVSV", &FlightDistPVSV);
    tree_->Branch("FlightDistPVSV_Err", &FlightDistPVSV_Err);
    tree_->Branch("FlightDistPVSV_Significance", &FlightDistPVSV_Significance);
    tree_->Branch("FlightDistPVSV_chi2", &FlightDistPVSV_chi2);
    
    tree_->Branch("MuonPt_HLT", &MuonPt_HLT);
    tree_->Branch("MuonEta_HLT", &MuonEta_HLT);
    tree_->Branch("MuonPhi_HLT", &MuonPhi_HLT);
        
    tree_->Branch("DistXY_PVSV", &DistXY_PVSV);
    tree_->Branch("DistXY_significance_PVSV", &DistXY_significance_PVSV);
    tree_->Branch("Quadruplet_IsoMu1", &Quadruplet_IsoMu1);
    tree_->Branch("Quadruplet_IsoMu2", &Quadruplet_IsoMu2);
    tree_->Branch("Quadruplet_IsoMu3", &Quadruplet_IsoMu3);
    tree_->Branch("FlightDistBS_SV", &FlightDistBS_SV);
    tree_->Branch("FlightDistBS_SV_Err", &FlightDistBS_SV_Err);
    tree_->Branch("FlightDistBS_SV_Significance", &FlightDistBS_SV_Significance);
    
    tree_->Branch("L1Muon_Pt", &L1Muon_Pt);
    tree_->Branch("L1Muon_Eta", &L1Muon_Eta);
    tree_->Branch("L1Muon_Phi", &L1Muon_Phi);
    tree_->Branch("L1Muon_EtaAtVtx", &L1Muon_EtaAtVtx);
    tree_->Branch("L1Muon_PhiAtVtx", &L1Muon_PhiAtVtx);
    tree_->Branch("L1Muon_BX", &L1Muon_BX);
    tree_->Branch("L1Muon_Quality", &L1Muon_Quality);
    tree_->Branch("L1Muon_Charge", &L1Muon_Charge);
    tree_->Branch("L1Muon_ChargeValid", &L1Muon_ChargeValid);
    tree_->Branch("L1Muon_TfMuonIndex", &L1Muon_TfMuonIndex);
    tree_->Branch("L1Muon_rank", &L1Muon_rank);
    tree_->Branch("L1Muon_isoSum", &L1Muon_isoSum);
}//MiniAnaB2Mu2K::beginJob

// ------------ method called once each job just after ending the event loop  ------------
void MiniAnaB2Mu2K::endJob() {
    
    tree_->GetDirectory()->cd();
    tree_->Write();
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void MiniAnaB2Mu2K::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MiniAnaB2Mu2K);
