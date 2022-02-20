#include <iostream>
#include <ostream>

#include "txt_parser.h"
#include "analyzer.h"

using namespace std;


int main(int argc, const char *argv[]) {
  auto verses = TxtParser("input/pg10.txt").getVerses();

  //for (auto &verse : verses) {
  //  cout << verse << endl;
  //}

	auto analyzer = Analyzer(verses);

	Triad start = {
		"Ezra", 1, 1
	}, last = {
		"Malachi", 4, 5
	};

	string phrase = "I will";

	cout << "phrase: " << phrase << endl
		<< "frequency: " << analyzer.evaluateFrequency(phrase, start, last) << endl;

}
