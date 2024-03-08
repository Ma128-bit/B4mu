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


looseMuons = cms.EDFilter("PATMuonSelector",
                          src = cms.InputTag("slimmedMuons"),
                          cut = cms.string('pt > 2 &&  abs(eta)<2.4 && (innerTrack().isNonnull)'),
                          filter = cms.bool(True)
)

TwoMuonsFilter = cms.EDFilter("CandViewCountFilter",
                             src = cms.InputTag("looseMuons"),
                             minNumber = cms.uint32(2),
                             #filter = cms.bool(True)
)

DiMuonCand  = cms.EDProducer("CandViewShallowCloneCombiner",
                             checkCharge = cms.bool(False),
                             cut = cms.string('(abs(charge)=0) && (mass < 3) && (mass >2)'),
                             decay = cms.string("looseMuons looseMuons")
)

DiMuonCandFilter = cms.EDFilter("CandViewCountFilter",
                                src = cms.InputTag("DiMuonCand"),
                                minNumber = cms.uint32(1),
)

LooseTrack = cms.EDFilter("PFCandFilter",
                          src = cms.InputTag("packedPFCandidates"),
                          cut = cms.string("pt > 2 &&  abs(eta)<2.4 &&  (charge!=0) && hasTrackDetails() && trackerLayersWithMeasurement()>5 && pixelLayersWithMeasurement()>=1"),
                          filter = cms.bool(True)                                
)

TwoTracksFilter  = cms.EDFilter("CandViewCountFilter",
                               src = cms.InputTag("LooseTrack"),
                               minNumber = cms.uint32(2),
)

LooseTrackCandidate = cms.EDProducer("TrackFromCandProducer",
				src = cms.InputTag("LooseTrack")
)

RecoTrackCand = cms.EDProducer("ConcreteChargedCandidateProducer",
                                src = cms.InputTag("LooseTrackCandidate"),
                                #particleType = cms.string("K+"),
)

TwoMuonsTwoTracksCand = cms.EDProducer("CandViewShallowCloneCombiner",
                                      checkCharge = cms.bool(False),
                                      cut = cms.string(' (abs(charge)=0) && ((daughter(0).charge+daughter(1).charge)==0) && (daughter(0).eta!=daughter(1).eta) && (daughter(2).eta!=daughter(1).eta) && (daughter(2).eta!=daughter(0).eta) && (daughter(3).eta!=daughter(0).eta) && (daughter(3).eta!=daughter(1).eta) && (daughter(3).eta!=daughter(2).eta) && (mass < 7) && (mass > 4)'),
                                      decay = cms.string("looseMuons looseMuons LooseTrackCandidate LooseTrackCandidate")
)

TwoMuonsTwoTracksCandFilter = cms.EDFilter("CandViewCountFilter",
                                    src = cms.InputTag("TwoMuonsTwoTracksCand"),
                                    minNumber = cms.uint32(1),
)

TwoMuonsTwoTracksKalmanVtxFit = cms.EDProducer("KalmanVertexFitCompositeCandProducer",
                                              src = cms.InputTag("TwoMuonsTwoTracksCand")
                                              #cut = cms.string('mass <5'),                          
)                                        

########################Define Histograms########################
InitialPlots = cms.EDAnalyzer('SimpleEventCounter',
                                   muonsInputTag = cms.InputTag("slimmedMuons"),
                                   )

PlotsAfterTrigger = cms.EDAnalyzer('RecoMuonAnalyzer',
                                   muonsInputTag = cms.InputTag("slimmedMuons"),
                                   )

PlotsAfterOnePFCand = cms.EDAnalyzer('RecoMuonAnalyzer',
                                     muonsInputTag = cms.InputTag("slimmedMuons"),
                                 )

PlotsAfterLooseMuon = cms.EDAnalyzer('RecoMuonAnalyzer',
                                   muonsInputTag = cms.InputTag("looseMuons"),
                                   )

PlotsAfterDiMuonCand = cms.EDAnalyzer('RecoMuonAnalyzer',
                                     muonsInputTag = cms.InputTag("looseMuons"),
                                     )

PlotsAfterTracksFilter = cms.EDAnalyzer('RecoMuonAnalyzer',
                                     muonsInputTag = cms.InputTag("looseMuons"),
                                     )

PlotsAfterJPsiKKCandSel = cms.EDAnalyzer('RecoMuonAnalyzer',
                                   muonsInputTag = cms.InputTag("looseMuons"),
                                   )




TwoMuTwoTracksSelSeq = cms.Sequence(InitialPlots *
                               B4MuHLTFilter *
                               PlotsAfterTrigger *
                               looseMuons *
                               PlotsAfterLooseMuon *
                               TwoMuonsFilter *
                               DiMuonCand *
                               DiMuonCandFilter *
                               PlotsAfterDiMuonCand *
                               LooseTrack *
                               TwoTracksFilter *
			       PlotsAfterTracksFilter *
                               LooseTrackCandidate *
                               #RecoTrackCand *
                               TwoMuonsTwoTracksCand *
                               TwoMuonsTwoTracksCandFilter *
                               TwoMuonsTwoTracksKalmanVtxFit *
                               PlotsAfterJPsiKKCandSel
                               )







