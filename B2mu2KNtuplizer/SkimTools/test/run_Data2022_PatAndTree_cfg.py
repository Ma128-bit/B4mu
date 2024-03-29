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

process.GlobalTag.globaltag = '124X_dataRun3_v14' #Data2022

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #"file:/afs/cern.ch/user/m/mbuonsan/B_4mu/B4mu_Ntuple_Analysis/CMSSW_13_0_13/src/B4muNtuplizer/SkimTools/test/Run3_Bd4Mu_step2_322.root"
        #'root://xrootd-cms.infn.it//store/user/mbuonsan/Bd4Mu_13p6TeV-pythia8_Run3/130X_mcRun3_2022_realistic_postEE_v6_Bd4Mu_MINIAODSIM/231123_153700/0000/Run3_Bd4Mu_step2_310.root'
        'root://xrootd-cms.infn.it//store/data/Run2022C/ParkingDoubleMuonLowMass0/MINIAOD/PromptReco-v1/000/355/862/00000/fc972444-ec73-42ea-897c-f2b918fbee7a.root'
    ),
            #eventsToProcess = cms.untracked.VEventRange('320012:56448719')
)


process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("TreeData.root"))




process.unpackedPatTrigger = cms.EDProducer("PATTriggerObjectStandAloneUnpacker",
    patTriggerObjectsStandAlone = cms.InputTag( 'slimmedPatTrigger' ),
    triggerResults              = cms.InputTag( 'TriggerResults::HLT' ),
    unpackFilterLabels = cms.bool(True)
)


process.TreeB2mu2K = cms.EDAnalyzer("MiniAnaB2Mu2K",
                                      isMcLabel = cms.untracked.bool(False),
                                      is2KLabel = cms.untracked.bool(True),
                                      isAnaLabel = cms.untracked.bool(True),
                                      muonLabel=cms.InputTag("looseMuons"),
                                      VertexLabel=cms.InputTag("offlineSlimmedPrimaryVertices"),
                                      TracksLabel=cms.InputTag("LooseTrack"),
                                      genParticleLabel=cms.InputTag("prunedGenParticles"),
                                      genParticleLabeln2=cms.InputTag("packedGenParticles"),
                                      #Cand2Mu2TracksLabel=cms.InputTag("TwoMuonsTwoTracksKalmanVtxFit"),
                                      Cand2Mu2TracksLabel=cms.InputTag("TwoMuonsTwoTracksKinVtxFit"),
                                      pileupSummary = cms.InputTag("slimmedAddPileupInfo"),
                                      triggerResults = cms.InputTag("TriggerResults", "", "HLT"),
                                      objects = cms.InputTag("unpackedPatTrigger"),
                                      AlgInputTag = cms.InputTag( "gtStage2Digis" ),
                                      algInputTag = cms.InputTag( "gtStage2Digis" ),
                                      extInputTag = cms.InputTag( "gtStage2Digis" )
                                      
)

process.TreeB2muKpi = cms.EDAnalyzer("MiniAnaB2Mu2K",
                                      isMcLabel = cms.untracked.bool(False),
                                      is2KLabel = cms.untracked.bool(False),
                                      isAnaLabel = cms.untracked.bool(True),
                                      muonLabel=cms.InputTag("looseMuons"),
                                      VertexLabel=cms.InputTag("offlineSlimmedPrimaryVertices"),
                                      TracksLabel=cms.InputTag("LooseTrack"),
                                      genParticleLabel=cms.InputTag("prunedGenParticles"),
                                      genParticleLabeln2=cms.InputTag("packedGenParticles"),
                                      #Cand2Mu2TracksLabel=cms.InputTag("TwoMuonsTwoTracksKalmanVtxFitpi"),
                                      Cand2Mu2TracksLabel=cms.InputTag("TwoMuonsTwoTracksKinVtxFitpi"),
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
