#pragma once
#include <cstddef>
#include <vector>
class IHeap {
public:
  virtual void Insert(int) = 0;
  virtual int GetMin() const = 0;
  virtual int ExtractMin() = 0;
  virtual void Meld(IHeap &) = 0;
  virtual ~IHeap() = 0;
};
IHeap::~IHeap() {}
class HeapList {
private:
  std::vector<IHeap *> Vec_;

public:
  void AddHeap(IHeap *nkey) {
    Vec_.push_back(nkey);
    return;
  }
  void Insert(size_t index, int key) {
    Vec_[index]->Insert(key);
    return;
  }
  int GetMin(size_t index) const { return Vec_[index]->GetMin(); }
  int ExtractMin(size_t index) { return Vec_[index]->ExtractMin(); }
  void Meld(size_t index1, size_t index2) {
    if (index1 == index2)
      return;
    Vec_[index1]->Meld(*Vec_[index2]);
    Vec_.erase(Vec_.begin() + index2);
    return;
  }
  ~HeapList() {
    for (auto it = Vec_.begin(); it != Vec_.end(); ++it) {
      delete *it;
    }
  }
  size_t size() { return Vec_.size(); }
  bool empty() { return Vec_.empty(); }
};
