#include "IHeap.hpp"
#include <list>
#include <vector>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>


template< typename T> 
using ordered_set = __gnu_pbds::tree<
  T,
  __gnu_pbds::null_type,
  std::less<T>,
  __gnu_pbds::rb_tree_tag,
  __gnu_pbds::tree_order_statistics_node_update> ;
class CBinomialTree{
	private:
		int key ;
		CBinomialTree *child ;
		CBinomialTree *next ;
		size_t degree ;
	public:
		typedef CBinomialTree _Self ;
		CBinomialTree(int key = 0, _Self *child=nullptr, _Self *next=nullptr, size_t degree = 1) :
			key(key),
			child(child),
			next(next),
			degree(degree) {}
		friend class CBinomialHeap ;
		static _Self* merge(_Self *left, _Self *right) {
			if (left==nullptr)
				return right ;
			if (right==nullptr) 
				return left ;
			_Self *res = new _Self ;
			if (left->key>right->key) 
				std::swap(left, right) ;
			res->key=left->key;
			right->next=left->child ;
			res->child=right ;
			res->degree=left->degree+1 ;
			return res ;
		}
};
class CBinomialHeapNode {
	private:
		CBinomialTree *head ;
	public:
		typedef CBinomialHeapNode _Self ;
		CBinomialHeapNode(CBinomialTree *head = nullptr) :
			head(head) {}
		static _Self* merge(_Self *mleft, _Self *mright) {
			CBinomialTree *left=mleft->head, *right=mright->head ;
			_Self *res = new _Self ;
			if (left==nullptr) 
				res->head=right ;
			else if (right==nullptr) 
				res->head=left ;
			else {
				CBinomialTree *&cur=res->head ;
				while (left!=nullptr && right!=nullptr) {
					
				}
			}
			return res ;
		}
};
/*class BinomialHeapNode {
	typedef BinomialHeapNode _Self ;
	private:
		int key ;
		_Self *parent ;
		_Self *child ;
		_Self *sibling ;
		size_t degree ;
	public:
		friend class BinomialHeap ;
		static _Self* merge(_Self *first, _Self *second) {
			if (first==NULL) 
				return second ;
			if (second==NULL)
				return first ;
			_Self *res ;
			if (first->key>second->key)
				std::swap(first, second) ;
			res->key=first->key ;
			first->parent=res ;
			res->child=:
		}
};
class BinomialHeap : public IHeap {
	typedef BinomialHeap _Self ;
	private:
		std::list<BinomialHeapNode*> heaps ;
	public:
		static _Self* Merge(_Self *first, _Self *second) {
			if (first==NULL) 
				return second ;
			if (second==NULL)
				return first ;
		}
};*/
