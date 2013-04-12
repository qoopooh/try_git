
// my first program in C++

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

//#define VERBOSE

using namespace std;

class ReverseWords {
public:
  ReverseWords(string input_file);
  void print(void);
protected:
  string reverse(string);
private:
  string input_file_name;
};

int main (int argc, char* argv[])
{
  if (argc != 2) {
    cout << "Please type:\n\t" << argv[0] << " {file name}" << endl; //help
    return 1;
  }
  
  string str = argv[1];

  ReverseWords rev = ReverseWords(argv[1]);
  rev.print();

  return 0;
}

ReverseWords::ReverseWords(string input_file)
  : input_file_name(input_file)
{
}

void ReverseWords::print(void)
{
  string sLine = "";
  ifstream infile;
  int n_cases = 0;
  int i = 0;

  infile.open(input_file_name.c_str());
  getline(infile, sLine);
  n_cases = atoi(sLine.c_str());

#ifdef VERBOSE
  cout << "file: " << input_file_name << endl;
  cout << "cases: " << n_cases << endl;
#endif
  //while (!infile.eof())
  while (i < n_cases)
  {
    string out;

    getline(infile, sLine);
#ifdef VERBOSE
    cout << "input: " << sLine << endl;
#endif
    out = reverse(sLine);
    ++ i;
    cout << "Case #" << i << ": " << out << endl;
  }

  infile.close();
}

string ReverseWords::reverse(string in)
{
  string out = "";
  vector<string> strings;
  istringstream f(in);
  string s; 

  while (std::getline(f, s, ' ')) {
    strings.push_back(s);
  }

  out += strings.back();
  strings.pop_back();
  while (!strings.empty()) {
    out += " " + strings.back();
    strings.pop_back();
  }

  return out;
}

