#include "storecredit.h"

int main (int argc, char* argv[])
{
  if (argc != 2) {
    std::cout << "Please type:\n\t" << argv[0] << " {file name}" << std::endl; //help
    return 1;
  }
  
  StoreCredit cs = StoreCredit(argv[1]);
  cs.print();

  return 0;
}
