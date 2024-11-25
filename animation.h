#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED
#include <SFML/Graphics.hpp>

class Animation{
    public:
        Animation(sf::Texture* texture, int initRow, sf::Vector2u _imgCount, float _frameTime, bool ppMode);
        void Update(float);
        void setFrameTime(float);
        void setRow(int);
        void setPPMode(bool);
        sf::IntRect tRect;
    private:
        sf::Vector2u imgCount;
        sf::Vector2u currentImage;
        float totalTime;
        float frameTime;
        bool pingpong;
        bool endloop;
};

class GuiDialog{
    public:
        GuiDialog(char* text, sf::Font& font, sf::Vector2f position, sf::Color color, unsigned int csize = 0, float time = 0);
        void Update(float deltaTime);
        void Draw(sf::RenderWindow* window);
        sf::Vector2f getPos();
        void setEnable(bool state);
        bool getEnable();
    private:
        bool enable;
        sf::Text dialog;
        float showTime;
        float totalTime;
};

class AnimationProp{
    protected:
        sf::Sprite* spr;
        float totalTime;
        float duration;
        bool end;
    public:
        AnimationProp(sf::Sprite* _spr, float _duration);
        virtual void Update(float deltaTime);
        bool isEnd();
        void Reset();
};

class AnimationPos: public AnimationProp{
    public:
        AnimationPos(sf::Sprite* _spr, sf::Vector2f startPos, sf::Vector2f endPos, float duration);
        void Update(float deltaTime);
    private:
        sf::Vector2f movePos;
        float velX, velY;
};

class AnimationRot: public AnimationProp{
    public:
        AnimationRot(sf::Sprite* _spr, float startPos, float endPos, float duration);
        void Update(float deltaTime);
    private:
        float rotation, vel;
};

class AnimationSiz: public AnimationProp{
    public:
        AnimationSiz(sf::Sprite* _spr, sf::Vector2f startPos, sf::Vector2f endPos, float duration);
        void Update(float deltaTime);
    private:
        sf::Vector2f size, vel;
};

class AnimationPlayer{
    public:
        void AddAnimation(AnimationProp* anim);
        void Update(float deltaTime);
    private:
        std::vector<AnimationProp*> anims;
        void Reset();
        int currentAnim = 0;
};
#endif // ANIMATION_H_INCLUDED
