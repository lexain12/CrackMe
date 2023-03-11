#include "stdio.h"
#include "stdlib.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

void Hacking (FILE* filepointer);

int main()
{
	char fileName[70] = {};
	int sizeOfInputStr = 0;
	printf ("Enter fileName\n");
	scanf ("%s%n", fileName, &sizeOfInputStr);
	if (sizeOfInputStr > 39)
	{
		fprintf (stderr, "buffef overflow\n");
		return EXIT_FAILURE;
	}

	FILE* fileptr = fopen (fileName, "r+");
	Hacking (fileptr);
	fclose (fileptr);



	sf::RenderWindow window(sf::VideoMode(1000, 1000), "VZLOM!");

	sf::Texture texture;
	if (!texture.loadFromFile("Illuminat.jpg", sf::IntRect(0, 0, 2000, 2000)))
		return EXIT_FAILURE;

	sf::Sprite sprite(texture);
	sprite.setScale(0.5, 0.5);

	sf::RectangleShape progressBar;
	progressBar.setFillColor (sf::Color(214, 140, 28));
	progressBar.setPosition (250, 800);
	int BarSize = 0;
	size_t isLoaded = 0;

	sf::Clock clock;

	sf::Font font;
	font.loadFromFile ("Arial Unicode.ttf");
	sf::Text loadedText("Hacked. Press any button to close the window", font, 20);
	loadedText.setPosition(250, 830);
	loadedText.setFillColor (sf::Color(214, 140, 28));

	while (window.isOpen())
	{
		sf::Time elapsed = clock.getElapsedTime();

		sf::Event Event;
		while (window.pollEvent(Event))
		{
			switch (Event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;

				case sf::Event::KeyReleased:
					if (isLoaded)
						window.close();
					break;

			}
		}

		if (BarSize < 10)
			BarSize = int (elapsed.asSeconds());
		else 
		{
			isLoaded = 1;
		}

		progressBar.setSize (sf::Vector2f (BarSize * 50, 30));

		window.clear();
		window.draw(sprite);
		window.draw(progressBar);

		if (isLoaded)
			window.draw(loadedText);

		window.display();
	}

	return 0;
}

void Hacking (FILE* filepointer)
{
	char str[] = {0xEB, 0x24};

	fwrite (str, 1, sizeof (str), filepointer);
}

