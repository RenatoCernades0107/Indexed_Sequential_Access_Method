//
// Created by renat on 9/7/2023.
//

#ifndef INDEXED_SEQUENTIAL_ACCESS_METHOD_ISAM_H
#define INDEXED_SEQUENTIAL_ACCESS_METHOD_ISAM_H

#include "Metadata.h"

#include <algorithm>
#include <limits>
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
template <typename KeyType> class ISAM {
private:
  Metadata<KeyType> metadata;
  POS_TYPE root; // Root of the ISAM

  std::string idx_filename =
      "isam_index_file.dat";                      // Name of the index page file
  std::string dt_filename = "isam_data_file.dat"; // Name of the data page file

  std::fstream idx_file; // Index page file
  std::fstream dt_file;  // Data page file
public:
  ISAM();

  const std::string &getIdxFilename() const;

  const std::string &getDtFilename() const;

  explicit ISAM(std::vector<std::pair<KeyType, POS_TYPE>> &data);

  bool open_files(std::ios::openmode op);

  void close_files();

  POS_TYPE max_records(POS_TYPE levels);

  POS_TYPE search(KeyType key);

  POS_TYPE add(std::pair<KeyType, POS_TYPE> record);

  POS_TYPE remove(KeyType key);

  std::vector<POS_TYPE> range_search(KeyType beg, KeyType end);

  ~ISAM();

  void build(std::vector<std::pair<KeyType, POS_TYPE>> &data);

private:
  void create_files_if_not_exist();

  POS_TYPE _search(POS_TYPE node_pos, KeyType key);

  POS_TYPE _insert(POS_TYPE node_pos, std::pair<KeyType, POS_TYPE> record);

  void _range_search(POS_TYPE node_pos, KeyType beg, KeyType end,
                     std::vector<POS_TYPE> &result);

  POS_TYPE _remove(POS_TYPE node_pos, KeyType key);
};

extern template class ISAM<int>;

#endif // INDEXED_SEQUENTIAL_ACCESS_METHOD_ISAM_H
