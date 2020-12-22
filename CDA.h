#ifndef __CDA_H__
#define __CDA_H__
#include <iostream>
#include <string>
#include <cstdlib>
#include <time.h>

using namespace std;
template <typename elmtype>
class CDA{
    public:
        CDA();
        CDA(int s);
        CDA(const CDA&);
        CDA<elmtype>& operator=(const CDA<elmtype>& array);
        //~CDA();
        elmtype& operator[](int i); 
        elmtype& at(elmtype *a, int i);
        void AddEnd(elmtype v);
        void AddFront(elmtype v); 
        void DelEnd(); 
        void DelFront();
        int Length(); 
        int EmptySlots(); 
        void Clear();
        int Ordered();
        int SetOrdered();
        elmtype Select(int k);
        elmtype QuickSelect(elmtype a[], int left, int right, int k);
        int partition(elmtype a[], int left, int right);
        void InsertionSort();
        void MergeSort();
        void MergeSort(int left, int right);
        void Merge(int left, int middle, int right);
        void CountingSort(int m);
        int Search(elmtype e);
        int BinarySearch(int left, int right, elmtype e);
        int getSize();
        int getCapacity();
    private: 
        elmtype *arr; 
        int capacity;
        int size;
        int startIndex; 
        int order;      
};

template <typename elmtype>
CDA<elmtype>::CDA(){
    arr = new elmtype[1];
    capacity = 1;
    size = 0; 
    order = 0;
}


template <typename elmtype>
CDA<elmtype>& CDA<elmtype>::operator=(const CDA<elmtype>& obj){
    delete arr;
    this->size = obj.size;
    this->capacity = obj.capacity;
    this->startIndex = obj.startIndex;
    this->arr = new elmtype[capacity];
    for(int i = 0; i < size; i++){
        (*this)[i] = obj.arr[(i + startIndex) % capacity];
    }
    return *this;
}


//any time this is used, set ordered back to false
template <typename elmtype>
elmtype& CDA<elmtype>::operator[](int i){

    //if(i < 0 || i >= size) cout << "out of bounds" << endl;
    return *(arr + ((startIndex + i) % capacity));
}

template <typename elmtype>
void CDA<elmtype>::AddEnd(elmtype v){
    if(size == 0) startIndex = 0;
    size++;
    if(size == 2) this->SetOrdered();
    int oldCap = capacity;
    if(size > capacity){
        capacity*= 2;
        elmtype *newArr = new elmtype[capacity]; 
        for(int i = startIndex; i < startIndex + size - 1; i++){
            *(newArr + (i % capacity)) = *(arr + (i % oldCap));
        }
        delete[] arr;
        arr = newArr;
    }
    *(arr + ((startIndex + size -1) % capacity)) = v;
    if(order != 0) this->SetOrdered();
 
}

template <typename elmtype>
void CDA<elmtype>::AddFront(elmtype v){
    if(size == 0) startIndex = 0;
    if(size >= 2) this->SetOrdered();

    if(size == capacity){
        int oldCap = capacity;
        capacity*= 2;
        elmtype *newArr = new elmtype[capacity]; 
        for(int i = startIndex; i < startIndex + size; i++){
            *(newArr + (i % capacity)) = *(arr + (i % oldCap));
        }
    
        delete arr;
        arr = newArr;
    }
    size++;

    if(startIndex > 0){
        *(arr + startIndex -1) = v;
        startIndex--;
    }
    else{
        *(arr + capacity - 1) = v;
        startIndex = capacity-1;
    }
    if(order != 0) this->SetOrdered();
}

template <typename elmtype>
void CDA<elmtype>::DelEnd(){
    size--;
    int oldCapacity = capacity;
    if (size <= (0.25 * capacity) && (capacity > 4)){
        capacity /= 2;
        elmtype *newArr = new elmtype[capacity];
        for(int i = startIndex; i < startIndex + size; i++){
            *(newArr + (i % capacity)) = *(arr + (i % oldCapacity));
        }
        delete arr;
        arr = newArr;
    }
}

template <typename elmtype>
void CDA<elmtype>::DelFront(){
    size--;
    int oldCapacity = capacity;
    startIndex = (startIndex + 1) % capacity;
    if ((size <= (0.25 * capacity)) && (capacity > 4)){
        capacity /= 2;
        elmtype *newArr = new elmtype[capacity];
        for(int i = startIndex; i < startIndex + size; i++){
            *(newArr + (i % capacity)) = *(arr + (i % oldCapacity));
        }
        delete arr;
        arr = newArr;
    }
}

template <typename elmtype>
int CDA<elmtype>::Length(){
    return size;
} 

template <typename elmtype>
int CDA<elmtype>::EmptySlots(){
    return capacity - size;
} 

template <typename elmtype>
void CDA<elmtype>::Clear(){
    delete arr;
    arr = new elmtype[1];
    capacity = 1;
    size = 0; 
    order = 0;
}

template <typename elmtype>
int CDA<elmtype>::SetOrdered(){
    bool increasing = true;
    bool decreasing = true; 
    for(int i = startIndex; i < startIndex + size - 1; i++){
        if(*(arr + (i % capacity)) < *(arr + ((i + 1) % capacity))){ 
            decreasing = false; 
        }
        if(*(arr + (i % capacity)) > *(arr + ((i + 1) % capacity))){ 
            increasing = false;
        }
    }
    if(increasing) order = 1;
    else if(decreasing) order = -1;
    else order = 0;

    return order;
} 


template <typename elmtype>
int CDA<elmtype>::getSize(){
    return this->size;
}

#endif


