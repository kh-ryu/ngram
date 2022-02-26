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
    vector<string> WordsandPhrases(const string &content);

    // divide the content of a verse into words
    vector<string> divideIntoWords(const string &content);

    vector<pair<string, double>> getWord(const vector<Triad> &BookLengthList, Trie* trie);

    pair<double, double> freqMeanVariance(const string &word, const vector<Triad> &BookLengthList, const vector<Verse> &verses);

    vector<pair<string, double>> freqVariance(const vector<string> &buffer, const vector<Triad> &BookLengthList);

    string process(const string &raw);

    template <typename T> bool contains(const vector<T> &v, const T &item) {
    return find(v.begin(), v.end(), item) != v.end();
    }
};

bool cmp(const pair<string,double> &a, const pair<string,double> &b);