#include "FileSystem.cpp"

int main()
{
    FileSystem fs;
    int option;
    std::string name, content;
    int size;

    do
    {
        std::cout << "\nMenu do Sistema de Arquivos:\n";
        std::cout << "Diretorio Atual:" << fs.currentDirectory->name << "\n"; //IMPLEMENTAR INTERPOLACAO
        std::cout << "1. Criar Arquivo\n";
        std::cout << "2. Deletar Arquivo\n";
        std::cout << "3. Redimensionar Arquivo\n";
        std::cout << "4. Pesquisar Arquivo\n";
        std::cout << "5. Criar Diretorio\n";
        std::cout << "6. Deletar Diretorio\n";
        std::cout << "7. Listar Conteudo do Diretorio\n";
        std::cout << "8. Navegar para Diretório\n";
        std::cout << "9. Editar Conteúdo de Arquivo\n";
        std::cout << "0. Sair\n";
        std::cout << "Escolha uma opcao: ";
        std::cin >> option;

        switch (option)
        {
        case 1:
            std::cout << "Digite o nome do arquivo: ";
            std::cin >> name;
            std::cout << "Digite o tamanho do arquivo (em bytes): ";
            std::cin >> size;
            fs.createFile(fs.currentDirectory, name, size);
            break;

        case 2:
            std::cout << "Digite o nome do arquivo a ser deletado: ";
            std::cin >> name;
            fs.deleteFile(fs.currentDirectory, name);
            break;

        case 3:
            std::cout << "Digite o nome do arquivo a ser redimensionado: ";
            std::cin >> name;
            std::cout << "Digite o novo tamanho do arquivo (em bytes): ";
            std::cin >> size;
            fs.resizeFile(fs.currentDirectory, name, size);
            break;

        case 4:
            std::cout << "Digite o nome do arquivo a ser pesquisado: ";
            std::cin >> name;
            fs.searchFile(fs.currentDirectory, name);
            break;

        case 5:
            std::cout << "Digite o nome do diretorio a ser criado: ";
            std::cin >> name;
            fs.createDirectory(fs.currentDirectory, name);
            break;

        case 6:
            std::cout << "Digite o nome do diretorio a ser deletado: ";
            std::cin >> name;
            fs.deleteDirectory(fs.currentDirectory, name);
            break;

        case 7:
            fs.listDirectory(fs.currentDirectory);
            break;

         case 8:
            std::cout << "Digite o nome do diretório (ou '..' para subir um nível): ";
            std::cin >> name;
            fs.changeDirectory(name, fs.currentDirectory);
            break;

        case 9:
            std::cout << "Digite o nome do arquivo para editar: ";
            std::cin >> name;
            std::cout << "Digite o conteúdo para adicionar: ";
            std::cin.ignore();
            std::getline(std::cin, content);
            fs.editFileContent(name, content);
            break;


        case 0:
            std::cout << "Saindo do programa.\n";
            break;

        default:
            std::cout << "Opcao invalida! Tente novamente.\n";
            break;
        }
    } while (option != 0);

    return 0;
}
