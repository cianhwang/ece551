#ifndef _LL_H_
#define _LL_H_
#include <assert.h>

#include <cstdlib>
#include <exception>

//YOUR CODE GOES HERE
template<typename T>
class LinkedList
{
 private:
  class Node
  {
   public:
    T data;
    Node * next;
    Node * prev;
    Node() : next(NULL), prev(NULL) {}
    /*    Node(const Node & rhs) {}
    Node & operator=(const Node & rhs) {}
    ~Node() {}*/
  };
  Node * head;
  Node * tail;
  int size;

 public:
  LinkedList() : head(NULL), tail(NULL), size(0) {}
  LinkedList(const LinkedList & rhs) {
    size = rhs.size;
    for (int i = 0; i < size; ++i) {
      addBack(rhs[i]);  //??
    }
  }
  LinkedList & operator=(const LinkedList & rhs) {
    size = rhs.size;
    if (this != &rhs) {
      for (int i = 0; i < size; ++i) {
        addBack(rhs[i]);  //??
      }
    }
  }
  ~LinkedList() {
    while (head != NULL) {
      Node * temp = head;
      head = head->next;
      delete temp;
    }
    tail = NULL;
  }
  void addFront(const T & item) {
    if (head == NULL) {
      head = new Node();
      head->data = item;
      head->next = head->prev = NULL;
      tail = head;
    }
    else {
      Node * temp = head;
      head = new Node();
      head->data = item;
      head->next = temp;
      head->prev = NULL;
    }
    size += 1;
  }
  void addBack(const T & item) {
    if (head == NULL) {
      addFront(item);
    }
    else {
      Node * temp = tail;
      tail = new Node();
      tail->data = item;
      tail->prev = temp;
      tail->next = NULL;
    }
    size += 1;
  }
  bool remove(const T & item) {
    bool flag = false;
    Node * temp = head;
    while (temp != NULL && temp->data != item) {
      temp = temp->next;
    }
    if (temp != NULL) {
      flag = true;
      size -= 1;
      if (size == 0) {
        delete head;
        head = tail = NULL;
      }
      else {
        if (temp->prev != NULL) {
          temp->prev->next = temp->next;
        }
        if (temp->next != NULL) {
          temp->next->prev = temp->prev;
        }
        delete temp;
      }
    }
    return flag;
  }
  T & operator[](int index) {
    assert(index >= 0 && index < size);
    Node * temp = head;
    int i;
    for (i = 0; i < index; ++i) {
      temp = temp->next;
    }
    return temp->data;
  }
  const T & operator[](int index) const {
    assert(index >= 0 && index < size);
    Node * temp = head;
    int i;
    for (i = 0; i < index; ++i) {
      temp = temp->next;
    }
    return temp->data;
  }
  int find(const T & item) const {
    int idx = -1;
    for (int i = 0; i < size; ++i) {
      if ((*this)[i] == item) {
        idx = i;
        break;
      }
    }
    return idx;
  }
  int getSize() { return size; }
};

#endif
