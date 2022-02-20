#pragma once

#include "txt_parser.h"

using namespace std;


using Triad = tuple<string, int, int>;
class Analyzer {
	vector<Verse> verses;

public:
  Analyzer(const vector<Verse> &verses);

  double evaluateFrequency(const string &phrase, Triad start, Triad last);

private:
  unsigned int countWords(const string &str);

  string mergeVerses(const Triad &start, const Triad &last);

  vector<int> calculate_pi(const string &pattern);

  /**
   * Finds indices where the pattern exists using KMP algorithm.
   *
   * For additional requirements, consider using Aho-Corasick algorithm
   */
  vector<int> match(string text, string pattern);
};
