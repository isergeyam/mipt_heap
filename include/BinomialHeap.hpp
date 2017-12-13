#include "IHeap.hpp"
#include <algorithm>
#include <list>
#include <vector>
class CBinomialTree {
private:
  int key;
  CBinomialTree *child;
  CBinomialTree *next;
  size_t degree;

public:
  typedef CBinomialTree _Self;
  CBinomialTree(int key = 0, _Self *child = nullptr, _Self *next = nullptr,
                size_t degree = 1)
      : key(key), child(child), next(next), degree(degree) {}
  friend class CBinomialHeap;
  static _Self *merge(_Self *left, _Self *right) {
    if (left == nullptr)
      return right;
    if (right == nullptr)
      return left;
    if (left->key > right->key)
      std::swap(left, right);
    right->next = left->child;
    _Self *res = new _Self(left->key, right, nullptr, left->degree + 1);
    left->next = left->child = nullptr;
    delete left;
    return res;
  }
  ~CBinomialTree() {
    delete child;
    delete next;
  }
};
class CBinomialHeap : public IHeap {
  typedef CBinomialHeap _Self;
  typedef CBinomialTree _Tree;
  typedef std::list<_Tree *> _List;

private:
  _List root;

public:
  void Insert(int x) {
    root.push_back(new _Tree(x));
    return;
  }
  CBinomialHeap(int x) { Insert(x); }
  void clear() {
    for (auto it = root.begin(); it != root.end(); ++it) {
      (*it)->next = nullptr;
      delete *it;
    }
  }
  ~CBinomialHeap() { clear(); }
  size_t size() const { return root.size(); }
  CBinomialHeap() {}
  void Meld(IHeap &mmright) {
    CBinomialHeap &mright = dynamic_cast<CBinomialHeap &>(mmright);
    if (mright.size() == 0)
      return;
    _List newroot;
    for (auto it1 = root.begin(), it2 = mright.root.begin();
         it1 != root.end() || it2 != mright.root.end();) {

      if (it2 == mright.root.end() ||
          (it1 != root.end() && (*it1)->degree < (*it2)->degree)) {
        newroot.push_back(*it1);
        ++it1;
      } else if (it1 == root.end() || (*it2)->degree < (*it1)->degree) {
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
      if ((*cur)->degree == (*ncur)->degree) {
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
  CBinomialHeap(_Self &&that) { root = std::move(that.root); }
  CBinomialHeap &operator=(CBinomialHeap that) {
    std::swap(root, that.root);
    return *this;
  }
  _List::const_iterator minimum() const {
    auto res = root.begin();
    for (auto it = root.begin(); it != root.end(); ++it) {
      if ((*it)->key < (*res)->key) {
        res = it;
      }
    }
    return res;
  }
  int ExtractMin() {
    if (size() == 0)
      return 0;
    auto min_it = minimum();
    int res = (*min_it)->key;
    _Self *newnode = new _Self;
    _Tree *start = (*min_it)->child;
    while (start != nullptr) {
      newnode->root.push_back(start);
      start = start->next;
      newnode->root.front()->next = nullptr;
    }
    std::reverse(newnode->root.begin(), newnode->root.end());
    (*min_it)->next = (*min_it)->child = nullptr;
    delete *min_it;
    root.erase(min_it);
    Meld(*newnode);
    return res;
  }
  int GetMin() const {
    if (size() == 0)
      return 0;
    return (*minimum())->key;
  }
};
