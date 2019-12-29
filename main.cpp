#include "SFML/Graphics.hpp"
#include "chip8.hpp"
#include <sys/stat.h>

chip8 myChip8;

int main(int argc, char **argv)
{
  // Check that a game have been selected
  if (argc == 2) {
    struct stat buffer;
    // Initialize the Chip8 system and load the game into the memory
    myChip8.initialize();
    if (!myChip8.loadGame(argv[1])) {
        std::cout << "Error: Could not load program" << std::endl;
        return 1;
    }
  }
  else {
    std::cout << "Error: Please specify program" << std::endl;
    return 1;
  }

  // Initialize SFML Graphics
  sf::RenderWindow renderWindow(sf::VideoMode(640, 320), "Chip8Dale");
  sf::Event event;

  while (true)
  {

    for (int i = 0; i <= 9; i++) {
      myChip8.cycle();
      myChip8.readKeys();
    }

    if (myChip8.delay_timer > 0)
    {
        myChip8.delay_timer--;
    }

    if (myChip8.sound_timer > 0)
    {
        std::cout << "BEEP!" << std::endl;
        myChip8.sound_timer--;
    }

    if (myChip8.drawFlag)
    {
      while (renderWindow.pollEvent(event))
      {
        if (event.type == sf::Event::Closed)
          renderWindow.close();

      } //Event handling done
      sf::Image image;
      sf::Texture texture;
      image.create(64, 32, sf::Color::Black);
      sf::Color blackPixel(0, 0, 0, 255);
      sf::Color whitePixel(255, 255, 255, 255);

      //Loop through each vertical row of the image
      for (int y = 0; y < 32; y++)
      {
        //then horizontal, setting pixels to black or white in blocks of 8
        for (int x = 0; x < 64; x++)
        {
          if (myChip8.graphics[(64 * y) + x])
            image.setPixel(x, y, whitePixel);
          else
            image.setPixel(x, y, blackPixel);
        }
      }
      texture.loadFromImage(image);
      sf::Sprite sprite(texture);
      sprite.scale(10, 10);
      renderWindow.clear(sf::Color::Black);
      renderWindow.draw(sprite);
      renderWindow.display();
      myChip8.drawFlag = false;
    }

  }
  return 0;
}
