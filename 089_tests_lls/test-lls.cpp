#include <cassert>
#include <cstdlib>

#include "il.h"

void testList(void) {
  // test Constructor()
  IntList a;
  assert((a.head == NULL) && (a.tail == NULL) && (a.size == 0));
  // test copy constructor(1)
  IntList * b = new IntList(a);
  assert((b->head == NULL) && (b->tail == NULL) && (b->size == 0));
  // test destructor(1)
  delete b;  //???

  // test addBack
  a.addBack(10);
  assert((a.head == a.tail) && (a.head->data = 10) && (a.size == 1));  // a.head->next and prev
  a.addBack(11);
  assert((a.head->data = 10) && (a.tail->data = 11) &&
         (a.size == 2));  // a.head/tail->next and prev
  // test copy constructor(2)
  IntList * c = new IntList(a);
  assert((c->size == a.size) && (c->head != a.head) &&
         (c->head->next != a.head->next));  // elementwise comparison
  // test destructor(2)
  delete c;  // ???
  // test =
  IntList e;
  e = a;
  assert((a.head != e.head) && (a.size == e.size) && (a.head->data == e.head->data));
  IntList::Node * temp = a.head;
  a = a;
  assert(a.head == temp);

  // test remove(1) and getSize
  a.addBack(12);
  a.addBack(11);
  assert(!a.remove(13) && (a.getSize() == 4));
  assert(a.remove(12) && (a.getSize() == 3));
  assert(a.remove(11) && (a.getSize() == 2));
  assert(a.remove(10) && (a.head->data == 11) && (a.getSize() == 1));
  assert((a.head->prev == NULL));
  assert(a.remove(11) && a.getSize() == 0);
  assert((a.head == a.tail) && (a.head == NULL));

  // test addfront
  IntList * d = new IntList;
  d->addFront(10);
  assert((d->head->data == 10) && (d->head == d->tail) && (d->size == 1));
  d->addFront(11);
  assert((d->head->data == 11) && (d->tail->data == 10) && (d->getSize() == 2));
  d->addFront(11);
  assert((d->find(11) == 0) && (d->find(10) == 2));
  // test []
  int aa = 0;
  aa += (*d)[0];
  assert(aa == 11);
  delete d;
}

int main(void) {
  testList();
  return EXIT_SUCCESS;
}
