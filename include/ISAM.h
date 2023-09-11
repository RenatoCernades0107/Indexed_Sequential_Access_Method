//
// Created by renat on 9/7/2023.
//

#ifndef INDEXED_SEQUENTIAL_ACCESS_METHOD_ISAM_H
#define INDEXED_SEQUENTIAL_ACCESS_METHOD_ISAM_H

#include "src/Metadata.cpp"
#include "src/IndexPage.cpp"
#include <vector>

constexpr int64_t ISAM_LEVELS = 3;

/*
 * Heuristic ISAM
 *
 * About ISAM:
 *      This implementation has 3 levels and
 *      manage the collision with chaining.
 *
 * About the Heuristic:
 *      We get the max and min value of the dataset
 *      and create a ISAM in with this range of values [min, max].
 */
template<typename KeyType>
class ISAM {
private:
    Metadata<KeyType> metadata;
    POS_TYPE root; // Root of the ISAM

    std::string idx_filename; // Name of the index page file
    std::string dt_filename; // Name of the data page file
    std::string ds_filename; // Name of the dataset file

    std::fstream idx_file; // Index page file
    std::fstream dt_file; // Data page file
    std::fstream ds_file; // Dataset file

public:
    ISAM();

    bool open_files(std::ios::openmode op);
    void close_files();

    ~ISAM();

private:

    void build(const std::vector<std::pair<KeyType, POS_TYPE>>& data);
};




#endif //INDEXED_SEQUENTIAL_ACCESS_METHOD_ISAM_H
