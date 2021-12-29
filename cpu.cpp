#include "cpu.hpp"
#include <iostream>
#include <fstream>

#define BYTE unsigned char

CPU::CPU() {
   pc = 0x100;
   sp = 0;
   startup = true;
   debugging = false;
   clearRam();
   clearVram();
   clearRegisters();
   runStartup();
   loadCartridge();
   initMemory();
}

void CPU::cycle() {
   executeOpcode();
}

void CPU::executeOpcode() {
   
   for(int i = 0; i < wait; i++) {
      // ADD DELAY HERE
   }

   wait = 0;
   if(debugging) {
      std::cout << "   -> Current opcode: " << std::hex;
      std::cout.width(2); std::cout.fill('0'); std::cout << (int)ram[pc] << "-> " << (int)(ram[pc]&0xf0) << ", " << (int)(ram[pc]&0x0f) << std::dec << std::endl << std::endl; 
   }
   switch(ram[pc]&0xf0) {

      case 0x00:
         switch(ram[pc]&0x0f) {
            case 0x00:
               pc++;
               wait += 4;
               if(debugging)
                  std::cout << "(NOP): NO OP" << std::endl;
               break;

            case 0x04:
               uint8_t flags = 0;
               // Zero flag.
               if(registers[0]+1 == 0) 
                  flags |= 0x80;
               
               if((((registers[0]&0x0f)+(1&0x0f))&0x10)==0x10)
                  flags |= 0x20;

               registers[0]++;
               wait+=4;
               pc++;
               break;
         }
         break;

      case 0x40:
         if((ram[pc]&0x8) != 0x8) {
            // LD == MOV x,y; LOAD y INTO x 
            registers[0] = registers[(ram[pc]&0x0f)];
            if(debugging)
               std::cout << "(LD B, x): LOADING REGISTER " << "B (0) <- " << (int)registers[(ram[pc]&0x0f)] << std::endl;
         }
         else
            registers[1] = registers[(ram[pc]&0x0f)-0x08];
   
         pc++;
         break;

      case 0x70:
         if((ram[pc]&0x80) != 0x80) {
            uint16_t result = 0;
            if((ram[pc]&0x0f) == 0x06) {
               wait+=4;
               std::cout << "(HALT): Halt the program" << std::endl;
               break;
            }
            else if((ram[pc]&0x0f) == 0x07) {
               
            }
            wait+=8;
            if(debugging)
               std::cout << "(LD A, x): LOADING REGISTER " << "A (0) <- " << (int)registers[4] << (int)registers[5] << std::endl;

         }
         else {
            
            if((ram[pc]&0x0f) == 0x0e) {
               uint16_t hl = (registers[5] << 8);
               hl |= registers[4];
               registers[6] = hl;
               wait+=4;
            }
            
            else if((ram[pc]&0x0f) == 0x0f) {
               registers[6] = registers[6];
            }

            else {
               registers[6] = registers[(ram[pc]&0x0f)];
            }
            wait+=4;
            if(debugging)
               std::cout << "(LD A, x): LOADING REGISTER " << "A (0) <- " << (int)registers[(ram[pc]&0x0f)] << std::endl;
         }
         break;


      // I am starting to suspect that i hit my head too hard when i were little.
      // this is sooooo dumb but i was too stupido to realize, and i am now too lazy to try and fix this, will do later...
      case 0x90:
         if((ram[pc]&0x0f) != 0x6 || (ram[pc]&0x0f) != 0x7 || (ram[pc]&0x0f) != 0xe || (ram[pc]&0x0f) != 0xf) {
            if((ram[pc]&0x08) != 0x08) {
               uint8_t result = 0x0;
               uint8_t flags = 0x0;
               
               // Zero flag.
               if(registers[6]+registers[(ram[pc]&0x0f)] == 0) 
                  flags |= 0x80;
               
               // Half carry flag.
               if((((registers[6]&0x0f)+(registers[(ram[pc]&0x0f)]&0x0f))&0x10)==0x10) 
                  flags |= 0x20;
               
               // Carry flag.
               if(registers[6]+registers[(ram[pc]&0x0f)] > 255) 
                  flags |= 0x10;
               
               registers[6] = registers[6] + registers[(ram[pc]&0x0f)];
               registers[7] = flags;
               wait+=4;
               if(debugging)
                  std::cout << "(ADD A,X): Add registers A and X, set flags to > Z 0 H C" << std::endl;
            }

            else {
               uint8_t result = 0x0;
               uint8_t flags = 0x0;
               
               if(registers[6]+registers[(ram[pc]&0x0f)] == 0) 
                  flags |= 0x80;
               
               if((((registers[6]&0x0f)+(registers[(ram[pc]&0x0f)]&0x0f))&0x10)==0x10) 
                  flags |= 0x20;
               
               if(registers[6]+registers[(ram[pc]&0x0f)] > 255) 
                  flags |= 0x10;
               
               if((flags&0x10) == 0x10) {
                  result = 1;
               }

               else {
                  result = 0;
               }

               registers[7] = flags;
               registers[6] = result;
               wait+=4;

               if(debugging)
                  std::cout << "(ADC A,X): Add registers A and X, if C flag is set, set A to 1 otherwise 0, keep Z 0 H C" << std::endl;
            }
         }
         else {
            if((ram[pc]&0x08) != 0x08) {
               if((ram[pc]&0x0f) == 0x06) {
                  uint16_t hl = (registers[5] << 8);
                  hl |= registers[4];
                  uint8_t flags = 0x0;
                  
                  // Zero flag.
                  if(registers[6]+hl == 0) 
                     flags |= 0x80;
                  
                  // Half carry flag.
                  if((((registers[6]&0x0f)+(hl&0x0f))&0x10)==0x10) 
                     flags |= 0x20;
                  
                  // Carry flag.
                  if(registers[6]+hl > 255) 
                     flags |= 0x10;
                  
                  registers[7] = flags;
                  registers[6] = registers[6] + hl;
                  
                  wait += 8;
                  if(debugging)
                     std::cout << "(ADD A,HL): Add registers A and X, set flags to > Z 0 H C" << std::endl;
               }
               else {
                  uint8_t result = 0x0;
                  uint8_t flags = 0x0;
                  
                  // Zero flag.
                  if(registers[6]+registers[6] == 0) 
                     flags |= 0x80;
                  
                  // Half carry flag.
                  if((((registers[6]&0x0f)+(registers[6]&0x0))&0x10)==0x10) 
                     flags |= 0x20;
                  
                  // Carry flag.
                  if(registers[6]+registers[6] > 255) 
                     flags |= 0x10;
                  
                  registers[6] = registers[6] + registers[(ram[pc]&0x0f)];
                  registers[7] = flags;
                  wait+=4;
                  if(debugging)
                     std::cout << "(ADD A,X): Add registers A and X, set flags to > Z 0 H C" << std::endl;
               }
            }

            else {
               if((ram[pc]&0x0f) == 0x0e) {
                  uint16_t hl = (registers[5] << 8);
                  hl |= registers[4];
                  uint8_t flags = 0x0;
                  
                  // Zero flag.
                  if(registers[6]+hl == 0) 
                     flags |= 0x80;
                  
                  // Half carry flag.
                  if((((registers[6]&0x0f)+(hl&0x0f))&0x10)==0x10) 
                     flags |= 0x20;
                  
                  // Carry flag.
                  if(registers[6]+hl > 255) 
                     flags |= 0x10;
                  
                  registers[7] = flags;

                  if((flags&0x10) == 0x10) {
                     registers[6] = 1;
                  }

                  else {
                     registers[6] = 0;
                  }
                  
                  wait += 8;
                  if(debugging)
                     std::cout << "(ADC A,HL): Add registers A and X, if C flag is set, set A to 1 otherwise 0, keep Z 0 H C" << std::endl;
               }
               else {
                  uint8_t result = 0x0;
                  uint8_t flags = 0x0;
                  
                  // Zero flag.
                  if(registers[6]+registers[6] == 0) 
                     flags |= 0x80;
                  
                  // Half carry flag.
                  if((((registers[6]&0x0f)+(registers[6]&0x0))&0x10)==0x10) 
                     flags |= 0x20;
                  
                  // Carry flag.
                  if(registers[6]+registers[6] > 255) 
                     flags |= 0x10;
                  
                  registers[7] = flags;

                  if((flags&0x10) == 0x10) {
                     registers[6] = 1;
                  }

                  else {
                     registers[6] = 0;
                  }

                  wait+=4;
                  if(debugging)
                     std::cout << "(ADC A,HL): Add registers A and X, if C flag is set, set A to 1 otherwise 0, keep Z 0 H C" << std::endl;
               }
            }
         }

         pc+=1;
         break;

      // JP ; jump to
      case 0xc0:
         uint16_t neigh;
         neigh = (ram[pc+1] << 8);
         neigh = neigh + (ram[pc+2]);
         switch(ram[pc]&0x0f) {
            case 0x03:
               if(debugging)
                  std::cout << "(JP, u16): Jump to " << std::hex << neigh << std::dec << std::endl;
               pc = neigh;
               wait += 4;
               break;
            
            case 0x04:
               if((ram[8]&0x80) != 0x80) {
                  sp-=0x2;
                  ram[sp] = ram[pc+1];
                  ram[sp+1] = ram[pc+2];
                  wait += 24;
               }
               else {
                  wait+=12;  
               }
               pc+=3;
               std::cout << "(CALL NZ, a16): Add address to stack if NZ" << std::endl;
               break;
               
            case 0x09:
               uint16_t callback = (ram[sp] << 8);
               callback += ram[sp+1];
               sp+=0x2;
               wait += 16;
               pc++;
               break;
         }
         break;



      default:
         std::cout << "idk.." << std::endl;
         break;
   } }

void CPU::initMemory() {

   for(int i = 0; i < 0x4000; i++) {
      ram[i] = cartridge[i];
   }
   
   for(int i = 0x4000; i < 0x8000; i++) {
      ram[i] = cartridge[i];
   }
}

void CPU::runStartup() {
   registers[6] = 0x01; // A
   registers[7] = 0xb0; // F
   registers[1] = 0x13; // C
   registers[3] = 0xd8; // E
   registers[4] = 0x01; // H
   registers[5] = 0x4d; // L
   sp   = 0xfffe;
   // bruh
   ram[0xff05] = 0x00;
   ram[0xff06] = 0x00;
   ram[0xff07] = 0x00;
   ram[0xff10] = 0x80;
   ram[0xff11] = 0xbf;
   ram[0xff12] = 0xf3;
   ram[0xff14] = 0xbf;
   ram[0xff16] = 0x3f;
   ram[0xff17] = 0x00;
   ram[0xff19] = 0xbf;
   ram[0xff1a] = 0x7f;
   ram[0xff1b] = 0xff;
   ram[0xff1c] = 0x9f;
   ram[0xff1e] = 0xbf;
   ram[0xff20] = 0xff;
   ram[0xff21] = 0x00;
   ram[0xff22] = 0x00;
   ram[0xff23] = 0xbf;
   ram[0xff24] = 0x77;
   ram[0xff25] = 0xf3;
   ram[0xff26] = 0xf1;
   ram[0xff40] = 0x91;
   ram[0xff42] = 0x00;
   ram[0xff43] = 0x00;
   ram[0xff45] = 0x00;
   ram[0xff47] = 0xfc;
   ram[0xff48] = 0xff;
   ram[0xff49] = 0xff;
   ram[0xff4a] = 0x00;
   ram[0xff4b] = 0x00;
   ram[0xffff] = 0x00;
}

void CPU::clearRam() {
   for(int i = 0; i < 1024; i++)
      ram[i] = 0;
}

void CPU::clearVram() {
   for(int i = 0; i < 8192; i++)
      ram[i] = 0;
}

void CPU::clearRegisters() {
   for(int i = 0; i < 7; i++)
      registers[i] = 0;
}

void CPU::loadCartridge() {
   char buffer[1024*512];

   std::ifstream file;
   file.open("Zelda.gb");
   file.read(buffer, 1024*512);
   file.close();
   for(int i = 0; i < 1024*512; i++) {
      cartridge[i] = buffer[i];
   }
}
