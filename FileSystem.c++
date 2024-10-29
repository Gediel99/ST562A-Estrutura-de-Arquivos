#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include "FileNode.c++"

 using namespace std;
 
 class FileSystem {
    public:
        shared_ptr<FileNode> root;

        FileSystem() {
            root = make_shared<FileNode>("root", false);
        }

        void createFile(const shared_ptr<FileNode>& parent, const string& fileName, int fileSize);
        void deleteFile(const shared_ptr<FileNode>& parent, const string& fileName);
        void resizeFile(const shared_ptr<FileNode>& parent, const string& fileName, int newSize);
        shared_ptr<FileNode> searchFile(const shared_ptr<FileNode>& parent, const string& fileName);
    };

    void FileSystem::createFile(const shared_ptr<FileNode>& parent, const string& fileName, int fileSize) {
        auto newFile = make_shared<FileNode>(fileName, true, fileSize);

        ofstream file(fileName, ios::binary);
        if (!file) {
            cerr << "Erro ao criar o arquivo " << fileName << ".\n";
            return;
        }
        file.seekp(fileSize - 1);
        file.write("", 1);
        file.close();

        newFile->parent = parent;
        parent->children.push_back(newFile);
        cout << "Arquivo '" << fileName << "' criado com tamanho " << fileSize << " bytes.\n";
    }

    void FileSystem::deleteFile(const shared_ptr<FileNode>& parent, const string& fileName) {
        for (int i = 0; i < parent->children.size(); i++) {
            if (parent->children[i]->name == fileName) {
                parent->children.erase(parent->children.begin() + i);
                return;
            }
        }
    }

    void FileSystem::resizeFile(const shared_ptr<FileNode>& parent, const string& fileName, int newSize) {
        auto file = searchFile(parent, fileName);
        if (file) file->fileSize = newSize;
    }

    shared_ptr<FileNode> FileSystem::searchFile(const shared_ptr<FileNode>& parent, const string& fileName) {
        for (auto& child : parent->children) {
            if (child->name == fileName) return child;
        }
        return nullptr;
    }

    int main() {
        FileSystem fs;
        fs.createFile(fs.root, "file1", 10);
        fs.createFile(fs.root, "file2", 20);
        fs.createFile(fs.root, "file3", 30);
        fs.resizeFile(fs.root, "file2", 25);
        fs.deleteFile(fs.root, "file3");
        return 0;
    }