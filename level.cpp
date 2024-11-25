#include "level.h"

void Level::newMap(int _tam, int typeMap, int modifier){
    mapa = new Map(_tam, typeMap, modifier);
}

Map* Level::getMap(){
    return mapa;
}

void Level::spawnPlayer(Player * j1){
    int points = mapa->getTam()/10;
    sf::Vector2f spawnPos;
    int f, c;
    do{
        f = rand()%10 + points/2*10;
        c = rand()%10 + points/2*10;
    }while(mapa->getPosInMap(f, c)!=casAct);
    spawnPos.x = (c * 64) + 32;
    spawnPos.y = (f * 64) + 32;
    j1->setPosInMap(f, c);
    j1->setPosition(spawnPos);
}

void Level::spawnChests(){
    int points = mapa->getTam()/10;
    int j, i, h, f, c;
    for(i=0;i<points;i++){
        for(h=0;h<points;h++){
            for(j=0;j<Global::getCantChestsSpawns(i, h, mapa->getTam());j++){
                do{
                    f = rand()%10;
                    c = rand()%10;
                }while(mapa->getPosInMap(f+(10*i), c+(h*10))!=casAct && mapa->getObjectInMap(f+(10*i), c+(h*10))!=0);
                mapa->setObjectInMap(f+(10*i), c+(h*10), 3);
            }
        }
    }
}
