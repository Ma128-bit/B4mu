/** \class cand::modules::CandReducer
 *
 * Given a collectin of candidates, produced a
 * collection of LeafCandidas identical to the
 * source collection, but removing all daughters
 * and all components.
 *
 * This is ment to produce a "light" collection
 * of candiadates just containing kimenatics
 * information for very fast analysis purpose
 *
 * \author Luca Lista, INFN
 *
 * \version $Revision: 1.3 $
 *
 * $Id: CandReducer.cc,v 1.3 2009/09/27 22:26:55 hegner Exp $
 *
 */
#include "FWCore/Framework/interface/global/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Candidate/interface/Candidate.h"

class CandReducer : public edm::global::EDProducer<> {
public:
  /// constructor from parameter set
  explicit CandReducer(const edm::ParameterSet&);
  /// destructor
  ~CandReducer() override = default;

private:
  /// process one evevnt
  void produce(edm::StreamID, edm::Event& e, edm::EventSetup const& c) const override;
  /// label of source candidate collection
  const edm::EDGetTokenT<reco::CandidateView> srcToken_;
};

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "FWCore/Utilities/interface/EDMException.h"

using namespace reco;
using namespace edm;

CandReducer::CandReducer(const edm::ParameterSet& cfg)
    : srcToken_(consumes<reco::CandidateView>(cfg.getParameter<edm::InputTag>("src"))) {
  produces<CandidateCollection>();
}

void CandReducer::produce(edm::StreamID, edm::Event& evt, edm::EventSetup const&) const {
  const Handle<reco::CandidateView> cands = evt.getHandle(srcToken_);
  std::unique_ptr<CandidateCollection> comp(new CandidateCollection);
  for (reco::CandidateView::const_iterator c = cands->begin(); c != cands->end(); ++c) {
    std::unique_ptr<Candidate> cand(new LeafCandidate(*c));
    comp->push_back(cand.release());
  }
  evt.put(std::move(comp));
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(CandReducer);
