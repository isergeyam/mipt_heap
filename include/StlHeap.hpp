#include "IHeap.hpp"
#include <vector>
#include <set>
using std::vector ;
using std::set ;
class CStlHeap : public IHeap {
	private:
		vector<set<int> > vec ;
	public:
		CStlHeap() {}
		virtual void AddHeap( int x ) {
			set<int> nset ;
			nset.insert(x) ;
			vec.push_back(nset) ;
		}
		virtual void Insert( size_t index, int x) {
			vec[index].insert(x) ;
		}
		virtual int GetMin(size_t index) const {
			if (vec[index].size()==0)
				return 0 ;
			return *vec[index].begin() ;
		}
		virtual int ExtractMin(size_t index)  {
			if (vec[index].size()==0)
				return 0 ;
			int res=GetMin(index) ;
			vec[index].erase(vec[index].begin()) ;
			return res ;
		}
		virtual void Meld(size_t index1, size_t index2) {
			if (vec.size()<2 || index1==index2)
				return ;
			vec[index1].insert(vec[index2].begin(), vec[index2].end()) ;
			vec.erase(vec.begin()+index2) ;
			return ;
		}
};
