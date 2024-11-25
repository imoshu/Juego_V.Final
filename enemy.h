#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED
#include <SFML/Graphics.hpp>
#include "player.h"
#include "dataType.h"
#include "map.h"

class Enemy: public BasicStructure{
    protected:
        sf::Texture e;
        sf::Sprite spr;
        //int vida, dmg, velMov, estado;
        int grid;
        bool atacar;
        float atqTime;
        sf::Vector2u posInMap;
        int visionDist, dirrecion, ms, antDir;
        Animation* anim;
        float daniado;
    public:
        virtual void Update(const float, Player*, Map*);
        int tipo;
        sf::Sprite getSpr();
        void drawEnemy(sf::RenderWindow *);
        void setPosInMap(int, int);
        sf::Vector2u getPosInMap();
        int enColision(Map*);
        int elegirDir(int);
        void move();
        void ataco();
        int getVida();
};

class EnemyTipoA: public Enemy{
    public:
        EnemyTipoA(int, int);
        void Update(const float, Player*, Map*);
};

class EnemyTipoB: public Enemy{
    public:
        Bullet* bala;
        EnemyTipoB(int, int);
        void Update(const float, Player*, Map*);
};
#endif // ENEMY_H_INCLUDED
