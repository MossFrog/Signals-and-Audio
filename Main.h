#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>

#include <cstdlib>
#include <vector>
#include <nfd.h>

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

//-- The downSample function halves the number of samples. --//
//-- Given the source has a sampling frequency of 44100Hz it should half the resolution. --//
void downSample(vector<sf::Int16> & samplesVector)
{
	bool toggle = false;
	vector<sf::Int16> tempVector;

	for (int i = 0; i < samplesVector.size(); i++)
	{
		if (toggle == true)
		{
			tempVector.push_back(samplesVector[i]);
			toggle = false;
		}

		else if (toggle == false)
		{
			toggle = true;
		}
	}

	samplesVector = tempVector;
}