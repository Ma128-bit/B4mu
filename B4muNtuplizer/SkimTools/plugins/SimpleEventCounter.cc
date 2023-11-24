
// -*- C++ -*-
//
// Package:    RecoMuonAnalyzer/RecoMuonAnalyzer
// Class:      RecoMuonAnalyzer
// 
/**\class RecoMuonAnalyzer RecoMuonAnalyzer.cc RecoMuonAnalyzer/RecoMuonAnalyzer/plugins/RecoMuonAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  venditti
//         Created:  Tue, 03 Jan 2017 15:52:05 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include <DataFormats/MuonReco/interface/Muon.h>
#include <DataFormats/MuonReco/interface/MuonFwd.h>
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include <memory>
#include <vector>
#include <cmath>
#include "TLorentzVector.h"

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.
using namespace edm;

class SimpleEventCounter: public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit SimpleEventCounter(const edm::ParameterSet&);
      ~SimpleEventCounter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
  edm::EDGetTokenT<edm::View<pat::Muon> > muons_;
  edm::InputTag muonsInputTag;
    
      // ----------member data ---------------------------


    edm::Service<TFileService> fs;
    
  TH1F * hEvtCount;
  TH1F *   hEvtCount_skim;
  TH1F * hMuonMult;
  TH1F *  hMuonPt;
  TH1F *  hMuonEta;
  TH1F *  hMuonIsGlobal;
  TH1F *  hMuonIsTracker;
  TH1F *  hMuonDxy;
  TH1F *  hMuonTrackNormChi2;
  TH1F *  hMuonNumberOfValidHits;
  TH1F *  hRelIso03;
  TH1F *  hZMass;
  TH1F *  hZMass_skim;
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

  SimpleEventCounter::SimpleEventCounter(const edm::ParameterSet& iConfig){
    //muons_(consumes< std::vector<reco::Muon> >(iConfig.getParameter< edm::InputTag >("muonsInputTag")))

    muons_ = consumes<edm::View<pat::Muon> >  (iConfig.getParameter<edm::InputTag>("muonsInputTag"));
    //now do what ever initialization is needed
    //   usesResource("TFileService");

  }


SimpleEventCounter::~SimpleEventCounter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
SimpleEventCounter::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;
   //Handle<std::vector<reco::Muon>> muons;
   //iEvent.getByToken(muons_,  muons);
   edm::Handle< edm::View<pat::Muon> > muons;
   iEvent.getByToken(muons_, muons);
    
    hEvtCount->Fill(1);
    hMuonMult->Fill( muons->size());
    //cout<<"N muons="<< muons->size()<<endl;
    for(edm::View<pat::Muon>::const_iterator muIt=muons->begin(); muIt!=muons->end(); ++muIt){
      //    for (std::vector<reco::Muon>::const_iterator muIt = muons->begin(); muIt!= muons->end(); ++muIt){
      hMuonPt->Fill(muIt->pt());
      hMuonEta->Fill(muIt->eta());
      hMuonIsGlobal->Fill(muIt->isGlobalMuon());
      hMuonIsTracker->Fill(muIt->isTrackerMuon());

    }

    
}


// ------------ method called once each job just before starting event loop  ------------
void 
SimpleEventCounter::beginJob()
{
  hEvtCount = fs->make<TH1F>("hEvtCount","hEvtCount",10,0,10);
  hEvtCount_skim = fs->make<TH1F>("hEvtCount_skim","hEvtCount_skim",10,0,10);
    hMuonMult = fs->make<TH1F>("hMuonMult","hMuonMult",50,0,50);
    hMuonPt = fs->make<TH1F>("hMuonPt","hMuonPt",500,0,500);
    hMuonEta = fs->make<TH1F>("hMuonEta","hMuonEta",500,-2.5,2.5);
    hMuonIsGlobal =  fs->make<TH1F>("hMuonIsGlobal","hMuonIsGlobal",2,0,2);
    hMuonIsTracker =  fs->make<TH1F>("hMuonIsTracker","hMuonIsTracker",2,0,2);
    hMuonDxy =  fs->make<TH1F>("hMuonDxy","hMuonDxy",800,-4,4);
    hMuonTrackNormChi2 =  fs->make<TH1F>("hMuonTrackNormChi2","hMuonTrackNormChi2",200,0,100);
    hMuonNumberOfValidHits=  fs->make<TH1F>("hMuonNumberOfValidHits","hMuonNumberOfValidHits",100,0,100);
    hRelIso03=  fs->make<TH1F>("hRelIso03","hRelIso03",200,0,100);
    hZMass = fs->make<TH1F>("hZMass","hZMass",300,0,300);
    hZMass_skim = fs->make<TH1F>("hZMass_skim","hZMass_skim",300,0,300);

}

// ------------ method called once each job just after ending the event loop  ------------
void 
SimpleEventCounter::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
SimpleEventCounter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(SimpleEventCounter);
