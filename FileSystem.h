#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "FileNode.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

class FileSystem {
public:
    shared_ptr<FileNode> root;

    FileSystem() {
        root = make_shared<FileNode>("root", false);
        loadIndex(); 
    }

    void createFile(const shared_ptr<FileNode>& parent, const string& fileName, int fileSize);
    void deleteFile(const shared_ptr<FileNode>& parent, const string& fileName);
    void resizeFile(const shared_ptr<FileNode>& parent, const string& fileName, int newSize);
    shared_ptr<FileNode> searchFile(const shared_ptr<FileNode>& parent, const string& fileName);
    void createDirectory(const shared_ptr<FileNode>& parent, const string& dirName);// to do
    void deleteDirectory(const shared_ptr<FileNode>& parent, const string& dirName);// to do
    void loadIndex();
    void updateIndex();
    void addToIndex(const string& fileName, int fileSize, bool isFile);
    void removeFromIndex(const string& fileName);
    void listDirectory(const shared_ptr<FileNode>& dir); //to do
};

#endif
