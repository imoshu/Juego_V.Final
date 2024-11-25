#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include <SFML/Graphics.hpp>
#include "dataType.h"
#include <vector>
#include "weapon.h"
#include "chest.h"
#include "map.h"

class Player{
private:
    int playerState;
    sf::Texture character;
    sf::Sprite sprPlayer;
    float vel;
    int vida;
    int municion;
    int cargador;
    int direccion;
    sf::Vector2u posInMap;
    Weapon *arma;
    int nArma;
    std::vector<Bullet> bala;

    sf::Clock pReloj;
    bool animLoop;
    float timeFr;
    int frame;
    float shootTime, reloadTime, modColor;
    sf::Vector2f setRecoil(sf::Vector2f);

public:
    int puntaje;
    bool disparo;
    Player();
    Player(FILE*);
    sf::Texture atm;
    sf::Sprite luz;

    bool gunPos(float);
    void setPosition(sf::Vector2f);
    void setPosInMap(int f, int c);
    sf::Vector2u getPosInMap();
    void playerUpdate(Map *, const std::vector<int>&, const float);
    sf::Sprite getPayerSpr();
    void drawPlayer(sf::RenderWindow *, float);
    sf::Vector2f procesarMovimientos(sf::Vector2f, float);
    void procesarEstado();
    void setState(int);
    void setVida(int);
    int getState();
    bool bulletOffScreen(int);
    int getPlayerLife();
    int getMunicion();
    int getCargador();
    void recibirDmg(int);
    void recibirVida(int);
    void dropToPlayer(std::vector<Chest>&);
    std::vector<Bullet>& getBalas();
    int getDmg();
    void Morir();
    int getWeaponT();

    bool guardarEnDisco(FILE*);
    bool cargarDelDisco(FILE*);
};

#endif // PLAYER_H_INCLUDED
