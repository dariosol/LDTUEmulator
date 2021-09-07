#ifndef IFIFOS_H
#define IFIFOS_H

const int FIFOsize=16;

class iFIFOs
{

 public:
  iFIFOs();  
  //Destructor()
  ~iFIFOs() {};

  // Member Functions()
  void WriteH(int);
  void WriteL(int);
  bool GetBaselineFlag();
  int Read();
  void Print();
 private:
  // Data Members
  int fFIFO10[FIFOsize];
  int fFIFO01[FIFOsize];
  int fwriteptr10;
  int fwriteptr01;
  int freadptr;
  int frefptr;
  int fwindow;
  bool fbaselineflag;
};
 
#endif
