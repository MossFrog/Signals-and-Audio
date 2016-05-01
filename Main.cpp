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

	std::vector<sf::Int16> modVector;

	sf::Sound playbackSound;

	vector<int> sampleRateArray = {44100, 22050, 11025, 5012};
	int sampleIndex = 0;

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
						cout << "Creating and saving downsamples." << endl;
					}
				}
			}

			//-- Single stroke keyPress and keyRelease Events --//
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::R)
				{
					playbackEnabled = false;
					mainRecorder.start();
					recordText.setColor(sf::Color::Red);
					recordText.setString("Recording...");

				}

				if (event.key.code == sf::Keyboard::P && playbackEnabled == true)
				{
					//-- Change the sampleArray pointer to point to the start of the Buffer --//
					sampleArray = mainRecorder.getBuffer().getSamples();
					sampleCount = mainRecorder.getBuffer().getSampleCount();

					//-- Make sure the modification vector is clear --//
					modVector.clear();

					//-- Copy all the contents of the audio Buffer into a modification vector --//
					for (int i = 0; i < sampleCount; i++)
					{
						modVector.push_back(*(sampleArray + i));
					}

					//-- Convert the audio to mono for simplicity --//
					intermediateBuffer.loadFromSamples(&modVector[0], modVector.size(), 1, sampleRateArray[sampleIndex]);

					//-- Move the contents back to the playbackSound entitity --//
					playbackSound.setBuffer(intermediateBuffer);
					playbackSound.play();
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
		{
			outputRect.setFillColor(sf::Color::White);
		}

		else
		{
			outputRect.setFillColor(sf::Color::Cyan);
		}



		//-- This code hides the console MainWindow, MS-WINDOWS specific --//
		/*HWND hWnd = GetConsoleWindow();
		ShowWindow(hWnd, SW_HIDE);*/


		//-- Clear and draw methods --//

		MainWindow.clear(sf::Color::Black);

		//-- We do all the drawing after the clear event, if not any drawing we do will not be visible --//
		MainWindow.draw(recordText);
		MainWindow.draw(frequencyText);
		MainWindow.draw(outputRect);
		MainWindow.draw(outputText);

		MainWindow.display();

	}

	return 0;
}
