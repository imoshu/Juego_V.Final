#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED
#include "dataType.h"
#include <SFML/Graphics.hpp>

class Chest{
    private:
        int tipo;// el peor-> 4, 5, 6, 7 ->el mejor
        int drop;
        void setChest(int t=0);
    public:
        Chest(int f, int c);
        Chest(int f, int c, int t);
        bool estado;
        sf::Vector2u pos;
        int chestDrop();
        int getTipo();
        void disable();
};

#endif // OBJECT_H_INCLUDED
