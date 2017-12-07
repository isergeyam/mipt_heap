#include "BinomialHeap.hpp"
#include <iostream>
using namespace std ;
int main () {
	CBinomialHeap heap ;
	heap.AddHeap(1) ;
	heap.AddHeap(2) ;
	heap.AddHeap(3) ;
	heap.Meld(0, 1) ;
	heap.Meld(0, 1) ;
	cout << heap.ExtractMin(0) << " " << heap.GetMin(0) << endl ;
}
