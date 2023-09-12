//
// Created by renat on 9/10/2023.
//

#ifndef INDEXED_SEQUENTIAL_ACCESS_METHOD_METADATA_H
#define INDEXED_SEQUENTIAL_ACCESS_METHOD_METADATA_H

#include <cstdint>
#include <fstream>
#include "utils/buffer.h"
#include "include/IndexPage.h"

using POS_TYPE = int64_t;
const char *METADATA_FILENAME = "ISAM.metadata";

/*
 * ISAM's Metadata
 * Global configuration
 */
template<typename KeyType>
class Metadata {
private:
    int index_page_capacity;
    int data_page_capacity;
    POS_TYPE root_position;
public:
    Metadata();

    void setIndexPageCapacity(int indexPageCapacity);

    void setDataPageCapacity(int dataPageCapacity);

    void setRootPosition(POS_TYPE rootPosition);

    int getIndexPageCapacity() const;

    int getDataPageCapacity() const;

    POS_TYPE getRootPosition() const;
private:
    void write();

    void read();
};




#endif //INDEXED_SEQUENTIAL_ACCESS_METHOD_METADATA_H
