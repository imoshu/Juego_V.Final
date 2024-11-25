#ifndef F_H_INCLUDED
#define F_H_INCLUDED
#include "animation.h"
#include "datagame.h"

class BasicStructure{
    protected:
        int health, damage, velocityMove, state;
        sf::Vector2f Position;
    public:
        void setPosition(sf::Vector2f pos);
        sf::Vector2f getPosition();
};

struct Tile{
    int tipo;
    int id;
    int objeto;
    bool visitado;
    sf::Vector2u position;
};

struct Room{
    int posX, posY, ancho, alto;
};

#endif // F_H_INCLUDED
