// -*- C++ -*-
//
// Package:    TrackFromCandProducer



// system include files
#include <memory>
#include <algorithm>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "TFile.h"
#include "TH1.h"
#include <vector>
#include <iostream>

//
// class declaration
//

class TrackFromCandProducer : public edm::stream::EDProducer<> {
   public:
      explicit TrackFromCandProducer(const edm::ParameterSet&);
      ~TrackFromCandProducer();

   private:
      virtual void beginStream(edm::StreamID) override;
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      virtual void endStream() override;

      // ----------member data ---------------------------
      typedef reco::Track Trk;
      typedef std::vector<Trk> TracksColl;
      edm::EDPutTokenT<TracksColl> TrkCollfin;
      edm::EDGetTokenT<std::vector<pat::PackedCandidate> >src_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
TrackFromCandProducer::TrackFromCandProducer(const edm::ParameterSet& iConfig)
{
    src_ = consumes<std::vector<pat::PackedCandidate> >(edm::InputTag("LooseTrack"));
    produces<TracksColl>("TrkCollfin");
    TrkCollfin =  produces< TracksColl >();
  
}


TrackFromCandProducer::~TrackFromCandProducer()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
void
TrackFromCandProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;
   using namespace reco;
   using std::vector;
    

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
   
   Handle<vector<pat::PackedCandidate> > PFCands;
   iEvent.getByToken(src_,PFCands);

   uint kk=0;
   TracksColl recoTrack;

   for (vector<pat::PackedCandidate>::const_iterator cand = PFCands->begin(); cand != PFCands->end(), kk!= PFCands->size(); ++cand, ++kk) {
       recoTrack.push_back( *(cand->bestTrack()) );
   }

    
    if( recoTrack.size()>0) cout<<"-----------Tracks collection size="<<recoTrack.size()<<endl;
    
    OrphanHandle< TracksColl > oh = iEvent.emplace(TrkCollfin, recoTrack);
    
} 


// ------------ method called once each stream before processing any runs, lumis or events  ------------
void
TrackFromCandProducer::beginStream(edm::StreamID)
{
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
TrackFromCandProducer::endStream() {
}


//define this as a plug-in
DEFINE_FWK_MODULE(TrackFromCandProducer);
