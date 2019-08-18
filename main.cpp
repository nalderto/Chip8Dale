#include "SFML/Graphics.hpp"
#include "chip8.hpp"

chip8 myChip8;

int main(int argc, char **argv)
{
  //Initialize SFML Graphics
  sf::RenderWindow renderWindow(sf::VideoMode(640, 320), "Chip8DalePlusPlus");
  sf::Event event;

  // Initialize the Chip8 system and load the game into the memory
  myChip8.initialize();
  if (!myChip8.loadGame("test_opcode.ch8"))
  {
    std::cout << "Error: Could not load game" << std::endl;
    return 1;
  }

  while (true)
  {
    myChip8.cycle();
    if (myChip8.drawFlag)
    {
      while (renderWindow.pollEvent(event))
      {
        if (event.type == sf::Event::Closed)
          renderWindow.close();

      } //Event handling done
      sf::Image image;
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
      sf::Texture texture;
      image.saveToFile("test.png");
      texture.loadFromImage(image);
      sf::Sprite sprite(texture);
      sprite.scale(10, 10);
      renderWindow.clear(sf::Color::Black);
      renderWindow.draw(sprite);
      renderWindow.display();
      myChip8.drawFlag = false;
    }
    myChip8.readKeys();
  }
  return 0;
}
