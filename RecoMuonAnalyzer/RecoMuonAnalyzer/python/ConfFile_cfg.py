import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.Services_cff')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:/lustre/cms/store/user/rosma/SingleMuon/crab_SingleMuonRun2016B_MyZMuSkim_CMSSW_8_0_10_v4/170108_161635/0000/ZMu_854.root'
#        'file:./Run2016B_SingleMuon_RAWRECO_ZMuPromptReco.root'
    )
)

process.recoMuAna = cms.EDAnalyzer('RecoMuonAnalyzer',
                                   muonsInputTag = cms.InputTag("muons"),
                                   
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histoSingleMu_skim.root")
                                   )


process.p = cms.Path(process.recoMuAna)
