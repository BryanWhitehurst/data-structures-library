#include <iostream>
#include <cstdlib>
#include "CDA.h"

using namespace std;
template <typename keytype>
class Heap{
    public:
        Heap();
        Heap(keytype k[], int s);
        void Heapify(int cur);
        ~Heap();
        Heap(Heap<keytype>& h);
        Heap<keytype>& operator=(Heap<keytype>& h);
        keytype peekKey();
        keytype extractMin();
        void insert(keytype k);
        void printKey();
        int Size();
        keytype getIndex(int i);


    private:
        CDA<keytype> data; //data[0] = -1, array should start at index 1
};

template <typename keytype>
Heap<keytype>::Heap(){
}

template <typename keytype>
Heap<keytype>::~Heap(){
}

template <typename keytype>
Heap<keytype>::Heap(Heap<keytype>& h){
    CDA<keytype> temp(h.Size());
    for(int i = 0; i < h.Size(); i++){
        temp[i] = h.getIndex(i);
    }
    data = temp;
}

template <typename keytype>
Heap<keytype>& Heap<keytype>::operator=(Heap<keytype>& h){
    CDA<keytype> temp(h.Size());
    for(int i = 0; i < h.Size(); i++){
        temp[i] = h.getIndex(i);
    }
    data = temp;
    return *this;
}

template <typename keytype>
Heap<keytype>::Heap(keytype k[], int s){
    data.AddEnd(k[0]);
    for(int i = 0; i < s; i++) {
        data.AddEnd(k[i]);
    }
    int firstInternal = (data.getSize() - 1) / 2;
    for(int i = firstInternal; i>=1; i--){
        if((((i * 2) + 1) >= data.getSize()) && data[i*2] < data[i]) swap(data[i*2], data[i]);

        else{
            int left = i * 2;
            int right = left + 1;
            if((data[i] > data[left] || data[i] > data[right]) && (data[left] <= data[right])) {
                swap(data[i], data[left]);
                Heapify(left);
            }
            else if((data[i] > data[left] || data[i] > data[right]) && (data[right] <= data[left])){
                swap(data[i], data[right]);
                Heapify(right);
            }
        }
    }
}

template <typename keytype>
void Heap<keytype>::Heapify(int cur){
    int left = cur * 2;
    int right = left + 1;

    if((left >= data.getSize()) || (right >= data.getSize())) return;

    if((data[cur] > data[left] || data[cur] > data[right]) && (data[left] <= data[right])){
        swap(data[cur], data[left]);
        Heapify(left);
    }
    else if((data[cur] > data[left] || data[cur] > data[right]) && (data[right] <= data[left])) {
        swap(data[cur], data[right]);
        Heapify(right);
    }
}

template <typename keytype>
keytype Heap<keytype>::peekKey(){
    return data[1];
}

template <typename keytype>
void Heap<keytype>::printKey(){
    for(int i = 1; i < data.getSize(); i++){
        cout << data[i] << " ";
    }
    cout << endl;
}

template<typename keytype>
void Heap<keytype>::insert(keytype k){
    if(data.getSize() == 0) data.AddFront(k);

    int curIndex = data.getSize();
    data.AddEnd(k);
    //if node is less than its parent, you must perform swap
    while(data[curIndex] < data[curIndex/2]){ 
        if(curIndex == 1) break;
        swap(data[curIndex/2], data[curIndex]);
        curIndex /= 2;
    }
}

template<typename keytype>
keytype Heap<keytype>::extractMin(){
    if(data.getSize() == 2){
        keytype num = data[1];
        data.DelFront();
        return num;  
    }
    keytype min = data[1];
    keytype last = data[data.getSize() - 1];
    data.DelEnd();
    data[1] = last;
    
    int root = 1; 
    //what if you swap with a leaf? check to see if curIndex * 2 = getSiz
    while((root * 2  <  data.getSize() && data[root] > data[root * 2]) || ((root * 2) + 1 <  data.getSize() && data[root] > data[(root * 2)+1])){

        if(root * 2  <  data.getSize() &&  ((root * 2) + 1)  <  data.getSize() &&  data[root] > data[root * 2] && data[root] > data[(root * 2) + 1] && data[root * 2] < data[(root * 2) + 1]){
            swap(data[root], data[root * 2]);
            root *= 2;
        }
        else if(root * 2  <  data.getSize() &&  ((root * 2) + 1)  <  data.getSize() && data[root] > data[root * 2] && data[root] > data[(root * 2) + 1] && data[(root * 2) + 1] < data[root * 2]){
            swap(data[root] , data[(root * 2) + 1]);
            root = (root *2) + 1;
        }
        else if(root * 2  <  data.getSize() && data[root] > data[root * 2]){
            swap(data[root], data[root * 2]);
            root *= 2;
        }
        else if((root * 2) + 1 < data.getSize()){
            swap(data[root] , data[(root * 2) + 1]);
            root = (root *2) + 1;   
        } 
    }
    return min;
}

template<typename keytype>
int Heap<keytype>::Size(){
    return data.getSize();
}

template<typename keytype>
keytype Heap<keytype>::getIndex(int i){
    return data[i];
}