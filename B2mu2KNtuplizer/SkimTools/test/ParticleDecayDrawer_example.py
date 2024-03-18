import FWCore.ParameterSet.Config as cms
import os

process = cms.Process('B2Mu2KSkim')

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.GlobalTag.globaltag = '130X_mcRun3_2022_realistic_postEE_v6' #MC2022

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'root://xrootd-cms.infn.it//store/mc/Run3Summer22MiniAODv4/BdtoJpsiKstar_Jpsito2Mu_KstartoKPi_MuFilter_TuneCP5_13p6TeV_pythia8-evtgen/MINIAODSIM/130X_mcRun3_2022_realistic_v5-v2/40000/070b41b3-52d1-469c-951c-4d128396cad9.root'
        #'root://xrootd-cms.infn.it//store/mc/Run3Summer22EEMiniAODv4/BdtoJpsiKstar_Jpsito2Mu_KstartoKPi_TuneCP5_13p6TeV_pythia8-evtgen/MINIAODSIM/130X_mcRun3_2022_realistic_postEE_v6-v2/40000/c741a6b8-14e4-4937-9547-5667b9c76483.root'
    ),
)


process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("TreeMC.root"))


process.printDecay = cms.EDAnalyzer("ParticleDecayDrawer",
    src = cms.InputTag("genParticles"),
    printP4 = cms.untracked.bool(False),
    printPtEtaPhi = cms.untracked.bool(False),
    printVertex = cms.untracked.bool(False)
  )

process.B2Mu2KSkim = cms.Path(process.ParticleDecayDrawer
                     )
