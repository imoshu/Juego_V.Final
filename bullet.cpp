#include "bullet.h"

Bullet::Bullet(sf::Vector2f aim, int mVel){
    b.loadFromFile("sprites/bullet.png");
    spr.setTexture(b);
    spr.setOrigin(0.f, 1.f);
    actVel.x = aim.x * mVel;
    actVel.y = aim.y * mVel;
}

void Bullet::setPosition(sf::Vector2f pos){
    spr.setPosition(pos);
}

sf::Sprite Bullet::getSpr(){
    return spr;
}

void Bullet::mover(){
    spr.move(actVel);
}

void Bullet::setAngle(float angle){
    /*const float PI = 3.14159265;  (sf::RenderWindow *window, sf::Vector2f jPos)
    sf::Vector2i m = sf::Mouse::getPosition(*window);
    m.x -= (window->getSize().x/2);
    m.y -= (window->getSize().y/2);
    float angle = (atan2(m.y - spr.getPosition().y, m.x - spr.getPosition().x)) * 180 / PI;
    */
    spr.setRotation(angle);
}
