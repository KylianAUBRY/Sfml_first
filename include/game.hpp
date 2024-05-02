#pragma once
# include "include.hpp"

class Position {
    public :
        Position();
        Position(int x, int y);
        int x;
        int y;
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
