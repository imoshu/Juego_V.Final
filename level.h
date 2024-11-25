#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED
#include <vector>
#include "map.h"
#include "enemy.h"
#include "chest.h"
#include "player.h"

class Level{
private:
    Map * mapa;
    std::vector<Enemy*> enemigos;
    std::vector<Chest*> cofres;

public:
    void newMap(int _tam, int typeMap, int modifier);
    Map* getMap();
    void spawnPlayer(Player*);
    void spawnChests();
};

#endif // LEVEL_H_INCLUDED
