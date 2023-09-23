//
// Created by renat on 9/12/2023.
//

#include "../include/Tester.h"
#include <iostream>
using namespace std;

template <typename KeyType> void Tester<KeyType>::test_all() {
  this->test_write_IndexPage();
  this->test_build_ISAM();
  this->test_search_ISAM();
  this->test_insert_ISAM();
  this->test_range_search_ISAM();
  this->test_remove_ISAM();
}

template <typename KeyType> void Tester<KeyType>::test_write_IndexPage() {
  IndexPage<KeyType> writer;

  // Fill attributes of indexPage
  for (int i = 0; i < writer.getCapacity(); ++i) {
    writer.setKey(i, i);
  }

  for (POS_TYPE i = 0; i < writer.getCapacity() + 1; ++i) {
    writer.setChild(i, i);
  }

  writer.setIsLeaf(true);

  // Try to write
  fstream file("test_IndexPage.dat", ios::binary | ios::out);
  writer.write(file);
  file.close();

  // Try to read
  fstream f("test_IndexPage.dat", ios::binary | ios::in);
  IndexPage<KeyType> reader;
  reader.read(f);

  // Testing
  KeyType *keys_writer = writer.getKeys();
  POS_TYPE *children_writer = writer.getChildren();

  KeyType *keys_reader = reader.getKeys();
  POS_TYPE *children_reader = reader.getChildren();

  bool compare_keys = true;
  for (int i = 0; i < writer.getCapacity(); ++i) {
    compare_keys = compare_keys && (keys_writer[i] == keys_reader[i]);
  }

  bool compare_children = true;
  for (int i = 0; i < writer.getCapacity() + 1; ++i) {
    compare_children =
        compare_children && (children_writer[i] == children_reader[i]);
  }

  bool compare_isLeaf = (writer.getIsLeaf() == reader.getIsLeaf());

  cout << "TEST test_write_IndexPage: " << endl;
  cout << "COMPARED KEYS: " << boolalpha << compare_keys << endl;
  cout << "COMPARED CHILDREN: " << boolalpha << compare_children << endl;
  cout << "COMPARED ISLEAF: " << boolalpha << compare_isLeaf << endl;

  f.close();
}

template <typename KeyType> void Tester<KeyType>::test_build_ISAM() {
  cout << "TEST test_build_ISAM: " << endl;

  int n = 81; // Num of records_pos
  std::vector<std::pair<KeyType, POS_TYPE>> test_data;
  for (int i = 0; i < n; ++i) {
    std::pair<KeyType, POS_TYPE> p = {i + 1, i};
    test_data.push_back(p);
  }
  isam.build(test_data);
  cout << "build test finished correctly" << endl;
}

template <typename KeyType> void Tester<KeyType>::test_search_ISAM() {
  cout << "TEST test_search_ISAM: " << endl;
  cout << boolalpha << (isam.search(80) == 79) << endl;
  cout << boolalpha << (isam.search(56) == 55) << endl;
  cout << boolalpha << (isam.search(10) == 9) << endl;
}

template <typename KeyType> void Tester<KeyType>::test_insert_ISAM() {
  cout << "TEST test_insert_ISAM: " << endl;
  cout << boolalpha << (isam.add(std::make_pair(10, 777)) != 777) << endl;
  cout << boolalpha << (isam.add(std::make_pair(90, 888)) == 888) << endl;
  cout << boolalpha << (isam.add(std::make_pair(55, 999)) != 999) << endl;
  cout << boolalpha << (isam.add(std::make_pair(100, 1111)) == 1111) << endl;
}

template <typename KeyType> void Tester<KeyType>::test_range_search_ISAM() {
  cout << "TEST test_range_serch_ISAM: " << endl;
  vector<POS_TYPE> result = isam.range_search(15, 19);
  int i = 14;
  for (auto r : result) {
    cout << (r == i++) << endl;
  }
}

template <typename KeyType> void Tester<KeyType>::test_remove_ISAM() {
  cout << "TEST test_remove_ISAM: " << endl;
  POS_TYPE position = isam.remove(68);
  cout << (position == 67) << " : " << position << endl;
}

template class Tester<int>;
