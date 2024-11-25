#include "weapon.h"
#include <iostream>

Weapon::Weapon(int tG){
    muzzleEffect.loadFromFile("sprites/mf.png");
    mfSpr.setTexture(muzzleEffect);

    tipo = tG;
    timeEfect = 0;
    wState = 0;
    recargando = false;

    ws.loadFromFile("sprites/weapons.png");
    gSpr.setTexture(ws);
    switch(tG){
        case 0:
            gSpr.setTextureRect(sf::IntRect(0, 0, 100, 30));
            gSpr.setOrigin(10, 10);
            mfSpr.setOrigin(-15, 16);
            tShoot = 0.35;
            maxBalas = 13;
            maxReloadTime = 0.6;
            velBalas = 15;
            dispersion = 0.03;
            dmg = 15;
            break;
        case 1:
            gSpr.setTextureRect(sf::IntRect(100, 0, 100, 30));
            gSpr.setOrigin(30, 10);
            mfSpr.setOrigin(-50, 16);
            tShoot = 0.2;
            maxBalas = 30;
            maxReloadTime = 1;
            velBalas = 30;
            dispersion = 0.07;
            dmg = 30;
            break;
        case 2:
            gSpr.setTextureRect(sf::IntRect(200, 0, 100, 30));
            gSpr.setOrigin(30, 10);
            mfSpr.setOrigin(-55, 16);
            tShoot = 0.4;
            maxBalas = 20;
            maxReloadTime = 1.3;
            velBalas = 40;
            dispersion = 0.03;
            dmg = 40;
            break;
        case 3:
            gSpr.setTextureRect(sf::IntRect(300, 0, 100, 30));
            gSpr.setOrigin(20, 8);
            mfSpr.setOrigin(-25, 16);
            tShoot = 0.07;
            maxBalas = 15;
            maxReloadTime = 0.7;
            velBalas = 30;
            dispersion = 0.15;
            dmg = 20;
            break;
        case 4:
            gSpr.setTextureRect(sf::IntRect(400, 0, 100, 30));
            gSpr.setOrigin(30, 10);
            mfSpr.setOrigin(-55, 16);
            tShoot = 1;
            maxBalas = 5;
            maxReloadTime = 1.7;
            velBalas = 50;
            dispersion = 0;
            dmg = 100;
            break;
    }
}

sf::Sprite Weapon::getSpr(){
    return gSpr;
}

void Weapon::setState(int var){
    wState = var;
}

int Weapon::getState(){
    return wState;
}

float Weapon::getTimeShoot(){
    return tShoot;
}

void Weapon::update(sf::Vector2f playerPos, float _angulo, float time){
    if(wState!=0)
        disparo=1;
    else
        disparo=0;
    angulo = _angulo;
    gSpr.setPosition(playerPos);
    gSpr.setRotation(angulo);
    if(angulo>90 || angulo<-90)
        gSpr.setScale(0.8, -0.8);
    else
        gSpr.setScale(0.8, 0.8);
    if(recargando)
        gSpr.setColor(sf::Color::Red);
    else
        gSpr.setColor(sf::Color::White);
    mfSpr.setPosition(gSpr.getPosition());
    procesarEstado(time);
}

void Weapon::drawWeapon(sf::RenderWindow *window){
    window->draw(gSpr);
    if(timeEfect>0 && !recargando){
        window->draw(mfSpr);
    }
}

void Weapon::selecEffect(int frame){
    mfSpr.setTextureRect(sf::IntRect(0, (muzzleEffect.getSize().y/5)*frame, muzzleEffect.getSize().x, muzzleEffect.getSize().y/5));
}

void Weapon::procesarEstado(float time){
    switch(wState){
        case 0:
            break;
        case 1:
            timeEfect=0.05;
            mfSpr.setRotation(angulo);
            wState=2;
            break;
        case 2:
            if(timeEfect>0){
                timeEfect-=time;
            }else wState=0;
            break;
    }
}

int Weapon::getTipo(){
    return tipo;
}
int Weapon::getDmg(){
    return dmg;
}
