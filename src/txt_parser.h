#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <ostream>
#include <regex>
#include <string>
#include <vector>
using namespace std;

#define until(expr) while (!(expr))

struct Verse {
  string book;
  int chapter;
  int verse;
  string content;

  Verse(string book, int chapter, int verse, string content) {
    this->book = book;
    this->chapter = chapter;
    this->verse = verse;
    this->content = content;
  }
};

ostream &operator<<(ostream &os, const Verse &item);

string remove_space(string str);

template <typename T> ostream &operator<<(ostream &os, const vector<T> &v) {
  os << '[';
  for (auto it = v.begin(); it != v.end(); ++it) {
    os << *it;
    if (it + 1 != v.end()) {
      os << ", ";
    }
  }
  return os << ']';
}

class TxtParser {
  vector<Verse> verse_list;
  ifstream ifs;

public:
  TxtParser(const string &filename);

  vector<Verse> getVerses();

private:
  void skipPrologue();

  /** Returns a line which does not hold CR(\r). */
  string getLine();

  vector<string> getBookList();

  template <typename T> bool contains(const vector<T> &v, const T &item) {
    return find(v.begin(), v.end(), item) != v.end();
  }

  vector<pair<string, string>>
  splitByBook(const vector<string> &book_name_list);

  vector<Verse> parseVerses(const string &book, const string &body);
};
