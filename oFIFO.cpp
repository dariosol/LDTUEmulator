#include <memory>
#include <iostream>
#include "oFIFO.h"
//-------------------------------------------------------------------
// Circular_Buffer (Class)
//     We will implement the buffer with a templated class so
//     the buffer can be a buffer of specified type.
//-------------------------------------------------------------------
using namespace std;
template <class T>
oFIFO<T>::oFIFO(size_t size) : buffer(std::unique_ptr<T[]>(new T[size])), max_size(size){
  printf("Module Storage Fifo generated\n");

};

// Add an item to this circular buffer.
template <class T>
void oFIFO<T>::Write(T item) {

  // if buffer is full, throw an error
  if (Full())
    //    throw std::runtime_error("buffer is full");
    cout<<("buffer is full")<<endl;
  
  // insert item at back of buffer
  buffer[tail] = item;

  // increment tail
  tail = (tail + 1) % max_size;
}

// Remove an item from this circular buffer and return it.
template <class T>
T oFIFO<T>::Read() {

  // if buffer is empty, throw an error
  if (Empty()) 
    //    throw std::runtime_error("buffer is empty");
    cout<<("buffer is empty")<<endl;

 
  // get item at head
  output = buffer[head];

  // set item at head to be empty
  //  T empty;
  buffer[head] = empty_item;

  // move head foward
  head = (head + 1) % max_size;

  // return item
  return output;
}

// Return the item at the front of this circular buffer.
template <class T>
T oFIFO<T>::Front() { return buffer[head]; }

// Return true if this circular buffer is empty, and false otherwise.
template <class T>
bool oFIFO<T>::Empty() { return head == tail; }

// Return true if this circular buffer is full, and false otherwise.
template <class T>
bool oFIFO<T>::Full() { return tail == (head - 1) % max_size; }

template <class T>
void oFIFO<T>::Clear() {
  while(!Empty()){
    Read();
  }
  cout<<"cleaned"<<endl;
}

// Return the size of this circular buffer.
template <class T>
size_t oFIFO<T>::Size() {
  if (tail >= head)
    return tail - head;
  return max_size - (head - tail);
}


