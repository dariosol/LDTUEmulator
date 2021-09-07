#ifndef ENCODER_H
#define ENCODER_H

const unsigned int BASELINEMASK  = 0x01   << 30;
const unsigned int BASELINEMASK1 = 0x2    << 30;
const unsigned int SIGNALMASK    = 0xA    << 26;
const unsigned int SIGNALMASK1   = 0xB    << 26;
const unsigned int SYNCHMASK     = 0x1554000;


enum State {
  baseline,
  baseline1,
  header,
  sign,
  sign1    
};
  
    
class Encoder
{

 public:
  Encoder();  
  //Destructor()
  ~Encoder() {};

  // Member Functions()
  unsigned int Encode(unsigned int, bool, bool);
  bool Ready();
  
 private:
  // Data Members
  unsigned int fencodeddata;
  int fbsnsample;
  int fsignsample;
  int foutput;
  int fnwords;
  bool fready;
  State nextstate;
};
 
#endif
