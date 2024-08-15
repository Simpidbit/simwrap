#include "simdir.h"

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

#include <iostream>

#include <string.h>

using namespace simpid;
using Enumdir::Filenode;

using std::vector;
using std::string;


vector<Filenode>
Enumdir::enum_dir(string path)
{
    vector<Filenode> nodes;

#ifdef _WIN32
    _finddata_t file_info;
    string search_range = path + "\\*";
    intptr_t handle = _findfirst(search_range.c_str(), &file_info);
    if (-1 == handle) {
        std::cout << "Failed" << std::endl;
        return nodes;
    }

    Filenode fnd;
    do {
        fnd.name = file_info.name;
        if (file_info.attrib == _A_SUBDIR) 
            fnd.type = DIRECTORY_DIRECTORY;
        else 
            fnd.type = DIRECTORY_FILE;
        fnd.last_modify_time = file_info.time_write;
        fnd.size = file_info.size;
        nodes.push_back(fnd);
        memset(&fnd, 0, sizeof(fnd));
    } while (!_findnext(handle, &file_info));

    _findclose(handle);
    return nodes;

#else
    DIR *dirp = opendir(path.c_str());
    struct dirent *dp;
    if (!dirp) {
        std::cout << "Failed" << std::endl;
        return nodes;
    }

    Filenode fnd;
    struct stat status;
    while ((dp = readdir(dirp)) != 0) {
        fnd.name = dp->d_name;
        if (dp->d_type == DT_DIR)
            fnd.type = DIRECTORY_DIRECTORY;
        else
            fnd.type = DIRECTORY_FILE;

        int fd = open(fnd.name.c_str(), O_RDONLY);
        fstat(fd, &status);
        fnd.size = status.st_size;
        fnd.last_modify_time = status.st_mtimespec.tv_sec;
        nodes.push_back(fnd);
        memset(&fnd, 0, sizeof(fnd));
        memset(&status, 0, sizeof(fnd));
    }
    (void)closedir(dirp);
    return nodes;
#endif
}
