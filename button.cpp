#include "button.h"

Button::Button(sf::Color cls, sf::Vector2f bSize):button(bSize){
    button.setFillColor(cls);
    button.setOrigin(button.getLocalBounds().width/2, button.getLocalBounds().height/2);
}

void Button::setString(const char *f){
    text.setString(f);
}
sf::FloatRect Button::getBounds(bool opc){
    if(opc)
        return text.getGlobalBounds();
    else return button.getGlobalBounds();
}
void Button::setTextSize(int s){
    text.setCharacterSize(s);
}
void Button::setSize(sf::Vector2f tam){
    button.setSize(tam);
}
void Button::setPosition(float x, float y){
    text.setPosition(x, y);
    button.setPosition(text.getGlobalBounds().left+text.getGlobalBounds().width/2.f, text.getGlobalBounds().top+text.getGlobalBounds().height/2.f);
}
void Button::drawButton(sf::RenderWindow * window){
    window->draw(button);
    window->draw(text);
}
void Button::setFont(sf::Font& f){
    text.setFont(f);
}
void Button::setColor(const char p, sf::Color cls){
    if(p=='t')
        text.setColor(cls);
    if(p=='b')
        button.setFillColor(cls);
}
void Button::centrar(){
    button.setOrigin(button.getLocalBounds().width/2, button.getLocalBounds().height/2);
    text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
}
void Button::setAutoSize(float fX, float fY){
    button.setSize(sf::Vector2f(text.getGlobalBounds().width*fX, text.getGlobalBounds().height*fY));
    button.setOrigin(button.getLocalBounds().width/2, button.getLocalBounds().height/2);
}
