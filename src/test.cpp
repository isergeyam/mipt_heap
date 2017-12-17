#include "BinomialHeap.hpp"
#include "LeftHeap.hpp"
#include "StlHeap.hpp"
#include <gtest/gtest.h>
enum OpType { ADD, INSERT, GET, EXTRACT, MELD };
struct OperationData {
  int type;
  int element;
  size_t index1;
  size_t index2;
  OperationData(OpType type = ADD, int element = 0, size_t index1 = 0,
                size_t index2 = 0)
      : type(type), element(element), index1(index1), index2(index2) {}
};
void generate_test(size_t Size, std::vector<OperationData> &Vec_) {
  size_t cur_size = 0;
  for (size_t i = 0; i < Size; ++i) {
    int x = (cur_size == 0) ? 0 : rand() % 5;
    switch (x) {
    case ADD:
      Vec_.push_back(OperationData(ADD, std::rand(), 0, 0));
      ++cur_size;
      break;
    case INSERT:
      Vec_.push_back(
          OperationData(INSERT, std::rand(), std::rand() % cur_size, 0));
      break;
    case GET:
      Vec_.push_back(OperationData(GET, 0, std::rand() % cur_size, 0));
      break;
    case EXTRACT:
      Vec_.push_back(OperationData(EXTRACT, 0, std::rand() % cur_size, 0));
      break;
    case MELD:
      Vec_.push_back(OperationData(MELD, 0, std::rand() % cur_size,
                                   std::rand() % cur_size));
      --cur_size;
      break;
    }
  }
  return;
}
static std::vector<OperationData> ExamVec;
static std::vector<std::vector<OperationData>> TestVec;
struct HeapLimits {
  static size_t correct_test_max;
  static size_t test_max;
  static size_t test_min;
  static size_t test_walk;
  static void init(int argc, char **argv) {
    if (argc < 5)
      return;
    correct_test_max = atoi(argv[1]);
    test_min = atoi(argv[2]);
    test_max = atoi(argv[3]);
    test_walk = atoi(argv[4]);
    return;
  }
};
template <typename _CurHeap> class HeapTest : public ::testing::Test {
protected:
  HeapList MyHeap;
  HeapList StdHeap;

public:
  HeapTest() {}
  template <typename PrHeap>
  int process_operation(OperationData Op_, HeapList &Heap_) {
    int res;
    switch (Op_.type) {
    case ADD:
      Heap_.AddHeap(new PrHeap(Op_.element));
      res = 0;
      break;
    case INSERT:
      Heap_.Insert(Op_.index1, Op_.element);
      res = 0;
      break;
    case GET:
      res = Heap_.GetMin(Op_.index1);
      break;
    case EXTRACT:
      res = Heap_.ExtractMin(Op_.index1);
      break;
    case MELD:
      Heap_.Meld(Op_.index1, Op_.index2);
      res = 0;
      break;
    }
    return res;
  }
  template <typename PrHeap, typename PrEHeap>
  void process_test(HeapList *Heap, HeapList *Exam,
                    const vector<OperationData> &Vec_) {
    for (auto it = Vec_.begin(); it != Vec_.end(); ++it) {
      int res1 = process_operation<PrHeap>(*it, *Heap), res2;
      if (Heap == Exam)
        res2 = res1;
      else
        res2 = process_operation<PrEHeap>(*it, *Exam);
      EXPECT_EQ(res1, res2);
    }
  }
};
typedef ::testing::Types<CBinomialHeap, LeftHeap<CLeftHeapNode>,
                         LeftHeap<CSkewHeapNode>>
    MyTypes;
TYPED_TEST_CASE(HeapTest, MyTypes);
TYPED_TEST(HeapTest, TEST_MAIN_FUNCTIONALITY) {
  TestFixture::template process_test<TypeParam, CStlHeap>(
      &this->MyHeap, &this->StdHeap, ExamVec);
}
TYPED_TEST(HeapTest, TEST_TIME) {
  for (size_t i = HeapLimits::test_min, j = 0; i <= HeapLimits::test_max;
       i *= HeapLimits::test_walk) {
    std::vector<OperationData> &CurVec = TestVec[j];
    clock_t time_dump = clock();
    TestFixture::template process_test<TypeParam, TypeParam>(
        &this->MyHeap, &this->MyHeap, CurVec);
    std::cout << "" << i << " "
              << static_cast<double>(clock() - time_dump) / CLOCKS_PER_SEC
              << std::endl;
    ++j;
  }
}
size_t HeapLimits::correct_test_max = 10000;
size_t HeapLimits::test_max = static_cast<size_t>(5 * 10e4);
size_t HeapLimits::test_min = static_cast<size_t>(10e2);
size_t HeapLimits::test_walk = 2;
int main(int argc, char **argv) {
  srand(time(NULL));
  generate_test(HeapLimits::correct_test_max, ExamVec);
  for (size_t i = HeapLimits::test_min, j = 0; i <= HeapLimits::test_max;
       i *= HeapLimits::test_walk) {
    vector<OperationData> curvec;
    generate_test(i, curvec);
    TestVec.push_back(std::move(curvec));
    ++j;
  }
  HeapLimits::init(argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
