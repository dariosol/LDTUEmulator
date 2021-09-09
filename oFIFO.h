#ifndef _OFIFO_H_
#define _OFIFO_H_

template <class T> class oFIFO {

 public:
  oFIFO<T>(size_t);  

  // Member Functions()
  void Write(T);
  T Read();
  bool Empty();
  bool Full();
  T Front();
  size_t Size();
  void Clear();
 private:
  std::unique_ptr<T[]> buffer; 
  size_t head = 0;             // size_t is an unsigned long
  size_t tail = 0;
  size_t max_size;
  T empty_item; // we will use this to clear data
  T output;  
};
 
#endif
