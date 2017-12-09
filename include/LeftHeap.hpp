#include "IHeap.hpp"
#include <vector>
#include <algorithm>
template<class HeapNode>
struct ILeftHeapNode{
	int key_ ;
	HeapNode *left_ ;
	HeapNode *right_ ;
	~ILeftHeapNode() {
		if (left_!=nullptr)
			delete left_ ;
		if (right_!=nullptr)
			delete right_ ;
	}
	template<typename _Heap>
		friend class CLeftHeap ;
};
struct CLeftHeapNode : public ILeftHeapNode<CLeftHeapNode> {
	size_t distance_ ;
	CLeftHeapNode(int key_ = 0, int distance_ = 0, CLeftHeapNode *left_=nullptr, CLeftHeapNode *right_=nullptr) :
		distance_(distance_) 
	{
		this->key_=key_ ;
		this->left_=left_ ;
		this->right_=right_ ;
		recalc_(this) ;
	}
	static size_t dist_(CLeftHeapNode *a) {
		return (a==nullptr) ? 0 : a->distance_ ;
	}
	static void recalc_(CLeftHeapNode *a) {
		if (a==nullptr)
			return ;
		a->distance_=std::max(dist_(a->left_), dist_(a->right_))+1 ;
	}
};
struct CSkewHeapNode : public ILeftHeapNode<CSkewHeapNode> {
	CSkewHeapNode(int key_ = 0,  CSkewHeapNode *left_=nullptr, CSkewHeapNode *right_=nullptr)
	{
		this->key_=key_ ;
		this->left_=left_ ;
		this->right_=right_ ;
	}
};
CLeftHeapNode* _correct_merge_(CLeftHeapNode *res) {
	if(CLeftHeapNode::dist_(res->left_)<CLeftHeapNode::dist_(res->right_))
		std::swap(res->left_, res->right_) ;
	CLeftHeapNode::recalc_(res) ;
	return res ;
}
CSkewHeapNode* _correct_merge_(CSkewHeapNode *res) {
	std::swap(res->left_ , res->right_) ;
	return res ;
}
template<class HeapNode> 
HeapNode* merge_(HeapNode *one_, HeapNode *second_) {
	if (one_==nullptr)
		return second_ ;
	if (second_==nullptr) 
		return one_ ;
	if (second_->key_<one_->key_)
		std::swap(one_, second_) ;
	one_->right_=merge_(one_->right_, second_) ;
	_correct_merge_(one_) ;
	return one_ ;
}
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
};
