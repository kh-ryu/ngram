#include "trie.h"

Node::Node (){
    for (int idx = 0; idx < ALPHABET_SIZE; idx++) {
        this->container[idx] = nullptr;
    }
}

Trie::Trie() {
    this->root = new Node();
}

void Trie::insert(const std::string &target, const double &mean, const double &var) {
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
    cursor->freqMean = mean;
    cursor->freqVar = var;
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