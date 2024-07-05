#include "FileSystem.h"
#include <iostream>

// Implementation of FileNode constructor
FileSystem::FileNode::FileNode(const std::string& n, bool isDir) : name(n), isDirectory(isDir), next(nullptr) {}

// Implementation of DirectoryNode constructor
FileSystem::DirectoryNode::DirectoryNode(const std::string& n) : name(n), files(nullptr), next(nullptr) {}

// Implementation of FileMoveOperation constructor
FileSystem::FileMoveOperation::FileMoveOperation(const std::string& srcDir, const std::string& destDir, const std::string& fname, bool isDir)
    : sourceDirectory(srcDir), destinationDirectory(destDir), filename(fname), isDirectory(isDir) {}

// Implementation of FileSystem constructor
FileSystem::FileSystem() : root(nullptr), bstRoot(nullptr) {}

// Implementation of FileSystem destructor
FileSystem::~FileSystem() {
    // Start by deleting all files within directories
    DirectoryNode* currentDir = root;
    while (currentDir != nullptr) {
        FileNode* currentFile = currentDir->files;
        while (currentFile != nullptr) {
            FileNode* nextFile = currentFile->next;
            delete currentFile;
            currentFile = nextFile;
        }
        // After deleting files in the directory, delete the directory node itself
        DirectoryNode* nextDir = currentDir->next;
        delete currentDir;
        currentDir = nextDir;
    }

    // Delete the BST nodes
    deleteBST(bstRoot);
}

// Private helper function to delete BST nodes recursively
void FileSystem::deleteBST(BSTNode* root) {
    if (root == nullptr) {
        return;
    }
    deleteBST(root->left);
    deleteBST(root->right);
    delete root;
}

// Private helper function to find a directory by name
FileSystem::DirectoryNode* FileSystem::findDirectory(const std::string& dirname) const {
    DirectoryNode* temp = root;
    while (temp != nullptr) {
        if (temp->name == dirname) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}

// Private helper function to find a file in a directory
FileSystem::FileNode* FileSystem::findFile(DirectoryNode* dir, const std::string& filename) const {
    FileNode* temp = dir->files;
    while (temp != nullptr) {
        if (temp->name == filename) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}

// Private helper function to insert a file into a directory
void FileSystem::insertFileIntoDirectory(DirectoryNode* dir, FileNode* fileToInsert) {
    if (!dir || !fileToInsert) {
        return;
    }
    
    // If the directory has no files yet, insert the file as the first file
    if (!dir->files) {
        dir->files = fileToInsert;
        return;
    }
    
    // Otherwise, find the last file in the directory and insert the new file after it
    FileNode* temp = dir->files;
    while (temp->next) {
        temp = temp->next;
    }
    temp->next = fileToInsert;
}

// Private helper function to insert a file into BST
void FileSystem::insertIntoBST(BSTNode*& root, const std::string& key) {
    if (root == nullptr) {
        root = new BSTNode(key);
    } else if (key < root->key) {
        insertIntoBST(root->left, key);
    } else {
        insertIntoBST(root->right, key);
    }
}

// Private helper function to search for a file in BST
bool FileSystem::searchBST(BSTNode* root, const std::string& key) const {
    if (root == nullptr) {
        return false;
    }

    if (root->key == key) {
        return true;
    } else if (key < root->key) {
        return searchBST(root->left, key);
    } else {
        return searchBST(root->right, key);
    }
}

// Function to insert a directory into the file system
void FileSystem::insertDirectory(const std::string& dirname) {
    // Check if directory with the same name already exists
    if (findDirectory(dirname) != nullptr) {
        std::cout << "Error: Directory '" << dirname << "' already exists." << std::endl;
        return;
    }

    // If directory does not exist, proceed with insertion
    if (root == nullptr) {
        root = new DirectoryNode(dirname);
    } else {
        DirectoryNode* temp = root;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = new DirectoryNode(dirname);
    }
}

// Function to insert a file into the file system
void FileSystem::insertFile(const std::string& dirname, const std::string& filename, bool isDir) {
    // Find the directory where the file is to be inserted
    DirectoryNode* dir = findDirectory(dirname);
    if (dir == nullptr) {
        std::cout << "Error: Directory '" << dirname << "' not found." << std::endl;
        return;
    }

    // Check if file with the same name already exists in the directory
    if (findFile(dir, filename) != nullptr) {
        std::cout << "Error: File '" << filename << "' already exists in directory '" << dirname << "'." << std::endl;
        return;
    }

    // If file does not exist, proceed with insertion
    if (dir->files == nullptr) {
        dir->files = new FileNode(filename, isDir);
    } else {
        FileNode* temp = dir->files;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = new FileNode(filename, isDir);
    }

    // Insert the file into the binary search tree
    insertIntoBST(bstRoot, filename);
}

// Function to search for a file in the file system using BST
bool FileSystem::search(const std::string& filename) const {
    return searchBST(bstRoot, filename);
}

// Function to display the directory structure
void FileSystem::displayDirectoryStructure() const {
    DirectoryNode* tempDir = root;
    while (tempDir != nullptr) {
        std::cout << "Directory: " << tempDir->name << std::endl;
        FileNode* tempFile = tempDir->files;
        while (tempFile != nullptr) {
            std::cout << "- " << tempFile->name << (tempFile->isDirectory ? " (Directory)" : " (File)") << std::endl;
            tempFile = tempFile->next;
        }
        tempDir = tempDir->next;
    }
}
void FileSystem::createBackup() {
    // Create a new FileSystem object to store the backup
    FileSystem* backup = new FileSystem();
    
    // Copy the entire file system to the backup
    // Traverse the current file system and insert directories and files into the backup
    DirectoryNode* currentDir = root;
    while (currentDir != nullptr) {
        backup->insertDirectory(currentDir->name); // Insert directory into the backup
        
        // Insert files from the current directory into the backup
        FileNode* currentFile = currentDir->files;
        while (currentFile != nullptr) {
            backup->insertFile(currentDir->name, currentFile->name, currentFile->isDirectory);
            currentFile = currentFile->next;
        }
        
        currentDir = currentDir->next;
    }
    
    // Push the backup onto the stack
    backups.push(backup);
}

void FileSystem::restoreBackup() {
    if (!backups.empty()) {
        // Get the most recent backup
        FileSystem* backup = backups.top();
        
        // Clear the current file system
        DirectoryNode* currentDir = root;
        while (currentDir != nullptr) {
            DirectoryNode* nextDir = currentDir->next;
            delete currentDir;
            currentDir = nextDir;
        }
        root = nullptr;
        
        // Copy the backup file system to the current file system
        DirectoryNode* backupDir = backup->root;
        while (backupDir != nullptr) {
            insertDirectory(backupDir->name); // Insert directory into the current file system
            
            // Insert files from the backup directory into the current file system
            FileNode* backupFile = backupDir->files;
            while (backupFile != nullptr) {
                insertFile(backupDir->name, backupFile->name, backupFile->isDirectory);
                backupFile = backupFile->next;
            }
            
            backupDir = backupDir->next;
        }
        
        // Pop the backup from the stack and delete it
        backups.pop();
        delete backup;
    } else {
        std::cout << "No backup available." << std::endl;
    }
}


// Function to remove a file from the file system
void FileSystem::remove(const std::string& filename) {
    DirectoryNode* tempDir = root;
    DirectoryNode* prevDir = nullptr;

    while (tempDir != nullptr) {
        FileNode* tempFile = tempDir->files;
        FileNode* prevFile = nullptr;

        while (tempFile != nullptr) {
            if (tempFile->name == filename) {
                if (prevFile != nullptr) {
                    prevFile->next = tempFile->next;
                } else {
                    tempDir->files = tempFile->next;
                }
                delete tempFile;
                return;
            }
            prevFile = tempFile;
            tempFile = tempFile->next;
        }

        prevDir = tempDir;
        tempDir = tempDir->next;
    }
}


// Function to enqueue a move operation
void FileSystem::enqueueMove(const std::string& sourceDir, const std::string& destDir, const std::string& filename, bool isDir) {
    moveQueue.push(FileMoveOperation(sourceDir, destDir, filename, isDir));
}


// Function to process the move queue
void FileSystem::processMoveQueue() {
    while (!moveQueue.empty()) {
        FileMoveOperation operation = moveQueue.front();
        moveQueue.pop();
        
        // Find the source and destination directories
        DirectoryNode* sourceDir = findDirectory(operation.sourceDirectory);
        DirectoryNode* destDir = findDirectory(operation.destinationDirectory);
        
        if (sourceDir && destDir) {
            // Find the file in the source directory
            FileNode* fileToMove = findFile(sourceDir, operation.filename);
            
            if (fileToMove) {
                // Remove the file from the source directory
                remove(fileToMove->name);
                
                // Insert the file into the destination directory
                this->insertFile(destDir->name, fileToMove->name, fileToMove->isDirectory);
                
                std::cout << "Moved file '" << operation.filename << "' from '" << operation.sourceDirectory << "' to '" << operation.destinationDirectory << "'" << std::endl;
            } else {
                std::cout << "File '" << operation.filename << "' not found in directory '" << operation.sourceDirectory << "'" << std::endl;
            }
        } else {
            std::cout << "Source or destination directory not found" << std::endl;
        }
    }
}

// Display specific directory
void FileSystem::displayDirectoryContents(const std::string& dirname) const {
    DirectoryNode* dir = findDirectory(dirname);
    if (dir) {
        if (dir->files) {
            std::cout << "Contents of directory '" << dirname << "':" << std::endl;
            FileNode* tempFile = dir->files;
            while (tempFile != nullptr) {
                std::cout << "- " << tempFile->name << (tempFile->isDirectory ? " (Directory)" : " (File)") << std::endl;
                tempFile = tempFile->next;
            }
        } else {
            std::cout << "Directory '" << dirname << "' is empty." << std::endl;
        }
    } else {
        std::cout << "Directory '" << dirname << "' not found." << std::endl;
    }
}

//Rename Directory
void FileSystem::renameDirectory(const std::string& oldName, const std::string& newName) {
    // Check if the directory with the old name exists
    DirectoryNode* oldDir = findDirectory(oldName);
    if (!oldDir) {
        std::cout << "Error: Directory '" << oldName << "' not found." << std::endl;
        return;
    }

    // Check if a directory with the new name already exists
    if (findDirectory(newName)) {
        std::cout << "Error: Directory '" << newName << "' already exists." << std::endl;
        return;
    }

    // Update the name of the directory node
    oldDir->name = newName;
    std::cout << "Directory '" << oldName << "' renamed to '" << newName << "'." << std::endl;
}

// Function to copy a file
void FileSystem::copyFile() {
    std::string filename;
    std::cout << "Enter the name of the file you want to copy: ";
    std::cin >> filename;

    // Find the file in the file system
    FileNode* fileToCopy = nullptr;
    DirectoryNode* currentDir = root;
    while (currentDir != nullptr) {
        fileToCopy = findFile(currentDir, filename);
        if (fileToCopy != nullptr) {
            break;
        }
        currentDir = currentDir->next;
    }

    if (fileToCopy != nullptr) {
        std::string destinationDir;
        std::cout << "Enter the name of the directory you want to paste the file into: ";
        std::cin >> destinationDir;

        // Find the destination directory
        DirectoryNode* destDir = findDirectory(destinationDir);
        if (destDir != nullptr) {
            // Check if a file with the copied name already exists in the destination directory
            std::string copiedFilename = "copy_" + filename;
            FileNode* existingFile = findFile(destDir, copiedFilename);
            int copyCount = 1;
            while (existingFile != nullptr) {
                // If a file with the copied name already exists, update the copied filename
                copiedFilename = "copy_" + std::to_string(copyCount) + "_" + filename;
                existingFile = findFile(destDir, copiedFilename);
                copyCount++;
            }

            // Create a copy of the file
            FileNode* copiedFile = new FileNode(copiedFilename, fileToCopy->isDirectory);

            // Insert the copied file into the destination directory
            insertFileIntoDirectory(destDir, copiedFile);
            std::cout << "File copied successfully." << std::endl;
        } else {
            std::cout << "Error: Destination directory '" << destinationDir << "' not found." << std::endl;
        }
    } else {
        std::cout << "Error: File '" << filename << "' not found." << std::endl;
    }
}

// Function to sort files in a specified directory by alphabetical order
void FileSystem::sortFilesInDirectory() {
    std::string dirname;
    std::cout << "Enter the name of the directory you want to sort: ";
    std::cin >> dirname;

    // Find the directory in the file system
    DirectoryNode* dir = findDirectory(dirname);
    if (dir) {
        // Sort the files within the directory alphabetically using Quicksort
        quickSortFiles(dir->files);

        std::cout << "Files in directory '" << dirname << "' sorted successfully." << std::endl;
    } else {
        std::cout << "Error: Directory '" << dirname << "' not found." << std::endl;
    }
}

// Quicksort implementation for sorting files
void FileSystem::quickSortFiles(FileNode*& head) {
    if (!head || !head->next) {
        return;
    }

    // Partition the list
    FileNode* pivot = head;
    FileNode* current = head->next;
    FileNode* smallerHead = nullptr;
    FileNode* smallerTail = nullptr;
    FileNode* largerHead = nullptr;
    FileNode* largerTail = nullptr;

    while (current) {
        FileNode* next = current->next;
        if (current->name < pivot->name) {
            if (!smallerHead) {
                smallerHead = current;
                smallerTail = current;
            } else {
                smallerTail->next = current;
                smallerTail = current;
            }
        } else {
            if (!largerHead) {
                largerHead = current;
                largerTail = current;
            } else {
                largerTail->next = current;
                largerTail = current;
            }
        }
        current = next;
    }

    // Concatenate smaller and larger partitions
    if (smallerTail) {
        smallerTail->next = nullptr;
        quickSortFiles(smallerHead);
        head = smallerHead;
        while (smallerTail->next) {
            smallerTail = smallerTail->next;
        }
        smallerTail->next = pivot;
    } else {
        head = pivot;
    }
    pivot->next = nullptr;
    quickSortFiles(largerHead);
    pivot->next = largerHead;
}