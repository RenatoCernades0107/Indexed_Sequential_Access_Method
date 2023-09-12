//
// Created by renat on 9/7/2023.
//

#ifndef INDEXED_SEQUENTIAL_ACCESS_METHOD_DATAPAGE_H
#define INDEXED_SEQUENTIAL_ACCESS_METHOD_DATAPAGE_H

#include <cstdint>
#include <iostream>
#include "utils/buffer.h"
#include <cmath>

using POS_TYPE = int64_t;


template<typename KeyType>
class DataPage {
private:
    KeyType *keys = nullptr;
    POS_TYPE *records = nullptr;
    POS_TYPE next;
public:
    DataPage();

    KeyType *getKeys() const;

    void setKey(POS_TYPE key, int64_t pos);

    POS_TYPE *getRecords() const;

    void setRecord(POS_TYPE record, int64_t pos);

    POS_TYPE getNext() const;

    void setNext(POS_TYPE next);

    int64_t getCapacity() const;

    POS_TYPE write(std::fstream& file);

    void read(std::fstream& file);

    ~DataPage();
};


#endif //INDEXED_SEQUENTIAL_ACCESS_METHOD_DATAPAGE_H
