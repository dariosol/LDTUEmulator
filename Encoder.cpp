#include <iostream>
#include "Encoder.h"
using namespace std;

Encoder::Encoder(){
  cout<<"Module Encoder Generated "<<endl;
  fencodeddata=0;
  fbsnsample=0;
  fsignsample=0;
  foutput=0;
  fnwords=0;
  fready=0;
  nextstate=baseline;
}

  
bool Encoder::Ready() {
  return fready;
}

unsigned int Encoder::Encode(unsigned int data,bool baselineflag,bool orbit) {
  //cout<<"ENCODER"<<endl;
  //cout<<"Data in: "<<hex<<data<<" baseflag:" <<baselineflag<<endl;

  switch (nextstate) {
    /////////////////////////////////////////////////////////
  case baseline: {  

    if(baselineflag==1 && orbit==0) {
      fencodeddata = fencodeddata | ((unsigned int)data<<(6*fbsnsample));
      //cout<<"BASELINE"<<endl;
      fsignsample=0;
      fready=0;
      fbsnsample++;
      if(fbsnsample==5) {
	foutput = BASELINEMASK | fencodeddata;
	fready=1;
	fnwords++;
	fbsnsample=0;
	fencodeddata=0;
      }
      nextstate = baseline;
      break;
    }


    
    else if(baselineflag==0 && orbit ==0 && fbsnsample<5) {      
      //cout<<"Baseline1 but going into signal: "<<dec<<fbsnsample<<endl;
      if(fbsnsample!=0) {
	foutput = BASELINEMASK1 | ((fbsnsample&0x3f)<<24) | fencodeddata; //close old data
	fready=1;
      }
      else {
	fready=0;
      }
      fnwords++;
      fsignsample++;
      fbsnsample=0;
      fencodeddata = data;
      nextstate = sign;
      break;
    }


    
    else if(baselineflag==0 && orbit ==0 && fbsnsample==5) {
      //cout<<"BASELINE completed, go into signal"<<dec<<fbsnsample<<endl;
      foutput = (BASELINEMASK | fencodeddata); //close old data
      fready=1;
      fnwords++;
      fbsnsample=0;
      fencodeddata = data;
      fsignsample++;
      nextstate = sign;
      break;
    }


    
    else if(orbit ==1) {
      //cout<<"BASELINE"<<endl;
      foutput = BASELINEMASK1 | (fbsnsample&0xf) | fencodeddata;
      fready=1;
      fnwords++;
      fbsnsample=0;
      fencodeddata = data;
      nextstate = header;
      break;
    }
    

  }
    /////////////////////////////////////////////////////////////////
  case sign: {
    if(baselineflag==0 && orbit ==0) {
      //cout<<"SIGNAL to signal"<<endl;
      fencodeddata = (fencodeddata | (data <<(13*(unsigned int)fsignsample)));
      fsignsample++;
      fbsnsample=0;
      fready=0;
      nextstate=sign;
      if(fsignsample==2) {
	//cout<<"RESET signsample"<<endl;
	foutput = SIGNALMASK | fencodeddata;
	fready=1;
	fnwords++;
	fsignsample=0;
	fencodeddata=0;
      }
      break;
    }

    if(baselineflag==1 && orbit ==0 && fsignsample==1) {
      //cout<<"SIGNAL but going to baseline"<<endl;
      foutput = SIGNALMASK1 | SYNCHMASK | fencodeddata;//close data and go to baseline
      fready=1;
      fnwords++;
      fsignsample=0;
      fencodeddata = ((unsigned int)data<<(6*fbsnsample));
      fbsnsample++;
      nextstate=baseline;
      break;
    }

    if(baselineflag==1 && orbit ==0 && fsignsample==0) {
      //cout<<"SIGNAL but going to baseline WITHOUT ANY OUTPUT"<<endl;
      fencodeddata = ((unsigned int)data<<(6*fbsnsample));
      fready=0;
      fbsnsample++;
      nextstate=baseline;
      break;
    }
    
  }

  }
  //cout<<"encodeddata "<<hex<<fencodeddata<<endl;
  //cout<<"output: "<<hex<<foutput<<endl;
  
  return foutput;
}

