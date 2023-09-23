#ifndef INDEXED_SEQUENTIAL_ACCESS_METHOD_TESTER_H
#define INDEXED_SEQUENTIAL_ACCESS_METHOD_TESTER_H

#include "ISAM.h"

template <typename KeyType> class Tester {
  ISAM<KeyType> isam;

public:
  void test_all();
  void test_write_IndexPage();
  void test_build_ISAM();
  void test_search_ISAM();
  void test_insert_ISAM();
  void test_range_search_ISAM();
  void test_remove_ISAM();
};

extern template class Tester<int>;

#endif // INDEXED_SEQUENTIAL_ACCESS_METHOD_TESTER_H
