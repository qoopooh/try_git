#include "command.h"

Command::Command(unsigned char id)
  : m_id(id), m_param_size(0), m_param(NULL),
  m_dispatch(false) {
}

Command::Command(unsigned char id, char *param, int p_size)
  : m_id(id), m_param_size(p_size),
  m_param_string(false), m_dispatch(false) {
  int i;

  if (p_size < 1) {
    m_param = NULL;
    return;
  }

  m_param = new char[p_size];
  for (i = 0; i < p_size; i++) {
    m_param[i] = param[i];
  }
}

Command::Command(unsigned char id, char param)
  : m_id(id), m_param_size(1),
  m_param_string(false), m_dispatch(false) {
  m_param = new char[1];
  m_param[0] = param;
}

//Command::Command(unsigned char id, char param1, char param2)
  //: m_id(id), m_param_size(2),
  //m_dispatch(false) {
  //m_param = new char[2];
  //m_param[0] = param1;
  //m_param[1] = param2;
//}

Command::~Command() {
  if (m_param != NULL) {
    delete m_param;
    m_param = NULL;
  }
}

unsigned char Command::getId() {
  return m_id;
}

int Command::getParamSize() {
  return m_param_size;
}

char* Command::getParam() {
  if (m_param_size < 1)
    return NULL;
  return m_param;
}

int Command::toString(char* buffer, int buffer_size) {
  char* p_param = buffer + 2;
  int max_buff_size;
  int p_size;
  int i;

  Convert::HexToAscii((const char *) &m_id, 1, buffer);

  if (m_param_size > 0) {
    if (m_param_string) {
      max_buff_size = buffer_size - 3;
      if (m_param_size > max_buff_size)
        p_size = max_buff_size;
      else
        p_size = m_param_size;

      for (i = 0; i < p_size; i++) {
        p_param[i] = m_param[i];
      }
      p_param[p_size] = '\0';
    } else {
      max_buff_size = (buffer_size>>1) - 2;
      if (m_param_size > max_buff_size)
        p_size = max_buff_size;
      else
        p_size = m_param_size;

      Convert::HexToAscii(m_param, p_size, p_param);
      p_size<<=1;
      p_param[p_size] = '\0';
    }
  }

  return p_size;
}

void Command::setParamString(bool on) {
  m_param_string = on;
}

void Command::setDispatched() {
  m_dispatch = true;
}

bool Command::isDispatched() {
  return m_dispatch;
}

