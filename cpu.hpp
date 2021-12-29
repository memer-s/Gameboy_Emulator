#ifndef CPU_H
#define CPU_H

#define BYTE unsigned char

class CPU {
   protected:
      // MEMROY
      BYTE internalRom[256];
      BYTE ram[65536];
      BYTE vram[8192];
      BYTE cartridge[1024*512];

      // REGISTERS
      BYTE registers[8];

      // STACK POINTER && PROGRAM COUNTER.
      int pc;
      unsigned short sp;

      // CYCLES WAITING TO BE DONE
      int wait;
      bool debugging;
      bool startup;

   public:
      CPU();
      void setReg();
      void setMem();
      void clearRam();
      void clearVram();
      void clearRegisters();
      void executeOpcode();
      void cycle();
      void loadCartridge();
      void initMemory();
      void runStartup();
};

#endif
