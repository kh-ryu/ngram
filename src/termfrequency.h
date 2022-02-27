#pragma once

#include <numeric>
#include <cmath>
#include <bits/stdc++.h>
#include <algorithm>

#include "analyzer.h"
#include "argument_parser.h"
#include "trie.h"

using namespace std;

class TermFrequency {
    vector<Verse> verses;
    Analyzer analyzer;

public:
    TermFrequency(const vector<Verse> &verse);

    // Find terms those are evenly or unevenly distributed
    void findTerms();

private:
    // return the list of Books and their last chapter, verse
    vector<Triad> getBookLengthList();
    
    // return words and phrases(less than 3 words) can be found in a verse
    vector<string> getWordsAndPhrases(const string &content);

    // divide the content of a verse into words
    vector<string> divideIntoWords(const string &content);

    vector<pair<string, double>> getWordWithVarList(const vector<Triad> &BookLengthList);

    vector<pair<string, double>> getFreqRSD(const vector<string> &buffer, const vector<Triad> &BookLengthList);

    string normalize(const string &raw);

    template <typename T> bool contains(const vector<T> &v, const T &item) {
    return find(v.begin(), v.end(), item) != v.end();
    }
};

bool cmp(const pair<string,double> &a, const pair<string,double> &b);
