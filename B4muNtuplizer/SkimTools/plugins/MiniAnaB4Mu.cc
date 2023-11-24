// -*- C++ -*-
// Package:    MiniAna2017/MiniAnaTau3Mu
// Class:      MiniAnaTau3Mu
//
/**\class MiniAnaTau3Mu MiniAnaTau3Mu.cc MiniAna2017/MiniAnaTau3Mu/plugins/MiniAnaTau3Mu.cc
 
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
//#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
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


#include "RecoVertex/KinematicFit/interface/KinematicParticleVertexFitter.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleFitter.h"
#include "RecoVertex/KinematicFit/interface/MassKinematicConstraint.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/TransientTrackKinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/TransientTrackKinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParticleFactoryFromTransientTrack.h"
#include "RecoVertex/AdaptiveVertexFit/interface/AdaptiveVertexFitter.h"


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

#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/PatCandidates/interface/TriggerFilter.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
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
////
class MiniAnaTau3Mu : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
public:
    explicit MiniAnaTau3Mu(const edm::ParameterSet&);
    ~MiniAnaTau3Mu();
    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
    float dR(float eta1, float eta2, float phi1, float phi2);
    //float dRtriggerMatch(pat::Muon m, trigger::TriggerObjectCollection triggerObjects);
    float dRtriggerMatch(pat::Muon m, vector<pat::TriggerObjectStandAlone> triggerObjects);
    void beginRun(edm::Run const &, edm::EventSetup const&, edm::Event const&);
    
    
private:
    virtual void beginJob() override;
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
    // virtual void beginRun(edm::Run const &, edm::EventSetup const&) override;
    virtual void endJob() override;
    edm::EDGetTokenT<edm::View<pat::Muon> > muons_;
    edm::EDGetTokenT<edm::View<reco::Vertex> > vertex_;
    edm::EDGetTokenT<edm::View<reco::Track> > trackToken_;
    edm::EDGetTokenT<std::vector<pat::PackedCandidate> >srcCands_;
    edm::EDGetTokenT<edm::View<reco::CompositeCandidate> > Cand3Mu_;
    edm::EDGetTokenT<edm::View<reco::GenParticle> > genParticles_;
    edm::EDGetTokenT<std::vector<PileupSummaryInfo> > puToken_ ;
    edm::EDGetTokenT<edm::TriggerResults> triggerToken_;
    edm::EDGetTokenT<BXVector<l1t::Muon> > l1muonsToken_;
    edm::EDGetTokenT<reco::BeamSpot> token_BeamSpot;
    //edm::EDGetTokenT<trigger::TriggerEvent> trigeventToken_;
    edm::EDGetTokenT<edm::View<pat::Photon> > photons_;
    edm::EDGetTokenT<std::vector<pat::TriggerObjectStandAlone> > triggerObjects_;
    edm::EDGetToken algToken_;
    edm::EDGetToken algTok_;
    edm::ESGetToken<TransientTrackBuilder, TransientTrackRecord> theTransientTrackBuilder_;
    //edm::ESGetToken<L1TUtmTriggerMenu, L1TUtmTriggerMenuRcd> m_l1tMenuToken;
    //edm::EDGetTokenT<GlobalAlgBlkBxCollection> m_l1t_results;
    bool isMc;
    bool isAna;
    bool is2016;
    bool is2017;
    bool is2018;
    bool isBParking;
    //edm::EDGetTokenT<edm::TriggerResults> trigResultsToken;
    //edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> trigObjCollToken;
    //const TransientTrackBuilder* theTransientTrackBuilder_;
    HLTConfigProvider hltConfig;
    //TPMERegexp* _re;
    
    edm::Service<TFileService> fs;
    l1t::L1TGlobalUtil* gtUtil_;
    TH1F *hEvents;
    TH1F *hEventsAfterGoodCand;
    TH1F *  hEventsAfterMu1ID;
    TH1F *  hEventsAfterMu2ID;
    TH1F *  hEventsAfterMu3ID;
    TH1F *  hEventsAfterTauMass;

    edm::InputTag algInputTag_;
    const edm::InputTag algTag_, extTag_;

    //tree
    TTree*      tree_;
    std::vector<float>  MuonPt, MuonEta, MuonPhi;
    std::vector<double> MuonEnergy,  MuonCharge;
    
    std::vector<double> PhotonPt, PhotonEt, PhotonEnergy, PhotonEta, PhotonPhi;

    std::vector<int> GenParticle_PdgId, GenParticle_MotherPdgId, GenParticle_GrandMotherPdgId;
    std::vector<double> GenParticle_Pt, GenParticle_Eta,    GenParticle_Phi, GenParticle_vx, GenParticle_vy, GenParticle_vz;
    
    //Vtx position
    std::vector<double>  Muon_vx,  Muon_vy,  Muon_vz;
    
    //MuonID
    std::vector<double>  Muon_isGlobal,  Muon_isTracker,  Muon_isSoft, Muon_isMVA, Muon_isMVASoft,  Muon_isLoose, Muon_isMedium, Muon_isTight,  Muon_isPF,  Muon_isRPCMuon,  Muon_isStandAloneMuon,  Muon_isTrackerMuon,  Muon_isCaloMuon,  Muon_isQualityValid,  Muon_isTimeValid,  Muon_isIsolationValid,  Muon_numberOfMatchedStations,  Muon_numberOfMatches;
    
    //MuonTime
    std::vector<double>  Muon_timeAtIpInOut,Muon_timeAtIpInOutErr;
    
    //Muon inner + outer track
    std::vector<double>  Muon_GLnormChi2, Muon_GLhitPattern_numberOfValidMuonHits,  Muon_trackerLayersWithMeasurement,  Muon_Numberofvalidpixelhits,  Muon_outerTrack_p,  Muon_outerTrack_eta, Muon_outerTrack_phi,  Muon_outerTrack_normalizedChi2,  Muon_outerTrack_muonStationsWithValidHits,  Muon_innerTrack_p,  Muon_innerTrack_eta,  Muon_innerTrack_phi,  Muon_innerTrack_normalizedChi2,  Muon_QInnerOuter;
    
    std::vector<double>   Muon_combinedQuality_updatedSta,  Muon_combinedQuality_trkKink,  Muon_combinedQuality_glbKink,  Muon_combinedQuality_trkRelChi2,  Muon_combinedQuality_staRelChi2,  Muon_combinedQuality_chi2LocalPosition,  Muon_combinedQuality_chi2LocalMomentum,  Muon_combinedQuality_localDistance,  Muon_combinedQuality_globalDeltaEtaPhi,  Muon_combinedQuality_tightMatch,  Muon_combinedQuality_glbTrackProbability,  Muon_calEnergy_em,  Muon_calEnergy_emS9,  Muon_calEnergy_emS25,  Muon_calEnergy_had,  Muon_calEnergy_hadS9,  Muon_segmentCompatibility,  Muon_caloCompatibility,  Muon_ptErrOverPt, Muon_BestTrackPt,  Muon_BestTrackPtErr, Muon_BestTrackEta,  Muon_BestTrackEtaErr,  Muon_BestTrackPhi,  Muon_BestTrackPhiErr;

    std::vector<int>  Muon_simPdgId, Muon_simMotherPdgId, Muon_simFlavour,  Muon_simType, Muon_simBX, Muon_simTpEvent, Muon_simMatchQuality;
    std::vector<double>  Mu1_Pt,  Mu1_Eta,  Mu1_Phi,  Mu2_Pt,  Mu2_Eta,  Mu2_Phi,  Mu3_Pt,  Mu3_Eta,  Mu3_Phi, GenMatchMu1_SimPt, GenMatchMu2_SimPt, GenMatchMu3_SimPt,GenMatchMu1_SimEta, GenMatchMu2_SimEta, GenMatchMu3_SimEta, GenMatchMu1_SimPhi, GenMatchMu2_SimPhi, GenMatchMu3_SimPhi,  GenMatchMu1_Pt,  GenMatchMu2_Pt,  GenMatchMu3_Pt,  GenMatchMu1_Eta,  GenMatchMu2_Eta,  GenMatchMu3_Eta,  GenMatchMu1_Phi,  GenMatchMu2_Phi,  GenMatchMu3_Phi;
    std::vector<double> RefTrack1_Pt, RefTrack1_Eta, RefTrack1_Phi, RefTrack1_TripletIndex;
    std::vector<double> RefTrack2_Pt, RefTrack2_Eta, RefTrack2_Phi, RefTrack2_TripletIndex;
    std::vector<double> RefTrack3_Pt, RefTrack3_Eta, RefTrack3_Phi, RefTrack3_TripletIndex;

    std::vector<double> RefittedSV_Chi2, RefittedSV_nDOF, RefittedSV_Mass;

    std::vector<double> IsoTrackMu1_Pt, IsoTrackMu1_Eta, IsoTrackMu1_Phi;
    std::vector<double> IsoTrackMu2_Pt, IsoTrackMu2_Eta, IsoTrackMu2_Phi;
    std::vector<double> IsoTrackMu3_Pt, IsoTrackMu3_Eta, IsoTrackMu3_Phi;

    std::vector<float> Mu1_dRtriggerMatch, Mu2_dRtriggerMatch, Mu3_dRtriggerMatch;
    std::vector<float> Mu1_dRtriggerMatch_Mu7, Mu2_dRtriggerMatch_Mu7, Mu3_dRtriggerMatch_Mu7;
    std::vector<float> Mu1_dRtriggerMatch_Mu8, Mu2_dRtriggerMatch_Mu8, Mu3_dRtriggerMatch_Mu8;
    std::vector<float> Mu1_dRtriggerMatch_Mu8_IP5, Mu1_dRtriggerMatch_Mu8_IP6, Mu1_dRtriggerMatch_Mu9_IP0, Mu1_dRtriggerMatch_Mu9_IP3, Mu1_dRtriggerMatch_Mu9_IP4, Mu1_dRtriggerMatch_Mu9_IP5, Mu1_dRtriggerMatch_Mu9_IP6,Mu1_dRtriggerMatch_Mu12_IP6,Mu1_dRtriggerMatch_2017, Mu2_dRtriggerMatch_2017, Mu3_dRtriggerMatch_2017;

    std::vector<double> Muon_emEt03, Muon_hadEt03, Muon_nJets03, Muon_nTracks03, Muon_sumPt03, Muon_emEt05,    Muon_hadEt05, Muon_nJets05, Muon_nTracks05, Muon_sumPt05,Muon_hadVetoEt03,Muon_emVetoEt03,    Muon_trackerVetoPt03,    Muon_hadVetoEt05,    Muon_emVetoEt05,    Muon_trackerVetoPt05;
    
    std::vector<double>     Triplet_mindca_iso, Triplet_relativeiso, Triplet_relativeiso2;
 
    std::vector<int>  Mu1_TripletIndex,  Mu2_TripletIndex,  Mu3_TripletIndex;
    std::vector<int>  Mu1_NTracks03iso,  Mu2_NTracks03iso,  Mu3_NTracks03iso;
    
    int TripletCollectionSize, PVCollection_Size, MuonCollectionSize;
    int PhotonCollectionSize;
    std::vector<double>  TripletVtx_x,  TripletVtx_y,  TripletVtx_z,  TripletVtx_Chi2,  TripletVtx_NDOF,  Triplet_Mass,  Triplet_Pt,  Triplet_Eta,  Triplet_Phi, Triplet_Charge;
    std::vector<std::vector<double>>  TripletVtx_cov;
    
    std::vector<double> dxy_mu1, dxy_mu2, dxy_mu3, dxyErr_mu1, dxyErr_mu2, dxyErr_mu3; 
    
    std::vector<double>  RefittedPV_x;
    std::vector<double>  RefittedPV_y;
    std::vector<double>  RefittedPV_z;
    std::vector<std::vector<double>>  RefittedPV_cov;
    std::vector<double>  RefittedPV_NTracks;
    std::vector<int>     RefittedPV_isValid;
    std::vector<double>  RefittedPV_Chi2, RefittedPV_nDOF;
    std::vector<double>  PV_bis_Chi2, PV_bis_nDOF;
    
    //RefittedPV_Chi2.push_back(PVertex.);
    
    std::vector<double>  FlightDistPVSV;
    std::vector<double>  FlightDistPVSV_Err;
    std::vector<double>  FlightDistPVSV_Significance;
    std::vector<double>  FlightDistPVSV_chi2;
    
    std::vector<double> PV_x,  PV_y,  PV_z,  PV_NTracks;
    std::vector<double> BS_x,  BS_y,  BS_z;
    std::vector<double> Vtx12_x, Vtx23_x, Vtx13_x, Vtx12_y, Vtx23_y, Vtx13_y, Vtx12_z, Vtx23_z, Vtx13_z, Vtx12_Chi2, Vtx23_Chi2, Vtx13_Chi2, Vtx12_nDOF, Vtx23_nDOF, Vtx13_nDOF;
    std::vector<int> NGoodTriplets;
    uint  evt, run, lumi, puN;
    std::vector<string>  Trigger_l1name;
    std::vector<int> Trigger_l1Initialdecision, Trigger_l1Finaldecision;
    std::vector<double> Trigger_l1prescale;

    std::vector<string>  Trigger_hltname;
    std::vector<int> Trigger_hltdecision;

    std::vector<double> MuonPt_HLT,  MuonEta_HLT,  MuonPhi_HLT;
    std::vector<double> MuonPt_HLT2017, MuonEta_HLT2017, MuonPhi_HLT2017, MuonPt_HLT_BPMu7, MuonEta_HLT_BPMu7, MuonPhi_HLT_BPMu7, MuonPt_HLT_BPMu8, MuonEta_HLT_BPMu8, MuonPhi_HLT_BPMu8, MuonPt_HLT_BPMu8_IP6,  MuonEta_HLT_BPMu8_IP6, MuonPhi_HLT_BPMu8_IP6, MuonPt_HLT_BPMu8_IP5, MuonEta_HLT_BPMu8_IP5, MuonPhi_HLT_BPMu8_IP5,   MuonPt_HLT_BPMu9_IP0, MuonEta_HLT_BPMu9_IP0, MuonPhi_HLT_BPMu9_IP0, MuonPt_HLT_BPMu9_IP3, MuonEta_HLT_BPMu9_IP3, MuonPhi_HLT_BPMu9_IP3, MuonPt_HLT_BPMu9_IP4,MuonEta_HLT_BPMu9_IP4,MuonPhi_HLT_BPMu9_IP4,MuonPt_HLT_BPMu9_IP5, MuonEta_HLT_BPMu9_IP5,MuonPhi_HLT_BPMu9_IP5,MuonPt_HLT_BPMu9_IP6,MuonEta_HLT_BPMu9_IP6,MuonPhi_HLT_BPMu9_IP6,MuonPt_HLT_BPMu12_IP6,MuonEta_HLT_BPMu12_IP6,MuonPhi_HLT_BPMu12_IP6;
    std::vector<double> MuonPt_HLT_DiMu_Incl, MuonEta_HLT_DiMu_Incl, MuonPhi_HLT_DiMu_Incl;
    std::vector<double> MuonPt_HLT_DiMu_Incl_displ, MuonEta_HLT_DiMu_Incl_displ, MuonPhi_HLT_DiMu_Incl_displ;

    std::vector<double>  Muon_innerTrack_highPurity,  Muon_innerTrack_ValidFraction, Muon_Numberofvalidtrackerhits, Muon_validMuonHitComb, Muon_IP2D_BS,  Muon_IP3D_BS,  Muon_IP2D_PV,  Muon_IP3D_PV, Muon_SoftMVA_Val;
    std::vector<double>  DistXY_PVSV,  DistXY_significance_PVSV;
    std::vector<double>  Triplet_IsoMu1, Triplet_IsoMu2, Triplet_IsoMu3;
    std::vector<double>  FlightDistBS_SV,  FlightDistBS_SV_Err,  FlightDistBS_SV_Significance;

    std::vector<double>  Mu1_IsGlobal, Mu2_IsGlobal, Mu3_IsGlobal, Mu1_IsPF, Mu2_IsPF, Mu3_IsPF;
    
    std::vector<double> L1Muon_Pt, L1Muon_Eta, L1Muon_Phi, L1Muon_EtaAtVtx, L1Muon_PhiAtVtx, L1Muon_BX, L1Muon_Quality, L1Muon_Charge, L1Muon_ChargeValid, L1Muon_TfMuonIndex, L1Muon_dPhi, L1Muon_dEta, L1Muon_rank, L1Muon_isoSum;

    //SyncTree
    /*  
    TTree*      SyncTree_;
    std::vector<float>  allmuons_pt, leadmuon_pt, leadmuon_phi, leadmuon_eta;
    std::vector<float>  alltracks_pt, leadtrack_pt,  leadtrack_eta,  leadtrack_phi;
    uint nprimevtxs, nmuons, evt, run, lumi;
    */
};
    
    
    
MiniAnaTau3Mu::MiniAnaTau3Mu(const edm::ParameterSet& iConfig){
    edm::InputTag algInputTag_;
    isMc = iConfig.getUntrackedParameter<bool>("isMcLabel");
    isAna = iConfig.getUntrackedParameter<bool>("isAnaLabel");
    is2016 = iConfig.getUntrackedParameter<bool>("is2016Label");
    is2017 = iConfig.getUntrackedParameter<bool>("is2017Label");
    is2018 = iConfig.getUntrackedParameter<bool>("is2018Label");
    isBParking = iConfig.getUntrackedParameter<bool>("isBParkingLabel");
    muons_ = consumes<edm::View<pat::Muon> >  (iConfig.getParameter<edm::InputTag>("muonLabel"));
    photons_ = consumes<edm::View<pat::Photon> >  (iConfig.getParameter<edm::InputTag>("photonLabel"));
    vertex_ = consumes<edm::View<reco::Vertex> > (iConfig.getParameter<edm::InputTag>("VertexLabel"));
    trackToken_ = consumes<edm::View<reco::Track> > (edm::InputTag("generalTracks"));
    //srcCands = consumes<std::vector<pat::PackedCandidate> >(iConfig.getParameter<edm::InputTag>("srcCands")));
    srcCands_ = consumes<std::vector<pat::PackedCandidate> >(edm::InputTag("packedPFCandidates"));
    genParticles_ = consumes<edm::View<reco::GenParticle>  > (iConfig.getParameter<edm::InputTag>("genParticleLabel"));
    Cand3Mu_ = consumes<edm::View<reco::CompositeCandidate> > (iConfig.getParameter<edm::InputTag>("Cand3MuLabel"));
    puToken_ =   consumes<std::vector<PileupSummaryInfo> >(iConfig.getParameter<edm::InputTag>("pileupSummary"));
    triggerToken_ = consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("triggerResults"));
    //trigeventToken_ = consumes<trigger::TriggerEvent>(iConfig.getParameter<edm::InputTag>("triggerSummary"));
    triggerObjects_ = consumes<std::vector<pat::TriggerObjectStandAlone> >(iConfig.getParameter<edm::InputTag>("objects"));
    algToken_ = consumes<BXVector<GlobalAlgBlk>>(iConfig.getParameter<edm::InputTag>("AlgInputTag"));
    algTok_ = consumes<BXVector<GlobalAlgBlk>>(iConfig.getParameter<edm::InputTag>("algInputTag"));
    gtUtil_ = new l1t::L1TGlobalUtil(iConfig, consumesCollector(), *this, algTag_, extTag_, l1t::UseEventSetupIn::Event);
    //
    //m_l1tMenuToken = esConsumes<edm::Transition::BeginRun>();
    //m_l1t_results = consumes<GlobalAlgBlkBxCollection>(iConfig.getUntrackedParameter<edm::InputTag>("l1tResults"));
    token_BeamSpot = consumes<reco::BeamSpot>(edm::InputTag("offlineBeamSpot"));
    l1muonsToken_ = consumes<BXVector<l1t::Muon>>(edm::InputTag("gmtStage2Digis", "Muon"  , "RECO"));
    theTransientTrackBuilder_ = esConsumes<TransientTrackBuilder, TransientTrackRecord>(edm::ESInputTag("", "TransientTrackBuilder"));
    //_hltInputTag(iConfig.getParameter<edm::InputTag>("hltInputTag")),
    //tauToken_(consumes(iConfig.getParameter("taus"))),
    //metToken_(consumes(iConfig.getParameter("mets")))
    //tree_(0);
    //MuonPt(0);
}

MiniAnaTau3Mu::~MiniAnaTau3Mu(){
    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)
}
    
    
float MiniAnaTau3Mu::dR(float eta1, float eta2, float phi1, float phi2){
    float dphi=(phi1-phi2);
    float deta=(eta1-eta2);
    float deltaR= TMath::Sqrt(dphi*dphi + deta*deta);
    return deltaR;
}

float MiniAnaTau3Mu::dRtriggerMatch(pat::Muon m, vector<pat::TriggerObjectStandAlone> triggerObjects) {
//float MiniAnaTau3Mu::dRtriggerMatch(pat::Muon m, trigger::TriggerObjectCollection triggerObjects) {
    float dRmin = 1.;
    for (unsigned int i = 0 ; i < triggerObjects.size() ; i++) {
        float deltaR = sqrt( reco::deltaR2(triggerObjects[i].eta(), triggerObjects[i].phi(), m.eta(), m.phi()));
        //float deltaR = sqrt( pow(triggerObjects[i].eta() - m.eta(), 2) + pow(acos(cos(triggerObjects[i].phi() - m.phi())), 2));
        if (deltaR < dRmin) dRmin = deltaR;
    }
    return dRmin;
}

    
bool isGoodTrack(const reco::Track &track) {
    if(track.pt()>1){
        if(std::fabs(track.eta())<2.4){
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
    
    
void MiniAnaTau3Mu::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup, const edm::Event& iEvent) {
  /*
  edm::Handle<edm::TriggerResults> trigResults; //our trigger result object
    edm::InputTag trigResultsTag("TriggerResults"," ","HLT"); //make sure have correct process on MC
    iEvent.getByLabel(trigResultsTag,trigResults);
   
    bool changed = true;
    if (hltConfig.init(iRun, iSetup, trigResultsTag.process(), changed)) {
        // if init returns TRUE, initialisation has succeeded!
        std::cout << "HLT config with process name "<< trigResultsTag.process() << " successfully extracted"<<std::endl;
    }
    else {
        // if init returns FALSE, initialisation has NOT succeeded, which indicates a problem
        // with the file and/or code and needs to be investigated!
        std::cout << "Error! HLT config extraction with process name " << trigResultsTag.process() << " failed"<<std::endl;
        // In this case, all access methods will return empty values!
    }
  */
}

    
void MiniAnaTau3Mu::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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
    
    edm::Handle<edm::View<reco::CompositeCandidate> > Cand3Mu;
    iEvent.getByToken(Cand3Mu_, Cand3Mu);
    
    edm::Handle< edm::View<reco::GenParticle> > genParticles;
    iEvent.getByToken(genParticles_, genParticles);
    
    edm::Handle<edm::View<reco::Track> > trackCollection;
    iEvent.getByToken(trackToken_, trackCollection);
    
    Handle<TriggerResults> triggerResults;
    iEvent.getByToken(triggerToken_, triggerResults);

    //	Handle<trigger::TriggerEvent> triggerSummary;
    //	iEvent.getByToken(trigeventToken_, triggerSummary);

    edm::Handle< edm::View<pat::Photon> > photons;
    iEvent.getByToken(photons_, photons);

    Handle<std::vector<pat::TriggerObjectStandAlone> > triggerObjects;
    iEvent.getByToken(triggerObjects_, triggerObjects);

    Handle<BXVector<GlobalAlgBlk>> alg;
    iEvent.getByToken(algToken_,alg);
    
    Handle<BXVector<l1t::Muon> > gmuons;
    iEvent.getByToken(l1muonsToken_, gmuons);
    
    //edm::ESHandle<L1TUtmTriggerMenu> hmenu = iSetup.getHandle(m_l1TriggerMenuToken);
    //edm::ESHandle<L1TUtmTriggerMenu> hmenu = iSetup.getHandle(m_l1tMenuToken);
    //Handle<TransientTrackBuilder> theTransientTrackBuilder;
    //edm::ESHandle<TransientTrackBuilder> theTransientTrackBuilder;
    //edm::ESGetToken<TransientTrackBuilder, TransientTrackRecord> theTTBuilderToken_;
    edm::ESHandle<TransientTrackBuilder> theTransientTrackBuilder = iSetup.getHandle(theTransientTrackBuilder_); 
    //iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theTransientTrackBuilder);
    //theTransientTrackBuilder_ = theTransientTrackBuilder.product();

    edm::Handle<std::vector<pat::PackedCandidate> > PFCands;
    iEvent.getByToken(srcCands_,PFCands);

    reco::BeamSpot beamSpot;
    edm::Handle<reco::BeamSpot> beamSpotHandle;
    iEvent.getByToken(token_BeamSpot, beamSpotHandle);
    const reco::BeamSpot& beamspot = *beamSpotHandle.product();
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
                //L1Muon_dPhi.push_back(itr->dPhi());
                //L1Muon_dEta.push_back(itr->dEta());
                L1Muon_rank.push_back(itr->hwRank());
                L1Muon_isoSum.push_back(itr->hwIso());
                cout << "Muon : "
                 << " BX=" << ibx << " ipt=" << itr->pt() << " eta=" << itr->eta()
                 << " phi=" << itr->phi() << " hweta="<< itr->hwEta()<<" charge="<<itr->charge()<< "rank="<<itr->hwRank()<<std::endl;
                //  int hwEtaAtVtx = 0,
                //  int hwPhiAtVtx = 0,
                //  double etaAtVtx = 0.,
                //  double phiAtVtx = 0.);
              }
            }

//edm::Handle<SimTrackContainer> simTracks;
//iEvent.getByLabel("g4SimHits",simTracks);

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

vector<pat::TriggerObjectStandAlone> TriggerObj_DsTau3Mu,  TriggerObj_DsTau3Mu2017, TriggerObj_Dimuon, TriggerObj_DimuonIncl, TriggerObj_DimuonIncl_displ;
vector<pat::TriggerObjectStandAlone> MuonsObjects_BPMu7, MuonsObjects_BPMu12_IP6, MuonsObjects_BPMu8, MuonsObjects_BPMu8_IP6,MuonsObjects_BPMu8_IP5,MuonsObjects_BPMu9_IP0, MuonsObjects_BPMu9_IP3, MuonsObjects_BPMu9_IP4, MuonsObjects_BPMu9_IP5, MuonsObjects_BPMu9_IP6;

for (pat::TriggerObjectStandAlone obj : *triggerObjects) { // note: not "const &" since we want to call unpackPathNames
    obj.unpackPathNames(triggerNames);

    for (unsigned h = 0; h < obj.filterLabels().size(); ++h) {
        if(obj.filterLabels()[h]=="hltdstau3muDisplaced3muFltr"){
            TriggerObj_DsTau3Mu.push_back(obj);
        }
        if(obj.filterLabels()[h]=="hltTau3muTkVertexFilter"){
            TriggerObj_DsTau3Mu2017.push_back(obj);
        }
        
        // Inclusive dimu triggers
        if(obj.filterLabels()[h]=="hltDisplacedmumuFilterDoubleMu43LowMass"){
            TriggerObj_DimuonIncl.push_back(obj);
        }
        if(obj.filterLabels()[h]=="hltDisplacedmumuFilterDoubleMu4LowMassDisplaced"){
            TriggerObj_DimuonIncl_displ.push_back(obj);
        }
        
        if(isBParking){
            if(obj.filterLabels()[h]=="hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered8Q"){
                MuonsObjects_BPMu8.push_back(obj);
            }
            if(obj.filterLabels()[h]=="hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered7IP4Q"){
                MuonsObjects_BPMu7.push_back(obj);
            }
            if(obj.filterLabels()[h]=="hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered8IP6Q"){
                MuonsObjects_BPMu8_IP6.push_back(obj);
            }
            if(obj.filterLabels()[h]=="hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered8IP5Q"){
                MuonsObjects_BPMu8_IP5.push_back(obj);
            }
            if(obj.filterLabels()[h]=="hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered9IP0Q"){
                MuonsObjects_BPMu9_IP0.push_back(obj);
            }
            if(obj.filterLabels()[h]=="hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered9IP3Q"){
                MuonsObjects_BPMu9_IP3.push_back(obj);
            }
            if(obj.filterLabels()[h]=="hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered9IP4Q"){
                MuonsObjects_BPMu9_IP4.push_back(obj);
            }
            if(obj.filterLabels()[h]=="hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered9IP5Q"){
                MuonsObjects_BPMu9_IP5.push_back(obj);
            }
            if(obj.filterLabels()[h]=="hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered9Q"){
                MuonsObjects_BPMu9_IP6.push_back(obj);
            }
            if(obj.filterLabels()[h]=="hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered12Q"){
                MuonsObjects_BPMu12_IP6.push_back(obj);
            }
        }//isBParking
    }//loop on filterLabels
}

for(uint t=0; t<TriggerObj_DsTau3Mu.size();t++){
    MuonPt_HLT.push_back(TriggerObj_DsTau3Mu.at(t).pt());
    MuonEta_HLT.push_back(TriggerObj_DsTau3Mu.at(t).eta());
    MuonPhi_HLT.push_back(TriggerObj_DsTau3Mu.at(t).phi());
}

for(uint t=0; t<TriggerObj_DsTau3Mu2017.size();t++){
    MuonPt_HLT2017.push_back(TriggerObj_DsTau3Mu2017.at(t).pt());
    MuonEta_HLT2017.push_back(TriggerObj_DsTau3Mu2017.at(t).eta());
    MuonPhi_HLT2017.push_back(TriggerObj_DsTau3Mu2017.at(t).phi());
}
    
    // Trigger matching inclusive dimu triggers
    for(uint t=0; t<TriggerObj_DimuonIncl.size();t++){
        MuonPt_HLT_DiMu_Incl.push_back(TriggerObj_DimuonIncl.at(t).pt());
        MuonEta_HLT_DiMu_Incl.push_back(TriggerObj_DimuonIncl.at(t).eta());
        MuonPhi_HLT_DiMu_Incl.push_back(TriggerObj_DimuonIncl.at(t).phi());
    }
    for(uint t=0; t<TriggerObj_DimuonIncl_displ.size();t++){
        MuonPt_HLT_DiMu_Incl_displ.push_back(TriggerObj_DimuonIncl_displ.at(t).pt());
        MuonEta_HLT_DiMu_Incl_displ.push_back(TriggerObj_DimuonIncl_displ.at(t).eta());
        MuonPhi_HLT_DiMu_Incl_displ.push_back(TriggerObj_DimuonIncl_displ.at(t).phi());
    }
    //

if(isBParking){
    for(uint t=0; t<MuonsObjects_BPMu8.size();t++){
        MuonPt_HLT_BPMu8.push_back(MuonsObjects_BPMu8.at(t).pt());
        MuonEta_HLT_BPMu8.push_back(MuonsObjects_BPMu8.at(t).eta());
        MuonPhi_HLT_BPMu8.push_back(MuonsObjects_BPMu8.at(t).phi());
    }
    for(uint t=0; t<MuonsObjects_BPMu7.size();t++){
        MuonPt_HLT_BPMu7.push_back(MuonsObjects_BPMu7.at(t).pt());
        MuonEta_HLT_BPMu7.push_back(MuonsObjects_BPMu7.at(t).eta());
        MuonPhi_HLT_BPMu7.push_back(MuonsObjects_BPMu7.at(t).phi());
    }
    for(uint t=0; t<MuonsObjects_BPMu8_IP6.size();t++){
        MuonPt_HLT_BPMu8_IP6.push_back(MuonsObjects_BPMu8_IP6.at(t).pt());
        MuonEta_HLT_BPMu8_IP6.push_back(MuonsObjects_BPMu8_IP6.at(t).eta());
        MuonPhi_HLT_BPMu8_IP6.push_back(MuonsObjects_BPMu8_IP6.at(t).phi());
    }
    for(uint t=0; t<MuonsObjects_BPMu8_IP5.size();t++){
        MuonPt_HLT_BPMu8_IP5.push_back(MuonsObjects_BPMu8_IP5.at(t).pt());
        MuonEta_HLT_BPMu8_IP5.push_back(MuonsObjects_BPMu8_IP5.at(t).eta());
        MuonPhi_HLT_BPMu8_IP5.push_back(MuonsObjects_BPMu8_IP5.at(t).phi());
    }
    for(uint t=0; t<MuonsObjects_BPMu9_IP0.size();t++){
        MuonPt_HLT_BPMu9_IP0.push_back(MuonsObjects_BPMu9_IP0.at(t).pt());
        MuonEta_HLT_BPMu9_IP0.push_back(MuonsObjects_BPMu9_IP0.at(t).eta());
        MuonPhi_HLT_BPMu9_IP0.push_back(MuonsObjects_BPMu9_IP0.at(t).phi());
    }
    for(uint t=0; t< MuonsObjects_BPMu9_IP3.size();t++){
        MuonPt_HLT_BPMu9_IP3.push_back(MuonsObjects_BPMu9_IP3.at(t).pt());
        MuonEta_HLT_BPMu9_IP3.push_back(MuonsObjects_BPMu9_IP3.at(t).eta());
        MuonPhi_HLT_BPMu9_IP3.push_back(MuonsObjects_BPMu9_IP3.at(t).phi());
    }
    for(uint t=0; t< MuonsObjects_BPMu9_IP4.size();t++){
        MuonPt_HLT_BPMu9_IP4.push_back(MuonsObjects_BPMu9_IP4.at(t).pt());
        MuonEta_HLT_BPMu9_IP4.push_back(MuonsObjects_BPMu9_IP4.at(t).eta());
        MuonPhi_HLT_BPMu9_IP4.push_back(MuonsObjects_BPMu9_IP4.at(t).phi());
    }
    for(uint t=0; t< MuonsObjects_BPMu9_IP5.size();t++){
        MuonPt_HLT_BPMu9_IP5.push_back(MuonsObjects_BPMu9_IP5.at(t).pt());
        MuonEta_HLT_BPMu9_IP5.push_back(MuonsObjects_BPMu9_IP5.at(t).eta());
        MuonPhi_HLT_BPMu9_IP5.push_back(MuonsObjects_BPMu9_IP5.at(t).phi());
    }
    for(uint t=0; t< MuonsObjects_BPMu9_IP6.size();t++){
        MuonPt_HLT_BPMu9_IP6.push_back(MuonsObjects_BPMu9_IP6.at(t).pt());
        MuonEta_HLT_BPMu9_IP6.push_back(MuonsObjects_BPMu9_IP6.at(t).eta());
        MuonPhi_HLT_BPMu9_IP6.push_back(MuonsObjects_BPMu9_IP6.at(t).phi());
    }
    for(uint t=0; t< MuonsObjects_BPMu12_IP6.size();t++){
        MuonPt_HLT_BPMu12_IP6.push_back(MuonsObjects_BPMu12_IP6.at(t).pt());
        MuonEta_HLT_BPMu12_IP6.push_back(MuonsObjects_BPMu12_IP6.at(t).eta());
        MuonPhi_HLT_BPMu12_IP6.push_back(MuonsObjects_BPMu12_IP6.at(t).phi());
    }
}//isBParking

///////////////Fill Genparticles ///////////////
if(isMc){
    uint j=0;
    uint ngenP=genParticles->size();
    std::vector<int> genPidx;
    
    for(edm::View<reco::GenParticle>::const_iterator gp=genParticles->begin(); gp!=genParticles->end(), j<ngenP; ++gp , ++j){
            
        if( fabs(gp->pdgId())==13  || fabs(gp->pdgId())==15  || fabs(gp->pdgId())==11 || fabs(gp->pdgId())==211 || 
            fabs(gp->pdgId())==321 ||  fabs(gp->pdgId())==12 || fabs(gp->pdgId())==14 || fabs(gp->pdgId())==16  ||
            fabs(gp->pdgId())==431 || fabs(gp->pdgId())==511 || fabs(gp->pdgId())==521) {

            GenParticle_PdgId.push_back(gp->pdgId());
            GenParticle_Pt.push_back(gp->pt());
            GenParticle_Eta.push_back(gp->eta());
            GenParticle_Phi.push_back(gp->phi());
            GenParticle_vx.push_back(gp->vx());
            GenParticle_vy.push_back(gp->vy());
            GenParticle_vz.push_back(gp->vz());
            if (gp->numberOfMothers()) {
                GenParticle_MotherPdgId.push_back(gp->mother(0)->pdgId());
                if(gp->mother(0)->mother(0)) {
                    GenParticle_GrandMotherPdgId.push_back(gp->mother(0)->mother(0)->pdgId());
                }else{
                    GenParticle_GrandMotherPdgId.push_back(-99);
                }
            }else{
                GenParticle_MotherPdgId.push_back(-99);
            }
        } //pdgId
    }
} //End GenParticles   


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

    int key = cand->vertexRef().key();
    int quality = cand->pvAssociationQuality();
    if(cand->fromPV(cand->vertexRef().key())<2) continue;
    if(cand->fromPV(cand->vertexRef().key())==2 && quality!=pat::PackedCandidate::UsedInFitLoose) continue;
    
    if ( !(cand->bestTrack()) ) continue;
    //if (quality != pat::PackedCandidate::UsedInFitTight)  continue;
    //if (quality != pat::PackedCandidate::UsedInFitLoose)  continue;
    //cout<<kk<<" vtx ref key="<<key<<" cand pt="<<cand->pt()<<" vtx x="<<cand->vertexRef()->x()<<endl;
    VtxIdV.push_back(key);
    SelectedCandIdx.push_back(kk);
    MyPFCands.push_back(*cand);
} // loop PFCandidates

/*
for(uint VtxIt =0;VtxIt<vertices->size();VtxIt++ ){
    cout<<"Vtx id="<<VtxIt<<" x="<<(*vertices)[VtxIt].x()<<endl;
    }
*/

    
//cout<<" vtx id size="<<VtxIdV.size()<<endl;
sort( VtxIdV.begin(), VtxIdV.end() );
VtxIdV.erase( unique(VtxIdV.begin(), VtxIdV.end() ), VtxIdV.end() );

/*
for(uint i=0; i<VtxIdV.size();i++){
    cout<<i<<" vtx id="<<VtxIdV.at(i)<<endl;
    }
*/

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



//Triplets  Loop
vector<int> Mu1C, Mu2C, Mu3C, TauMass;

//cout<<"Number Of Triplets="<<Cand3Mu->size()<<endl;
std::vector<int> NTripl;
if(isAna){
    TripletCollectionSize = Cand3Mu->size() ;
    int TripletIndex =-99; uint trIn=0;
    for(edm::View<reco::CompositeCandidate>::const_iterator TauIt=Cand3Mu->begin(); TauIt!=Cand3Mu->end(), trIn<Cand3Mu->size(); ++TauIt, ++trIn){
        //cout<<"----------------"<<trIn<<"----------------"<<endl;
        const Candidate * c1 = TauIt->daughter(0)->masterClone().get();
        const pat::Muon *mu1 = dynamic_cast<const pat::Muon *>(c1);
      
        const Candidate * c2 = TauIt->daughter(1)->masterClone().get();
        const pat::Muon *mu2 = dynamic_cast<const pat::Muon *>(c2);
        
        const Candidate * c3 = TauIt->daughter(2)->masterClone().get();
        const pat::Muon *mu3 = dynamic_cast<const pat::Muon *>(c3);

        //cout<<"mu1 pt="<<mu1->pt()<<" m2="<<mu2->pt()<<" m3="<<mu3->pt()<<endl;
        TrackRef trk1, trk2, trk3;
        trk1 = mu1->innerTrack();
        trk2 = mu2->innerTrack();
        trk3 = mu3->innerTrack();
        //cout<<" trk1 id="<<trk1.id()<<" tr2:"<<trk2.id()<<" trk3="<<trk3.id()<<endl;
        //const reco::TransientTrack transientTrack1=theTransientTrackBuilder_->build( trk1 );
        //const reco::TransientTrack transientTrack2=theTransientTrackBuilder_->build( trk2 );
        //const reco::TransientTrack transientTrack3=theTransientTrackBuilder_->build( trk3 );
        const reco::TransientTrack transientTrack1=theTransientTrackBuilder->build( trk1 );
        const reco::TransientTrack transientTrack2=theTransientTrackBuilder->build( trk2 );
        const reco::TransientTrack transientTrack3=theTransientTrackBuilder->build( trk3 );
        reco::Track Track1 =transientTrack1.track();
        reco::Track Track2 =transientTrack2.track();
        reco::Track Track3 =transientTrack3.track();
        reco::Track* TrackRef1=&Track1;
        reco::Track* TrackRef2=&Track2;
        reco::Track* TrackRef3=&Track3;
        vector<reco::Track*> SVTrackRef;
        SVTrackRef.push_back(TrackRef1);
        SVTrackRef.push_back(TrackRef2);
        SVTrackRef.push_back(TrackRef3); 
        //cout<<" track ref vector= "<<SVTrackRef.size()<<endl;

        reco::Vertex TripletVtx = reco::Vertex(TauIt->vertex(), TauIt->vertexCovariance(), TauIt->vertexChi2(), TauIt->vertexNdof(), TauIt->numberOfDaughters() );
        double dphi_pv = -1.0;
        uint primaryvertex_index=0;
        TLorentzVector ThreeCandidate;
        uint selVtxId = 0;
        
        ThreeCandidate.SetPtEtaPhiM(TauIt->pt(), TauIt->eta(), TauIt->phi(), TauIt->mass());
        //cout<<"Tau pt="<<TauIt->pt()<<" eta="<<TauIt->eta()<<" phi="<<TauIt->phi()<<" mass="<<TauIt->mass()<<endl;

        //loop on privary vertices
        //cout<<"Among "<<VtxIdV.size()<<" vertices we select the closest, i.e. maximum Cosdphi_3D"<<endl;
        if(VtxIdV.size()>0 && vertices->size()>0) {
            for(uint VtxIt=0; VtxIt<vertices->size(); VtxIt++ ){
                //cout << "vertex collection n." << VtxIt << " has size "<< VtxIdV.size() << endl;
                for(uint k=0; k<VtxIdV.size(); k++){
                    //cout << "   VtxIdV.at("<<k<<") = " << VtxIdV.at(k) << endl;
                    if(VtxIdV[k]==VtxIt){
                        //cout<<"     Vtx id="<<VtxIt<<" x="<<(*vertices)[VtxIt].x()<<" y="<<(*vertices)[VtxIt].y()<<" z="<<(*vertices)[VtxIt].z()<<endl;
                        TVector3 Dv3D_reco(TripletVtx.x() - (*vertices)[VtxIt].x(), TripletVtx.y() - (*vertices)[VtxIt].y(), TripletVtx.z() - (*vertices)[VtxIt].z());
                        double Cosdphi_3D = Dv3D_reco.Dot(ThreeCandidate.Vect())/(Dv3D_reco.Mag()*ThreeCandidate.Vect().Mag());
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
                if(PVertex.isValid() && TauIt->vertexChi2() >0 ){
                    NTripl.push_back(1);
                    TripletIndex=trIn;
                    if((mu1->isGlobalMuon()) && (mu1->isPFMuon())) {
                        Mu1C.push_back(1);
                        if((mu2->isGlobalMuon()) && (mu2->isPFMuon())){
                            Mu2C.push_back(1);
                            if((mu3->isGlobalMuon()) && (mu3->isPFMuon())){
                                Mu3C.push_back(1);
                                if( (TauIt->mass()>1.62) &&  (TauIt->mass()<2.0) ){
                                    TauMass.push_back(1);
                                }
                            }
                        }
                    }
                    Mu1_Pt.push_back(mu1->pt());
                    Mu1_Eta.push_back(mu1->eta());
                    Mu1_Phi.push_back(mu1->phi());
                    Mu1_TripletIndex.push_back(TripletIndex);
                      
                    Mu2_Pt.push_back(mu2->pt());
                    Mu2_Eta.push_back(mu2->eta());
                    Mu2_Phi.push_back(mu2->phi());
                    Mu2_TripletIndex.push_back(TripletIndex);
                      
                    Mu3_Pt.push_back(mu3->pt());
                    Mu3_Eta.push_back(mu3->eta());
                    Mu3_Phi.push_back(mu3->phi());
                    Mu3_TripletIndex.push_back(TripletIndex);

                    Mu1_IsGlobal.push_back(mu1->isGlobalMuon());
                    Mu2_IsGlobal.push_back(mu2->isGlobalMuon());
                    Mu3_IsGlobal.push_back(mu3->isGlobalMuon());
                    
                    Mu1_IsPF.push_back(mu1->isPFMuon());
                    Mu2_IsPF.push_back(mu2->isPFMuon());
                    Mu3_IsPF.push_back(mu3->isPFMuon());
                    //cout<<"Reco mu1 pt="<<mu1->pt()<<" mu2 pt="<<mu2->pt()<<" mu3 pt="<<mu3->pt()<<endl;
                    
                    //Refitted vars related to SV
                    std::vector<reco::TransientTrack> Ttracks;
                    Ttracks.push_back(transientTrack1);
                    Ttracks.push_back(transientTrack2);
                    Ttracks.push_back(transientTrack3);
                    KalmanVertexFitter SVfitter (true);
                    TransientVertex SVertex_ref = SVfitter.vertex(Ttracks);
                    vector < TransientTrack > ttrks = SVertex_ref.refittedTracks(); 
                    //cout<<"ttrks.size() :"<<ttrks.size()<<endl;
                        
                    TLorentzVector LVtau;
                    LVtau.SetPxPyPzE(0, 0, 0, 0);

                    if(SVertex_ref.isValid() && SVertex_ref.hasRefittedTracks() && ttrks.size()>2){
                        //cout<<"VALID ref SV chi2="<<SVertex_ref.totalChiSquared()<<" NDF="<<SVertex_ref.degreesOfFreedom()<<endl;
                        reco::Track SVTrack1 =ttrks.at(0).track();
                        reco::Track SVTrack2 =ttrks.at(1).track();
                        reco::Track SVTrack3 =ttrks.at(2).track();

                        TLorentzVector LV1, LV2, LV3;
                        LV1.SetPxPyPzE(SVTrack1.px(), SVTrack1.py(), SVTrack1.pz(), sqrt(pow(SVTrack1.p(), 2.0) + pow(0.10565, 2.0)));
                        LV2.SetPxPyPzE(SVTrack2.px(), SVTrack2.py(), SVTrack2.pz(), sqrt(pow(SVTrack2.p(), 2.0) + pow(0.10565, 2.0)));
                        LV3.SetPxPyPzE(SVTrack3.px(), SVTrack3.py(), SVTrack3.pz(), sqrt(pow(SVTrack3.p(), 2.0) + pow(0.10565, 2.0)));
                        LVtau = LV1 + LV2 + LV3;
                        
                        //cout<<"SVTrack1.pt() "<<SVTrack1.pt()<<" SVTrack1.eta() "<<SVTrack1.eta()<<" SVTrack1.phi() "<<SVTrack1.phi()<<endl;
                        //cout<<"Track1.pt() "<<Track1.pt()<<" Track1.eta() "<<Track1.eta()<<" Track1.phi() "<<Track1.phi()<<endl;
                        //cout<<"SVTrack2.pt() "<<SVTrack2.pt()<<" SVTrack2.eta() "<<SVTrack2.eta()<<" SVTrack2.phi() "<<SVTrack2.phi()<<endl;
                        //cout<<"Track2.pt() "<<Track2.pt()<<" Track2.eta() "<<Track2.eta()<<" Track2.phi() "<<Track2.phi()<<endl;
                        //cout<<"SVTrack3.pt() "<<SVTrack3.pt()<<" SVTrack3.eta() "<<SVTrack3.eta()<<" SVTrack3.phi() "<<SVTrack3.phi()<<endl;
                        //cout<<"Track3.pt() "<<Track3.pt()<<" Track3.eta() "<<Track3.eta()<<" Track3.phi() "<<Track3.phi()<<endl;
                        //cout<<"mu1->pt() "<<mu1->pt()<<" mu2->pt() "<<mu2->pt()<<" mu3->pt() "<<mu3->pt()<<endl;

                        RefTrack1_Pt.push_back(SVTrack1.pt()); RefTrack1_Eta.push_back(SVTrack1.eta()); RefTrack1_Phi.push_back(SVTrack1.phi()); RefTrack1_TripletIndex.push_back(TripletIndex);
                        RefTrack2_Pt.push_back(SVTrack2.pt()); RefTrack2_Eta.push_back(SVTrack2.eta()); RefTrack2_Phi.push_back(SVTrack2.phi()); RefTrack2_TripletIndex.push_back(TripletIndex);
                        RefTrack3_Pt.push_back(SVTrack3.pt()); RefTrack3_Eta.push_back(SVTrack3.eta()); RefTrack3_Phi.push_back(SVTrack3.phi()); RefTrack3_TripletIndex.push_back(TripletIndex);

                        RefittedSV_Chi2.push_back(SVertex_ref.totalChiSquared());
                        RefittedSV_nDOF.push_back(SVertex_ref.degreesOfFreedom());
                        RefittedSV_Mass.push_back(LVtau.M());
                        cout<<"Bebug mass LVtau.M()="<<LVtau.M()<<endl;
                    } else {
                        RefTrack1_Pt.push_back(-99); RefTrack1_Eta.push_back(-99); RefTrack1_Phi.push_back(-99); RefTrack1_TripletIndex.push_back(TripletIndex);
                        RefTrack2_Pt.push_back(-99); RefTrack2_Eta.push_back(-99); RefTrack2_Phi.push_back(-99); RefTrack2_TripletIndex.push_back(TripletIndex);
                        RefTrack3_Pt.push_back(-99); RefTrack3_Eta.push_back(-99); RefTrack3_Phi.push_back(-99); RefTrack3_TripletIndex.push_back(TripletIndex);

                        RefittedSV_Chi2.push_back(-99);
                        RefittedSV_nDOF.push_back(-99);
                        RefittedSV_Mass.push_back(-99);
                    }
                    ///////////////Check Trigger Matching///////////////
                    float dR1 = 999., dR2 = 999., dR3 = 999.;
                    float dR1_2017 = 999., dR2_2017 = 999., dR3_2017 = 999.;
                    float dR1_Mu7=999.,dR2_Mu7 = 999., dR3_Mu7 = 999.;
                    float dR1_Mu8=999.,dR2_Mu8 = 999., dR3_Mu8 = 999.;
                    float dR1_Mu8_IP6=999., dR1_Mu12_IP6=999, dR1_Mu8_IP5=999.;
                    float dR1_Mu9_IP0=999., dR1_Mu9_IP3=999.,  dR1_Mu9_IP4=999., dR1_Mu9_IP5=999., dR1_Mu9_IP6=999.;

                    dR1_2017 = MiniAnaTau3Mu::dRtriggerMatch(*mu1, TriggerObj_DsTau3Mu2017);
                    dR2_2017 = MiniAnaTau3Mu::dRtriggerMatch(*mu2, TriggerObj_DsTau3Mu2017);
                    dR3_2017 = MiniAnaTau3Mu::dRtriggerMatch(*mu3, TriggerObj_DsTau3Mu2017);
                    Mu1_dRtriggerMatch_2017.push_back(dR1_2017);
                    Mu2_dRtriggerMatch_2017.push_back(dR2_2017);
                    Mu3_dRtriggerMatch_2017.push_back(dR3_2017);

                    dR1 = MiniAnaTau3Mu::dRtriggerMatch(*mu1, TriggerObj_DsTau3Mu);
                    dR2 = MiniAnaTau3Mu::dRtriggerMatch(*mu2, TriggerObj_DsTau3Mu);
                    dR3 = MiniAnaTau3Mu::dRtriggerMatch(*mu3, TriggerObj_DsTau3Mu);
                    //cout<<"Trigger Matching: dR1="<<dR1<<" dR2="<<dR2<<" dR3="<<dR3<<endl;
                    Mu1_dRtriggerMatch.push_back(dR1);
                    Mu2_dRtriggerMatch.push_back(dR2);
                    Mu3_dRtriggerMatch.push_back(dR3);

                    if( isBParking){
                        dR1_Mu8 = MiniAnaTau3Mu::dRtriggerMatch(*mu1, MuonsObjects_BPMu8);
                        dR2_Mu8 = MiniAnaTau3Mu::dRtriggerMatch(*mu2, MuonsObjects_BPMu8);
                        dR3_Mu8 = MiniAnaTau3Mu::dRtriggerMatch(*mu3, MuonsObjects_BPMu8);
                        Mu1_dRtriggerMatch_Mu8.push_back(dR1_Mu8);
                        Mu2_dRtriggerMatch_Mu8.push_back(dR2_Mu8);
                        Mu3_dRtriggerMatch_Mu8.push_back(dR3_Mu8);

                        dR1_Mu7 = MiniAnaTau3Mu::dRtriggerMatch(*mu1, MuonsObjects_BPMu7);
                        dR2_Mu7 = MiniAnaTau3Mu::dRtriggerMatch(*mu2, MuonsObjects_BPMu7);
                        dR3_Mu7 = MiniAnaTau3Mu::dRtriggerMatch(*mu3, MuonsObjects_BPMu7);
                        Mu1_dRtriggerMatch_Mu7.push_back(dR1_Mu7);
                        Mu2_dRtriggerMatch_Mu7.push_back(dR2_Mu7);
                        Mu3_dRtriggerMatch_Mu7.push_back(dR3_Mu7);

                        dR1_Mu8_IP5 = MiniAnaTau3Mu::dRtriggerMatch(*mu1, MuonsObjects_BPMu8_IP5);
                        Mu1_dRtriggerMatch_Mu8_IP5.push_back(dR1_Mu8_IP5);

                        dR1_Mu8_IP6 = MiniAnaTau3Mu::dRtriggerMatch(*mu1, MuonsObjects_BPMu8_IP6);
                        Mu1_dRtriggerMatch_Mu8_IP6.push_back(dR1_Mu8_IP6);

                        dR1_Mu9_IP0 = MiniAnaTau3Mu::dRtriggerMatch(*mu1, MuonsObjects_BPMu9_IP0);
                        Mu1_dRtriggerMatch_Mu9_IP0.push_back(dR1_Mu9_IP0);

                        dR1_Mu9_IP3 = MiniAnaTau3Mu::dRtriggerMatch(*mu1, MuonsObjects_BPMu9_IP3);
                        Mu1_dRtriggerMatch_Mu9_IP3.push_back(dR1_Mu9_IP3);

                        dR1_Mu9_IP4 = MiniAnaTau3Mu::dRtriggerMatch(*mu1, MuonsObjects_BPMu9_IP4);
                        Mu1_dRtriggerMatch_Mu9_IP4.push_back(dR1_Mu9_IP4);

                        dR1_Mu9_IP5 = MiniAnaTau3Mu::dRtriggerMatch(*mu1, MuonsObjects_BPMu9_IP5);
                        Mu1_dRtriggerMatch_Mu9_IP5.push_back(dR1_Mu9_IP5);

                        dR1_Mu9_IP6 = MiniAnaTau3Mu::dRtriggerMatch(*mu1, MuonsObjects_BPMu9_IP6);
                        Mu1_dRtriggerMatch_Mu9_IP6.push_back(dR1_Mu9_IP6);

                        dR1_Mu12_IP6 = MiniAnaTau3Mu::dRtriggerMatch(*mu1, MuonsObjects_BPMu12_IP6);
                        Mu1_dRtriggerMatch_Mu12_IP6.push_back(dR1_Mu12_IP6);
                    }//isBParking

                    ///////////////Check GEN matching and Fill SimInfo///////////////
                    if(isMc){
                        bool isMatch1=false; bool isMatch2=false; bool isMatch3=false;
                        if( (mu1->simType() == reco::MatchedMuonFromHeavyFlavour) && (fabs(mu1->simMotherPdgId()) == 15) ){
                            isMatch1=true;
                        }
                        if( (mu2->simType() == reco::MatchedMuonFromHeavyFlavour) && (fabs(mu2->simMotherPdgId()) == 15) ){
                            isMatch2=true;
                        }
                        if( (mu3->simType() == reco::MatchedMuonFromHeavyFlavour) && (fabs(mu3->simMotherPdgId()) == 15) ){
                            isMatch3=true;
                        }
                        // cout<<TripletIndex<<"Triplet Mass:"<<TauIt->mass()<<" pt="<<TauIt->pt()<<" vtx.x="<<TauIt->vx()<<" vtx x="<<TripletVtx.x()<<" chi2="<<TauIt->vertexChi2()<<" ndof="<<TauIt->vertexNdof()<<endl;
                        // cout<<TripletIndex<<"--Muon 1 pt="<<mu1->pt()<<" Muon2 pt="<<mu2->pt()<<" Mu3 pt="<<mu3->pt()<<" "<<endl;
                        if( isMatch1 && isMatch2 && isMatch3) {
                            //cout<<" Matched Triplets mass="<<TauIt->mass()<<endl;
                            GenMatchMu1_SimPt.push_back(mu1->simPt());
                            GenMatchMu2_SimPt.push_back(mu2->simPt());
                            GenMatchMu3_SimPt.push_back(mu3->simPt());
                            
                            GenMatchMu1_SimEta.push_back(mu1->simEta());
                            GenMatchMu2_SimEta.push_back(mu2->simEta());
                            GenMatchMu3_SimEta.push_back(mu3->simEta());
                            
                            GenMatchMu1_SimPhi.push_back(mu1->simPhi());
                            GenMatchMu2_SimPhi.push_back(mu2->simPhi());
                            GenMatchMu3_SimPhi.push_back(mu3->simPhi());
                            
                            GenMatchMu1_Pt.push_back(mu1->pt());
                            GenMatchMu2_Pt.push_back(mu2->pt());
                            GenMatchMu3_Pt.push_back(mu3->pt());
                            
                            GenMatchMu1_Eta.push_back(mu1->eta());
                            GenMatchMu2_Eta.push_back(mu2->eta());
                            GenMatchMu3_Eta.push_back(mu3->eta());
                            
                            GenMatchMu1_Phi.push_back(mu1->phi());
                            GenMatchMu2_Phi.push_back(mu2->phi());
                            GenMatchMu3_Phi.push_back(mu3->phi());
                        }
                        //GenVtx vars to be added
                    } //if(isMC)
                    
                    ///////////////Fill Triplet Vars///////////////
                    TripletVtx_x.push_back(TauIt->vx());
                    TripletVtx_y.push_back(TauIt->vy());
                    TripletVtx_z.push_back(TauIt->vz());
                    
                    TripletVtx_Chi2.push_back(TauIt->vertexChi2());
                    TripletVtx_NDOF.push_back(TauIt->vertexNdof());
                    
                    Triplet_Mass.push_back(TauIt->mass());
                    Triplet_Pt.push_back(TauIt->pt());
                    Triplet_Eta.push_back(TauIt->eta());
                    Triplet_Phi.push_back(TauIt->phi());
                    Triplet_Charge.push_back(TauIt->charge());
                      
                    //////////////Dimu vertices////////////////////
                    std::vector<reco::TransientTrack> SVTracks12_Vtx, SVTracks23_Vtx, SVTracks13_Vtx;
                        
                    SVTracks12_Vtx.push_back(transientTrack1);
                    SVTracks12_Vtx.push_back(transientTrack2);
                    SVTracks23_Vtx.push_back(transientTrack2);
                    SVTracks23_Vtx.push_back(transientTrack3);
                    SVTracks13_Vtx.push_back(transientTrack1);
                    SVTracks13_Vtx.push_back(transientTrack3);
                    ////DiMu12////
                    KalmanVertexFitter DiMu12_fitter (true);
                    TransientVertex DiMu12Vtx = DiMu12_fitter.vertex(SVTracks12_Vtx);
                    if(DiMu12Vtx.isValid()){
                        Vtx12_Chi2.push_back(DiMu12Vtx.totalChiSquared());
                        //cout << "Vtx12_Chi2: " << DiMu12Vtx.totalChiSquared() << endl;
                        Vtx12_nDOF.push_back(DiMu12Vtx.degreesOfFreedom());
                        GlobalPoint DiMu12Pos (DiMu12Vtx.position());
                        Vtx12_x.push_back(DiMu12Pos.x());
                        //cout <<"Vtx12_x: " <<DiMu12Pos.x() << endl;
                        Vtx12_y.push_back(DiMu12Pos.y());
                        Vtx12_z.push_back(DiMu12Pos.z());
                    }else{
                        Vtx12_Chi2.push_back(-99);
                        Vtx12_nDOF.push_back(-99);
                        Vtx12_x.push_back(-99);
                        Vtx12_y.push_back(-99);
                        Vtx12_z.push_back(-99);
                    }
                    ////DiMu23///
                    KalmanVertexFitter DiMu23_fitter (true);
                    TransientVertex DiMu23Vtx = DiMu23_fitter.vertex(SVTracks23_Vtx);
                    if(DiMu23Vtx.isValid()){
                        Vtx23_Chi2.push_back(DiMu23Vtx.totalChiSquared());
                        //cout << "Vtx23_Chi2: " << DiMu23Vtx.totalChiSquared() << endl;
                        Vtx23_nDOF.push_back(DiMu23Vtx.degreesOfFreedom());
                        GlobalPoint DiMu23Pos (DiMu23Vtx.position());
                        Vtx23_x.push_back(DiMu23Pos.x());
                        //cout <<"Vtx23_x: " <<DiMu23Pos.x() << endl;
                        Vtx23_y.push_back(DiMu23Pos.y());
                        Vtx23_z.push_back(DiMu23Pos.z());
                    }else{
                        Vtx23_Chi2.push_back(-99);
                        Vtx23_nDOF.push_back(-99);
                        Vtx23_x.push_back(-99);
                        Vtx23_y.push_back(-99);
                        Vtx23_z.push_back(-99);
                    }
                    ////DiMu13///
                    KalmanVertexFitter DiMu13_fitter (true);
                    TransientVertex DiMu13Vtx = DiMu13_fitter.vertex(SVTracks13_Vtx);
                    if(DiMu13Vtx.isValid()){
                        Vtx13_Chi2.push_back(DiMu13Vtx.totalChiSquared());
                        //cout << "Vtx13_Chi2: " << DiMu13Vtx.totalChiSquared() << endl;
                        Vtx13_nDOF.push_back(DiMu13Vtx.degreesOfFreedom());
                        GlobalPoint DiMu13Pos (DiMu13Vtx.position());
                        Vtx13_x.push_back(DiMu13Pos.x());
                        //cout <<"Vtx13_x: " <<DiMu13Pos.x() << endl;
                        Vtx13_y.push_back(DiMu13Pos.y());
                        Vtx13_z.push_back(DiMu13Pos.z());
                    }else{
                        Vtx13_Chi2.push_back(-99);
                        Vtx13_nDOF.push_back(-99);
                        Vtx13_x.push_back(-99);
                        Vtx13_y.push_back(-99);
                        Vtx13_z.push_back(-99);
                    }
                    
        
                    /////////////////Defining ISO VAR related to the triplet//////////////////////
                    TLorentzVector LV1=TLorentzVector( mu1->px(), mu1->py(), mu1->pz(), mu1->energy() );
                    TLorentzVector LV2=TLorentzVector( mu2->px(), mu2->py(), mu2->pz(), mu2->energy() );
                    TLorentzVector LV3=TLorentzVector( mu3->px(), mu3->py(), mu3->pz(), mu3->energy() );
                    TLorentzVector LVTau = LV1 + LV2 + LV3;
                    cout<<TripletIndex<<" TauCandMass "<<TauIt->mass()<<" TauPt="<<TauIt->pt()<<endl;
                    cout<<TripletIndex<<" TauVectMass "<<LVTau.M()<<" TauPt="<<LVTau.Pt()<<endl;
                      
                    int nTracks03_mu1=0, nTracks03_mu2=0, nTracks03_mu3=0;
                    double mindist=9999;
                    double sumPtTrack1=0, sumPtTrack2=0, sumPtTrack3=0, maxSumPtTracks=0;

                    math::XYZPoint SVertexPoint = math::XYZPoint(TripletVtx.x(), TripletVtx.y(), TripletVtx.z());
                    uint kk=0;

                    for (std::vector<pat::PackedCandidate>::const_iterator cand = PFCands->begin(); cand != PFCands->end(), kk!= PFCands->size(); ++cand, ++kk) {
                        if(  (cand->pt()>1) && (fabs(cand->eta())<2.4) && (cand->trackerLayersWithMeasurement()>5) && (cand->pixelLayersWithMeasurement()>1) && (cand->trackHighPurity())  ){
                            
                            double dR1 = sqrt( reco::deltaR2(Track1.eta(), Track1.phi(), cand->eta(), cand->phi()) );
                            double dR2 = sqrt( reco::deltaR2(Track2.eta(), Track2.phi(), cand->eta(), cand->phi()) );
                            double dR3 = sqrt( reco::deltaR2(Track3.eta(), Track3.phi(), cand->eta(), cand->phi()) );
                            //cout<<"Skip muon track"<<endl;
                            if (dR1 < 0.01 || dR2 < 0.01 || dR3 < 0.01) continue;
                            
                            double dz = abs(cand->dz(SVertexPoint));
                            double dxy = abs(cand->dxy(SVertexPoint));
                            double dca_fv = sqrt(dz*dz+dxy*dxy);
                            if(dca_fv<mindist && dca_fv>0) {
                                mindist = dca_fv;
                            }
                            //for eack track having pt>1, excluded the muon tracks,
                            //for each muon in the triplet, if deltaR<0.3 and the DCA is smaller than 1 mm
                            //the pt of the track is added -> I will take the largest total pt from the three muons
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
                            }
                        }
                    }//loop on tracks
                      
                    Triplet_mindca_iso.push_back(mindist);
                    maxSumPtTracks = std::max(sumPtTrack1, std::max(sumPtTrack2,sumPtTrack3));
                    //cout<<TripletIndex<<" TauMass "<<TauIt->mass()<<" SumPt Tracks in cone="<<maxSumPtTracks<<" TauPt="<<TauIt->pt()<<endl;
                    double relativeiso = maxSumPtTracks/LVTau.Pt();
                    Triplet_relativeiso.push_back(relativeiso);
                    
                    Mu1_NTracks03iso.push_back(nTracks03_mu1);
                    Mu2_NTracks03iso.push_back(nTracks03_mu2);
                    Mu3_NTracks03iso.push_back(nTracks03_mu3);

                    double sumPtTrackRel1=0, sumPtTrackRel2=0, sumPtTrackRel3=0, maxSumPtRelTracks =0;
                    sumPtTrackRel1=sumPtTrack1/LV1.Pt();
                    sumPtTrackRel2=sumPtTrack2/LV2.Pt();
                    sumPtTrackRel3=sumPtTrack3/LV3.Pt();
                    maxSumPtRelTracks = std::max(sumPtTrackRel1, std::max(sumPtTrackRel2,sumPtTrackRel3));
                    Triplet_relativeiso2.push_back(maxSumPtRelTracks);
                    Triplet_IsoMu1.push_back(sumPtTrack1);
                    Triplet_IsoMu2.push_back(sumPtTrack2);
                    Triplet_IsoMu3.push_back(sumPtTrack3);

                    /////////////////Defining variables related to PV and SV positions and errors//////////////////////
                    GlobalPoint PVertexPos  (PVertex.position());
                    GlobalPoint SVertexPos  (TripletVtx.x(), TripletVtx.y(), TripletVtx.z());
                    math::XYZPoint PVertexPoint = math::XYZPoint(PVertexPos.x(), PVertexPos.y(), PVertexPos.z());

                    VertexState PVstate(PVertex.position(),PVertex.positionError());
                    //cout<<"PV position="<<PVertex.position()<<endl;
                    //cout<<"PV covariance matrix: \n"<<PVertex.positionError().matrix()<<endl;
                    //cout<<"SV position="<<TripletVtx.position()<<endl;
                    //cout<<"SV covariance matrix: \n"<<TripletVtx.error()<<endl;

                    VertexDistance3D vertTool;
                    double distance = vertTool.distance(PVstate, TripletVtx).value();
                    double dist_err = vertTool.distance(PVstate, TripletVtx).error();
                    double dist_sign =vertTool.distance(PVstate, TripletVtx).significance();

                    VertexDistanceXY vdistXY;
                    Measurement1D distXY = vdistXY.distance(TripletVtx, PVertex);
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
                            SV_cov.push_back(TripletVtx.error()[i][j]);
                        }
                    }
                    //cout<<"PV covariance matrix: \n"<<PV_cov<<endl;
                    RefittedPV_x.push_back(PVertexPos.x());
                    RefittedPV_y.push_back(PVertexPos.y());
                    RefittedPV_z.push_back(PVertexPos.z());
                    RefittedPV_cov.push_back(PV_cov);
                    //RefittedPV_NTracks.push_back(pvTracks_refit.size());
                    //RefittedPV_Chi2.push_back(PVertex.);

                    TripletVtx_cov.push_back(SV_cov);
                      
                    VertexState BSstate(beamSpot);
                    VertexDistanceXY vertTool2D;
                    double BSdistance2D = vertTool2D.distance(BSstate, TripletVtx).value();
                    double BSdist_err2D = vertTool2D.distance(BSstate, TripletVtx).error();
                    double BSdist_sign2D =vertTool2D.distance(BSstate, TripletVtx).significance();

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
                    GlobalVector dir3(mu3->px(), mu3->py(),  mu3->pz()); //To compute sign of IP
                    std::pair<bool,Measurement1D> signed_IP2D_mu1 = IPTools::signedTransverseImpactParameter(transientTrack1, dir1, PVertex);
                    std::pair<bool,Measurement1D> signed_IP2D_mu2 = IPTools::signedTransverseImpactParameter(transientTrack2, dir2, PVertex);
                    std::pair<bool,Measurement1D> signed_IP2D_mu3 = IPTools::signedTransverseImpactParameter(transientTrack3, dir3, PVertex);
                    //cout<<" IP mu1="<<signed_IP2D_mu1.second.value()<<" IP mu2="<<signed_IP2D_mu2.second.value()<<" IP mu3="<<signed_IP2D_mu3.second.value()<<endl;
                    dxy_mu1.push_back(signed_IP2D_mu1.second.value());
                    dxy_mu2.push_back(signed_IP2D_mu2.second.value());
                    dxy_mu3.push_back(signed_IP2D_mu3.second.value());
                    dxyErr_mu1.push_back(signed_IP2D_mu1.second.error());
                    dxyErr_mu2.push_back(signed_IP2D_mu2.second.error());
                    dxyErr_mu3.push_back(signed_IP2D_mu3.second.error());

                    /////////////////Study on phi->KK and K*->Kpi decays//////////////////////
                    /////min mu+track vtx chi2
                    double mu1_minvtxchi2 = 9999., mu2_minvtxchi2 = 9999., mu3_minvtxchi2 = 9999.;
                    double IsoTrack1_Pt  = -99, IsoTrack1_Eta = -99, IsoTrack1_Phi = -99; 
                    double IsoTrack2_Pt  = -99, IsoTrack2_Eta = -99, IsoTrack2_Phi = -99; 
                    double IsoTrack3_Pt  = -99, IsoTrack3_Eta = -99, IsoTrack3_Phi = -99; 
                    for (std::vector<pat::PackedCandidate>::const_iterator cand = PFCands->begin(); cand != PFCands->end(); ++cand) {
                        if(! ((cand->pt()>0.4) && (fabs(cand->eta())<2.4) && (cand->trackerLayersWithMeasurement()>5) && (cand->pixelLayersWithMeasurement()>1) && (cand->trackHighPurity())) ) continue;
                        //track-PV dz<0.5
                        double dz = abs(cand->dz(PVertexPoint));
                        if(dz>=0.5) continue;

                        reco::Track add_track = *(cand->bestTrack());
                        //track-triplet collimation
                        double dR = sqrt( reco::deltaR2(TauIt->eta(), TauIt->phi(), add_track.eta(), add_track.phi()) );
                        if(dR>1) continue; //loose cut on track+tau collimation

                        //skip track matching with mu2 and mu3
                        double dR1 = sqrt( reco::deltaR2(Track1.eta(), Track1.phi(), add_track.eta(), add_track.phi()) );
                        double dR2 = sqrt( reco::deltaR2(Track2.eta(), Track2.phi(), add_track.eta(), add_track.phi()) );
                        double dR3 = sqrt( reco::deltaR2(Track3.eta(), Track3.phi(), add_track.eta(), add_track.phi()) );
                        if(dR1<0.01 || dR2<0.01 || dR3<0.01) continue;

                        reco::TransientTrack add_ttrack = theTransientTrackBuilder->build(*(cand->bestTrack()));
                        //mu1
                        if (cand->charge()*Track1.charge() < 0) { //OS
                            //build mu1-track vertex
                            std::vector<reco::TransientTrack> mu1_track;
                            mu1_track.push_back(transientTrack1);
                            mu1_track.push_back(add_ttrack);
                            //vertex fit
                            KalmanVertexFitter mu1_track_fitter (true);
                            TransientVertex mu1_track_vtx = mu1_track_fitter.vertex(mu1_track);
                            if(!mu1_track_vtx.isValid()) continue;
                            
                            double vtxchi2 = mu1_track_vtx.totalChiSquared();
                            if (vtxchi2 < mu1_minvtxchi2) {mu1_minvtxchi2 = vtxchi2; IsoTrack1_Pt = add_track.pt(); IsoTrack1_Eta = add_track.eta(); IsoTrack1_Phi = add_track.phi();}
                        }//mu1
                        //mu2
                        if (cand->charge()*Track2.charge() < 0) { //OS
                            //build mu2-track vertex
                            std::vector<reco::TransientTrack> mu2_track;
                            mu2_track.push_back(transientTrack2);
                            mu2_track.push_back(add_ttrack);
                            //vertex fit
                            KalmanVertexFitter mu2_track_fitter (true);
                            TransientVertex mu2_track_vtx = mu2_track_fitter.vertex(mu2_track);
                            if(!mu2_track_vtx.isValid()) continue;
                            
                            double vtxchi2 = mu2_track_vtx.totalChiSquared();
                            if (vtxchi2 < mu2_minvtxchi2) {mu2_minvtxchi2 = vtxchi2; IsoTrack2_Pt = add_track.pt(); IsoTrack2_Eta = add_track.eta(); IsoTrack2_Phi = add_track.phi();}
                        }//mu2
                        //mu3
                        if (cand->charge()*Track3.charge() < 0) { //OS
                            //build mu1-track vertex
                            std::vector<reco::TransientTrack> mu3_track;
                            mu3_track.push_back(transientTrack3);
                            mu3_track.push_back(add_ttrack);
                            //vertex fit
                            KalmanVertexFitter mu3_track_fitter (true);
                            TransientVertex mu3_track_vtx = mu3_track_fitter.vertex(mu3_track);
                            if(!mu3_track_vtx.isValid()) continue;
                            
                            double vtxchi2 = mu3_track_vtx.totalChiSquared();
                            if (vtxchi2 < mu3_minvtxchi2) {mu3_minvtxchi2 = vtxchi2; IsoTrack3_Pt = add_track.pt(); IsoTrack3_Eta = add_track.eta(); IsoTrack3_Phi = add_track.phi();}
                        }//mu3
                    }//loop on tracks
                    //cout<<"min mu1+track vtx chi2="<<mu1_minvtxchi2<<" iso track pt="<<IsoTrack1_Pt<<" eta="<<IsoTrack1_Eta<<" phi="<<IsoTrack1_Phi<<endl;
                    //cout<<"  collimation dR="<<sqrt( reco::deltaR2(Track1.eta(), Track1.phi(), IsoTrack1_Eta, IsoTrack1_Phi) )<<endl;
                    //cout<<"min mu2+track vtx chi2="<<mu2_minvtxchi2<<" iso track pt="<<IsoTrack2_Pt<<" eta="<<IsoTrack2_Eta<<" phi="<<IsoTrack2_Phi<<endl;
                    //cout<<"  collimation dR="<<sqrt( reco::deltaR2(Track2.eta(), Track2.phi(), IsoTrack2_Eta, IsoTrack2_Phi) )<<endl;
                    //cout<<"min mu3+track vtx chi2="<<mu3_minvtxchi2<<" iso track pt="<<IsoTrack3_Pt<<" eta="<<IsoTrack3_Eta<<" phi="<<IsoTrack3_Phi<<endl;
                    //cout<<"  collimation dR="<<sqrt( reco::deltaR2(Track3.eta(), Track3.phi(), IsoTrack3_Eta, IsoTrack3_Phi) )<<endl;
                    IsoTrackMu1_Pt.push_back(IsoTrack1_Pt); IsoTrackMu1_Eta.push_back(IsoTrack1_Eta); IsoTrackMu1_Phi.push_back(IsoTrack1_Phi); 
                    IsoTrackMu2_Pt.push_back(IsoTrack2_Pt); IsoTrackMu2_Eta.push_back(IsoTrack2_Eta); IsoTrackMu2_Phi.push_back(IsoTrack2_Phi); 
                    IsoTrackMu3_Pt.push_back(IsoTrack3_Pt); IsoTrackMu3_Eta.push_back(IsoTrack3_Eta); IsoTrackMu3_Phi.push_back(IsoTrack3_Phi); 

                }else{ //!(PVertex.isValid() && TauIt->vertexChi2() >0)
                    Mu1_Pt.push_back(-99);
                    Mu1_Eta.push_back(-99);
                    Mu1_Phi.push_back(-99);
                    Mu1_TripletIndex.push_back(-99);

                    Mu2_Pt.push_back(-99);
                    Mu2_Eta.push_back(-99);
                    Mu2_Phi.push_back(-99);
                    Mu2_TripletIndex.push_back(-99);

                    Mu3_Pt.push_back(-99);
                    Mu3_Eta.push_back(-99);
                    Mu3_Phi.push_back(-99);
                    Mu3_TripletIndex.push_back(-99);

                    TripletVtx_x.push_back(-99);
                    TripletVtx_y.push_back(-99);
                    TripletVtx_z.push_back(-99);
                    
                    TripletVtx_Chi2.push_back(-99);
                    TripletVtx_NDOF.push_back(-99);

                    Triplet_Mass.push_back(-99);
                    Triplet_Pt.push_back(-99);
                    Triplet_Eta.push_back(-99);
                    Triplet_Phi.push_back(-99);
                    Triplet_Charge.push_back(-99);
                    
                    Triplet_mindca_iso.push_back(-99);
                    Triplet_relativeiso.push_back(-99);

                    Mu1_NTracks03iso.push_back(-99);
                    Mu2_NTracks03iso.push_back(-99);
                    Mu3_NTracks03iso.push_back(-99);
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
                    TripletVtx_cov.push_back(cov);
                    //RefittedPV_Chi2.push_back(PVertex.);
                    Vtx12_Chi2.push_back(-99);
                    Vtx12_nDOF.push_back(-99);
                    Vtx12_x.push_back(-99);
                    Vtx12_y.push_back(-99);
                    Vtx12_z.push_back(-99);
                    Vtx23_Chi2.push_back(-99);
                    Vtx23_nDOF.push_back(-99);
                    Vtx23_x.push_back(-99);
                    Vtx23_y.push_back(-99);
                    Vtx23_z.push_back(-99);
                    Vtx13_Chi2.push_back(-99);
                    Vtx13_nDOF.push_back(-99);
                    Vtx13_x.push_back(-99);
                    Vtx13_y.push_back(-99);
                    Vtx13_z.push_back(-99);
                    
                    RefTrack1_Pt.push_back(-99); RefTrack1_Eta.push_back(-99); RefTrack1_Phi.push_back(-99); RefTrack1_TripletIndex.push_back(-99);
                    RefTrack2_Pt.push_back(-99); RefTrack2_Eta.push_back(-99); RefTrack2_Phi.push_back(-99); RefTrack2_TripletIndex.push_back(-99);
                    RefTrack3_Pt.push_back(-99); RefTrack3_Eta.push_back(-99); RefTrack3_Phi.push_back(-99); RefTrack3_TripletIndex.push_back(-99);
                    RefittedSV_Chi2.push_back(-99);
                    RefittedSV_nDOF.push_back(-99);
                    RefittedSV_Mass.push_back(-99);

                    IsoTrackMu1_Pt.push_back(-99); IsoTrackMu1_Eta.push_back(-99); IsoTrackMu1_Phi.push_back(-99); 
                    IsoTrackMu2_Pt.push_back(-99); IsoTrackMu2_Eta.push_back(-99); IsoTrackMu2_Phi.push_back(-99); 
                    IsoTrackMu3_Pt.push_back(-99); IsoTrackMu3_Eta.push_back(-99); IsoTrackMu3_Phi.push_back(-99); 

                    Mu1_dRtriggerMatch.push_back(-99);
                    Mu2_dRtriggerMatch.push_back(-99);
                    Mu3_dRtriggerMatch.push_back(-99);

                    Mu1_dRtriggerMatch_Mu7.push_back(-99);
                    Mu2_dRtriggerMatch_Mu7.push_back(-99);
                    Mu3_dRtriggerMatch_Mu7.push_back(-99);

                    Mu1_dRtriggerMatch_Mu8.push_back(-99);
                    Mu2_dRtriggerMatch_Mu8.push_back(-99);
                    Mu3_dRtriggerMatch_Mu8.push_back(-99);

                    Mu1_dRtriggerMatch_Mu8_IP5.push_back(-99);
                    Mu1_dRtriggerMatch_Mu8_IP6.push_back(-99);
                    Mu1_dRtriggerMatch_Mu9_IP0.push_back(-99);
                    Mu1_dRtriggerMatch_Mu9_IP3.push_back(-99);
                    Mu1_dRtriggerMatch_Mu9_IP4.push_back(-99);
                    Mu1_dRtriggerMatch_Mu9_IP5.push_back(-99);
                    Mu1_dRtriggerMatch_Mu9_IP6.push_back(-99);
                    Mu1_dRtriggerMatch_Mu12_IP6.push_back(-99);

                    Mu1_dRtriggerMatch_2017.push_back(-99);
                    Mu2_dRtriggerMatch_2017.push_back(-99);
                    Mu3_dRtriggerMatch_2017.push_back(-99);

                    FlightDistPVSV.push_back(-99);
                    FlightDistPVSV_Err.push_back(-99);
                    FlightDistPVSV_Significance.push_back(-99);
                    FlightDistPVSV_chi2.push_back(-99);
                    dxy_mu1.push_back(-99);
                    dxy_mu2.push_back(-99);
                    dxy_mu3.push_back(-99);

                    dxyErr_mu1.push_back(-99);
                    dxyErr_mu2.push_back(-99);
                    dxyErr_mu3.push_back(-99);
                    
                    GenMatchMu1_SimPt.push_back(-99);
                    GenMatchMu2_SimPt.push_back(-99);
                    GenMatchMu3_SimPt.push_back(-99);

                    GenMatchMu1_SimEta.push_back(-99);
                    GenMatchMu2_SimEta.push_back(-99);
                    GenMatchMu3_SimEta.push_back(-99);

                    GenMatchMu1_SimPhi.push_back(-99);
                    GenMatchMu2_SimPhi.push_back(-99);
                    GenMatchMu3_SimPhi.push_back(-99);

                    GenMatchMu1_Pt.push_back(-99);
                    GenMatchMu2_Pt.push_back(-99);
                    GenMatchMu3_Pt.push_back(-99);

                    GenMatchMu1_Eta.push_back(-99);
                    GenMatchMu2_Eta.push_back(-99);
                    GenMatchMu3_Eta.push_back(-99);

                    GenMatchMu1_Phi.push_back(-99);
                    GenMatchMu2_Phi.push_back(-99);
                    GenMatchMu3_Phi.push_back(-99);
                    Triplet_relativeiso2.push_back(-99);

                    DistXY_PVSV.push_back(-99);
                    DistXY_significance_PVSV.push_back(-99);
                    Triplet_IsoMu1.push_back(-99);
                    Triplet_IsoMu2.push_back(-99);
                    Triplet_IsoMu3.push_back(-99);

                    FlightDistBS_SV.push_back(-99);
                    FlightDistBS_SV_Err.push_back(-99);
                    FlightDistBS_SV_Significance.push_back(-99);
                    
                    Mu1_IsGlobal.push_back(-99);
                    Mu2_IsGlobal.push_back(-99);
                    Mu3_IsGlobal.push_back(-99);
                    
                    Mu1_IsPF.push_back(-99);
                    Mu2_IsPF.push_back(-99);
                    Mu3_IsPF.push_back(-99);

                }//!(PVertex.isValid() && TauIt->vertexChi2() >0)
            }//transTracksAssoToVtx_copy.size()>1
        }//(VtxIdV.size()>0 && vertices->size()>0)
    }//loop Cand3Mu
}//isAna

NGoodTriplets.push_back(NTripl.size());
if(NTripl.size()>0) hEventsAfterGoodCand->Fill(1);
if(Mu1C.size()>0) hEventsAfterMu1ID->Fill(1);
if(Mu2C.size()>0)  hEventsAfterMu2ID->Fill(1);
if(Mu3C.size()>0)  hEventsAfterMu3ID->Fill(1);
if(TauMass.size()>0)  hEventsAfterTauMass->Fill(1);

//////Fill info photons//////   
PhotonCollectionSize = photons->size();
//cout<<" PhotonCollectionSize="<<PhotonCollectionSize<<endl;
uint j=0;
for(edm::View<pat::Photon>::const_iterator pho=photons->begin(); pho!=photons->end(), j<photons->size(); ++pho, ++j){
    //Basic Kinematics
    //cout<<"   PhotonEnergy="<<pho->energy()<<endl;
    PhotonPt.push_back(pho->pt());
    PhotonEt.push_back(pho->et());
    PhotonEnergy.push_back(pho->energy());
    PhotonEta.push_back(pho->eta());
    PhotonPhi.push_back(pho->phi());
}   
    
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
    Muon_simType.push_back(mu->simType());
    Muon_simBX.push_back(mu->simBX());
    //Muon_simTpEvent.push_back(mu->simTpEvent());
    //Muon_simMatchQuality.push_back(mu->simMatchQuality());

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
	
    
    ////Synch Tree//////
    /*   double maxPt =0; double maxPhi=0, maxEta=0; vector<pat::Muon> SyncSortedMu ;
     double maxTrPt =0; double maxTrPhi=0, maxTrEta=0; vector<reco::Track> SyncSortedTr ;
     for(uint i=0; i<SyncMu.size();i++){
         if(SyncMu.at(i).pt() > maxPt){
             maxPt  = SyncMu.at(i).pt();
             maxPhi = SyncMu.at(i).phi();
             maxEta = SyncMu.at(i).eta();
             SyncSortedMu.push_back(SyncMu.at(i));
         }
     }
     
     allmuons_pt.push_back(AllMuPt);
     
     leadmuon_pt.push_back(maxPt);
     leadmuon_eta.push_back(maxEta);
     leadmuon_phi.push_back(maxPhi);
     nmuons = SyncMu.size();
     nprimevtxs =vertices->size();
     
     edm::View<reco::Track>::const_iterator trIt  = trackCollection->begin();
     edm::View<reco::Track>::const_iterator trEnd = trackCollection->end();
     
     double AllTrPt=0;
     for (; trIt != trEnd; ++trIt){
         const reco::Track track = (*trIt);
         if(  (track.pt()>1) && (fabs(track.eta())<2.4) && (track.hitPattern().trackerLayersWithMeasurement()>5) && (track.hitPattern().pixelLayersWithMeasurement()>1)  ){
             AllTrPt +=trIt->pt();
             if(track.pt() > maxTrPt){
                 maxTrPt = track.pt();
                 maxTrEta = track.eta();
                 maxTrPhi= track.phi();
             }
         }
     }
     alltracks_pt.push_back(AllTrPt);
     leadtrack_pt.push_back(maxTrPt);
     leadtrack_eta.push_back(maxTrEta);
     leadtrack_phi.push_back(maxTrPhi);
     
     evt   = iEvent.id().event();
     run = iEvent.id().run();
     lumi = iEvent.luminosityBlock();
     */
    ///////SyncTree
    
    evt  = iEvent.id().event();
    run  = iEvent.id().run();
    lumi = iEvent.luminosityBlock();
   
    //cout<<"Fillin ntuple evt="<<evt<<" run="<<run<<" lumi="<<lumi<<endl; 
    //SyncTree_->Fill();
    tree_->Fill();
    
    /*
    allmuons_pt.clear();
    alltracks_pt.clear();
    leadmuon_pt.clear();
    leadmuon_phi.clear();
    leadmuon_eta.clear();
    allmuons_pt.clear();
    leadtrack_pt.clear();
    leadtrack_eta.clear();
    leadtrack_phi.clear();
    nmuons = -999;
    */
    run= -999;

    evt= -999;
    lumi= -999;
    puN= -999;

    GenParticle_PdgId.clear();
    GenParticle_Pt.clear();
    GenParticle_Eta.clear();
    GenParticle_Phi.clear();
    GenParticle_MotherPdgId.clear();
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
    Muon_simType.clear();
    Muon_simBX.clear();
    //Muon_simTpEvent.clear();
    //Muon_simMatchQuality.clear();
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
    
    Triplet_mindca_iso.clear();
    Triplet_relativeiso.clear();

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
    
    Mu1_Pt.clear();
    Mu1_Eta.clear();
    Mu1_Phi.clear();
    Mu1_NTracks03iso.clear();
    Mu1_dRtriggerMatch.clear();
    Mu1_dRtriggerMatch_Mu7.clear();
    Mu1_dRtriggerMatch_Mu8.clear();
    Mu1_dRtriggerMatch_Mu8_IP5.clear();
    Mu1_dRtriggerMatch_Mu8_IP6.clear();
    Mu1_dRtriggerMatch_Mu9_IP0.clear();
    Mu1_dRtriggerMatch_Mu9_IP3.clear();
    Mu1_dRtriggerMatch_Mu9_IP4.clear();
    Mu1_dRtriggerMatch_Mu9_IP5.clear();
    Mu1_dRtriggerMatch_Mu9_IP6.clear();
    Mu1_dRtriggerMatch_Mu12_IP6.clear();
    
    Mu2_Pt.clear();
    Mu2_Eta.clear();
    Mu2_Phi.clear();
    Mu2_NTracks03iso.clear();
    Mu2_dRtriggerMatch.clear();
    Mu2_dRtriggerMatch_Mu7.clear();
    Mu2_dRtriggerMatch_Mu8.clear();

    Mu3_Pt.clear();
    Mu3_Eta.clear();
    Mu3_Phi.clear();
    Mu3_NTracks03iso.clear();
    Mu3_dRtriggerMatch.clear();
    Mu3_dRtriggerMatch_Mu7.clear();
    Mu3_dRtriggerMatch_Mu8.clear();

    RefTrack1_Pt.clear();
    RefTrack1_Eta.clear();
    RefTrack1_Phi.clear();
    RefTrack1_TripletIndex.clear();

    RefTrack2_Pt.clear();
    RefTrack2_Eta.clear();
    RefTrack2_Phi.clear();
    RefTrack2_TripletIndex.clear();

    RefTrack3_Pt.clear();
    RefTrack3_Eta.clear();
    RefTrack3_Phi.clear();
    RefTrack3_TripletIndex.clear();

    RefittedSV_Chi2.clear();
    RefittedSV_nDOF.clear();
    RefittedSV_Mass.clear();

    IsoTrackMu1_Pt.clear();
    IsoTrackMu1_Eta.clear();
    IsoTrackMu1_Phi.clear();

    IsoTrackMu2_Pt.clear();
    IsoTrackMu2_Eta.clear();
    IsoTrackMu2_Phi.clear();

    IsoTrackMu3_Pt.clear();
    IsoTrackMu3_Eta.clear();
    IsoTrackMu3_Phi.clear();

    Mu1_dRtriggerMatch_2017.clear();
    Mu2_dRtriggerMatch_2017.clear();
    Mu3_dRtriggerMatch_2017.clear();

    Mu1_TripletIndex.clear();
    Mu2_TripletIndex.clear();
    Mu3_TripletIndex.clear();

    GenMatchMu1_SimPhi.clear();
    GenMatchMu2_SimPhi.clear();
    GenMatchMu3_SimPhi.clear();
    
    GenMatchMu1_SimPt.clear();
    GenMatchMu2_SimPt.clear();
    GenMatchMu3_SimPt.clear();
    
    GenMatchMu1_SimEta.clear();
    GenMatchMu2_SimEta.clear();
    GenMatchMu3_SimEta.clear();
    
    GenMatchMu1_Pt.clear();
    GenMatchMu2_Pt.clear();
    GenMatchMu3_Pt.clear();
    
    GenMatchMu1_Eta.clear();
    GenMatchMu2_Eta.clear();
    GenMatchMu3_Eta.clear();
    
    GenMatchMu1_Phi.clear();
    GenMatchMu2_Phi.clear();
    GenMatchMu3_Phi.clear();
    
    TripletCollectionSize = -99;
    
    TripletVtx_x.clear();
    TripletVtx_y.clear();
    TripletVtx_z.clear();
    
    TripletVtx_Chi2.clear();
    TripletVtx_NDOF.clear();
    TripletVtx_cov.clear();
    
    Triplet_Mass.clear();
    Triplet_Pt.clear();
    Triplet_Eta.clear();
    Triplet_Phi.clear();
    Triplet_Charge.clear();
    
    dxy_mu1.clear();
    dxy_mu2.clear();
    dxy_mu3.clear();
    dxyErr_mu1.clear();
    dxyErr_mu2.clear();
    dxyErr_mu3.clear();

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
    NGoodTriplets.clear();
    Triplet_relativeiso2.clear();

    MuonPt_HLT.clear();
    MuonEta_HLT.clear();
    MuonPhi_HLT.clear();
    
    MuonPt_HLT_DiMu_Incl.clear();
    MuonEta_HLT_DiMu_Incl.clear();
    MuonPhi_HLT_DiMu_Incl.clear();
    MuonPt_HLT_DiMu_Incl_displ.clear();
    MuonEta_HLT_DiMu_Incl_displ.clear();
    MuonPhi_HLT_DiMu_Incl_displ.clear();

    MuonPt_HLT2017.clear();
    MuonEta_HLT2017.clear();
    MuonPhi_HLT2017.clear();
    MuonPt_HLT_BPMu7.clear();
    MuonEta_HLT_BPMu7.clear();
    MuonPhi_HLT_BPMu7.clear();
    MuonPt_HLT_BPMu8.clear();
    MuonEta_HLT_BPMu8.clear();
    MuonPhi_HLT_BPMu8.clear();
    MuonPt_HLT_BPMu8_IP6.clear();
    MuonEta_HLT_BPMu8_IP6.clear();
    MuonPhi_HLT_BPMu8_IP6.clear();
    MuonPt_HLT_BPMu8_IP5.clear();
    MuonEta_HLT_BPMu8_IP5.clear();
    MuonPhi_HLT_BPMu8_IP5.clear();
    MuonPt_HLT_BPMu9_IP0.clear();
    MuonEta_HLT_BPMu9_IP0.clear();
    MuonPhi_HLT_BPMu9_IP0.clear();
    MuonPt_HLT_BPMu9_IP3.clear();
    MuonEta_HLT_BPMu9_IP3.clear();
    MuonPhi_HLT_BPMu9_IP3.clear();
    MuonPt_HLT_BPMu9_IP4.clear();
    MuonEta_HLT_BPMu9_IP4.clear();
    MuonPhi_HLT_BPMu9_IP4.clear();
    MuonPt_HLT_BPMu9_IP5.clear();
    MuonEta_HLT_BPMu9_IP5.clear();
    MuonPhi_HLT_BPMu9_IP5.clear();
    MuonPt_HLT_BPMu9_IP6.clear();
    MuonEta_HLT_BPMu9_IP6.clear();
    MuonPhi_HLT_BPMu9_IP6.clear();
    MuonPt_HLT_BPMu12_IP6.clear();
    MuonEta_HLT_BPMu12_IP6.clear();
    MuonPhi_HLT_BPMu12_IP6.clear();
    
    DistXY_PVSV.clear();
    DistXY_significance_PVSV.clear();
    Triplet_IsoMu1.clear();
    Triplet_IsoMu2.clear();
    Triplet_IsoMu3.clear();
    FlightDistBS_SV.clear();
    FlightDistBS_SV_Err.clear();
    FlightDistBS_SV_Significance.clear();
    
    Mu1_IsGlobal.clear();
    Mu2_IsGlobal.clear();
    Mu3_IsGlobal.clear();

    Mu1_IsPF.clear();
    Mu2_IsPF.clear();
    Mu3_IsPF.clear();
    
    L1Muon_Pt.clear();
    L1Muon_Eta.clear();
    L1Muon_Phi.clear();
    L1Muon_EtaAtVtx.clear();
    L1Muon_PhiAtVtx.clear();
    L1Muon_BX.clear();
    L1Muon_Quality.clear();
    L1Muon_ChargeValid.clear();
    L1Muon_Charge.clear();
    L1Muon_TfMuonIndex.clear();
    L1Muon_rank.clear();
    L1Muon_isoSum.clear();
}
    
// ------------ method called once each job just before starting event loop  ------------
void MiniAnaTau3Mu::beginJob() {

    hEvents = fs->make<TH1F>("hEvents","hEvents",10,0,10);
    hEventsAfterGoodCand = fs->make<TH1F>("hEventsAfterGoodCand","hEventsAfterGoodCand",10,0,10);

    //      if(NTripl.size()>0) hEventsAfterGoodCand->Fill(1);
    hEventsAfterMu1ID = fs->make<TH1F>("hEventsAfterMu1ID","hEventsAfterMu1ID",10,0,10);
    hEventsAfterMu2ID = fs->make<TH1F>("hEventsAfterMu2ID","hEventsAfterMu2ID",10,0,10);
    hEventsAfterMu3ID = fs->make<TH1F>("hEventsAfterMu3ID","hEventsAfterMu3ID",10,0,10);
    hEventsAfterTauMass = fs->make<TH1F>("hEventsAfterTauMass","hEventsAfterTauMass",10,0,10);
        
    tree_ = fs->make<TTree>("ntuple","LFVTau ntuple");
    tree_->Branch("evt", &evt);
    tree_->Branch("run", &run);
    tree_->Branch("lumi", &lumi);
    tree_->Branch("NGoodTriplets", &NGoodTriplets);
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
    tree_->Branch("GenParticle_MotherPdgId", &GenParticle_MotherPdgId);
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
    tree_->Branch("Muon_simType", &Muon_simType);
    tree_->Branch("Muon_simBX", &Muon_simBX);
    //tree_->Branch("Muon_simTpEvent", &Muon_simTpEvent);
    //tree_->Branch("Muon_simMatchQuality", &Muon_simMatchQuality);

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
    
    tree_->Branch("TripletCollectionSize", &TripletCollectionSize);
    tree_->Branch("Mu1_Pt",&Mu1_Pt);
    tree_->Branch("Mu1_Eta", &Mu1_Eta);
    tree_->Branch("Mu1_Phi", &Mu1_Phi);
    tree_->Branch("Mu1_NTracks03iso", &Mu1_NTracks03iso);
    tree_->Branch("Mu1_TripletIndex", &Mu1_TripletIndex);
    tree_->Branch("Mu1_dRtriggerMatch", &Mu1_dRtriggerMatch);
    tree_->Branch("Mu1_dRtriggerMatch_Mu7", &Mu1_dRtriggerMatch_Mu7);
    tree_->Branch("Mu1_dRtriggerMatch_Mu8", &Mu1_dRtriggerMatch_Mu8);
    tree_->Branch("Mu1_dRtriggerMatch_Mu8_IP5", &Mu1_dRtriggerMatch_Mu8_IP5);
    tree_->Branch("Mu1_dRtriggerMatch_Mu8_IP6", &Mu1_dRtriggerMatch_Mu8_IP6);
    tree_->Branch("Mu1_dRtriggerMatch_Mu9_IP0", &Mu1_dRtriggerMatch_Mu9_IP0);
    tree_->Branch("Mu1_dRtriggerMatch_Mu9_IP3", &Mu1_dRtriggerMatch_Mu9_IP3);
    tree_->Branch("Mu1_dRtriggerMatch_Mu9_IP4", &Mu1_dRtriggerMatch_Mu9_IP4);
    tree_->Branch("Mu1_dRtriggerMatch_Mu9_IP5", &Mu1_dRtriggerMatch_Mu9_IP5);
    tree_->Branch("Mu1_dRtriggerMatch_Mu9_IP6", &Mu1_dRtriggerMatch_Mu9_IP6);
    tree_->Branch("Mu1_dRtriggerMatch_Mu12_IP6", &Mu1_dRtriggerMatch_Mu12_IP6);

    tree_->Branch("Mu1_dRtriggerMatch_2017", &Mu1_dRtriggerMatch_2017);
    tree_->Branch("Mu2_dRtriggerMatch_2017", &Mu2_dRtriggerMatch_2017);
    tree_->Branch("Mu3_dRtriggerMatch_2017", &Mu3_dRtriggerMatch_2017);
 
    tree_->Branch("Mu2_Pt", &Mu2_Pt);
    tree_->Branch("Mu2_Eta", &Mu2_Eta);
    tree_->Branch("Mu2_Phi", &Mu2_Phi);
    tree_->Branch("Mu2_NTracks03iso", &Mu2_NTracks03iso);
    tree_->Branch("Mu2_dRtriggerMatch", &Mu2_dRtriggerMatch);
    tree_->Branch("Mu2_dRtriggerMatch_Mu7", &Mu2_dRtriggerMatch_Mu7);
    tree_->Branch("Mu2_dRtriggerMatch_Mu8", &Mu2_dRtriggerMatch_Mu8);
    tree_->Branch("Mu2_TripletIndex", &Mu2_TripletIndex);
        
    tree_->Branch("Mu3_Pt", &Mu3_Pt);
    tree_->Branch("Mu3_Eta",&Mu3_Eta);
    tree_->Branch("Mu3_Phi", &Mu3_Phi);
    tree_->Branch("Mu3_NTracks03iso", &Mu3_NTracks03iso);
    tree_->Branch("Mu3_dRtriggerMatch", &Mu3_dRtriggerMatch);
    tree_->Branch("Mu3_dRtriggerMatch_Mu7", &Mu3_dRtriggerMatch_Mu7);
    tree_->Branch("Mu3_dRtriggerMatch_Mu8", &Mu3_dRtriggerMatch_Mu8);
    tree_->Branch("Mu3_TripletIndex", &Mu3_TripletIndex);

    tree_->Branch("Mu1_IsGlobal", &Mu1_IsGlobal);
    tree_->Branch("Mu2_IsGlobal", &Mu2_IsGlobal);
    tree_->Branch("Mu3_IsGlobal", &Mu3_IsGlobal);
    
    tree_->Branch("Mu1_IsPF", &Mu1_IsPF);
    tree_->Branch("Mu2_IsPF", &Mu2_IsPF);
    tree_->Branch("Mu3_IsPF", &Mu3_IsPF);
    
    tree_->Branch("dxy_mu1", &dxy_mu1);
    tree_->Branch("dxy_mu2", &dxy_mu2);
    tree_->Branch("dxy_mu3", &dxy_mu3);
    tree_->Branch("dxyErr_mu1", &dxyErr_mu1);
    tree_->Branch("dxyErr_mu2", &dxyErr_mu2);
    tree_->Branch("dxyErr_mu3", &dxyErr_mu3);

    tree_->Branch("RefTrack1_Pt",           &RefTrack1_Pt);
    tree_->Branch("RefTrack1_Eta",          &RefTrack1_Eta);
    tree_->Branch("RefTrack1_Phi",          &RefTrack1_Phi);
    tree_->Branch("RefTrack1_TripletIndex", &RefTrack1_TripletIndex);
    tree_->Branch("RefTrack2_Pt",           &RefTrack2_Pt);
    tree_->Branch("RefTrack2_Eta",          &RefTrack2_Eta);
    tree_->Branch("RefTrack2_Phi",          &RefTrack2_Phi);
    tree_->Branch("RefTrack2_TripletIndex", &RefTrack2_TripletIndex);
    tree_->Branch("RefTrack3_Pt",           &RefTrack3_Pt);
    tree_->Branch("RefTrack3_Eta",          &RefTrack3_Eta);
    tree_->Branch("RefTrack3_Phi",          &RefTrack3_Phi);
    tree_->Branch("RefTrack3_TripletIndex", &RefTrack3_TripletIndex);

    tree_->Branch("RefittedSV_Chi2", &RefittedSV_Chi2);
    tree_->Branch("RefittedSV_nDOF", &RefittedSV_nDOF);
    tree_->Branch("RefittedSV_Mass", &RefittedSV_Mass);

    tree_->Branch("IsoTrackMu1_Pt",         &IsoTrackMu1_Pt);
    tree_->Branch("IsoTrackMu1_Eta",        &IsoTrackMu1_Eta);
    tree_->Branch("IsoTrackMu1_Phi",        &IsoTrackMu1_Phi);
    tree_->Branch("IsoTrackMu2_Pt",         &IsoTrackMu2_Pt);
    tree_->Branch("IsoTrackMu2_Eta",        &IsoTrackMu2_Eta);
    tree_->Branch("IsoTrackMu2_Phi",        &IsoTrackMu2_Phi);
    tree_->Branch("IsoTrackMu3_Pt",         &IsoTrackMu3_Pt);
    tree_->Branch("IsoTrackMu3_Eta",        &IsoTrackMu3_Eta);
    tree_->Branch("IsoTrackMu3_Phi",        &IsoTrackMu3_Phi);

    tree_->Branch("GenMatchMu1_SimPt", &GenMatchMu1_SimPt);
    tree_->Branch("GenMatchMu2_SimPt", &GenMatchMu2_SimPt);
    tree_->Branch("GenMatchMu3_SimPt", &GenMatchMu3_SimPt);
    
    tree_->Branch("GenMatchMu1_SimEta", &GenMatchMu1_SimEta);
    tree_->Branch("GenMatchMu2_SimEta", &GenMatchMu2_SimEta);
    tree_->Branch("GenMatchMu3_SimEta", &GenMatchMu3_SimEta);
    
    tree_->Branch("GenMatchMu1_SimPhi", &GenMatchMu1_SimPhi);
    tree_->Branch("GenMatchMu2_SimPhi", &GenMatchMu2_SimPhi);
    tree_->Branch("GenMatchMu3_SimPhi", &GenMatchMu3_SimPhi);
    
    tree_->Branch("GenMatchMu1_Pt", &GenMatchMu1_Pt);
    tree_->Branch("GenMatchMu2_Pt", &GenMatchMu2_Pt);
    tree_->Branch("GenMatchMu3_Pt", &GenMatchMu3_Pt);
    
    tree_->Branch("GenMatchMu1_Eta", &GenMatchMu1_Eta);
    tree_->Branch("GenMatchMu2_Eta", &GenMatchMu2_Eta);
    tree_->Branch("GenMatchMu3_Eta", &GenMatchMu3_Eta);
    
    tree_->Branch("GenMatchMu1_Phi", &GenMatchMu1_Phi);
    tree_->Branch("GenMatchMu2_Phi", &GenMatchMu2_Phi);
    tree_->Branch("GenMatchMu3_Phi", &GenMatchMu3_Phi);
    
    tree_->Branch("TripletVtx_x", &TripletVtx_x);
    tree_->Branch("TripletVtx_y", &TripletVtx_y);
    tree_->Branch("TripletVtx_z", &TripletVtx_z);
    
    tree_->Branch("TripletVtx_Chi2", &TripletVtx_Chi2);
    tree_->Branch("TripletVtx_NDOF", &TripletVtx_NDOF);
    tree_->Branch("TripletVtx_cov", &TripletVtx_cov);
    
    tree_->Branch("Triplet_Mass", &Triplet_Mass);
    tree_->Branch("Triplet_Pt", &Triplet_Pt);
    tree_->Branch("Triplet_Eta", &Triplet_Eta);
    tree_->Branch("Triplet_Phi", &Triplet_Phi);
    tree_->Branch("Triplet_Charge", &Triplet_Charge);
    
    tree_->Branch("Triplet_mindca_iso", &Triplet_mindca_iso);
    tree_->Branch("Triplet_relativeiso", &Triplet_relativeiso);
    tree_->Branch("Triplet_relativeiso2", &Triplet_relativeiso2);
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
    tree_->Branch("MuonPt_HLT2017", &MuonPt_HLT2017);
    tree_->Branch("MuonEta_HLT2017", &MuonEta_HLT2017);
    tree_->Branch("MuonPhi_HLT2017", &MuonPhi_HLT2017);
    
    tree_->Branch("MuonPt_HLT_DiMu_Incl", &MuonPt_HLT_DiMu_Incl);
    tree_->Branch("MuonEta_HLT_DiMu_Incl", &MuonEta_HLT_DiMu_Incl);
    tree_->Branch("MuonPhi_HLT_DiMu_Incl", &MuonPhi_HLT_DiMu_Incl);
    tree_->Branch("MuonPt_HLT_DiMu_Incl_displ", &MuonPt_HLT_DiMu_Incl_displ);
    tree_->Branch("MuonEta_HLT_DiMu_Incl_displ", &MuonEta_HLT_DiMu_Incl_displ);
    tree_->Branch("MuonPhi_HLT_DiMu_Incl_displ", &MuonPhi_HLT_DiMu_Incl_displ);

    tree_->Branch("DistXY_PVSV", &DistXY_PVSV);
    tree_->Branch("DistXY_significance_PVSV", &DistXY_significance_PVSV);
    tree_->Branch("Triplet_IsoMu1", &Triplet_IsoMu1);
    tree_->Branch("Triplet_IsoMu2", &Triplet_IsoMu2);
    tree_->Branch("Triplet_IsoMu3", &Triplet_IsoMu3);
    tree_->Branch("FlightDistBS_SV", &FlightDistBS_SV);
    tree_->Branch("FlightDistBS_SV_Err", &FlightDistBS_SV_Err);
    tree_->Branch("FlightDistBS_SV_Significance", &FlightDistBS_SV_Significance);

    if(isBParking){
        tree_->Branch("MuonPt_HLT_BPMu7", &MuonPt_HLT_BPMu7);
        tree_->Branch("MuonEta_HLT_BPMu7", &MuonEta_HLT_BPMu7);
        tree_->Branch("MuonPhi_HLT_BPMu7", &MuonPhi_HLT_BPMu7);
        tree_->Branch("MuonPt_HLT_BPMu8", &MuonPt_HLT_BPMu8);
        tree_->Branch("MuonEta_HLT_BPMu8", &MuonEta_HLT_BPMu8);
        tree_->Branch("MuonPhi_HLT_BPMu8", &MuonPhi_HLT_BPMu8);
        tree_->Branch("MuonPt_HLT_BPMu8_IP6", &MuonPt_HLT_BPMu8_IP6);
        tree_->Branch("MuonEta_HLT_BPMu8_IP6", &MuonEta_HLT_BPMu8_IP6);
        tree_->Branch("MuonPhi_HLT_BPMu8_IP6", & MuonPhi_HLT_BPMu8_IP6);
        tree_->Branch("MuonPt_HLT_BPMu8_IP5", &MuonPt_HLT_BPMu8_IP5);
        tree_->Branch("MuonEta_HLT_BPMu8_IP5", &MuonEta_HLT_BPMu8_IP5);
        tree_->Branch("MuonPhi_HLT_BPMu8_IP5", &MuonPhi_HLT_BPMu8_IP5);
        tree_->Branch("MuonPt_HLT_BPMu9_IP0", &MuonPt_HLT_BPMu9_IP0);
        tree_->Branch("MuonEta_HLT_BPMu9_IP0", &MuonEta_HLT_BPMu9_IP0);
        tree_->Branch("MuonPhi_HLT_BPMu9_IP0", &MuonPhi_HLT_BPMu9_IP0);
        tree_->Branch("MuonPt_HLT_BPMu9_IP3", &MuonPt_HLT_BPMu9_IP3);
        tree_->Branch("MuonEta_HLT_BPMu9_IP3", &MuonEta_HLT_BPMu9_IP3);
        tree_->Branch("MuonPhi_HLT_BPMu9_IP3", &MuonPhi_HLT_BPMu9_IP3);
        tree_->Branch("MuonPt_HLT_BPMu9_IP4", &MuonPt_HLT_BPMu9_IP4);
        tree_->Branch("MuonEta_HLT_BPMu9_IP4", &MuonEta_HLT_BPMu9_IP4);
        tree_->Branch("MuonPhi_HLT_BPMu9_IP4", &MuonPhi_HLT_BPMu9_IP4);
        tree_->Branch("MuonPt_HLT_BPMu9_IP5", &MuonPt_HLT_BPMu9_IP5);
        tree_->Branch("MuonEta_HLT_BPMu9_IP5", &MuonEta_HLT_BPMu9_IP5);
        tree_->Branch("MuonPhi_HLT_BPMu9_IP5", &MuonPhi_HLT_BPMu9_IP5);
        tree_->Branch("MuonPt_HLT_BPMu9_IP6", &MuonPt_HLT_BPMu9_IP6);
        tree_->Branch("MuonEta_HLT_BPMu9_IP6", &MuonEta_HLT_BPMu9_IP6);
        tree_->Branch("MuonPhi_HLT_BPMu9_IP6", &MuonPhi_HLT_BPMu9_IP6);
        tree_->Branch("MuonPt_HLT_BPMu12_IP6", &MuonPt_HLT_BPMu12_IP6);
        tree_->Branch("MuonEta_HLT_BPMu12_IP6", &MuonEta_HLT_BPMu12_IP6);
        tree_->Branch("MuonPhi_HLT_BPMu12_IP6", &MuonPhi_HLT_BPMu12_IP6);
    }//isBParking
    
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
    
    /*
    SyncTree_ = fs->make<TTree>("t","Sync ntuple");
    SyncTree_ ->Branch("allmuons_pt",&allmuons_pt);
    SyncTree_->Branch("leadmuon_pt",&leadmuon_pt);
    SyncTree_->Branch("leadmuon_phi",&leadmuon_phi);
    SyncTree_->Branch("leadmuon_eta",&leadmuon_eta);
    SyncTree_->Branch("nmuons",&nmuons);
    SyncTree_->Branch("nprimevtxs",&nprimevtxs); 
    
    SyncTree_->Branch("leadtrack_pt", &leadtrack_pt);
    SyncTree_->Branch("leadtrack_eta", &leadtrack_eta);
    SyncTree_->Branch("leadtrack_phi", &leadtrack_phi);
    SyncTree_->Branch("alltracks_pt", &alltracks_pt);
    SyncTree_->Branch("evt", &evt);
    SyncTree_->Branch("run", &run);
    SyncTree_->Branch("lumi", &lumi);
    */
}//MiniAnaTau3Mu::beginJob
    
// ------------ method called once each job just after ending the event loop  ------------
void MiniAnaTau3Mu::endJob() {

    tree_->GetDirectory()->cd();
    tree_->Write();
    //SyncTree_->GetDirectory()->cd();
    //SyncTree_->Write();
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void MiniAnaTau3Mu::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MiniAnaTau3Mu);
