#ifndef DQM_SIPIXELHETEROGENEOUS_SIPIXELHETEROGENEOUSDQMUTIL_H
#define DQM_SIPIXELHETEROGENEOUS_SIPIXELHETEROGENEOUSDQMUTIL_H
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/Core/interface/DQMStore.h"

namespace siPixelHeteroDQMUtil {
  // same logic used for the MTV:
  // cf https://github.com/cms-sw/cmssw/blob/master/Validation/RecoTrack/src/MTVHistoProducerAlgoForTracker.cc
  typedef dqm::reco::DQMStore DQMStore;

  inline void setBinLog(TAxis* axis) {
    int bins = axis->GetNbins();
    float from = axis->GetXmin();
    float to = axis->GetXmax();
    float width = (to - from) / bins;
    std::vector<float> new_bins(bins + 1, 0);
    for (int i = 0; i <= bins; i++) {
      new_bins[i] = TMath::Power(10, from + i * width);
    }
    axis->Set(bins, new_bins.data());
  }

  inline void setBinLogX(TH1* h) {
    TAxis* axis = h->GetXaxis();
    setBinLog(axis);
  }
  inline void setBinLogY(TH1* h) {
    TAxis* axis = h->GetYaxis();
    setBinLog(axis);
  }

  template <typename... Args>
  dqm::reco::MonitorElement* make2DIfLog(DQMStore::IBooker& ibook, bool logx, bool logy, Args&&... args) {
    auto h = std::make_unique<TH2I>(std::forward<Args>(args)...);
    if (logx)
      setBinLogX(h.get());
    if (logy)
      setBinLogY(h.get());
    const auto& name = h->GetName();
    return ibook.book2I(name, h.release());
  }
} // namespace siPixelHeteroDQMUtil
#endif
