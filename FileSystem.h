#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <queue>
#include <stack>

class FileSystem {
private:
    class FileNode {
    public:
        std::string name;
        bool isDirectory;
        FileNode* next;
        FileNode(const std::string& n, bool isDir);
    };

    class DirectoryNode {
    public:
        std::string name;
        FileNode* files;
        DirectoryNode* next;
        DirectoryNode(const std::string& n);
    };

    class FileMoveOperation {
    public:
        std::string sourceDirectory;
        std::string destinationDirectory;
        std::string filename;
        bool isDirectory;
        FileMoveOperation(const std::string& srcDir, const std::string& destDir, const std::string& fname, bool isDir);
    };

    class BSTNode {
    public:
        std::string key;
        BSTNode* left;
        BSTNode* right;
        BSTNode(const std::string& k) : key(k), left(nullptr), right(nullptr) {}
    };

    DirectoryNode* root;
    BSTNode* bstRoot;
    std::queue<FileMoveOperation> moveQueue;
    std::stack<FileSystem*> backups; // Declaration of backups stack

    // Private helper functions for directory and file management...
    DirectoryNode* findDirectory(const std::string& dirname) const;
    FileNode* findFile(DirectoryNode* dir, const std::string& filename) const;
    void insertFileIntoDirectory(DirectoryNode* dir, FileNode* fileToInsert);
    void insertIntoBST(BSTNode*& root, const std::string& key);
    bool searchBST(BSTNode* root, const std::string& key) const;
    void deleteBST(BSTNode* root);


public:
    FileSystem();
    ~FileSystem();

    // Functions for directory operations...
    void insertDirectory(const std::string& dirname);
    void displayDirectoryStructure() const;

    // Functions for file operations...
    void insertFile(const std::string& dirname, const std::string& filename, bool isDir);
    bool search(const std::string& filename) const;
    void remove(const std::string& filename);

    // Function to enqueue move operation
    void enqueueMove(const std::string& sourceDir, const std::string& destDir, const std::string& filename, bool isDir);

    // Function to process move queue
    void processMoveQueue();
    
    // Function to create a backup of the file system
    void createBackup();

    // Function to restore the most recent backup
    void restoreBackup();
    
    // Function to display contents of a specified directory
    void displayDirectoryContents(const std::string& dirname) const;
    
    // Function to rename the directory
    void renameDirectory(const std::string& oldName, const std::string& newName);
    
    // Function to copy a file
    void copyFile();

    // Function to sort files in a specified directory by alphabetical order
    void sortFilesInDirectory();
    
    // Function to sort files in a specified directory by alphabetical order using Quicksort
    void quickSortFiles(FileNode*& head);


};

#endif // FILESYSTEM_H
