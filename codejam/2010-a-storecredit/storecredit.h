#ifndef STORE_CREDIT_H
#define STORE_CREDIT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

typedef struct {
  int index;
  /*bool available;*/
  int price;
  /*int rank;*/
} st_item;

class StoreCredit {
  public:
    StoreCredit(std::string);
    void print();

  protected:
    std::string printList(const int, std::vector<st_item> &);
    std::vector<int> getPricsList(std::string);
    std::vector<st_item> getItemList(const int, const std::vector<int> &);
    std::vector<st_item> rankItems(std::vector<st_item> &);
    int minItemsIndex(const int, std::vector<st_item> &);
    int maxItemsIndex(const int, std::vector<st_item> &);

  private:
    std::string input_file_name;
};

#endif /* STORE_CREDIT_H */

