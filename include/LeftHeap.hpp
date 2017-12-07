#include "IHeap.hpp"
#include <vector>
#include <algorithm>
class CLeftHeapNode{
	typedef CLeftHeapNode _Self ;
	private:
		int key_ ;
		size_t distance_ ;
		_Self *left_ ;
		_Self *right_ ;
	public:
		static size_t dist_(_Self *a) {
			return (a==nullptr) ? 0 : a->distance_ ;
		}
		static void recalc_(_Self *a) {
			if (a==nullptr)
				return ;
			a->distance_=std::max(dist_(a->left_), dist_(a->right_))+1 ;
		}
		CLeftHeapNode(int key_ = 0, int distance_ = 0, _Self *left_=nullptr, _Self *right_=nullptr) :
			key_(key_),
			distance_(distance_),
			left_(left_),
			right_(right_) 
	{
		recalc_(this) ;
	}
		~CLeftHeapNode() {
			if (left_!=nullptr)
				delete left_ ;
			if (right_!=nullptr)
				delete right_ ;
		}
		static _Self* merge_(_Self *one_, _Self *second_) {
			if (one_==nullptr)
				return second_ ;
			if (second_==nullptr) 
				return one_ ;
			if (second_->key_<one_->key_)
				std::swap(one_, second_) ;
			one_->right_=merge_(one_->right_, second_) ;
			if(dist_(one_->right_)>dist_(one_->left_))
				std::swap(one_->right_, one_->left_) ;
			recalc_(one_) ;
			return one_ ;
		}
		friend class CLeftHeap ;
};
class CLeftHeap : public IHeap {
	typedef CLeftHeapNode _Heap ;
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
			Vec_[index]=_Heap::merge_(Vec_[index], new _Heap(key)) ;
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
			_Heap *newcur = _Heap::merge_(cur->left_, cur->right_) ;
			cur->left_=cur->right_=nullptr ;
			int res = cur->key_ ;
			delete cur ;
			Vec_[index]=newcur ;
			return res ;
		}
		virtual void Meld(size_t index1, size_t index2) {
			if (index1==index2)
				return ;
			Vec_[index1]=_Heap::merge_(Vec_[index1], Vec_[index2]) ;
			Vec_.erase(Vec_.begin()+index2) ;
		}
};
