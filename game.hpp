#pragma once
# include "include.hpp"

class Game {
    public :
        Game();
        ~Game();
        void setNumberOfAnts(int nb);
        int getNumberOfAnts();
    private :
        int _numberOfAnts;
};
