#include <iostream>
#include <ostream>

#include "analyzer.h"
#include "argument_parser.h"
#include "txt_parser.h"

using namespace std;

int main(int argc, const char *argv[]) {
  auto args = ArgumentParser(argc, argv).parse();

  auto verses = TxtParser("input/pg10.txt").getVerses();

  auto analyzer = Analyzer(verses);

  Triad start = {"Ezra", 1, 1}, last = {"Malachi", 4, 5};

  string phrase = "I will";

  cout << "phrase: " << phrase << endl
       << "frequency: "
       << analyzer.evaluateFrequency(args.phrase, args.start, args.last)
       << endl;
}
