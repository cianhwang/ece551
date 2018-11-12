#include "node.h"
void Node::buildMap(BitString b, std::map<unsigned, BitString> & theMap) {
  if (sym != NO_SYM) {
    assert(left == NULL);
    assert(right == NULL);
    theMap.insert(std::pair<unsigned, BitString>(sym, b));
    return;
  }
  assert(left != NULL);
  assert(right != NULL);
  this->left->buildMap(b.plusZero(), theMap);
  this->right->buildMap(b.plusOne(), theMap);
  return;
}
