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
    DataPage() = default;

    int64_t getCapacity() const;

    ~DataPage();
};


#endif //INDEXED_SEQUENTIAL_ACCESS_METHOD_DATAPAGE_H
