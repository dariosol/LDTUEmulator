#include "TRandom3.h"
#include "TH1.h"
#include "TH2.h"
#include "TRandom3.h"
#include "Riostream.h"
#include <algorithm>
class EBChannel {

 public:
  EBChannel(double);
  ~EBChannel();
  double energySignal()   { return energySignal_; };
  double energySpike()    { return energySpike_;  };
  double timeSpike()      { return timeSpike_;    };
  void   GenerateBX(int);

 private:
  TH1D  *hpdf_signal_;
  TH2D  *hpdf_spike_;
  double apd_probability_;
  double energySignal_;
  double energySpike_;
  double timeSpike_;
  TRandom r1;
  double rnd1, rnd2;  
};

