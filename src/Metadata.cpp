//
// Created by renat on 9/10/2023.
//

#include "include/Metadata.h"

template <typename KeyType>
Metadata<KeyType>::Metadata() {
    std::ifstream file(METADATA_FILENAME);
    if (file) {
        std::cout << "Reading metadata" <<  std::endl;
        read();
    }
    else {
        std::cout << "Creating default metadata" <<  std::endl;
        // Setting the default metadata
        IndexPage<KeyType> ip;
        DataPage<KeyType> dp;
        // NOTE: Read the definition of IndexPage for more information
        index_page_capacity = ip.getCapacity();
        // NOTE: Read the definition of Datapage for more information
        data_page_capacity = dp.getCapacity();

        root_position = 0;

        write();
    }
}

template<typename KeyType>
void Metadata<KeyType>::write() {
    std::ofstream file(METADATA_FILENAME);
    file.write((char*) &index_page_capacity, sizeof(int));
    file.write((char*) &data_page_capacity, sizeof(int));
    file.write((char*) &root_position, sizeof(POS_TYPE));
}

template<typename KeyType>
void Metadata<KeyType>::read() {
    std::ifstream file(METADATA_FILENAME);
    file.read((char*) &index_page_capacity, sizeof(int));
    file.read((char*) &data_page_capacity, sizeof(int));
    file.read((char*) &root_position, sizeof(POS_TYPE));
}

template<typename KeyType>
void Metadata<KeyType>::setIndexPageCapacity(int indexPageCapacity) {
    index_page_capacity = indexPageCapacity;
}

template<typename KeyType>
void Metadata<KeyType>::setDataPageCapacity(int dataPageCapacity) {
    data_page_capacity = dataPageCapacity;
}

template<typename KeyType>
void Metadata<KeyType>::setRootPosition(POS_TYPE rootPosition) {
    root_position = rootPosition;
}

template<typename KeyType>
int Metadata<KeyType>::getIndexPageCapacity() const  {
    return index_page_capacity;
}

template<typename KeyType>
int Metadata<KeyType>::getDataPageCapacity() const {
    return data_page_capacity;
}

template<typename KeyType>
POS_TYPE Metadata<KeyType>::getRootPosition() const {
    return root_position;
}


