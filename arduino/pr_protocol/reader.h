#ifndef READER_H
#define READER_H

#include "def.h"
#include "command.h"

class Reader {
  public:
    Reader();
    ~Reader();

    void handle(Command* cmd);
    bool isCommandAvailable();
    bool isAutoRead();
    Command* getCommand();

  protected:
    void assignNewCommand(Command* cmd);

  private:
    Command* m_cmd;
    bool m_cmd_available;
    bool m_auto_read;
};

#endif /* READER_H */
