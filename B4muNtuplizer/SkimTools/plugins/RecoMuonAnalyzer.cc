
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

class RecoMuonAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit RecoMuonAnalyzer(const edm::ParameterSet&);
      ~RecoMuonAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      edm::InputTag muonsInputTag;
    
      // ----------member data ---------------------------

    edm::EDGetTokenT< std::vector<pat::Muon> > muons_;
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
RecoMuonAnalyzer::RecoMuonAnalyzer(const edm::ParameterSet& iConfig):
muons_(consumes< std::vector<pat::Muon> >(iConfig.getParameter< edm::InputTag >("muonsInputTag")))
{
   //now do what ever initialization is needed
  //   usesResource("TFileService");

}


RecoMuonAnalyzer::~RecoMuonAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
RecoMuonAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;
   Handle<std::vector<pat::Muon>> muons;
   iEvent.getByToken(muons_,  muons);
   //edm::Handle< edm::View<pat::Muon> > muons;
   //iEvent.getByToken(muons_, muons);
    
    hEvtCount->Fill(1);
    hMuonMult->Fill( muons->size());
    //cout<<"N muons="<< muons->size()<<endl;
    TLorentzVector mu1, mu2, Zmu1mu2;     
    TLorentzVector sel_mu1, sel_mu2, sel_Zmu1mu2;
    std::vector<double>    ZMass_skim;
    for (std::vector<pat::Muon>::const_iterator muIt = muons->begin(); muIt!= muons->end(); ++muIt){
      //cout<<" muon1 pt="<<muIt->pt()<<" is global="<<muIt->isGlobalMuon()<<endl;
      //      if ( muIt->innerTrack().isAvailable() )      cout<<" #validHits="<<muIt->innerTrack()->hitPattern().numberOfValidHits()<<endl;
      mu1.SetPtEtaPhiE(muIt->pt(), muIt->eta(), muIt->phi(), muIt->energy());
      hMuonPt->Fill(muIt->pt());
      hMuonEta->Fill(muIt->eta());
      hMuonIsGlobal->Fill(muIt->isGlobalMuon());
      hMuonIsTracker->Fill(muIt->isTrackerMuon());
      hRelIso03->Fill(muIt->isolationR03().sumPt/muIt->pt());
      
      if ( muIt->innerTrack().isAvailable() ){
        hMuonDxy->Fill(muIt->innerTrack()->dxy());
	hMuonNumberOfValidHits->Fill(muIt->innerTrack()->hitPattern().numberOfValidHits());
      }
      if ( muIt->globalTrack().isAvailable() ){
        hMuonTrackNormChi2->Fill(muIt->globalTrack()->normalizedChi2());
      }

      if(1/*muIt->pt()>5 && fabs(muIt->eta())<2.4*/) {
	sel_mu1.SetPtEtaPhiE(muIt->pt(), muIt->eta(), muIt->phi(), muIt->energy()); 
	for (std::vector<pat::Muon>::const_iterator mu2It = muIt+1; mu2It!= muons->end(); ++mu2It){
	  //	  cout<<" --muon2 pt="<<mu2It->pt()<<endl;
	  mu2.SetPtEtaPhiE(mu2It->pt(), mu2It->eta(), mu2It->phi(), mu2It->energy());
	  Zmu1mu2 = mu1+mu2;
	  hZMass->Fill(Zmu1mu2.M());
	  if(1 /*(mu2It->pt()>25) && (fabs(mu2It->eta())<2.4) && (mu2It->isGlobalMuon()==1) && (mu2It->isTrackerMuon()==1)   
	     && (mu2It->globalTrack()->normalizedChi2()<10) && (mu2It->innerTrack()->hitPattern().numberOfValidHits()>10)*/
	     /* && ((mu2It->isolationR03().sumPt/mu2It->pt())<0.4) && fabs(mu2It->innerTrack()->dxy())<2.0*/ ) {
	    sel_mu2.SetPtEtaPhiE(mu2It->pt(), mu2It->eta(), mu2It->phi(), mu2It->energy()); 
	    sel_Zmu1mu2 = sel_mu1+sel_mu2;
	    
	    //cout<<" zmass="<<sel_Zmu1mu2.M()<<" pt mu1="<<sel_mu1.Pt()<<" pt mu2="<<sel_mu2.Pt()<<endl;
	      if((sel_Zmu1mu2.M()<120) && (sel_Zmu1mu2.M()>60) && ( (muIt->charge()+mu2It->charge())==0) ){
		hZMass_skim->Fill(sel_Zmu1mu2.M());
		ZMass_skim.push_back(sel_Zmu1mu2.M());
	      }

	    }
	  }
	}

    }
    if(ZMass_skim.size()>0) hEvtCount_skim->Fill(1);
    
}


// ------------ method called once each job just before starting event loop  ------------
void 
RecoMuonAnalyzer::beginJob()
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
RecoMuonAnalyzer::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
RecoMuonAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(RecoMuonAnalyzer);
