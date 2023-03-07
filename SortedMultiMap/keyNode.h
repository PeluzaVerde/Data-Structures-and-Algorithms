//
// Created by HP on 4/30/2022.
//

#pragma once
//DO NOT INCLUDE SMMITERATOR

//DO NOT CHANGE THIS PART
#include <vector>
#include <utility>
typedef int TKey;
typedef int TValue;
typedef std::pair<TKey, TValue> TElem;
#define NULL_TVALUE -111111
#define NULL_TELEM pair<TKey, TValue>(-111111, -111111);
using namespace std;
class SMMIterator;
typedef bool(*Relation)(TKey, TKey);


class ValueNode{
public:
    TValue val;
    int next;
};

class KeyNode {
private:
    void resize();
public:
    TKey key;
    int next;
    ValueNode *values;
    int head;
    int length;
    int capacity;
    int keyFree;
    int tail;
    void addVal(TValue v);
    void removeVal(TValue v);


    KeyNode(const KeyNode &smm);

    KeyNode & operator=(const KeyNode &smm);

    ~KeyNode();

    // constructor
    KeyNode();
};

