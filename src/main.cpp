#include <iostream>
#include <ostream>
#include <time.h>

#include "analyzer.h"
#include "argument_parser.h"
#include "txt_parser.h"
#include "termfrequency.h"
#include "trie.h"
#include "datawriter.h"

using namespace std;

int main(int argc, const char *argv[]) {
  cout << "Start" << endl;
  
  /* Receive range and word/phrase for searching */
  auto args = ArgumentParser(argc, argv).parse();

  /* Read txt file and parse each verses */
  auto verses = TxtParser(args.path).getVerses();

  /* Ready for searching word or phrase in the range */
  auto analyzer = Analyzer(verses);

  Triad start = args.start, last = args.last;

  string phrase = args.phrase;

  cout << "phrase: " << phrase << endl
       << "frequency: "
       << analyzer.evaluateFrequency(args.phrase, args.start, args.last) // Calculate frequency of each word or phrase
       << endl;

  string cont;
  cout << "**************************************************************" << endl;
  cout << "Do you want to search words or phrases that are evenly/unevenly distributed?" << endl;
  cout << "Please type \"yes\" if you want to search and continue" << endl;
  cin >> cont;

  pair<vector<string> ,vector<string>> words;
  if (cont == "yes"){
    int num;
    cout << "enter the number of words you want to find (less than 10)" << endl;
    cin >> num;
    cout << "Searching..." << endl;
    auto search = TermFrequency(verses);
    words = search.findTerms(num);
  }

  string data;
  cout << "***************************************************************" << endl;
  cout << "If you want to save frequencies of those words, please type \"yes\" " << endl;
  cout << "Warning: it can take quite a long time" << endl;
  cin >> data;
  if (data == "yes"){
    int fast;
    cout << "1. Save frequency per book" << endl;
    cout << "2. Save frequency per book & frequency per each chapter" << endl;
    cin >> fast;
    cout << "Saving data" << endl;
    auto t = time(nullptr);
    auto tm = localtime(&t);
    char datetime[20];
    strftime(datetime, 20, "%F %H_%M", tm);
    string head(datetime);

    auto write = DataWriter(head, verses, words);
    write.saveData(fast);
  }

  cout << "Thank you" << endl;
}
