#ifndef FILENODE_H
#define FILENODE_H

#include <memory>
#include <string>
#include <vector>

class FileNode {
public:
    std::string name;
    bool isFile;
    int fileSize;
    std::shared_ptr<FileNode> parent;
    std::vector<std::shared_ptr<FileNode>> children;

    FileNode(const std::string& name, bool isFile, int fileSize = 0);

    // Declare the appendContent method in the class header
    void appendContent(const std::string &content); 
};

#endif
