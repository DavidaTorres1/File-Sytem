#include "FileNode.h"

FileNode::FileNode(const std::string& n, bool isDir) : name(n), isDirectory(isDir), left(nullptr), right(nullptr) {}