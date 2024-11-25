#include "animation.h"
#include <iostream>
Animation::Animation(sf::Texture* texture, int initRow, sf::Vector2u _imgCount, float _frameTime, bool ppMode){
    currentImage = {0, initRow};
    frameTime = _frameTime;
    totalTime = 0;
    imgCount = _imgCount;
    pingpong = ppMode;

    tRect.width = texture->getSize().x/imgCount.x;
    tRect.height = texture->getSize().y/imgCount.y;
}

void Animation::Update(float deltaTime){
    totalTime+=deltaTime;
    if(totalTime >= frameTime){
        totalTime = 0;
        if(pingpong){
            if(endloop){
                if(currentImage.x == 0){
                    endloop = 0;
                    currentImage.x++;
                }
                else
                    currentImage.x--;
            }
            else{
                if(currentImage.x == imgCount.x-1){
                    endloop = 1;
                    currentImage.x--;
                }
                else
                    currentImage.x++;
            }
        }else{
            currentImage.x++;
            if(currentImage.x >= imgCount.x){
                currentImage.x = 0;

            }
        }
    }
    tRect.left = currentImage.x*tRect.width;
    tRect.top = currentImage.y*tRect.height;
}

void Animation::setRow(int row){
    currentImage.y = row;
}

void Animation::setFrameTime(float _frameTime){
    totalTime = 0;
    frameTime = _frameTime;
}

void Animation::setPPMode(bool mode){
    pingpong = mode;
}

GuiDialog::GuiDialog(char* text, sf::Font& f, sf::Vector2f position, sf::Color color, unsigned int csize, float time){
    enable = true;
    dialog.setFont(f);
    dialog.setPosition(position);
    dialog.setFillColor(color);
    dialog.setString(text);
    if(csize != 0)
        dialog.setCharacterSize(csize);
    showTime = time;
}

void GuiDialog::Update(float deltaTime){
    if(showTime != 0){
        totalTime+=deltaTime;
        if(totalTime >= showTime)
            enable = false;
    }
}
void GuiDialog::Draw(sf::RenderWindow* window){
    if(enable)
        window->draw(dialog);
}
sf::Vector2f GuiDialog::getPos(){
    return dialog.getPosition();
}
void GuiDialog::setEnable(bool state){
    enable = state;
}
bool GuiDialog::getEnable(){
    return enable;
}

AnimationProp::AnimationProp(sf::Sprite* _spr, float _duration){
    spr = _spr;
    end = false;
    totalTime = 0;
    duration = _duration;
}
bool AnimationProp::isEnd(){
    return this->end;
}

AnimationPos::AnimationPos(sf::Sprite* _spr, sf::Vector2f startPos, sf::Vector2f endPos, float _duration) : AnimationProp(_spr, _duration){
    movePos = {endPos.x - startPos.x, endPos.y - startPos.y};
    velX = (1.f/60.f)*(movePos.x/duration);
    velY = (1.f/60.f)*(movePos.y/duration);
}
AnimationRot::AnimationRot(sf::Sprite* _spr, float startPos, float endPos, float _duration) : AnimationProp(_spr, _duration){
    rotation = endPos - startPos;
    vel = (1.f/60.f)*(rotation/duration);
}
AnimationSiz::AnimationSiz(sf::Sprite* _spr, sf::Vector2f startPos, sf::Vector2f endPos, float _duration) : AnimationProp(_spr, _duration){
    size = endPos - startPos;
    vel = (1.f/60.f)*(size/duration);
}
void AnimationProp::Update(float time){
    totalTime+=time;
    if(totalTime >= duration){
        end = true;
    }
}
void AnimationProp::Reset(){
    end = false;
    totalTime = 0;
}
void AnimationPos::Update(float time){
    if(!end)
        spr->move(velX, velY);
    AnimationProp::Update(time);
}

void AnimationRot::Update(float time){
    if(!end)
        spr->rotate(vel);
    AnimationProp::Update(time);
}

void AnimationSiz::Update(float time){
    if(!end)
        spr->setScale(spr->getScale()+vel);
    AnimationProp::Update(time);
}

void AnimationPlayer::AddAnimation(AnimationProp* anim){
    anims.push_back(anim);
}

void AnimationPlayer::Update(float deltaTime){
    if(anims[currentAnim]->isEnd()){
        currentAnim++;
        if(currentAnim == anims.size()){
            currentAnim = 0;
            Reset();
        }
    }
    anims[currentAnim]->Update(deltaTime);
}
void AnimationPlayer::Reset(){
    for(int i=0;i<anims.size();i++){
        anims[i]->Reset();
    }
}
