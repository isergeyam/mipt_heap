#include "IHeap.hpp"
#include <algorithm>
#include <list>
#include <vector>
class CBinomialTree {
private:
  int key_;
  CBinomialTree *child_;
  CBinomialTree *next_;
  size_t degree_;

public:
  typedef CBinomialTree _Self;
  explicit CBinomialTree(int key_ = 0, _Self *child_ = nullptr,
                         _Self *next_ = nullptr, size_t degree_ = 1)
      : key_(key_), child_(child_), next_(next_), degree_(degree_) {}
  friend class CBinomialHeap;
  static _Self *merge(_Self *left, _Self *right) {
    if (left == nullptr)
      return right;
    if (right == nullptr)
      return left;
    if (left->key_ > right->key_)
      std::swap(left, right);
    right->next_ = left->child_;
    _Self *res = new _Self(left->key_, right, nullptr, left->degree_ + 1);
    left->next_ = left->child_ = nullptr;
    delete left;
    return res;
  }
  ~CBinomialTree() {
    delete child_;
    delete next_;
  }
};
class CBinomialHeap : public IHeap {
private:
  typedef CBinomialHeap _Self;
  typedef CBinomialTree _Tree;
  typedef std::list<_Tree *> _List;

  _List root;

public:
  void Insert(int x) {
    root.push_back(new _Tree(x));
    return;
  }
  explicit CBinomialHeap(int x) { Insert(x); }
  void clear() {
    for (auto it = root.begin(); it != root.end(); ++it) {
      (*it)->next_ = nullptr;
      delete *it;
    }
  }
  ~CBinomialHeap() { clear(); }
  size_t size() const { return root.size(); }
  explicit CBinomialHeap() {}
  void Meld(IHeap &mmright) {
    CBinomialHeap &mright = dynamic_cast<CBinomialHeap &>(mmright);
    if (mright.size() == 0)
      return;
    _List newroot;
    for (auto it1 = root.begin(), it2 = mright.root.begin();
         it1 != root.end() || it2 != mright.root.end();) {

      if (it2 == mright.root.end() ||
          (it1 != root.end() && (*it1)->degree_ < (*it2)->degree_)) {
        newroot.push_back(*it1);
        ++it1;
      } else if (it1 == root.end() || (*it2)->degree_ < (*it1)->degree_) {
        newroot.push_back(*it2);
        ++it2;
      } else {
        newroot.push_back(_Tree::merge(*it1, *it2));
        ++it1;
        ++it2;
      }
      _List::iterator ncur = std::prev(newroot.end());
      if (ncur == newroot.begin())
        continue;
      auto cur = std::prev(ncur);
      if ((*cur)->degree_ == (*ncur)->degree_) {
        newroot.push_back(_Tree::merge(*cur, *ncur));
        newroot.erase(cur);
        newroot.erase(ncur);
      }
    }
    root.clear();
    mright.root.clear();
    root.splice(root.end(), newroot);
    return;
  }
  explicit CBinomialHeap(_Self &&that) { root = std::move(that.root); }
  CBinomialHeap &operator=(CBinomialHeap that) {
    std::swap(root, that.root);
    return *this;
  }
  _List::const_iterator minimum() const {
    auto res = root.begin();
    for (auto it = root.begin(); it != root.end(); ++it) {
      if ((*it)->key_ < (*res)->key_) {
        res = it;
      }
    }
    return res;
  }
  int ExtractMin() {
    if (size() == 0)
      return 0;
    auto min_it = minimum();
    int res = (*min_it)->key_;
    _Self *newnode = new _Self;
    _Tree *start = (*min_it)->child_;
    while (start != nullptr) {
      newnode->root.push_back(start);
      start = start->next_;
      newnode->root.front()->next_ = nullptr;
    }
    std::reverse(newnode->root.begin(), newnode->root.end());
    (*min_it)->next_ = (*min_it)->child_ = nullptr;
    delete *min_it;
    root.erase(min_it);
    Meld(*newnode);
    return res;
  }
  int GetMin() const {
    if (size() == 0)
      return 0;
    return (*minimum())->key_;
  }
};
