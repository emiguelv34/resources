#include "llist.h"
 
template<typename T>
LList<T>::Item::Item(const T& v, Item* p, Item* n)
: val(v), prev(p), next(n)
{
}

template<typename T>
LList<T>::LList() {
  mHead = new Item(T(), nullptr, nullptr);
  mTail = mHead;
  mSize = 0;
}

template<typename T>
LList<T>::LList(const LList& other) {
  mHead = new Item(int(), nullptr, nullptr);
  mTail = mHead;
  mSize = 0;
  Item* temp = other.mHead;
  while(temp != other.mTail){
    push_back(temp->val);
    temp = temp->next;
  }
}

template<typename T>
LList<T>& LList<T>::operator=(const LList& other) {
  if(this == &other){
    return *this;
  }
  clear();
  Item* temp = other.mHead;
  while(temp != other.mTail){
    push_back(temp->val);
    temp = temp->next;
  }
  return *this;
}

template<typename T>
LList<T>::~LList() {
  clear();
  delete mHead;
}

template<typename T>
int LList<T>::size() const {
  return mSize;
}

template<typename T>
bool LList<T>::empty() const {
  return mSize == 0;
}

template<typename T>
void LList<T>::push_back(const T& val) {
  Item* n = new Item (val, mTail->prev, mTail);
  if(mHead == mTail){
    mHead = n;
  }
  else
  {
    mTail->prev->next = n;
  }
  mTail->prev = n;
  ++mSize;
}

template<typename T>
T& LList<T>::get(int loc) {
  Item *temp = getNodeAt(loc);
  return temp->val;
}

template<typename T>
void LList<T>::clear() {
  while(mHead != mTail) {
    Item *temp = mHead->next;
    delete mHead;
    mHead = temp;
  }
  mSize = 0;
}

template<typename T>
typename LList<T>::Item* LList<T>::getNodeAt(int loc) const {
  if(loc >= 0 && loc < mSize){
    Item *temp = mHead;
    while(loc > 0){
      temp = temp->next;
      loc--;
    }
    return temp;
  }
  else {
    return nullptr;
  }
}