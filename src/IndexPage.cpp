//
// Created by renat on 9/7/2023.
//

#include "include/IndexPage.h"

template<typename KeyType>
IndexPage<KeyType>::IndexPage() {
    int64_t capacity = getCapacity();
    keys = new KeyType[capacity];
    children = new POS_TYPE[capacity + 1];
}

template<typename KeyType>
void IndexPage<KeyType>::setKey(KeyType key, int64_t pos) {
    try {
        keys[pos] = key;
    } catch (std::exception &e) {
        throw (std::runtime_error(
                "Cannot read key: " + std::to_string(key) +
                " in position: " + std::to_string(pos)
        ));
    }
}

template<typename KeyType>
void IndexPage<KeyType>::setChild(POS_TYPE child, int64_t pos) {
    try {
        children[pos] = child;
    } catch (std::exception &e) {
        throw (std::runtime_error(
                "Cannot read child: " + std::to_string(child) +
                " in position: " + std::to_string(pos)
        ));
    }
}

template<typename KeyType>
void IndexPage<KeyType>::setIsLeaf(bool isLeaf) {
    this->isLeaf = isLeaf;
}

template<typename KeyType>
void IndexPage<KeyType>::write(std::ofstream& file) {
    std::stringstream buffer;
    int64_t capacity =  this->getCapacity();
    // Write keys
    for (int i = 0; i < capacity; ++i) {
        buffer.write((char*)& (keys[i]), sizeof(KeyType));
    }
    // Write children positions
    for (int i = 0; i < capacity + 1; ++i) {
        buffer.write((char*)& (children[i]), sizeof(POS_TYPE));
    }
    // Write isLeaf attribute
    buffer.write((char*)& isLeaf, sizeof(bool));

    file.write(buffer.str().c_str(), this->size_of());
}

// TODO ARREGLAR ESTO
template<typename KeyType>
void IndexPage<KeyType>::read(std::ifstream& file) {
    char* buffer = new char[this->size_of()];
    file.read(buffer, this->size_of());

    std::stringstream ss{std::string(buffer)};
    delete [] buffer;

    int64_t capacity = this->getCapacity();
    // Read keys
    for (int i = 0; i < capacity; ++i) {
       ss.read((char*)& keys[i], sizeof(KeyType));
    }
    // Read children positions
    for (int i = 0; i < capacity + 1; ++i) {
        ss.read((char*)& children[i], sizeof(POS_TYPE));
    }
    // Read isLeaf attribute
    ss.read((char*)& isLeaf, sizeof(bool));
}


template<typename KeyType>
int64_t IndexPage<KeyType>::size_of() {
    int64_t capacity = this->getCapacity();
    return sizeof(KeyType)*capacity + sizeof(POS_TYPE)*(capacity+1) + sizeof(bool);
}


template<typename KeyType>
KeyType *IndexPage<KeyType>::getKeys() const {
    return keys;
}

template<typename KeyType>
POS_TYPE *IndexPage<KeyType>::getChildren() const {
    return children;
}

template<typename KeyType>
bool IndexPage<KeyType>::getIsLeaf() const {
    return isLeaf;
}

/*
 * M: Number of keys of ISAM Indexes Pages
 *              |------------------keys-----------------|  |------------------children---------------|  |---- isLeaf ---|
 * BufferSize = sizeof(KeyType*) + (sizeof(KeyType) * M) + sizeof(KeyType*) + sizeof(POS_TYPE) * (M+1) + sizeof(bool)
 * BufferSize = sizeof(KeyType*) + (sizeof(KeyType) * M) + sizeof(KeyType*) + (sizeof(POS_TYPE) * M) + sizeof(POS_TYPE) + sizeof(bool)
 * BufferSize = M * (sizeof(KeyType) + sizeof(POS_TYPE)) + sizeof(POS_TYPE) + (2 * sizeof(KeyType*)) + sizeof(bool)
 * M = (BufferSize - sizeof(POS_TYPE) - (2 * sizeof(KeyType*)) - sizeof(bool)) / (sizeof(KeyType) + sizeof(POS_TYPE))
 *
 */

template<typename KeyType>
int64_t IndexPage<KeyType>::getCapacity() const {
    return std::floor((buffer::BUFFER_SIZE - sizeof(POS_TYPE) - 2 * sizeof(KeyType *) - sizeof(bool)) /
                      (sizeof(KeyType) + sizeof(POS_TYPE)));
}

template<typename KeyType>
IndexPage<KeyType>::~IndexPage() {
    delete keys;
    delete children;
}