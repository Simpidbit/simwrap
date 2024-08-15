#ifndef SIMPIDBIT_SIMWRAP_H
#define SIMPIDBIT_SIMWRAP_H

#define DIRECTORY_DIRECTORY     0x01
#define DIRECTORY_FILE          0x02

// Standard C Library
#include <stdint.h>

// Standard C++ Library
#include <string>
#include <vector>


namespace simpid {

namespace Enumdir {
    typedef struct {
        std::string     name;
        uint8_t         type;
        uint64_t        last_modify_time;
        uint64_t        size;
    } Filenode;
    std::vector<Filenode> enum_dir(std::string path = ".");
}

}

namespace spb       = simpid;
namespace Simpid    = simpid;
namespace SP        = simpid;
namespace SPB       = simpid;

#endif
