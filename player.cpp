#include "player.h"
#include <unistd.h>
#include <iostream>

Player::Player(){
    disparo = 0;
    vida = 100;
    direccion = 1;
    vel = 5;
    playerState = 0;
    animLoop = 0;
    character.loadFromFile("sprites/player.png");
    sprPlayer.setTexture(character);
    sprPlayer.setTextureRect(sf::IntRect(0.f, 0.f, character.getSize().x/3, character.getSize().y/2));
    sprPlayer.setOrigin(sprPlayer.getTextureRect().width/2.f, sprPlayer.getTextureRect().height/2.f);

    atm.loadFromFile("sprites/atm.png");
    luz.setTexture(atm);
    luz.setOrigin(256, 256);

    timeFr = frame = nArma = shootTime = reloadTime = puntaje = 0;
    arma = new Weapon(nArma);
    municion = 13;
    cargador = 13;
}

Player::Player(FILE*p){
    if(!cargarDelDisco(p)) exit(1);
    disparo = 0;
    direccion = 1;
    vel = 5;
    playerState = 0;
    animLoop = 0;
    character.loadFromFile("sprites/player.png");
    sprPlayer.setTexture(character);
    sprPlayer.setTextureRect(sf::IntRect(0.f, 0.f, character.getSize().x/3, character.getSize().y/2));
    sprPlayer.setOrigin(sprPlayer.getTextureRect().width/2.f, sprPlayer.getTextureRect().height/2.f);

    atm.loadFromFile("sprites/atm.png");
    luz.setTexture(atm);
    luz.setOrigin(256, 256);
    luz.setScale(4, 4);
    luz.setColor(sf::Color::White);

    timeFr = 0;
    frame = 0;
    arma = new Weapon(nArma);
    shootTime = 0;
    reloadTime = 0;
    sprPlayer.setPosition((posInMap.x*64)+32, (posInMap.y*64)+32);
}

void Player::setState(int var){
    playerState = var;
}

int Player::getState(){
    return playerState;
}

void Player::setPosition(sf::Vector2f pos){
    sprPlayer.setPosition(pos);
}

void Player::playerUpdate(Map *mapa, const std::vector<int> &activeWalls, const float Time){
    //printf("%d : %d = t: %d \n", posInMap.y, posInMap.x, mapa->getPosInMap(posInMap.y, posInMap.x));
    shootTime+=Time;
    timeFr += Time;
    float angulo = Global::getAngulo(sprPlayer.getPosition());
    sf::Vector2f aim = Global::aimDirNorm(sprPlayer.getPosition(), sf::Mouse::getPosition(*Global::currentWindow));

    if(vida>0 && playerState!=-1){
        disparo = arma->disparo;
        if(sprPlayer.getColor()!=sf::Color::White){
            modColor+=Time;
            if(modColor>0.2)
                sprPlayer.setColor(sf::Color::White);
        }else{
            modColor=0;
        }
        if(reloadTime>=arma->maxReloadTime){
            arma->recargando=false;
            reloadTime=0;
        }
        if(arma->recargando==true)
            reloadTime+=Time;

        //MOVIMIENTO
        if(Global::currentWindow->mapPixelToCoords(sf::Mouse::getPosition(*Global::currentWindow)).x > sprPlayer.getPosition().x) direccion = 1;
        else direccion = -1;
        sf::Vector2f mov = procesarMovimientos(aim, angulo);
        //COLISIONES
        int i, f;
        sf::FloatRect player = sprPlayer.getGlobalBounds();
        sf::Vector2f delta;
        //std::cout<<"size: "<<activeWalls.size()<<"\n";
        for(f=0;f<activeWalls.size();f++){

            sf::FloatRect wall = mapa->getWall(activeWalls[f]);
            sf::FloatRect xRect(player.left+mov.x, player.top, player.width, player.height);
            sf::FloatRect yRect(player.left, player.top+mov.y, player.width, player.height);
            if(xRect.intersects(wall)){
                mov.x = 0;
            }
            if(yRect.intersects(wall)){
                mov.y = 0;
            }

            ///////////////////////////
            for(i=0;i<bala.size();i++){
                if(bala[i].getSpr().getGlobalBounds().intersects(mapa->getWall(activeWalls[f])) || bulletOffScreen(i)){
                    bala.erase(bala.begin()+i);
                }
            }
        }
        sprPlayer.move(mov);

        for(i=0;i<bala.size();i++) bala[i].mover();
        arma->update(sprPlayer.getPosition(), angulo, Time);

        luz.setPosition(sprPlayer.getPosition());
        //
        if(disparo) luz.setScale(4, 4);
        else  luz.setScale(3.5, 3.5);
        //
        posInMap.x = sprPlayer.getPosition().x/64;
        posInMap.y = sprPlayer.getPosition().y/64;
        procesarEstado();
    }
}

sf::Sprite Player::getPayerSpr(){
    return sprPlayer;
}

void Player::setPosInMap(int f, int c){
    posInMap.y = f;
    posInMap.x = c;
}

sf::Vector2u Player::getPosInMap(){
    return posInMap;
}

bool Player::gunPos(float mPos){
    if(mPos > sprPlayer.getPosition().y-sprPlayer.getGlobalBounds().height)
        return true;
    else return false;
}

void Player::drawPlayer(sf::RenderWindow *window, float mPos){
    for(int i=0;i<bala.size();i++){
        window->draw(bala[i].getSpr());
    }
    if(gunPos(mPos)){
        window->draw(sprPlayer);
        if(playerState!=-1)
            arma->drawWeapon(window);
    }else{
        if(playerState!=-1)
            arma->drawWeapon(window);
        window->draw(sprPlayer);
    }
}

sf::Vector2f Player::procesarMovimientos(sf::Vector2f aim, float angulo){
    sf::Vector2f mov;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        //sprPlayer.move(-vel, 0.y);
        mov.x = -vel;
        //direccion = -1;
    if(playerState==0) playerState=1;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        //sprPlayer.move(0.y, -vel);
        mov.y = -vel;
        if(playerState==0) playerState=1;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        //sprPlayer.move(vel, 0.y);
        mov.x = vel;
        //direccion = 1;
        if(playerState==0) playerState=1;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        //sprPlayer.move(0.y, vel);
        mov.y = vel;
        if(playerState==0) playerState=1;
    }
    if((sf::Keyboard::isKeyPressed(sf::Keyboard::R) || cargador==0) && (municion>0 && !arma->recargando && cargador<arma->maxBalas)){
        arma->recargando = true;
        if(municion>=arma->maxBalas){
            municion -= arma->maxBalas;
            cargador = arma->maxBalas;
        }else{
            cargador = municion;
            municion = 0;
        }
    }
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && cargador>0 && !arma->recargando && shootTime>arma->getTimeShoot()){
        //sf::Mouse::setPosition(sf::Mouse::getPosition(Global::*currentWindow)+sf::Vector2i(5, 5));
        mov+={aim.x*-3, aim.y*-3};
        cargador--;
        shootTime=0;
        Bullet *b1;
        aim = setRecoil(aim);
        b1 = new Bullet(aim, arma->velBalas);
        b1->setPosition(sprPlayer.getPosition());
        b1->setAngle(angulo);
        bala.push_back(*b1);
        if(arma->getState()==0){
            arma->selecEffect(rand()%5);
            arma->setState(1);
        }

    }
    return mov;
}

void Player::procesarEstado(){
    switch(playerState){
        case 0:
            if(timeFr>0.2){
                int offset=0;
                if(direccion == -1) offset = character.getSize().x/3;
                sprPlayer.setTextureRect(sf::IntRect(offset+(character.getSize().x/3)*frame, 0, character.getSize().x/3*direccion, character.getSize().y/2));
                timeFr = 0;
                if(animLoop==0){
                    if(frame==2)
                        animLoop=1;
                    else
                        frame++;
                }
                else{
                    if(frame==0)
                        animLoop=0;
                    else
                        frame--;
                }
            }
            break;
        case 1:
            if(timeFr>0.08){
                int offset=0;
                if(direccion == -1) offset = character.getSize().x/3;
                sprPlayer.setTextureRect(sf::IntRect(offset+(character.getSize().x/3)*frame, character.getSize().y/2, character.getSize().x/3*direccion, character.getSize().y/2));
                timeFr = 0;
                if(animLoop==0){
                    if(frame==2)
                        animLoop=1;
                    else
                        frame++;
                }
                else{
                    if(frame==0)
                        animLoop=0;
                    else
                        frame--;
                }
                playerState = 0;
            }
            break;
    }
}

bool Player::bulletOffScreen(int f){
    if(bala[f].getSpr().getPosition().x>sprPlayer.getPosition().x+600 || bala[f].getSpr().getPosition().x<sprPlayer.getPosition().x-600
       || bala[f].getSpr().getPosition().y<sprPlayer.getPosition().y-400 || bala[f].getSpr().getPosition().y>sprPlayer.getPosition().y+400)
        return true;
    else
        return false;
}

int Player::getPlayerLife(){
    return vida;
}

int Player::getCargador(){
    return cargador;
}

int Player::getMunicion(){
    return municion;
}

void Player::dropToPlayer(std::vector<Chest> &cofres){
    if(cofres.size()>0){
        int x = -1, i;
        for(i=0;i<cofres.size();i++){
            if(posInMap.x == cofres[i].pos.x && posInMap.y == cofres[i].pos.y){
                x = i;
            }
        }
        if(x==-1) return;
        if(cofres[x].estado==1){
            int drop = cofres[x].chestDrop();
            cofres[x].disable();
            if(drop==1){
                do{drop = rand()%4 + 1;
                }while(drop == nArma);
                arma = new Weapon(drop);
                nArma = drop;
                cargador = arma->maxBalas;
                if(municion<arma->maxBalas) municion=arma->maxBalas;
            }else municion += drop;
        }
    }
}

void Player::recibirDmg(int dmg){
    if(vida>0){
        vida -= dmg;
        sprPlayer.setColor(sf::Color::Red);
        if(vida<=0){
            Morir();
        }
    }
}

void Player::recibirVida(int lf){
    if(vida+lf<100 && vida>0){
        vida+=lf;
    }
    sprPlayer.setColor(sf::Color::Green);
}

std::vector<Bullet>& Player::getBalas(){
    return bala;
}
int Player::getDmg(){
    return arma->getDmg();
}
int Player::getWeaponT(){
    return arma->getTipo();
}
void Player::setVida(int f){
    vida = f;
}
void Player::Morir(){
    vida = 0;
    sprPlayer.setRotation(90);
    sprPlayer.setColor(sf::Color::Black);
    sprPlayer.move(0, 16);
    luz.setScale(2, 2);
    playerState = -1;
}

sf::Vector2f Player::setRecoil(sf::Vector2f aim){
    float disp = arma->dispersion;
    int r = rand()%6;
    switch(r){
    case 0:
        aim += sf::Vector2f(0, Global::randomFloat(0, disp));
        break;
    case 1:
        aim -= sf::Vector2f(0, Global::randomFloat(0, disp));
        break;
    case 2:
        aim += sf::Vector2f(Global::randomFloat(0, disp), Global::randomFloat(0, disp));
        break;
    case 3:
        aim -= sf::Vector2f(Global::randomFloat(0, disp), Global::randomFloat(0, disp));
        break;
    case 4:
        aim += sf::Vector2f(Global::randomFloat(0, disp), 0);
        break;
    case 5:
        aim -= sf::Vector2f(Global::randomFloat(0, disp), 0);
        break;
    }
    return aim;
}

bool Player::guardarEnDisco(FILE*p){
    if(!p) return false;
    fwrite(&vida, sizeof(int), 1, p);
    fwrite(&puntaje, sizeof(int), 1, p);
    fwrite(&nArma, sizeof(int), 1, p);
    fwrite(&cargador, sizeof(int), 1, p);
    fwrite(&municion, sizeof(int), 1, p);
    std::cout<<nArma<<std::endl;
    fwrite(&posInMap, sizeof(sf::Vector2u), 1, p);
    return true;
}
bool Player::cargarDelDisco(FILE*p){
    if(!p) return false;
    fread(&vida, sizeof(int), 1, p);
    fread(&puntaje, sizeof(int), 1, p);
    fread(&nArma, sizeof(int), 1, p);
    fread(&cargador, sizeof(int), 1, p);
    fread(&municion, sizeof(int), 1, p);
    fread(&posInMap, sizeof(sf::Vector2u), 1, p);
    std::cout<<nArma<<std::endl;
    return true;
}
