#ifndef SIMPIDBIT_BINARY_SEARCH_TREE_H
#define SIMPIDBIT_BINARY_SEARCH_TREE_H

#include <utility>
#include <stdint.h>


namespace simpid {


template <typename T, typename child_container_t = std::pair<void *, void *>>
class Nodefortrees {
public:
    T                   value;
    child_container_t   childs;
    Nodefortrees        *parent;
    uint64_t            repeat_time;

    Nodefortrees();
    ~Nodefortrees();
};

class BSTree {
public:
    // 若有重复，是否记录
    bool if_record_repeats;

    simpid::Nodefortrees<int, std::pair<void *, void *>> root;

public:
    BSTree(bool if_record_repeats = false);
    ~BSTree();

    // 添加元素
    void add();
};


}       // namespace simpid

#endif
