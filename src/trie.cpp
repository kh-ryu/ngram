#include "trie.h"

Node::Node (){
    for (int idx = 0; idx < ALPHABET_SIZE; idx++) {
        this->container[idx] = nullptr;
    }
    wordcount = 0;
    fail = nullptr;
}

void Node::addcount(){
    wordcount += 1;
}

Trie::Trie() {
    this->root = new Node();
}

void Trie::insert(const std::string &target) {
    Node* cursor = this->root;
    for (char element : target) {
        int ch;
        if (element == ' ') ch = 26;
        else ch = element - 'a';
        if (!cursor->container[ch]) {
            cursor->container[ch] = new Node();
        }
        cursor = cursor->container[ch];
    }
    cursor->isEnd = true;
}

bool Trie::search(const std::string &target) {
    bool result = true;
    Node* cursor = this->root;
    for (char element : target) {
        int ch;
        if (element == ' ') ch = 26;
        else ch = element - 'a';
        if (!cursor->container[ch]) {
            result = false;
            break;
        }
        cursor = cursor->container[ch];
    }
    if (!cursor->isEnd) {
        result = false;
    }

    return result;
}

void Trie::fail() {
    queue<Node*> q;
    root->fail = root;

    q.push(root);

    while (!q.empty()) {
        Node* cursor = q.front();
        q.pop();

        for(int i=0; i<27; i++) {
            Node *next = cursor->container[i];
            if(next == nullptr) continue;

            if(cursor == root)
                next->fail = root;
            else {
                Node *failure = cursor->fail;
                while(failure != root && failure->container[i] == nullptr)
                    failure = failure->fail;
                if(failure->container[i] != nullptr) 
                    failure = failure->container[i];
                next->fail = failure;
            }
            if(next->fail->isEnd) next->isEnd = true;

            q.push(next);
        }
    }
}

Node* Trie::getroot(){
    return root;
}

int Trie::getWordcount(const string &target) {
    int result = 0;
    Node* cursor = this->root;
    for (char element : target) {
        int ch;
        if (element == ' ') ch = 26;
        else ch = element - 'a';
        if (!cursor->container[ch]) {
            result = 0;
            break;
        }
        cursor = cursor->container[ch];
    }
    if (cursor->isEnd) {
        result = cursor->wordcount;
    }

    return result;
}