#pragma once
#include <iostream>
#include <vector>

#define ALPHABET_SIZE 27 // 26 alphabets + space

class Node {
public:
    bool isEnd;
    double freqMean;
    double freqVar;
    Node* container[ALPHABET_SIZE];
    Node();
};

class Trie {
private:
    Node* root;
public:
    Trie();

    void insert(const std::string &target, const double &mean, const double &var);

    bool search(const std::string &target);
};