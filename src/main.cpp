#include <iostream>
#include <ostream>

#include "analyzer.h"
#include "argument_parser.h"
#include "txt_parser.h"
#include "termfrequency.h"
#include "trie.h"

using namespace std;

int main(int argc, const char *argv[]) {
  cout << "Start" << endl;
  /*
  ifstream ifs("input/curly.txt");
  string line;
  wstring line_w;
  getline(ifs, line);
  cout << line << endl;
  line_w.append(line.begin(), line.end());
  wcout << line_w << endl;
  for (int i = 0, len = line.size(); i < line_w.size(); i++) {
    if (line[i]<0) {
            line.erase(i--, 1);
            len = line.size();
        }
  }
  cout << line << endl;
  */



  /* Receive range and word/phrase for searching */
  auto args = ArgumentParser(argc, argv).parse();

  /* Read txt file and parse each verses */
  auto verses = TxtParser("input/pg10.txt").getVerses();

  /* Ready for searching word or phrase in the range */
  auto analyzer = Analyzer(verses);

  Triad start = args.start, last = args.last;

  string phrase = args.phrase;

  cout << "phrase: " << phrase << endl
       << "frequency: "
       << analyzer.evaluateFrequency(args.phrase, args.start, args.last) // Calculate frequency of each word or phrase
       << endl;

  auto search = TermFrequency(verses);
  search.findTerms();
}
