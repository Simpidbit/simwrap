#include "simdir.h"

#include <iostream>


int main()
{
    auto files = SP::Enumdir::enum_dir();
    for (auto each : files) {
        std::cout << each.name << std::endl;
        std::cout << (int)each.type << std::endl;
        std::cout << each.last_modify_time << std::endl;
        std::cout << each.size << std::endl;
        std::cout << "------------" << std::endl;
    }
    return 0;
}
