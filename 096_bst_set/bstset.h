#ifndef BSTSET_H
#define BSTSET_H

#include <iostream>

#include "set.h"

template<typename T>
class BstSet : public Set<T>
{
 private:
  class Node
  {
   public:
    T data;
    Node * lnode;
    Node * rnode;
    Node * parent;

   public:
    Node() : lnode(NULL), rnode(NULL), parent(NULL) {}
    Node(T dat) : data(dat), lnode(NULL), rnode(NULL), parent(NULL) {}
    Node(T dat, Node * par) : data(dat), lnode(NULL), rnode(NULL), parent(par) {}
    Node(T dat, Node * ln, Node * rn, Node * par) : data(dat), lnode(ln), rnode(rn), parent(par) {}
    ~Node() {}
  };
  Node * root;
  void des_helper(Node * root) {
    if (root == NULL) {
      return;
    }
    des_helper(root->lnode);
    des_helper(root->rnode);
    delete root;
  }
  Node * construct_helper(Node * rootRhs) {
    if (rootRhs == NULL) {
      return NULL;
    }
    Node * root = new Node(rootRhs->data, NULL);
    root->lnode = construct_helper(rootRhs->lnode);
    root->rnode = construct_helper(rootRhs->rnode);
    return root;
  }
  Node * remove_helper(Node * root, const T & key) {
    if (key < root->data) {
      root->lnode = remove_helper(root->lnode, key);
    }
    else if (key > root->data) {
      root->rnode = remove_helper(root->rnode, key);
    }
    else {
      if (root->lnode == NULL) {
        Node * temp = root;
        root = root->rnode;
        delete temp;
      }
      else if (root->rnode == NULL) {
        Node * temp = root;
        root = root->lnode;
        delete temp;
      }
      else {
        Node * curr = root->lnode;
        while (curr->rnode != NULL) {
          curr = curr->rnode;
        }
        root->data = curr->data;
        root->lnode = remove_helper(root->lnode, curr->data);
      }
    }
    return root;
  }
  void print_helper(const Node * root) const {
    if (root == NULL) {
      return;
    }
    std::cout << root->data << ' ';
    print_helper(root->lnode);
    print_helper(root->rnode);
  }
  void print_helper2(const Node * root) const {
    if (root == NULL) {
      return;
    }
    print_helper2(root->lnode);
    std::cout << root->data << ' ';
    print_helper2(root->rnode);
  }

 public:
  BstSet() : root(NULL) {}
  BstSet(const BstSet & rhs) { root = construct_helper(rhs.root); }
  BstSet & operator=(const BstSet & rhs) {
    if (this != &rhs) {
      Node * temp = construct_helper(rhs.root);
      des_helper(root);
      root = temp;
    }
    return *this;
  }
  ~BstSet() { des_helper(root); }  // destroy
  void Print() const {
    std::cout << "Pre: ";
    print_helper(root);
    std::cout << std::endl;
    std::cout << "In: ";
    print_helper2(root);
    std::cout << std::endl;
  }
  virtual void add(const T & key) {
    if (!this->contains(key)) {
      Node ** curr = &root;
      Node * parent = NULL;
      while ((*curr) != NULL) {
        parent = *curr;
        if (key < (*curr)->data) {
          curr = &((*curr)->lnode);
        }
        else {
          curr = &((*curr)->rnode);
        }
      }
      Node * newNode = new Node(key, parent);
      *curr = newNode;
    }
  }
  virtual bool contains(const T & key) const {
    Node * const * curr = &root;
    while (*curr != NULL) {
      if (key < (*curr)->data) {
        curr = &((*curr)->lnode);
      }
      else if (key > (*curr)->data) {
        curr = &((*curr)->rnode);
      }
      else {
        return true;
      }
    }
    return false;
  }
  virtual void remove(const T & key) {
    root = remove_helper(root, key);
    /*    Node ** curr = &root;
    while (*curr != NULL) {
      if (key < (*curr)->mapDat.first) {
        curr = &((*curr)->lnode);
      }
      else if (key > (*curr)->mapDat.first) {
        curr = &((*curr)->rnode);
      }
      else {
        if ((*curr)->lnode == NULL) {
          if ((*curr)->parent->lnode == *curr) {
            (*curr)->parent->lnode = (*curr)->rnode;
          }
          else {
            (*curr)->parent->rnode = (*curr)->rnode;
          }
          delete *curr;
        }
        else if ((*curr)->rnode == NULL) {
          if ((*curr)->parent->lnode == *curr) {
            (*curr)->parent->lnode = (*curr)->lnode;
          }
          else {
            (*curr)->parent->rnode = (*curr)->lnode;
          }
          delete *curr;
        }
        else {
          Node * temp = *curr;
          curr = &((*curr)->lnode);
          while ((*curr)->rnode != NULL) {
            curr = &((*curr)->rnode);
          }
          temp->mapDat = (*curr)->mapDat;
          if ((*curr)->parent != temp) {
            (*curr)->parent->rnode = (*curr)->lnode;
          }
          else {
            (*curr)->parent->lnode = (*curr)->lnode;
          }
          delete *curr;
        }
      }
      }*/
  }
};

#endif
