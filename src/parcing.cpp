
#include "../include/main.h"

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
    for(; i < line.length(); i++) {
        if (j - i >= 8)
            return false;
        if (!isdigit(line[i]))
        {
            while (line[i] == ' ')
                i++;
            if (line[i])
                return false;
            return true;
        }
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
    for (; i < line.length(); i++) {
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
    while (1) {
        std::getline(std::cin, line);
        if (cin.eof())
            break ;
        if (line.empty() || is_comment(line))
            continue ;
        if (i == 0) {
            if (!is_number(line)) {
                cerr << "ERROR :" << endl << "The number of ants is not a number." << endl;
                return 1;
            }
            if (stoi(line) <= 0) {
                cerr << "ERROR :" << endl << "There must be at least one ant" << endl;
                return 1;
            }
            game.setNumberOfAnts(stoi(line));
            cout << "is number" << endl ;
            i++;
            continue ;
        }
        else if (i == 1)  {
            int code = is_room(line);
            if (code == 1){
                i++;
                cout << "is room" << endl;
            }
            else if (code == 2) {
                cerr << "ERROR :" << endl << "Room problem." << endl;
                return 1;
            }
            else {
                istringstream split(line);
                string name;
                string cord1;
                string cord2;
                split >> name;
                split >> cord1;
                split >> cord2;
                for (std::map<std::string, Room>::const_iterator it = game.rooms.begin(); it != game.rooms.end(); ++it) {
                    if (it->first == name) {
                        cerr << "ERROR :" << endl << "Duplicate name." << endl;
                        return 1;
                    }
                    if (it->second.pos.x == stoi(cord1) && it->second.pos.y == stoi(cord2)){
                        cerr << "ERROR :" << endl << "Duplicate coordinate." << endl;
                        return 1;
                    }
                }
                game.rooms[name] = Room(stoi(cord1), stoi(cord2), name);
            }
        }
        if (i == 2) {
            size_t pos = line.find('-');
            if(pos != string::npos) {
                string name1 = line.substr(0, pos);
                string name2 = line.substr(pos + 1);
                std::map<std::string, Room>::iterator it_name1 = game.rooms.end();
                std::map<std::string, Room>::iterator it_name2 = game.rooms.end();
                for (std::map<std::string, Room>::iterator it = game.rooms.begin(); it != game.rooms.end(); ++it) {
                    if (it->first == name1) {
                        it_name1 = it;
                    }
                    if (it->first == name2) {
                        it_name2 = it;
                    }
                }
                if (it_name1 == game.rooms.end() || it_name2 == game.rooms.end()) {
                    cerr << "ERROR :" << endl << "The names of the links are not correct." << endl;
                    return 1;
                }
                it_name1->second.links.push_back(it_name2->second);
            }
        }
    }
    return 0;
}