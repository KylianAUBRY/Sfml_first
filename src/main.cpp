
#include "../include/main.h"
#include "../include/global_var.hpp"

Game game;

class MyEntity : public sf::Drawable
{
    public :
        bool load(sf::Font &font){
            for (std::map<std::string, Room>::const_iterator it = game.rooms.begin(); it != game.rooms.end(); ++it) {
                sf::CircleShape circle;
                sf::Text text;

                circle.setRadius(CIRCLE_SIZE);
                circle.setFillColor(sf::Color(53, 53, 53));
                circle.setOutlineThickness(CIRCLE_OUTLINE_SIZE);
                circle.setOutlineColor(sf::Color::Black);
                circle.setPosition(it->second.pos.x * FACTEUR_PX, it->second.pos.y * FACTEUR_PX);
                rooms.push_back(circle);

                text.setFont(font);
                text.setString(it->second.name);
                text.setFillColor(sf::Color(166, 166, 166));
                text.setPosition(it->second.pos.x * FACTEUR_PX + ((CIRCLE_OUTLINE_SIZE) / 2), it->second.pos.y * FACTEUR_PX  + ((CIRCLE_SIZE  / 2 + CIRCLE_OUTLINE_SIZE) / 2));
                if (it->second.name.length() > 4)
                {
                    text.setPosition(it->second.pos.x * FACTEUR_PX + ((CIRCLE_OUTLINE_SIZE) / 2), it->second.pos.y * FACTEUR_PX  + ((CIRCLE_SIZE+ CIRCLE_OUTLINE_SIZE) / 2));
                    text.setCharacterSize(20 - it->second.name.length());
                    if (it->second.name.length() > 7)
                    {
                        text.setCharacterSize(20 - 11);
                        text.setString(it->second.name.substr(0, 7));
                    }
                }
                else
                    text.setCharacterSize(20);
                name_rooms.push_back(text);
            }
            sf::VertexArray triangle(sf::TrianglesStrip, 3);
            triangle[0].position = sf::Vector2f(500, 503);
            triangle[1].position = sf::Vector2f(500, 498);
            triangle[2].position = sf::Vector2f(1000, 503);
            
            triangle.resize(triangle.getVertexCount() + 1);
            triangle[3].position = sf::Vector2f(1000, 498);
            strips.push_back(triangle);
            // strips
            return true;
        }
        
    private:

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            for (const sf::VertexArray& line : strips) {
                target.draw(line, states);
            }
            for (const sf::CircleShape& circle : rooms) {
                target.draw(circle, states);
            }
            // Dessiner chaque texte sur la cible
            for (const sf::Text& text : name_rooms) {
                target.draw(text, states);
            }
            
            
        }

        vector<sf::CircleShape> rooms;
        vector<sf::Text> name_rooms;

        vector<sf::VertexArray> strips;


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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        view.move(0, -SPEED_MOOV);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        view.move(0, SPEED_MOOV);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        view.move(SPEED_MOOV, 0);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        view.move(-SPEED_MOOV, 0);
}

void event_detection()
{
    sf::Event event;
    sf::Vector2f viewSize;
    
    while (window.pollEvent(event)) {
        switch (event.type) {
            case (sf::Event::Closed):
                window.close();
                break ;
            case (sf::Event::KeyPressed) :
                switch (event.key.code) {
                    case (73) :
                        viewSize = view.getSize();
                        view.setSize(viewSize.x + ZOOM_SIZE, viewSize.y + ZOOM_SIZE);

                        break;
                    case (74) :
                        viewSize = view.getSize();

                        if (viewSize.x <= 140 && viewSize.y <= 40)
                            break ;
                        view.setSize(viewSize.x - ZOOM_SIZE, viewSize.y - ZOOM_SIZE);
                        break ;
                }
                break ;
        }
    }
}

void setView(sf::View &view2)
{
    sf::Vector2f viewSize = view.getSize();
    int maxX = 0;
    int maxY = 0;
    int minX = -1;
    int minY = -1;
    for (std::map<std::string, Room>::const_iterator it = game.rooms.begin(); it != game.rooms.end(); ++it) {
        if(it->second.pos.x > maxX)
            maxX = it->second.pos.x;
        if (minX == -1 || minX > it->second.pos.x)
            minX = it->second.pos.x;
        if (it->second.pos.y > maxY)
            maxY = it->second.pos.y;
        if (minY == -1 || minY > it->second.pos.y)
            minY = it->second.pos.y;
    }
    maxX ++;
    maxY ++;
    if (maxX - minX >= maxY - minY)
        view.setSize((maxX - minX) * FACTEUR_PX + 5, (maxX - minX) * FACTEUR_PX + 5);
    else 
        view.setSize((maxY - minY) * FACTEUR_PX + 5, (maxY - minY) * FACTEUR_PX + 5);
    view.setCenter(((((maxX - minX) / 2 ) + minX) * FACTEUR_PX) + CIRCLE_SIZE, ((((maxY - minY) / 2) + minY) * FACTEUR_PX) + CIRCLE_SIZE);
    view2.setViewport(sf::FloatRect(0.85f, 0, 0.25f, 0.25f));
    view.setViewport(sf::FloatRect(0, 0, 1, 1));
}

int main()
{
    if (fill_arg(game) == 1)
    {
        return -1;
    }
    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf"))
    {
        cerr << "ERROR :" << endl << "Unable to load arial.ttf asset" << endl;
        return -1;
    }
    window.create(sf::VideoMode(800, 600), "My window", sf::Style::Close | sf::Style::Resize);
    window.setVerticalSyncEnabled(true); //active la synchronisaation verticale;

    sf::View view2;
    setView(view2);
    
    MyEntity entity;
    entity.load(font);

    sf::Text text;
    text.setFont(font); // font est un sf::Font
    text.setFillColor(sf::Color::Red);
    text.setCharacterSize(100); // exprimée en pixels, pas en points !

    
    while (window.isOpen())
    {
        event_detection(); //sert a detecter tout les evenement survenue;
        keyboard_detection();

        window.clear(sf::Color(26, 43, 56));

        window.setView(view);
        window.draw(entity);

        sf::Vector2f viewCenter = view.getCenter();
        text.setString(to_string(static_cast<int>(viewCenter.x)) + " " + to_string(static_cast<int>(viewCenter.y)));
        
        window.setView(view2);
        window.draw(text);

        window.display();
    }
    return 0;
}