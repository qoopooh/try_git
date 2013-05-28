#include "rotate.h"

int main (int argc, char* argv[])
{
  if (argc != 2) {
    std::cout << "Please type:\n\t" << argv[0] << " {file name}" << std::endl; //help
    return 1;
  }
  
  Rotate rt = Rotate(argv[1]);
  rt.print();

  return 0;
}
