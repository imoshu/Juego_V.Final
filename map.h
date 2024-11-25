#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED
#include <SFML/Graphics.hpp>
#include "dataType.h"
#define casAct 1
#define casWall 2

class Map{
private:
    sf::Texture f, w, c, dw;
    sf::Sprite floor[3], swall[4], chest, doorway[2];
    sf::FloatRect *wall;
    Tile **m;
    int tam, cWalls;
    std::vector<sf::Vector2u> salida;
    std::vector<sf::Vector2u> doors;
    void mapaEnCero();
    void loadTextures();
    int countWalls();
    void decide(Room *);
    int checkSpace(Room);
    void makeRoom(Room *);
    void buildRoom(Room, int&);
    void buildWalls();
    bool checkVecinos(int, int);
    void newMap(int ocupacion);
    void newDungeon(int nPasadas);
    void simpleConnect(int nPasadas = 1);
    void exitSpawn();
    int cellsCount(int, int, int);
    sf::Vector2u getRandomNextStep(sf::Vector2u Position);
    void drunkWalker(sf::Vector2u startPos, sf::Vector2u endPos, int tileType);
    void drunkWalker(sf::Vector2u startPos, int nMoves, int tileType);
public:
    Map(int, int, int);
    Map(FILE*);
    ~Map();
    sf::FloatRect getWall(int);
    int getTam();
    int getWalls();
    sf::Vector2u exitDraw(int);
    void spawnDoors();
    void setCollisions();
    void setWallsToDraw();
    void dibujarMapa(bool, sf::RenderWindow *, sf::Vector2u j);
    void drawObject(int, int, sf::RenderWindow*);
    void drawSecondLayer(sf::RenderWindow*);
    //void getDrawObject(int, int);
    void drawChests(int, int);
    std::vector<sf::Vector2u> secondLayer;
        //////////////////////////////
    int setWall(int, int);
    bool isWall(int, int);
    int getPosInMap(int, int);
    int getIdInMap(int, int);
    int setIdInMap(int, int, int);
    void setInMap(int, int, int);
    void setObjectInMap(int, int, int);
    int getObjectInMap(int, int);
    void setVisitadoMap(int , int, bool);
    bool getVisitadoMap(int, int);
    Tile* getTile(sf::Vector2u);
    Tile getTileInGrid(int grid, sf::Vector2u pos);

    void mapConsole();
    bool guardarEnDisco(FILE*);
    bool cargarDelDisco(FILE*);
    std::vector<sf::Vector2u> getSalida();

};

#endif // MAP_H_INCLUDED
