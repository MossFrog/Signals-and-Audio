#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <cstdlib>

//-- Commonly Used Variables. --//
sf::Font UIFont;

using namespace std;

//-- Function for loading external data into program. --//
void loadResources()
{
	if (UIFont.loadFromFile("Pixel Emulator.ttf"))
	{
		cout << "Loaded the font 'Pixel Emulator.ttf'" << endl;
	}

	else
	{
		cout << "Failed to load the font 'Pixel Emulator.ttf'" << endl;
	}

}