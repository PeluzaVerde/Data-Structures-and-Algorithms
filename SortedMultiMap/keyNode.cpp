//
// Created by HP on 4/30/2022.
//

#include "keyNode.h"


///Theta (1)
void KeyNode::addVal(TValue v) {
    if(this->length >= this->capacity)
        this->resize();

    if(this->head == -1)
    {
        this->values[0].val=v;
        this->values[0].next=-1;
        this->keyFree=1;
        this->head=0;
        this->length=1;
        this->tail = 0;
    }
    else
    {
        int newNode = this->keyFree;
        this->keyFree = this->values[this->keyFree].next;
        this->values[this->tail].next = newNode;
        this->values[newNode].val =v;
        this->values[newNode].next = -1;
        this->length++;
        this->tail = newNode;

    }
}
///O(2*capacity)
void KeyNode::resize() {
    auto *els = new ValueNode[this->capacity*2];

    for( int i=0; i<this->length; i++)
        els[i] = this->values[i];

    for( int j= this->capacity; j< this->capacity*2-1; j++)
        els[j].next= j + 1;

    this->keyFree = this->capacity;
    this->capacity*=2;
    els[this->capacity-1].next = -1;
    delete[] this->values;
    this->values= els;
}
///Best case: Theta(1)
///worst case: O(capacity)
///average: O(capacity)
void KeyNode::removeVal(TValue v) {
if (this->length ==0)
    throw exception();
else{
    int current = this->head;
    if(this->values[current].val== v)
    {
        this->head= this->values[current].next;
        this->values[current].next = this->keyFree;
        this->keyFree = current;
        this->length--;
    }
    else {
        while(this->values[current].next != -1 && this->values[this->values[current].next].val != v)
            current = this->values[current].next;
        if(this->values[this->values[current].next].val == v) {
            int newnext = this->values[current].next;
            this->values[current].next = this->values[this->values[current].next].next;
            this->values[newnext].next = this->keyFree;
            this->keyFree = newnext;
            this->length--;
        }
    }

}

}

///O(capacity)
KeyNode::KeyNode(const KeyNode &smm) {
    this->capacity = smm.capacity;
    this->length = smm.length;
    this->head = smm.head;
    this->keyFree = smm.keyFree;
    this->tail =smm.tail;
    delete[] this->values;
    this->values= new ValueNode[this->capacity*2];
    for (int i=0; i<this->capacity-1;i++)
        this->values[i]=smm.values[i];

}

///O(capacity)
KeyNode &KeyNode::operator=(const KeyNode &smm) {
    if(this==&smm)
        return *this;
    this->capacity = smm.capacity;
    this->length = smm.length;
    this->head = smm.head;
    this->keyFree = smm.keyFree;
    this->tail= smm.tail;
    delete[] this->values;
    this->values= new ValueNode[this->capacity*2];
    for (int i=0; i<this->capacity-1;i++)
        this->values[i]=smm.values[i];
    return *this;

}
///Theta(1)
KeyNode::~KeyNode() {
    delete[] this->values;

}

///O(capacity)
KeyNode::KeyNode(){
    this->head = -1;
    this->length =0;
    this->capacity = 10;
    this->keyFree =0;
    this->tail = 0;

    this->values = new ValueNode[this->capacity];
    for (int i=0; i<this->capacity-1;i++)
        this->values[i].next=i+1;
    this->values[this->capacity-1].next=-1;




}

