#include "analyzer.h"

Analyzer::Analyzer(const vector<Verse> &verses) : verses(verses) {}

double Analyzer::evaluateFrequency(const string &phrase, Triad start,
                                   Triad last) {
  get<0>(start) = remove_space(get<0>(start));
  get<0>(last) = remove_space(get<0>(last));

  auto text = mergeVerses(start, last);
  auto count = match(text, phrase).size();
  auto n_words = countWords(text);

  return count * 1000. / n_words;
}
unsigned int Analyzer::countWords(const string &str) {
  stringstream ss(str);
  return distance(istream_iterator<string>(ss), istream_iterator<string>());
}
std::string Analyzer::mergeVerses(const Triad &start, const Triad &last) {
  string buffer;

  auto comparator = [](const Triad &range) {
    return [&range](const Verse v) {
      return v.book == get<0>(range) && v.chapter == get<1>(range) &&
             v.verse == get<2>(range);
    };
  };

  auto beg = find_if(verses.begin(), verses.end(), comparator(start));
  auto end = find_if(verses.begin(), verses.end(), comparator(last)) + 1;

  for (auto it = beg; it != end; ++it) {
    buffer.append(it->content);
  }

  return buffer;
}
std::vector<int> Analyzer::calculate_pi(const string &pattern) {
  vector<int> pi(pattern.size());
  pi[0] = -1;
  int j = -1;

  for (int i = 1; i < pattern.size(); ++i) {
    while (j >= 0 && pattern[i] != pattern[j + 1])
      j = pi[j];

    if (pattern[i] == pattern[j + 1])
      pi[i] = ++j;
    else
      pi[i] = -1;
  }

  return pi;
}
std::vector<int> Analyzer::match(string text, string pattern) {
  vector<int> positions;
  if (pattern.empty())
    return positions;

  auto pi = calculate_pi(pattern);

  int j = -1;
  for (int i = 0; i < text.size(); ++i) {
    while (j >= 0 && text[i] != pattern[j + 1])
      j = pi[j];

    if (text[i] == pattern[j + 1]) {
      j++;
      if (j + 1 == pattern.size()) {
        positions.push_back(i - j);
        j = pi[j];
      }
    }
  }

  return positions;
}
