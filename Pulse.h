#include <TRandom3.h>
#include <TMath.h>
#include <TFile.h>
#include <TH1.h>
#include <TF1.h>
#include <TGraph.h>
#include <TTree.h>
#include <TGraph.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>


class Pulse{

 public:
  Pulse(int);
  ~Pulse();
  double norm()  const { return norm_; };
  void SetNorm(double x)   { norm_ = x; };
  double Value(double);


 private:
  TGraph *grPulse_;
  TF1    *fPulse_;
  double tMin_;
  double tLimit_;
  double tMax_;
  double norm_;
  double offset_;
};



Pulse::Pulse(int iopt)
{
  TFile *file  = new TFile("TIA_ASIC_signal.root");
  TFile *file2 = new TFile("TIA_ASIC_signal_v2.root");
  if(iopt==0){
    grPulse_ = (TGraph*)file->Get("grTIA_Signal");
    fPulse_ = new TF1("fPulse_","[0]*exp(-x/[1])",0.,1e+6);
    fPulse_->SetParameters(2.21312e-05,1e+2);
    tMin_ = -0.4;
    tLimit_ = 200.;
    tMax_ = 1e+5;
    norm_ = 164.844;
    offset_ = 0;
  }else if(iopt==1){
    grPulse_ = (TGraph*)file->Get("grTIA_Spike");
    fPulse_ = new TF1("fPulse_","[0]*exp(-pow((x-[1])/[2],2))",0.,1e+6);
    fPulse_->SetParameters(3.79488e-01,5.44116e+01,9.62779);
    tMin_ = -0.4;
    tLimit_ = 51.3750;
    tMax_ = 1e+5;
    norm_ = 0.0235474;
    offset_ = +0.95;
  }else{
    grPulse_ = (TGraph*)file2->Get("grTIA_Signal_v2");
    fPulse_ = new TF1("fPulse_","[0]*exp(-x/[1])+[2]*exp(-x/[3])",0.,1e+6);
    fPulse_->SetParameters(1.65336e+05, 5.18932e+00, 4.33376e-02, 1.00000e+02);
    tMin_ =  0.0;
    tLimit_ = 50.0;
    tMax_ = 1e+5;
    //    norm_ = 1;
    norm_ = 1./503.888;
    offset_ = 0;
  }
  file->Close();
  file2->Close();
}



Pulse::~Pulse()
{
  delete fPulse_;
}




double Pulse::Value(double t)
{
  if(t < tMin_){
    return 0.;
  }else if(t < tLimit_){
    return grPulse_->Eval(t + offset_) * norm_;
  }else if(t < tMax_){
    return fPulse_->Eval(t + offset_) * norm_;
  }else{
    return 0.;
  }
}
