
#include "../include/main.h"
#include "../include/global_var.hpp"

Game game;

class MyEntity : public sf::Drawable {
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

void keyboard_detection() {
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

void event_detection() {
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

void setView(sf::View &view2) {
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

class Link {
    public :
        std::string name1;
        Position pos1;
        std::string name2;
        Position pos2;
        std::vector<Position> links;
        float dist;
};

float count_dist (Position p1, Position p2) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    return std::sqrt(dx * dx + dy * dy);
}

bool link_exists(std::vector<Link> all_links, std::string name1, std::string name2) {
    if (all_links.size() <= 0)
        return false;
    for(std::vector<Link>::iterator it = all_links.begin(); it != all_links.end(); it++)
    {
        if ((it->name1 == name1 || it->name1 == name2) && (it->name2 == name1 || it->name2 == name2))
            return true;
    }
    return false;
}

std::vector<Link> sort_links() {
    std::vector<Link> all_links;
    Link min;
    bool if_min = false;

    while(1) {
        for (std::map<std::string, Room>::iterator it = game.rooms.begin(); it != game.rooms.end(); ++it) {
            for(std::vector<Room>::iterator it_room = it->second.links.begin(); it_room != it->second.links.end(); it_room++)
            {
                if ((!if_min || min.dist > count_dist(it->second.pos, it_room->pos)) && !link_exists(all_links, it->second.name, it_room->name))
                {
                    if_min = true;
                    min.name1 = it->second.name;
                    min.name2 = it_room->name;
                    min.pos1 = it->second.pos;
                    min.pos2 = it_room->pos;
                    min.dist = count_dist(it->second.pos, it_room->pos);
                }
            }
        }
        if (!if_min)
            break ;
        all_links.push_back(min);
        if_min = false;
    }
    return all_links;
}

int nbIntersect(Position A, Position B, std::vector<Link>& links) {
    int nb = 0;
    Position lastPos;
    bool is_lastPos = false;

    for(std::vector<Link>::iterator it = links.begin(); it != links.end(); it++) {
        is_lastPos = false;
        for (std::vector<Position>::iterator link = it->links.begin(); link != it->links.end(); link++) {
            if (!is_lastPos) {
                lastPos = *link;
                is_lastPos = true;
                continue ;
            }
            float cross1 = (link->x - lastPos.x) * (A.y - lastPos.y) - (link->y - lastPos.y) * (A.x - lastPos.x);
            float cross2 = (link->x - lastPos.x) * (B.y - lastPos.y) - (link->y - lastPos.y) * (B.x - lastPos.x);
            float cross3 = (B.x - A.x) * (A.y - lastPos.y) - (B.y - A.y) * (A.x - lastPos.x);
            float cross4 = (B.x - A.x) * (A.y - link->y) - (B.y - A.y) * (A.x - link->x);

            if (((cross1 > 0 && cross2 < 0) || (cross1 < 0 && cross2 > 0)) &&
                ((cross3 > 0 && cross4 < 0) || (cross3 < 0 && cross4 > 0)))
                nb ++;
            lastPos = *link;
        }
    }
    return nb;
}

void calculate_links() {
    std::vector<Link> all_links = sort_links();
    if (all_links.size() <= 0)
        exit(0) ;
    for(std::vector<Link>::iterator it = all_links.begin(); it != all_links.end(); it++) {
        int nb = nbIntersect(it->pos1, it->pos2, all_links);
        if (nb > 0)
        {
            cout << it->name1 << " " << it->name2 << " " << nb << endl;
        }
        it->links.push_back(it->pos1);
        it->links.push_back(it->pos2);
    }
}

int main() {
    if (fill_arg(game) == 1) {
        return -1;
    }

    calculate_links();


    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf")) {
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

    
    while (window.isOpen()) {
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