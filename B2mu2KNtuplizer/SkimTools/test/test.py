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
        'root://xrootd-cms.infn.it//store/mc/Run3Summer22MiniAODv4/BdtoJpsiKstar_Jpsito2Mu_KstartoKPi_MuFilter_TuneCP5_13p6TeV_pythia8-evtgen/MINIAODSIM/130X_mcRun3_2022_realistic_v5-v2/40000/070b41b3-52d1-469c-951c-4d128396cad9.root'
        #'root://xrootd-cms.infn.it//store/mc/Run3Summer22EEMiniAODv4/BdtoJpsiKstar_Jpsito2Mu_KstartoKPi_TuneCP5_13p6TeV_pythia8-evtgen/MINIAODSIM/130X_mcRun3_2022_realistic_postEE_v6-v2/40000/c741a6b8-14e4-4937-9547-5667b9c76483.root'
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

process.TreeB2muKpi = cms.EDAnalyzer("MiniAnaB2Mu2K",
                                      isMcLabel = cms.untracked.bool(True),
                                      is2KLabel = cms.untracked.bool(False),
                                      isAnaLabel = cms.untracked.bool(True),
                                      muonLabel=cms.InputTag("looseMuons"),
                                      VertexLabel=cms.InputTag("offlineSlimmedPrimaryVertices"),
                                      TracksLabel=cms.InputTag("LooseTrack"),
                                      genParticleLabel=cms.InputTag("prunedGenParticles"),
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
                              process.TreeB2muKpi
                     )
