#include "enemy.h"
#include <iostream>
/*
mov x pixel(mov)

veces = 64/mov
tiempo de movimiento = 1/60 * veces

DISPARO DEL ENEMIGO

bala = new Bullet(Global::aimDirNorm(spr.getPosition(), Global::currentWindow->mapCoordsToPixel(jg->getPayerSpr().getPosition())), 30);
bala->setPosition(spr.getPosition());

Global::currentWindow->draw(bala->getSpr());
Global::currentWindow->display();
bala->mover();

*/

void Enemy::drawEnemy(sf::RenderWindow *f){
    f->draw(spr);
}

sf::Sprite Enemy::getSpr(){
    return spr;
}

int Enemy::enColision(Map * mapa){
    int x = posInMap.x;
    int y = posInMap.y;
    switch(dirrecion){
    case 0:
        if(mapa->getTileInGrid(grid, sf::Vector2u(x-1, y)).tipo!=1)
            return 1;
        break;
    case 1:
        if(mapa->getTileInGrid(grid, sf::Vector2u(x-1, y-1)).tipo!=1)
            return 1;
        break;
    case 2:
        if(mapa->getTileInGrid(grid, sf::Vector2u(x, y-1)).tipo!=1)
            return 1;
        break;
    case 3:
        if(mapa->getTileInGrid(grid, sf::Vector2u(x+1, y-1)).tipo!=1)
            return 1;
        break;
    case 4:
        if(mapa->getTileInGrid(grid, sf::Vector2u(x+1, y)).tipo!=1)
            return 1;
        break;
    case 5:
        if(mapa->getTileInGrid(grid, sf::Vector2u(x+1, y+1)).tipo!=1)
            return 1;
break;
    case 6:
        if(mapa->getTileInGrid(grid, sf::Vector2u(x, y+1)).tipo!=1)
            return 1;
        break;
    case 7:
        if(mapa->getTileInGrid(grid, sf::Vector2u(x-1, y+1)).tipo!=1)
            return 1;
        break;
    }
    return 0;
}

void Enemy::move(){
    switch(dirrecion){
        case 0:
            spr.move(-velocityMove, 0);
            break;
        case 1:
            spr.move(-velocityMove, -velocityMove);
            break;
        case 2:
            spr.move(0, -velocityMove);
            break;
        case 3:
            spr.move(velocityMove, -velocityMove);
            break;
        case 4:
            spr.move(velocityMove, 0);
            break;
        case 5:
            spr.move(velocityMove, velocityMove);
            break;
        case 6:
            spr.move(0, velocityMove);
            break;
        case 7:
            spr.move(-velocityMove, velocityMove);
            break;

    }
}

void Enemy::setPosInMap(int f, int c){
    posInMap.y = f;
    posInMap.x = c;
}

sf::Vector2u Enemy::getPosInMap(){
    return posInMap;
}

int Enemy::elegirDir(int dir){
    int opc = rand()%3;
    switch(opc){
        case 0:
            if(dir == 0)
                return 7;
            else
                dir--;
            return dir;
            break;
        case 1:
            return dir;
            break;
        case 2:
            if(dir == 7)
                return 0;
            else
                dir++;
            return dir;
            break;
    }
}

void Enemy::Update(const float _Time, Player * jg, Map *mapa){
    if(health>0){
        if(daniado>0.05) spr.setColor(sf::Color::White);
        else{
            spr.setColor(sf::Color::Red);
            daniado += _Time;
        }
        atqTime+=_Time;
        if(atqTime>0.3) atacar = true;
        anim->Update(_Time);
        spr.setTextureRect(anim->tRect);

        std::vector<Bullet>& balas = jg->getBalas();
        sf::FloatRect interseccion;
        if(spr.getGlobalBounds().intersects(jg->getPayerSpr().getGlobalBounds(), interseccion) && atacar && (interseccion.width>=8 || interseccion.width>=8) && jg->getState()!=-1){
            jg->recibirDmg(damage);
            atacar = false;
            atqTime = 0;
        }
        for(unsigned int i=0;i<balas.size();i++){
            if(balas[i].getSpr().getGlobalBounds().intersects(spr.getGlobalBounds()) && jg->getState()!=-1){
                health-=jg->getDmg();
                daniado = 0;
                if(state == 0)  state = 1;
                if(health<=0){
                    if(jg->getPlayerLife()<50 && rand()%2) jg->recibirVida(rand()%16+5);
                    jg->puntaje+=rand()%36+5;
                    state = -1;
                }
                if(jg->getWeaponT()!=4)
                    balas.erase(balas.begin()+i);
            }
        }
    }
}

void EnemyTipoA::Update(const float _Time, Player * jg, Map *mapa){
    sf::Vector2u j;
    j.x = jg->getPosInMap().x*2;
    j.y = jg->getPosInMap().y*2;
    Enemy::Update(_Time, jg, mapa);
    switch(state){
        case -1:
            {
            spr.setColor(sf::Color::Black);
            sf::Vector2u diff = Global::diffTo(posInMap, j);
            if(diff.y >= 20 || diff.x >= 20)
                tipo = -1;
            break;
            }
        case 0:
            {
            spr.setColor(sf::Color::White);
            sf::Vector2u dist = Global::diffTo(posInMap, j);
            if(dist.x == 0 && dist.y == 0){
                if(rand()%10 == 0){
                    anim->setFrameTime(0.02);
                    state = 1;
                }
            }else{
                if((dist.y<=visionDist) && (dist.x<=visionDist) && jg->getState()!=-1){
                    state = 1;
                    anim->setFrameTime(0.05);
                }
            }
            break;
            }
        case 1:
            if(rand()%2){
                if(posInMap.x == j.x){
                    if(posInMap.y > j.y){
                        dirrecion = elegirDir(2);
                    }
                    else{
                        dirrecion = elegirDir(6);
                    }
                }
                else if(posInMap.y == j.y){
                    if(posInMap.x > j.x){
                        dirrecion = elegirDir(0);
                    }
                    else{
                        dirrecion = elegirDir(4);
                    }
                }
                else if(posInMap.x > j.x && posInMap.y > j.y)
                    dirrecion = elegirDir(1);  //1
                else if(posInMap.x < j.x && posInMap.y < j.y)
                    dirrecion = elegirDir(5);  //5
                else if(posInMap.x > j.x && posInMap.y < j.y)
                    dirrecion = elegirDir(7);  //7
                else if(posInMap.x < j.x && posInMap.y > j.y)
                    dirrecion = elegirDir(3);  //3

            }else{
                if(posInMap.y == j.y){
                    if(posInMap.x > j.x){
                        dirrecion = elegirDir(0);
                    }
                    else{
                        dirrecion = elegirDir(4);
                    }
                }else if(posInMap.x == j.x){
                    if(posInMap.y > j.y){
                        dirrecion = elegirDir(2);
                    }
                    else{
                        dirrecion = elegirDir(6);
                    }
                }
                else if(posInMap.x > j.x && posInMap.y > j.y)
                    dirrecion = elegirDir(1);  //1
                else if(posInMap.x < j.x && posInMap.y < j.y)
                    dirrecion = elegirDir(5);  //5
                else if(posInMap.x > j.x && posInMap.y < j.y)
                    dirrecion = elegirDir(7);  //7
                else if(posInMap.x < j.x && posInMap.y > j.y)
                    dirrecion = elegirDir(3);  //3
            }

            state = 2;
            ms = 0;
            break;
        case 2:
            if(ms == 0){
                bool lm = rand()%2;
                while(enColision(mapa)==1){
                    if(lm) dirrecion++;
                    else dirrecion--;

                    if(dirrecion<=0){
                        lm = true;
                        dirrecion = 0;
                    }
                    if(dirrecion>=7){
                        lm = false;
                        dirrecion = 7;
                    }
                }
            }
            if(ms<=8){
                move();
                ms++;
            }
            if(ms>=8){
                state = 0;
                anim->setFrameTime(0.5);
                int f = spr.getPosition().y/32;
                int c = spr.getPosition().x/32;
                posInMap.y = f;
                posInMap.x = c;
            }
            break;
    }
}

void EnemyTipoB::Update(const float _Time, Player * jg, Map *mapa){
    //1.07, 0.53
    sf::Vector2u j = jg->getPosInMap();
    Enemy::Update(_Time, jg, mapa);
    switch(state){
        case -1:
            {
            spr.setColor(sf::Color::Black);
            sf::Vector2u diff = Global::diffTo(posInMap, j);
            if(diff.x >= 10 || diff.y >= 10)
                tipo = -1;
            }
            break;
        case 0:
            {
            spr.setColor(sf::Color::White);
            sf::Vector2u diff = Global::diffTo(posInMap, j);
            if(diff.y == 0 && diff.x == 0){
                state = 0;
            }else if((diff.x <= visionDist && diff.y <= visionDist) && jg->getState()!=-1)
                state = 1;
            }
            break;
        case 1:
            if(posInMap.x == j.x){
                if(posInMap.y > j.y)
                    dirrecion = 2;
                else
                    dirrecion = 6;
            }
            else if(posInMap.y == j.y){
                if(posInMap.x > j.x)
                    dirrecion = 0;
                else
                    dirrecion = 4;
            }
            else if(posInMap.x > j.x && posInMap.y > j.y)
                dirrecion = elegirDir(1);  //1
            else if(posInMap.x < j.x && posInMap.y < j.y)
                dirrecion = elegirDir(5);  //5
            else if(posInMap.x > j.x && posInMap.y < j.y)
                dirrecion = elegirDir(7);  //7
            else if(posInMap.x < j.x && posInMap.y > j.y)
                dirrecion = elegirDir(3);  //3

            state = 2;
            ms = 0;
            break;
        case 2:
            if(ms<=32){ //32
                move();
                ms++;
            }
            if(ms>=32){
                state = 0;
                if(mapa->getObjectInMap(posInMap.y, posInMap.x) == 14)
                    mapa->setObjectInMap(posInMap.y, posInMap.x, 0);
                int f = (spr.getPosition().y-32)/64;
                int c = (spr.getPosition().x-32)/64;
                posInMap.y = f;
                posInMap.x = c;
                if(mapa->getObjectInMap(posInMap.y, posInMap.x) == 0)
                    mapa->setObjectInMap(f, c, 14);
            }
            break;
    }
}

EnemyTipoA::EnemyTipoA(int f, int c){ //13
    state = tipo = atqTime = 0;
    visionDist = (rand()%6+5)*2;
    velocityMove = 4;
    damage = rand()%6+6;
    health = 50;
    daniado = 1;
    grid = 32;

    e.loadFromFile("sprites/enemy.png");
    anim = new Animation(&e, 0, sf::Vector2u(4, 2), 0.5, 0);
    spr.setTexture(e);
    spr.setTextureRect(anim->tRect);
    spr.setOrigin(32, 32);
    spr.setScale(0.5, 0.5);
    spr.setColor(sf::Color::Transparent);

    posInMap.y = f*2;
    posInMap.x = c*2;
    //spr.setPosition((c*64)+32, (f*64)+32);
    spr.setPosition(posInMap.x*32+16, posInMap.y*32+16);
}

EnemyTipoB::EnemyTipoB(int f, int c){ //14
    state = atqTime = 0;
    tipo = daniado = 1;
    visionDist = rand()%5+8;
    velocityMove = 2;
    damage = rand()%7+12;
    health = 100;

    e.loadFromFile("sprites/enemy.png");
    e.setSmooth(true);
    anim = new Animation(&e, 1, sf::Vector2u(4, 2), 0.07, 1);
    spr.setTexture(e);
    spr.setOrigin(32, 32);
    spr.setColor(sf::Color::Transparent);

    posInMap.y = f;
    posInMap.x = c;
    spr.setPosition((c*64)+32, (f*64)+32);
}

int Enemy::getVida(){
    return health;
}
