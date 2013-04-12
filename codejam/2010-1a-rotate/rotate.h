#ifndef ROTATE_H
#define ROTATE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

typedef enum {
  up, right_up,
  dir_right, right_down,
  down, left_down,
  left, left_up,
  none
} direction;

typedef struct {
  int x;
  int y;
} st_position;

typedef struct {
  st_position next_pos;
  direction dir;
  int count;
} st_trace_count;

class Rotate {
  public:
    Rotate(std::string);
    void print();

  protected:
    void printTable(std::vector<std::string> &);
    void rotate(std::vector<std::string> &);
    bool havePieces(const unsigned int p, const char ch,
        std::vector<std::string> &);
    const unsigned int countPieces(const unsigned int,
        std::vector<st_position> &);
    void setNexPositions(const st_position, std::vector<st_trace_count> &);
    const int countPosition(const st_position, std::vector<st_trace_count> &);

  private:
    void printTrace(std::vector<st_position> &);
    std::string input_file_name;
};

#endif /* ROTATE_H */

