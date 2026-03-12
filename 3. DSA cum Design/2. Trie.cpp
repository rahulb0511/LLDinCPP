#include <bits/stdc++.h>
#include <iostream>
using namespace std;

class TrieNode {
public:
    char data;
    vector<TrieNode*> children;
    bool isTerminal;


    TrieNode(char data) {
        this->data = data;
        children.resize(26, nullptr);
        isTerminal = false;
    }
};


class Trie {
public:
    TrieNode* root;

    Trie() {
        this -> root = new TrieNode('\0');
    }

    void insert(string word) {
        TrieNode* temp = root;
        for(int i=0; i<word.size(); i++) {
            if(temp->children[word[i]-'a']==nullptr) {
                TrieNode* node = new TrieNode(word[i]);
                temp->children[word[i]-'a'] = node;
            }
            temp = temp->children[word[i]-'a'];
        }
        temp->isTerminal = true;
    }

    void remove(string word) {
        TrieNode* temp = root;
        for(int i=0; i<word.size(); i++) {
            if(temp->children[word[i]-'a']==nullptr) {
                return;
            }
            temp = temp->children[word[i]-'a'];
        }
        temp->isTerminal = false;
    }

    bool startsWith(string word) {
        TrieNode* temp = root;
        for(int i=0; i<word.size(); i++) {
            if(temp->children[word[i]-'a']==nullptr) {
                return false;
            }
            temp = temp->children[word[i]-'a'];
        }
        return true;
    }

    bool search(string word) {
        TrieNode* temp = root;
        for(int i=0; i<word.size(); i++) {
            if(temp->children[word[i]-'a']==nullptr) {
                return false;
            }
            temp = temp->children[word[i]-'a'];
        }
        return temp->isTerminal == true;
    }

};


int main() {

    Trie trie;
    trie.insert("rahul");
    trie.insert("bharti");
    cout<< trie.startsWith("rah") << " ";
    cout<< trie.search("kumar") << " ";
    cout<< trie.search("bharti") << " ";
    return 0;
}