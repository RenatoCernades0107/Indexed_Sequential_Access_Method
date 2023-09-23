//
// Created by renat on 9/7/2023.
//

#ifndef INDEXED_SEQUENTIAL_ACCESS_METHOD_INDEXPAGE_H
#define INDEXED_SEQUENTIAL_ACCESS_METHOD_INDEXPAGE_H

#include "DataPage.h"
#include <fstream>
#include <sstream>

template <typename KeyType> class IndexPage {
private:
  KeyType *keys = nullptr;
  POS_TYPE *children = nullptr;
  bool isLeaf = false;

public:
  IndexPage();

  void setKey(KeyType key, int64_t pos);

  void setChild(POS_TYPE child, int64_t pos);

  void setIsLeaf(bool isLeaf);

  POS_TYPE write(std::fstream &file);

  void read(std::fstream &file);

  int64_t size_of();

  KeyType *getKeys() const;

  POS_TYPE *getChildren() const;

  bool getIsLeaf() const;

  int64_t getCapacity() const;

  ~IndexPage();
};

extern template class IndexPage<int>;

#endif // INDEXED_SEQUENTIAL_ACCESS_METHOD_INDEXPAGE_H
