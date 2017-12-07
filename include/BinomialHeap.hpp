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
			if (left->key>right->key) 
				std::swap(left, right) ;
			right->next=left->child ;
			_Self* res = new _Self(left->key, right, nullptr, left->degree+1) ;
			left->next=left->child=nullptr ;
			delete left ;
			return res ;
		}
		~CBinomialTree() {
			if (child!=nullptr)
				delete child ;
			if (next!=nullptr)
				delete next ;
		}
};
class CBinomialHeapNode {
	typedef CBinomialHeapNode _Self ;
	typedef CBinomialTree _Tree ;
	typedef std::list<_Tree*> _List ;
	private:
		_List root ;
	public:
		CBinomialHeapNode(int x) {
			root.push_back(new _Tree(x)) ;
		}
		void clear() {
			for (auto it=root.begin();it!=root.end();++it) {
				(*it)->next=nullptr ;
				delete *it ;
			}
		}
		size_t size() const {
			return root.size() ;
		}
		CBinomialHeapNode() {}
		void merge(_Self &mright) {
			if (mright.size() == 0)
				return ;
			_List newroot ;
			for (auto it1=root.begin(), it2=mright.root.begin();it1!=root.end() || it2!=mright.root.end();) {

				if (it2==mright.root.end() || (it1!=root.end() && (*it1)->degree<(*it2)->degree)) {
					newroot.push_back(*it1) ;
					++it1 ;
					root.pop_front() ;
				}
				else if (it1==root.end() || (*it2)->degree < (*it1)->degree) {
					newroot.push_back(*it2) ;
					++it2 ;
				}
				else {
					newroot.push_back(_Tree::merge(*it1, *it2)) ;
					++it1 ;
					++it2 ;
					if(!root.empty()) 
						root.pop_front() ;
				}
				_List::iterator ncur=std::prev(newroot.end()) ;
				if(ncur==newroot.begin())
					continue ;
				auto cur=std::prev(ncur) ;
				if ((*cur)->degree==(*ncur)->degree) {
					newroot.push_back(_Tree::merge(*cur, *ncur)) ;
					newroot.erase(cur) ;
					newroot.erase(ncur) ;
				}
			}
			root.splice(root.end(), newroot) ;
			return ;
		}
		CBinomialHeapNode(_Self &&that) {
			root=std::move(that.root) ;
		}
		CBinomialHeapNode& operator=(CBinomialHeapNode that) {
			std::swap(root, that.root) ;
			return *this ;
		}
		void insert(int x) {
			_Self ntr(x) ;
			merge(ntr) ;
			return ;
		}
		_List::const_iterator minimum() const {
			auto res=root.begin() ;
			for (auto it=root.begin();it!=root.end();++it) {
				if ((*it)->key<(*res)->key) {
					res=it ;
				}
			}
			return res ;
		}
		int ExtractMin() {
			if(size()==0) 
				return 0 ;
			auto min_it=minimum() ;
			int res=(*min_it)->key ;
			_Self newnode ;
			_Tree *start=(*min_it)->child ;
			while(start!=nullptr) {
				newnode.root.push_back(start) ;
				start=start->next ;
				newnode.root.front()->next=nullptr ;
			}
			std::reverse(newnode.root.begin(), newnode.root.end()) ;
			(*min_it)->next=(*min_it)->child=nullptr ;
			delete *min_it ;
			root.erase(min_it) ;
			merge(newnode) ;
			return res ; 
		}
		friend class CBinomialHeap ;
};
class CBinomialHeap : public IHeap {
	typedef CBinomialHeapNode Heap ;
	private:
	std::vector<Heap> vec ;
	public:
	CBinomialHeap() {}
	~CBinomialHeap() {
		for (auto it=vec.begin();it!=vec.end();++it) {
			it->clear() ;
		}
	}
	virtual void AddHeap( int x) {
		vec.push_back(Heap(x)) ;	
	}
	virtual void Insert( size_t index, int x) {
		vec[index].insert(x) ;
	}
	virtual int GetMin(size_t index) const {
		if (vec[index].size()==0)
			return 0 ;
		return (*vec[index].minimum())->key ;
	}
	virtual int ExtractMin(size_t index)  {
		if (vec[index].size()==0)
			return 0 ;
		return vec[index].ExtractMin() ;
	}
	virtual void Meld(size_t index1, size_t index2) {
		if (vec.size()<2 || index1==index2)
			return ;
		vec[index1].merge(vec[index2]) ;
		vec.erase(vec.begin()+index2) ;
	}
};
