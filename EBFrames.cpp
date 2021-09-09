#include "EBFrames.h"

EBFrames::EBFrames()
{
  // Filling scheme:
  // 3564 = 12 x 297 = ((81b + 8e) x 3 + 30e) x 11 + ((81b + 8e) x 2 + 119e)

  for(int i=0; i<nbxmax_; ++i){
    orbitScheme_[i] = false;
  }
  int idFilled = 0;
  int idAll    = 0;
  for(int itrain=0; itrain<11; ++itrain){
    for(int igroup=0; igroup<3; ++igroup){
      for(int ib=0; ib<81; ib++){
	orbitScheme_[idAll] = true;
	idFilled++;
	idAll++;
      }
      for(int ib=0; ib<8; ib++){
	idAll++;
      }
    }
    for(int ib=0; ib<30; ib++){
      idAll++;
    }
  }
  for(int igroup=0; igroup<2; ++igroup){
    for(int ib=0; ib<81; ib++){
      orbitScheme_[idAll] = true;
      idFilled++;
      idAll++;
    }
    for(int ib=0; ib<8; ++ib){
      idAll++;
    }
  }
  for(int ib=0; ib<119; ++ib){
    idAll++;
  }

  for(int i=0; i<nBXs_; ++i){
    eSignal_[i] = 0;
    tSignal_[i] = 0;
    eAPD_[i] = 0;
    tAPD_[i] = 0;
  }

  for(int i=0; i<nSamples_; ++i){
    eSample_[i] = 0;
  }
}



EBFrames::~EBFrames()
{
}



void EBFrames::pushBX(double eS=0, double tS=0, double eA=0, double tA=0)
{
  for(int i=1; i<nBXs_; ++i){
    eSignal_[i-1] = eSignal_[i];
    tSignal_[i-1] = tSignal_[i];
    eAPD_[i-1] = eAPD_[i];
    tAPD_[i-1] = tAPD_[i];
  }
  eSignal_[nBXs_-1] = eS;
  tSignal_[nBXs_-1] = tS;
  eAPD_[nBXs_-1] = eA;
  tAPD_[nBXs_-1] = tA;
}



void EBFrames::pushSample(double e=0)
{
  for(int i=1; i<nSamples_; ++i){
    eSample_[i-1] = eSample_[i];
  }
  eSample_[nSamples_-1] = e;
}
