#pragma once 
#include <cstddef> 
class IHeap{
	public:
		virtual void AddHeap( int ) = 0;
		virtual void Insert( size_t, int) = 0;
		virtual int GetMin(size_t) const = 0;
		virtual int ExtractMin(size_t) = 0;
		virtual void Meld(size_t, size_t) = 0;
};
