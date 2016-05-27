#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>

#include <cmath>
#include <cstdlib>
#include <vector>
#include <nfd.h>
#include "fft.h"

//-- Commonly Used Variables. --//
sf::Font UIFont;

using namespace std;

//-- Color palette variables --//
vector<sf::Color> colorPalette = { sf::Color::Cyan, sf::Color::White, sf::Color::Yellow, sf::Color::Green, sf::Color::Red };
int colorIndex = 0;

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


	cout << endl << "-  Welcome to the S.A.M.T (Simple Audio Manipulation Tool)  -" << endl << endl;
	cout << "> The program Assumes that the input has a Fsamp = 44100Hz." << endl;
	cout << "> The program Supports opening WAV and OGG Vorbis files." << endl;
	cout << "> The Waveform color can be changed by using the 'C' key." << endl;
	cout << "> The Waveform is updated whenever playback is called." << endl;
	cout << "> The 8-Bit output files are contained within a 16-Bit wrapper." << endl << endl;
	cout << "// For any Bugs or Issues Check out The Github Repository" << endl << "@ Github.com/MossFrog //" << endl << endl;

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

		tempLine[0].color = sf::Color::Black;
		tempLine[1].color = colorPalette[colorIndex];

		lineVector.push_back(tempLine);
		xPos++;
		i = i + stepSize;
	}
}


//-- Since SFML cannot output in 8-bit format, we emulate this by removing the sign bit and any data within the leftmost 8-Bits --//
void lowerBitrate(vector<sf::Int16> sourceVect, vector<sf::Int16> & outputVector)
{
	//-- Make sure the output vector is clean --//
	outputVector.clear();

	//-- Use bit shifting operations to remove any "extra" data. --//
	for (int i = 0; i < sourceVect.size(); i++)
	{
		//-- Limit the audio to the maximum range of 8-Bit audio. --//
		sourceVect[i] = sourceVect[i] >> 8;
		if (sourceVect[i] > 128)
		{
			sourceVect[i] = 128;
		}
		if (sourceVect[i] < -128)
		{
			sourceVect[i] = -128;
		}
		sourceVect[i] = abs(sf::Int8(sourceVect[i]));
		sourceVect[i] = sourceVect[i] << 9;
		
		outputVector.push_back(sourceVect[i]);
	}
}

void colorShift(vector<sf::VertexArray> & lineVector)
{
	colorIndex++;
	if (colorIndex >= colorPalette.size())
	{
		colorIndex = 0;
	}
	for (int i = 0; i < lineVector.size(); i++)
	{
		lineVector[i][1].color = colorPalette[colorIndex];
	}
}

void forceMono(vector<sf::Int16> & stereoVect)
{
	vector<sf::Int16> tempVect;
	for (int i = 0; i < stereoVect.size(); i+=2)
	{
		tempVect.push_back((stereoVect[i] + stereoVect[i + 1])/2);
	}

	stereoVect = tempVect;
}