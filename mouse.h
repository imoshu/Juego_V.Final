#ifndef MOUSE_H_INCLUDED
#define MOUSE_H_INCLUDED
#include "player.h"

class Mouse{
    private:
        sf::RenderWindow *window;
        sf::Texture p;
        sf::Sprite aim;
        sf::Vector2f mousePosWindow;
        //sf::Vector2f aimDirNorm;
    public:
        Mouse(sf::RenderWindow *);
        sf::Sprite getSpr();
        //sf::Vector2f getMouse();
        sf::Vector2f getMousePos();
        void updateMouse();
};

#endif // MOUSE_H_INCLUDED
