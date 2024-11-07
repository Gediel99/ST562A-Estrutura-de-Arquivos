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
    shared_ptr<FileNode> currentDirectory;  // Diretório atual
    
    FileSystem() {
        root = make_shared<FileNode>("root", false);
        currentDirectory = root; // Inicializa o diretório atual com o diretório raiz
        loadIndex();
    }

    // Declarações das funções
    void changeDirectory(const std::string& dirName, std::shared_ptr<FileNode> &currentDirectory);
    void createFile(const shared_ptr<FileNode>& parent, const string& fileName, int fileSize);
    void deleteFile(const shared_ptr<FileNode>& parent, const string& fileName);
    void resizeFile(const shared_ptr<FileNode>& parent, const string& fileName, int newSize);
    shared_ptr<FileNode> searchFile(const shared_ptr<FileNode>& parent, const string& fileName);
    void createDirectory(const shared_ptr<FileNode>& parent, const string& dirName);
    void deleteDirectory(const shared_ptr<FileNode>& parent, const string& dirName);
    void loadIndex();
    void updateIndex();
    void addToIndex(const string& fileName, int fileSize, bool isFile);
    void removeFromIndex(const string& fileName);
    void listDirectory(const shared_ptr<FileNode>& dir);
    void editFileContent(const std::string& fileName, const std::string& content);
};


#endif
