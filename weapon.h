#ifndef WEAPON_H_INCLUDED
#define WEAPON_H_INCLUDED
#include <SFML/Graphics.hpp>
#include "bullet.h"

class Weapon{
    private:
        int wState, tipo;
        float angulo;
        sf::Texture ws;
        sf::Sprite gSpr;
        float tShoot;
        int dmg;

        sf::Texture muzzleEffect;
        sf::Sprite mfSpr;
        float timeEfect;
    public:
        Weapon(int);
        bool disparo;
        int velBalas;
        int maxBalas;
        float maxReloadTime, dispersion;
        bool recargando;
        void update(sf::Vector2f, float, float);
        float getTimeShoot();
        sf::Sprite getSpr();
        void drawWeapon(sf::RenderWindow *);
        void procesarEstado(float);
        void selecEffect(int);
        void setState(int);
        int getState();
        int getTipo();
        int getDmg();
};

#endif // WEAPON_H_INCLUDED
