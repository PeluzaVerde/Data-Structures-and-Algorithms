#include "SMMIterator.h"
#include "SortedMultiMap.h"

using namespace std;
///theta (1)
SMMIterator::SMMIterator(const SortedMultiMap& d) : map(d){
    //TODO - Implementation
    int node = this->map.root;
    this->current_position = 0;
    while (node != NULL_TVALUE) {
        this->stack.push(node);
        node = this->map.entities[node].left;
    }
    if (this->map.isEmpty() == false)
        this->current_node = this->stack.top();
    else
        this->current_node = NULL_TVALUE;
}

///theta (1)
void SMMIterator::first(){
    //TODO - Implementation
    int node = this->map.root;
    this->current_position = 0;
    while (this->stack.empty() == false)
        this->stack.pop();
    while (node != NULL_TVALUE) {
        this->stack.push(node);
        node = this->map.entities[node].left;
    }
    if (this->map.isEmpty() == false)
        this->current_node = this->stack.top();
    else
        this->current_node = NULL_TVALUE;
}

///theta (1)
void SMMIterator::next(){
    //TODO - Implementation
    if (this->valid() == false)
        throw exception();
    if (this->map.entities[this->current_node].information.elem_size - 1 > this->current_position)
        this->current_position++;
    else {
        int node = this->stack.top();
        this->stack.pop();
        if (this->map.entities[node].right != NULL_TVALUE) {
            node = this->map.entities[node].right;
            while (node != NULL_TVALUE) {
                this->stack.push(node);
                node = this->map.entities[node].left;
            }
        }
        if (this->stack.empty() == false) {
            this->current_node = this->stack.top();
            this->current_position = 0;
        }
        else
            this->current_node = NULL_TVALUE;
    }
}
///theta (1)
bool SMMIterator::valid() const{
    //TODO - Implementation
    if(this->current_node != NULL_TVALUE && this->current_position <= this->map.entities[this->current_node].information.elem_size)
        return true;
    return false;
}
///theta (1)
TElem SMMIterator::getCurrent() const{
    //TODO - Implementation
    if (this->valid() == false)
        throw exception();
    return pair<TKey, TValue>{this->map.entities[current_node].information.key, this->map.entities[current_node].information.values[this->current_position]};
}

