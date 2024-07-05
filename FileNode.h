#ifndef FILE_NODE_H
#define FILE_NODE_H

#include <string>

struct FileNode {
    std::string name;
    bool isDirectory;
    FileNode* left;
    FileNode* right;

    FileNode(const std::string& n, bool isDir);
};

#endif // FILE_NODE_H