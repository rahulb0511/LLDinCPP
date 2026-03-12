#include <iostream>
using namespace std;

class BST {
private:
    struct Node {
        int val;
        Node* left;
        Node* right;

        Node(int v) {
            val = v;
            left = right = nullptr;
        }
    };

    Node* root;

    // ----------- INSERT (LC 701) -----------
    Node* insertNode(Node* node, int val) {
        if (!node) return new Node(val);

        if (val < node->val)
            node->left = insertNode(node->left, val);
        else
            node->right = insertNode(node->right, val);

        return node;
    }

    // ----------- SEARCH -----------
    bool searchNode(Node* node, int key) {
        if (!node) return false;
        if (node->val == key) return true;

        if (key < node->val)
            return searchNode(node->left, key);
        else
            return searchNode(node->right, key);
    }

    // helper: find min in right subtree
    Node* findMin(Node* node) {
        while (node->left)
            node = node->left;
        return node;
    }

    // ----------- DELETE (LC 450) -----------
    Node* deleteNode(Node* node, int key) {
        if (!node) return nullptr;

        if (key < node->val) {
            node->left = deleteNode(node->left, key);
        }
        else if (key > node->val) {
            node->right = deleteNode(node->right, key);
        }
        else {
            // case 1: no child
            if (!node->left && !node->right) {
                delete node;
                return nullptr;
            }
            // case 2: one child
            else if (!node->left) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            // case 3: two children
            else {
                Node* successor = findMin(node->right);
                node->val = successor->val;
                node->right = deleteNode(node->right, successor->val);
            }
        }

        return node;
    }

public:
    BST() {
        root = nullptr;
    }

    void insert(int val) {
        root = insertNode(root, val);
    }

    bool search(int key) {
        return searchNode(root, key);
    }

    void deleteKey(int key) {
        root = deleteNode(root, key);
    }
};


int main() {
    
}