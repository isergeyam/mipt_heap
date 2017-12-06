#include "IHeap.hpp"
#include <list>
#include <vector>
#include <algorithm>
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
		friend class CBinomialHeapNode ;
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
	typedef CBinomialHeapNode _Self ;
	typedef CBinomialTree _Tree ;
	typedef std::list<_Tree> _List ;
	private:
		_List root ;
	public:
		CBinomialHeapNode(_Tree head) {
			root.push_back(head) ;
		}
		size_t size() {
			return root.size() ;
		}
		CBinomialHeapNode() {}
		static _Self* merge(_Self *mleft, _Self *mright) {
			if (mleft==nullptr || mleft->size() == 0) 
				return mright ;
			if (mright==nullptr || mright->size() == 0)
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
					if(ncur==res->root.begin())
						continue ;
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
		CBinomialHeapNode(_Self &&that) {
			root=std::move(that.root) ;
		}
		CBinomialHeapNode& operator=(CBinomialHeapNode that) {
			std::swap(root, that.root) ;
			return *this ;
		}
		void insert(int x) {
			_Self *ntr = new _Self(_Tree(x)) ;
			root=std::move(merge(this, ntr)->root) ;
			return ;
		}
		_List::iterator minimum() {
			_List::iterator res=root.begin() ;
			for (auto it=root.begin();it!=root.end();++it) {
				if (it->key<res->key) {
					res=it ;
				}
			}
			return res ;
		}
		int ExtractMin() {
			if(size()==0) 
				return 0 ;
			auto min_it=minimum() ;
			int res=min_it->key ;
			_Self *newnode = new _Self ;
			_Tree *start=min_it->child ;
			while(start!=nullptr) {
				newnode->root.push_back(*start) ;
				start=start->next ;
			}
			std::reverse(newnode->root.begin(), newnode->root.end()) ;
			root.erase(min_it) ;
			root=std::move(merge(this, newnode)->root) ;
			return res ; 
		}
};
class CBinomialHeap : public IHeap {
	typedef CBinomialHeapNode Heap ;
	private:
		std::vector<Heap*> vec ;
	public:
		CBinomialHeap() {}
		virtual void AddHeap( int x) {
			vec.push_back(new Heap(x)) ;	
		}
		virtual void Insert( size_t index, int x) {
			vec[index]->insert(x) ;
		}
		virtual int GetMin(size_t index) const {
			return vec[index]->minimum()->key ;
		}
		virtual int ExtractMin(size_t index)  {
			return vec[index]->ExtractMin() ;
		}
		virtual void Meld(size_t index1, size_t index2) {
			if (vec.size()<2 || index1==index2)
				return ;
			vec[index1]=Heap::merge(vec[index1], vec[index2]) ;
			vec.erase(vec.begin()+index2) ;
		}
};
