// -*- C++ -*-
//
// Package:    PFCandFilter/PFCandFilter
// Class:      PFCandFilter
// 
/**\class PFCandFilter PFCandFilter.cc PFCandFilter/PFCandFilter/plugins/PFCandFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  venditti
//         Created:  Tue, 19 May 2020 09:37:20 GMT
//
//


// system include files
#include <memory>
#include <algorithm>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
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

//
// class declaration
//

class PFCandFilter : public edm::stream::EDFilter<> {
   public:
      explicit PFCandFilter(const edm::ParameterSet&);
      ~PFCandFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginStream(edm::StreamID) override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endStream() override;
      edm::EDGetTokenT<std::vector<pat::PackedCandidate> >srcCands_;
      edm::EDPutTokenT<std::vector<pat::PackedCandidate> > MyPFCands2;
      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
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
PFCandFilter::PFCandFilter(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
  srcCands_ = consumes<std::vector<pat::PackedCandidate> >(edm::InputTag("packedPFCandidates"));
  produces<std::vector< pat::PackedCandidate> >("MyPFCands").setBranchAlias("MyPFCands");
  produces<std::vector<pat::PackedCandidate> >("MyPFCands2");
  MyPFCands2 =  produces<std::vector<pat::PackedCandidate> >();
    // ptokenPuppiCandidates_ = produces<reco::PFCandidateCollection>();
}


PFCandFilter::~PFCandFilter()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
PFCandFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;
   using namespace reco;
   using std::vector;
#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
   edm::Handle<std::vector<pat::PackedCandidate> > PFCands;
   iEvent.getByToken(srcCands_,PFCands);
   bool myflt = false;
   std::unique_ptr< vector<pat::PackedCandidate> > MyPFCands(new vector<pat::PackedCandidate> );
   uint kk=0;
   vector<pat::PackedCandidate> theCand;
   //cout<<" STO QUA "<<endl;
   for (std::vector<pat::PackedCandidate>::const_iterator cand = PFCands->begin(); cand != PFCands->end(), kk!= PFCands->size(); ++cand, ++kk) {
     //cout<<kk<<" PFCand: pt="<<cand->pt()<<" fabs_eta"<<fabs(cand->eta())<<" vtx ref "<<cand->vertexRef().isNull()<<" q="<<cand->charge()<<endl;

     //if(cand->charge()!=0 && cand->vertexRef().isNull()  && cand->pt() > 2 && fabs(cand->eta())<2.4 && cand->trackerLayersWithMeasurement()>5 &&  cand->pixelLayersWithMeasurement()>0 ) {
     if(cand->pt() > 2 && fabs(cand->eta())<2.4 && cand->charge()!=0 && cand->hasTrackDetails()!=0 && cand->trackerLayersWithMeasurement()>5 && cand->pixelLayersWithMeasurement()>=1) {
       MyPFCands->push_back(*cand);
       theCand.push_back(*cand);
       //selectedPFCand->push_back(*cand);

     }
   }

   //for (std::vector<pat::PackedCandidate>::const_iterator c = theCand.begin(); c != theCand.end(); ++c) {
     //cout<<" cand pt="<<c->pt()<<endl;}
   if( MyPFCands->size()>0) myflt=true;
   //   if( MyPFCands->size()>0) iEvent.put(std::move(MyPFCands));
   //if( MyPFCands->size()>0) iEvent.emplace(MyPFCands2, std::move(theCand));
   if( MyPFCands->size()>0)edm::OrphanHandle< vector<pat::PackedCandidate> > oh = iEvent.emplace(MyPFCands2, theCand);
   if( MyPFCands->size()>0) cout<<"-----------Good Evt------------ size="<<MyPFCands->size()<<endl;
   return myflt;
			     
} 

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void
PFCandFilter::beginStream(edm::StreamID)
{
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
PFCandFilter::endStream() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
PFCandFilter::beginRun(edm::Run const&, edm::EventSetup const&)
{ 
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
PFCandFilter::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
PFCandFilter::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
PFCandFilter::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PFCandFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(PFCandFilter);
