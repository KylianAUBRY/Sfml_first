
#include "include.hpp"
#include "game.hpp"
#include "global_var.hpp"

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
            case (sf::Event::Closed):
                window.close();
                break ;
        }
    }
}

bool is_comment(std::string line) {
    int j = 0;
    while (line[j] == ' ')
            j++;
    if (!line[j] || (line[j] == '#' && (line.find("##start") != j && line.find("##end") != j)))
        return true ;
    return false;
}

bool is_number(std::string line) {
    int i = 0;
    while (line[i] == '0')
        i++;
    int j = i;
    for(i; i < line.length(); i++) {
        if (!isdigit(line[i]) || j - i >= 8)
            return false;
    }
    return true;
}

int is_room(std::string line)
{
    std::string arg2 = "";
    std::string arg3 = "";
    int i = 0;
    int count = 0;
    while (line[i] == ' ')
        i++;
    if (line[i] == 'L' || line[i] == '#')
        return 2;
    for (i; i < line.length(); i++) {
        if (line[i] == ' ')
        {
            while (line[i + 1] == ' ')
                i++;
            if (!line[i + 1])
                break ;
            count++;
            continue ;
        }
        if (line[i] == '-')
            return 1;
        if (count == 1)
            arg2 += line[i];
        else if (count == 2)
            arg3 += line[i];
    }
    if (count != 2 || !is_number(arg2) || !is_number(arg3))
        return 2;
    return 0;
}

int fill_arg(Game &game) {
    std::string start = "";
    std::string end = "";
    int i = 0;
    std::string line;
    std::map<std::string, std::string> stock;
    while (1) {
        std::getline(std::cin, line);
        if (cin.eof())
            break ;
        if (line.empty() || is_comment(line))
            continue ;
        if (i == 0) {
            if (!is_number(line))
                return 1;
            game.setNumberOfAnts(stoi(line));
            i++;
            continue ;
        }
        else if (i == 1)  {
            int code = is_room(line);
            if (code == 1)
                i++;
            else if (code == 2)
                return -1;
            else {
                istringstream split(line);
                string tmp;
                string tmp1;
                string tmp2;
                split >> tmp;
                split >> tmp1;
                split >> tmp2;
                stock[tmp] = tmp1 + "-" + tmp2;
            }
        }
        if (i == 2) {

        }
        cout << "line :" << line << endl;
    } 
    return 0;
}

int main()
{
    Game game;
    if (fill_arg(game) == 1)
    {
        cerr << "Error map" << endl;
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