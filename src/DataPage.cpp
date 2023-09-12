//
// Created by renat on 9/7/2023.
//

#include "include/DataPage.h"

template<typename KeyType>
DataPage<KeyType>::DataPage() {
    int64_t capacity = this->getCapacity();
    keys = new KeyType[capacity];
    records = new POS_TYPE[capacity];
    next = -1;
}

template<typename KeyType>
POS_TYPE DataPage<KeyType>::write(std::fstream &file) {
    POS_TYPE pos = file.tellp();
    int64_t capacity = this->getCapacity();
    // Write isLeaf attribute
    file.write((char*)& next, sizeof(next));
    // Write keys
    for (int i = 0; i < capacity; ++i) {
        file.write((char*)& (keys[i]), sizeof(keys[i]));
    }
    // Write children positions
    for (int i = 0; i < capacity + 1; ++i) {
        file.write((char*)& (records[i]), sizeof(records[i]));
    }
    return pos;
}

template<typename KeyType>
void DataPage<KeyType>::read(std::fstream &file) {
    int64_t capacity = this->getCapacity();
    // Read isLeaf attribute
    file.read((char*)& next, sizeof(next));
    // Read keys
    for (int i = 0; i < capacity; ++i) {
        file.read((char*)& keys[i], sizeof(keys[i]));
    }
    // Read children positions
    for (int i = 0; i < capacity + 1; ++i) {
        file.read((char*)& records[i], sizeof(records[i]));
    }
}

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
    auto a = std::floor((buffer::BUFFER_SIZE - 2 * sizeof(POS_TYPE) - 2 * sizeof(KeyType *)) /
                      (sizeof(KeyType) + sizeof(POS_TYPE)));
    return 3;
}


template<typename KeyType>
KeyType *DataPage<KeyType>::getKeys() const {
    return keys;
}

template<typename KeyType>
void DataPage<KeyType>::setKey(POS_TYPE key, int64_t pos) {
    try {
        keys[pos] = key;
    } catch (std::exception &e) {
        throw (std::runtime_error(
                "Cannot read record: " + std::to_string(key) +
                " in position: " + std::to_string(pos)
        ));
    }
}

template<typename KeyType>
POS_TYPE *DataPage<KeyType>::getRecords() const {
    return records;
}

template<typename KeyType>
void DataPage<KeyType>::setRecord(POS_TYPE record, int64_t pos) {
    try {
        records[pos] = record;
    } catch (std::exception &e) {
        throw (std::runtime_error(
                "Cannot read record: " + std::to_string(record) +
                " in position: " + std::to_string(pos)
        ));
    }
}

template<typename KeyType>
POS_TYPE DataPage<KeyType>::getNext() const {
    return next;
}

template<typename KeyType>
void DataPage<KeyType>::setNext(POS_TYPE next) {
    DataPage::next = next;
}

template<typename KeyType>
DataPage<KeyType>::~DataPage() {
    delete [] keys;
    delete [] records;
}