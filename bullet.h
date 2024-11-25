#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED
#include <SFML/Graphics.hpp>

class Bullet{
    private:
        sf::Texture b;
        sf::Sprite spr;
        sf::Vector2f actVel;
    public:
        Bullet(sf::Vector2f, int);
        sf::Sprite getSpr();
        void setPosition(sf::Vector2f);
        void mover();
        void setAngle(float);
};

#endif // BULLET_H_INCLUDED
