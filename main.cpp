#include <iostream>
#include <memory>
#include <algorithm>
#include <random>
#include<vector> // for vectors
#include<iterator> // for iterators
#include <bits/stdc++.h>
#include <fstream>

#include "BaselineSubtraction.h"
#include "iFIFOs.h"
#include "oFIFO.h"
#include "oFIFO.cpp"
#include "Encoder.h"

#include "Pulse.h"
#include "EBChannel.h"
#include "EBFrames.h"

#include <chrono>


#include "TH1I.h"
#include "TH1D.h"
#include "TFile.h"
#include <TF1.h>
#include <TRandom3.h>

using namespace std;

const bool dumpfile=0;
const int Datasize = 5e6; //number of samples: 1 day = 2.x10^13...
const int nbx=100;//Number of previous BX considered for the PU

const float pedestal = 20;
const float noise    = 10 ; // max noise 250 GeV at the end of HL-LHC = 5 adc counts.

const float adc2GeV  = 0.0488 ;// GeV/adc
const float adc2GeVdiv10  = 0.00488;// GeV/adc

const float eta = 1.4;
const float npu = 200.;
const float rophase = 1.0;
const float step = 6.25;
const float timestamp = 25.;




int main()
{
  ///////////////////////////////////////////////
  //MODULES
  unique_ptr<BaselineSubtraction> BS01 (new BaselineSubtraction());
  BS01->SetShift(0); ///shift gain10
  BS01->SetSubtractor(0); ///Baseline Subtraction

  unique_ptr<BaselineSubtraction> BS10 (new BaselineSubtraction());
  BS10->SetShift(0); ///shift gain10
  BS10->SetSubtractor(0); ///Baseline Subtraction


  unique_ptr<iFIFOs>  FIFOs (new iFIFOs());
  unique_ptr<Encoder> Enc  (new Encoder());
  oFIFO <uint32_t> StorageFifo(16);

  ///////////////////////////////////////////////
  //FILES:
  std::ofstream file ("emulation.dat", std::ofstream::out);

  ///////////////////////////////////////////////
  //PULSES:
  unique_ptr<Pulse> pulseS(new Pulse(0));
  unique_ptr<Pulse> pulseA(new Pulse(1));


  ////////////////////////////////////////////////
  //CMS:
  unique_ptr<EBChannel> ch (new EBChannel(eta));
  unique_ptr<EBFrames>  frame (new EBFrames());
  
  ////////////////////////////////////////////////
  ////CHRONO
  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::duration;
  using std::chrono::milliseconds;

  
  ////////////////////////////////////////////////
  ////ROOT
  unique_ptr<TH1I> FifoOccupancy(new TH1I("FifoOccupancy","FifoOccupancy",17,-0.5,16.5));
  unique_ptr<TH1D> hEnergy(new TH1D("hEnergy","hEnergy",2000,0,1000));
  unique_ptr<TRandom3> random(new TRandom3());
  unique_ptr<TRandom3> random1(new TRandom3());
  //////////////////////////////////////////////
  //Other variables
  int Data10   = 0;
  int Data01   = 0;
  unsigned int encdata = 0;
  bool Load = 0;
  unsigned int dout = 0;
  int nwords=0;
  unsigned int datatoenc=0;
  int baselineflag=-1;

  float tSample=0;
  float tS=0;
  float tA=0;
  float energy=0;
  int bxNow=0;
  int nOrbit= bxNow / 3564;
  int bxLocal= bxNow % 3564;
  int is=0;


  int NPU = 0;

  /////////////////////////////////////////////////
  //INIT OF FIRST BX:
  if(frame->bxIsFilled(bxLocal)){
    NPU = random1->Poisson(npu);
  }

  ch->GenerateBX(NPU);
  frame->pushBX(ch->energySignal(), 0., ch->energySpike(), ch->timeSpike());
  

  cout<<"Starting the loop over bx"<<endl;
  auto t1 = high_resolution_clock::now();
  
  //Every 4 samples I have a bunch crossing
  for(int isample = 0; isample < Datasize; ++isample) {
    if(isample%200000==0) printf("doing...percentage: %f\n",1.*isample/Datasize);

    energy=0;
    tSample = step * is;
    is++;
    for(int ibx = 0; ibx < nbx; ++ibx){
      tS = tSample + ((nbx-1) - ibx) * timestamp + rophase + frame->tSignal(ibx);
      if(tS>0){
	energy += frame->eSignal(ibx) * pulseS->Value(tS);
      }
      tA = tSample + ((nbx-1) - ibx) * timestamp + rophase + frame->tAPD(ibx);
      if(tA>0){
	energy += frame->eAPD(ibx) * pulseA->Value(tA);
      }
    }
    frame->pushSample(energy);
    //    cout<<"PU "<<NPU<<" energy "<<energy<<endl;
    hEnergy->Fill(energy);
    Data10  = (unsigned int)(energy/adc2GeV + random->Gaus(pedestal,noise));
    Data01  = (unsigned int)(energy/adc2GeVdiv10 + random->Gaus(pedestal,noise/10));

    if(Data10 <= 0) Data10 = pedestal;
    if(Data01 <= 0) Data01 = pedestal;
    
    //    if(isample > 4660 - 50 && isample < 4660 + 100) cout<<"sample"<<dec<<isample<<hex<<" Data10 "<<Data10<<" Data01 "<<Data01 <<endl;

    FIFOs->WriteH(Data10);
    FIFOs->WriteL(Data01);


    datatoenc=(unsigned int)FIFOs->Read();

    baselineflag=FIFOs->GetBaselineFlag();
    //if(isample > 4660 - 50 && isample < 4660 + 100) cout<<"Datatoenc "<<hex<<datatoenc<<hex<<" baselineflag "<<baselineflag <<endl;
   
    encdata = Enc->Encode(datatoenc,baselineflag,0);
    //    if(isample > 4660 - 50 && isample < 4660 + 100) cout<<"encdata "<<encdata <<endl;
    Load = Enc->Ready();
   
    if(Load==1 && StorageFifo.Size()!=15) {//FIFO full condition doesnt work
      nwords++;
      StorageFifo.Write(encdata);
    }


    if(nwords==50 && StorageFifo.Size()!=15) {//writeTrailer
      StorageFifo.Write(nwords|((unsigned int)0xd<<28));
      nwords=0;
    } 
   ///////////////////////////////////////////////////////////////////////
    if(isample%4==0) {//New BX
      is=0;
      FifoOccupancy->Fill(StorageFifo.Size());

      if(!StorageFifo.Empty()) {
	dout=StorageFifo.Read();
	if(dumpfile==1) file <<dec<< isample <<" "<< hex << dout << " "<<StorageFifo.Size()<<std::endl;
      }
      
      else {
	if(dumpfile==1) file <<dec<< isample << " eaaaaaaa" << std::endl;
	}

      if(isample!=0) {
	///////New Bunch crossing
	bxNow++;
	nOrbit  = bxNow / 3564;
	bxLocal = bxNow % 3564;
	
	if(frame->bxIsFilled(bxLocal)){
	  NPU = random1->Poisson(npu);
	}
	ch->GenerateBX(NPU);
	frame->pushBX(ch->energySignal(), 0., ch->energySpike(), ch->timeSpike());
      }
    }//end BX
  }//loop over samples

  
  auto t2 = high_resolution_clock::now();
  auto ms_int = duration_cast<milliseconds>(t2 - t1);

  std::cout <<dec<< ms_int.count() << "ms\n";   
  cout<<"samples read: "<<dec<<Datasize<<"("<<Datasize*0.00000000625<<" seconds)"<<endl;
  unique_ptr<TFile> rootfile (new TFile("emulator.root","recreate"));
  FifoOccupancy->Write();
  hEnergy->Write();
  return 0;
}
