#include <cstdio>
#include <string>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "stdio.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Hack.h"

static float ScaleX = 0.7;
static float ScaleY = 0.7;
static int WindowWidth = 2560;
static int WindowHeight = 1440;

int main()
{

	FILE* fileptr = OpenFile();
	int error = Hacking (fileptr);
	if (error)
		return 1;

	fclose (fileptr);

	sf::RenderWindow window(sf::VideoMode(WindowWidth * ScaleX, WindowHeight * ScaleY), "VZLOM!");

	sf::Texture texture;
	if (!texture.loadFromFile("Beach.jpg", sf::IntRect(0, 0, WindowWidth, WindowHeight)))
		return EXIT_FAILURE;

	sf::Sprite sprite(texture);
	sprite.setScale(ScaleX, ScaleY);

	sf::Music music;
	setupMusic(&music);

	sf::Font font;
	font.loadFromFile ("Arial Unicode.ttf");

	sf::Text loadedText("Hacked. Press any button to close the window", font, 20);
	SetupText (&loadedText);

	ProgressBar progressBar;
	SetupProgBar (&progressBar);
	bool isLoaded = false;
	int firstIter = 0;


	while (window.isOpen())
	{
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			switch (Event.type)
			{
				case sf::Event::KeyReleased:
					if (isLoaded)
						window.close();

					break;
			}
		}


		window.clear();
		window.draw(sprite);

		if (isLoaded)
			window.draw(loadedText);

		if (!isLoaded && firstIter)
			drawProgBar(progressBar, window, &isLoaded);
		else 
			firstIter = 1;

		window.display();

	}

	return 0;
}

int drawProgBar(ProgressBar &progressBar, sf::RenderWindow &window,
            bool *programFinished)
{
    if (progressBar.progress < 1.f )
    {
	progressBar.bar.setSize(sf::Vector2f(progressBar.width * progressBar.progress, 30));
        window.draw(progressBar.bar);
	progressBar.progress += 0.1;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    else 
    {
    	*programFinished = true;
    }

    return 0;
}

int Hacking (FILE* filepointer)
{
	if (filepointer == nullptr)
		return 1;

	system("./figlet Vitek");

	char str[] = {0xEB, 0x24};

	fseek(filepointer, 0, SEEK_SET);
	fwrite (str, 1, sizeof (str), filepointer);
	return 0;
}

void SetupProgBar (ProgressBar* progressBar)
{
	progressBar->progress = 0;
	progressBar->width    = 500;
	progressBar->height   = 50;
	progressBar->colorR   = 250;
	progressBar->colorG   = 24;
	progressBar->colorB   = 127;
	progressBar->bar.setFillColor (sf::Color(progressBar->colorR, progressBar->colorG, progressBar->colorB));
	progressBar->bar.setPosition (WindowWidth * ScaleX / 2 - progressBar->width/2, WindowHeight * ScaleY - 100);
	progressBar->bar.setSize (sf::Vector2f(progressBar->width, progressBar->height));
}

void SetupText (sf::Text* loadedText)
{
	loadedText->setPosition(WindowWidth * ScaleX / 2 - 250, WindowHeight * ScaleY - 70);
	loadedText->setFillColor (sf::Color(251, 24, 148));
}

void setupMusic (sf::Music* music)
	
{
	if (!music->openFromFile("music.ogg"))
		return;

	music->play();
}

FILE* OpenFile ()
{
	char fileName[70] = {};
	int sizeOfInputStr = 0;
	printf ("Enter fileName\n");
	scanf ("%s%n", fileName, &sizeOfInputStr);
	if (sizeOfInputStr > 70)
	{
		fprintf (stderr, "buffer overflow\n");
		return nullptr;
	}

	FILE* fileptr = fopen (fileName, "ra");

	size_t numOfChars = fileSize(fileptr);
	char* progBuf = (char*) calloc(numOfChars, (*progBuf));
	readFile (fileptr, &progBuf);
	int error = countAndCheckHash(progBuf, numOfChars);
	if (error)
		return nullptr;

	return fileptr;
}

int countAndCheckHash (char* buff, size_t numberOfChars)
{
	size_t sum = 0;
	for (size_t i = 0; i < numberOfChars; ++i)
	{
		sum += buff[i];
	}

	if (sum != 4268)
		return 1;

	return 0;
}

size_t fileSize (FILE* file)
{
    fseek(file, 0l, SEEK_END); 
    size_t size = ftell(file); 
    fseek(file, 0l, SEEK_SET);

    return size;
}

int readFile(FILE* openedFile, char** dest)
{
    size_t numberOfChars = fileSize(openedFile);

//    printf("filesize = %lu\n", numberOfChars);

    *dest = (char*) calloc(numberOfChars + 1, sizeof(char));

    size_t charsReaded = fread((void*) *dest, sizeof(char), numberOfChars, openedFile); // 
    if (charsReaded != numberOfChars)
        return 1;
    
    return 0;
}
