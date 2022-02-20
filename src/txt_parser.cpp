#include "txt_parser.h"

ostream &operator<<(ostream &os, const Verse &item) {
  return os << item.book << '\t' << item.chapter << '\t' << item.verse << '\t'
            << item.content;
}

TxtParser::TxtParser(const string &filename) : ifs(filename) {
  skipPrologue();

  auto book_name_list = getBookList();
  auto book_body_list = splitByBook(book_name_list);

  for (auto &[book, body] : book_body_list) {
    auto verses = parseVerses(book, body);
    verse_list.insert(verse_list.end(), verses.begin(), verses.end());
  }
}

vector<Verse> TxtParser::getVerses() { return verse_list; }

void TxtParser::skipPrologue() {
  until(getLine() ==
        "*** START OF THE PROJECT GUTENBERG EBOOK THE KING JAMES BIBLE ***") {
    ; // Do nothing
  }
}
std::string TxtParser::getLine() {
  string buffer;
  getline(ifs, buffer);
  if (!buffer.empty() && buffer.back() == '\r') {
    buffer.erase(buffer.end() - 1);
  }
  return buffer;
}
std::vector<string> TxtParser::getBookList() {
  vector<string> book_name_list;
  while (true) {
    auto old_position =
        ifs.tellg(); // Stores current position to unget one line.
    auto buffer = getLine();
    if (buffer.empty()) {
      continue;
    }
    string unspaced_buffer = remove_space(buffer);
    if (contains(book_name_list, unspaced_buffer)) {
      // book_list.push_back({no_space_buf,""});
      ifs.seekg(old_position);
      break;
    }
    book_name_list.push_back(unspaced_buffer);
  }

  return book_name_list;
}
std::string remove_space(string str) {
  auto it = remove(str.begin(), str.end(), ' ');
  str.erase(it, str.end());

  return str;
}
std::vector<pair<string, string>>
TxtParser::splitByBook(const vector<string> &book_name_list) {
  vector<pair<string, string>> book_list;

  while (true) {
    string line;
    line = getLine();
    if (line ==
        "*** END OF THE PROJECT GUTENBERG EBOOK THE KING JAMES BIBLE ***") {
      break;
    }
    string unspaced_line = remove_space(line);

    if (contains(book_name_list, unspaced_line)) {
      // new book
      book_list.push_back({unspaced_line, ""});
    } else {
      // Appends verses into the current book.
      book_list.back().second.append(line);
    }
  }
  return book_list;
}
std::vector<Verse> TxtParser::parseVerses(const string &book,
                                          const string &body) {
  vector<Verse> verses;
  regex pattern("(\\d+):(\\d+) ((.|\\n)+?)(?=(\\d+:\\d+)|$)");

  auto beg = sregex_iterator(body.begin(), body.end(), pattern);
  auto end = sregex_iterator();

  for (auto it = beg; it != end; ++it) {
    int chapter = stoi(it->str(1));
    int verse = stoi(it->str(2));
    string content = it->str(3);
	replace(content.begin(), content.end(), '\n', ' ');

    verse_list.push_back(Verse(book, chapter, verse, content));
  }
  return verses;
}
