#include <iostream>
#include <vector>
#include <string>

using namespace std;

class FileSystemItem {
public:
    virtual void ls() = 0;
    virtual void openAll() = 0;
    virtual int getSize() = 0;
    virtual FileSystemItem* cd(string name) = 0;
    virtual string getName() = 0;
    virtual bool isFolder() = 0;
};

class File : public FileSystemItem {
    string name;
    int size;

public:
    File(string n, int s) {
        name = n;
        size = s;
    }

    void ls() {
        cout << name << endl;
    }

    void openAll() {
        cout << name << endl;
    }

    int getSize() {
        return size;
    }

    FileSystemItem* cd(string name) {
        return NULL;
    }

    string getName() {
        return name;
    }

    bool isFolder() {
        return false;
    }
};

class Folder : public FileSystemItem {
    string name;
    vector<FileSystemItem*> children;

public:
    Folder(string n) {
        name = n;
    }

    void add(FileSystemItem* item) {
        children.push_back(item);
    }

    void ls() {
        for(auto child : children) {
            if(child->isFolder()) {
                cout << "+ " << child->getName() << endl;
            } else {
                cout << child->getName() << endl;
            }
        }
    }

    void openAll() {
        cout << "+ " << name << endl;

        for(auto child : children) {
            child->openAll();
        }
    }

    int getSize() {
        int total = 0;

        for(auto child : children) {
            total += child->getSize();
        }

        return total;
    }

    FileSystemItem* cd(string target) {
        for(auto child : children) {
            if(child->isFolder() && child->getName() == target) {
                return child;
            }
        }

        return NULL;
    }

    string getName() {
        return name;
    }

    bool isFolder() {
        return true;
    }
};

int main() {

    Folder* root = new Folder("root");

    root->add(new File("file1.txt",1));
    root->add(new File("file2.txt",1));

    Folder* docs = new Folder("docs");
    docs->add(new File("resume.pdf",1));
    docs->add(new File("notes.txt",1));

    root->add(docs);

    Folder* images = new Folder("images");
    images->add(new File("photo.jpg",1));

    root->add(images);

    root->ls();

    docs->ls();

    root->openAll();

    FileSystemItem* cwd = root->cd("docs");

    if(cwd != NULL) {
        cwd->ls();
    }

    cout << root->getSize();
}