#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED
#include "score.h"
#define distCam 100

class Game{
    private:
        unsigned int tamMap;
        int nivelActual;
        int seed, gameState;
        bool reanudarPartida;
        sf::RenderWindow *window;
        sf::View *camera;
        sf::Event event;
        Map *dungeon;
        Player *jugador;
        GuiDialog* objInteract;
        std::vector<Chest> cofres;
        std::vector<Enemy*> enemigos;
        std::vector<Level*> niveles;
        Mouse *mira;
        sf::Vector2f camPos;
        sf::Vector2u antPosPlayer;
        sf::Clock reloj;
        float Time;
        bool salidaLibre, dungeonState;

        //interfaz
        sf::Texture tBar;
        sf::Sprite lifebar;
        sf::Texture tmun;
        sf::Sprite ammo;
        sf::RectangleShape vida;
        sf::Font f;
        sf::Text mText[3], pf, sc;
        sf::RectangleShape pausaFondo;
        sf::Text mPausa[2];
    public:
        Game(unsigned int _tamMap, sf::RenderWindow *vent);
        Game(sf::RenderWindow *vent);
        ~Game();
        int getGameState();
        //std::vector<int> scopeMap(sf::Vector2u, int);
        void posicionarCamara();
        void updateInterfaz();
        void drawInterfaz();
        void loadInterfaz();
        void iniciarJuego();
        void menuPausa();
        void loadMenuPausa();
        void procesarLogica();
        void procesarTeclas();
        void gameLoop();
        void renderizar();
        void inDungeon();
        void enemyDraw();
        void enemySpawn();
        void showDialogs();
        void actionObject(int);
        void puedeGanar();
        void mostrarPuntaje();
        void guardarPartida();
        void cargarPartida();
};

Game::Game(unsigned int _tamMap, sf::RenderWindow *vent){
    window = vent;
    gameState = salidaLibre = reanudarPartida = 0;
    tamMap = _tamMap;
    dungeonState = false;
}

Game::Game(sf::RenderWindow *vent){
    window = vent;
    gameState = 0;
    reanudarPartida = 1;
}

Game::~Game(){
    delete window;
    if(!gameState){
        delete camera;
        delete jugador;
    }
}

void Game::iniciarJuego(){
    seed = time(NULL);
    srand(seed);

    if(reanudarPartida){
        cargarPartida();
    }else{
        nivelActual = 0;                //mapa = new Map(tamMap, 0, 3);
        niveles.push_back(new Level);
        jugador = new Player();
        niveles[nivelActual]->newMap(tamMap, 0, rand()%11 + 42);
        niveles[nivelActual]->spawnPlayer(jugador);
        niveles[nivelActual]->spawnChests();
        enemySpawn();
    }
    camera = new sf::View;
    objInteract = nullptr;
    mira = new Mouse(window);
    loadInterfaz();
    loadMenuPausa();

    antPosPlayer = jugador->getPosInMap();
    *camera = window->getDefaultView();
    //camera->zoom(8);
    gameState = 1;
    //mapa->mapConsole();
}

void Game::procesarLogica(){
    std::vector<int> walls;
    Map * mapa = niveles[nivelActual]->getMap();
    int tam = 12;
    sf::Vector2i j = (sf::Vector2i)jugador->getPosInMap();
    Chest *cofre;
    if(j.x-tam<0) j.x = tam;
    if(j.x+tam>mapa->getTam()) j.x = mapa->getTam()-tam;
    if(j.y-tam<0) j.y = tam;
    if(j.y+tam>mapa->getTam()) j.y = mapa->getTam()-tam;
    int unsigned i, h;
    for(i=j.y-tam;i<j.y+tam;i++){
        for(h=j.x-tam;h<j.x+tam;h++){
            if(mapa->isWall(i, h)){
                walls.push_back(mapa->getIdInMap(i, h));
            }else if(mapa->getPosInMap(i, h)==casAct){
                if(mapa->getObjectInMap(i, h)<13){
                    if(mapa->getObjectInMap(i, h)==3 && !mapa->getVisitadoMap(i , h)){
                        cofre = new Chest(i, h);
                        cofres.push_back(*cofre);
                        mapa->setObjectInMap(i, h, cofre->getTipo());
                    }else if(mapa->getObjectInMap(i, h)>=4 && mapa->getObjectInMap(i, h)<=7 && !mapa->getVisitadoMap(i , h)){
                        cofre = new Chest(i, h, mapa->getObjectInMap(i, h));
                        cofres.push_back(*cofre);
                        mapa->setObjectInMap(i, h, cofre->getTipo());
                    }
                }else{
                    if(mapa->getObjectInMap(i, h)==14 && !mapa->getVisitadoMap(i , h)){
                        enemigos.push_back(new EnemyTipoB(i, h));
                    }
                    if(mapa->getObjectInMap(i, h)==13 && !mapa->getVisitadoMap(i , h)){
                        enemigos.push_back(new EnemyTipoA(i, h));
                    }
                }
            }
            mapa->setVisitadoMap(i, h, 1);
        }
    }

    for(i=0;i<enemigos.size();i++){
        if(enemigos[i]->tipo != -1) enemigos[i]->Update(Time, jugador, mapa);
        else enemigos.erase(enemigos.begin()+i);
    }
    jugador->playerUpdate(mapa, walls, Time);
    showDialogs();

    for(i=0;i<cofres.size();i++){
        if(cofres[i].estado == 0){
            mapa->setObjectInMap(cofres[i].pos.y, cofres[i].pos.x, cofres[i].getTipo());
            cofres.erase(cofres.begin()+i);
        }
    }
    puedeGanar();
    if(jugador->getPlayerLife()<=0 && jugador->getState()==-1){
        gameState = 2;
    }
    if(gameState == 2){
        mostrarPuntaje();
        static float mTime = 0;
        mTime+=Time;
        if(mTime>6){
            Score reg;
            reg.setScore(seed, tamMap, jugador->puntaje);
            if(reg.puntaje > 0)
                reg.guardarEnDisco();
            gameState = -1;
            mTime = 0;
        }
    }
}

void Game::inDungeon(){
    dungeon = niveles[nivelActual]->getMap();
    std::vector<int> walls;
    const int tam = 15;
    sf::Vector2i j = (sf::Vector2i)jugador->getPosInMap();
    if(j.x-tam<0) j.x = tam;
    if(j.x+tam>dungeon->getTam()) j.x = dungeon->getTam()-tam;
    if(j.y-tam<0) j.y = tam;
    if(j.y+tam>dungeon->getTam()) j.y = dungeon->getTam()-tam;
    int unsigned i, h;
    for(i=j.y-tam;i<j.y+tam;i++){
        for(h=j.x-tam;h<j.x+tam;h++){
            if(dungeon->isWall(i, h)){
                walls.push_back(dungeon->getIdInMap(i, h));
            }
            dungeon->setVisitadoMap(i, h, 1);
        }
    }
    jugador->playerUpdate(dungeon, walls, Time);
    showDialogs();
}

void Game::procesarTeclas(){
    Map * mapa = niveles[nivelActual]->getMap();
    while(window->pollEvent(event)){
        if(event.type == sf::Event::Closed)
            window->close();
        if(event.type == sf::Event::KeyReleased){
            if(event.key.code == sf::Keyboard::Escape){
                if(gameState == 1){
                    gameState = 0;
                    window->setMouseCursorVisible(true);
                }else if(gameState == 0){
                    gameState = 1;
                    window->setMouseCursorVisible(false);
                }
            }
            if(event.key.code == sf::Keyboard::Return){
                sf::Vector2u pos = jugador->getPosInMap();
                std::cout<<pos.x<<" "<<pos.y<<" -> "<<mapa->getPosInMap(pos.y, pos.x)<<"\n";
                //if(dungeonState) dungeonState = false;
            }
            if(event.key.code == sf::Keyboard::E){
                actionObject(mapa->getObjectInMap(jugador->getPosInMap().y, jugador->getPosInMap().x));
            }
        }
    }
}

void Game::posicionarCamara(){
    if(jugador->getState()!=-1){
        camPos = {(jugador->getPayerSpr().getPosition().x+mira->getMousePos().x)/2.f, (jugador->getPayerSpr().getPosition().y+mira->getMousePos().y)/2.f};
        if(camPos.x > jugador->getPayerSpr().getPosition().x+distCam)
            camPos.x = jugador->getPayerSpr().getPosition().x+distCam;
        if(camPos.y > jugador->getPayerSpr().getPosition().y+distCam)
            camPos.y = jugador->getPayerSpr().getPosition().y+distCam;
        if(camPos.x < jugador->getPayerSpr().getPosition().x-distCam)
            camPos.x = jugador->getPayerSpr().getPosition().x-distCam;
        if(camPos.y < jugador->getPayerSpr().getPosition().y-distCam)
            camPos.y = jugador->getPayerSpr().getPosition().y-distCam;
    }else
        camPos = {jugador->getPayerSpr().getPosition().x, jugador->getPayerSpr().getPosition().y};

    camera->setCenter(camPos);
}

void Game::gameLoop(){
    while(gameState!=-1 && window->isOpen()){
        Time = reloj.restart().asSeconds();
        procesarTeclas();
        if(gameState==0){
            menuPausa();
        }
        if(gameState==1 || gameState==2){
            mira->updateMouse();
            posicionarCamara();
            updateInterfaz();
            if(dungeonState)
                inDungeon();
            else
                procesarLogica();
        }
        renderizar();
    }
}

void Game::mostrarPuntaje(){
    pf.setPosition(camera->getCenter().x-pf.getLocalBounds().width/2, camera->getCenter().y-window->getSize().y/3);
    pf.setString("PUNTAJE FINAL");

    sc.setPosition(camera->getCenter().x-sc.getLocalBounds().width/2, camera->getCenter().y-window->getSize().y/5);
    sc.setString(std::to_string(jugador->puntaje));
}

void Game::renderizar(){
    Map * mapa = niveles[nivelActual]->getMap();

    window->setView(*camera);
    window->clear(sf::Color::Black);
    if(dungeonState){
        dungeon->dibujarMapa(1, window, jugador->getPosInMap());
        jugador->drawPlayer(window, mira->getMousePos().y);
        if(dungeon->secondLayer.size()>0)
            dungeon->drawSecondLayer(window);
    }else{
        mapa->dibujarMapa(1, window, jugador->getPosInMap());
        jugador->drawPlayer(window, mira->getMousePos().y);
        if(mapa->secondLayer.size()>0)
            mapa->drawSecondLayer(window);
        enemyDraw();
    }
    window->draw(jugador->luz);
    if(objInteract != nullptr)
        objInteract->Draw(window);
    window->draw(mira->getSpr());
    if(gameState==2){
        window->draw(pf);
        window->draw(sc);
    }
    if(gameState!= 2 && gameState!= -1)
        drawInterfaz();

    if(gameState==0){
        window->draw(pausaFondo);
        window->draw(mPausa[0]);
        window->draw(mPausa[1]);
    }

    window->display();
}

void Game::menuPausa(){
    pausaFondo.setPosition(camera->getCenter());
    mPausa[0].setPosition(pausaFondo.getPosition().x-70, pausaFondo.getPosition().y-55);
    mPausa[1].setPosition(pausaFondo.getPosition().x-50, pausaFondo.getPosition().y+15);
    sf::Vector2f mPos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
    if(mPausa[1].getGlobalBounds().contains(mPos)){
        mPausa[1].setFillColor(sf::Color::Red);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            gameState = -1;
        }
    }else{
        mPausa[1].setFillColor(sf::Color::White);
    }
    if(mPausa[0].getGlobalBounds().contains(mPos)){
        mPausa[0].setFillColor(sf::Color::Red);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            guardarPartida();
            gameState = 1;
        }
    }else{
        mPausa[0].setFillColor(sf::Color::White);
    }
}

void Game::loadMenuPausa(){
    pausaFondo.setSize(sf::Vector2f(200, 150));
    pausaFondo.setOrigin(100, 75);
    pausaFondo.setFillColor(sf::Color(0, 0, 0, 100));//(47, 79, 79, 120)

    mPausa[0].setString("GUARDAR");
    mPausa[1].setString("SALIR");
    for(int i=0;i<2;i++){
        mPausa[i].setFont(f);
        mPausa[i].setCharacterSize(32);
    }
}

void Game::showDialogs(){
    Map * mapa = niveles[nivelActual]->getMap();
    int offsetY = -32;
    int offsetX = 25;
    sf::Vector2u jg = jugador->getPosInMap();
    sf::Vector2f pos = sf::Vector2f(jg.x*64+offsetX, jg.y*64+offsetY);
    if(objInteract != nullptr){
        if(pos != objInteract->getPos()){
            int obj = mapa->getObjectInMap(jg.y, jg.x);
            if((obj>=4 && obj<=8)){
                objInteract = new GuiDialog("E", f, pos, sf::Color::Red);
            }else{
                delete objInteract;
                objInteract = nullptr;
            }
        }
    }else{
        int obj = mapa->getObjectInMap(jg.y, jg.x);
        if(obj>=4 && obj<=8){
            objInteract = new GuiDialog("E", f, pos, sf::Color::Red);
        }else{
            delete objInteract;
            objInteract = nullptr;
        }
    }
}

void Game::updateInterfaz(){
    if(jugador->getPlayerLife()>50) vida.setFillColor(sf::Color::Green);
    else vida.setFillColor(sf::Color::Red);

    vida.setScale(jugador->getPlayerLife()/100.f, 1);
    lifebar.setPosition(camera->getCenter().x-500, camera->getCenter().y-288); //ancho 290, alto
    vida.setPosition(lifebar.getPosition().x+17, lifebar.getPosition().y);
    ammo.setPosition(lifebar.getPosition().x, lifebar.getPosition().y+30);
    mText[0].setString(std::to_string(jugador->getMunicion()));
    mText[1].setString(std::to_string(jugador->getCargador()));
    mText[2].setString("/");
    mText[0].setPosition(lifebar.getPosition().x+30, lifebar.getPosition().y+32);
    mText[1].setPosition(mText[0].getPosition().x+mText[0].getGlobalBounds().width+30, lifebar.getPosition().y+32);
    mText[2].setPosition((mText[0].getPosition().x+mText[0].getGlobalBounds().width+mText[1].getPosition().x-10)/2, lifebar.getPosition().y+32);

}

void Game::drawInterfaz(){
    for(int i=0;i<3;i++){
        window->draw(mText[i]);
    }
    window->draw(lifebar);
    window->draw(vida);
    window->draw(ammo);
}

void Game::loadInterfaz(){
    tBar.loadFromFile("sprites/lifebar.png");
    tmun.loadFromFile("sprites/ammo.png");

    pf.setFont(f);
    pf.setCharacterSize(40);
    pf.setFillColor(sf::Color::Red);
    sc.setFont(f);
    sc.setCharacterSize(40);
    sc.setFillColor(sf::Color::Red);

    ammo.setTexture(tmun);
    ammo.setScale(0.8, 0.8);
    lifebar.setTexture(tBar);
    lifebar.setScale(2, 2);
    vida.setSize(sf::Vector2f(196, 16));
    vida.setFillColor(sf::Color::Green);
    f.loadFromFile("sprites/font.ttf");

    for(int i=0;i<3;i++){
        mText[i].setFont(f);
        mText[i].setCharacterSize(20);
    }
}

void Game::enemyDraw(){
    int i;
    for(i=0;i<enemigos.size();i++){
        enemigos[i]->drawEnemy(window);
    }
}

void Game::enemySpawn(){
    Map * mapa = niveles[nivelActual]->getMap();
    int points = mapa->getTam()/10;
    int i, h, f, c, j;
    for(i=0;i<points;i++){
        for(h=0;h<points;h++){
            for(j=0;j<Global::getCantEnemySpawns(i, h, tamMap);j++){
                do{
                    f = rand()%10;
                    c = rand()%10;
                }while(mapa->getPosInMap(f+(10*i), c+(h*10))!=casAct && mapa->getObjectInMap(f+(10*i), c+(h*10))!=0);
                if(rand()%2)
                    mapa->setObjectInMap(f+(10*i), c+(h*10), 13);
                else
                    mapa->setObjectInMap(f+(10*i), c+(h*10), 14);
            }
        }
    }
}

void Game::guardarPartida(){
    FILE*p = fopen("savedata.sk", "wb");
    fwrite(&tamMap, sizeof(int), 1, p);
    niveles[nivelActual]->getMap()->guardarEnDisco(p);
    jugador->guardarEnDisco(p);
    fclose(p);
}

void Game::cargarPartida(){
    FILE*p = fopen("savedata.sk", "rb");
    fread(&tamMap, sizeof(int), 1, p);
//    mapa = new Map(p);
    jugador = new Player(p);
    fclose(p);
}

void Game::puedeGanar(){
    Map * mapa = niveles[nivelActual]->getMap();
    std::vector<sf::Vector2u> salida = mapa->getSalida();
    sf::Vector2u j = jugador->getPosInMap();
    int i, n;
    bool despejado;
    for(i=0;i<salida.size();i++){
        sf::Vector2u distToExit = Global::diffTo(salida[i], j);
        if(distToExit.y <= 15 && distToExit.x <= 15){
            despejado = true;
            for(n=0;n<enemigos.size();n++){
                sf::Vector2u diff = Global::diffTo(salida[i], enemigos[n]->getPosInMap());
                if((diff.y <= 7 && diff.x <= 7) && enemigos[n]->getVida()>0)
                    despejado = false;
            }

            if(despejado){
                mapa->setInMap(salida[i].y, salida[i].x, 9);
                if(j.y == salida[i].y && j.x == salida[i].x){
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::E) && gameState!=2){
                        jugador->luz.setScale(4.5, 4.5);
                        jugador->puntaje *= 1.5;
                        jugador->setState(-1);
                        gameState = 2;
                    }
                }
            }else{
                mapa->setInMap(salida[i].y, salida[i].x, -9);
            }
        }
    }

}

void Game::actionObject(int obj){
    if(obj >= 4 && obj <=7){
        jugador->dropToPlayer(cofres);
    }else if(obj == 8){
        niveles.push_back(new Level);
        nivelActual++;
        niveles[nivelActual]->newMap(40, 1, 3);
        niveles[nivelActual]->spawnPlayer(jugador);
        dungeonState = true;
        camera->zoom(1.5);
    }
}
#endif // JUEGO_H_INCLUDED
