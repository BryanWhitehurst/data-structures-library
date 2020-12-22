#include <iostream>
#include <cstdlib>
#include "CDA.h"

using namespace std;

template <typename keytype>
class Node{
    public:
    CDA<Node*> children;
    Node<keytype> *parent;
    Node<keytype> *next, *prev; //only utilized if in root list
    int k; //order of tree, only in use if node is root
    keytype key;
    Node(){
        parent = NULL;
        next = NULL;
        prev = NULL;
        k = 0; 
    }
    Node(keytype key){
        parent = NULL;
        next = NULL;
        prev = NULL;
        k = 0; 
        this->key = key;
    }
};


template <typename keytype>
class BHeap{
    public:
        BHeap(); 
        BHeap(keytype k[], int s);
        //~BHeap();
        void delPostOrder(Node<keytype> *cur);
        BHeap(BHeap<keytype>& h);
        BHeap<keytype>& operator=(BHeap<keytype>& h);
        void inorder(Node<keytype> *cur);
        keytype peekKey(); 
        keytype extractMin();
        void insert(keytype k);
        void merge(BHeap<keytype> &H2);
        void printKey();
        keytype getIndex(int i);
        Node<keytype>* mergeRoots(Node<keytype> *h1_root, Node<keytype> *h2_root);
        void fixCascade(Node<keytype> *cur);
        void preorder(Node<keytype> *cur);


    private:
    Node<keytype> *head = NULL; //head of linked list
    int maxK = 0;
};

template <typename keytype>
BHeap<keytype>::BHeap(){
    head = NULL;
}

template <typename keytype>
BHeap<keytype>::BHeap(BHeap<keytype>& h){
    for(int i = h.maxK; i >=0; i--){
        for(Node<keytype>*cur = h.head; cur != NULL; cur = cur->next){
            if(cur->k == i){
                inorder(cur);
                break;
            }
        }
    } 
}
template <typename keytype>
BHeap<keytype>& BHeap<keytype>::operator=(BHeap<keytype>& h){
    Node<keytype> *cur = head;
    while(cur != NULL){
        Node<keytype>* temp = cur->next;
        delPostOrder(cur);
        cur = temp;
    }
    head = NULL;
    maxK = 0;

    for(int i = h.maxK; i >=0; i--){
        for(Node<keytype>*cur = h.head; cur != NULL; cur = cur->next){
            if(cur->k == i){
                inorder(cur);
                break;
            }
        }
    } 
    return *this;
}

template <typename keytype>
void BHeap<keytype>::inorder(Node<keytype> *cur){
    insert(cur->key);
    for(int i = cur->children.getSize() - 1; i >= 0; i--) inorder(cur->children[i]); 
}

/*template <typename keytype>
BHeap<keytype>::~BHeap(){
    Node<keytype> *cur = head;
    while(cur != NULL){
        Node<keytype>* temp = cur->next;
        delPostOrder(cur);
        cur = NULL;
        cur = temp;
    }
    head = NULL;
    maxK = 0;
}*/

template <typename keytype>
void BHeap<keytype>::delPostOrder(Node<keytype> *cur){
    for(int i = 0; i < cur->children.getSize(); i++){ 
        delPostOrder(cur->children[i]);
    }
    delete cur; 
    cur = NULL;
}

template <typename keytype>
BHeap<keytype>::BHeap(keytype k[], int s){
    for(int i = 0; i < s; i++) insert(k[i]);
}

template <typename keytype>
void BHeap<keytype>::merge(BHeap<keytype> &H2){
    Node<keytype> *h2_cur = H2.head;
    while(h2_cur != NULL){//for each tree in second heap
        for(Node<keytype> *h1_cur = head; h1_cur != NULL; h1_cur = h1_cur->next){ //find a tree with matching rank
            if(h2_cur->k == h1_cur->k){

                Node<keytype> *temp = h2_cur->next;
                h1_cur = mergeRoots(h1_cur, h2_cur);
                fixCascade(h1_cur);
                h2_cur = temp;
                if(h2_cur != NULL) h2_cur->prev = NULL;
                break;
            }
            else if(h1_cur->next == NULL){
                //if(h2_cur->key == 114 && h2_cur->next != NULL ) cout << h2_cur->next->key<< endl;
                if(maxK < h2_cur->k) maxK = h2_cur->k;
                Node<keytype> *temp = h2_cur->next;
                h1_cur->next = h2_cur;
                h2_cur->prev = h1_cur;
                h2_cur->next = NULL;
                h2_cur = temp; 
                if(h2_cur != NULL) h2_cur->prev = NULL; //if the next node is H2 is not null, then prev = NULL
                break;
            }
        }
        //if h1 is an empty heap, h2_cur is new head of h1
        if(head == NULL){
            maxK = h2_cur->k;
            Node<keytype> *temp = h2_cur->next;
            head = h2_cur;
            h2_cur->prev = NULL;
            h2_cur->next = NULL;
            h2_cur = temp;
            if(h2_cur != NULL) h2_cur->prev = NULL;
        }

    }
    
}

template <typename keytype>
Node<keytype>* BHeap<keytype>::mergeRoots(Node<keytype> *h1_root, Node<keytype> *h2_root){
    if(h1_root->key <= h2_root->key){
        (h1_root->k)++;
        if(h1_root->k > maxK) maxK = h1_root->k;
        h1_root->children.AddFront(h2_root);
        h2_root->parent = h1_root;
        if(h2_root->prev == NULL && h2_root->next != NULL) h2_root->next->prev = NULL;
        else if(h2_root->next == NULL && h2_root->prev != NULL) h2_root->prev->next = h2_root->next;
        else if(h2_root->prev != NULL && h2_root->next != NULL){ 
            h2_root->prev->next = h2_root->next;
            h2_root->next->prev = h2_root->prev;
        }
        return h1_root;
    }
    else {
        (h2_root->k)++;
        if(h2_root->k > maxK) maxK = h2_root->k;
        Node<keytype> *temp = head;

        if(h2_root->prev == NULL && h2_root->next != NULL) h2_root->next->prev = NULL;
        else if(h2_root->next == NULL && h2_root->prev != NULL) h2_root->prev->next = h2_root->next;
        else if(h2_root->prev != NULL && h2_root->next != NULL){
            h2_root->prev->next = h2_root->next;
            h2_root->next->prev = h2_root->prev;
        }
        head = h2_root;
        h2_root->prev = NULL;
        if(h2_root != temp) { //if h2_root is not already the head, then we must set next = to the head 
            h2_root->next = temp;
            if(h2_root->next != NULL) h2_root->next->prev = h2_root;
        } 
        if(temp == h1_root){ //if the head is h1_root, we must set head->next = h1_root->next bc we are getting rid of h1_root
            h2_root->next = h1_root->next;
            if(h1_root->next != NULL) h1_root->next->prev = h2_root;
        } 
        h1_root->parent = h2_root;
        h2_root->children.AddFront(h1_root);

        if(h2_root->next != NULL) h2_root->next->prev = h2_root;
        //if(h2_root->key == 7 && h1_root->key == 9 && h1_root->children[0]->key == 10) cout << h1_root->prev->key << endl;
        if(h1_root->prev == NULL && h1_root->next != NULL) h1_root->next->prev = NULL;
        else if(h1_root->next == NULL && h1_root->prev != NULL) h1_root->prev->next = h1_root->next;
        else if(h1_root->prev != NULL && h1_root->next != NULL){
            h1_root->prev->next = h1_root->next;
            h1_root->next->prev = h1_root->prev;
        }
        return h2_root;
    }
}
template <typename keytype>
void BHeap<keytype>::fixCascade(Node<keytype> *cur){
    for(Node<keytype> *i = head; i != NULL; i = i->next){
        if((i->k == cur->k) && (i != cur)){
            cur = mergeRoots(i, cur);
            fixCascade(cur);
            break;
        }
    }
}

template <typename keytype>
keytype BHeap<keytype>::extractMin(){
    Node<keytype> *min = head;
    for(Node<keytype> *cur = head->next; cur != NULL; cur = cur->next){
        if(cur->key < min->key) min = cur;
    }
    keytype minKey = min->key;

    //code to put all immediate children into a new Heap
    BHeap<keytype> H2;
    if(min->children.getSize() > 0){
        H2.head = min->children[0];
        Node<keytype> *cur = H2.head;
        for(int i = 1; i < min->children.getSize(); i++){
            cur->next = min->children[i];
            min->children[i]->prev = cur;
            cur = cur->next;
            //if(i == min->children.getSize() - 1 && cur->next != NULL) cout << "\ncur next: "<< cur->next->key << endl;
            if(i == min->children.getSize() - 1) cur->next = NULL;
        }
    }
    //if(minKey == 103) cout << H2.head->next->next << endl;

    //now we delete min node from H1 and merge H1 and H2
    if(min->prev == NULL && min->next != NULL) min->next->prev = NULL;
    else if(min->next == NULL && min->prev != NULL) min->prev->next = min->next;
    else if(min->prev != NULL && min->next != NULL){
        min->prev->next = min->next;
        min->next->prev = min->prev;
    }
    if(head == min) head = head->next;

    delete min;
    min = NULL; //must reset min to NULL
   // if(minKey == 50) cout << min << endl;

    if(H2.head != NULL) merge(H2);

    return minKey;
}

template <typename keytype>
void BHeap<keytype>::insert(keytype k){
    BHeap<keytype> H2;
    H2.head = new Node<keytype>(k);
    merge(H2);
}

template <typename keytype>
void BHeap<keytype>::printKey(){
  
    //go through and print 0's, then 2's, then 3's, then 4's, 
    for(int i = 0; i <= maxK; i++){
        for(Node<keytype>*cur = head; cur != NULL; cur = cur->next){
            if(cur->k == i){
                cout << "B" << cur->k << endl;
                preorder(cur);
                cout << endl;
                cout << endl;
                break;
            }
        }
    } 
}

template <typename keytype>
void BHeap<keytype>::preorder(Node<keytype> *cur){
    cout << cur->key << " ";
    for(int i = 0; i < cur->children.getSize(); i++) preorder(cur->children[i]); 
}

template <typename keytype>
keytype BHeap<keytype>::peekKey(){
    Node<keytype> *min = head;
    for(Node<keytype> *cur = head->next; cur != NULL; cur = cur->next){
        if(cur->key < min->key) min = cur;
    }
    return min->key;
}

