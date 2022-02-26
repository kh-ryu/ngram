#pragma once
#include <iostream>
#include <vector>
#include <queue>

#define ALPHABET_SIZE 27 // 26 alphabets + space

using namespace std;

class Node {
public:
    bool isEnd;
    int wordcount;
    Node* container[ALPHABET_SIZE];
    Node* fail;
    Node();
    void addcount();
};

class Trie {
private:
    Node* root;
public:
    Trie();

    void insert(const string &target);

    bool search(const string &target);

    Node* getroot();

    void fail();

    int getWordcount(const string &target);
};