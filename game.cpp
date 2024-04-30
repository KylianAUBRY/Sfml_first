
#include "game.hpp"

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