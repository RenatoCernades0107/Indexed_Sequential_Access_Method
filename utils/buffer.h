//
// Created by renat on 9/10/2023.
//

#ifndef INDEXED_SEQUENTIAL_ACCESS_METHOD_BUFFER_H
#define INDEXED_SEQUENTIAL_ACCESS_METHOD_BUFFER_H

#include <cstdint>

#if defined(_WIN64)
#include <windows.h>
#elif defined(__unix__)
#include <unistd.h>
#endif

namespace buffer {
    const unsigned long get_buffer_size() {
    #if defined(_WIN64)
            SYSTEM_INFO systemInfo;
            GetSystemInfo(&systemInfo);
            DWORD page_size = systemInfo.dwPageSize;
            return page_size;
    #elif defined(__unix__)
            return getpagesize();
    #endif
    }

    const int64_t BUFFER_SIZE = get_buffer_size();
}

/*
 * Special acknowledgments to Juan Diego Castro
 * Reference: https://github.com/ByJuanDiego/disk-static-hash/blob/master/utils/buffer_size.h
 */

#endif //INDEXED_SEQUENTIAL_ACCESS_METHOD_BUFFER_H
