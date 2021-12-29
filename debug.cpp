#include <iostream>
#include <iomanip>
#include <string>
#include <map>

#include "debug.hpp"

#define RESET   "\033[0m"         // Reset
#define GREEN   "\033[1m\033[32m" // Green 
#define RED     "\033[1m\033[31m" // Bold Red 

std::map<uint8_t, char> registerMap = {
   {0, 'B'},
   {1, 'C'},
   {2, 'D'},
   {3, 'E'},
   {4, 'H'},
   {5, 'L'},
   {6, 'A'},
   {7, 'F'}
};

DEBUG::DEBUG() {
   cycleTotal = 0;
   debugging = true;
}

void DEBUG::debugCycle() {
   lineBreak();
   showRam(pc/256);
   showGeneral();
   transferRegisterState();
   cycle();
   cycleTotal++;
}

void DEBUG::showGeneral() {
   std::cout << "    --- Registers ---              --- General ---" << std::endl << std::endl;
   int left = 4;
   for(int i = 0; i < 2; i++) {
      std::cout << " ";
      for(int j = 0; j < 4; j++) {
         std::cout << registerMap[(i*4)+j] << ":";

         if(registers[(i*4)+j] == oldRegisterState[(i*4)+j])
            std::cout << " ";
         else
            std::cout << RED << " ";

         std::cout.width(2); std::cout.fill('0'); std::cout << std::hex << (int)registers[(i*4)+j] << std::dec << " " << RESET;
      }
      if(i == 0) {
         std::cout << "         " << "pc: " << pc << std::hex << " (0x" << pc << ")" << std::dec;
      }
      else if(i == 1) {
         std::cout << "         " << "sp: " << sp << std::hex << " (0x" << sp << ")" << std::dec;
      }
      std::cout << std::endl;
   }
   std::cout << std::endl << std::endl;

}

void DEBUG::lineBreak() {
   std::cout << std::endl << "---+-----------------------------------------------------------------------------------------+---" << std::endl;
   std::cout << "   |    Total cycles: ";
   std::cout.width(8); std::cout.fill(' ');
   std::cout << std::left << cycleTotal << std::right << "                                                               |   " << std::endl;
   std::cout << "   +-----------------------------------------------------------------------------------------+   " << std::endl << std::endl;

}

void DEBUG::showRam(int pos) {
   std::cout << "   --- RAM locations: " << pos*256 << "(0x" << std::hex << pos*256 << std::dec << ") - " << (pos+1)*256 << "(0x" << std::hex << (pos+1)*256 << std::dec << ") ---" << std::endl << std::endl;

   for(int row = 0; row < 8; row++) {
      for(int col = 0; col < 32; col++) {
         if((pos*256)+(row*32)+col == pc)
            std::cout << GREEN << ">" << RESET;
         else
            std::cout << " ";
         std::cout.width(2); std::cout.fill('0'); 
         std::cout << std::hex << (int)ram[(row*32)+col + (pos*256)] << std::dec;        
      }

      std::cout << std::endl;
   }

   std::cout << std::endl << std::endl;
}

void DEBUG::showVram(int pos) {
   std::cout << "   --- Video RAM locations: " << pos*256 << " - " << (pos+1)*256 << " ---" << std::endl << std::endl;
   for(int row = 0; row < 8; row++) {
      for(int col = 0; col < 32; col++) {

         std::cout << " ";
         std::cout.width(2); std::cout.fill('0'); 
         std::cout << std::hex << (int)ram[(row*32)+col + (pos*256)] << std::dec;        
      }

      std::cout << std::endl;
   }
}

void DEBUG::showCart(int pos) {
   std::cout << "   --- Cartridge Memory locations: " << pos*256 << " - " << (pos+1)*256 << " ---" << std::endl << std::endl;
   for(int row = 0; row < 8; row++) {
      for(int col = 0; col < 32; col++) {

         std::cout << " ";
         std::cout.width(2); std::cout.fill('0'); 
         std::cout << std::hex << (int)cartridge[(row*32)+col + (pos*256)] << std::dec;        
      }

      std::cout << std::endl;
   }
}

void DEBUG::transferRegisterState() {
   for(int i = 0; i < 8; i++) {
      oldRegisterState[i] = registers[i];
   }
}

