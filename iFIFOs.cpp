#include <iostream>
#include "iFIFOs.h"
using namespace std;
iFIFOs::iFIFOs(){
  //Constructor
  std::cout<<"Input FIFOs created\n";
  fwriteptr10=0;
  fwriteptr01=0;
  freadptr=0x6;
  frefptr=freadptr + (0x9-0x1);
  fwindow = 0x0;
  
  for(int i=0; i<FIFOsize; ++i) {
    fFIFO10[i]=0;
    fFIFO01[i]=0;
  }
}


void iFIFOs::WriteH(int data) {
  fFIFO10[fwriteptr10]=data;
  if(fwriteptr10 < FIFOsize - 1) fwriteptr10++;
  else(fwriteptr10=0);
}


void iFIFOs::WriteL(int data) {
  fFIFO01[fwriteptr01]=data;
  if(fwriteptr01 < FIFOsize - 1 ) fwriteptr01++;
  else(fwriteptr01=0);
}

bool iFIFOs::GetBaselineFlag(){
  return fbaselineflag;
}

int iFIFOs::Read() {
  int g10=fFIFO10[freadptr];
  int g01=fFIFO01[freadptr];
  int ref10=fFIFO10[frefptr];
  

  if(frefptr < FIFOsize - 1) frefptr++;
  else(frefptr=0);

  if(freadptr < FIFOsize - 1) freadptr++;
  else(freadptr=0);
  
  if(ref10 == 0xfff) {
    fwindow = 16;
  }

  if(fwindow!=0) {
    fwindow--;
    fbaselineflag=0;
    g01 = g01 | (1 << 12);
    return g01;
  }
  else {
    if(g10 > 63) {
      fbaselineflag=0;
    }
    else {
      fbaselineflag=1;
    }
    
    return g10; 


  }
  
}


void iFIFOs::Print(){

  for (int i =0; i < FIFOsize; ++i ) {


    cout<< "Entry: "<<i<<" Data gain10 "<<fFIFO10[i]<<endl;
    cout<< "Entry: "<<i<<" Data gain01 "<<fFIFO01[i]<<endl;

  }

}

