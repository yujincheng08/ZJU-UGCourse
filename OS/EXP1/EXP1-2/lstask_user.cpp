#include <fstream>
#include <iostream>
#include <string>
using namespace std;

class Logger {
private:
  ifstream in;
  string Result;
  void iter();

public:
  Logger(const string &path) : in(path) { iter(); }

  ~Logger() { in.close(); }

  void print() { cout << Result << endl; }
};

int main(int argc, char *argv[]) {
  ios::sync_with_stdio(false);
  string line;
  Logger logger("/var/log/kern.log");
  logger.print();
  return 0;
}

void Logger::iter() {
  string line;
  bool finished = false;
  while (getline(in, line)) {
    cout << line << endl;
    string::size_type n;
    if ((n = line.find("lstask")) != string::npos) {
      if (finished) {
        Result.clear();
        finished = false;
      }
      Result += line.substr(n + 8);
      Result += '\n';
      if (line.find("lstask: Good bye!") != string::npos)
        finished = true;
    }
  }
}
