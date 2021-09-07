#ifndef BASELINESUBTRACTION_H
#define BASELINESUBTRACTION_H


class BaselineSubtraction
{

 public:
  BaselineSubtraction();  
  //Destructor()
  ~BaselineSubtraction() {};

  // Member Functions()
  void SetShift(int);
  void SetSubtractor(int);
  int Shift(int);
  int Subtract(int);


 private:
  // Data Members
  int fsubtractor;
  int fshift;
};
 
#endif
