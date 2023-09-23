//
// Created by renat on 9/7/2023.
//

#include "../include/ISAM.h"

template <typename KeyType> ISAM<KeyType>::ISAM() {
  std::cout << "Calling constructor" << std::endl;

  create_files_if_not_exist();

  root = -1;

  close_files();
};

template <typename KeyType>
ISAM<KeyType>::ISAM(std::vector<std::pair<KeyType, POS_TYPE>> &data) {
  std::cout << "Calling constructor" << std::endl;

  root = metadata.getRootPosition();

  create_files_if_not_exist();
  close_files();

  build(data);
}

template <typename KeyType>
bool ISAM<KeyType>::open_files(std::ios::openmode op) {
  idx_file.open(idx_filename, op);
  dt_file.open(dt_filename, op);
  return (idx_file.good() && dt_file.good());
}

template <typename KeyType> void ISAM<KeyType>::close_files() {
  idx_file.close();
  dt_file.close();
}

template <typename KeyType>
const std::string &ISAM<KeyType>::getIdxFilename() const {
  return idx_filename;
}

template <typename KeyType>
const std::string &ISAM<KeyType>::getDtFilename() const {
  return dt_filename;
}

template <typename KeyType>
POS_TYPE ISAM<KeyType>::max_records(POS_TYPE levels) {
  POS_TYPE M = metadata.getIndexPageCapacity();
  POS_TYPE N = metadata.getDataPageCapacity();
  POS_TYPE mult = 1;
  for (int i = 0; i < levels; ++i) {
    mult *= M + 1;
  }
  return mult * N;
}

template <typename KeyType> void ISAM<KeyType>::create_files_if_not_exist() {
  // Create the index and data page file if not exist
  if (!open_files(std::ios::in)) {
    std::cout << "Files didn't exists" << std::endl;
    if (!open_files(std::ios::out)) {
      throw std::runtime_error("Files couldn't open");
    };
  }
}

template <typename KeyType>
void ISAM<KeyType>::build(std::vector<std::pair<KeyType, POS_TYPE>> &data) {
  POS_TYPE M = metadata.getIndexPageCapacity();
  POS_TYPE N = metadata.getDataPageCapacity();

  if (data.size() < (size_t)max_records(ISAM_LEVELS))
    throw std::runtime_error("It's necesary have more than " +
                             std::to_string(max_records(ISAM_LEVELS)) +
                             " in order to create the ISAM");

  auto compare = [](std::pair<KeyType, POS_TYPE> &a,
                    std::pair<KeyType, POS_TYPE> &b) {
    return a.first < b.first;
  };
  std::sort(begin(data), end(data), compare);

  open_files(std::ios::binary | std::ios::out);

  POS_TYPE index = 0;            // Data index
  IndexPage<KeyType> page0; //  first level page
  for (int j = 0; j < M + 1; ++j) {
    IndexPage<KeyType> page1; // second level page
    for (int k = 0; k < M + 1; ++k) {
      IndexPage<KeyType> page2; // third level page
      for (int h = 0; h < M + 1; ++h) {
        DataPage<KeyType> page3; // data level page
        for (int l = 0; l < N; ++l) {
          page3.setRecord(data[index], l);
          ++index;
        }
        POS_TYPE pos = page3.write(dt_file);
        page2.setChild(pos, h);
        if (h < M)
          page2.setKey(data[index].first, h);
      }
      page2.setIsLeaf(true);
      POS_TYPE pos = page2.write(idx_file);
      page1.setChild(pos, k);
      if (k < M)
        page1.setKey(data[index].first, k);
    }
    page1.setIsLeaf(false);
    POS_TYPE pos = page1.write(idx_file);
    page0.setChild(pos, j);
    if (j < M)
      page0.setKey(data[index].first, j);
  }
  page0.setIsLeaf(false);
  root = page0.write(idx_file);

  // Save root position in metadata
  metadata.setRootPosition(root);

  close_files();

  // Insert the rest of records
  if (data.size() > (size_t)max_records(ISAM_LEVELS)) {
    while ((size_t)index < data.size()) {
      this->add(data[index]);
      ++index;
    }
  }
}

template <typename KeyType> POS_TYPE ISAM<KeyType>::search(KeyType key) {
  if (root == -1)
    throw std::runtime_error("No indexed data in ISAM, please build before");

  open_files(std::ios::binary | std::ios::in);
  POS_TYPE pos = _search(root, key);
  close_files();
  return pos;
}

template <typename KeyType>
POS_TYPE ISAM<KeyType>::_search(POS_TYPE node_pos, KeyType key) {
  // Read the node
  idx_file.seekg(node_pos, std::ios::beg);
  IndexPage<KeyType> node;
  node.read(idx_file);

  POS_TYPE M = metadata.getIndexPageCapacity();

    int index = 0;
    // Search left-to-right where go down

    while (index < M && node.getkey(index) <= key) {
        ++index;
    }

    if (node.getIsLeaf()) {
        DataPage<KeyType> page;
        POS_TYPE data_pos = node.getChildren()[index];
        do {
            // Read the Data page
            dt_file.seekg(data_pos, std::ios::beg);
            page.read(dt_file);

      for (int i = 0; i < page.getCount(); ++i) {
        if (page.getKeys()[i] == key) {
          return page.getRecords()[i];
        }
      }

      // Continue with the next page
      data_pos = page.getNext();

    } while (data_pos != -1);
        return POS_TYPE(-1);
    } else {
        // Call recursively search with the new node
        POS_TYPE new_pos = node.getChild(index);
        return _search(new_pos, key);
    }
}

template <typename KeyType>
POS_TYPE ISAM<KeyType>::add(std::pair<KeyType, POS_TYPE> record) {
  if (root == -1)
    throw std::runtime_error("No indexed data in ISAM, please build before");

  open_files(std::ios::binary | std::ios::in | std::ios::app);
  POS_TYPE pos = _insert(root, record);
  close_files();

  return pos;
}

template <typename KeyType>
POS_TYPE ISAM<KeyType>::_insert(POS_TYPE node_pos,
                                std::pair<KeyType, POS_TYPE> record) {
  // Read the node
  idx_file.seekg(node_pos, std::ios::beg);
  IndexPage<KeyType> node;
  node.read(idx_file);

  POS_TYPE M = metadata.getIndexPageCapacity();

  int index = 0;
  // Find a key to descend through the tree
  KeyType *keys = node.getKeys();
  while (index < M && keys[index] <= record.first) {
    ++index;
  }

  if (node.getIsLeaf()) {

    DataPage<KeyType> page;
    POS_TYPE data_pos = node.getChildren()[index];

    do {
      // Read the Data page
      dt_file.seekg(data_pos, std::ios::beg);
      page.read(dt_file);

      // Check duplicates
      int i = 0;
      for (; i < page.getCount(); ++i) {
        if (page.getKeys()[i] == record.first) {
          std::cout << "Duplicado" << std::endl;
          return page.getRecords()[i];
        }
      }

      // Insert in a non full sorted data page
      if (!page.isFull()) {
        dt_file.seekp(data_pos, std::ios::beg);
        POS_TYPE j = page.getCount() - 1;
        while (j > 0 && page.getKeys()[j] >= record.first) {
          page.setRecord(page.getRecord(j), j + 1);
          --j;
        }
        page.setRecord(record, j);
        page.write(dt_file);
        return record.second;
      }

      // Continue with the next page
      data_pos = page.getNext();

    } while (data_pos != -1);

    // Creating a new data page at the end of the file
    dt_file.seekp(0, std::ios::end);
    POS_TYPE new_pos = dt_file.tellp();

    DataPage<KeyType> new_page;
    new_page.setRecord(record, 0);
    new_page.write(dt_file);

    // Linking with the other page
    page.setNext(new_pos);
    dt_file.seekp(node.getChildren()[index], std::ios::beg);
    page.write(dt_file);

    return record.second;
  } else {
    POS_TYPE pos = node.getChildren()[index];
    return _insert(pos, record);
  }
}

template <typename KeyType>
std::vector<POS_TYPE> ISAM<KeyType>::range_search(KeyType beg, KeyType end) {
  if (root == -1)
    throw std::runtime_error("No indexed data in ISAM, please build before");

  std::vector<POS_TYPE> result;

  open_files(std::ios::binary | std::ios::in | std::ios::app);
  _range_search(root, beg, end, result);
  close_files();

  return result;
}

template <typename KeyType>
void ISAM<KeyType>::_range_search(POS_TYPE node_pos, KeyType beg, KeyType end,
                                  std::vector<POS_TYPE> &result) {
  // Read the node
  idx_file.seekg(node_pos, std::ios::beg);
  IndexPage<KeyType> node;
  node.read(idx_file);

  POS_TYPE M = metadata.getIndexPageCapacity();

  // Find a key to descend through the tree
  int index = 0;
  KeyType *keys = node.getKeys();
  while (index < M && keys[index] < beg) {
    ++index;
  }

  if (node.getIsLeaf()) {
    // ISAM has (M+1)^level of DATA PAGES and each of them have a fix
    auto last_pos = static_cast<POS_TYPE>(std::ceil(pow(static_cast<double>(M + 1), ISAM_LEVELS) *
                                                        static_cast<double>(DataPage<KeyType>::size_of())));
    KeyType key = beg;
    POS_TYPE data_pos = node.getChildren()[index];
    do {
      DataPage<KeyType> page;

      // Iterate through linked data pages
      POS_TYPE _pos = data_pos;
      do {
        // Read the Data page
        dt_file.seekg(_pos, std::ios::beg);
        page.read(dt_file);

        // Push the key in the range [beg, end]
        for (int i = 0; i < page.getCount(); ++i) {
          key = page.getKeys()[i];
          if (beg <= key && key <= end) {
            result.push_back(page.getRecords()[i]);
          }
        }

        // Continue with the next page
        _pos = page.getNext();
      } while (_pos != -1);

      // Go to the next brother
      data_pos += page.size_of();
    } while (data_pos < last_pos && key <= end);
    return;
  } else {
    POS_TYPE new_pos = node.getChildren()[index];
    _range_search(new_pos, beg, end, result);
  }
}

template <typename KeyType> POS_TYPE ISAM<KeyType>::remove(KeyType key) {
  if (root == -1)
    throw std::runtime_error("No indexed data in ISAM, please build before");

  open_files(std::ios::binary | std::ios::in | std::ios::app);
  POS_TYPE result = _remove(root, key);
  close_files();

  return result;
}

template <typename KeyType>
POS_TYPE ISAM<KeyType>::_remove(POS_TYPE node_pos, KeyType key) {
  // Read the node
  idx_file.seekg(node_pos, std::ios::beg);
  IndexPage<KeyType> node;
  node.read(idx_file);

  POS_TYPE M = metadata.getIndexPageCapacity();

  int index = 0;
  // Search left-to-right where go down
  KeyType *keys = node.getKeys();
  while (index < M && keys[index] <= key) {
    ++index;
  }

  if (node.getIsLeaf()) {
    DataPage<KeyType> page;
    POS_TYPE data_pos = node.getChildren()[index];
    do {
      // Read the Data page
      dt_file.seekg(data_pos, std::ios::beg);
      page.read(dt_file);

      for (int i = 0; i < page.getCount(); ++i) {
        if (page.getKeys()[i] == key) {
          POS_TYPE deleted_pos = page.getRecord(i).second;
          page.remove(i);
          dt_file.seekp(data_pos, std::ios::beg);
          page.write(dt_file);
          return deleted_pos;
        }
      }

      // Continue with the next page
      data_pos = page.getNext();

    } while (data_pos != -1);

    return POS_TYPE(-1);
  } else {
    // Call recursively search with the new node
    POS_TYPE new_pos = node.getChildren()[index];
    return _search(new_pos, key);
  }
}

template <typename KeyType> ISAM<KeyType>::~ISAM() {
  std::cout << "Calling destructor" << std::endl;
  // Save metadata
  // Close files
}

template class ISAM<int>;
