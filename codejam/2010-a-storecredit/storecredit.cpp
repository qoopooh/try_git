#include "storecredit.h"

//#define VERBOSE

using namespace std;

StoreCredit::StoreCredit(string filename)
  : input_file_name(filename)
{
}

void StoreCredit::print(void)
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
    int credit;
    int n_items;
    vector<int> price_list;
    vector<st_item> item_list;

    getline(infile, sLine);
    credit = atoi(sLine.c_str());
    getline(infile, sLine);
    n_items = atoi(sLine.c_str());
    getline(infile, sLine);
    price_list = getPricsList(sLine);
    item_list = getItemList(credit, price_list);
    item_list = rankItems(item_list);

#ifdef VERBOSE
    cout << "input: " << credit << ", " << n_items << endl;
    for (unsigned int j=0; j<price_list.size(); j++) {
      cout << price_list[j] << ", ";
    }
    cout << endl << "input (sort): ";
    for (unsigned int j=0; j<item_list.size(); j++) {
      cout << item_list[j].index << "-" << item_list[j].price << ", ";
    }
    cout << endl;
#endif
    out = printList(credit, item_list);
    ++ i;
    cout << "Case #" << i << ": " << out << endl;
#ifdef VERBOSE
    cout << endl;
#endif
  }

  infile.close();
}

string StoreCredit::printList(const int credit, vector<st_item> &items)
{
  ostringstream out;
  int cost = 0;
  int index_price = 0;
  st_item checked_item;
  st_item paired_item = { 0, 0 };

  do {
    checked_item = items.back();
    items.pop_back();
    cost = checked_item.price;
    for (vector<st_item>::iterator it = items.begin() ; it != items.end(); it++) {
      paired_item = *it;
      index_price = paired_item.price;

      cost += index_price;
      if (cost >= credit) {
        break;
      }
      cost -= index_price;
    }
  } while (cost != credit);

  ++ checked_item.index;
  ++ paired_item.index;
  if (checked_item.index < paired_item.index) {
    out << checked_item.index << " " << paired_item.index;
  } else {
    out << paired_item.index << " " << checked_item.index;
  }

  return out.str();
}

vector<int> StoreCredit::getPricsList(string in)
{
  vector<int> prices;
  istringstream f(in);
  string s; 

  while (std::getline(f, s, ' ')) {
    int i = atoi(s.c_str());
    prices.push_back(i);
  }

  return prices;
}

vector<st_item> StoreCredit::getItemList(const int credit, const vector<int> &in)
{
  vector<st_item> items;
  string s; 

  for (unsigned int i=0; i<in.size(); i++) {
    if (in[i] > credit)
      continue;
    //st_item item = { i, true, in[i], 0 };
    st_item item = { i, in[i] };
    items.push_back(item);
  }

  return items;
}

vector<st_item> StoreCredit::rankItems(vector<st_item> &items)
{
  vector<st_item> ranked_item;
  int index = 0;
#ifdef VERBOSE
  int i_min = minItemsIndex(0, items);
  int i_max = maxItemsIndex(0, items);

  cout << "minItemsIndex: " << items[i_min].index << "-" << items[i_min].price << endl;
  cout << "maxItemsIndex: " << items[i_max].index << "-" << items[i_max].price << endl;
#endif
  while (items.size() > 0) {
    index = minItemsIndex(0, items);
    ranked_item.push_back(items[index]);
    items.erase(items.begin() + index);
  }

  return ranked_item;
}

int StoreCredit::minItemsIndex(const int index, vector<st_item> &in)
{
  for (unsigned int i=0; i<in.size(); i++) {
    if (in[i].price < in[index].price) {
      return minItemsIndex(i, in);
    }
  }

  return index;
}

int StoreCredit::maxItemsIndex(const int index, vector<st_item> &in)
{
  for (unsigned int i=0; i<in.size(); i++) {
    if (in[i].price > in[index].price) {
      return maxItemsIndex(i, in);
    }
  }

  return index;
}



