#include "FileSystem.h"
#include "FileNode.h"

void FileSystem::createFile(const shared_ptr<FileNode>& parent, const string& fileName, int fileSize)
{
    if (!parent) {
        cerr << "Erro: O pai não pode ser nulo.\n";
        return;
    }

    string filePath = parent->name + "/" + fileName;

    auto newFile = make_shared<FileNode>(fileName, true, fileSize);
    ofstream file(filePath, ios::binary);
    if (!file) {
        cerr << "Erro ao criar o arquivo " << fileName << ".\n";
        return;
    }
    file.seekp(fileSize - 1);
    file.write("", 1);
    file.close();

    parent->children.push_back(newFile);
    newFile->parent = parent;
    
    cout << "Arquivo '" << fileName << " Pai:" << parent << "' criado com tamanho " << fileSize << " bytes.\n";
    addToIndex(fileName, fileSize, true);
}

void FileSystem::deleteFile(const shared_ptr<FileNode>& parent, const string& fileName)
{
    if (parent == nullptr) {
        cerr << "Erro: O pai não pode ser nulo.\n";
        return;
    }

    auto it = find_if(parent->children.begin(), parent->children.end(),
        [&](const shared_ptr<FileNode>& child) {
            return child->isFile && child->name == fileName;
        });

    if (it != parent->children.end()) {
        string path = (*it)->name;
        
        if (remove(path.c_str()) == 0) {
            parent->children.erase(it);
            cout << "Arquivo '" << fileName << "' deletado.\n";
            removeFromIndex(fileName);
        } else {
            cerr << "Erro ao deletar o arquivo " << fileName << ". Verifique se o arquivo existe e se você tem permissões adequadas.\n";
        }
    } else {
        cerr << "Arquivo '" << fileName << "' não encontrado.\n";
    }
}

void FileSystem::resizeFile(const shared_ptr<FileNode>& parent, const string& fileName, int newSize)
{
    auto fileNode = searchFile(parent, fileName);
    if (fileNode && fileNode->isFile)
    {
        ifstream file(fileName, ios::binary | ios::ate);
        if (!file)
        {
            cerr << "Erro ao abrir o arquivo " << fileName << " para redimensionamento.\n";
            return;
        }

        streamsize currentSize = file.tellg();
        file.close();

        if (newSize < currentSize)
        {
            ofstream file(fileName, ios::binary | ios::in | ios::out);
            if (!file)
            {
                cerr << "Erro ao abrir o arquivo " << fileName << " para redimensionamento.\n";
                return;
            }

            file.seekp(newSize);
            file.write("", 1);
            file.close();

            fileNode->fileSize = newSize;
            updateIndex();

            cout << "Arquivo '" << fileName << "' redimensionado para " << newSize << " bytes.\n";
        }
        else if (newSize > currentSize)
        {
            ofstream file(fileName, ios::binary | ios::in | ios::out);
            if (!file)
            {
                cerr << "Erro ao abrir o arquivo " << fileName << " para redimensionamento.\n";
                return;
            }

            file.seekp(newSize - 1);
            file.write("", 1);
            file.close();

            fileNode->fileSize = newSize;
            updateIndex();

            cout << "Arquivo '" << fileName << "' redimensionado para " << newSize << " bytes.\n";
        }
        else
        {
            cout << "O arquivo '" << fileName << "' já está no tamanho desejado.\n";
        }
    }
    else
    {
        cerr << "Arquivo '" << fileName << "' não encontrado.\n";
    }
}

shared_ptr<FileNode> FileSystem::searchFile(const shared_ptr<FileNode>& parent, const string& fileName)
{
    if (!parent) {
        cerr << "Erro: O pai não pode ser nulo.\n";
        return nullptr;
    }

    for (const auto& child : parent->children) {
        if (child->isFile && child->name == fileName) {
            cout << "Arquivo '" << fileName << "' encontrado com tamanho: " << child->fileSize << " bytes.\n";
            return child;
        }
    }
    cerr << "Arquivo '" << fileName << "' não encontrado.\n";
    return nullptr;
}

void FileSystem::loadIndex()
{
    ifstream indexFile("index.txt");
    if (!indexFile) {
        cout << "Nenhum índice encontrado. Iniciando um novo.\n";
        return;
    }

    string line;
    while (getline(indexFile, line)) {
        size_t spacePos = line.find(' ');
        size_t spacePos2 = line.find(' ', spacePos + 1);
        string fileName = line.substr(0, spacePos);
        char type = line[spacePos + 1];
        int fileSize = (type == 'f') ? stoi(line.substr(spacePos2 + 1)) : 0;
        auto newFile = make_shared<FileNode>(fileName, type == 'f', fileSize);        
        root->children.push_back(newFile);
    }
}

void FileSystem::addToIndex(const string& name, int size, bool isFile) {
    ofstream indexFile("index.txt", ios::app);
    indexFile << name << " " << (isFile ? 'f' : 'd') << " " << size << endl;
}

void FileSystem::removeFromIndex(const string& fileName)
{
    ifstream indexFile("index.txt");
    vector<string> lines;
    string line;

    while (getline(indexFile, line)) {
        if (line.find(fileName) != 0) {
            lines.push_back(line);
        }
    }
    indexFile.close();

    ofstream outIndexFile("index.txt");
    for (const auto& l : lines) {
        outIndexFile << l << endl;
    }
}

void FileSystem::updateIndex()
{
    ofstream outIndexFile("index.txt");
    for (const auto& child : root->children) {
        if (child->isFile) {
            outIndexFile << child->name << " " << child->fileSize << endl;
        }
    }
}

void FileSystem::createDirectory(const shared_ptr<FileNode>& parent, const string& dirName)
{
    if (!parent) {
        cerr << "Erro: O diretório pai não foi especificado.\n";
        return;
    }

    auto existingNode = find_if(parent->children.begin(), parent->children.end(),
        [&](const shared_ptr<FileNode>& child) {
            return child->name == dirName;
        });

    if (existingNode != parent->children.end()) {
        cerr << "Erro: Já existe um arquivo ou diretório com o nome '" << dirName << "'.\n";
        return;
    }

    if (mkdir(dirName.c_str()) != 0) {
        cerr << "Erro ao criar o diretório '" << dirName << "'.\n";
        return;
    }

    auto newDir = make_shared<FileNode>(dirName, false);
    newDir->parent = parent;
    parent->children.push_back(newDir);
    addToIndex(dirName, 0, false); 

    cout << "Diretório '" << dirName << "' criado com sucesso.\n";
}

void FileSystem::deleteDirectory(const shared_ptr<FileNode>& parent, const string& dirName)
{
    if (parent == nullptr) {
        cerr << "Erro: O diretório pai não foi especificado.\n";
        return;
    }

    auto it = find_if(parent->children.begin(), parent->children.end(),
        [&](const shared_ptr<FileNode>& child) {
            return !child->isFile && child->name == dirName;
        });

    if (it != parent->children.end()) {
        string path = (*it)->name;

        if (rmdir(path.c_str()) == 0) {
            parent->children.erase(it);
            removeFromIndex(dirName); 
            cout << "Diretório '" << dirName << "' deletado com sucesso.\n";
        } else {
            cerr << "Erro ao deletar o diretório '" << dirName << "'. Verifique se ele está vazio ou as permissões.\n";
        }
    } else {
        cerr << "Diretório '" << dirName << "' não encontrado.\n";
    }
}

void FileSystem::listDirectory(const shared_ptr<FileNode>& dir)
{
    if (!dir) {
        cerr << "Erro: O diretório não foi especificado.\n";
        return;
    }

    if (dir->children.empty()) {
        cout << "O diretório '" << dir->name << "' está vazio.\n";
        return;
    }

    cout << "Conteúdo do diretório '" << dir->name << "':\n";
    for (const auto& child : dir->children) {
        if (child->isFile) {
            cout << "[Arquivo] " << child->name << " (" << child->fileSize << " bytes)\n";
        } else {
            cout << "[Diretório] " << child->name << "\n";
        }
    }
}

void FileSystem::changeDirectory(const std::string& dirName, std::shared_ptr<FileNode> &currentDirectory) {
    if (dirName == ".." && currentDirectory->parent) {
        currentDirectory = currentDirectory->parent;
        std::cout << "Subindo um nivel.\n";
    } else {
        for (const auto& child : currentDirectory->children) {
            if (!child->isFile && child->name == dirName) {
                currentDirectory = child;
                std::cout << "Diretorio alterado para: " << currentDirectory->name << "\n";
                return;
            }
        }
        std::cout << "Diretorio nao encontrado. \n";
    }
}

void FileSystem::editFileContent(const std::string& fileName, const std::string& content) {
    auto fileNode = searchFile(currentDirectory, fileName);
    if (fileNode && fileNode->isFile) {
        fileNode->appendContent(content);
        std::cout << "Conteúdo adicionado ao arquivo '" << fileName << "'.\n";
    } else {
        std::cout << "Arquivo não encontrado ou não é um arquivo de texto.\n";
    }
}
void FileNode::appendContent(const std::string &content)
{
    if (isFile)
    {
        std::ofstream file(name, std::ios::app);
        if (file.is_open()) {
            file << content;
            fileSize += content.size();
            file.close();
        }
        else {
            std::cout << "Erro ao abrir o arquivo " << name << std::endl;
        }
    }
    else
    {
        std::cout << "Não é um arquivo, não pode adicionar conteúdo!" << std::endl;
    }
}

FileNode::FileNode(const std::string& name, bool isFile, int fileSize)
    : name(name), isFile(isFile), fileSize(fileSize) {}
