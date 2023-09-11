//
// Created by renat on 9/7/2023.
//

#ifndef INDEXED_SEQUENTIAL_ACCESS_METHOD_INDEXPAGE_H
#define INDEXED_SEQUENTIAL_ACCESS_METHOD_INDEXPAGE_H

#include <sstream>
#include <fstream>
#include "src/DataPage.cpp"

template<typename KeyType>
class IndexPage {
private:
    KeyType *keys = nullptr;
    POS_TYPE *children = nullptr;
    bool isLeaf = false;
public:
    IndexPage();

    void setKey(KeyType key, int64_t pos);

    void setChild(POS_TYPE child, int64_t pos);

    void setIsLeaf(bool isLeaf);

    void write(std::ofstream& file);

    void read(std::ifstream& file);

    int64_t size_of();

    KeyType *getKeys() const;

    POS_TYPE *getChildren() const;

    bool getIsLeaf() const;

    int64_t getCapacity() const;

    ~IndexPage();
};


#endif //INDEXED_SEQUENTIAL_ACCESS_METHOD_INDEXPAGE_H