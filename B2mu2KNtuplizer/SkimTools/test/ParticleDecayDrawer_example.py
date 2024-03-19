import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("DecayDrawer")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'root://xrootd-cms.infn.it//store/mc/Run3Summer22EEMiniAODv4/BdtoJpsiKstar_Jpsito2Mu_KstartoKPi_MuFilter_TuneCP5_13p6TeV_pythia8-evtgen/MINIAODSIM/130X_mcRun3_2022_realistic_postEE_v6-v2/2520000/2aa5b179-119a-47d1-8cb4-d58583db62ee.root'
        #'root://xrootd-cms.infn.it//store/mc/Run3Summer22EEMiniAODv4/BdtoJpsiKstar_Jpsito2Mu_KstartoKPi_TuneCP5_13p6TeV_pythia8-evtgen/MINIAODSIM/130X_mcRun3_2022_realistic_postEE_v6-v2/40000/c741a6b8-14e4-4937-9547-5667b9c76483.root'
    ),
)

"""
process.printTree = cms.EDAnalyzer("ParticleListDrawer",
  maxEventsToPrint = cms.untracked.int32(100),
  printVertex = cms.untracked.bool(False),
  printOnlyHardInteraction = cms.untracked.bool(False),
  src = cms.InputTag("prunedGenParticles")
)
"""
process.printTree = cms.EDAnalyzer("ParticleTreeDrawer",
                                   src = cms.InputTag("prunedGenParticles"),                                                                 
                                   printP4 = cms.untracked.bool(False),
                                   printPtEtaPhi = cms.untracked.bool(False),
                                   printVertex = cms.untracked.bool(False),
                                   printStatus = cms.untracked.bool(False),
                                   printIndex = cms.untracked.bool(False),
                                   #status = cms.untracked.vint32( 3 )
                                   )
"""
process.printDecay = cms.EDAnalyzer("ParticleDecayDrawer",
    src = cms.InputTag("prunedGenParticles"),
    printP4 = cms.untracked.bool(False),
    printPtEtaPhi = cms.untracked.bool(False),
    printVertex = cms.untracked.bool(False)
  )
"""
process.p = cms.Path(process.printTree)
