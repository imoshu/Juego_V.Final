#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED
#include <SFML/Graphics.hpp>

class Button{
    private:
        sf::RectangleShape button;
        sf::Text text;
    public:
        Button(sf::Color, sf::Vector2f = sf::Vector2f(0, 0));
        void setString(const char *);
        void setTextSize(int);
        void setColor(const char, sf::Color);
        void setSize(sf::Vector2f);
        void setPosition(float x, float y);
        void setFont(sf::Font&);
        void centrar();
        void setAutoSize(float fX = 1, float fY = 1);
        sf::FloatRect getBounds(bool opc = 0);
        void drawButton(sf::RenderWindow * window);
};

#endif // BUTTON_H_INCLUDED
