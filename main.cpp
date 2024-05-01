
#include "main.h"
#include "global_var.hpp"

void keyboard_detection()
{
    if (!window.hasFocus()) //si la fenettre n'est pas au premier plan 
        return ;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        window.close();
}

void event_detection()
{
    sf::Event event;
    
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case (sf::Event::Closed):
                window.close();
                break ;
        }
    }
}

int main()
{
    Game game;
    if (fill_arg(game) == 1)
    {
        return -1;
    }
    window.create(sf::VideoMode(800, 600), "My window", sf::Style::Close | sf::Style::Resize);
    
    window.setVerticalSyncEnabled(true); //active la synchronisaation verticale;

    while (window.isOpen())
    {
        event_detection(); //sert a detecter tout les evenement survenue;
        keyboard_detection();

        window.clear(sf::Color::Black);

        // window.draw();

        window.display();
    }
    return 0;
}