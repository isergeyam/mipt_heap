#include "IHeap.hpp"
#include <set>
#include <vector>
using std::vector;
using std::set;
class CStlHeap : public IHeap {
private:
  set<int> mset;

public:
  CStlHeap(int key) { Insert(key); }
  virtual void Insert(int x) { mset.insert(x); }
  virtual int GetMin() const {
    if (mset.size() == 0)
      return 0;
    return *mset.begin();
  }
  virtual int ExtractMin() {
    if (mset.size() == 0)
      return 0;
    int res = GetMin();
    mset.erase(mset.begin());
    return res;
  }
  virtual void Meld(IHeap &mmheap_) {
    CStlHeap &mheap_ = dynamic_cast<CStlHeap &>(mmheap_);
    mset.insert(mheap_.mset.begin(), mheap_.mset.end());
    mheap_.mset.clear();
    return;
  }
};
