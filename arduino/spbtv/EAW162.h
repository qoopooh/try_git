#ifndef __EAW162_H
#define __EAW162_H

#include "Arduino.h"
#include "DogM162.h"

class EAW162 : public DogM162 {
public:

  EAW162(uint8_t db0, uint8_t db1, uint8_t db2, uint8_t db3,
          uint8_t rs, uint8_t enable);
          
  EAW162(uint8_t db0, uint8_t db1, uint8_t db2, uint8_t db3,
    uint8_t rw, uint8_t rs, uint8_t enable, uint8_t reset);
    
   void displayOff();

   void displayOn();    
    
   void displayBright(bool bright);
   
private: 
  virtual void sendDisplayInitCommands();
  virtual void deviceSpecificSetPosition(unsigned char x, unsigned char line);

};

#endif /* __EAW162_H */

