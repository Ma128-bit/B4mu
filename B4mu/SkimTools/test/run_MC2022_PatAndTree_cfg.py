import FWCore.ParameterSet.Config as cms
import os

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
process.load("B4MuSkim_miniAOD_cff", os.path.join(os.getcwd(), "B4MuSkim_miniAOD_cff.py"))

process.GlobalTag.globaltag = '130X_mcRun3_2022_realistic_postEE_v6' #MC2022

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
      'root://xrootd-cms.infn.it///lustre/cms/store/user/mbuonsan/Bd4Mu_13p6TeV-pythia8_Run3/130X_mcRun3_2022_realistic_postEE_v6_Bd4Mu_MINIAODSIM/231123_153700/0000/Run3_Bd4Mu_step2_310.root'
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
"""
process.TreeMakerBkg = cms.EDAnalyzer("MiniAnaB4Mu",
                                      isMcLabel = cms.untracked.bool(True),
                                      isAnaLabel = cms.untracked.bool(True),
                                      is2016Label = cms.untracked.bool(True),
                                      is2017Label = cms.untracked.bool(True),
                                      is2018Label = cms.untracked.bool(True),
                                      isBParkingLabel = cms.untracked.bool(False),
                                      muonLabel=cms.InputTag("looseMuons"),
                                      photonLabel=cms.InputTag("slimmedPhotons"),
                                      VertexLabel=cms.InputTag("offlineSlimmedPrimaryVertices"),
                                      genParticleLabel=cms.InputTag("prunedGenParticles"),
                                      pileupSummary = cms.InputTag("slimmedAddPileupInfo"),
                                      Cand3MuLabel=cms.InputTag("ThreeMuonsVtxKalmanFit"),
                                      triggerResults = cms.InputTag("TriggerResults", "", "HLT"),
                                      objects = cms.InputTag("unpackedPatTrigger"),
                                      AlgInputTag = cms.InputTag( "gtStage2Digis" ),
                                      algInputTag = cms.InputTag( "gtStage2Digis" ),
                                      extInputTag = cms.InputTag( "gtStage2Digis" )
                                      
)
"""



process.B4MuSkim = cms.Path(process.FourMuonSelSeq*
                              process.unpackedPatTrigger
                              #process.TreeMakerBkg
                     )




