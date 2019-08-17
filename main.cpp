#include "SFML/Graphics.hpp"
#include "chip8.hpp"

chip8 myChip8;

sf::Sprite drawGraphics()
{
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
      if (myChip8.graphics[(32 * y) + x])
        image.setPixel(x, y, whitePixel);
      else
        image.setPixel(x, y, blackPixel);
    }
  }
  sf::Texture texture;
  texture.loadFromImage(image);
  sf::Sprite sprite(texture);
  return sprite;
}

int main(int argc, char **argv)
{
  //Initialize SFML Graphics
  sf::RenderWindow renderWindow(sf::VideoMode(64, 32), "Chip8DalePlusPlus");

  // Initialize the Chip8 system and load the game into the memory
  myChip8.initialize();
  if (!myChip8.loadGame("Pong.ch8"))
  {
    std::cout << "Error: Could not load game" << std::endl;
    return 1;
  }

  while (true)
  {
    myChip8.cycle();
    if (myChip8.drawFlag)
    {
      sf::Sprite sprite = drawGraphics();
      renderWindow.clear();
      renderWindow.draw(sprite);
      renderWindow.display();
      myChip8.drawFlag = false;
    }
    myChip8.readKeys();
  }
  return 0;
}
