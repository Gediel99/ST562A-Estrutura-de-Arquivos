#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class FileNode
{
public:
    string name;
    bool isFile;
    int fileSize;
    shared_ptr<FileNode> parent;
    vector<shared_ptr<FileNode>> children;

    FileNode(const string &name, bool isFile, int fileSize = 0)
        : name(name), isFile(isFile), fileSize(fileSize) {}

    shared_ptr<FileNode> findChild(const string &name)
    {
        for (auto &child : children)
        {
            if (child->name == name)
                return child;
        }
        return nullptr;
    }
};