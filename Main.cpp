#include "Main.h"
#include "strutils.h"

using namespace std;

//-- This program is a simple demonstration of primitive shapes using the SFML library --//
//-- Software Created by MossFrog --//
int main()
{
	//-- We create the render MainWindow and Set its initial properties. --//
	sf::RenderWindow MainWindow(sf::VideoMode(1000, 400), "Audio Sampler", sf::Style::Close);
	MainWindow.setFramerateLimit(60);
	MainWindow.setKeyRepeatEnabled(false);


	//-- Load the necessary Resources into the program. --//
	loadResources();

	//-- Audio systems set up. --//
	if (!sf::SoundBufferRecorder::isAvailable())
	{
		cout << "Error, sound recording device inacessible." << endl;
	}

	sf::SoundBufferRecorder mainRecorder;
	const sf::Int16 * sampleArray = mainRecorder.getBuffer().getSamples();
	int sampleCount = mainRecorder.getBuffer().getSampleCount();

	sf::SoundBuffer intermediateBuffer;

	vector<sf::Int16> modVector;
	//-- The low bitrate Vector contains downsampled 16-Bit audio samples, the leftmost 8-Bits are merely padding zeroes. --//
	vector<sf::Int16> lowBitrateVect;
	
	sf::Sound playbackSound;

	vector<int> sampleRateArray = {44100, 22050, 11025, 5012};
	int sampleIndex = 0;

	//-- Waveform Render Vector --//
	vector<sf::VertexArray> lineVector;


	//-- Boolean elements --//

	bool playbackEnabled = false;

	//-- Text and Button Elements --//
	sf::Text recordText;
	recordText.setFont(UIFont);
	recordText.setString("Hold 'R' to Record.");
	recordText.setColor(sf::Color::White);
	recordText.setPosition(50, 50);
	recordText.setCharacterSize(20);

	sf::Text frequencyText;
	frequencyText.setFont(UIFont);
	frequencyText.setString("Sampling Frequency: " + itoa(sampleRateArray[0]) + "Hz");
	frequencyText.setColor(sf::Color::White);
	frequencyText.setPosition(50, 75);
	frequencyText.setCharacterSize(10);

	sf::Text playText;
	playText.setFont(UIFont);
	playText.setString("Press 'P' for playback.");
	playText.setColor(sf::Color::White);
	playText.setPosition(50, 100);
	playText.setCharacterSize(20);


	sf::Text outputText;
	outputText.setFont(UIFont);
	outputText.setString("Create Downsamples");
	outputText.setColor(sf::Color::Black);
	outputText.setPosition(755, 60);
	outputText.setCharacterSize(15);

	sf::RectangleShape outputRect;
	outputRect.setFillColor(sf::Color::Cyan);
	outputRect.setPosition(750, 50);
	outputRect.setSize(sf::Vector2f(200, 50));


	sf::Text openFileText;
	openFileText.setFont(UIFont);
	openFileText.setString("Open File");
	openFileText.setColor(sf::Color::Black);
	openFileText.setPosition(60, 350);
	openFileText.setCharacterSize(15);

	sf::RectangleShape openFileRect;
	openFileRect.setFillColor(sf::Color::Cyan);
	openFileRect.setPosition(50, 340);
	openFileRect.setSize(sf::Vector2f(115, 50));

	//-- Main game loop --//
	while (MainWindow.isOpen())
	{
		//-- Get the mouse co-ordinates --//
		sf::Vector2i mouse = sf::Mouse::getPosition(MainWindow);

		//-- The code below checks for user bound events such as keyboard, UI, mouse and joystick events --//
		sf::Event event;

		while (MainWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				MainWindow.close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (outputRect.getGlobalBounds().contains(mouse.x, mouse.y) && playbackEnabled == true)
					{
						//-- Create downsamples here. --//
						cout << endl <<"- Creating and saving downsamples to program directory. -" << endl;

						//-- Convert the audio to mono for simplicity --//
						intermediateBuffer.loadFromSamples(&modVector[0], modVector.size(), 1, sampleRateArray[sampleIndex]);

						intermediateBuffer.saveToFile("44K-Source.wav");

						//-- Perform the Bitrate Changing operations --//
						lowerBitrate(modVector, lowBitrateVect);
						intermediateBuffer.loadFromSamples(&lowBitrateVect[0], lowBitrateVect.size(), 1, sampleRateArray[sampleIndex]);

						intermediateBuffer.saveToFile("8-Bit-44K-Source.wav");

						//-- Half the sample rate and repeat the saving process. --//
						downSample(modVector);
						sampleIndex++;
						intermediateBuffer.loadFromSamples(&modVector[0], modVector.size(), 1, sampleRateArray[sampleIndex]);

						intermediateBuffer.saveToFile("22K-Sample.wav");

						//-- Perform the Bitrate Changing operations --//
						lowerBitrate(modVector, lowBitrateVect);
						intermediateBuffer.loadFromSamples(&lowBitrateVect[0], lowBitrateVect.size(), 1, sampleRateArray[sampleIndex]);

						intermediateBuffer.saveToFile("8-Bit-22K-Sample.wav");

						//-- Half the sample rate and repeat the saving process. --//
						downSample(modVector);
						sampleIndex++;
						intermediateBuffer.loadFromSamples(&modVector[0], modVector.size(), 1, sampleRateArray[sampleIndex]);

						intermediateBuffer.saveToFile("11K-Sample.wav");

						//-- Perform the Bitrate Changing operations --//
						lowerBitrate(modVector, lowBitrateVect);
						intermediateBuffer.loadFromSamples(&lowBitrateVect[0], lowBitrateVect.size(), 1, sampleRateArray[sampleIndex]);

						intermediateBuffer.saveToFile("8-Bit-11K-Sample.wav");

						//-- Half the sample rate and repeat the saving process. --//
						downSample(modVector);
						sampleIndex++;
						intermediateBuffer.loadFromSamples(&modVector[0], modVector.size(), 1, sampleRateArray[sampleIndex]);

						intermediateBuffer.saveToFile("5K-Sample.wav");

						//-- Perform the Bitrate Changing operations --//
						lowerBitrate(modVector, lowBitrateVect);
						intermediateBuffer.loadFromSamples(&lowBitrateVect[0], lowBitrateVect.size(), 1, sampleRateArray[sampleIndex]);

						intermediateBuffer.saveToFile("8-Bit-5K-Sample.wav");


						//-- Re-load the Source audio back into the buffer. --//
						intermediateBuffer.loadFromFile("44K-Source.wav");
						sampleIndex = 0;

						//-- Update the mod vector to contain the original intermediate buffer. --//

						sampleArray = intermediateBuffer.getSamples();
						sampleCount = intermediateBuffer.getSampleCount();

						//-- Make sure the modification vector is clear --//
						modVector.clear();

						//-- Copy all the contents of the audio Buffer into the modification vector --//
						for (int i = 0; i < sampleCount; i++)
						{
							modVector.push_back(*(sampleArray + i));
						}
					}

					//-- Opening a file from an external source. --//
					else if (openFileRect.getGlobalBounds().contains(mouse.x, mouse.y))
					{
						
						nfdchar_t *outPath = NULL;
						nfdresult_t result = NFD_OpenDialog("wav,ogg", NULL, &outPath);

						//-- Make sure the directory path given is valid, or else the program will crash searching for a NULL directory --//
						if (outPath != NULL && outPath != "")
						{
							playbackEnabled = true;
							intermediateBuffer.loadFromFile(outPath);

							//-- Update the mod vector to contain the new intermediate buffer. --//

							sampleArray = intermediateBuffer.getSamples();
							sampleCount = intermediateBuffer.getSampleCount();

							//-- Make sure the modification vector is clear --//
							modVector.clear();

							//-- Copy all the contents of the audio Buffer into the modification vector --//
							for (int i = 0; i < sampleCount; i++)
							{
								modVector.push_back(*(sampleArray + i));
							}
						}
						
					}
				}
			}

			//-- Single stroke keyPress and keyRelease Events --//
			if (event.type == sf::Event::KeyPressed)
			{
				//-- Begin Recording. --//
				if (event.key.code == sf::Keyboard::R)
				{
					playbackEnabled = false;
					mainRecorder.start();
					recordText.setColor(sf::Color::Red);
					recordText.setString("Recording...");

				}

				if (event.key.code == sf::Keyboard::P && playbackEnabled == true)
				{
					//-- Move the contents back to the playbackSound entitity --//
					playbackSound.setBuffer(intermediateBuffer);
					playbackSound.play();

					updateVerteces(modVector, lineVector);
				}
			}

			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::R)
				{
					//-- Stop recording. --//
					mainRecorder.stop();

					recordText.setColor(sf::Color::White);
					recordText.setString("Hold 'R' to Record.");

					//-- Change the sampleArray pointer to point to the start of the Buffer --//
					sampleArray = mainRecorder.getBuffer().getSamples();
					sampleCount = mainRecorder.getBuffer().getSampleCount();

					//-- Make sure the modification vector is clear --//
					modVector.clear();

					//-- Copy all the contents of the audio Buffer into the modification vector --//
					for (int i = 0; i < sampleCount; i++)
					{
						modVector.push_back(*(sampleArray + i));
					}

					//-- Convert the audio to mono for simplicity --//
					intermediateBuffer.loadFromSamples(&modVector[0], modVector.size(), 1, sampleRateArray[sampleIndex]);

					//-- Re-enable playback --//
					playbackEnabled = true;

					//-- Output Recording Statistics to the console. --//
					cout << endl;
					cout << "-- Audio Recording --" << endl << endl;
					cout << "Duration: " << mainRecorder.getBuffer().getDuration().asMilliseconds() << "ms" << endl;
					cout << "Sample Count: " << mainRecorder.getBuffer().getSampleCount() << endl;
					cout << "Channel Count: " << mainRecorder.getBuffer().getChannelCount() << endl;
					cout << "Samples Memory Index: " << mainRecorder.getBuffer().getSamples() << endl;

				}
			}
		}

		


		//-- Hovering over UI elements changes their color. --//
		if (outputRect.getGlobalBounds().contains(mouse.x, mouse.y))
		{ outputRect.setFillColor(sf::Color::White); }

		else
		{ outputRect.setFillColor(sf::Color::Cyan); }


		if (openFileRect.getGlobalBounds().contains(mouse.x, mouse.y))
		{ openFileRect.setFillColor(sf::Color::White); }

		else
		{ openFileRect.setFillColor(sf::Color::Cyan); }



		//-- This code hides the console MainWindow, MS-WINDOWS specific --//
		/*HWND hWnd = GetConsoleWindow();
		ShowWindow(hWnd, SW_HIDE);*/


		//-- Clear and draw methods --//

		MainWindow.clear(sf::Color::Black);

		//-- We do all the drawing after the clear event, if not any drawing we do will not be visible --//
		MainWindow.draw(recordText);
		MainWindow.draw(playText);
		MainWindow.draw(frequencyText);
		MainWindow.draw(outputRect);
		MainWindow.draw(outputText);
		MainWindow.draw(openFileRect);
		MainWindow.draw(openFileText);
		for (int i = 0; i < lineVector.size(); i++)
		{
			MainWindow.draw(lineVector[i]);
		}

		MainWindow.display();

	}

	return 0;
}
