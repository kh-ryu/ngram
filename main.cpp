#include <iostream>
#include <ostream>

#include "txt_parser.h"

using namespace std;


int main(int argc, const char *argv[]) {
  auto verses = TxtParser("input/pg10.txt").getVerses();

  for (auto &verse : verses) {
    cout << verse << endl;
  }
}
