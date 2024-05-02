
#include "../include/main.h"
#include "../include/global_var.hpp"


class MyEntity : public sf::Drawable
{
    public :
        bool load(unsigned int width, unsigned int height){
            m_vertices.setPrimitiveType(sf::Quads);
            m_vertices.resize(width * height * 4);
            sf::Vertex* quad = &m_vertices[0];

            // on le définit comme un rectangle, placé en (10, 10) et de taille 100x100
            quad[0].position = sf::Vector2f(10, 10);
            quad[1].position = sf::Vector2f(110, 10);
            quad[2].position = sf::Vector2f(110, 110);
            quad[3].position = sf::Vector2f(10, 110);
            return true;
        }
        
    private:

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            target.draw(m_vertices, states);
        }

        sf::VertexArray m_vertices;
        sf::Texture m_tileset;

};


sf::View view(sf::FloatRect(0, 0, 300, 200));

void keyboard_detection()
{
    if (!window.hasFocus()) //si la fenettre n'est pas au premier plan 
        return ;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        window.close();
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        view.move(0, -5);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        view.move(0, 5);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        view.move(5, 0);
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        view.move(-5, 0);
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
    // Game game;
    // if (fill_arg(game) == 1)
    // {
    //     return -1;
    // }
    window.create(sf::VideoMode(800, 600), "My window", sf::Style::Close | sf::Style::Resize);
    
    window.setVerticalSyncEnabled(true); //active la synchronisaation verticale;

    MyEntity entity;
    entity.load(800, 600);



    // view.setRotation(20);
    // view.rotate(5);
    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf"))
    {
        // erreur...
    }

    sf::Text text;
    text.setFont(font); // font est un sf::Font
    text.setFillColor(sf::Color::Red);
    text.setCharacterSize(100); // exprimée en pixels, pas en points !


    // view.setViewport(sf::FloatRect(0, 0, 1, 1));
    sf::View view2;
    view2.setViewport(sf::FloatRect(0.85f, 0, 0.25f, 0.25f));
    view.setViewport(sf::FloatRect(0, 0, 1, 1));
    while (window.isOpen())
    {
        event_detection(); //sert a detecter tout les evenement survenue;
        keyboard_detection();

        window.clear(sf::Color::Black);

        // sf::CircleShape shape(50);
        window.setView(view);
        window.draw(entity);

        window.setView(view2);
        window.draw(text);
       
       sf::Vector2f viewCenter = view.getCenter();
       text.setString(to_string(static_cast<int>(viewCenter.x)) + " " + to_string(static_cast<int>(viewCenter.y)));
        // window.draw(text);
    
        window.display();
    }
    return 0;
}