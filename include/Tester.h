//
// Created by renat on 9/12/2023.
//

#ifndef INDEXED_SEQUENTIAL_ACCESS_METHOD_TESTER_H
#define INDEXED_SEQUENTIAL_ACCESS_METHOD_TESTER_H


template <typename KeyType>
class Tester {
    ISAM<KeyType> isam;
public:
    void test_all();
    void test_write_IndexPage();
    void test_build_ISAM();
    void test_search_ISAM();
};




#endif //INDEXED_SEQUENTIAL_ACCESS_METHOD_TESTER_H
