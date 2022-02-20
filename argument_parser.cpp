#include "argument_parser.h"

ArgumentParser::ArgumentParser(int argc, const char *argv[]) {
  for (const char **it = argv; *it; it++) {
    args.push_back(*it);
  }
}
Argument ArgumentParser::parse() {
  if (contains("-h") || contains("--help")) {
    printHelpAndExit();
  }

  Argument argument;
  argument.start = parseRange(args[1]);
  argument.last = parseRange(args[2]);
  argument.phrase = args[3];

  return argument;
}
void ArgumentParser::printHelpAndExit() {
  cout << "Usage: " << args[0]
       << " <start range> <end range> <word or phrase>\n"
       << "\trange: [start, end]\n"
       << "\trange format: 'book name,chapter,verse'\n";

  exit(0);
}
bool ArgumentParser::contains(const string &str) {
  return find(args.begin(), args.end(), str) != args.end();
}
Triad ArgumentParser::parseRange(const string &str) {
  regex pattern("(\\w+),(\\d+),(\\d+)");
  sregex_iterator iter(str.begin(), str.end(), pattern);

  return {iter->str(1), stoi(iter->str(2)), stoi(iter->str(3))};
}
