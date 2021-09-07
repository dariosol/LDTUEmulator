#include <iostream>
#include "BaselineSubtraction.h"
BaselineSubtraction::BaselineSubtraction(){
  //Constructor
  std::cout<<"Baseline Subtraction module created\n";
}


void BaselineSubtraction::SetShift(int shift) {
  fshift = shift;
}

void BaselineSubtraction::SetSubtractor(int subtractor) {
  fsubtractor = subtractor;
}


int BaselineSubtraction::Shift(int data) {
  return data>>fshift;
}

int BaselineSubtraction::Subtract(int data) {
  if(data-fsubtractor<0) return 0;
  else return data-fsubtractor;
}



