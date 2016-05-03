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

void updateVerteces(vector<sf::Int16> samplesVector, vector<sf::VertexArray> & lineVector)
{
	//-- Clear any previous data. --//
	lineVector.clear();

	//-- Set the "Step Size" essentially the resolution downscale. --//
	int stepSize = int(samplesVector.size() / 950);

	//-- Create a temporary storage line. --//
	sf::VertexArray tempLine(sf::Lines, 2);

	int xPos = 0;
	int yPos = 270;

	//-- Gather periodic data and store in the lineVector. --//
	for (int i = 0; i < samplesVector.size();)
	{
		tempLine[0].position = sf::Vector2f(xPos, yPos);
		tempLine[1].position = sf::Vector2f(xPos, yPos - int(samplesVector[i]/150));
		lineVector.push_back(tempLine);
		xPos++;
		i = i + stepSize;
	}
}