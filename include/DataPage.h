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
    POS_TYPE *records_pos = nullptr;
    POS_TYPE next;
    POS_TYPE count;
public:
    DataPage();

    KeyType *getKeys() const;

    void setKey(POS_TYPE key, int64_t pos);

    bool isFull() const;

    POS_TYPE getCount() const;

    POS_TYPE *getRecords() const;

    void setRecord(std::pair<KeyType, POS_TYPE> record, int64_t pos);

    std::pair<KeyType, POS_TYPE> getRecord(int64_t pos);

    POS_TYPE getNext() const;

    void setNext(POS_TYPE next);

    static int64_t getCapacity();


    POS_TYPE write(std::fstream& file);

    void read(std::fstream& file);

    static int64_t size_of();

    void remove(int64_t i);

    ~DataPage();
};

#endif //INDEXED_SEQUENTIAL_ACCESS_METHOD_DATAPAGE_H
