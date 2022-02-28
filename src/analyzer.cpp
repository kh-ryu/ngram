#include "analyzer.h"

Analyzer::Analyzer() {}

Analyzer::Analyzer(const vector<Verse> &verses) : verses(verses) {}

double Analyzer::evaluateFrequency(const string &phrase, Triad start,
                                   Triad last) {
	vector<string> phrases { phrase };
	return evaluateFrequency(phrases, start, last).at(phrase);
}

map<string, double> Analyzer::evaluateFrequency(const vector<string> &phrases, Triad start,
                                   Triad last) {                                  
  get<0>(start) = remove_space(get<0>(start));
  get<0>(last) = remove_space(get<0>(last));

  auto text = normalize(mergeVerses(start, last));
  auto count = aho_corasick(text, phrases);
  auto n_words = countWords(text);

  map<string, double> result;

  for (auto word : count){
    auto num = word.second;
    result.insert({word.first, num * 1000. / n_words});
  }
  return result;
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
  auto end = find_if(verses.begin(), verses.end(), comparator(last));
  if (beg == verses.end() || end == verses.end()){
    cout << "Range input Error! Please check existence of Book, Chapter, Verse in the Bible" << endl;
    exit(100);
  }
  end = end + 1;

  for (auto it = beg; it != end; ++it) {
    buffer.append(it->content);
  }

  return buffer;
}

map<string, int> Analyzer::aho_corasick(const string &text, const vector<string> &words) {
  Trie* trie = new Trie();
  auto root = trie->getroot();
  auto cursor = root;

  map<string, int> wordcount;
  vector<string> spacedWords;
  for (auto it : words){
    spacedWords.push_back(" " + it + " ");
  }

  for (auto it : spacedWords) {
    trie->insert(it);
  }

  trie->fail();

  for (int i =0; i < text.size(); i++) {
    int idx;
    if (text[i] == ' ')
      idx = 26;
    else
      idx = text[i] - 'a';

    while (cursor != root && cursor->container[idx] == nullptr) {
      cursor = cursor->fail;
    }

    if (cursor->container[idx] != nullptr)
      cursor = cursor->container[idx];
    if (cursor->isEnd){
      cursor->addcount();
    }
  }

  for (auto it : spacedWords) {
    int num = trie->getWordcount(it);
    auto restoredWord = it;
    restoredWord.erase(restoredWord.begin());
    restoredWord.erase(restoredWord.end()-1);
    wordcount.insert({restoredWord, num});
  }
  delete trie;
  return wordcount;
}

string Analyzer::normalize(const string &raw) {
    string word = raw;

    // uncapitalize
    transform(word.begin(), word.end(), word.begin(), ::tolower);

    for(int i = 0, len = word.size(); i < len; i++) {
        if ((word[i] < 'a' || word[i] > 'z') && word[i] != ' ') {
            word.erase(i--, 1);
            len = word.size();
        }
    }

    return word;
}
