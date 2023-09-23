//
// Created by renat on 9/7/2023.
//

#include "include/DataPage.h"

template<typename KeyType>
DataPage<KeyType>::DataPage() {
    int64_t capacity = this->getCapacity();
    keys = new KeyType[capacity];
    records_pos = new POS_TYPE[capacity];
    next = -1;
    count = 0;
}

template<typename KeyType>
POS_TYPE DataPage<KeyType>::write(std::fstream &file) {
    POS_TYPE pos = file.tellp();
    int64_t capacity = this->getCapacity();
    // Write count attribute
    file.write((char *) &count, sizeof(count));
    // Write isLeaf attribute
    file.write((char *) &next, sizeof(next));
    // Write keys
    for (int i = 0; i < capacity; ++i) {
        file.write((char *) &(keys[i]), sizeof(keys[i]));
    }
    // Write children positions
    for (int i = 0; i < capacity; ++i) {
        file.write((char *) &(records_pos[i]), sizeof(records_pos[i]));
    }
    return pos;
}

template<typename KeyType>
void DataPage<KeyType>::read(std::fstream &file) {
    int64_t capacity = this->getCapacity();
    // Read count attribute
    file.read((char *) &count, sizeof(count));
    // Read isLeaf attribute
    file.read((char *) &next, sizeof(next));
    // Read keys
    for (int i = 0; i < capacity; ++i) {
        file.read((char *) &keys[i], sizeof(keys[i]));
    }
    // Read children positions
    for (int i = 0; i < capacity; ++i) {
        file.read((char *) &records_pos[i], sizeof(records_pos[i]));
    }
}

template<typename KeyType>
int64_t DataPage<KeyType>::size_of() {
    int64_t size = 0;
    int64_t capacity = getCapacity();
    size += sizeof(count) + sizeof(next);
    size += sizeof(KeyType) * capacity;
    size += sizeof(POS_TYPE) * (capacity);
    return size;
}

/*
 * N: Number of records_pos in ISAM Data Pages
 *              |-----------------keys-----------------|   |------------------records---------------|   |-----next-----|   |-----count-----|
 * BufferSize = sizeof(KeyType*) + (sizeof(KeyType) * N) + sizeof(KeyType*) + sizeof(POS_TYPE) * (N) + sizeof(POS_TYPE) + sizeof(POS_TYPE)
 * BufferSize = sizeof(KeyType*) + (sizeof(KeyType) * N) + sizeof(KeyType*) + (sizeof(POS_TYPE) * N) + sizeof(POS_TYPE) + sizeof(POS_TYPE)
 * BufferSize = N * (sizeof(KeyType) + sizeof(POS_TYPE)) + (2 * sizeof(POS_TYPE)) + (2 * sizeof(KeyType*))
 * N = (BufferSize - 2 * sizeof(POS_TYPE) - 2 * sizeof(KeyType*)) / (sizeof(KeyType) + sizeof(POS_TYPE))
 *
 */

template<typename KeyType>
int64_t DataPage<KeyType>::getCapacity() {
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
bool DataPage<KeyType>::isFull() const {
    return count == getCapacity();
}


template<typename KeyType>
POS_TYPE DataPage<KeyType>::getCount() const {
    return count;
}

template<typename KeyType>
POS_TYPE *DataPage<KeyType>::getRecords() const {
    return records_pos;
}

template<typename KeyType>
void DataPage<KeyType>::setRecord(std::pair<KeyType, POS_TYPE> record, int64_t pos) {
    try {
        keys[pos] = record.first;
        records_pos[pos] = record.second;
        ++count;
    } catch (std::exception &e) {
        throw (std::runtime_error(
                "Cannot write record with key: " + std::to_string(record.first) +
                " in position: " + std::to_string(pos)
        ));
    }
}

template<typename KeyType>
std::pair<KeyType, POS_TYPE> DataPage<KeyType>::getRecord(int64_t pos) {
    return std::pair<KeyType, POS_TYPE>(keys[pos], records_pos[pos]);
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
void DataPage<KeyType>::remove(int64_t i) {
    if (i < 0 || i >= getCount())
        throw std::runtime_error("Cannot index a record in position: " + std::to_string(i));
    if (i == getCount()-1) {
        keys[i] = -1;
        records_pos[i] = -1;
    } else {
        uint64_t j = i+1;
        for (; j < getCount(); ++j) {
            setRecord(getRecord(j), j-1);
        }
        keys[j] = -1;
        records_pos[j] = -1;
    }
    --count;
}



template<typename KeyType>
DataPage<KeyType>::~DataPage() {
    delete[] keys;
    delete[] records_pos;
}