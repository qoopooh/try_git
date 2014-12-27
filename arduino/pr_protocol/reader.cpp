#include "reader.h"

Reader::Reader() {
}

Reader::~Reader() {
}

void Reader::handle(Command* cmd) {
  Command* t_cmd = NULL;
  String message;
  char param[16];
  unsigned char id = cmd->getId();

  switch (id) {
    case CMD_SOFTWARE_VERSION:
      message = FIRMWARE_VERSION;
      message.toCharArray(param, sizeof(param));
      t_cmd = new Command(id, param, message.length());
      t_cmd->setParamString(true);
      assignNewCommand(t_cmd);
      break;
    case CMD_HARDWARE_VERSION:
      param[0] = 0;
      param[1] = 1;
      t_cmd = new Command(id, param, 2);
      assignNewCommand(t_cmd);
      break;
    default:
      break;
  }
}

bool Reader::isCommandAvailable() {
  return m_cmd_available;
}

Command* Reader::getCommand() {
  if (m_cmd_available)
    return m_cmd;
  return NULL;
}

void Reader::assignNewCommand(Command* cmd) {
  if (m_cmd != NULL) {
    delete m_cmd;
  }
  m_cmd = cmd;
  m_cmd_available = true;
}

