#ifndef HERO_H
#define HERO_H

#include "AnimatedSprite.h"
#include "Sprite.h"
#include "Sound.h"

class HeroBird
{
    public:
        HeroBird();
        ~HeroBird();
        void update();
        #ifdef PENJIN_SDL
            void render(SDL_Surface* screen);
        #else
            void render();
        #endif
        void wingFlap();

        void setOnPlatform(){onplatform = true;}
        bool getOnPlatform()const{return onplatform;}
        void resetOnPlatform(){onplatform = false;}
        int getDirection()const{return direction;}
        void switchDirection(){direction = -direction;}
        void setDirection(CRbool x){direction = x;}
        float getXVelocity()const{return velocity.x;}
        float getYVelocity()const{return velocity.y;}
        void setXVelocity(CRfloat x){velocity.x = x;}
        void setYVelocity(CRfloat y){velocity.y = y;}
        void setVelocity(CRfloat x,CRfloat y){velocity.x = x;velocity.y = y;}
        void changeXVelocity(CRfloat x){velocity.x = velocity.x + x;}
        void changeYVelocity(CRfloat y){velocity.y = velocity.y + y;}
        void setX(CRint x){position.x = x;}
        void setY(CRint y){position.y = y;}
        int getX()const{return position.x;}
        int getY()const{return position.y;}
        Vector2di getPosition()const{return position;}
        void setPosition(const Vector2di& pos){position = pos;}
        void setPosition(const Vector2dx& position){this->position.x = position.x.intValue >>16;this->position.y = position.y.intValue >>16;}
        void setPosition(CRint x,CRint y){setPosition(Vector2di(x,y));}
        void setPosition(CRFixed x,CRFixed y){setPosition(Vector2dx(x,y));}

    private:
        bool onplatform;
        bool direction;
        Vector2df velocity;
        Vector2di position;
        AnimatedSprite bird;
        AnimatedSprite birdright;
        Sprite birdstatic;
        AnimatedSprite herowalking;
        AnimatedSprite herowalkingright;
        Sound wingflap;
        Sound walking;
        int livesleft;

};

#endif // HERO_H
