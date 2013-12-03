#include "biomin.h"


Biomin::Biomin(void)
{
  qc = 0;
}

void Biomin::CONVERT_INT_TO_STRING (int i, char *out)
{
  itoa(i, out, 10);
}

void Biomin::cabinet_to_1line(const char * const uid, char *data)
{
  char code[15];
  char name[15];
  cabinet_to_code(uid, code, name);
  strcpy(data, code);
  strcpy(&data[strlen(data)], " ");
  strcpy(&data[strlen(data)], name);
}

void Biomin::cabinet_to_code(const char * const uid, char *code, char *name)
{
  unsigned int i;

  memset(code, 0, 15);
  memset(name, 0, 15);

  memcpy(name, uid, 2);
  i = (uid[2] << 8) + uid[3];
  strcpy(&name[2], zeropad(i, 4));

  i = uid[6];
  strcpy(code, zeropad(i, 2));
  strcpy(&code[2], ".");
  i = ((uid[7] & 0x03) << 8) + uid[8];
  strcpy(&code[3], zeropad(i, 3));
  strcpy(&code[6], ".");
  i = (uid[9] << 8) + uid[10];
  strcpy(&code[7], zeropad(i, 3));
  i = uid[11];
  if (i != 0xFF) {
    strcpy(&code[10], ".");
    strcpy(&code[11], zeropad(i, 1));
  }
}

void Biomin::bag_to_1line(const char * const uid, char *data)
{
  char code[15];
  char batch[15];
  char qc = bagepc_to_code(uid, code, batch);

  strcpy(data, code);
  strcpy(&data[strlen(data)], " ");
  strcpy(&data[strlen(data)], batch);
  strcpy(&data[strlen(data)], " ");
  if (qc)
    strcpy(&data[strlen(data)], " QC");
}

char Biomin::bagepc_to_code(const char * const uid, char *code, char *batch)
{
  unsigned int i;
  unsigned int j = 0;

  memset(code, 0, 15);
  memset(batch, 0, 15);

  batch[j++] = uid[0];
  if (is_upper_case(uid[1]))
    batch[j++] = uid[1];
  if (is_upper_case(uid[2]))
    batch[j++] = uid[2];
  i = (uid[3] << 8) + uid[4];
  strcpy(&batch[j], zeropad(i, 3));
  j += 3;
  strcpy(&batch[j++], "/");
  i = uid[5];
  strcpy(&batch[j], zeropad(i, 2));

  i = uid[6];
  strcpy(code, zeropad(i, 2));
  strcpy(&code[2], ".");
  i = ((uid[7] & 0x03) << 8) + uid[8];
  strcpy(&code[3], zeropad(i, 3));
  strcpy(&code[6], ".");
  i = (uid[9] << 8) + uid[10];
  strcpy(&code[7], zeropad(i, 3));
  i = uid[11];
  if (i != 0xFF) {
    strcpy(&code[10], ".");
    strcpy(&code[11], zeropad(i, 1));
  }

  return (uid[7] & 0x40);
}

void Biomin::bagepc_to_ascii(const char * const uid, char *data)
{
  char code[15];
  char batch[15];
  char qc = bagepc_to_code(uid, code, batch);

  strcpy(data, "Bag code: ");
  strcpy(&data[strlen(data)], code);
  strcpy(&data[strlen(data)], "\nBatch number: ");
  strcpy(&data[strlen(data)], batch);
  if (qc)
    strcpy(&data[strlen(data)], "\nQC: Pass");
  else
    strcpy(&data[strlen(data)], "\nQC: Fail");
}

void Biomin::bagusr_to_ascii(const char * const usr, char *data)
{
  unsigned int i;
  unsigned int j = 0;
  char mandate[6];
  char expdate[6];
  char quantity[10];

  memset(mandate, 0, sizeof(mandate));
  memset(expdate, 0, sizeof(expdate));
  memset(quantity, 0, sizeof(quantity));

  i = usr[0];
  strcpy(&mandate[j], zeropad(i, 2));
  j += 2;
  strcpy(&mandate[j], "/");
  i = usr[1];
  strcpy(&mandate[++j], zeropad(i, 2));

  j = 0;
  i = usr[2];
  strcpy(&expdate[j], zeropad(i, 2));
  j += 2;
  strcpy(&expdate[j], "/");
  i = usr[3];
  strcpy(&expdate[++j], zeropad(i, 2));

  j = 0;
  i = (usr[4] << 8) + usr[5];
  strcpy(&quantity[j], zeropad(i, 4));
  j += 4;
  strcpy(&quantity[j], ",");
  i = (usr[6] << 8) + usr[7];
  strcpy(&quantity[++j], zeropad(i, 3));
  j += 3;
  quantity[j] = usr[8];

  strcpy(data, "\nMan. date: ");
  strcpy(&data[strlen(data)], mandate);
  strcpy(&data[strlen(data)], "\nExp. date: ");
  strcpy(&data[strlen(data)], expdate);
  strcpy(&data[strlen(data)], "\nQuantity: ");
  strcpy(&data[strlen(data)], quantity);
}

char * Biomin::zeropad(unsigned int i, int digit)
{
  char data[10];
  int len, j;

  memset(data, 0, sizeof(data));
  if (digit < 2) {
    CONVERT_INT_TO_STRING(i, data);
    return data;
  }
  if (i < 1) {
    memset(data, '0', digit);
  } else if (i < 10) {
    for (j = 0; j < digit-1; ++j)
      data[j] = '0';
    CONVERT_INT_TO_STRING(i, &data[j]);
  } else {
    strcpy(data, zeropad(i / 10, --digit));
    len = strlen(data);
    CONVERT_INT_TO_STRING(i % 10, &data[len]);
  }

  return data;
}

bool Biomin::is_upper_case(char c)
{
  return ((c >= 'A') && (c <= 'Z'));
}

bool Biomin::is_char(char c)
{
  return (
      ((c >= '0') && (c <= '9'))
      || ((c >= 'A') && (c <= 'Z'))
      || ((c >= 'a') && (c <= 'z'))
      );
}

void Biomin::displayQc(void)
{
//  DISPLAY_DRAW_TEXT(BOX_TYPE_LABEL,30,0,68, FALSE, FONT_11_NORMAL, ALIGN_CENTRE, "QC CHECK");
//  DISPLAY_DRAW_TEXT(BOX_TYPE_LABEL,0,1,128, FALSE, FONT_11_NORMAL, ALIGN_CENTRE, "");
//  if (qc)
//    DISPLAY_DRAW_TEXT(BOX_TYPE_LABEL,0,2,128, FALSE, FONT_11_NORMAL,ALIGN_CENTRE, "<       PASS        >");
//  else
//    DISPLAY_DRAW_TEXT(BOX_TYPE_LABEL,0,2,128, FALSE, FONT_11_NORMAL,ALIGN_CENTRE, "<       FAIL        >");
//  DISPLAY_DRAW_TEXT(BOX_TYPE_LABEL,0,3,128, FALSE, FONT_11_NORMAL, ALIGN_LEFT, "");
}

char Biomin::writeQc(char *epc, char len)
{
  char res = 0;
//  char val[2];

//  memcpy(val, &epc[6], 2);
//  (qc)?(val[1] |= 0x40):(val[1] &= ~(0x40));
//  if (RFE_WRITE_TO_TAG(epc, len, 1, 5, val, sizeof(val), "WRITE_QC_BIT")
//    || RFE_READ_EPC(uid, &uid_len, "WRITE_QC_CONFIRM")
//    || memcmp(val, &uid[6], 2) == 0
//    ) {
//    res = 1;
//  }

  return res;
}

using namespace std;

BiominTag::BiominTag()
  : m_productcode("0.000.000.0"),
    m_batch("AAA000/00"),
    m_name("AA0000"),
    m_mandate("03/13"),
    m_expdate("03/13"),
    m_quantity("0000,000u"),
    m_type(Bag), m_qc(false), m_epc(NULL)
{
}

bool BiominTag::fromEpc(const char *epc, int len)
{
  std::ostringstream ss;
  int i, j;

  m_epc = epc;
  if (len != 12)
    return false;

  if (m_epc[7] & 0x80)
    m_type = Cabinet;
  else
    m_type = Bag;

  if (m_epc[7] & 0x40)
    m_qc = true;
  else
    m_qc = false;

  i = m_epc[6];
  ss.clear();
  ss << ZeroPadNumber(i, 2);
  i = ((static_cast<uint8_t>(m_epc[7]) & 0x03) << 8)
      + static_cast<uint8_t>(m_epc[8]);
  j = ((static_cast<uint8_t>(m_epc[9]) & 0x03) << 8)
      + static_cast<uint8_t>(m_epc[10]);
  ss << "." << ZeroPadNumber(i, 3) << "." << ZeroPadNumber(j, 3);
  i = static_cast<uint8_t>(m_epc[11]);
  if (i != 0xFF)
    ss << "." << ZeroPadNumber(i, 1);
  m_productcode = ss.str();

  m_mandate = "";
  m_expdate = "";
  m_quantity = "";
  if (m_type == Bag) {
    m_name = "";
    ss.clear();
    ss << putChar(m_epc[0]) << putChar(m_epc[1]) << putChar(m_epc[2]);
    i = ((static_cast<uint8_t>(m_epc[3]) & 0x03) << 8)
        + static_cast<uint8_t>(m_epc[4]);
    j = static_cast<uint8_t>(m_epc[5]);
    ss << ZeroPadNumber(i, 3) << "/" << ZeroPadNumber(j, 2);
    m_batch = ss.str();
  } else {
    m_batch = "";
    i = ((static_cast<uint8_t>(m_epc[3]) & 0x02) << 8)
        + static_cast<uint8_t>(m_epc[3]);
    ss << putChar(m_epc[0]) << putChar(m_epc[1]) << ZeroPadNumber(i, 4);
    m_name = ss.str();
  }

  return true;
}

string BiominTag::getCode(void)
{
  return m_productcode;
}

string BiominTag::getBatch(void)
{
  return m_batch;
}

bool BiominTag::isQc(void)
{
  return m_qc;
}

BiominTag::Type BiominTag::getType()
{
  return m_type;
}

string BiominTag::getCabinetName()
{
  return m_name;
}

string BiominTag::getManufactureDate()
{
  return m_mandate;
}

string BiominTag::getExpirationDate()
{
  return m_expdate;
}

string BiominTag::getQuantity()
{
  return m_quantity;
}

std::string BiominTag::putChar(char b)
{
  std::ostringstream ss;
  ss << b;
  return ss.str();
}

std::string BiominTag::ZeroPadNumber(int num, int digit)
{
  std::ostringstream ss;
  ss << std::setw(digit) << std::setfill( '0' ) << num;
  return ss.str();
}

