#include "t9.h"

//#define VERBOSE

using namespace std;

T9::T9(string input_file)
  : input_file_name(input_file)
{
}

void T9::print(void)
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
    out = printT9(sLine);
    ++ i;
    cout << "Case #" << i << ": " << out << endl;
  }

  infile.close();
}

string T9::reverse(string in)
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

string T9::printT9(string in)
{
  string out = "";
  vector<string> strings;
  istringstream f(in);
  string s; 
  const char *ch = in.c_str();
  st_num num;
  st_num prev_num = { '1', 0 };
  
  while (*ch != '\0') {
    num = convertAscii(*(ch++));
    out += printNumKey(num, prev_num);
    prev_num = num;
  }

  return out;
}

string T9::printNumKey(const st_num &current, const st_num &previous)
{
  string out = "";

  if (current.key == previous.key)
    out += ' ';

  for (int i=0; i<current.pressing_time; i++) {
    out += current.key;
  }

  return out;
}

st_num T9::convertAscii(const char ch)
{
  st_num num = { '1', 0 };
  int i(0);

  while (ch >= k_ascii_map[i])
    ++i;

  num.key = '0' + i;
  if (num.key == '1') {
    num.key = '0';
  }
  --i;
  num.pressing_time = ch - k_ascii_map[i] + 1;

  return num;
}

