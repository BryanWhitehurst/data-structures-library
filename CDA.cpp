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
        ~CDA();
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
CDA<elmtype>::CDA(int s){
    arr = new elmtype[s];
    capacity = s;
    size = s; 
    order = 0; 
    startIndex = 0;
}

template <typename elmtype>
CDA<elmtype>::CDA(const CDA& array){
    size = array.size;
    capacity = array.capacity;
    startIndex = array.startIndex;
    arr = new elmtype[array.capacity];
    for(int i = 0; i < array.size; i++){
        (*this)[i] = (array.arr)[(i+startIndex) % capacity];
    }
}

template <typename elmtype>
CDA<elmtype>& CDA<elmtype>::operator=(const CDA<elmtype>& obj){
    delete arr;
    this->size = obj.size;
    this->capacity = obj.capacity;
    this->startIndex = obj.startIndex;
    this->arr = new elmtype[capacity];
    cout<< startIndex << endl;
    for(int i = 0; i < size; i++){
        (*this)[i] = obj.arr[(i + startIndex) % capacity];
    }
    return *this;
}

template <typename elmtype>
CDA<elmtype>::~CDA(){
    delete arr;
    capacity = 0;
    size = 0;
}

//any time this is used, set ordered back to false
template <typename elmtype>
elmtype& CDA<elmtype>::operator[](int i){

    if(i < 0 || i >= size) cout << "out of bounds" << endl;
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
        delete arr;
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
int CDA<elmtype>::Ordered(){
    return order;
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
elmtype CDA<elmtype>::Select(int k){
    if(order == 1)
        return (*this)[k - 1];

    else if(order == -1) return (*this)[size-k];
    else{
        elmtype a[capacity];
        for(int i = 0; i < size; i++) {
            a[i] = (*this)[i];
        }  
        cout << endl;
        return QuickSelect(a, 0, size-1, k);
    }
}

template <typename elmtype>
int CDA<elmtype>::partition(elmtype a[], int l, int r){
    srand(time(0));
    int pivot = a[r]; 
    int i = (l - 1); 
    for (int j = l; j <= r - 1; j++) { 
        if (a[j] <= pivot) { 
            i++; 
            swap(a[i], a[j]); 
        } 
    } 
    swap(a[i + 1], a[r]); 
    return (i + 1); 
}

template <typename elmtype>
elmtype CDA<elmtype>::QuickSelect(elmtype a[], int left, int right, int k){
    while (left <= right) { 

        int pivotIndex = partition(a, left, right); 

        if (pivotIndex == k - 1) 
            return a[pivotIndex]; 
  
        else if (pivotIndex > k - 1) 
            right = pivotIndex - 1; 

        else
            left = pivotIndex + 1; 
    } 
    return -1; 
}


template <typename elmtype>
void CDA<elmtype>::InsertionSort(){
    elmtype key; 
    int j;

    for(int i = 1; i < size; i++){
        key = (*this)[i]; 
        j = i - 1;


        while(j >= 0 && (*this)[j] < key){
        (*this)[j + 1] = (*this)[j];
        j--;
    }
    (*this)[j + 1] = key;
    }

    order = -1;
}

template <typename elmtype>
void CDA<elmtype>::MergeSort(){
    MergeSort(0, size-1);
    order = -1;
}

template <typename elmtype>
void CDA<elmtype>::MergeSort(int left, int right){
    if(left < right){
        int middle = left + (right - left) / 2;

        MergeSort(left, middle); 
        MergeSort(middle + 1, right);

        Merge(left, middle, right); 
    }
}

template <typename elmtype>
void CDA<elmtype>::Merge(int left, int middle, int right){
    int i, j, k;
    
    int n1 = middle - left + 1;
    int n2 = right - middle; 

    elmtype tempL[n1], tempR[n2];

    for (i = 0; i < n1; i++) 
        tempL[i] = (*this)[left + i]; 

    for (j = 0; j < n2; j++) 
        tempR[j] = (*this)[middle + 1 + j]; 

    i = 0; 
    j = 0; 
    k = left;
    while (i < n1 && j < n2) { 
        if (tempL[i] >= tempR[j]) { 
            (*this)[k] = tempL[i]; 
            i++; 
        } 
        else { 
            (*this)[k] = tempR[j]; 
            j++; 
        } 
        k++; 
    } 

    while (i < n1) { 
        (*this)[k] = tempL[i]; 
        i++; 
        k++; 
    } 
    while (j < n2) { 
        (*this)[k] = tempR[j]; 
        j++; 
        k++; 
    } 
}

template <typename elmtype>
void CDA<elmtype>::CountingSort(int m){
    elmtype output[size];
    int count[m + 1];

    for(int i = 0; i < m + 1; i++)
        count[i] = 0;
    
    for (int i = 0; i < size; i++)
		count[(*this)[i]]++;
    
    int total = 0;
	for (int i = 0; i < m + 1; i++){
		int oldCount = count[i];
		count[i] = total;
		total += oldCount;
	}

    for (int i = 0; i < size; i++){
		output[count[(*this)[i]]] = (*this)[i];
		count[(*this)[i]]++;
	}

    for (int i = 0, j = size - 1; i < size; i++, j--)
		(*this)[i] = output[j];
    
    order  = -1;
}

template <typename elmtype>
int CDA<elmtype>::Search(elmtype e){
    if(order != 0) return BinarySearch(0, size - 1, e);

    for(int i = 0; i < size; i++){
        if((*this)[i] == e) return i;
    }
    return -1;            
}

template <typename elmtype>
int CDA<elmtype>::BinarySearch(int l, int r, elmtype e){
    if(order == 1){
         if (r >= l) { 
        int mid = l + (r - l) / 2; 
   
        if ((*this)[mid] == e) 
            return mid; 
  
        if ((*this)[mid] > e) return BinarySearch(l, mid - 1, e); 
        return BinarySearch(mid + 1, r, e); 
    } 

    }
    if(order == -1){
        if (r >= l) { 
        int mid = l + (r - l) / 2; 
  
        if ((*this)[mid] == e) return mid; 
  
        if ((*this)[mid] < e) return BinarySearch(l, mid - 1, e); 

        return BinarySearch(mid + 1, r, e); 
    }
    return -1; 
    }
} 

template <typename elmtype>
elmtype& CDA<elmtype>::at(elmtype *a, int i){
    return *(a + (startIndex + i % capacity));
}


template <typename elmtype>
int CDA<elmtype>::getSize(){
    return this->size;
}
template <typename elmtype>
int CDA<elmtype>::getCapacity(){
    return this->capacity;
}