#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <cstdlib>
#include <math.h>
#include <string>
#include <time.h>
#include <vector>
#include "weapon.h"
#include "map.h"
#include "player.h"
#include "enemy.h"
#include "mouse.h"
#include "level.h"
#include "juego.h"
#include "menu.h"

int main()
{
    short int mainState = 0;
    sf::RenderWindow window(sf::VideoMode(1024, 600), "SeeK"); // 1024,600
    window.setFramerateLimit(60);
    sf::Ftp server;
    Global::currentWindow = &window;
	Menu *menuPrincipal = new Menu(&window, mainState);
	//menuPrincipal->downloadData(&server);
	Game *juego;
	while (window.isOpen()){
		sf::Event event;

        switch(mainState){
            case -1:
                window.close();
                break;
            case 0:
                menuPrincipal->mainMenu();
                break;
            case 1:
                {
                    int tamMap;
                    menuPrincipal->selecTamMap(tamMap);
                    if(tamMap!=-1){
                        window.setMouseCursorVisible(false);
                        juego = new Game(tamMap, &window);
                        juego->iniciarJuego();
                        juego->gameLoop();
                        window.setView(window.getDefaultView());
                        window.setMouseCursorVisible(true);
                    }
                    //menuPrincipal->uploadData(&server);
                    mainState = 0;
                    break;
                }
            case 2:
                {
                    window.setMouseCursorVisible(false);
                    juego = new Game(&window);
                    juego->iniciarJuego();
                    juego->gameLoop();
                    juego=NULL;
                    window.setView(window.getDefaultView());
                    mainState = 0;
                    window.setMouseCursorVisible(true);
                    break;
                }
            case 3:
                menuPrincipal->scoresMenu();
                break;
        }
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed)
                window.close();
		}
	}
    return 0;
}

/*
//unsigned int tamMap;
    Menu menuPrincipal(800, 600);
    menuPrincipal.mainMenu();
    int seed = time(NULL);
    srand(seed);
    std::cout<<"Semilla: "<<seed<<std::endl;
    Game altoJuego(50, menuPrincipal.getWindow());*/
