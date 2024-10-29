#include "FileSystem.h"

void FileSystem::createFile(const shared_ptr<FileNode>& parent, const string& fileName, int fileSize)
{
    if (!parent) {
        cerr << "Erro: O pai não pode ser nulo.\n";
        return;
    }

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
    addToIndex(fileName, fileSize);
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
        string fileName = line.substr(0, spacePos);
        int fileSize = stoi(line.substr(spacePos + 1));
        auto newFile = make_shared<FileNode>(fileName, true, fileSize);
        root->children.push_back(newFile);
    }
}

void FileSystem::addToIndex(const string& fileName, int fileSize)
{
    ofstream indexFile("index.txt", ios::app);
    indexFile << fileName << " " << fileSize << endl;
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
