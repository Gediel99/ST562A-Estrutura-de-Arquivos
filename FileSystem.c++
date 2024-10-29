#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include "FileNode.c++"

using namespace std;

class FileSystem
{
public:
    shared_ptr<FileNode> root;

    FileSystem()
    {
        root = make_shared<FileNode>("root", false);
    }

    void createFile(const shared_ptr<FileNode> &parent, const string &fileName, int fileSize);
    void deleteFile(const shared_ptr<FileNode> &parent, const string &fileName);
    void resizeFile(const shared_ptr<FileNode> &parent, const string &fileName, int newSize);
    shared_ptr<FileNode> searchFile(const shared_ptr<FileNode> &parent, const string &fileName);
};

void FileSystem::createFile(const shared_ptr<FileNode> &parent, const string &fileName, int fileSize)
{
    auto newFile = make_shared<FileNode>(fileName, true, fileSize);

    ofstream file(fileName, ios::binary);
    if (!file)
    {
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

void FileSystem::deleteFile(const shared_ptr<FileNode> &parent, const string &fileName)
{
    auto it = remove_if(parent->children.begin(), parent->children.end(),
                        [&](const shared_ptr<FileNode> &child)
                        { return child->isFile && child->name == fileName; });

    if (it != parent->children.end())
    {
        string path = (*it)->name;
        if (remove(path.c_str()) == 0)
        {
            parent->children.erase(it);
            cout << "Arquivo '" << fileName << "' deletado.\n";
        }
        else
        {
            cerr << "Erro ao deletar o arquivo " << fileName << ".\n";
        }
    }
    else
    {
        cerr << "Arquivo '" << fileName << "' não encontrado.\n";
    }
}

void FileSystem::resizeFile(const shared_ptr<FileNode> &parent, const string &fileName, int newSize)
{
    auto file = searchFile(parent, fileName);
    if (file)
        file->fileSize = newSize;
}

shared_ptr<FileNode> FileSystem::searchFile(const shared_ptr<FileNode> &parent, const string &fileName)
{
    for (auto &child : parent->children)
    {
        if (child->name == fileName)
            return child;
    }
    return nullptr;
}

int main()
{
    FileSystem fs;
    int option;
    string fileName;
    int fileSize;

    do
    {
        cout << "\nMenu do Sistema de Arquivos:\n";
        cout << "1. Criar Arquivo\n";
        cout << "2. Deletar Arquivo\n";
        cout << "3. Redimensionar Arquivo\n";
        cout << "0. Sair\n";
        cout << "Escolha uma opção: ";
        cin >> option;

        switch (option)
        {
        case 1:
            cout << "Digite o nome do arquivo: ";
            cin >> fileName;
            cout << "Digite o tamanho do arquivo (em bytes): ";
            cin >> fileSize;
            fs.createFile(fs.root, fileName, fileSize);
            break;

        case 2:
            cout << "Digite o nome do arquivo a ser deletado: ";
            cin >> fileName;
            fs.deleteFile(fs.root, fileName);
            break;

        case 3:
            cout << "Digite o nome do arquivo a ser redimensionado: ";
            cin >> fileName;
            cout << "Digite o novo tamanho do arquivo (em bytes): ";
            cin >> fileSize;
            fs.resizeFile(fs.root, fileName, fileSize);
            break;

        case 0:
            cout << "Saindo do programa.\n";
            break;

        default:
            cout << "Opção inválida! Tente novamente.\n";
            break;
        }
    } while (option != 0);

    return 0;
}