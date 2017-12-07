#include <gtest/gtest.h>
#include "BinomialHeap.hpp"
#include "StlHeap.hpp"
#include "LeftHeap.hpp"
namespace {
	struct HeapLimits{
		static int correct_test_max ;
		static int test_max ;
	};
	class HeapTest : public ::testing::Test {
		public:
			CBinomialHeap BinHeap ;
			CStlHeap StdHeap ;
			CLeftHeap LHeap ;
			HeapTest() {}
			enum OpType {ADD, INSERT, GET, EXTRACT, MELD};
			struct OperationData{
				int type ;
				int element ;
				size_t index1 ;
				size_t index2 ;
				OperationData(OpType type = ADD, int element = 0, size_t index1 = 0, size_t index2 = 0) :
					type(type),
					element(element),
					index1(index1), 
					index2(index2) {}
			};
			static void generate_test(size_t Size, std::vector<OperationData> &Vec_) {
				size_t cur_size = 0 ;
				for (size_t i=0;i<Size;++i) {
					int x = (cur_size==0) ? 0 : rand() % 5 ;
					switch (x) {
						case ADD:
							Vec_.push_back(OperationData(ADD, std::rand(), 0, 0)) ;
							++cur_size ;
							break ;
						case INSERT:
							Vec_.push_back(OperationData(INSERT, std::rand(), std::rand()%cur_size, 0)) ; break ;
						case GET:
							Vec_.push_back(OperationData(GET, 0, std::rand()%cur_size, 0)) ;
							break ;
						case EXTRACT:
							Vec_.push_back(OperationData(EXTRACT, 0, std::rand()%cur_size, 0)) ;
							break ;
						case MELD:
							Vec_.push_back(OperationData(MELD, 0, std::rand()%cur_size, std::rand()%cur_size)) ;
							--cur_size ;
							break ;
					}
				}
				return ;
			}
			int process_operation(OperationData Op_, IHeap &Heap_) {
				int res ;
				switch(Op_.type) {
					case ADD:
						Heap_.AddHeap(Op_.element) ;
						res = 0;
						break ;
					case INSERT:
						Heap_.Insert(Op_.index1, Op_.element) ;
						res = 0;
						break ;
					case GET:
						res = Heap_.GetMin(Op_.index1) ;
						break ;
					case EXTRACT:
						res = Heap_.ExtractMin(Op_.index1) ;
						break ;
					case MELD:
						Heap_.Meld(Op_.index1, Op_.index2) ;
						res = 0 ;
						break ;
				}
				return res ;
			}
			void process_test(IHeap *Heap, IHeap *Exam, const vector<OperationData> &Vec_) {
				for (auto it=Vec_.begin();it!=Vec_.end();++it) {
					int res1 = process_operation(*it, *Exam), res2 ;
					if (Heap == Exam) 
						res2=res1 ;
					else 
						res2=process_operation(*it, *Heap) ;
					EXPECT_EQ(res1, res2) ;
				}
			}
			void correct_test(IHeap &Heap, IHeap &Exam) {
				std::vector<OperationData> Vec_ ;
				generate_test(HeapLimits::correct_test_max, Vec_) ;
				process_test(&Heap, &Exam, Vec_) ;
				return ;
			}
	};
	TEST_F(HeapTest, TEST_MAIN_FUNCTIONALITY_BIN_HEAP) {
		correct_test(BinHeap, StdHeap) ;
	}
	TEST_F(HeapTest, TEST_MAIN_FUNCTIONALITY_LEFT_HEAP) {
		correct_test(LHeap, StdHeap) ;
	}
}; //namespace
int HeapLimits::correct_test_max=10000 ;
int main (int argc, char **argv) {
	srand(time(NULL)) ;
	::testing::InitGoogleTest(&argc, argv) ;
	return RUN_ALL_TESTS() ;
}
