#pragma once

#include <iostream>
#include <ostream>

#include "analyzer.h"
#include "txt_parser.h"
#include "termfrequency.h"
#include "trie.h"

using namespace std;

class DataWriter {
    string head;
    vector<Verse> verses;
    Analyzer analyzer;
    pair<vector<string>, vector<string>> words;

public:
    DataWriter(const string &givenhead, const vector<Verse> &givenverses, 
                const pair<vector<string>, vector<string>> &givenwords);

    void saveData();

private:
    void writeFreqPerBook(const vector<string> &words, string name);

    void writeAccumulatedFreq(const vector<string> &words, string name);
};