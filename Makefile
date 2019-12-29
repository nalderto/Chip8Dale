chip8make: chip8.cpp chip8.hpp main.cpp
	g++ -o chip8dale chip8.cpp chip8.hpp main.cpp -lsfml-graphics -lsfml-window -lsfml-system -g


