import FWCore.ParameterSet.Config as cms
import os, sys
sys.path.append('/lustre/cms/store/user/adiflori/MCProductions/BsJPsiPhi4Mu')
from bs_4mu import *

process = cms.Process('B4MuSkim')

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
process.load("B4muNtuplizer.SkimTools.B4MuSkim_miniAOD_cff")

process.GlobalTag.globaltag = '130X_mcRun3_2023_realistic_v8' #Run3_2023 

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(files
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

process.TreeMakerBkg = cms.EDAnalyzer("MiniAnaB4Mu",
                                      isMcLabel = cms.untracked.bool(True),
                                      isAnaLabel = cms.untracked.bool(True),
                                      muonLabel=cms.InputTag("looseMuons"),
                                      photonLabel=cms.InputTag("slimmedPhotons"),
                                      VertexLabel=cms.InputTag("offlineSlimmedPrimaryVertices"),
                                      genParticleLabel=cms.InputTag("prunedGenParticles"),
                                      pileupSummary = cms.InputTag("slimmedAddPileupInfo"),
                                      Cand4MuLabel=cms.InputTag("FourMuonsVtxKalmanFit"),
                                      triggerResults = cms.InputTag("TriggerResults", "", "HLT"),
                                      objects = cms.InputTag("unpackedPatTrigger"),
                                      AlgInputTag = cms.InputTag( "gtStage2Digis" ),
                                      algInputTag = cms.InputTag( "gtStage2Digis" ),
                                      extInputTag = cms.InputTag( "gtStage2Digis" )
                                      
)




process.B4MuSkim = cms.Path(process.FourMuonSelSeq*
                              process.unpackedPatTrigger*
                              process.TreeMakerBkg
                     )
"""
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string("fileMINIADOSIM.root"),
                               SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('B4MuSkim')),
                               outputCommands = cms.untracked.vstring(
        'drop *',
        'keep *_*_*_Tau3MuSkim', 
        'keep *_offlineSlimmedPrimaryVertices_*_*',
        'keep *_generator_*_*',
        'keep *_offlineBeamSpot_*_*',
        'keep *_slimmedMuons_*_*',
        'keep *_TriggerResults_*_*',
        'keep *_gtStage2Digis_*_*',
        'keep *_gmtStage2Digis_*_*',
        'keep *_scalersRawToDigi_*_*',
        'keep *_offlineSlimmedPrimaryVertices_*_*',
        'keep *_patTrigger_*_*',
        'keep *_slimmedAddPileupInfo_*_*',
        'keep *_slimmedMETs_*_*',
        'keep *_slimmedMETsNoHF_*_*',
        'keep *_slimmedMETsPuppi_*_*',
        'keep *_packedGenParticles_*_*',
        'keep *_selectedPatTrigger_*_*',
        'keep *_offlineSlimmedPrimaryVertices_*_*',
        'keep *_slimmedSecondaryVertices_*_*',
        'keep *_bunchSpacingProducer_*_*',
        )
)


process.outpath = cms.EndPath(process.out) 
"""

