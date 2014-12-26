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
  m_param = new char[p_size];

  // copy
  for (i = 0; i < p_size; i++) {
    m_param[i] = param[i];
  }
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

char * Command::getParam() {
  if (m_param_size < 1)
    return NULL;
  return m_param;
}

