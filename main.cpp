#include <SFML/Graphics.hpp>
#include "chip8.hpp"

chip8 myChip8;
 
int main(int argc, char **argv) 
{
  // Initialize the Chip8 system and load the game into the memory  
  myChip8.initialize();
  if (!myChip8.loadGame("Pong.ch8")) {
    std::cout<<"Error: Could not load game"<<std::endl;
    return 1;
  }
  return 0;
}