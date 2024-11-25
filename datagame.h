#ifndef DATAGAME_H_INCLUDED
#define DATAGAME_H_INCLUDED
#include <SFML/Graphics.hpp>
#include <math.h>

class Global{
public:
static sf::RenderWindow *currentWindow;
static float getAngulo(sf::Vector2f);
static float randomFloat(float LO, float HI);
static sf::Vector2u diffTo(sf::Vector2u startPos, sf::Vector2u endPos);
static sf::Vector2f aimDirNorm(sf::Vector2f Pos, sf::Vector2i target);
static int getCantChestsSpawns(int f, int c, int tamMap);
static int getCantEnemySpawns(int f, int c, int tamMap);
};


#endif // DATAGAME_H_INCLUDED
