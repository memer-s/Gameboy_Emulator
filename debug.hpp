#ifndef DEBUG_H
#define DEBUG_H

#include "cpu.hpp"

#define BYTE unsigned char

class DEBUG : public CPU {
   private:
      int cycleTotal;
      BYTE oldRegisterState[8] = {0};
   public: 
      DEBUG();
      void lineBreak();
      void showRam(int);
      void showVram(int);
      void showCart(int);
      void showGeneral();
      void debugCycle();
      void transferRegisterState();

};


#endif
