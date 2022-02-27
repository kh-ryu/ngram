#include "argument_parser.h"

ArgumentParser::ArgumentParser(int argc, const char *argv[]) {
  for (const char **it = argv; *it; it++) {
    args.push_back(*it);
  }
}
Argument ArgumentParser::parse() {
  if (contains("-h") || contains("--help")) {
    printHelpAndExit();
  } else if (args.size() != 5) {
    cout << "Input Error!" << endl;
    printHelpAndExit(100);
  }

  Argument argument;
  argument.path = args[1];
  try {
    argument.start = parseRange(args[2]);
    argument.last = parseRange(args[3]);
  } catch (const regex_error &err) {
    cerr << "Error occurred while parsing ranges" << endl
         << "Reason: " << err.what() << endl;
    exit(10);
  }
  argument.phrase = args[4];
  cout << "start chapter" << get<0>(argument.start) << endl;

  return argument;
}
void ArgumentParser::printHelpAndExit(int exitcode) {
  cout << "Usage: " << args[0]
       << " <file path> <start range> <end range> <word or phrase>\n"
       << "\trange: [start, end]\n"
       << "\trange format: 'book name,chapter,verse'\n";

  exit(exitcode);
}
bool ArgumentParser::contains(const string &str) {
  return find(args.begin(), args.end(), str) != args.end();
}
Triad ArgumentParser::parseRange(const string &str) {
  regex pattern("([\\w\\s:]*),(\\d+),(\\d+)");
  sregex_iterator iter(str.begin(), str.end(), pattern);

  return {iter->str(1), stoi(iter->str(2)), stoi(iter->str(3))};
}
