#pragma once

#include "txt_parser.h"
#include "trie.h"

using namespace std;


using Triad = tuple<string, int, int>;
class Analyzer {
	vector<Verse> verses;

public:
  Analyzer();

  Analyzer(const vector<Verse> &verses);

  double evaluateFrequency(const string &phrase, Triad start, Triad last);

  // use Aho-Corasick Algorithm for finding multiple strings
  map<string, double> evaluateFrequency(const vector<string> &phrases, Triad start, Triad last);

  string mergeVerses(const Triad &start, const Triad &last);

private:
  unsigned int countWords(const string &str);

  map<string, int> aho_corasick(const string &text, const vector<string> &words);

  string normalize(const string &raw);
};
