#include "Main.h"
#include "strutils.h"

using namespace std;

//-- This program is a simple demonstration of audio signal Manipulation using the SFML library as a GUI --//
//-- Software Created by MossFrog --//
int main()
{
	//-- We create the render MainWindow and Set its initial properties. --//
	sf::RenderWindow MainWindow(sf::VideoMode(1000, 600), "Audio Sampler", sf::Style::Close);
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

	vector<int> sampleRateArray = { 44100, 22050, 11025, 5012 };
	int sampleIndex = 0;

	//-- Waveform Render Vector --//
	vector<sf::VertexArray> lineVector;

	//-- Boolean elements --//
	bool playbackEnabled = false;

	//-- FFT Variables --//

	/* IMPULSE RESPONSE SECTION */
	sf::SoundBuffer IRRawBuffer;
	vector<sf::Int16> IRBuffer;
	int IRSampleCount;
	const sf::Int16 * IRMemLoc;
	vector<complex> IRComplex;

	/* MAIN SIGNAL SECTION */
	vector<sf::Int16> SignalBuffer;
	vector<complex> SignalComplex;

	/* OUTPUT SIGNAL SECTION */
	vector<complex> outputComplex;
	vector<sf::Int16> outputConverted;
	
	//-- Domain change variable --//
	string domain = "Frequency";

	//-- DSP Filter section --//

	


	//-------------------//

	sf::SoundBuffer fftBuffer;


	bool EffectEnabled = false;

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
	openFileText.setPosition(60, 450);
	openFileText.setCharacterSize(15);

	sf::RectangleShape openFileRect;
	openFileRect.setFillColor(sf::Color::Cyan);
	openFileRect.setPosition(50, 440);
	openFileRect.setSize(sf::Vector2f(115, 50));

	sf::Text EffectText;
	EffectText.setFont(UIFont);
	EffectText.setString("Apply Effect");
	EffectText.setColor(sf::Color::Black);
	EffectText.setPosition(60, 510);
	EffectText.setCharacterSize(15);

	sf::RectangleShape EffectRect;
	EffectRect.setFillColor(sf::Color::Cyan);
	EffectRect.setPosition(50, 500);
	EffectRect.setSize(sf::Vector2f(145, 50));

	sf::RectangleShape FourierRect;
	FourierRect.setFillColor(sf::Color::Green);
	FourierRect.setPosition(700, 500);
	FourierRect.setSize(sf::Vector2f(25, 25));
	FourierRect.setOutlineThickness(5);
	FourierRect.setOutlineColor(sf::Color::Cyan);

	sf::Text FourierText;
	FourierText.setFont(UIFont);
	FourierText.setString("Frequency Domain");
	FourierText.setColor(sf::Color::White);
	FourierText.setPosition(750, 500);
	FourierText.setCharacterSize(15);


	sf::RectangleShape whistleRect;
	whistleRect.setFillColor(sf::Color::Cyan);
	whistleRect.setPosition(700, 440);
	whistleRect.setSize(sf::Vector2f(25, 25));
	whistleRect.setOutlineThickness(5);
	whistleRect.setOutlineColor(sf::Color::Cyan);

	sf::Text whistleText;
	whistleText.setFont(UIFont);
	whistleText.setString("Whistle Filter");
	whistleText.setColor(sf::Color::White);
	whistleText.setPosition(750, 440);
	whistleText.setCharacterSize(15);

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
						cout << endl << "- Creating and saving downsamples to program directory. -" << endl;

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

							EffectEnabled = true;
						}

					}

					//-- Load and apply an audio effect --//
					else if (EffectRect.getGlobalBounds().contains(mouse.x, mouse.y) && EffectEnabled == true)
					{

						nfdchar_t *outPath = NULL;
						nfdresult_t result = NFD_OpenDialog("wav,ogg", NULL, &outPath);

						//-- Make sure the directory path given is valid, or else the program will crash searching for a NULL directory --//
						if (outPath != NULL && outPath != "" && domain == "Frequency")
						{
							IRBuffer.clear();
							IRRawBuffer.loadFromFile(outPath);

							IRMemLoc = IRRawBuffer.getSamples();
							IRSampleCount = IRRawBuffer.getSampleCount();

							for (int i = 0; i < IRSampleCount; i++)
							{
								IRBuffer.push_back(*(IRMemLoc + i));
							}

							//-- Convert the Impulse Response Signal to Mono --//
							forceMono(IRBuffer);

							//-- Convert the original Signal (Contained in ModVector) to Mono --//

							SignalBuffer = modVector;

							forceMono(SignalBuffer);

							//-- Add the padding zeroes to both Audio Signals --//

							int requiredSize = SignalBuffer.size() + IRBuffer.size() - 1;
							int exponent = 0;

							while (requiredSize > pow(2.0, exponent))
							{
								exponent += 1;
							}

							requiredSize = pow(2.0, exponent);

							int SignalBufferZeroes = requiredSize - SignalBuffer.size();
							int IRBufferZeroes = requiredSize - IRBuffer.size();

							for (int i = 0; i < SignalBufferZeroes; i++)
							{
								SignalBuffer.push_back(0);
							}

							for (int i = 0; i < IRBufferZeroes; i++)
							{
								IRBuffer.push_back(0);
							}


							//-- Fill both of the "complex" variabe type Vectors --//
							SignalComplex.clear();
							IRComplex.clear();

							for (int i = 0; i < IRBuffer.size(); i++)
							{
								//-- We also normalize the values in the process using Cmath --//
								complex IRSample(IRBuffer[i] / pow(2.0, 16.0));
								complex SignalSample(SignalBuffer[i] / pow(2.0,16.0));

								IRComplex.push_back(IRSample);
								SignalComplex.push_back(SignalSample);
							}

							complex * FFTSignalComplex = new complex[SignalComplex.size()];
							complex * FFTIRComplex = new complex[IRComplex.size()];

							for (int i = 0; i < IRComplex.size(); i++)
							{
								*(FFTIRComplex + i) = IRComplex[i];
								*(FFTSignalComplex + i) = SignalComplex[i];
							}

							//-- Apply the Inplace Forward FFT on both signals. --//

							CFFT newCFFT;
							newCFFT.Forward(FFTIRComplex, IRComplex.size());
							newCFFT.Forward(FFTSignalComplex, SignalComplex.size());

							for (int i = 0; i < IRComplex.size(); i++)
							{
								IRComplex[i] = *(FFTIRComplex + i);
								SignalComplex[i] = *(FFTSignalComplex + i);
							}

							//-- Convolve both of the transformed Signals within the frequency domain --//

							outputComplex.clear();

							for (int i = 0; i < IRComplex.size(); i++)
							{
								double Real = IRComplex[i].re()*SignalComplex[i].re() - IRComplex[i].im()*SignalComplex[i].im();
								double Imaginary = IRComplex[i].re()*SignalComplex[i].im() + IRComplex[i].im()*SignalComplex[i].re();

								complex TempComplex(Real, Imaginary);

								outputComplex.push_back(TempComplex);
							}


							//-- Apply the inverse Fourier Transform to the Input --//

							newCFFT.Inverse(&(outputComplex[0]), outputComplex.size(), true);
							
							//-- Re-Fill the converted vector with De-Normalized values --//

							outputConverted.clear();
							modVector.clear();

							for (int i = 0; i < outputComplex.size(); i++)
							{
								modVector.push_back(sf::Int16(outputComplex[i].re() * pow(2.0, 16.0)));
							}

							//-- Play the convoluted signal --//
							intermediateBuffer.loadFromSamples(&(modVector[0]), modVector.size(), 1, 22050);
							cout << "- Applied the convolution Operation -" << endl;
						}

						//-- Convolution is performed within the Time Domain without utilizing the Fourier Transform --//
						else if (outPath != NULL && outPath != "" && domain == "Time")
						{
							IRBuffer.clear();
							IRRawBuffer.loadFromFile(outPath);

							IRMemLoc = IRRawBuffer.getSamples();
							IRSampleCount = IRRawBuffer.getSampleCount();

							for (int i = 0; i < IRSampleCount; i++)
							{
								IRBuffer.push_back(*(IRMemLoc + i));
							}

							//-- Convert the Impulse Response Signal to Mono --//
							forceMono(IRBuffer);

							//-- Convert the original Signal (Contained in ModVector) to Mono --//

							SignalBuffer = modVector;

							forceMono(SignalBuffer);

							//-- Normalize all the values in the Vectors --//
							vector<double> normalizedSource;
							vector<double> normalizedIR;
							normalizedSource.clear();
							normalizedIR.clear();


							for (int i = 0; i < SignalBuffer.size(); i++)
							{
								normalizedSource.push_back(SignalBuffer[i] / pow(2.0, 16.0));
							}

							for (int i = 0; i < IRBuffer.size(); i++)
							{
								normalizedIR.push_back(IRBuffer[i] / pow(2.0, 16.0));
							}

							//-- Nested for Loops for Vector element by element Multiplication --//
							vector<double> resultVector;

							for (int i = 0; i < normalizedSource.size(); i++)
							{
								resultVector.push_back(0);
								for (int j = 0; j < normalizedIR.size() + normalizedSource.size() - 1; j++)
								{
									if ((i - j >= 0) && (i + j < normalizedSource.size()))
									{
										resultVector[i] += normalizedSource[i - j] * normalizedIR[j];
									}

									else
									{
										//-- Do nothing --//
									}
									
								}
							}

							//-- Scale the resultVector values back to normal and write them to the modVector --//
							for (int i = 0; i < resultVector.size(); i++)
							{
								//cout << resultVector[i] * pow(2.0, 16.0) * pow(2.0, 16.0)<< endl;
							}
							modVector.clear();
							cout << resultVector.size() << endl;
							for (int i = 0; i < resultVector.size(); i++)
							{
								modVector.push_back(resultVector[i] * pow(2.0, 16.0));
							}

							intermediateBuffer.loadFromSamples(&(modVector[0]), modVector.size(), 1, 22050);
						}
					}

					if (FourierRect.getGlobalBounds().contains(mouse.x, mouse.y))
					{
						if (domain == "Time") 
						{ 
							domain = "Frequency";
							FourierRect.setFillColor(sf::Color::Green);
							FourierText.setString("Frequency Domain");
						}
						else 
						{ 
							domain = "Time";
							FourierRect.setFillColor(sf::Color::Red);
							FourierText.setString("Time Domain");
						}
					}

					else if (whistleRect.getGlobalBounds().contains(mouse.x, mouse.y) && EffectEnabled == true)
					{
						//-- Create a buffer for the audio to be filtered --//
						vector<sf::Int16> whistleBuffer;

						//-- Convert the original Signal (Contained in ModVector) to Mono --//

						whistleBuffer = modVector;

						forceMono(whistleBuffer);

						//-- Add the padding zeroes to the Audio Signal --//

						int requiredSize = whistleBuffer.size();
						int exponent = 0;

						while (requiredSize > pow(2.0, exponent))
						{
							exponent += 1;
						}

						requiredSize = pow(2.0, exponent);

						while (whistleBuffer.size() != requiredSize)
						{
							whistleBuffer.push_back(0);
						}

						
						complex * FFTComplexPtr = new complex[whistleBuffer.size()];

						for (int i = 0; i < whistleBuffer.size(); i++)
						{
							*(FFTComplexPtr + i) = complex(double(whistleBuffer[i]) / pow(2.0,16.0));
						}


						CFFT newFFT;
						newFFT.CustomForward(FFTComplexPtr, whistleBuffer.size());

						int stepSize = whistleBuffer.size() / 22050;

						float RC = 1.0 / (5000 * 2 * 3.14);
						float dt = 1.0 / 22050;
						float alpha = RC / (RC + dt);

						//-- Apply a High-Pass Filter to filter out the whistle in the frequency domain, greatly reduces speech also --//

						for (int i = 1; i < whistleBuffer.size(); i++)
						{
							if (i < whistleBuffer.size() / 15)
							{
								*(FFTComplexPtr + i) = complex(0);
							}

							else if (i > whistleBuffer.size() * 14 / 15)
							{
								*(FFTComplexPtr + i) = complex(0);
							}
						}

						newFFT.Inverse(FFTComplexPtr, whistleBuffer.size(), true);

						modVector.clear();

						vector<complex> outputComplex;
						for (int i = 0; i < whistleBuffer.size(); i++)
						{
							outputComplex.push_back(*(FFTComplexPtr + i));
							modVector.push_back(outputComplex[i].re() * pow(2.0, 16.0));
						}

						intermediateBuffer.loadFromSamples(&(modVector[0]), modVector.size(), 1, 22050);


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

				//-- Color change command. --//
				if (event.key.code == sf::Keyboard::C)
				{
					colorShift(lineVector);
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
					EffectEnabled = true;

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


		if (openFileRect.getGlobalBounds().contains(mouse.x, mouse.y))
		{
			openFileRect.setFillColor(sf::Color::White);
		}

		else
		{
			openFileRect.setFillColor(sf::Color::Cyan);
		}


		if (EffectRect.getGlobalBounds().contains(mouse.x, mouse.y))
		{
			if (EffectEnabled)
			{
				EffectRect.setFillColor(sf::Color::White);
			}
			else
			{
				EffectRect.setFillColor(sf::Color::Red);
			}

		}

		else
		{
			EffectRect.setFillColor(sf::Color::Cyan);
		}


		if (whistleRect.getGlobalBounds().contains(mouse.x, mouse.y))
		{
			whistleRect.setFillColor(sf::Color::White);
		}

		else
		{
			whistleRect.setFillColor(sf::Color::Cyan);
		}




		//-- This code hides the console window, MS-WINDOWS specific --//
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
		MainWindow.draw(EffectRect);
		MainWindow.draw(EffectText);
		MainWindow.draw(FourierRect);
		MainWindow.draw(FourierText);
		MainWindow.draw(whistleRect);
		MainWindow.draw(whistleText);

		for (int i = 0; i < lineVector.size(); i++)
		{
			MainWindow.draw(lineVector[i]);
		}

		MainWindow.display();

	}

	return 0;
}
