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
    /*if (root->left_ == nullptr)
      newroot = dynamic_cast<HeapNode *>(root->right_);
    else if (root->right_ == nullptr)
      newroot = dynamic_cast<HeapNode *>(root->left_);
    else
      newroot =
          dynamic_cast<HeapNode *>(root->merge_(root->left_, root->right_));*/
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
/*template<class HeapNode>
template<typename _Heap>
class CLeftHeap : public IHeap {
        private:
                std::vector<_Heap*> Vec_ ;
        public:
                ~CLeftHeap() {
                        for (auto it=Vec_.begin();it!=Vec_.end();++it) {
                                if (*it!=nullptr)
                                        delete *it ;
                        }
                }
                virtual void AddHeap(int key ) {
                        Vec_.push_back(new _Heap(key)) ;
                        return ;
                }
                virtual void Insert(size_t index, int key) {
                        Vec_[index]=merge_(Vec_[index], new _Heap(key)) ;
                }
                virtual int GetMin(size_t index) const {
                        if (Vec_[index]==nullptr)
                                return 0 ;
                        return Vec_[index]->key_ ;
                }
                virtual int ExtractMin(size_t index) {
                        if (Vec_[index]==nullptr)
                                return 0 ;
                        _Heap *cur=Vec_[index] ;
                        _Heap *newcur = merge_(cur->left_, cur->right_) ;
                        cur->left_=cur->right_=nullptr ;
                        int res = cur->key_ ;
                        delete cur ;
                        Vec_[index]=newcur ;
                        return res ;
                }
                virtual void Meld(size_t index1, size_t index2) {
                        if (index1==index2)
                                return ;
                        Vec_[index1]=merge_(Vec_[index1], Vec_[index2]) ;
                        Vec_.erase(Vec_.begin()+index2) ;
                }
};*/
