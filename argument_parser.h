#pragma once

#include <cstdlib>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include "analyzer.h"

using namespace std;

struct Argument {
  Triad start;
  Triad last;
  string phrase;
};

class ArgumentParser {
  vector<string> args;

public:
  ArgumentParser(int argc, const char *argv[]);
  Argument parse();
  void printHelpAndExit();

private:
  bool contains(const string &str);

  Triad parseRange(const string &str);
};
