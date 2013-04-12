#include "rotate.h"

#define VERBOSE

using namespace std;

Rotate::Rotate(string filename)
  : input_file_name(filename)
{
}

void Rotate::print(void)
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
  while (i < n_cases)
  {
    string out;
    vector<int> nk; /* n-by-n table and k-piece */
    int ans = 0;

    getline(infile, sLine);
    istringstream f(sLine);
    string s; 
    while (std::getline(f, s, ' ')) {
      nk.push_back(atoi(s.c_str()));
    }
#ifdef VERBOSE
    cout << endl << "N-by-N: " << nk[0] << endl;
    cout << "pieces: " << nk[1] << endl;
#endif
    vector<string> table;
    for (int r=0; r<nk[0]; r++) {
      getline(infile, sLine);
      table.push_back(sLine);
    }

    //printTable(table);
    rotate(table);
    if (havePieces(static_cast<const unsigned int>(nk[1]), 'B', table)) {
      ans |= 1;
    }
    printTable(table);
    if (havePieces(static_cast<const unsigned int>(nk[1]), 'R', table)) {
      ans |= 2;
    }

    if (ans == 1) {
      out = "Blue";
    } else if (ans == 2) {
      out = "Red";
    } else if (ans == 3) {
      out = "Both";
    } else {
      out = "Neither";
    }
    ++ i;
    cout << "Case #" << i << ": " << out << endl;
  }

  infile.close();
}

void Rotate::printTable(vector<string> &table)
{
  for (vector<string>::iterator it = table.begin(); it != table.end(); ++it) {
    for (string::iterator ch = (*it).begin(); ch != (*it).end(); ++ch) {
      cout << *ch << " ";
    }
    cout << endl;
  }
}

void Rotate::rotate(vector<string> &table)
{
  vector<string> new_table;

  for (vector<string>::iterator it = table.begin(); it != table.end(); ++it) {
    string new_row;
    string pieces;

    for (string::iterator ch = (*it).begin(); ch != (*it).end(); ++ch) {
      if (*ch != '.') {
        pieces.push_back(*ch);
      } else {
        new_row.push_back(*ch);
      }
    }
    for (string::iterator ch = pieces.begin(); ch != pieces.end(); ++ch) {
      new_row.push_back(*ch);
    }
    new_table.push_back(new_row);
  }

  table = new_table;
}

bool Rotate::havePieces(const unsigned int p_num, const char k_ch,
    vector<string> &table)
{
  vector<st_position> trace;
  unsigned int row(0);

  for (vector<string>::iterator it = table.begin(); it != table.end(); ++it) {
    unsigned int col(0);

    ++row;
    for (string::iterator ch = (*it).begin(); ch != (*it).end(); ++ch) {
      ++col;
      if (*ch != k_ch)
        continue;

      st_position p = { row, col };
      trace.push_back(p);
    }
  }

  if (trace.size() < p_num)
    return false;
#ifdef VERBOSE
  printTrace(trace);
#endif
  if (countPieces(p_num, trace) < p_num)
    return false;

  return true;
}

void Rotate::printTrace(vector<st_position> &trace)
{
  for (vector<st_position>::iterator it = trace.begin(); it != trace.end(); ++it) {
    cout << "x: " << (*it).x << " y: " << (*it).y << "\t";
  }
  cout << endl;
}

const unsigned int Rotate::countPieces(const unsigned int p_num, vector<st_position> &trace)
{
  unsigned int count(0);
  unsigned int max_count(0);
  vector<st_trace_count> trace_count;

  for (unsigned int i=0; i<trace.size(); ++i) {
    setNexPositions(trace[i], trace_count);
    for (unsigned int j=(i+1); j<trace.size(); ++j) {
      count = static_cast<unsigned int>(countPosition(trace[j], trace_count));
      if (count > max_count) {
        max_count = count;
        if (count >= p_num)
          return count;
      }
    }
  }

  return max_count;
}

void Rotate::setNexPositions(const st_position pos, vector<st_trace_count> &trace_count)
{
  st_position pos_tmp = { pos.x - 1, pos.y + 1 };
  st_trace_count trace = { pos_tmp, left_down, 1 };
  trace_count.push_back(trace);
  pos_tmp.x = pos.x;
  pos_tmp.y = pos.y + 1;
  trace.next_pos = pos_tmp;
  trace.dir = down;
  trace_count.push_back(trace);
  pos_tmp.x = pos.x + 1;
  pos_tmp.y = pos.y + 1;
  trace.next_pos = pos_tmp;
  trace.dir = right_down;
  trace_count.push_back(trace);
  pos_tmp.x = pos.x + 1;
  pos_tmp.y = pos.y;
  trace.next_pos = pos_tmp;
  trace.dir = dir_right;
  trace_count.push_back(trace);
}

const int Rotate::countPosition(const st_position pos,
    vector<st_trace_count> &trace_count)
{
  int count(0);

  for (vector<st_trace_count>::iterator it = trace_count.begin();
      it != trace_count.end(); ++it) {
    st_trace_count trace = *it;
    if ((pos.x != trace.next_pos.x) || (pos.y != trace.next_pos.y))
      continue;
    direction dir = trace.dir;

    cout << "trace dir: " << trace.dir << ", x: " << trace.next_pos.x << ", y: "
      << trace.next_pos.y << ", count: " << trace.count << endl;
    ++ (*it).count;
    if ((*it).count > count)
      count = (*it).count;
    if (dir == left_down) {
      (*it).next_pos.x = pos.x - 1;
      (*it).next_pos.y = pos.y + 1;
    } else if (dir == down) {
      (*it).next_pos.x = pos.x;
      (*it).next_pos.y = pos.y + 1;
    } else if (dir == right_down) {
      (*it).next_pos.x = pos.x + 1;
      (*it).next_pos.y = pos.y + 1;
    } else if (dir == dir_right) {
      (*it).next_pos.x = pos.x + 1;
      (*it).next_pos.y = pos.y;
    }
  }

  return count;
}


