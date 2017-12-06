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
		~CBinomialTree() {
			if (child!=nullptr)
				delete child ;
			if (next!=nullptr)
				delete next ;
		}
		friend class CBinomialHeapNode ;
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
	typedef CBinomialHeapNode _Self ;
	typedef CBinomialTree _Tree ;
	typedef std::list<_Tree> _List ;
	private:
		_List root ;
	public:
		CBinomialHeapNode(_Tree head) {
			root.push_back(head) ;
		}
		CBinomialHeapNode() {}
		static _Self* merge(_Self *mleft, _Self *mright) {
			if (mleft==nullptr) 
				return mright ;
			if (mright==nullptr)
				return mleft ;
			_Self *res = new _Self ;
			for (auto it1=mleft->root.begin(), it2=mright->root.begin(), cur=res->root.begin();it1!=mleft->root.end() || it2!=mright->root.end();++cur) {
					if (it2==mright->root.end() || it1->degree<it2->degree) {
						res->root.push_back(*it1) ;
						++it1 ;
					}
					else if (it1==mleft->root.end() || it2->degree < it1->degree) {
						res->root.push_back(*it2) ;
						++it2 ;
					}
					else {
						res->root.push_back(*_Tree::merge(&(*it1), &(*it2))) ;
						++it1 ;
						++it2 ;
					}
					_List::iterator ncur=std::next(cur) ;
					if (cur->degree==ncur->degree) {
						res->root.push_back(*_Tree::merge(&(*cur), &(*ncur))) ;
						res->root.erase(cur) ;
						res->root.erase(ncur) ;
					}
			}
			/*if (left==nullptr) 
				res->root.insert(res->root.begin(), mright->root.begin(), mright->root.end()) ;
			else if (right==nullptr) 
				res->root.insert(res->root.begin(), mleft->root.begin(), mleft->root.end()) ;
			else {
				_List::iterator cur=res->root.begin(), pcur ;
				while (left!=nullptr || right!=nullptr) {
					if (right==nullptr || left->degree<right->degree) {
						cur->next=left ;
						left=left->next ;
					}
					else if (left==nullptr || right->degree<left->degree) {
						cur->next=right ;
						right=right->next ;
					}
					else { 
						_Tree *nleft=left->next, *nright=right->next ;
						cur->next=_Tree::merge(left, right) ;
						left=nleft ;
						right=nright ;
					}
					if (cur->degree == cur->next->degree) {
						cur = _Tree::merge(cur, cur->next) ;
						pcur->next=cur ;
					}
					else {
						pcur=cur ;
						cur=cur->next ;
					}
				}
			}*/
			return res ;
		}
		void insert(int x) {
			_Self *ntr = new _Self(_Tree(x)) ;
			merge(this, ntr) ;
			return ;
		}
		_List::iterator minimum() {
			int curmin = std::numeric_limits<int>::max() ;
			_List::iterator res ;
			for (auto it=root.begin();it!=root.end();++it) {
				if (it->key<curmin) {
					curmin=it->key ;
					res=it ;
				}
			}
			return res ;
		}
};
class CBinomialHeap : public IHeap {

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
