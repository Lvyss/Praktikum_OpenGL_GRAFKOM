#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <string>

class FileSystem {
public:
    static std::string getPath(const std::string& path) {
        return path; // Simple implementation, just return the path as is
    }
};

#endif