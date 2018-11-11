#include "node.h"

Node * buildTree(uint64_t * counts) {
  priority_queue_t pq;
  for (int i = 0; i < 257; ++i) {
    if (counts[i] != 0) {
      pq.push(new Node(i, counts[i]));
    }
  }
  while (pq.size() > 1) {
    Node * a = pq.top();
    pq.pop();
    Node * b = pq.top();
    pq.pop();
    Node * tmproot = new Node(a, b);
    pq.push(tmproot);
  }
  Node * root = pq.top();
  pq.pop();
  return root;
}
