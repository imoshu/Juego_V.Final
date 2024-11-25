#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "button.h"
#include "score.h"

//"savedata.dat"

class guiScore{
    public:
        sf::Text seed, tamM, score;
        sf::RectangleShape body;
        int data;
        guiScore(sf::Font *f):body(sf::Vector2f(1024, 80)){
            seed.setFont(*f);
            tamM.setFont(*f);
            score.setFont(*f);
        }
        guiScore():body(sf::Vector2f(1024, 80)){}
        void operator= (Score obj){
            seed.setString(std::to_string(obj.seed));
            tamM.setString(std::to_string(obj.tamMap));
            score.setString(std::to_string(obj.puntaje));
            data = obj.puntaje;
        }
        void setPosition(float x, float y){
            seed.setPosition(x, y);
            tamM.setPosition(x*2.7, y);
            score.setPosition(x*4, y);
            body.setPosition(0, y-20);
        }
};

class Menu{
    private:
        sf::Texture playerTexture;
        sf::Texture weaponTexture;
        sf::Texture backTexture;
        sf::Sprite sprB;
        sf::Sprite sprP;
        sf::Sprite sprW;
        Animation* playerBG;

        sf::RenderWindow *window;
        short int& mainState;
        sf::Font *f;
        std::vector<Button> mbt;
    public:
        Menu(sf::RenderWindow *, short int&);
        sf::Vector2f getMousePos();
        void loadMenu();
        void selecTamMap(int&);
        void mainMenu();
        void scoresMenu();
        void ordenarRanking(std::vector<guiScore>&);
        int cantScores();
        bool checkArchivo(const char*);
        bool uploadData(sf::Ftp* sv);
        bool downloadData(sf::Ftp* sv);
};

Menu::Menu(sf::RenderWindow *_window, short int& _state):mainState(_state), mbt(6, Button(sf::Color(24, 24, 24))){
    window = _window;
    loadMenu();
}

void Menu::loadMenu(){
    playerTexture.loadFromFile("sprites/player.png");
    weaponTexture.loadFromFile("sprites/weapons.png");
    backTexture.loadFromFile("sprites/backg.png");
    sprB.setTexture(backTexture);
    sprB.setOrigin(sprB.getTextureRect().width/2, sprB.getTextureRect().height/2);
    sprB.setScale(1.5, 1.5);
    sprW.setTexture(weaponTexture);
    sprW.setTextureRect(sf::IntRect(100, 0, 100, 30));
    sprW.setOrigin(30, 10);
    sprW.setScale(2.5, 2.5);
    playerBG = new Animation(&playerTexture, 0, sf::Vector2u(3, 2), 0.2, true);
    sprP.setTexture(playerTexture);
    sprP.setTextureRect(playerBG->tRect);
    sprP.setScale(6, 6);
    sprP.setOrigin(sprP.getTextureRect().width/2, sprP.getTextureRect().height/2);
    sprP.setPosition(window->getSize().x/2*1.3, window->getSize().y/2);
    sprW.setPosition(sprP.getPosition().x-15, sprP.getPosition().y+15);
    sprB.setPosition(sprP.getPosition().x, sprP.getPosition().y);

    f = new sf::Font();
    f->loadFromFile("sprites/font.ttf");
    mbt[0].setString("CONTINUAR");
    mbt[1].setString("NUEVA PARTIDA");
    mbt[2].setString("PUNTAJES");
    mbt[3].setString("SALIR");
    mbt[4].setString("TAMAÑO DEL MAPA: ");
    mbt[5].setString("EMPEZAR PARTIDA");
    for(int i=0;i<6;i++){
        mbt[i].setFont(*f);
        mbt[i].setTextSize(32);
        mbt[i].setAutoSize(1.4f, 1.2f);
        mbt[i].centrar();
    }
    for(int i=0;i<4;i++)
        mbt[i].setPosition(100+mbt[i].getBounds(1).width/2, 120+i*100);
}

sf::Vector2f Menu::getMousePos(){
    return window->mapPixelToCoords(sf::Mouse::getPosition(*window));
}

void Menu::mainMenu(){
    float Time;
    sf::Clock reloj;
    bool fSave = false; //checkArchivo("savedata.sk");
    bool fScores = checkArchivo("scores.sk");
    sf::Clock sReloj;
    float time=0;
    sf::Event event;
    AnimationPlayer anim[3];
    sf::Vector2f sPos = sprP.getPosition()+sf::Vector2f(-5, 20);
    anim[0].AddAnimation(new AnimationPos(&sprW, sPos, sPos+sf::Vector2f(-2, 10), 1.5));
    anim[0].AddAnimation(new AnimationPos(&sprW, sPos+sf::Vector2f(-2, 10), sPos+sf::Vector2f(2, 0), 2));
    anim[0].AddAnimation(new AnimationPos(&sprW, sPos+sf::Vector2f(2, 0), sPos+sf::Vector2f(0, -10), 1));
    anim[0].AddAnimation(new AnimationPos(&sprW, sPos+sf::Vector2f(0, -10), sPos, 1.5));
    anim[1].AddAnimation(new AnimationRot(&sprW, 0, -10, 2));
    anim[1].AddAnimation(new AnimationRot(&sprW, -10, 0, 1));
    anim[1].AddAnimation(new AnimationRot(&sprW, 0, 5, 2));
    anim[1].AddAnimation(new AnimationRot(&sprW, 5, 0, 1));
    anim[2].AddAnimation(new AnimationSiz(&sprB, sf::Vector2f(1.5, 1.5), sf::Vector2f(1.3, 1.3), Global::randomFloat(1, 2)));
    anim[2].AddAnimation(new AnimationSiz(&sprB, sf::Vector2f(1.3, 1.3), sf::Vector2f(1.5, 1.5), Global::randomFloat(1, 2)));
    anim[2].AddAnimation(new AnimationSiz(&sprB, sf::Vector2f(1.5, 1.5), sf::Vector2f(1.6, 1.6), Global::randomFloat(1, 2)));
    anim[2].AddAnimation(new AnimationSiz(&sprB, sf::Vector2f(1.6, 1.6), sf::Vector2f(1.5, 1.5), Global::randomFloat(1, 2)));

    while (mainState == 0){
        Time = reloj.restart().asSeconds();
        sf::Vector2f mPos = getMousePos();

        while (window->pollEvent(event)){
			if (event.type == sf::Event::Closed)
                mainState = -1;
		}

        if(mbt[0].getBounds().contains(mPos)){
            if(fSave){
                mbt[0].setColor('t', sf::Color::Red);
                mbt[0].setTextSize(35);
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && time>0.2){
                    //mainState = 2;
                }
            }else{
                mbt[0].setColor('t', sf::Color(169, 169, 169));
            }
        }else{
            mbt[0].setTextSize(32);
            mbt[0].setColor('t', sf::Color::White);
        }

        if(mbt[1].getBounds().contains(mPos)){
            mbt[1].setColor('t', sf::Color::Red);
            mbt[1].setTextSize(35);
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && time>0.2){
                mainState = 1;
            }
        }else{
            mbt[1].setTextSize(32);
            mbt[1].setColor('t', sf::Color::White);
        }

        if(mbt[3].getBounds().contains(mPos)){
            mbt[3].setColor('t', sf::Color::Red);
            mbt[3].setTextSize(35);
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && time>0.5){
                mainState = -1;
            }
        }else{
            mbt[3].setTextSize(32);
            mbt[3].setColor('t', sf::Color::White);
        }

        if(mbt[2].getBounds().contains(mPos)){
            if(fScores){
                mbt[2].setColor('t', sf::Color::Red);
                mbt[2].setTextSize(35);
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && time>0.2){
                    mainState = 3;
                }
            }else{
                mbt[2].setColor('t', sf::Color(169, 169, 169));
            }
        }else{
            mbt[2].setTextSize(32);
            mbt[2].setColor('t', sf::Color::White);
        }
        playerBG->Update(Time);
        sprP.setTextureRect(playerBG->tRect);
        anim[0].Update(Time);
        anim[1].Update(Time);
        anim[2].Update(Time);
        window->clear(sf::Color(24, 24, 24));
        window->draw(sprB);
        window->draw(sprP);
        window->draw(sprW);
        for(int i=0;i<4;i++)
            mbt[i].drawButton(window);
        window->display();
        time += sReloj.restart().asSeconds();
    }

}

void Menu::selecTamMap(int& tamMap){
    int tam;
    unsigned int mult = 2;
    bool activo = true;
    sf::Text tamText;
    tamText.setFont(*f);

    sf::Texture p;
    p.loadFromFile("sprites/up.png");
    p.setSmooth(true);
    sf::Sprite flecha[2];
    for(int k=0;k<2;k++){
        flecha[k].setTexture(p);
        flecha[k].setScale(0.6, 0.6);
        flecha[k].setOrigin(flecha[k].getLocalBounds().width/2.f, flecha[k].getLocalBounds().height/2.f);
    }

    mbt[4].setPosition((window->getSize().x/2.f)-100, window->getSize().y/3.f);
    mbt[5].setPosition((window->getSize().x/2.f), (window->getSize().y/2.f)+100);
    tamText.setPosition((window->getSize().x/2.f)+85, (window->getSize().y/3.f)-5);
    tamText.setCharacterSize(33);
    flecha[1].setRotation(180);

    while (window->isOpen() && activo){
        sf::Vector2f mPos = getMousePos();
        sf::Event event;
        flecha[0].setPosition(tamText.getPosition().x+4, tamText.getPosition().y+40);
        flecha[1].setPosition(tamText.getPosition().x+4, tamText.getPosition().y-22);

        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();

            if(event.type == sf::Event::MouseWheelScrolled){
                if(event.mouseWheelScroll.delta == -1)
                    mult--;
                if(event.mouseWheelScroll.delta == 1)
                    mult++;
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
                activo = false;
                tamMap = -1;
            }
            if(flecha[0].getGlobalBounds().contains(mPos)){
                flecha[0].setColor(sf::Color::Red);
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    mult--;
            }else{
                flecha[0].setColor(sf::Color::White);
            }
            if(flecha[1].getGlobalBounds().contains(mPos)){
                flecha[1].setColor(sf::Color::Red);
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    mult++;
            }else{
                flecha[1].setColor(sf::Color::White);
            }
        }
        if(mult == 1){
            mult = 2;
        }
        if(mult > 30){
            mult = 30;
        }
        tam = mult * 50;
        tamText.setString(std::to_string(tam));
        tamText.setOrigin(tamText.getLocalBounds().width/2, tamText.getLocalBounds().height/2);

        if(mbt[5].getBounds().contains(mPos)){
            mbt[5].setColor('t', sf::Color::Red);
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                tamMap = tam;
                activo = false;
            }
        }else{
            mbt[5].setColor('t', sf::Color::White);
        }

        window->clear(sf::Color(24, 24, 24));
        mbt[4].drawButton(window);
        mbt[5].drawButton(window);
        window->draw(tamText);
        for(int i=0;i<2;i++){
            window->draw(flecha[i]);
        }
        window->display();
    }
}

void Menu::scoresMenu(){
    sf::RectangleShape topBody(sf::Vector2f(1024, 100));
    topBody.setFillColor(sf::Color(24, 24, 24));
    sf::Text top[3];
    top[0].setString("SEMILLA"); top[0].setPosition(170, 25);
    top[1].setString("MAPA"); top[1].setPosition(450, 25);
    top[2].setString("PUNTAJE"); top[2].setPosition(650, 25);
    int i, c=1, cantS = cantScores();
    for(i=0;i<3;i++){
        top[i].setFont(*f);
        top[i].setCharacterSize(40);
    }

    std::vector<guiScore> ranking(cantS, guiScore(f));
    Score reg;
    for(i=0;i<cantS;i++){
        reg.leerDelDisco(i);
        ranking[i]=reg;
    }
    ordenarRanking(ranking);
    while (window->isOpen() && mainState==3){
        sf::Vector2f mPos = getMousePos();
        sf::Event event;

        while (window->pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window->close();
            if(cantS>5){
                if(event.type == sf::Event::MouseWheelScrolled){
                    if(event.mouseWheelScroll.delta == -1){
                        c--;
                        if(c<cantS*-3) c = cantS*-3;
                    }
                    if(event.mouseWheelScroll.delta == 1){
                        c++;
                        if(c>1) c = 1;
                    }
                }
            }
            if(event.type == sf::Event::KeyReleased){
                if(event.key.code == sf::Keyboard::Escape)
                    mainState = 0;
                if(event.key.code == sf::Keyboard::Return)
                    c=1;
            }
        }

        for(i=0;i<cantS;i++){
            ranking[i].setPosition(window->getSize().x/6, 100+i*100+(c*30));
            if(ranking[i].body.getGlobalBounds().contains(getMousePos()))
                ranking[i].body.setFillColor(sf::Color(38, 38, 38));
            else
                ranking[i].body.setFillColor(sf::Color(24, 24, 24));
        }

        window->clear(sf::Color(24, 24, 24));
        for(i=0;i<cantS;i++){
            window->draw(ranking[i].body);
            window->draw(ranking[i].score);
            window->draw(ranking[i].seed);
            window->draw(ranking[i].tamM);
        }
        window->draw(topBody);
        for(i=0;i<3;i++)
            window->draw(top[i]);
        window->display();
    }
}

int Menu::cantScores(){
    FILE*f = fopen("scores.sk", "rb");
    fseek(f, 0, 2);
    int cantR = ftell(f)/sizeof(Score);
    return cantR;
}

void Menu::ordenarRanking(std::vector<guiScore>& ranking){
    int i, f;
    guiScore aux;
    for(i=0;i<ranking.size();i++){
        int max = i;
        for(f=i+1;f<ranking.size();f++){
            if(ranking[max].data < ranking[f].data){
                max = f;
            }
        }
        aux = ranking[max];
        ranking[max] = ranking[i];
        ranking[i] = aux;
    }
}

bool Menu::checkArchivo(const char *file){
    FILE*p = fopen(file, "rb");
    if(!p){
        return false;
    }else{
        fclose(p);
        return true;
    }
}

bool Menu::uploadData(sf::Ftp* sv){
    sv->connect("files.000webhost.com");
    sf::Ftp::Response resp = sv->login("7223", "kraryctes");
    sv->upload("scores.sk", "/");
    std::cout<<resp.getStatus()<<" "<<resp.getMessage()<<std::endl;
    sv->disconnect();
}
bool Menu::downloadData(sf::Ftp* sv){
    sv->connect("files.000webhost.com");
    sf::Ftp::Response resp = sv->login("7223", "kraryctes");
    sv->download("scores.sk", "");
    std::cout<<resp.getStatus()<<" "<<resp.getMessage()<<std::endl;
    sv->disconnect();
}
#endif // MENU_H_INCLUDED
