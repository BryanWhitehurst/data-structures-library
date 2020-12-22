#include <iostream>
#include <cstdlib>

using namespace std;

template <typename keytype, typename valuetype>
class Node {
public:
    int numKeys;
    int leftKids, middleLeftKids, middleRightKids, rightKids;
    keytype a, b, c;
    valuetype aValue, bValue, cValue;
    Node<keytype, valuetype> *left, *middleLeft, *middleRight, *right, *parent;
    Node(int keyNum = 0){
        numKeys = keyNum;
        left = NULL;
        middleLeft = NULL;
        middleRight = NULL;
        right = NULL;
        parent = NULL;
        leftKids = 0;
        middleLeftKids = 0;
        middleRightKids = 0;
        rightKids = 0;
    }; 

    Node(int keyNum, Node *left, Node *middleLeft, Node *middleRight, Node *right, Node *parent){
        numKeys = keyNum;
        this->left = left;
        this->middleLeft = middleLeft;
        this->middleRight = middleRight;
        this->right = right; 
        this->parent = parent;

        leftKids = 0;
        middleLeftKids = 0;
        middleRightKids = 0;
        rightKids = 0;
    };

    bool isFull();
};


//if there are 3 non-null keys in the node, then the node is full
template <typename keytype, typename valuetype>
bool Node<keytype, valuetype>::isFull(){
    if(numKeys == 3) return true;
    else return false;
};

template <typename keytype, typename valuetype>
class Two4Tree {
public: //change to private, public for tesitng
    Node<keytype, valuetype> *root;
    int numNodes; 
    int totalKeys;
public:
    Two4Tree(); 
    Two4Tree(keytype k[], valuetype V[], int s); 
    ~Two4Tree();
    Two4Tree(const Two4Tree &tree);
    Two4Tree<keytype, valuetype>& operator=(const Two4Tree<keytype, valuetype>& tree);
    void copyTraversal(Node<keytype, valuetype> *cur);
    void deconstruct(Node<keytype, valuetype>* root);
    valuetype * search(keytype k); 
    valuetype * search(keytype k, Node<keytype, valuetype> *cur); 
    Node<keytype, valuetype> * searchKey(keytype k, Node<keytype, valuetype> *cur); 
    void insert(keytype k, valuetype v); 
    void inserter(keytype k, valuetype v, Node<keytype, valuetype> *cur);
    void addKidNum(Node<keytype, valuetype> *cur);
    int remove(keytype k);
    int remove(keytype k, Node<keytype, valuetype> *cur);
    void subtractKidNum(Node<keytype, valuetype> *cur);
    Node<keytype, valuetype> * recursiveFix(Node<keytype, valuetype> * cur, Node<keytype, valuetype>* parent);
    void rotate(Node<keytype, valuetype> *cur, Node<keytype, valuetype> *parent, Node<keytype, valuetype> *sibling, keytype &pKey, valuetype &pValue);
    Node<keytype, valuetype>* merge(Node<keytype, valuetype> *cur, Node<keytype, valuetype> *parent, Node<keytype, valuetype> *sibling, keytype &pKey, valuetype &pValue, string flag);
    Node<keytype, valuetype> * searchDelete(keytype k, Node<keytype, valuetype> *cur);  
    Node<keytype, valuetype>* predecessorDelete(keytype k, Node<keytype, valuetype> *cur);
    Node<keytype, valuetype>* successorDelete(keytype k, Node<keytype, valuetype> *cur);
    int rank(keytype k); 
    Node<keytype, valuetype> * searchRank(keytype k, Node<keytype, valuetype> *cur);
    keytype select(int pos); 
    keytype select(Node<keytype, valuetype> *cur, int pos, int total);
    keytype successor(keytype k);
    keytype successorInternal(keytype k, Node<keytype, valuetype> *cur);
    keytype successorLeaf(keytype k, Node<keytype, valuetype> *cur);
    keytype predecessor(keytype k);
    keytype predecessorInternal(keytype k, Node<keytype, valuetype> *cur);
    keytype predecessorLeaf(keytype k, Node<keytype, valuetype> *cur);
    int size(); 
    void preorder();
    void preorder(Node<keytype, valuetype> *root);
    void inorder();
    void inorder(Node<keytype, valuetype> *root);
    void postorder();
    void postorder(Node<keytype, valuetype> *root);

};

template <typename keytype, typename valuetype>
Two4Tree<keytype, valuetype>::Two4Tree(){
    root = new Node<keytype, valuetype>();
    numNodes = 1;
    totalKeys = 0;

}

template <typename keytype, typename valuetype>
Two4Tree<keytype, valuetype>::Two4Tree(const Two4Tree &tree){
    numNodes = tree.numNodes;
    totalKeys = tree.totalKeys;
    root = new Node<keytype, valuetype>();

    copyTraversal(tree.root);

}

template <typename keytype, typename valuetype>
Two4Tree<keytype, valuetype>& Two4Tree<keytype, valuetype>::operator=(const Two4Tree<keytype, valuetype>& tree){
    deconstruct(this->root);

    root = new Node<keytype, valuetype>();
    numNodes = 1;
    totalKeys = 0;

    copyTraversal(tree.root);

}

template<typename keytype, typename valuetype>
void Two4Tree<keytype, valuetype>::copyTraversal(Node<keytype, valuetype> *cur){
    if(cur == NULL) return;
    //if 2 node, traverse middleLeft
    //visit b
    //traverse middleRight
    if(cur->numKeys == 1){
        copyTraversal(cur->middleLeft);
        insert(cur->b, cur->bValue);
        copyTraversal(cur->middleRight);
    }
    //if 3 node, traverse left
    //visit a
    //traverse middleLeft
    //visit b
    //traverse middleRight
    else if(cur->numKeys == 2){
        copyTraversal(cur->left);
        insert(cur->a, cur->aValue);
        copyTraversal(cur->middleLeft);
        insert(cur->b, cur->bValue);
        copyTraversal(cur->middleRight);
    }

    //if 4 node, traverse left
    //visit a
    //traverse middleleft
    //visit b
    //traverse middleRight
    //visit c
    //traverse right 
    else if(cur->numKeys == 3){
        copyTraversal(cur->left);
        insert(cur->a, cur->aValue);
        copyTraversal(cur->middleLeft);
        insert(cur->b, cur->bValue);
        copyTraversal(cur->middleRight);
        insert(cur->c, cur->cValue);
        copyTraversal(cur->right);
    }
}


template <typename keytype, typename valuetype>
Two4Tree<keytype, valuetype>::~Two4Tree(){
    deconstruct(root);
}

template <typename keytype, typename valuetype>
void Two4Tree<keytype, valuetype>::deconstruct(Node<keytype, valuetype> * root){
    if(root == NULL) return;
    //if 2 node, traverse middleLeft
    //visit b
    //traverse middleRight
    if(root->numKeys == 1){
        deconstruct(root->middleLeft);
        deconstruct(root->middleRight);
        delete root;
    }
    //if 3 node, traverse left
    //visit a
    //traverse middleLeft
    //visit b
    //traverse middleRight
    else if(root->numKeys == 2){
        deconstruct(root->left);
        deconstruct(root->middleLeft);
        deconstruct(root->middleRight);
        delete root;
    }

    //if 4 node, traverse left
    //visit a
    //traverse middleleft
    //visit b
    //traverse middleRight
    //visit c
    //traverse right 
    else if(root->numKeys == 3){
        deconstruct(root->left);
        deconstruct(root->middleLeft);
        deconstruct(root->middleRight);
        deconstruct(root->right);
        delete root;
    }
}

//write insert function first and then call insert to make tree
template <typename keytype, typename valuetype>
Two4Tree<keytype, valuetype>::Two4Tree(keytype k[], valuetype V[], int s){
    root = new Node<keytype, valuetype>();
    numNodes = 1;
    totalKeys = 0;

    for(int i = 0; i < s; i++){
        insert(k[i], V[i]);
    }
} 

//insert() acts as a wrapper class for the inserter() class, passing the root node so recursion may occur
template <typename keytype, typename valuetype>
void Two4Tree<keytype, valuetype>::insert(keytype k, valuetype v){
    totalKeys++;
    inserter(k, v, root);
}

template <typename keytype, typename valuetype>
void Two4Tree<keytype, valuetype>::inserter(keytype k, valuetype v, Node<keytype, valuetype> *cur){

    //if node is full and node is the root creates parent 2 node with 2 children
    Node<keytype, valuetype> *parent = cur->parent; 
    if(cur->isFull() && parent == NULL){ 

        Node<keytype, valuetype> *newParent = new Node<keytype, valuetype>(1);
        Node<keytype, valuetype> *newLeft = new Node<keytype, valuetype>(1);
        Node<keytype, valuetype> *newRight = new Node<keytype, valuetype>(1);

        newParent->b = cur->b;
        newParent->bValue = cur->bValue;
        newParent->middleLeft = newLeft;
        newParent->middleRight = newRight;

        newLeft->parent = newParent;
        newLeft->b = cur->a;
        newLeft->bValue = cur->aValue;
        newLeft->middleLeft = cur->left;
        newLeft->middleRight = cur->middleLeft;

        if(newLeft->middleLeft != NULL){
            newLeft->middleLeft->parent = newLeft;
            newLeft->middleRight->parent = newLeft;
        }


        newRight->parent = newParent; 
        newRight->b = cur->c;
        newRight->bValue = cur->cValue;
        newRight->middleLeft = cur->middleRight;
        newRight->middleRight = cur->right;

        newRight->middleLeftKids = cur->middleRightKids;
        newRight->middleRightKids = cur->rightKids;
        newLeft->middleLeftKids = cur->leftKids;
        newLeft->middleRightKids = cur->middleLeftKids;

        newParent->middleLeftKids = 1 + newLeft->middleLeftKids + newLeft->middleRightKids;
        newParent->middleRightKids = 1 + newRight->middleLeftKids + newRight->middleRightKids;

        if(newRight->middleLeft != NULL){
            newRight->middleLeft->parent = newRight;
            newRight->middleRight->parent = newRight;
        }

        delete cur;
        cur = newParent;
        root = newParent;
        numNodes += 2;


    }

    //if cur node is full and cur node is NOT the root
    if(cur->isFull()){
        //if parent has 1 key and curKey is less than parent b, key goes to parent a
            //parent will now have 1 more child than before
            //create new node, left 2 children of cur node go to middle left and middleright of new node and cur node rightmost children become middleleft and middle right
            //new node will become the left child of parent, cur node stays in its remaining position 
        if(parent->numKeys == 1 && (cur->b <= parent->b)){
            parent->a = cur->b;
            parent->aValue = cur->bValue; 
            parent->numKeys = 2;

            Node<keytype, valuetype> *newLeft = new Node<keytype, valuetype>(1, NULL, cur->left, cur->middleLeft, NULL, parent);
            newLeft->b = cur->a;
            newLeft->bValue = cur->aValue;
            cur->b = cur->c;
            cur->bValue = cur->cValue;
            cur->middleLeft = cur->middleRight;
            cur->middleRight = cur->right;
            cur->left = NULL;
            cur->right = NULL;
            cur->numKeys = 1;

            parent->left = newLeft;

            newLeft->middleLeftKids = cur->leftKids;
            newLeft->middleRightKids = cur->middleLeftKids;
            cur->middleLeftKids = cur->middleRightKids;
            cur->middleRightKids = cur->rightKids;

            parent->leftKids = 1 + newLeft->middleLeftKids + newLeft->middleRightKids;
            parent->middleLeftKids = 1 + cur->middleLeftKids + cur->middleRightKids;

            if(cur->middleLeft != NULL){
                newLeft->middleLeft->parent = newLeft;
                newLeft->middleRight->parent = newLeft;
            } 
        }

        else if(parent->numKeys == 1 && (cur->b >= parent->b)){
            parent->a = parent->b;
            parent->aValue = parent->bValue;
            parent->b = cur->b;
            parent->bValue = cur->bValue;
            parent->numKeys = 2;

            Node<keytype, valuetype> *newRight =  new Node<keytype, valuetype>(1, NULL, cur->middleRight, cur->right, NULL, parent);
            newRight->b = cur->c;
            newRight->bValue = cur->cValue; 
            cur->b = cur->a;
            cur->bValue = cur->aValue;
            cur->middleRight = cur->middleLeft;
            cur->middleLeft = cur->left;
            cur->numKeys = 1; 


            //resetting the children node's parent pointer to the correct node
            if(cur->middleLeft != NULL){
                newRight->middleLeft->parent = newRight;
                newRight->middleRight->parent = newRight;
            }

            parent->left = parent->middleLeft;
            parent->middleLeft = parent->middleRight;
            parent->middleRight = newRight; 
            parent->leftKids = parent->middleLeftKids;

            parent->middleRightKids = cur->middleRightKids;

            newRight->middleLeftKids = cur->middleRightKids;
            newRight->middleRightKids = cur->rightKids;
            cur->middleRightKids = cur->middleLeftKids;
            cur->middleLeftKids = cur->leftKids;

            parent->middleLeftKids = 1 + cur->middleLeftKids + cur->middleRightKids; 
            parent->middleRightKids = 1 + newRight->middleLeftKids + newRight->middleRightKids;
        }

        //if parent has 2 keys and curKey is less than parent b, parent parent b goes to c, a goes to b, and curkey goes to a
            //create new node, left 2 children of cur node go to middle left and middleright of new node and cur node rightmost children become middleleft and middleright
            //each child pointer gets moved to the right one position, newnode becomes left child
        else if(parent->numKeys == 2 && (cur->b <= parent->a)){
            parent->c = parent->b;
            parent->cValue = parent->bValue;
            parent->b = parent->a;
            parent->bValue = parent->aValue;
            parent->a = cur->b;
            parent->aValue = cur->bValue; 
            parent->numKeys = 3; 

            Node<keytype, valuetype> *newLeft =  new Node<keytype, valuetype>(1, NULL, cur->left, cur->middleLeft, NULL, parent);
            newLeft->b = cur->a;
            newLeft->bValue = cur->aValue;
            cur->b = cur->c;
            cur->bValue = cur->cValue;
            cur->middleLeft = cur->middleRight;
            cur->middleRight = cur->right;
            cur->left = NULL;
            cur->right = NULL;
            cur->numKeys = 1;

            parent->right = parent->middleRight;
            parent->middleRight = parent->middleLeft;
            parent->middleLeft = parent->left;
            parent->left = newLeft; 
            parent->rightKids = parent->middleRightKids;
            parent->middleRightKids = parent->middleLeftKids;
            
            newLeft->middleLeftKids = cur->leftKids;
            newLeft->middleRightKids = cur->middleLeftKids;
            cur->middleLeftKids = cur->middleRightKids;
            cur->middleRightKids = cur->rightKids;

            parent->leftKids = 1 + newLeft->middleLeftKids + newLeft->middleRightKids;
            parent->middleLeftKids = 1 + cur->middleLeftKids + cur->middleRightKids;

            if(cur->middleLeft != NULL){
                newLeft->middleLeft->parent = newLeft;
                newLeft->middleRight->parent = newLeft;
            } 
        }

        else if(parent->numKeys == 2 && (cur->b <= parent->b)){
            parent->c = parent->b;
            parent->cValue = parent->bValue;
            parent->b = cur->b;
            parent->bValue = cur->bValue; 
            parent->numKeys = 3; 

            Node<keytype, valuetype> *newLeft =  new Node<keytype, valuetype>(1, NULL, cur->left, cur->middleLeft, NULL, parent);
            newLeft->b = cur->a;
            newLeft->bValue = cur->aValue;
            cur->b = cur->c;
            cur->bValue = cur->cValue;
            cur->middleLeft = cur->middleRight;
            cur->middleRight = cur->right;
            cur->left = NULL;
            cur->right = NULL;
            cur->numKeys = 1;

            parent->right = parent->middleRight;
            parent->middleRight = cur;
            parent->middleLeft = newLeft;
            parent->rightKids = parent->middleRightKids;

            newLeft->middleLeftKids = cur->leftKids;
            newLeft->middleRightKids = cur->middleLeftKids;
            cur->middleLeftKids = cur->middleRightKids;
            cur->middleRightKids = cur->rightKids;

            parent->middleLeftKids = 1 + newLeft->middleLeftKids + newLeft->middleRightKids;
            parent->middleRightKids = 1 + cur->middleLeftKids + cur->middleRightKids;

            if(cur->middleLeft != NULL){
                newLeft->middleLeft->parent = newLeft;
                newLeft->middleRight->parent = newLeft;
            } 

        }

        //if parent has 2 keys and curkey is > parent b, curKey goes to c
            //create new node, right 2 children of cur node go to middle left and middleright of new node and cur node leftmost children become middleleft and middleright
            //newnode becomes right child of parent
        else if(parent->numKeys == 2 && (cur->b >= parent->b)){
            parent->c = cur->b; 
            parent->cValue = cur->bValue;
            parent->numKeys = 3;

            Node<keytype, valuetype> *newRight =  new Node<keytype, valuetype>(1, NULL, cur->middleRight, cur->right, NULL, parent);
            newRight->b = cur->c;
            newRight->bValue = cur->cValue; 
            cur->b = cur->a;
            cur->bValue = cur->aValue;
            cur->middleRight = cur->middleLeft;
            cur->middleLeft = cur->left;
            cur->numKeys = 1; 

            parent->right = newRight;

            newRight->middleLeftKids = cur->middleRightKids;
            newRight->middleRightKids = cur->rightKids;
            cur->middleRightKids = cur->middleLeftKids;
            cur->middleLeftKids = cur->leftKids;

            parent->rightKids = 1 + newRight->middleLeftKids + newRight->middleRightKids;
            parent->middleRightKids = 1 + cur->middleLeftKids + cur->middleRightKids;

            if(cur->middleLeft != NULL){
                newRight->middleLeft->parent = newRight;
                newRight->middleRight->parent = newRight;
            }

        }

        numNodes++; //1 node added to the tree
        cur = parent;
    }

    //we want to perform inserts at leafs, so if we're not currently at a leaf recurse on the correct child node
    //if we have not arrived at a leaf, recurse on child (if middleLeft is NULL, then we are at a leaf because middleLeft is always in use)
    if(cur->middleLeft != NULL){
        //if cur->numKeys == 1 and less go middleLeft
        if(cur->numKeys == 1 && k < cur->b) inserter(k, v, cur->middleLeft);
        //if cur->numKeys == 1 and more go middleRight
        else if(cur->numKeys == 1 && k > cur->b) inserter(k, v, cur->middleRight);

        //if cur->numKeys == 2 and less than key a, go left
        else if(cur->numKeys == 2 && k < cur->a) inserter(k, v, cur->left);
        //if cur->numKeys == 2 and less than key b, go middleLeft
        else if(cur->numKeys == 2 && k < cur->b) inserter(k, v, cur->middleLeft);
        //if cur->numKeys == 2 and more than key b, go middleRight
        else if(cur->numKeys == 2 && k > cur->b) inserter(k, v, cur->middleRight);

        //if cur->numKeys == 3 and less than key a, go left
        else if(cur->numKeys == 3 && k < cur->a) inserter(k, v, cur->left);
        //if cur->numKeys == 3 and less than key b, go middleLeft
        else if(cur->numKeys == 3 && k < cur->b) inserter(k, v, cur->middleLeft);
        //if cur->numKeys == 3 and less than key c, go middleRight
        else if(cur->numKeys == 3 && k < cur->c) inserter(k, v, cur->middleRight);
        //if cur->numKeys == 3 and more than key c, go right
        else if(cur->numKeys == 3 && k > cur->c) inserter(k, v, cur->right);
    }

    //now we are at leaf so we can perform insert
    else{

        //if tree is empty
        if(cur->numKeys == 0){
            cur->b = k; 
            cur->bValue = v;
            cur->numKeys = 1;
        }
        //if numkeys == 1 and less than b, insert at a and increment numkeys 
        else if(cur->numKeys == 1 && k < cur->b){
            cur->a = k;
            cur->aValue = v;
            cur->numKeys = 2;
        }
        //if numkeys == 1 and more than b, b goes to a and insert at b, increment numkeys
        else if(cur->numKeys == 1 && k > cur->b){
            cur->a = cur->b;
            cur->aValue = cur->bValue;
            cur->b = k;
            cur->bValue = v;
            cur->numKeys = 2;
        }
        //if numkeys == 2 and less than a, b goes to c, a goes to b, insert at a
        else if(cur->numKeys == 2 && k < cur->a){
            cur->c = cur->b;
            cur->cValue = cur->bValue; 
            cur->b = cur->a;
            cur->bValue = cur->aValue; 
            cur->a = k;
            cur->aValue = v;
            cur->numKeys = 3;
        }

        //if numkeys == 2 and less than b, b goes to c, insert at b
        else if(cur->numKeys == 2 && k < cur->b){
            cur->c = cur->b;
            cur->cValue = cur->bValue;
            cur->b = k;
            cur->bValue = v;
            cur->numKeys = 3;
        }
        //if numkeys == 2 and more than b, insert at c
        else if(cur->numKeys == 2 && k > cur->b){
            cur->c = k;
            cur->cValue = v;
            cur->numKeys = 3;
        }
        addKidNum(cur);
    }
}
template <typename keytype, typename valuetype>
void Two4Tree<keytype, valuetype>::addKidNum(Node<keytype, valuetype> *cur){
    if(cur->parent == NULL) return;
    Node<keytype, valuetype> *parent = cur->parent;
    if(parent->middleLeft->b == cur->b) parent->middleLeftKids++;
    else if(parent->middleRight->b == cur->b) parent->middleRightKids++;
    else if(parent->numKeys != 1 && parent->left->b == cur->b) parent->leftKids++;
    else if(parent->numKeys == 3 && parent->right->b == cur->b) parent->rightKids++;

    addKidNum(parent);
}

template <typename keytype, typename valuetype>
void Two4Tree<keytype, valuetype>::subtractKidNum(Node<keytype, valuetype> *cur){
    if(cur->parent == NULL) return;
    Node<keytype, valuetype> *parent = cur->parent;
    if(parent->middleLeft->b == cur->b) parent->middleLeftKids--;
    else if(parent->middleRight->b == cur->b) parent->middleRightKids--;
    else if(parent->numKeys != 1 && parent->left->b == cur->b) parent->leftKids--;
    else if(parent->numKeys == 3 && parent->right->b == cur->b) parent->rightKids--;

    subtractKidNum(parent);
}

template <typename keytype, typename valuetype>
int Two4Tree<keytype, valuetype>::remove(keytype k){

    if(root->numKeys == 1 && root->middleLeft == NULL){
        root->numKeys--;
        numNodes--;
        return 1; 
    }
    //in the case where the root and both of its children are 2 nodes, we do a preemptive merge, else we go left or right

    if(root->middleLeft != NULL && root->numKeys == 1 && root->middleLeft->numKeys == 1 && root->middleRight->numKeys == 1){
        Node<keytype, valuetype> *newNode = new Node<keytype, valuetype>(3, root->middleLeft->middleLeft, root->middleLeft->middleRight, root->middleRight->middleLeft, root->middleRight->middleRight, NULL);
        newNode->a = root->middleLeft->b; 
        newNode->aValue = root->middleLeft->bValue;
        newNode->b = root->b;
        newNode->bValue = root->bValue;
        newNode->c = root->middleRight->b;
        newNode->cValue = root->middleRight->bValue;

        newNode->leftKids = root->middleLeft->middleLeftKids;
        newNode->middleLeftKids = root->middleLeft->middleRightKids;
        newNode->middleRightKids = root->middleRight->middleLeftKids;
        newNode->rightKids = root->middleRight->middleRightKids;


       if(newNode->middleLeft != NULL){
            newNode->left->parent = newNode;
            newNode->middleLeft->parent = newNode;
            newNode->middleRight->parent = newNode;
            newNode->right->parent = newNode;
       }
        delete root->middleLeft;
        delete root->middleRight;
        delete root; 
        root = newNode;
        numNodes-= 2;
    }

    int returned;
    if(root->b == k ||  (root->numKeys != 1 && root->a == k) || (root->numKeys == 3 && root->c == k)){
        returned = remove(k, root);
        if(returned == 1) totalKeys--;
        return returned;
    }
    
    if(root->middleLeft == NULL) returned = remove(k, root);
    if(root->numKeys == 1 && k < root->b) returned = remove(k, root->middleLeft);
    else if(root->numKeys == 1 && k > root->b) returned = remove(k, root->middleRight);
    else if(root->numKeys == 2 && k < root->a) returned = remove(k, root->left);
    else if(root->numKeys == 2 && k < root->b)returned = remove(k, root->middleLeft);
    else if(root->numKeys == 2 && k >root->b) returned = remove(k, root->middleRight);
    else if(root->numKeys == 3 && k < root->a) returned = remove(k, root->left);
    else if(root->numKeys == 3 && k < root->b) returned = remove(k, root->middleLeft);
    else if(root->numKeys == 3 && k < root->c) returned = remove(k, root->middleRight);
    else if(root->numKeys == 3 && k > root->c) returned = remove(k, root->right);

    if(returned == 1) totalKeys--;
    
    return returned;

}

template <typename keytype, typename valuetype>
int Two4Tree<keytype, valuetype>::remove(keytype k, Node<keytype, valuetype> *cur){
    //now we need to search for the key using the search function
    cur = searchDelete(k, cur);
    if(cur == NULL) return 0;
    Node<keytype, valuetype> *parent = cur->parent;
    //if cur is 2 node, we must do a fix
    //we need to pass a pointer to the parent key and parent value, that way we can change them
    if(cur->numKeys == 1 && parent){
        if(parent->numKeys == 1 && parent->middleLeft->b == cur->b && parent->middleRight->numKeys != 1) rotate(cur, parent, parent->middleRight, parent->b, parent->bValue);
        else if(parent->numKeys == 1 && parent->middleRight->b == cur->b && parent->middleLeft->numKeys != 1) rotate(cur, parent, parent->middleLeft, parent->b, parent->bValue);
        else if(parent->left->b == cur->b && parent->middleLeft->numKeys != 1) rotate(cur, parent, parent->middleLeft, parent->a, parent->aValue); 
        else if(parent->middleLeft->b == cur->b && parent->middleRight->numKeys != 1) rotate(cur, parent, parent->middleRight, parent->b, parent->bValue);
        else if(parent->middleLeft->b == cur->b && parent->left->numKeys != 1) rotate(cur, parent, parent->left, parent->a, parent->aValue);
        else if(parent->numKeys == 2 && parent->middleRight->b == cur->b && parent->middleLeft->numKeys != 1) rotate(cur, parent, parent->middleLeft, parent->b, parent->bValue);
        else if(parent->numKeys == 3 && parent->middleRight->b == cur->b && parent->right->numKeys != 1) rotate(cur, parent, parent->right, parent->c, parent->cValue);
        else if(parent->numKeys == 3 && parent->middleRight->b == cur->b && parent->middleLeft->numKeys != 1) rotate(cur, parent, parent->middleLeft, parent->b, parent->bValue);
        else if(parent->numKeys == 3 && parent->right->b == cur->b && parent->middleRight->numKeys != 1) rotate(cur, parent, parent->middleRight, parent->c, parent->cValue);

        //if we make it this far, then no rotation is possible and we must perform a merge
        //adjacent siblings are 2 nodes, no rotation can occur, parent is a 2 node or 3 node
        else if(parent->left->b == cur->b) cur = merge(cur, parent, parent->middleLeft, parent->a, parent->aValue, "less");
        else if(parent->middleLeft->b == cur->b) cur = merge(cur, parent, parent->middleRight, parent->b, parent->bValue, "less");
        else if(parent->numKeys == 2 && parent->middleRight->b == cur->b) cur = merge(cur, parent, parent->middleLeft, parent->b, parent->bValue, "more");
        else if(parent->numKeys == 3 && parent->middleRight->b == cur->b) cur = merge(cur, parent, parent->right, parent->c, parent->cValue, "less");
        else if(parent->numKeys == 3 && parent->right->b == cur->b) cur = merge(cur, parent, parent->middleRight, parent->c, parent->cValue, "more");
        
        
        //we have fixed so now we recursively call our remove function
        return remove(k, cur);
    }   


    else{
        if(cur->middleLeft != NULL){
            Node<keytype, valuetype> * returned;
            bool succ;
            bool eval = false;
            if(k == cur->a && cur->left->numKeys != 1){
                returned = predecessorDelete(k, cur->left);
                succ = false;
                eval = true;
            }
            else if(cur->numKeys != 1 && k == cur->a && cur->middleLeft->numKeys != 1){
                returned = successorDelete(k, cur->middleLeft);
                succ = true;
                eval = true;
            }
            else if(k == cur->b && cur->middleLeft->numKeys != 1){
                returned =  predecessorDelete(k, cur->middleLeft);
                succ = false; 
                eval = true;
            }
            else if(k == cur->b && cur->middleRight->numKeys != 1){
                returned =  successorDelete(k, cur->middleRight);
                succ = true;
                eval = true;
            }
            else if(cur->numKeys != 1 && k == cur->c && cur->middleRight->numKeys != 1){
                returned =  predecessorDelete(k, cur->middleRight);
                succ = false;
                eval = true;
            }
            else if(cur->numKeys == 3 && k == cur->c && cur->right->numKeys != 1){
                returned =  successorDelete(k, cur->right);
                succ = true;
                eval = true;
            }
            if(eval){
                keytype key;
                valuetype val;
                if(returned->numKeys == 1){
                    key = returned->b;
                    val = returned->bValue;
                }
                else if(succ && returned->numKeys == 2){
                    key = returned->a;
                    val = returned->aValue;
                }
                else if(!succ && returned->numKeys == 2){
                    key = returned->b;
                    val = returned->bValue;
                }
                else if(succ && returned->numKeys == 3){
                    key = returned->a;
                    val = returned->aValue;
                }
                else if(!succ && returned->numKeys == 3){
                    key = returned->c;
                    val = returned->cValue;
                }
                
                if(!succ && cur->numKeys != 1 && k == cur->a){ 
                    cur->a = key;
                    cur->aValue = val;
                }
                else if(succ && cur->numKeys != 1 && k == cur->a){ 
                    cur->a = key;
                    cur->aValue = val;
                }
                else if(!succ && k == cur->b){
                    cur->b = key;
                    cur->bValue = val;
                }
                else if(succ && k == cur->b){
                    cur->b = key;
                    cur->bValue = val;
                }
                else if(!succ && k == cur->c){
                    cur->c = key;
                    cur->cValue = val;
                }
                else if(succ && k == cur->c){
                    cur->c = key;
                    cur->cValue = val;
                }
            }

            else{
                //no predecessor or successor
                //so we merge left child, right child, and parent key and then recurively remove k
                //what if the merged element is in the root? we go ahead and remove the element
                //write helper function that deletes at 
                if(k == cur->a){
                    cur = merge(cur->left, cur, cur->middleLeft, cur->a, cur->aValue, "less");
                }
                else if(k == cur->b){
                    cur = merge(cur->middleLeft, cur, cur->middleRight, cur->b, cur->bValue, "less");
                }
                else if(k == cur->c){
                    cur = merge(cur->middleRight, cur, cur->right, cur->c, cur->cValue, "less"); 
                }
                return remove(k, cur);
            }
            
        }

        //our key is in a leaf that is not a 2-node
        else{
            //if  node numKeys == 2 and key == a decrement the keynum
            if(cur->numKeys == 2 && k == cur->a) {
                cur->numKeys--;
            }
            //if node numkeys == 2 and key == b, copy a to b and decrement keynum
            else if(cur->numKeys == 2 && k == cur->b){
                cur->b = cur->a;
                cur->bValue = cur->aValue;
                cur->numKeys--;
            }
            //if node numkeys == 3 and key == a, copy b to a and c to b
            else if(cur->numKeys == 3 && k == cur->a){
                cur->a = cur->b;
                cur->aValue = cur->bValue;
                cur->b = cur->c;
                cur->bValue = cur->cValue;
                cur->numKeys--;

            }
            //if node numkeys == 3 and key == b, copy c to b
            else if(cur->numKeys == 3 && k == cur->b){
                cur->b = cur->c;
                cur->bValue = cur->cValue;
                cur->numKeys--;
            }
            else if(cur->numKeys == 3 && k == cur->c) cur->numKeys--;
            else return 0;

            subtractKidNum(cur);
        }
    }
    return 1;
}



template <typename keytype, typename valuetype>
void Two4Tree<keytype, valuetype>::rotate(Node<keytype, valuetype> *cur, Node<keytype, valuetype> *parent, Node<keytype, valuetype> *sibling, keytype &pKey, valuetype &pValue){
    //2 possible cases: left rotation or right rotation
    //if cur b < cur sibling we do a left rotation: we take pkey and copy it to rightmost node in cur, we take leftmost key in sibling (a) copy it to parent 
    if(cur->b < sibling->b){
        //copy pkey to rightmost node in cur, cur is 2 node so we copy b over to a and place our new node in a
        cur->a = cur->b;
        cur->aValue = cur->bValue;
        cur->b = pKey;
        cur->bValue = pValue;
        pKey = sibling->a;
        pValue = sibling->aValue;
        cur->numKeys = 2;
        sibling->numKeys--;

        if(sibling->b == parent->middleRight->b){
            parent->middleLeftKids++;
            parent->middleRightKids--;
        }
        else if(sibling->b == parent->middleLeft->b){
            parent->leftKids++;
            parent->middleLeftKids--;
        }
        else if(sibling->b == parent->right->b){
            parent->middleRightKids++;
            parent->rightKids--;
        }
        
        
        //if children exists, the middleright gains another kid bc it receives the sibling child, sibling will LOSE another kid
        
        //if there are children, then cur children get moved over and the left child in sibling gets moved over to cur and if sibling has 2 keys move children over
        if(cur->middleLeft != NULL){
            cur->left = cur->middleLeft;
            cur->middleLeft = cur->middleRight;
            cur->middleRight = sibling->left; 
            cur->middleRight->parent = cur;

            cur->leftKids = cur->middleLeftKids;
            cur->middleLeftKids = cur->middleRightKids;
            cur->middleRightKids = sibling->leftKids;
            
            for(int i = 0; i < sibling->leftKids; i++){ 
                subtractKidNum(sibling);
                addKidNum(cur);
            }

            if(sibling->numKeys == 2){
                sibling->left = sibling->middleLeft;
                sibling->middleLeft = sibling->middleRight;
                sibling->middleRight = sibling->right;
                sibling->right == NULL;

                sibling->leftKids = sibling->middleLeftKids;
                sibling->middleLeftKids = sibling->middleRightKids;
                sibling->middleRightKids = sibling->rightKids;

                sibling->a = sibling->b;
                sibling->aValue = sibling->bValue;
                sibling->b = sibling->c;
                sibling->bValue = sibling->cValue;
            }
        }

    }

    //if cur b > cur sibling we do a right rotation: we take pkey and copy it to leftmost node in cur, we take rightmost key in sibling and copy it to parent
    //leftmost node in cur is always going to be a, and if sibling numkeys == 3, rightmost is c, numkeys == 1 or 2, rightmost is b
    else if(cur->b > sibling->b){

        //copy pkey to leftmost node in cur, cur is 2 node so we copy sibling rightmost over to pkey
        keytype sKey;
        valuetype sValue;
        if(sibling->numKeys == 3){ 
            sKey = sibling->c;
            sValue = sibling->cValue;
        }
        else {
            sKey = sibling->b;
            sValue = sibling->bValue;
        }

        cur->a = pKey;
        cur->aValue = pValue;
        pKey = sKey;
        pValue = sValue;
        cur->numKeys = 2;
        sibling->numKeys--;

        if(sibling->numKeys == 1){
            sibling->b = sibling->a;
            sibling->bValue = sibling->aValue;

        }

        if(sibling->b == parent->middleLeft->b){
            parent->middleRightKids++;
            parent->middleLeftKids--;
        }
        else if(sibling->b == parent->left->b){
            parent->middleLeftKids++;
            parent->leftKids--;
        }
        else if(sibling->b == parent->middleRight->b){
            parent->rightKids++;
            parent->middleRightKids--;
        }


        //if sibling has 1 key, then we move middleright over to left child in cur and move middleleft to middleright and left to middleLeft
        //if sibling has 2 keys, then you copy right child over to left child in cur and no additional moving required
        if(cur->middleLeft != NULL){
        
            if(sibling->numKeys == 1){

                for(int i = 0; i < sibling->middleRightKids; i++){
                    addKidNum(cur);
                    subtractKidNum(sibling);
                }

                cur->left = sibling->middleRight;
                sibling->middleRight = sibling->middleLeft;
                sibling->middleLeft = sibling->left; 

                cur->leftKids = sibling->middleRightKids;
                sibling->middleRightKids = sibling->middleLeftKids;
                sibling->middleLeftKids = sibling->leftKids;

            }
            else{
                for(int i = 0; i < sibling->rightKids; i++){
                    addKidNum(cur);
                    subtractKidNum(sibling);
                }
                cur->left = sibling->right;
                cur->leftKids = sibling->rightKids; 
            }

            cur->left->parent = cur;
        }
    }
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype>* Two4Tree<keytype, valuetype>::merge(Node<keytype, valuetype> *cur, Node<keytype, valuetype> *parent, Node<keytype, valuetype> *sibling, keytype &pKey, valuetype &pValue, string flag){
    //need to know if less or more
    Node<keytype, valuetype> *newNode = new Node<keytype, valuetype>(3);
    //cur key is less than sibling key
    if(flag == "less"){
        newNode->a = cur->b;
        newNode->aValue = cur->bValue;
        newNode->b = pKey;
        newNode->bValue = pValue;
        newNode->c = sibling->b;
        newNode->cValue = sibling->bValue;
        newNode->left = cur->middleLeft;
        newNode->middleLeft = cur->middleRight;
        newNode->middleRight = sibling->middleLeft;
        newNode->right = sibling->middleRight;

        newNode->leftKids = cur->middleLeftKids;
        newNode->middleLeftKids = cur->middleRightKids;
        newNode->middleRightKids = sibling->middleLeftKids;
        newNode->rightKids = sibling->middleRightKids; 
    }
    //cur is more than sibling
    else{
        newNode->a = sibling->b;
        newNode->aValue = sibling->bValue;
        newNode->b = pKey;
        newNode->bValue = pValue;
        newNode->c = cur->b;
        newNode->cValue = cur->bValue;
        newNode->left = sibling->middleLeft;
        newNode->middleLeft = sibling->middleRight;
        newNode->middleRight = cur->middleLeft;
        newNode->right = cur->middleRight;

        newNode->leftKids = sibling->middleLeftKids;
        newNode->middleLeftKids = sibling->middleRightKids;
        newNode->middleRightKids = cur->middleLeftKids;
        newNode->rightKids = cur->middleRightKids;
    }

    newNode->parent = parent; 

    if(newNode->middleLeft != NULL){
        newNode->left->parent = newNode;
        newNode->middleLeft->parent = newNode;
        newNode->middleRight->parent = newNode;
        newNode->right->parent = newNode;
    }
    parent->numKeys--;  

    //now we need to handle the parent
    //we need to decrease the num nodes in parent
    //if we removed the a node from parent and numKeys == 2, we move b to a and c to b
    //we know that we removed the a node if pKey == parent->a
    if(pKey == parent->a && parent->numKeys == 2){
        parent->a = parent->b;
        parent->aValue = parent->bValue;
        parent->b = parent->c;
        parent->bValue = parent->cValue;
    }
    //if we removed the b node from parent and numnodes == 1, we move a to b
    else if(pKey == parent->b && parent->numKeys == 1){
        parent->b = parent->a;
        parent->bValue = parent->aValue;
    }
    //if we removed the b node from parent and numnodes == 2, we move c to b
    else if(pKey == parent->b && parent->numKeys == 2){
        parent->b = parent->c;
        parent->bValue = parent->cValue;
    }

    //we need to reset the parent children to the right value
    if(parent->left->b == cur->b && parent->numKeys == 1){
        parent->middleLeft = newNode;
        parent->middleLeftKids = 3 + newNode->leftKids + newNode->middleLeftKids + newNode->middleRightKids + newNode->rightKids;   
    }
    else if(parent->middleLeft->b == cur->b && parent->numKeys == 1){
        parent->middleLeft = parent->left;
        parent->middleRight = newNode;

        parent->middleLeftKids = parent->leftKids;
        parent->middleRightKids = 3 + newNode->leftKids + newNode->middleLeftKids + newNode->middleRightKids + newNode->rightKids;
    }
    else if(parent->middleRight->b == cur->b && parent->numKeys == 1){
        parent->middleLeft = parent->left;
        parent->middleRight = newNode;

        parent->middleLeftKids = parent->leftKids;
        parent->middleRightKids = 3 + newNode->leftKids + newNode->middleLeftKids + newNode->middleRightKids + newNode->rightKids;
    }
    else if(parent->left->b == cur->b && parent->numKeys == 2){
        parent->left = newNode;
        parent->middleLeft = parent->middleRight;
        parent->middleRight = parent->right;

        parent->middleLeftKids = parent->middleRightKids;
        parent->middleRightKids = parent->rightKids;
        parent->leftKids = 3 + newNode->leftKids + newNode->middleLeftKids + newNode->middleRightKids + newNode->rightKids;
    }
    else if(parent->middleLeft->b == cur->b && parent->numKeys == 2){
        parent->middleLeft = newNode;
        parent->middleRight = parent->right;

        parent->middleRightKids = parent->rightKids; 
        parent->middleLeftKids = 3 + newNode->leftKids + newNode->middleLeftKids + newNode->middleRightKids + newNode->rightKids;
    }
    else if((parent->middleRight->b == cur->b || parent->right->b == cur->b) && parent->numKeys == 2){
        parent->middleRight = newNode;
        parent->middleRightKids = 3 + newNode->leftKids + newNode->middleLeftKids + newNode->middleRightKids + newNode->rightKids;
    }
    delete cur;
    delete sibling;
    numNodes -= 1; 
    return newNode;
}

//will return a 2 node if it steps on a 2 node and will return the node if the element is found 
template <typename keytype, typename valuetype>
Node<keytype, valuetype> * Two4Tree<keytype, valuetype>::searchDelete(keytype k, Node<keytype, valuetype> *cur){
    //if cur node null, not found
    if(cur == NULL) return NULL;

    //does current node contain key
    //if cur node is a 2 node return current node
    if(cur->numKeys == 1) return cur;
    //if cur node is a 3 node does a contain key or does b contain key
    else if(cur->numKeys == 2 && (k == cur->a || k == cur->b)) return cur;
    //if cur node is a 4 node, does a contain key, b contain key, or c contain key 
    else if(cur->numKeys == 3 && (k == cur->a  ||k == cur->b || k == cur->c))return cur;

    //if current node doesnt contain key
        //if currentNode is a 3 node, if key < a go left 
        else if(cur->numKeys == 2 && k < cur->a) return searchDelete(k, cur->left);
        //if currentNode is a 3 node, if key < b go middleLeft
        else if(cur->numKeys == 2 && k < cur->b) return searchDelete(k, cur->middleLeft);
        //if currentNode is a 3 node, if key > b go middleRight
        else if(cur->numKeys == 2 && k > cur->b) return searchDelete(k, cur->middleRight);
        //if currentNode is a 4 node, if key < a go left  
        else if(cur->numKeys == 3 && k < cur->a) return searchDelete(k, cur->left);
        //if currentNode is a 4 node, if key < b go middleLeft 
        else if(cur->numKeys == 3 && k < cur->b) return searchDelete(k, cur->middleLeft);
        //if currentNode is a 4 node, if key < c go middleRight
        else if(cur->numKeys == 3 && k < cur->c) return searchDelete(k, cur->middleRight);
        //if currentNode is a 4 node, if key > c go right  
        else if(cur->numKeys == 3 && k > cur->c) return searchDelete(k, cur->right);
}


//return pointer to key if there is a key in a non 2 node, return null otherwise
template <typename keytype, typename valuetype>
Node<keytype, valuetype>* Two4Tree<keytype, valuetype>::predecessorDelete(keytype k, Node<keytype, valuetype> *cur){
    //now we make our way to right leaf
    if(cur->numKeys == 1){
            Node<keytype, valuetype> *parent = cur->parent;
            if(parent->numKeys == 1 && parent->middleLeft->b == cur->b && parent->middleRight->numKeys != 1) rotate(cur, parent, parent->middleRight, parent->b, parent->bValue);
            else if(parent->numKeys == 1 && parent->middleRight->b == cur->b && parent->middleLeft->numKeys != 1) rotate(cur, parent, parent->middleLeft, parent->b, parent->bValue);
            else if(parent->left->b == cur->b && parent->middleLeft->numKeys != 1) rotate(cur, parent, parent->middleLeft, parent->a, parent->aValue); 
            else if(parent->middleLeft->b == cur->b && parent->middleRight->numKeys != 1) rotate(cur, parent, parent->middleRight, parent->b, parent->bValue);
            else if(parent->middleLeft->b == cur->b && parent->left->numKeys != 1) rotate(cur, parent, parent->left, parent->a, parent->aValue);
            else if(parent->numKeys == 2 && parent->middleRight->b == cur->b && parent->middleLeft->numKeys != 1) rotate(cur, parent, parent->middleLeft, parent->b, parent->bValue);
            else if(parent->numKeys == 3 && parent->middleRight->b == cur->b && parent->right->numKeys != 1) rotate(cur, parent, parent->right, parent->b, parent->bValue);
            else if(parent->numKeys == 3 && parent->middleRight->b == cur->b && parent->middleLeft->numKeys != 1) rotate(cur, parent, parent->middleLeft, parent->b, parent->bValue);
            else if(parent->numKeys == 3 && parent->right->b == cur->b && parent->middleRight->numKeys != 1) rotate(cur, parent, parent->middleRight, parent->b, parent->bValue);

        //if we make it this far, then no rotation is possible and we must perform a merge
        //adjacent siblings are 2 nodes, no rotation can occur, parent is a 2 node or 3 node
        else if(parent->left->b == cur->b) cur = merge(cur, parent, parent->middleLeft, parent->a, parent->aValue, "less");
        else if(parent->middleLeft->b == cur->b) cur = merge(cur, parent, parent->middleRight, parent->b, parent->bValue, "less");
        else if(parent->numKeys == 2 && parent->middleRight->b == cur->b) cur = merge(cur, parent, parent->middleLeft, parent->b, parent->bValue, "more");
        else if(parent->numKeys == 3 && parent->middleRight->b == cur->b) cur = merge(cur, parent, parent->right, parent->c, parent->cValue, "less");
        else if(parent->numKeys == 3 && parent->right->b == cur->b) cur = merge(cur, parent, parent->middleRight, parent->c, parent->cValue, "more");
    }
    if(cur->middleLeft != NULL){
        //if numkeys == 1 recursively fix
        if(cur->numKeys == 2) return predecessorDelete(k, cur->middleRight);
        //if numkeys == 3, go right
        else if(cur->numKeys == 3) return predecessorDelete(k, cur->right); 
    }

    //if we can't go right anymore, then we return rightmost key in cur node
    else{
        Node<keytype, valuetype> *temp = new Node<keytype, valuetype>(cur->numKeys);
        if(temp->numKeys == 2){
            temp->a = cur->a;
            temp->aValue = cur->aValue;
            temp->b = cur->b;
            temp->bValue = cur->bValue;
        }
        if(temp->numKeys == 3){
            temp->a = cur->a;
            temp->aValue = cur->aValue;
            temp->b = cur->b;
            temp->bValue = cur->bValue;
            temp->c = cur->c;
            temp->cValue = cur->cValue;
        }

        if(cur->numKeys == 2){
            cur->b = cur->a;
            cur->bValue = cur->aValue;
            cur->numKeys--;
        }
        else if(cur->numKeys == 3){
            cur->numKeys--;
        }

        subtractKidNum(cur);
        return temp;
    }

}

template <typename keytype, typename valuetype>
Node <keytype, valuetype>* Two4Tree<keytype, valuetype>::successorDelete(keytype k, Node<keytype, valuetype> *cur){
    //now we make our way to leftmost leaf
    if(cur->numKeys == 1){
            Node<keytype, valuetype> *parent = cur->parent;
            if(parent->numKeys == 1 && parent->middleLeft->b == cur->b && parent->middleRight->numKeys != 1) rotate(cur, parent, parent->middleRight, parent->b, parent->bValue);
            else if(parent->numKeys == 1 && parent->middleRight->b == cur->b && parent->middleLeft->numKeys != 1) rotate(cur, parent, parent->middleLeft, parent->b, parent->bValue);
            else if(parent->left->b == cur->b && parent->middleLeft->numKeys != 1) rotate(cur, parent, parent->middleLeft, parent->a, parent->aValue); 
            else if(parent->middleLeft->b == cur->b && parent->middleRight->numKeys != 1) rotate(cur, parent, parent->middleRight, parent->b, parent->bValue);
            else if(parent->middleLeft->b == cur->b && parent->left->numKeys != 1) rotate(cur, parent, parent->left, parent->a, parent->aValue);
            else if(parent->numKeys == 2 && parent->middleRight->b == cur->b && parent->middleLeft->numKeys != 1) rotate(cur, parent, parent->middleLeft, parent->b, parent->bValue);
            else if(parent->numKeys == 3 && parent->middleRight->b == cur->b && parent->right->numKeys != 1) rotate(cur, parent, parent->right, parent->b, parent->bValue);
            else if(parent->numKeys == 3 && parent->middleRight->b == cur->b && parent->middleLeft->numKeys != 1) rotate(cur, parent, parent->middleLeft, parent->b, parent->bValue);
            else if(parent->numKeys == 3 && parent->right->b == cur->b && parent->middleRight->numKeys != 1) rotate(cur, parent, parent->middleRight, parent->b, parent->bValue);

        //if we make it this far, then no rotation is possible and we must perform a merge
        //adjacent siblings are 2 nodes, no rotation can occur, parent is a 2 node or 3 node
        else if(parent->left->b == cur->b) cur = merge(cur, parent, parent->middleLeft, parent->a, parent->aValue, "less");
        else if(parent->middleLeft->b == cur->b) cur = merge(cur, parent, parent->middleRight, parent->b, parent->bValue, "less");
        else if(parent->numKeys == 2 && parent->middleRight->b == cur->b) cur = merge(cur, parent, parent->middleLeft, parent->b, parent->bValue, "more");
        else if(parent->numKeys == 3 && parent->middleRight->b == cur->b) cur = merge(cur, parent, parent->right, parent->c, parent->cValue, "less");
        else if(parent->numKeys == 3 && parent->right->b == cur->b) cur = merge(cur, parent, parent->middleRight, parent->c, parent->cValue, "more");
    }
    
    if(cur->middleLeft != NULL){
        //if numkeys == 1 go midleft


        //if numkeys == 2 or numkeys == 3, go left
         if(cur->numKeys == 2 || cur->numKeys == 3){
            return successorDelete(k, cur->left); 
        }
    }

    //if we can't go left anymore, then we return leftmost key in cur node
    else{
        Node<keytype, valuetype> *temp = new Node<keytype, valuetype>(cur->numKeys);
        if(temp->numKeys == 2){
            temp->a = cur->a;
            temp->aValue = cur->aValue;
            temp->b = cur->b;
            temp->bValue = cur->bValue;
        }
        if(temp->numKeys == 3){
            temp->a = cur->a;
            temp->aValue = cur->aValue;
            temp->b = cur->b;
            temp->bValue = cur->bValue;
            temp->c = cur->c;
            temp->cValue = cur->cValue;
        }

        if(cur->numKeys == 2){
            cur->numKeys--;
        }
        else if(cur->numKeys == 3){
            cur->a = cur->b;
            cur->aValue = cur->bValue;
            cur->b = cur->c;
            cur->bValue = cur->cValue;
            cur->numKeys--;
        }

        subtractKidNum(cur);
        return temp;
    }
}



template <typename keytype, typename valuetype>
keytype Two4Tree<keytype, valuetype>::successor(keytype k){
    //first we are going to go to the location where our key is at
    Node<keytype, valuetype> *cur = searchKey(k, root);
    
    //if current node is root or internal node, then we move to the right once and go as far left as possible
    if(cur->middleLeft != NULL){
        if(cur->numKeys == 1) cur = cur->middleRight;
        else if(cur->numKeys == 2 && k == cur->a) cur = cur->middleLeft;
        else if(cur->numKeys == 2 && k == cur->b) cur = cur->middleRight;
        else if(cur->numKeys == 3 && k == cur->a) cur = cur->middleLeft; 
        else if(cur->numKeys == 3 && k == cur->b) cur = cur->middleRight; 
        else if(cur->numKeys == 3 && k == cur->c) cur = cur->right;
        return successorInternal(k, cur);
    }

    //if leaf node and not rightmost element in leaf
    if(cur->numKeys == 2 && k == cur->a) return cur->b;
    else if(cur->numKeys == 3 && k == cur->a) return cur->b;
    else if(cur->numKeys == 3 && k == cur->b) return cur->c;

    //leaf node and rightmost element in leaf
    else return successorLeaf(k, cur);
}

//successor will be the first parent that is greater than the key
template <typename keytype, typename valuetype>
keytype Two4Tree<keytype, valuetype>::successorLeaf(keytype k, Node<keytype, valuetype> *cur){
    Node <keytype, valuetype> *p = cur->parent; 
    //if parent == null, successor not found
    if(p == NULL) return NULL;
    //if parent keynum == 1 and k < parent b, then parent b is successor
    if(p->numKeys == 1 && k < p->b)return p->b;
    //if parent keynum == 2 and k < parent a, a is successor
    else if(p->numKeys == 2 && k < p->a) return p->a;
    //if parent keynum == 2 and k < parent b, b is successor
    else if(p->numKeys == 2 && k < p->b) return p->b;
    //if parent keynum == 3 and k < parent a, a is successor
    else if(p->numKeys == 3 && k < p->a) return p->a;
    //if parent keynum == 3 and k < parent b, b is successor
    else if(p->numKeys == 3 && k < p->b) return p->b;
    //if parent keynum == 3 and k < parent c, c is successor
    else if(p->numKeys == 3 && k < p->c) return p->c;
    else return successorLeaf(k, p); //recursing on parent
}

//write search first
template <typename keytype, typename valuetype>
keytype Two4Tree<keytype, valuetype>::successorInternal(keytype k, Node<keytype, valuetype> *cur){
    //now we make our way to right leaf
    if(cur->middleLeft != NULL){
        //if numkeys == 1 go midleft
        if(cur->numKeys == 1) return successorInternal(k, cur->middleLeft);
        //if numkeys == 2 or numkeys == 3, go left
        else if(cur->numKeys == 2 || cur->numKeys == 3) return successorInternal(k, cur->left); 
    }

    //if we can't go left anymore, then we return leftmost key in cur node
    else{
        //if numkeys == 1, return b
        if(cur->numKeys == 1) return cur->b;
        //if numkeys == 2 or numkeys == 3, return a
        if(cur->numKeys == 2 || cur->numKeys == 3) return cur->a;
    }
}

template <typename keytype, typename valuetype>
keytype Two4Tree<keytype, valuetype>::predecessor(keytype k){
    //first we are going to go to the location where our key is at
    Node<keytype, valuetype> *cur = searchKey(k, root);
    
    //if current node is root or internal node, then we move to the left once and go as far right as possible
    if(cur->middleLeft != NULL){
        if(cur->numKeys == 1) cur = cur->middleLeft;
        else if(cur->numKeys == 2 && k == cur->a) cur = cur->left;
        else if(cur->numKeys == 2 && k == cur->b) cur = cur->middleLeft;
        else if(cur->numKeys == 3 && k == cur->a) cur = cur->left; 
        else if(cur->numKeys == 3 && k == cur->b) cur = cur->middleLeft; 
        else if(cur->numKeys == 3 && k == cur->c) cur = cur->middleRight;
        return predecessorInternal(k, cur);
    }

    //if leaf node and not leftmost element in leaf
    if(cur->numKeys == 2 && k == cur->b) return cur->a;
    else if(cur->numKeys == 3 && k == cur->b) return cur->a;
    else if(cur->numKeys == 3 && k == cur->c) return cur->b;

    //leaf node and rightmost element in leaf
    else return predecessorLeaf(k, cur);
}

template <typename keytype, typename valuetype>
keytype Two4Tree<keytype, valuetype>::predecessorInternal(keytype k, Node<keytype, valuetype> *cur){
    //now we make our way to right leaf
    if(cur->middleLeft != NULL){
        //if numkeys == 1 go midright
        if(cur->numKeys == 1 || cur->numKeys == 2) return predecessorInternal(k, cur->middleRight);
        //if numkeys == 3, go right
        else if(cur->numKeys == 3) return predecessorInternal(k, cur->right); 
    }

    //if we can't go right anymore, then we return rightmost key in cur node
    else{
        //if numkeys == 1 or 2, return b
        if(cur->numKeys == 1 || cur->numKeys == 2) return cur->b;
        //if numkeys == 3, return c
        if(cur->numKeys == 3) return cur->c;
    }
}

template <typename keytype, typename valuetype>
keytype Two4Tree<keytype, valuetype>::predecessorLeaf(keytype k, Node<keytype, valuetype> *cur){
    Node <keytype, valuetype> *p = cur->parent; 
    //if parent == null, successor not found
    if(p == NULL) return NULL;
    //if parent keynum == 1 and k < parent b, then parent b is successor
    if(p->numKeys == 1 && k > p->b)return p->b;
    //if parent keynum == 2 and k < parent a, a is successor
    else if(p->numKeys == 2 && k > p->b) return p->b;
    //if parent keynum == 2 and k < parent b, b is successor
    else if(p->numKeys == 2 && k > p->a) return p->a;
    //if parent keynum == 3 and k  parent a, a is successor
    else if(p->numKeys == 3 && k > p->c) return p->c;
    //if parent keynum == 3 and k < parent b, b is successor
    else if(p->numKeys == 3 && k > p->b) return p->b;
    //if parent keynum == 3 and k < parent c, c is successor
    else if(p->numKeys == 3 && k > p->a) return p->a;
    else return predecessorLeaf(k, p); //recursing on parent
}

template <typename keytype, typename valuetype>
valuetype * Two4Tree<keytype,valuetype>::search(keytype k){
    return search(k, root);
}

template <typename keytype, typename valuetype>
valuetype * Two4Tree<keytype,valuetype>::search(keytype k, Node<keytype, valuetype> *cur){
    //if cur node null, not found
    if(cur == NULL) return NULL;

    
    //does current node contain key
    //if cur node is a 2 node, does b = key
    if(cur->numKeys == 1 && k == cur->b) return &(cur->bValue);
    //if cur node is a 3 node does a contain key or does b contain key
    else if(cur->numKeys == 2 && k == cur->a) return &(cur->aValue);
    else if(cur->numKeys == 2 && k == cur->b) return &(cur->bValue);
    //if cur node is a 4 node, does a contain key, b contain key, or c contain key 
    else if(cur->numKeys == 3 && k == cur->a) return &(cur->aValue);
    else if(cur->numKeys == 3 && k == cur->b) return &(cur->bValue);
    else if(cur->numKeys == 3 && k == cur->c) return &(cur->cValue);

    //if current node doesnt contain key
        //if current node is a 2 node, if key < b go middleLeft
        if(cur->numKeys == 1 && k < cur->b)
            return search(k, cur->middleLeft);
        //if current node is a 2 node, if key > b go middleRight
        else if(cur->numKeys == 1 && k > cur->b) return search(k, cur->middleRight);
        //if currentNode is a 3 node, if key < a go left 
        else if(cur->numKeys == 2 && k < cur->a) return search(k, cur->left);
        //if currentNode is a 3 node, if key < b go middleLeft
        else if(cur->numKeys == 2 && k < cur->b) return search(k, cur->middleLeft);
        //if currentNode is a 3 node, if key > b go middleRight
        else if(cur->numKeys == 2 && k > cur->b) return search(k, cur->middleRight);
        //if currentNode is a 4 node, if key < a go left  
        else if(cur->numKeys == 3 && k < cur->a) return search(k, cur->left);
        //if currentNode is a 4 node, if key < b go middleLeft 
        else if(cur->numKeys == 3 && k < cur->b) return search(k, cur->middleLeft);
        //if currentNode is a 4 node, if key < c go middleRight
        else if(cur->numKeys == 3 && k < cur->c) return search(k, cur->middleRight);
        //if currentNode is a 4 node, if key > c go right  
        else if(cur->numKeys == 3 && k > cur->c) return search(k, cur->right);
}

//this is used for the purpose of finding a successor
template <typename keytype, typename valuetype>
Node<keytype, valuetype> * Two4Tree<keytype, valuetype>::searchKey(keytype k, Node<keytype, valuetype> *cur){
    //if cur node null, not found
    if(cur == NULL) return NULL;

    
    //does current node contain key
    //if cur node is a 2 node, does b = key
    if(cur->numKeys == 1 && k == cur->b) return cur;
    //if cur node is a 3 node does a contain key or does b contain key
    else if(cur->numKeys == 2 && (k == cur->a || k == cur->b)) return cur;
    //if cur node is a 4 node, does a contain key, b contain key, or c contain key 
    else if(cur->numKeys == 3 && (k == cur->a  ||k == cur->b || k == cur->c)) return cur;

    //if current node doesnt contain key
        //if current node is a 2 node, if key < b go middleLeft
        if(cur->numKeys == 1 && k < cur->b) return searchKey(k, cur->middleLeft);
        //if current node is a 2 node, if key > b go middleRight
        else if(cur->numKeys == 1 && k > cur->b) return searchKey(k, cur->middleRight);
        //if currentNode is a 3 node, if key < a go left 
        else if(cur->numKeys == 2 && k < cur->a) return searchKey(k, cur->left);
        //if currentNode is a 3 node, if key < b go middleLeft
        else if(cur->numKeys == 2 && k < cur->b) return searchKey(k, cur->middleLeft);
        //if currentNode is a 3 node, if key > b go middleRight
        else if(cur->numKeys == 2 && k > cur->b) return searchKey(k, cur->middleRight);
        //if currentNode is a 4 node, if key < a go left  
        else if(cur->numKeys == 3 && k < cur->a) return searchKey(k, cur->left);
        //if currentNode is a 4 node, if key < b go middleLeft 
        else if(cur->numKeys == 3 && k < cur->b) return searchKey(k, cur->middleLeft);
        //if currentNode is a 4 node, if key < c go middleRight
        else if(cur->numKeys == 3 && k < cur->c) return searchKey(k, cur->middleRight);
        //if currentNode is a 4 node, if key > c go right  
        else if(cur->numKeys == 3 && k > cur->c) return searchKey(k, cur->right);
}


//traverse left subtree
//visit root
//traverse right subtree
template <typename keytype, typename valuetype>
void Two4Tree<keytype, valuetype>::inorder(){
    inorder(root); 
    cout << endl;
}

template <typename keytype, typename valuetype>
void Two4Tree<keytype, valuetype>::inorder(Node<keytype, valuetype> *root){
    if(root == NULL) return;
    //if 2 node, traverse middleLeft
    //visit b
    //traverse middleRight
    if(root->numKeys == 1){
        inorder(root->middleLeft);
        cout << root->b;
        cout << " ";
        inorder(root->middleRight);
    }
    //if 3 node, traverse left
    //visit a
    //traverse middleLeft
    //visit b
    //traverse middleRight
    else if(root->numKeys == 2){
        inorder(root->left);
        cout << root->a;
        cout << " ";
        inorder(root->middleLeft);
        cout << root->b;
        cout << " ";
        inorder(root->middleRight);
    }

    //if 4 node, traverse left
    //visit a
    //traverse middleleft
    //visit b
    //traverse middleRight
    //visit c
    //traverse right 
    else if(root->numKeys == 3){
        inorder(root->left);
        cout << root->a;
        cout << " ";
        inorder(root->middleLeft);
        cout << root->b;
        cout << " ";
        inorder(root->middleRight);
        cout << root->c; 
        cout << " ";
        inorder(root->right);
    }
}

template<typename keytype, typename valuetype>
keytype Two4Tree<keytype, valuetype>::select(int pos){
    return select(root, pos, 0);
}
template<typename keytype, typename valuetype>
keytype Two4Tree<keytype, valuetype>::select(Node<keytype, valuetype> *cur, int pos, int total){
    if(cur->middleLeft == NULL && cur->numKeys == 1 && (total + 1 == pos)) return cur->b;
    else if(cur->middleLeft == NULL && cur->numKeys != 1 && (total + 1 == pos)) return cur->a;
    else if(cur->middleLeft == NULL && cur->numKeys != 1 && (total + 2 == pos)) return cur->b;
    else if(cur->middleLeft == NULL && cur->numKeys != 1 && (total + 3 == pos)) return cur->c;

    if(cur->numKeys == 1 && 1 + total + cur->middleLeftKids == pos) return cur->b;
    else if(cur->numKeys != 1 && (1 + total + cur->leftKids == pos)) return cur->a;
    else if(cur->numKeys != 1 && (2 + total + cur->leftKids + cur->middleLeftKids) == pos) return cur->b;
    else if(cur->numKeys == 3 && (3 + total + cur->leftKids + cur->middleLeftKids + cur->middleRightKids == pos)) return cur->c;

    else if(cur->numKeys == 1 && (total + cur->middleLeftKids >= pos)) return select(cur->middleLeft, pos, total);
    else if(cur->numKeys == 1 && (1 + total + cur->middleLeftKids + cur->middleRightKids >= pos)) return select(cur->middleRight, pos, 1 + total + cur->middleLeftKids);

    else if((total + cur->leftKids) >= pos) return select(cur->left, pos, total);
    else if((1 + total + cur->leftKids + cur->middleLeftKids) >= pos) return select(cur->middleLeft, pos, 1 + total + cur->leftKids);
    else if((2 + total + cur->leftKids + cur->middleLeftKids + cur->middleRightKids) >= pos) return select(cur->middleRight, pos, 2 + total + cur->leftKids + cur->middleLeftKids);
    else if((3 + total + cur->leftKids + cur->middleLeftKids + cur->middleRightKids + cur->rightKids) >= pos) return select(cur->right, pos, 3 + total + cur->leftKids + cur->middleLeftKids + cur->middleRightKids);

}

template <typename keytype, typename valuetype>
int Two4Tree<keytype, valuetype>::rank(keytype k){
    Node<keytype, valuetype> *cur = searchRank(k, root); 
    if(cur == NULL) return 0;
    int total = 0;

    //getting an error with the rightmost subtree

    if(cur->middleLeft == NULL){
        if(cur->numKeys == 3 && cur->c == k) total = 3;
        else if(cur->numKeys != 1 && cur->b == k) total = 2;
        else if(cur->numKeys == 1 && cur->b == k) total = 1;
        else if(cur->a == k) total = 1;
    }

    else {
        if(cur->numKeys == 1) total = 1 + cur->middleLeftKids;
        else if(cur->numKeys != 1 && k == cur->a) total = 1 + cur->leftKids;  
        else if(cur->numKeys != 1 && k == cur->b) total = 2 + cur->leftKids + cur->middleLeftKids;  
        else if(cur->numKeys == 3 && k == cur->c) total = 3 + cur->leftKids + cur->middleLeftKids + cur->middleRightKids;
    }

    if(cur->parent == NULL) return total;

    while(1){
        Node<keytype, valuetype> *parent = cur->parent;
        if(parent->numKeys == 1 && parent->middleRight->b == cur->b) total += 1 + parent->middleLeftKids;
        else if(parent->numKeys != 1 && parent->middleLeft->b == cur->b) total += 1 + parent->leftKids;
        else if(parent->numKeys != 1 && parent->middleRight->b == cur->b) total += 2 + parent->leftKids + parent->middleLeftKids;
        else if(parent->numKeys == 3 && parent->right->b == cur->b) total += 3 + parent->leftKids + parent->middleLeftKids + parent->middleRightKids; 

        if(parent->parent == NULL) break;    
        cur= cur->parent;

            
            
    }
    return total;
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype>* Two4Tree<keytype,valuetype>::searchRank(keytype k, Node<keytype, valuetype> *cur){
    //if cur node null, not found
    if(cur == NULL) return NULL;

    
    //does current node contain key
    //if cur node is a 2 node, does b = key
    if(cur->numKeys == 1 && k == cur->b) return cur;
    //if cur node is a 3 node does a contain key or does b contain key
    else if(cur->numKeys == 2 && (k == cur->a || k == cur->b)) return cur;
    //if cur node is a 4 node, does a contain key, b contain key, or c contain key 
    else if(cur->numKeys == 3 && (k == cur->a || k == cur->b || k == cur->c)) return cur;


    //if current node doesnt contain key
        //if current node is a 2 node, if key < b go middleLeft
        if(cur->numKeys == 1 && k < cur->b) return searchRank(k, cur->middleLeft);
        //if current node is a 2 node, if key > b go middleRight
        else if(cur->numKeys == 1 && k > cur->b) return searchRank(k, cur->middleRight);
        //if currentNode is a 3 node, if key < a go left 
        else if(cur->numKeys == 2 && k < cur->a) return searchRank(k, cur->left);
        //if currentNode is a 3 node, if key < b go middleLeft
        else if(cur->numKeys == 2 && k < cur->b) return searchRank(k, cur->middleLeft);
        //if currentNode is a 3 node, if key > b go middleRight
        else if(cur->numKeys == 2 && k > cur->b) return searchRank(k, cur->middleRight);
        //if currentNode is a 4 node, if key < a go left  
        else if(cur->numKeys == 3 && k < cur->a) return searchRank(k, cur->left);
        //if currentNode is a 4 node, if key < b go middleLeft 
        else if(cur->numKeys == 3 && k < cur->b) return searchRank(k, cur->middleLeft);
        //if currentNode is a 4 node, if key < c go middleRight
        else if(cur->numKeys == 3 && k < cur->c) return searchRank(k, cur->middleRight);
        //if currentNode is a 4 node, if key > c go right  
        else if(cur->numKeys == 3 && k > cur->c) return searchRank(k, cur->right);
}


template <typename keytype, typename valuetype>
void Two4Tree<keytype, valuetype>::preorder(){
    preorder(root); 
    cout << endl;
}

template <typename keytype, typename valuetype>
void Two4Tree<keytype, valuetype>::preorder(Node<keytype, valuetype> *root){
    if(root == NULL) return;
    //if 2 node, traverse middleLeft
    //visit b
    //traverse middleRight
    if(root->numKeys == 1){
        cout << root->b;
        cout << " ";
        preorder(root->middleLeft);
        preorder(root->middleRight);
    }
    //if 3 node, traverse left
    //visit a
    //traverse middleLeft
    //visit b
    //traverse middleRight
    else if(root->numKeys == 2){
        cout << root->a;
        cout << " ";
        cout << root->b;
        cout << " ";
        preorder(root->left);
        preorder(root->middleLeft);
        preorder(root->middleRight);
    }

    //if 4 node, traverse left
    //visit a
    //traverse middleleft
    //visit b
    //traverse middleRight
    //visit c
    //traverse right 
    else if(root->numKeys == 3){
        cout << root->a;
        cout << " ";
        cout << root->b;
        cout << " ";
        cout << root->c; 
        cout << " ";
        preorder(root->left);
        preorder(root->middleLeft);
        preorder(root->middleRight);
        preorder(root->right);
    }
}


template <typename keytype, typename valuetype>
void Two4Tree<keytype, valuetype>::postorder(){
    postorder(root); 
    cout << endl;
}

template <typename keytype, typename valuetype>
void Two4Tree<keytype, valuetype>::postorder(Node<keytype, valuetype> *root){
    if(root == NULL){
        return;
    }
    //if 2 node, traverse middleLeft
    //visit b
    //traverse middleRight
    if(root->numKeys == 1){
        postorder(root->middleLeft);
        postorder(root->middleRight);
        cout << root->b;
        cout << " ";
    }
    //if 3 node, traverse left
    //visit a
    //traverse middleLeft
    //visit b
    //traverse middleRight
    else if(root->numKeys == 2){
        postorder(root->left);
        postorder(root->middleLeft);
        postorder(root->middleRight);
        cout << root->a;
        cout << " ";
        cout << root->b;
        cout << " ";
    }

    //if 4 node, traverse left
    //visit a
    //traverse middleleft
    //visit b
    //traverse middleRight
    //visit c
    //traverse right 
    else if(root->numKeys == 3){
        postorder(root->left);
        postorder(root->middleLeft);
        postorder(root->middleRight);
        postorder(root->right);
        cout << root->a;
        cout << " ";
        cout << root->b;
        cout << " ";
        cout << root->c; 
        cout << " ";
    }
}

template<typename keytype, typename valuetype>
int Two4Tree<keytype, valuetype>::size(){
    return totalKeys;
}
