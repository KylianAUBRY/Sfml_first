#include "include.hpp"

// int main()
// {
//     sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
//     sf::CircleShape shape(100.f);
//     shape.setFillColor(sf::Color::Green);

//     while (window.isOpen())
//     {
//         sf::Event event;
//         while (window.pollEvent(event))
//         {
//             if (event.type == sf::Event::Closed)
//                 window.close();
//         }

//         window.clear();
//         window.draw(shape);
//         window.display();
//     }

//     return 0;
// }

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
            case (sf::Event::Closed ) :
                window.close();
                break ;
        }
    }
}

int main()
{

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