// using https://github.com/yhirose/cpp-linenoise readline library
#include <cli.hpp>
#include <cpp-linenoise/linenoise.hpp>
#include <extra.hpp>
#include <iostream>
#include <string>

// int main(int argc, char* argv[]) {
int main(int argc, char* argv[]) {
  linenoise::SetCompletionCallback([](const char* editBuffer, std::vector<std::string>& completions) {
    switch (editBuffer[0]) {
      case 'h':
        completions.push_back("help");
        break;
      case 's':
        completions.push_back("save");
        completions.push_back("sort");
        completions.push_back("status");
        completions.push_back("select");
        completions.push_back("sselect");
        break;
      case 'l':
        completions.push_back("ld");
        completions.push_back("load");
        completions.push_back("ls");
        break;
      case 'r':
        completions.push_back("rm");
        completions.push_back("rename");
        break;
      case 'e':
        completions.push_back("edit");
        completions.push_back("exit");
        break;
      case 'a':
        completions.push_back("add");
        break;
      case 'c':
        completions.push_back("create");
        break;
    }
  });

  if (argc != 2) {
    throw std::invalid_argument("error: should pass only the path to database directory");
  }

  DIR_PATH = std::string(argv[1]);
  CLI cli;

  std::string line;
  linenoise::SetMultiLine(true);
  linenoise::SetHistoryMaxLen(4);
  while (true) {
    auto quit = linenoise::Readline(Bold("db> ").c_str(), line);
    if (quit) break;

    try {
      cli.Process(Split(line, " "));
    } catch (const std::exception& e) {  // is thrown in Exit
      std::cout << Green("goodbye!") << std::endl;
      break;
    }

    linenoise::AddHistory(line.c_str());
  }

  return 0;
}
