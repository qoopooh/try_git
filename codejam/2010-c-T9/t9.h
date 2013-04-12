#ifndef T9_H_
#define T9_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

typedef struct {
  char key;           /* '0' - '9' */
  int pressing_time;  /* 1 - 4 */
} st_num;

const char k_ascii_map[] = { ' ', 'a', 'd', 'g', 'j', 'm', 'p', 't', 'w', '{' };

class T9 {
public:
  T9(std::string input_file);
  void print(void);

protected:
  std::string reverse(std::string);
  std::string printT9(std::string);
  std::string printNumKey(const st_num &current, const st_num &previous);
  st_num convertAscii(const char ch);
private:
  std::string input_file_name;
};

#endif /* T9_H_ */

