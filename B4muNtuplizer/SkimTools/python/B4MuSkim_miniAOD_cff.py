import FWCore.ParameterSet.Config as cms

import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
from PhysicsTools.PatAlgos.producersLayer1.genericParticleProducer_cfi import patGenericParticles
from PhysicsTools.PatAlgos.producersLayer1.muonProducer_cfi import patMuons
from PhysicsTools.PatAlgos.selectionLayer1.muonSelector_cfi import *
#from PhysicsTools.PatAlgos.triggerLayer1.triggerEventProducer_cfi import *
#from PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cfi import *
#from PhysicsTools.PatAlgos.tools.trigTools import *


B4MuHLTFilter = copy.deepcopy(hltHighLevel)
B4MuHLTFilter.throw = cms.bool(False)
B4MuHLTFilter.HLTPaths = ["HLT_DoubleMu4_3_LowMass*"]
#B4MuHLTFilter.HLTPaths = ["HLT_DoubleMu4_3_LowMass*", "HLT_DoubleMu3_Trk_Tau3mu*", "HLT_DoubleMu3_TkMu_DsTau3Mu_v*", "HLT_DoubleMu3_Trk_Tau3mu_NoL1Mass_v*"]


PatMuons = patMuons.clone(
    src = cms.InputTag("muons"),
    useParticleFlow = cms.bool( False ),
    #embedHighLevelSelection = cms.bool(True),
    computeMiniIso = cms.bool(False),
    computeMuonMVA= cms.bool(False),
    computeSoftMuonMVA = cms.bool(True),
    addTriggerMatching = cms.bool(False),
    addGenMatch   = cms.bool(False),
    embedGenMatch = cms.bool(True),
)



looseMuons = cms.EDFilter("PATMuonSelector",
                          src = cms.InputTag("slimmedMuons"),
                          #cut = cms.string('pt > 2 &&  abs(eta)<2.4 && (innerTrack().isNonnull) && (charge!=0) && (innerTrack().hitPattern().numberOfValidPixelHits()>0)'),
                          cut = cms.string('pt > 0. &&  abs(eta)<2.5 && (innerTrack().isNonnull) && (charge!=0)'), #Reduce pt threshold 
                          filter = cms.bool(True)
)

FourMuonsFilter = cms.EDFilter("CandViewCountFilter",
                             src = cms.InputTag("looseMuons"),
                             minNumber = cms.uint32(4),
                             #filter = cms.bool(True)
)


FourMuonsCand = cms.EDProducer("CandViewShallowCloneCombiner",
                         checkCharge = cms.bool(False),
                         #cut = cms.string('(mass < 10) && (mass >0.5)  && (abs(charge)=1) && (abs(daughter(0).vz - daughter(1).vz) < 1) && (abs(daughter(1).vz - daughter(2).vz) < 1) && (abs(daughter(0).vz - daughter(2).vz) < 1)'),
                         #cut = cms.string('(mass < 7) && (mass >4)  && (charge=0)'),
                         cut = cms.string('(mass < 10) && (mass >0.5)'), #Increse mass range before fit, accept also event with charge!=0
                         decay = cms.string("looseMuons looseMuons looseMuons looseMuons")
)

FourMuonsCandFilter = cms.EDFilter("CandViewCountFilter",
                                    src = cms.InputTag("FourMuonsCand"),
                                    minNumber = cms.uint32(1),
                                    #filter = cms.bool(True)
)

FourMuonsVtxKinFit = cms.EDProducer("KinematicVertexFitCompositeCandProducer",
                                     src = cms.InputTag("FourMuonsCand"),
                                     cut = cms.string('(mass < 7) && (mass >4)'), #Restore mass range after fit  
                                     )

FourMuonsVtxKalmanFit = cms.EDProducer("KalmanVertexFitCompositeCandProducer",
                                        src = cms.InputTag("FourMuonsCand"),
                                        #cut = cms.string('mass <5'),
                                        #cut = cms.string('(vertexChi2 < 40) && (vertexNdof == 3) && (mass <5)'),
                                        ##filter = cms.bool(True)
                                        )
#GoodFourMuonsVtxKalmanFit = cms.EDFilter("CompositeCandSelector",
#                                            src = cms.InputTag("FourMuonsVtxKalmanFit"),
#                                            cut = cms.string('(vertexChi2 < 40) && (vertexNdof == 3)'),
#                                            filter = cms.bool(True)
#                                            )

########################Define Histograms########################
InitialPlots = cms.EDAnalyzer('SimpleEventCounter',
                                   muonsInputTag = cms.InputTag("slimmedMuons"),
                                   )


PlotsAfterTrigger = cms.EDAnalyzer('RecoMuonAnalyzer',
                                   muonsInputTag = cms.InputTag("slimmedMuons"),
                                   )

PlotsAfterLooseMuon = cms.EDAnalyzer('RecoMuonAnalyzer',
                                   muonsInputTag = cms.InputTag("looseMuons"),
                                   )

PlotsAfter4Muons = cms.EDAnalyzer('RecoMuonAnalyzer',
                                   muonsInputTag = cms.InputTag("looseMuons"),
                                   )

PlotsAfterBCand = cms.EDAnalyzer('RecoMuonAnalyzer',
                                   muonsInputTag = cms.InputTag("looseMuons"),
                                   )


PlotsAfterBCandSel = cms.EDAnalyzer('RecoMuonAnalyzer',
                                   muonsInputTag = cms.InputTag("looseMuons"),
                                   )


FourMuonSelSeq = cms.Sequence(InitialPlots *
                               B4MuHLTFilter *
                               #PatMuons *
                               PlotsAfterTrigger *
                               looseMuons *
                               PlotsAfterLooseMuon *
                               FourMuonsFilter *
                               PlotsAfter4Muons *
                               FourMuonsCand *
                               FourMuonsCandFilter *
                               PlotsAfterBCand *
                               FourMuonsVtxKinFit
                               #GoodFourMuonsVtxKalmanFit *
                               #PlotsAfterBCandSel
                               )







