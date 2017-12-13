#include "IHeap.hpp"
#include <algorithm>
#include <vector>
class ILeftHeapNode {
public:
  int key_;
  ILeftHeapNode *left_;
  ILeftHeapNode *right_;

public:
  virtual ~ILeftHeapNode() {
    if (left_ != nullptr)
      delete left_;
    if (right_ != nullptr)
      delete right_;
  }
  virtual ILeftHeapNode *_correct_merge_(ILeftHeapNode *) = 0;
  static ILeftHeapNode *merge_(ILeftHeapNode *one_, ILeftHeapNode *second_) {
    if (one_ == nullptr)
      return second_;
    if (second_ == nullptr)
      return one_;
    if (second_->key_ < one_->key_)
      std::swap(one_, second_);
    one_->right_ = merge_(one_->right_, second_);
    one_ = second_->_correct_merge_(one_);
    return one_;
  }
};
class CLeftHeapNode : public ILeftHeapNode {
  size_t distance_;

public:
  CLeftHeapNode(int key_ = 0, int distance_ = 0, CLeftHeapNode *left_ = nullptr,
                CLeftHeapNode *right_ = nullptr)
      : distance_(distance_) {
    this->key_ = key_;
    this->left_ = left_;
    this->right_ = right_;
    recalc_(this);
  }
  static size_t dist_(CLeftHeapNode *a) {
    return (a == nullptr) ? 0 : a->distance_;
  }
  static void recalc_(CLeftHeapNode *a) {
    if (a == nullptr)
      return;
    a->distance_ = std::max(dist_(dynamic_cast<CLeftHeapNode *>(a->left_)),
                            dist_(dynamic_cast<CLeftHeapNode *>(a->right_))) +
                   1;
  }
  virtual ILeftHeapNode *_correct_merge_(ILeftHeapNode *res) {
    if (dist_(dynamic_cast<CLeftHeapNode *>(res->left_)) <
        dist_(dynamic_cast<CLeftHeapNode *>(res->right_)))
      std::swap(res->left_, res->right_);
    recalc_(dynamic_cast<CLeftHeapNode *>(res));
    return res;
  }
};
class CSkewHeapNode : public ILeftHeapNode {
public:
  CSkewHeapNode(int key_ = 0, CSkewHeapNode *left_ = nullptr,
                CSkewHeapNode *right_ = nullptr) {
    this->key_ = key_;
    this->left_ = left_;
    this->right_ = right_;
  }
  virtual ILeftHeapNode *_correct_merge_(ILeftHeapNode *res) {
    std::swap(res->left_, res->right_);
    return res;
  }
};
template <typename HeapNode> class LeftHeap : public IHeap {
private:
  typedef LeftHeap<HeapNode> _Self;
  HeapNode *root;

public:
  void Insert(int key) {
    HeapNode *newnode = new HeapNode(key);
    root = dynamic_cast<HeapNode *>(ILeftHeapNode::merge_(newnode, root));
  }
  LeftHeap(int key) {
    root = nullptr;
    Insert(key);
  }
  int GetMin() const {
    if (root == nullptr)
      return 0;
    return root->key_;
  }
  int ExtractMin() {
    if (root == nullptr)
      return 0;
    int res = root->key_;
    HeapNode *newroot = dynamic_cast<HeapNode *>(
        ILeftHeapNode::merge_(root->left_, root->right_));
    root->left_ = root->right_ = nullptr;
    delete root;
    root = newroot;
    return res;
  }
  void Meld(IHeap &mmheap_) {
    _Self &mheap_ = dynamic_cast<_Self &>(mmheap_);
    root = dynamic_cast<HeapNode *>(ILeftHeapNode::merge_(root, mheap_.root));
  }
  virtual ~LeftHeap() { delete root; }
};
