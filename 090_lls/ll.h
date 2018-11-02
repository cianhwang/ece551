#ifndef _LL_H_
#define _LL_H_
#include <assert.h>

#include <cstdlib>
#include <exception>
#include <iostream>

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
    Node(T dat) : data(dat), next(NULL), prev(NULL) {}
    Node(T dat, Node * nex) : data(dat), next(nex), prev(NULL) {}
    Node(T dat, Node * nex, Node * pre) : data(dat), next(nex), prev(pre) {}

    /*    Node(const Node & rhs) {}
    Node & operator=(const Node & rhs) {}
    ~Node() {}*/
  };
  Node * head;
  Node * tail;
  int size;

  Node * remove(const T & item, Node * current, bool & flag) {
    if (current == NULL) {
      return NULL;
    }
    if (item == current->data) {
      Node * ans = current->next;
      delete current;
      flag = true;
      return ans;
    }
    current->next = remove(item, current->next, flag);
    return current;
  }

 public:
  LinkedList() : head(NULL), tail(NULL), size(0) {}  //ok
  LinkedList(const LinkedList & rhs) : head(NULL), tail(NULL), size(rhs.size) {
    if (size > 0) {
      head = new Node(rhs.head->data);
      Node * curr = head;
      Node * rhsCurr = rhs.head;
      while (rhsCurr->next != NULL) {
        curr->next = new Node(rhsCurr->next->data);
        curr->next->prev = curr;
        curr = curr->next;
        rhsCurr = rhsCurr->next;
      }
      tail = curr;
    }
  }
  LinkedList & operator=(const LinkedList & rhs) {
    if (this != &rhs) {
      while (head != NULL) {
        Node * temp = head->next;
        delete head;
        head = temp;
      }
      tail = NULL;
      size = rhs.size;
      /*for (int i = 0; i < size; ++i) {
        addBack(rhs[i]);  //??
	}*/
      if (size > 0) {
        head = new Node(rhs.head->data);
        Node * curr = head;
        Node * rhsCurr = rhs.head;
        while (rhsCurr->next != NULL) {
          curr->next = new Node(rhsCurr->next->data);
          curr->next->prev = curr;
          curr = curr->next;
          rhsCurr = rhsCurr->next;
        }
        tail = curr;
      }
    }
    return *this;
  }
  ~LinkedList() {  // ok
    while (head != NULL) {
      Node * temp = head->next;
      delete head;
      head = temp;
    }
  }
  void addFront(const T & item) {  //ok
    head = new Node(item, head);
    if (tail == NULL) {
      tail = head;
    }
    else {
      head->next->prev = head;
    }
    ++size;
  }
  void addBack(const T & item) {  //ok
    tail = new Node(item, NULL, tail);
    if (head == NULL) {
      head = tail;
    }
    else {
      tail->prev->next = tail;
    }
    ++size;
  }
  bool remove(const T & item) {  //ok
    Node * temp = head;
    if (temp == NULL) {
      return false;
    }
    if (temp->data == item) {
      head = head->next;
      delete temp;
      if (head != NULL) {
        head->prev = NULL;
      }
      else {
        tail = NULL;
      }
      --size;
      return true;
    }
    while (temp->next != NULL && temp->next->data != item) {
      temp = temp->next;
    }
    if (temp->next == NULL) {
      return false;
    }
    else {
      Node * Next = temp->next;
      temp->next = temp->next->next;
      if (temp->next != NULL) {
        temp->next->prev = temp;
      }
      else {
        tail = temp;
      }
      delete Next;
      --size;
      return true;
    }
  }
  T & operator[](int index) {
    Node * temp = head;
    try {
      for (int i = 0; i < index; ++i) {
        temp = temp->next;
      }
    }
    catch (const std::exception & e) {
      std::cout << e.what() << std::endl;
      throw;
    }
    return temp->data;
  }
  const T & operator[](int index) const {
    Node * temp = head;
    try {
      for (int i = 0; i < index; ++i) {
        temp = temp->next;
      }
    }
    catch (const std::exception & e) {
      std::cout << e.what() << std::endl;
      throw;
    }
    return temp->data;
  }
  int find(const T & item) const {  //ok
    int idx = -1;
    Node * temp = head;
    while (temp != NULL) {
      ++idx;
      if (temp->data == item) {
        return idx;
      }
      temp = temp->next;
    }
    return -1;
  }
  int getSize() const { return size; }  //ok
};

#endif
