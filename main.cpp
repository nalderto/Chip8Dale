#include "Chip8.hpp"
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define CLOCKSPEED 600

Chip8* myChip8;

int main(int argc, char **argv)
{
  // Check that a game have been selected
  if (argc == 2) {
    // Initialize the Chip8 system and load the game into the memory
    myChip8 = new Chip8();
    if (!myChip8->loadGame(argv[1])) {
        std::cout << "Error: Could not load program" << std::endl;
        return 1;
    }
  }
  else {
    std::cout << "Error: Please specify program" << std::endl;
    return 1;
  }

  while (true)
  {
    // Program Start Time
    std::chrono::time_point<std::chrono::steady_clock> prev_time = std::chrono::steady_clock::now();
    double clockCyclesPerSecond = CLOCKSPEED / 60.0;

    for (int i = 0; i < clockCyclesPerSecond; i++) {
      myChip8->cycle();
    }

    if (myChip8->delay_timer > 0)
    {
        myChip8->delay_timer--;
    }

    if (myChip8->sound_timer > 0)
    {
        std::cout << "BEEP!" << std::endl;
        myChip8->sound_timer--;
    }

    if (myChip8->drawFlag)
    {
      myChip8->draw();
    }
    std::chrono::time_point<std::chrono::steady_clock> now_time = std::chrono::steady_clock::now();

    std::chrono::duration<double> diff = now_time - prev_time;

    std::chrono::duration<double> wait = std::chrono::microseconds(16700) - diff;

    std::this_thread::sleep_for(wait);

  }
  return 0;
}
