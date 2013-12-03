#ifndef __BIOMIN_H
#define __BIOMIN_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

class Biomin {
public:
  Biomin(void);
  bool is_upper_case(char c);
  bool is_char(char c);
  void cabinet_to_1line(const char * const uid, char *data);
  void bag_to_1line(const char * const uid, char *data);
  void displayQc(void);
  char writeQc(char *epc, char len);

protected:
  void cabinet_to_code(const char * const uid, char *code, char *name);
  char bagepc_to_code(const char * const uid, char *code, char *batch);
  void bagepc_to_ascii(const char * const uid, char *data);
  void bagusr_to_ascii(const char * const usr, char *data);
  char * zeropad(unsigned int i, int digit);
  void CONVERT_INT_TO_STRING (int i, char *out);

private:
  char qc;
};

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

class BiominTag {
public:
  enum Type
  {
    Bag,
    Cabinet,
    Unknown
  };

  BiominTag();
  bool fromEpc(const char *epc, int len);
  std::string getCode(void);
  std::string getBatch(void);
  bool isQc(void);
  Type getType();
  std::string getCabinetName();
  std::string getManufactureDate();
  std::string getExpirationDate();
  std::string getQuantity();

protected:
  std::string putChar(char b);
  std::string ZeroPadNumber(int num, int digit);

private:
  std::string m_productcode;
  std::string m_batch;
  std::string m_name;
  std::string m_mandate;
  std::string m_expdate;
  std::string m_quantity;
  Type m_type;
  bool m_qc;
  const char *m_epc;
};

#endif /* __BIOMIN_H */

