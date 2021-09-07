#include "EBChannel.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TGraphAsymmErrors.h"
#include "TRandom3.h"
#include "Riostream.h"
#include <algorithm>

EBChannel::EBChannel(double eta)
{
  char hname[120];
  int indx;

  indx = int(fabs(eta-0.05) * 10.0);
  if(indx > 13) indx = 13;
  if(indx < 0)  indx = 0;
  sprintf(hname,"pupdf_%d",indx);
  TFile *fin1 = new TFile("pileupPDFs.root");
  hpdf_signal_ = (TH1D*)fin1->Get(hname);
  hpdf_signal_->SetDirectory(0);
  fin1->Close();

  TFile *fin2 = new TFile("pdf_apd_1M.root");
  TProfile *hp = (TProfile*)fin2->Get("h01");
  indx = int(fabs(eta)/1.5 * 85.) + 1;
  if(indx < 1)  indx = 1;
  if(indx > 85) indx = 85;
  apd_probability_ = hp->GetBinContent(indx);
  hpdf_spike_ = (TH2D*)fin2->Get("h03");
  hpdf_spike_->SetDirectory(0);
  fin2->Close();

  energySignal_ = 0;
  energySpike_  = 0;
  timeSpike_    = 0;
}



EBChannel::~EBChannel()
{
  delete hpdf_signal_;
  delete hpdf_spike_;
}

void EBChannel::GenerateBX(int NPU)
{
  energySignal_ = 0;
  energySpike_  = 0;
  timeSpike_  = 0;


  for(int i=0; i<NPU; ++i){
    rnd1 = hpdf_signal_->GetRandom();
    if(rnd1 > -10.9){
      energySignal_ += pow(10, rnd1);
    }
  }

  if( r1.Rndm() < (NPU * apd_probability_) ){
    hpdf_spike_->GetRandom2(rnd1,rnd2);
    energySpike_ += pow(10, rnd1) * 1.476;    // factor 1.476 accounts for pulse shaping by CATIA vs legacy FE
    if(rnd2 < 0)   rnd2 += 25.;
    if(rnd2 > 25.) rnd2 -= 25.;
    timeSpike_ = rnd2;
    timeSpike_ -= 9.3; // offset wrt prompt signal
  }
}
