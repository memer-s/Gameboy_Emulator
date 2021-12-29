#include <iostream>
#include "cpu.hpp"
#include "debug.hpp"

using namespace std;

int main() {
   DEBUG cpu;
   for(int i = 0; true; i++) {
      cpu.debugCycle();
      //cpu.showCart(i);
      std::cin.get();
   }
   return 0;
}
