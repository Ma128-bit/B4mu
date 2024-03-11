import FWCore.ParameterSet.Config as cms
import os

process = cms.Process('B2Mu2KSkim')

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load("B2mu2KNtuplizer.SkimTools.B2Mu2KSkim_miniAOD_cff")

process.GlobalTag.globaltag = '130X_mcRun3_2022_realistic_postEE_v6' #MC2022

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'root://xrootd-cms.infn.it//store/mc/Run3Summer22EEMiniAODv4/BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/MINIAODSIM/130X_mcRun3_2022_realistic_postEE_v6-v2/50000/43d2b67f-908f-4c7f-952e-22d02e1852a5.root'
      #"file:/afs/cern.ch/user/m/mbuonsan/B_4mu/B4mu_Ntuple_Analysis/CMSSW_13_0_13/src/B4muNtuplizer/SkimTools/test/Run3_Bd4Mu_step2_322.root"
      #'root://xrootd-cms.infn.it//store/user/mbuonsan/Bd4Mu_13p6TeV-pythia8_Run3/130X_mcRun3_2022_realistic_postEE_v6_Bd4Mu_MINIAODSIM/231123_153700/0000/Run3_Bd4Mu_step2_310.root'
      #'/store/user/caruta/Pythia8_DsTau3mu_Run3_2022/124X_mcRun3_2022_realistic_v12_MINIAODSIM/221120_083655/0000/DsTau3mu_2022_step2_1.root'
    ),
            #eventsToProcess = cms.untracked.VEventRange('320012:56448719')
)


process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("TreeMC.root"))




process.unpackedPatTrigger = cms.EDProducer("PATTriggerObjectStandAloneUnpacker",
    patTriggerObjectsStandAlone = cms.InputTag( 'slimmedPatTrigger' ),
    triggerResults              = cms.InputTag( 'TriggerResults::HLT' ),
    unpackFilterLabels = cms.bool(True)
)

process.TreeB2mu2K = cms.EDAnalyzer("MiniAnaB2Mu2K",
                                      isMcLabel = cms.untracked.bool(True),
                                      is2KLabel = cms.untracked.bool(True),
                                      isAnaLabel = cms.untracked.bool(True),
                                      muonLabel=cms.InputTag("looseMuons"),
                                      VertexLabel=cms.InputTag("offlineSlimmedPrimaryVertices"),
                                      TracksLabel=cms.InputTag("LooseTrack"),
                                      genParticleLabel=cms.InputTag("prunedGenParticles"),
                                      Cand2Mu2TracksLabel=cms.InputTag("TwoMuonsTwoTracksKalmanVtxFit"),
                                      pileupSummary = cms.InputTag("slimmedAddPileupInfo"),
                                      triggerResults = cms.InputTag("TriggerResults", "", "HLT"),
                                      objects = cms.InputTag("unpackedPatTrigger"),
                                      AlgInputTag = cms.InputTag( "gtStage2Digis" ),
                                      algInputTag = cms.InputTag( "gtStage2Digis" ),
                                      extInputTag = cms.InputTag( "gtStage2Digis" )
                                      
)

process.TreeB2muKpi = cms.EDAnalyzer("MiniAnaB2Mu2K",
                                      isMcLabel = cms.untracked.bool(True),
                                      is2KLabel = cms.untracked.bool(False),
                                      isAnaLabel = cms.untracked.bool(True),
                                      muonLabel=cms.InputTag("looseMuons"),
                                      VertexLabel=cms.InputTag("offlineSlimmedPrimaryVertices"),
                                      TracksLabel=cms.InputTag("LooseTrack"),
                                      genParticleLabel=cms.InputTag("prunedGenParticles"),
                                      Cand2Mu2TracksLabel=cms.InputTag("TwoMuonsTwoTracksKalmanVtxFitpi"),
                                      pileupSummary = cms.InputTag("slimmedAddPileupInfo"),
                                      triggerResults = cms.InputTag("TriggerResults", "", "HLT"),
                                      objects = cms.InputTag("unpackedPatTrigger"),
                                      AlgInputTag = cms.InputTag( "gtStage2Digis" ),
                                      algInputTag = cms.InputTag( "gtStage2Digis" ),
                                      extInputTag = cms.InputTag( "gtStage2Digis" )
                                      
)

process.B2Mu2KSkim = cms.Path(process.TwoMuTwoTracksSelSeq*
                              process.unpackedPatTrigger*
                              process.TreeB2mu2K*
                              process.TreeB2muKpi
                     )
