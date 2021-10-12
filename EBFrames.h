#include <TRandom3.h>


const int    nbxmax_ = 3564;
const int    nBXs_ = 100;
const double tStep_ = 25./4.;
const int    nSamples_ = 4;//was 20

class EBFrames{

 public:
  EBFrames();
  ~EBFrames();
  bool bxIsFilled(int i)  { return orbitScheme_[i]; };
  double eSignal(int i)   { return eSignal_[i];     };
  double tSignal(int i)   { return tSignal_[i];     };
  double eAPD(int i)      { return eAPD_[i];        };
  double tAPD(int i)      { return tAPD_[i];        };
  double eSample(int i)   { return eSample_[i];     };
  void   pushBX(double,double,double,double);
  void   pushSample(double);

 private:
  bool         orbitScheme_[nbxmax_];
  double       eSignal_[nBXs_];
  double       tSignal_[nBXs_];
  double       eAPD_[nBXs_];
  double       tAPD_[nBXs_];
  double       eSample_[nSamples_];
};
