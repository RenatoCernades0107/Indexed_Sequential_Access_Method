//
// Created by renat on 9/7/2023.
//

#include "include/DataPage.h"


/*
 * N: Number of records in ISAM Data Pages
 *              |-----------------keys-----------------|   |------------------children---------------|   |-----next-----|
 * BufferSize = sizeof(KeyType*) + (sizeof(KeyType) * N) + sizeof(KeyType*) + sizeof(POS_TYPE) * (N+1) + sizeof(POS_TYPE)
 * BufferSize = sizeof(KeyType*) + (sizeof(KeyType) * N) + sizeof(KeyType*) + (sizeof(POS_TYPE) * N) + sizeof(POS_TYPE) + sizeof(POS_TYPE)
 * BufferSize = N * (sizeof(KeyType) + sizeof(POS_TYPE)) + (2 * sizeof(POS_TYPE)) + (2 * sizeof(KeyType*))
 * N = (BufferSize - 2 * sizeof(POS_TYPE) - 2 * sizeof(KeyType*)) / (sizeof(KeyType) + sizeof(POS_TYPE))
 *
 */

template<typename KeyType>
int64_t DataPage<KeyType>::getCapacity() const {
    return std::floor((buffer::BUFFER_SIZE - 2 * sizeof(POS_TYPE) - 2 * sizeof(KeyType *)) /
                      (sizeof(KeyType) + sizeof(POS_TYPE)));
}

template<typename KeyType>
DataPage<KeyType>::~DataPage() {
    delete keys;
    delete records;
}