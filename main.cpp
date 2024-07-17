#include <iostream>
#include "FileSystem.h"

int main() {
    FileSystem fileSystem;

    // Insert directories
    fileSystem.insertDirectory("documents");
    fileSystem.insertDirectory("pictures");
    fileSystem.insertDirectory("music");

    // Insert files into "documents" directory
    fileSystem.insertFile("documents", "resume.docx", false);
    fileSystem.insertFile("documents", "presentation.pptx", false);

    // Insert files into "pictures" directory
    fileSystem.insertFile("pictures", "vacation.jpg", false);
    fileSystem.insertFile("pictures", "family.jpg", false);

    // Insert files into "music" directory
    fileSystem.insertFile("music", "song1.mp3", false);
    fileSystem.insertFile("music", "song2.mp3", false);

    int choice;
    std::string name;
    std::string filename;
    std::string destination;
    bool isDir;
    std::string dirName;
    std::string newName;

    do {
        std::cout << "\nFile System Menu:\n";
        std::cout << "1.  Insert Directory\n";
        std::cout << "2.  Insert File\n";
        std::cout << "3.  Search for File\n";
        std::cout << "4.  Remove File\n";
        std::cout << "5.  Move File\n";
        std::cout << "6.  Display Directory Structure\n";
        std::cout << "7.  Undo previous action\n";
        std::cout << "8.  Display contents of a specified directory\n";
        std::cout << "9.  Rename a directory\n";
        std::cout << "10. Copy a file\n";
        std::cout << "11. Sort a specified directory\n";
        std::cout << "12. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                fileSystem.createBackup();
                std::cout << "Enter directory name: ";
                std::cin >> name;
                fileSystem.insertDirectory(name);
                break;
            case 2:
                fileSystem.createBackup();
                std::cout << "Enter directory name: ";
                std::cin >> name;
                std::cout << "Enter file name: ";
                std::cin >> filename;
                isDir = false;
                fileSystem.insertFile(name, filename, isDir);
                break;
            case 3:
                fileSystem.createBackup();
                std::cout << "Enter file name to search: ";
                std::cin >> filename;
                if (fileSystem.search(filename)) {
                    std::cout << "File found: " << filename << std::endl;
                } else {
                    std::cout << "File not found: " << filename << std::endl;
                }
                break;
            case 4:
                fileSystem.createBackup();
                std::cout << "Enter file name to remove: ";
                std::cin >> filename;
                fileSystem.remove(filename);
                break;
            case 5:
                fileSystem.createBackup();
                std::cout << "Enter source directory: ";
                std::cin >> name;
                std::cout << "Enter destination directory: ";
                std::cin >> filename;
                std::cout << "Enter file name: ";
                std::cin >> filename;
                isDir = false;
                fileSystem.enqueueMove(name, destination, filename, isDir);
                break;
            case 6:
                std::cout << std::endl << "Directory structure:" << std::endl;
                fileSystem.displayDirectoryStructure();
                break;
            case 7:
                fileSystem.restoreBackup();
                std::cout << "Previous structure restored." << std::endl;
                break;
            case 8:
                std::cout << "Enter desired directory: ";
                std::cin >> dirName;
                fileSystem.displayDirectoryContents(dirName);
            case 9:
                std::cout << "Enter the directory that will be renamed: ";
                std::cin >> dirName;
                std::cout << "Enter the new name for the directory: ";
                std::cin >> newName;
                fileSystem.renameDirectory(dirName, newName);
            case 10:
                fileSystem.copyFile();
            case 11:
                fileSystem.sortFilesInDirectory();
            case 12:
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }

    } while (choice != 12);

    return 0;
}
