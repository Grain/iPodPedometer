#include <SFML/Graphics.hpp>
#include <cstdio>
#include <iostream>
#include "calendar.h"

int main()
{
    Calendar calendar;

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Test", sf::Style::Close);
    window.setFramerateLimit(30);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Key::Left)
                    {
                        calendar.previousMonth();
                    }
                    if (event.key.code == sf::Keyboard::Key::Right)
                    {
                        calendar.nextMonth();
                    }
                default:
                    break;
            }
        }
        window.clear(sf::Color::White);

        calendar.draw(&window);

        window.display();
    }

    return 0;
}
