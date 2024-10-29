#include "FileSystem.h"

int main() {
    FileSystem fs;
    int option;
    std::string name;
    int size;

    do {
        std::cout << "\nMenu do Sistema de Arquivos:\n";
        std::cout << "1. Criar Arquivo\n";
        std::cout << "2. Deletar Arquivo\n";
        std::cout << "3. Redimensionar Arquivo\n";
        std::cout << "4. Pesquisar Arquivo\n";
        std::cout << "5. Criar Diretório\n";
        std::cout << "6. Deletar Diretório\n";
        std::cout << "7. Listar Conteúdo do Diretório\n";
        std::cout << "0. Sair\n";
        std::cout << "Escolha uma opção: ";
        std::cin >> option;

        switch (option) {
            case 1:
                std::cout << "Digite o nome do arquivo: ";
                std::cin >> name;
                std::cout << "Digite o tamanho do arquivo (em bytes): ";
                std::cin >> size;
                fs.createFile(fs.root, name, size);
                break;

            case 2:
                std::cout << "Digite o nome do arquivo a ser deletado: ";
                std::cin >> name;
                fs.deleteFile(fs.root, name);
                break;

            case 3:
                std::cout << "Digite o nome do arquivo a ser redimensionado: ";
                std::cin >> name;
                std::cout << "Digite o novo tamanho do arquivo (em bytes): ";
                std::cin >> size;
                fs.resizeFile(fs.root, name, size);
                break;
                
            case 4:
                std::cout << "Digite o nome do arquivo a ser pesquisado: ";
                std::cin >> name;
                fs.searchFile(fs.root, name);
                break;

            case 5:
                std::cout << "Digite o nome do diretório a ser criado: ";
                std::cin >> name;
                fs.createDirectory(fs.root, name);
                break;

            case 6:
                std::cout << "Digite o nome do diretório a ser deletado: ";
                std::cin >> name;
                fs.deleteDirectory(fs.root, name);
                break;

            case 7:
                fs.listDirectory(fs.root);
                break;

            case 0:
                std::cout << "Saindo do programa.\n";
                break;

            default:
                std::cout << "Opção inválida! Tente novamente.\n";
                break;
        }
    } while (option != 0);

    return 0;
}
