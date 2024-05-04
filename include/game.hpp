#pragma once
# include "include.hpp"

class Position {
    public :
        Position();
        Position(float x, float y);
        float x;
        float y;
};

class Link {
    public :
        std::string name1;
        Position pos1;
        std::string name2;
        Position pos2;
        std::vector<Position> links;
        float dist;
};


class Room {
    public :
        Room();
        Room(int x, int y, std::string name);
        Position pos;
        std::string name;
        std::vector <Room> links;
};


class Game {
    public :
        Game();
        ~Game();
        void setNumberOfAnts(int nb);
        int getNumberOfAnts();
        std::map<std::string, Room> rooms;
    private :
        int _numberOfAnts;
};
