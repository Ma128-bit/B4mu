import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.Services_cff')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
       'file:/lustrehome/venditti/ZMuSkim_80X/CMSSW_8_0_10/src/ZMu_original_file854.root'

#        'file:./Run2016B_SingleMuon_RAWRECO_ZMuPromptReco.root'
    )
)

process.recoMuAna = cms.EDAnalyzer('RecoMuonAnalyzer',
                                   muonsInputTag = cms.InputTag("muons"),
                                   
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histoSingleMu_oldSkim.root")
                                   )


process.p = cms.Path(process.recoMuAna)
