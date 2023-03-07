#include "SMMIterator.h"
#include "SortedMultiMap.h"
#include "keyNode.h"

#include <iostream>
#include <vector>
#include <exception>
using namespace std;
/// O(capacity)
SortedMultiMap::SortedMultiMap(Relation r) {
    //TODO - Implementation
    this->relation = r;
    this->capacity = 2;
    this->size_of_entities = 0;
    this->actual_size = 0;
    this->root = NULL_TVALUE;
    this->next_empty = new int[this->capacity];
    for (int i = 0; i < this->capacity - 1; i++)
        this->next_empty[i] = i + 1;
    this->next_empty[this->capacity - 1] = NULL_TVALUE;
    this->firstEmpty = 0;
    this->entities = new Node[this->capacity];
    for (int i = 0; i < this->capacity; i++) {
        this->entities[i].information.elem_capacity = 5;
        this->entities[i].information.values = new TValue[this->entities[i].information.elem_capacity];
        this->entities[i].information.elem_size = 0;
    }
}
///Best case: Theta(1)
///worst case: O(capacity)
///average: O(capacity)
void SortedMultiMap::add(TKey c, TValue v) {
    //if the tree is empty
    if (this->root == NULL_TVALUE) {
        this->entities[this->firstEmpty].information.key = c;
        this->entities[this->firstEmpty].information.values[0] = v;
        this->entities[this->firstEmpty].information.elem_size = 1;
        this->entities[this->firstEmpty].left = NULL_TVALUE;
        this->entities[this->firstEmpty].right = NULL_TVALUE;
        this->actual_size++;
        this->size_of_entities++;
        this->root = this->firstEmpty;
        this->firstEmpty = this->next_empty[this->firstEmpty];
        return;
    }
    //now I have to search for the key in the tree
    int current_node = this->root;
    int previous_node;
    while (current_node != NULL_TVALUE) {
        if (this->entities[current_node].information.key == c) {
            //if the key is already in the tree
            Element& elem = this->entities[current_node].information;
            if (elem.elem_size == elem.elem_capacity) {
                elem.elem_capacity *= 2;
                int* aux_values = new int[elem.elem_capacity];
                for (int i = 0; i < elem.elem_size; i++)
                    aux_values[i] = elem.values[i];
                delete[] elem.values;
                elem.values = aux_values;
            }
            //now I have to insert the value at the right position in the array
            if (v > elem.values[elem.elem_size - 1]) {
                elem.values[elem.elem_size] = v;
                elem.elem_size++;
            }
            else {
                int pos = 0;
                for(int i = 0 ; i < elem.elem_size; i++)
                    if (elem.values[i] > v) {
                        pos = i;
                        break;
                    }
                for (int i = elem.elem_size; i > pos; i--)
                    elem.values[i] = elem.values[i - 1];
                elem.values[pos] = v;
                elem.elem_size++;
            }
            this->actual_size++;
            return;
        }
        else if (this->relation(this->entities[current_node].information.key, c)) {
            previous_node = current_node;
            current_node = this->entities[current_node].right;
        }
        else {
            previous_node = current_node;
            current_node = this->entities[current_node].left;
        }
    }
    //if the entities array is full we have to do a resize
    if (this->size_of_entities == this->capacity)
        this->resize();

    //cout << previous_node;
    this->entities[this->firstEmpty].information.key = c;
    this->entities[this->firstEmpty].information.values[0] = v;
    this->entities[this->firstEmpty].information.elem_size = 1;
    this->entities[this->firstEmpty].left = NULL_TVALUE;
    this->entities[this->firstEmpty].right = NULL_TVALUE;
    if (this->relation(this->entities[previous_node].information.key , c))
        this->entities[previous_node].right = this->firstEmpty;
    else
        this->entities[previous_node].left = this->firstEmpty;
    this->firstEmpty = this->next_empty[firstEmpty];
    this->actual_size++;
    this->size_of_entities++;
}
///Best case: Theta(1)
///worst case: O(length)
///average: O(length)
vector<TValue> SortedMultiMap::search(TKey c) const {
    //TODO - Implementation
    vector<TValue> vec_values{};
    int current_node = this->root;
    while (current_node != NULL_TVALUE) {
        if (this->entities[current_node].information.key == c) {
            Element& elem = this->entities[current_node].information;
            for (int i = 0; i < elem.elem_size; i++)
                vec_values.push_back(elem.values[i]);
            return vec_values;
        }
        else if (this->relation(this->entities[current_node].information.key, c))
            current_node = this->entities[current_node].right;
        else
            current_node = this->entities[current_node].left;
    }
    return vec_values;
}
///Best case: Theta(1)
///worst case: O(length)
///average: O(length)
bool SortedMultiMap::remove(TKey c, TValue v) {
    //TODO - Implementation
    //firstly I have to search for the node
    int current_node = this->root;
    int previous_node = this->root;

    while (current_node != NULL_TVALUE) {
        if (this->entities[current_node].information.key == c) {
            Element& elem = this->entities[current_node].information;
            //I have to check if the key has more than I key
            if (elem.elem_size > 1) {
                int pos{ -1 };
                for (int i = 0; i < elem.elem_size; i++)
                    if (elem.values[i] == v) {
                        pos = i;
                        break;
                    }
                if (pos == -1)
                    return false;
                for (int i = pos; i < elem.elem_size - 1; i++)
                    elem.values[i] = elem.values[i + 1];
                elem.elem_size--;
                this->actual_size--;
                return true;
            }

            if (this->entities[current_node].information.values[0] != v)
                return false;

            //reset the elem.information just to be sure it's ok
            elem.elem_capacity = 5;
            elem.elem_size = 0;
            delete[] elem.values;
            elem.values = new int[5];
            elem.key = NULL_TVALUE;



            //if the node does not have children
            if (this->entities[current_node].left == NULL_TVALUE && this->entities[current_node].right == NULL_TVALUE){
                this->entities[current_node].left = NULL_TVALUE;
                this->entities[current_node].right = NULL_TVALUE;
                if (previous_node != current_node) {
                    if (this->relation(this->entities[previous_node].information.key , c))
                        this->entities[previous_node].right = NULL_TVALUE;
                    else
                        this->entities[previous_node].left = NULL_TVALUE;
                }
                else {
                    this->root = NULL_TVALUE;
                }
                this->next_empty[current_node] = this->firstEmpty;
                this->firstEmpty = current_node;
            }
                //if the node has a right child
            else if (this->entities[current_node].left == NULL_TVALUE) {
                //if I delete the root
                if (previous_node != current_node) {
                    if (this->relation(this->entities[previous_node].information.key, c))
                        this->entities[previous_node].right = this->entities[current_node].right;
                    else
                        this->entities[previous_node].left = this->entities[current_node].right;
                }
                else {
                    this->root = this->entities[current_node].right;
                }
                this->entities[current_node].left = NULL_TVALUE;
                this->entities[current_node].right = NULL_TVALUE;
                this->next_empty[current_node] = this->firstEmpty;
                this->firstEmpty = current_node;
            }
                //if the node has a left child
            else if (this->entities[current_node].right == NULL_TVALUE) {
                if (current_node != previous_node) {
                    if (this->relation(this->entities[previous_node].information.key, c))
                        this->entities[previous_node].right = this->entities[current_node].left;
                    else
                        this->entities[previous_node].left = this->entities[current_node].left;
                }
                else {
                    this->root = this->entities[current_node].left;
                }
                this->entities[current_node].left = NULL_TVALUE;
                this->entities[current_node].right = NULL_TVALUE;
                this->next_empty[current_node] = this->firstEmpty;
                this->firstEmpty = current_node;
            }
                //if the node has 2 children
            else {
                //I have to find the maximum on the left subtree
                int curr_pos = this->entities[current_node].left;
                int parent_max = curr_pos;
                while (this->entities[curr_pos].right != NULL_TVALUE) {
                    parent_max = curr_pos;
                    curr_pos = this->entities[curr_pos].right;
                }
                int maximum = curr_pos;
                elem.elem_capacity = this->entities[maximum].information.elem_capacity;
                elem.elem_size = this->entities[maximum].information.elem_size;
                elem.key = this->entities[maximum].information.key;
                elem.values = this->entities[maximum].information.values;


                this->entities[maximum].information.elem_capacity = 5;
                this->entities[maximum].information.elem_size = 0;
                delete[] this->entities[maximum].information.values;
                this->entities[maximum].information.values = new int[5];
                this->entities[maximum].information.key = NULL_TVALUE;

                if (curr_pos != parent_max)
                    this->entities[parent_max].right = NULL_TVALUE;
                else
                    this->entities[current_node].left = NULL_TVALUE;
                this->next_empty[maximum] = this->firstEmpty;
                this->firstEmpty = maximum;
            }
            this->actual_size--;
            this->size_of_entities--;
            return true;
        }
        else if (this->relation(this->entities[current_node].information.key, c)) {
            previous_node = current_node;
            current_node = this->entities[current_node].right;
        }
        else {
            previous_node = current_node;
            current_node = this->entities[current_node].left;
        }
    }
    return false;
}


///Best case: Theta(1)
///worst case: O(length)
///average: O(length)
int SortedMultiMap::size() const {
    //TODO - Implementation
    return this->actual_size;
}
///Thetha(1)
bool SortedMultiMap::isEmpty() const {
    //TODO - Implementation
    if(this->actual_size == 0)
        return true;
    return false;
}
///thetha(1)
SMMIterator SortedMultiMap::iterator() const {
	return SMMIterator(*this);
}
///Theta(1)
SortedMultiMap::~SortedMultiMap() {
    //TODO - Implementation
    for (int i = 0; i < this->capacity; i++) {
        if(this->entities[i].information.key != NULL_TVALUE)
            delete[] this->entities[i].information.values;
    }
    delete[] this->entities;
}

/// O(2*capacity)
void SortedMultiMap::resize()
{
    this->capacity *= 2;

    int* aux_next = new int[this->capacity];
    for (int i = 0; i < this->size_of_entities; i++) {
        if (this->next_empty[i] == NULL_TVALUE)
            aux_next[i] = this->size_of_entities;
        else
            aux_next[i] = this->next_empty[i];
    }
    for (int i = this->size_of_entities; i < this->capacity - 1; i++)
        aux_next[i] = i + 1;
    aux_next[this->capacity - 1] = NULL_TVALUE;
    delete[] this->next_empty;
    this->next_empty = aux_next;
    this->firstEmpty = this->size_of_entities;


    Node* aux_entities = new Node[this->capacity];
    for (int i = 0; i < this->size_of_entities; i++) {
        aux_entities[i].information.key = this->entities[i].information.key;
        aux_entities[i].information.values = this->entities[i].information.values;
        aux_entities[i].information.elem_capacity = this->entities[i].information.elem_capacity;
        aux_entities[i].information.elem_size = this->entities[i].information.elem_size;
        aux_entities[i].left = this->entities[i].left;
        aux_entities[i].right = this->entities[i].right;
    }
    for (int i = this->size_of_entities; i < this->capacity; i++) {
        aux_entities[i].information.elem_capacity = 5;
        aux_entities[i].information.values = new TValue[5];
        aux_entities[i].information.elem_size = 0;
    }

    /*for (int i = 0; i < this->capacity; i++)
        cout << aux_entities[i].information.elem_size << " ";*/

    //delete[] this->entities;
    this->entities = aux_entities;
}

void SortedMultiMap::print()
{
    for (int i = 0; i < this->capacity; i++)
        cout << i << "  " << this->entities[i].information.key << "  " << this->entities[i].left << "  " << this->entities[i].right << endl;
    cout << endl << "First empty: "<<this->firstEmpty << endl << endl;
    //cout << endl << "Next First empty: "<<this->next_empty[firstEmpty] << endl << endl;
    cout << endl << "Root: "<<this->root<< endl << endl;
}
///Best case: Theta(1)
///worst case: O(length)
///average: O(length)
void SortedMultiMap::replace(TKey k,TValue oldValue,TValue newValue){
    int current_node = this->root;
    while (current_node != NULL_TVALUE) {
        if (this->entities[current_node].information.key == k) {
            Element &elem = this->entities[current_node].information;
            for (int i = 0; i < elem.elem_size; i++)
                if (elem.values[i] == oldValue) {
                    elem.values[i] = newValue;
                    break;
                }
            break;
        } else if (this->relation(this->entities[current_node].information.key, k))
            current_node = this->entities[current_node].right;
        else
            current_node = this->entities[current_node].left;
    }

}


///O(capacity*2)
void SortedMultiMap::replacev2(TKey k,TValue oldValue,TValue newValue){

    if(remove(k, oldValue))
        add(k,newValue);

}


/////O(capacity)
//SortedMultiMap::SortedMultiMap(const SortedMultiMap &smm) {
//    this->r=smm.r;
//    this->capcity = smm.capcity;
//    this->length = smm.length;
//    this->head = smm.head;
//    this->firstFree = smm.firstFree;
//    delete[] this->elems;
//    this->elems= new KeyNode[this->capcity];
//    for (int i=0; i<this->capcity-1;i++)
//        this->elems[i]=smm.elems[i];
//
//}

/////O(capacity)
//SortedMultiMap &SortedMultiMap::operator=(const SortedMultiMap &smm) {
//    if(this==&smm)
//        return *this;
//    this->r=smm.r;
//    this->capcity = smm.capcity;
//    this->length = smm.length;
//    this->head = smm.head;
//    this->firstFree = smm.firstFree;
//    delete[] this->elems;
//    this->elems= new KeyNode[this->capcity];
//    for (int i=0; i<this->capcity-1;i++)
//        this->elems[i]=smm.elems[i];
//
//    return *this;
//}






