#include "t9.h"

int main (int argc, char* argv[])
{
  if (argc != 2) {
    std::cout << "Please type:\n\t" << argv[0] << " {file name}" << std::endl; //help
    return 1;
  }
  
  T9 t9 = T9(argv[1]);
  t9.print();

  return 0;
}
