#ifndef SIMPIDBIT_SIMPOOL_H
#define SIMPIDBIT_SIMPOOL_H

#include <stdint.h>

namespace simpid {


class Simpool {
public:



public:
    Simpool(uint64_t amount_size = 1024 * 1024);
    ~Simpool();

    void allocmem(uint64_t size);
};



}       // namespace simpid


#endif
