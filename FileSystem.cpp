#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include "FileNode.c++"

using namespace std;

class FileSystem
{
public:
    shared_ptr<FileNode> root;

    FileSystem()
    {
        root = make_shared<FileNode>("root", false);
        loadIndex(); // Carrega o índice ao iniciar
    }

    void createFile(const shared_ptr<FileNode> &parent, const string &fileName, int fileSize);
    void deleteFile(const shared_ptr<FileNode> &parent, const string &fileName);
    void resizeFile(const shared_ptr<FileNode> &parent, const string &fileName, int newSize);
    shared_ptr<FileNode> searchFile(const shared_ptr<FileNode> &parent, const string &fileName);
    void loadIndex();
    void updateIndex();
    void addToIndex(const string &fileName, int fileSize);
    void removeFromIndex(const string &fileName);
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
    addToIndex(fileName, fileSize); // Atualiza o índice
}

void FileSystem::deleteFile(const shared_ptr<FileNode> &parent, const string &fileName)
{
    if (parent == nullptr)
    {
        cerr << "Erro: O pai não pode ser nulo.\n";
        return;
    }

    auto it = remove_if(parent->children.begin(), parent->children.end(),
                        [&](const shared_ptr<FileNode> &child)
                        {
                            return child->isFile && child->name == fileName;
                        });

    if (it != parent->children.end())
    {
        string path = (*it)->name;
        if (remove(path.c_str()) == 0)
        {
            parent->children.erase(it);
            cout << "Arquivo '" << fileName << "' deletado.\n";
            removeFromIndex(fileName); // Atualiza o índice
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
    auto fileNode = searchFile(parent, fileName);
    if (fileNode && fileNode->isFile)
    {
        ofstream file(fileName, ios::binary | ios::in | ios::out);
        if (!file)
        {
            cerr << "Erro ao abrir o arquivo " << fileName << " para redimensionamento.\n";
            return;
        }

        // Redimensiona o arquivo
        file.seekp(newSize - 1);
        file.write("", 1);
        file.close();

        // Atualiza o tamanho do nó do arquivo
        fileNode->fileSize = newSize;

        // Atualiza o índice (defina sua lógica aqui)
        updateIndex();

        cout << "Arquivo '" << fileName << "' redimensionado para " << newSize << " bytes.\n";
    }
    else
    {
        cerr << "Arquivo '" << fileName << "' não encontrado.\n";
    }
}

shared_ptr<FileNode> FileSystem::searchFile(const shared_ptr<FileNode>& parent, const string& fileName)
{
    for (const auto& child : parent->children)
    {
        if (child->isFile && child->name == fileName)
        {
            cout << "Arquivo '" << fileName << "' encontrado com tamanho: " << child->fileSize << " bytes.\n";
            return child;
        }
    }
    cout << "Arquivo '" << fileName << "' não encontrado.\n";
    return nullptr;
}


void FileSystem::loadIndex()
{
    ifstream indexFile("index.txt");
    if (!indexFile)
    {
        cout << "Nenhum índice encontrado. Iniciando um novo.\n";
        return;
    }

    string line;
    while (getline(indexFile, line))
    {
        // Supondo que o formato seja: nome tamanho
        size_t spacePos = line.find(' ');
        string fileName = line.substr(0, spacePos);
        int fileSize = stoi(line.substr(spacePos + 1));
        auto newFile = make_shared<FileNode>(fileName, true, fileSize);
        root->children.push_back(newFile); // Adiciona ao root ou ao pai adequado
    }
}

void FileSystem::addToIndex(const string &fileName, int fileSize)
{
    ofstream indexFile("index.txt", ios::app);
    indexFile << fileName << " " << fileSize << endl;
}

void FileSystem::removeFromIndex(const string &fileName)
{
    // Lê todas as entradas existentes
    ifstream indexFile("index.txt");
    vector<string> lines;
    string line;

    // Adiciona as linhas ao vetor, exceto a que deve ser removida
    while (getline(indexFile, line))
    {
        if (line.find(fileName) != 0)
        { // Se a linha não começa com o nome do arquivo
            lines.push_back(line);
        }
    }
    indexFile.close();

    // Escreve de volta o arquivo de índice
    ofstream outIndexFile("index.txt");
    for (const auto &l : lines)
    {
        outIndexFile << l << endl;
    }
}

void FileSystem::updateIndex()
{
    // Para redimensionar um arquivo, você precisará reescrever o índice
    ofstream outIndexFile("index.txt");
    for (const auto &child : root->children)
    {
        if (child->isFile)
        {
            outIndexFile << child->name << " " << child->fileSize << endl;
        }
    }
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
        cout << "4. Pesquisar Arquivo\n";
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
        
        case 4:
            cout << "Digite o nome do arquivo a ser pesquisado: ";
            cin >> fileName;
            fs.searchFile(fs.root, fileName);
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