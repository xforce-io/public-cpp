#include "gtest/gtest.h"
#include "../../../src/close_hashmap.hpp"
#include "../../../src//time/time.h"

using namespace xforce;

namespace xforce {
LOGGER_IMPL(xforce_logger, "xforce")
}

int main(int argc, char** argv) {
  srand(time(NULL));
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

class TestCloseHashmap: public ::testing::Test {
 private: 
  typedef CloseHashmap<int, int> Container;

 protected:
  virtual ~TestCloseHashmap() {};

  virtual void SetUp() {}
  virtual void TearDown() {}
};

TEST_F(TestCloseHashmap, case0) {
  Container close_hashmap;
  ASSERT_TRUE(close_hashmap.Init(100));

  ASSERT_TRUE(close_hashmap.Insert(3,2));
  ASSERT_TRUE(close_hashmap.Insert(1,6));
  ASSERT_TRUE(!close_hashmap.Insert(1,1));
  ASSERT_TRUE(close_hashmap.Insert(2,7));
  ASSERT_TRUE(close_hashmap.Insert(7,9));
  ASSERT_TRUE(!close_hashmap.Erase(10));
  ASSERT_TRUE(close_hashmap.Erase(1));

  // (3,2) (2,7) (7,9)
  ASSERT_EQ(3, close_hashmap.Size());
  ASSERT_EQ(100, close_hashmap.Capacity());
  ASSERT_EQ(7, *(close_hashmap.Get(2)));
  ASSERT_EQ(2, *(close_hashmap.Get(3)));
  ASSERT_EQ(9, *(close_hashmap.Get(7)));
  ASSERT_TRUE(NULL == close_hashmap.Get(70));

  size_t sum=0;
  Container::Iterator iter;
  for (iter = close_hashmap.Begin(); iter != close_hashmap.End(); iter.Next()) {
    sum += iter->first;
  }
  ASSERT_EQ(12, sum);
}

TEST_F(TestCloseHashmap, case1) {
  Container close_hashmap;
  ASSERT_TRUE(close_hashmap.Init(1));
  ASSERT_TRUE(close_hashmap.Insert(3,2));
  ASSERT_TRUE(close_hashmap.Insert(1,6));
  ASSERT_TRUE(close_hashmap.Capacity() == 2);
  ASSERT_TRUE(*(close_hashmap.Get(3)) == 2);
  ASSERT_TRUE(*(close_hashmap.Get(1)) == 6);
}

TEST_F(TestCloseHashmap, upsert) {
  Container close_hashmap;
  ASSERT_TRUE(close_hashmap.Init(1));
  ASSERT_TRUE(close_hashmap.Upsert(3,2));
  ASSERT_TRUE(close_hashmap.Upsert(1,6));
  ASSERT_TRUE(close_hashmap.Upsert(3,4));
  ASSERT_TRUE(close_hashmap.Capacity() == 2);
  ASSERT_TRUE(*(close_hashmap.Get(3)) == 4);
  ASSERT_TRUE(*(close_hashmap.Get(1)) == 6);
}

TEST_F(TestCloseHashmap, erase) {
  const size_t kNumItems = 5;
  Container close_hashmap;
  ASSERT_TRUE(close_hashmap.Init(kNumItems, false));
  for (size_t i=0; i<kNumItems; ++i) {
    ASSERT_TRUE(close_hashmap.Insert(i, i));
  }
  for (size_t i=0; i<kNumItems; ++i) {
    ASSERT_TRUE(close_hashmap.Erase(i));
  }
  for (size_t i=0; i<kNumItems; ++i) {
    ASSERT_TRUE(close_hashmap.Insert(i+kNumItems, i+kNumItems));
  }
}

TEST_F(TestCloseHashmap, pressure) {
  typedef std::tr1::unordered_set<size_t> CmpContainer;
  typedef CloseHashmap<size_t, size_t> TargetContainer;

  static const size_t kTimesTest=1;
  static const size_t kRange=100000;
  static const size_t kCapacity=10000;
  static const size_t kNumElements = 3*kCapacity;

  for (size_t i=0; i<kTimesTest; ++i) {
    srand(i);

    std::pair<bool, size_t>* array_test = new std::pair<bool, size_t> [kNumElements];
    for (size_t i=0; i<kNumElements; ++i) {
      array_test[i].first = rand() % 2;
      array_test[i].second = rand() % kRange;
    }

    CmpContainer cmp_container;
    TargetContainer target_container;
    ASSERT_TRUE(target_container.Init(kCapacity));

    Timer t;
    t.Start(true);
    for (size_t i=0; i < kNumElements; ++i) {
      if (array_test[i].first) {
        cmp_container.insert(array_test[i].second);
      } else {
        cmp_container.erase(array_test[i].second);
      }
    }
    t.Stop(true);
    std::cout << "unordered_set_cost[" << t.TimeUs() << "]" << std::endl;

    t.Start(true);
    for (size_t i=0; i < kNumElements; ++i) {
      if (array_test[i].first) {
        target_container.Insert(array_test[i].second, array_test[i].second);
      } else {
        target_container.Erase(array_test[i].second);
      }
    }
    t.Stop(true);
    std::cout << "close_hashmap_cost[" << t.TimeUs() << "]" << std::endl;

    if (cmp_container.size() != target_container.Size()) {
      for (size_t i=0; i<kNumElements; ++i) {
        std::cout << "[" << array_test[i].first << "|" << array_test[i].second << "]" << std::endl;
      }
    }
    ASSERT_EQ(cmp_container.size(), target_container.Size());

    delete [] array_test;
  }
}
