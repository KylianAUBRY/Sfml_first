
#include "../include/game.hpp"

Game::Game() {    
}

Game::~Game(){

}

void Game::setNumberOfAnts(int nb) {
    this->_numberOfAnts = nb;
}

int Game::getNumberOfAnts() {
    return this->_numberOfAnts;
}

Room::Room(int x, int y, std::string name) : pos(x, y), name(name) {}



Position::Position(float x, float y)
{
    this->x = x;
    this->y = y;
}

Room::Room() {}

Position::Position() {}