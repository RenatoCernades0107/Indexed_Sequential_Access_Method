//
// Created by renat on 9/7/2023.
//

#include "include/ISAM.h"

template<typename KeyType>
ISAM<KeyType>::ISAM() {
    std::cout << "Calling constructor" << std::endl;

    // Create the index and data page file if not exist
    if (open_files(std::ios::in)) {
        std::cout << "Files didn't exists" << std::endl;
        if (!open_files(std::ios::out)) {
            throw std::runtime_error("Files couldn't open");
        };
    }

    // Close files
    close_files();
};

template<typename KeyType>
bool ISAM<KeyType>::open_files(std::_Ios_Openmode op) {
    idx_file.open(idx_filename, op);
    dt_file.open(dt_filename, op);
    return idx_file.good() && dt_file.good();
}

template<typename KeyType>
void ISAM<KeyType>::close_files() {
    idx_file.close();
    dt_file.close();
}

template<typename KeyType>
void ISAM<KeyType>::build(const std::vector<std::pair<KeyType, POS_TYPE>> &data) {
    if (data.empty())
        throw std::runtime_error("Building the ISAM failed, data is empty!");

    // Get the min and max value of the dataset
    KeyType min;
    KeyType max;

    for (const auto &pair: data) {
        if (pair.first >= max) {
            max = pair.first;
        }
        if (pair.first <= min) {
            min = pair.first;
        }
    }

    /*
     * Calculate the step with the number of ISAM's keys
     *
     * M: Capacity of keys in a IndexPage
     * n: Total of ISAM's keys
     * d: Levels of ISAM
     *
     *          N = Σ M*(M+1)^i
     *       From i=0 to d-1
     *
     */

    int64_t M = metadata.getIndexPageCapacity();
    int64_t n = M + (M+1)*M + pow(M+1, 2);

     /*
     * inf, sup: min and max key in data.
     *
     *         step = (sup - inf) / N
     *
     */

    KeyType step = std::floor((max - min) / n);


    /*
     * Let f(i) the number of keys in the left child of a node (IndexPage) with height i
     *
     *             f(i) = {0 : i == 0, M : i == 1, f(i-1) + M*(M+1)^i-1 : i >= 2}
     *
     * Where 0 <= i < d
     * */

    POS_TYPE f[ISAM_LEVELS] = {0, 1};
    for (int i = 2; i < ISAM_LEVELS; ++i) {
        f[i] = f[i-1] + M * pow(M+1, i-1);
    }

    auto num_of_nodes = [M](const POS_TYPE& i){
        return pow(M+1, ISAM_LEVELS - i - 1);
    };

    auto nextLevel = [&f](const POS_TYPE& i) -> const POS_TYPE {
        return f[i];
    };

    auto nextKey = [&f](const POS_TYPE& x, const POS_TYPE& i) -> const POS_TYPE {
        return x + f[i] + 1;
    };

    auto nextBrother = [&f](const POS_TYPE& x, const POS_TYPE& i) -> const POS_TYPE {
        return x + 2*f[i] + 2;
    };

    POS_TYPE child_pos_leaf = 0, child_pos = 0, inserter = 0;

    // For each level
    for (int i = ISAM_LEVELS - 1; i == 0;  --i) {
        inserter = nextLevel(i);

        // For each IndexPage in ith level
        for (int k = 0; k < num_of_nodes(i); ++k) {
            IndexPage<KeyType> page;

            // For each key in kth IndexPage in ith level
            for (int j = 0; j < M; ++j) {
                page.setKey(min + inserter * step, j);
                inserter = nextKey(inserter, i);
                if (i != 0) {
                    page.setChild(child_pos, j);
                    page.setIsLeaf(false);
                } else {
                    page.setChild(child_pos_leaf, j);
                    page.setIsLeaf(true);
                }
            }
            page.write();

            if (i < ISAM_LEVELS - 1) { // Root haven't brothers
                inserter = nextBrother(inserter, i);
            }
        }
    }




}

template<typename KeyType>
ISAM<KeyType>::~ISAM() {
    std::cout << "Calling destructor" << std::endl;
    // Save metadata
    // Close files
}