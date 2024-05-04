
#include "../include/main.h"
#include "../include/global_var.hpp"

Game game;

std::vector<Link> calculate_links();

float count_dist (Position p1, Position p2) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    return std::sqrt(dx * dx + dy * dy);
}

sf::View view(sf::FloatRect(0, 0, 300, 200));

void calculateTrianglePoints(const Position& p1, const Position& p2, Position& t1, Position& t2, Position& t3, Position& t4) {
    // Calculer la pente de la droite
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float len = count_dist(p1, p2);
    float nx = dx / len;
    float ny = dy / len;

    // Calculer les décalages pour les sommets des triangles
    float tx = -(SIZE_LINE / 2) * ny;
    float ty = (SIZE_LINE / 2) * nx;

    // Calculer les coordonnées des sommets des triangles
    t1 = { p1.x + tx, p1.y + ty };  // Sommet supérieur gauche
    t2 = { p1.x - tx, p1.y - ty };  // Sommet inférieur gauche
    t3 = { p2.x + tx, p2.y + ty };  // Sommet supérieur droit
    t4 = { p2.x - tx, p2.y - ty };  // Sommet inférieur droit
}


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
                circle.setPosition(it->second.pos.x * FACTEUR_PX - CIRCLE_SIZE, it->second.pos.y * FACTEUR_PX - CIRCLE_SIZE);
                rooms.push_back(circle);

                string labelText = it->second.name;
                int textSize = 20;
                if (it->second.name.size() >= 7)
                    labelText = it->second.name.substr(0, 6);
                if (labelText.size() > 4) {
                    textSize = static_cast<int>(20 - 2 * (labelText.size() - 4));
                    if (textSize < 6) {
                        textSize = 6; // Taille minimale de la police
                    }
                }
                text.setFont(font);
                text.setCharacterSize(textSize);
                text.setString(labelText);
                text.setFillColor(sf::Color(166, 166, 166));
                sf::FloatRect textBounds = text.getLocalBounds();
                text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
                text.setPosition(it->second.pos.x * FACTEUR_PX, it->second.pos.y * FACTEUR_PX);

                name_rooms.push_back(text);
            }
            

            all_links = calculate_links();
            for(std::vector<Link>::iterator it = all_links.begin(); it != all_links.end(); it++) {
                sf::VertexArray triangle(sf::TrianglesStrip, 4);
                bool is_triangle = false;
                Position p1;
                int i = 0;
                for (std::vector<Position>::iterator it2 = it->links.begin(); it2 != it->links.end(); it2++)
                {
                    it2->x *= FACTEUR_PX;
                    it2->y *= FACTEUR_PX;
                    if (!is_triangle)
                    {
                        p1 = *it2;
                        is_triangle = true;
                        continue ;
                    }
                    Position t1, t2, t3, t4;
                    calculateTrianglePoints(p1, *it2, t1, t2, t3, t4);
                    p1 = *it2;
                    if (i == 0)
                    {
                        triangle[0].position = sf::Vector2f(t1.x, t1.y);
                        triangle[1].position = sf::Vector2f(t2.x, t2.y);
                        triangle[2].position = sf::Vector2f(t3.x, t3.y);
                        triangle[3].position = sf::Vector2f(t4.x, t4.y);
                        i = 4;
                    }
                    else
                    {
                        triangle.resize(triangle.getVertexCount() + 4);
                        triangle[i].position = sf::Vector2f(t1.x, t1.y);
                        i++;
                        triangle[i].position = sf::Vector2f(t2.x, t2.y);
                        i++;
                        triangle[i].position = sf::Vector2f(t3.x, t3.y);
                        i++;
                        triangle[i].position = sf::Vector2f(t4.x, t4.y);
                        i++;
                    }
                }
                if (is_triangle)
                    strips.push_back(triangle); 
            }
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
        vector<Link> all_links;

        vector<sf::VertexArray> strips;


        sf::VertexArray m_vertices;
        sf::Texture m_tileset;

};

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

std::vector<Link> calculate_links() {
    std::vector<Link> all_links = sort_links();
    if (all_links.size() <= 0)
        exit(0) ;
    for(std::vector<Link>::iterator it = all_links.begin(); it != all_links.end(); it++) {
        int nb = nbIntersect(it->pos1, it->pos2, all_links);
        if (nb > 0) {
            cout << it->name1 << " " << it->name2 << " " << nb << endl;
        }
        else {
            it->links.push_back(it->pos1);
            it->links.push_back(it->pos2);
        }
    }

    return all_links;
}

int main() {
    if (fill_arg(game) == 1) {
        return -1;
    }

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