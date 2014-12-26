#include "command.h"

Command::Command(unsigned char cmd) {
  m_cmd = cmd;
  m_param_size = 0;
  m_param = NULL;
}

Command::Command(unsigned char cmd, char *param, int p_size) {
  int i;

  m_cmd = cmd;
  m_param_size = p_size;

  if (p_size < 1) {
    m_param = NULL;
    return;
  }

  m_param = new char[p_size];
  for (i = 0; i < p_size; i++) {
    m_param[i] = param[i];
  }
}

Command::Command(unsigned char cmd, char param) {
  m_cmd = cmd;
  m_param_size = 1;

  m_param = new char[1];
  m_param[0] = param;
}

Command::Command(unsigned char cmd, char param1, char param2) {
  m_cmd = cmd;
  m_param_size = 2;

  m_param = new char[2];
  m_param[0] = param1;
  m_param[1] = param2;
}

Command::~Command() {
  if (m_param != NULL) {
    delete m_param;
    m_param = NULL;
  }
}

unsigned char Command::getCommand() {
  return m_cmd;
}

int Command::getParamSize() {
  return m_param_size;
}

char* Command::getParam() {
  if (m_param_size < 1)
    return NULL;
  return m_param;
}

char* Command::toString(char* buffer, int buffer_size) {
  char* p_param = buffer + 2;

  Convert::HexToAscii((const char *) &m_cmd, 1, buffer);

  if (m_param_size > 0) {
    int max_buff_size = (buffer_size>>1) - 2;
    int p_size = m_param_size;

    if (p_size > max_buff_size)
      p_size = max_buff_size;

    Convert::HexToAscii(m_param, p_size, p_param);
    p_param[p_size<<1] = '\0';
  }

  return buffer;
}

