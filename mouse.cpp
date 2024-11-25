#include "mouse.h"
#include <math.h>
#include <iostream>
Mouse::Mouse(sf::RenderWindow *vent){
    p.loadFromFile("sprites/aim.png");
    aim.setTexture(p);
    aim.setOrigin(32.f, 32.f);
    aim.setScale(0.5, 0.5);
    window = vent;
}

sf::Sprite Mouse::getSpr(){
    return aim;
}

void Mouse::updateMouse(){
    mousePosWindow = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
    aim.setPosition(mousePosWindow);
/*
    sf::Vector2f aimDir = sf::Vector2f(sf::Mouse::getPosition(*window));
    aimDir.x -= window->getSize().x/2;
    aimDir.y -= window->getSize().y/2;
    aimDirNorm = aimDir / (static_cast<float>(sqrt(pow(aimDir.x, 2)+pow(aimDir.y, 2))));*/
}
/*
sf::Vector2f Mouse::getMouse(){

    return aimDirNorm;
}
*/
sf::Vector2f Mouse::getMousePos(){
    return mousePosWindow;
}

