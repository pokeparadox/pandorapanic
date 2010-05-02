#ifndef STATECAKE_H
#define STATECAKE_H

#include "mgBaseState.h"
#include "PenjinStates.h"

#include "AnimatedSprite.h"
#include "Sprite.h"
#include "Background.h"
#include "CollisionMap.h"
#include "HitRegion.h"
#include "Text.h"
#include "CountDown.h"

class StateCake : public mgBaseState
{
    public:
        StateCake();
        virtual ~StateCake();

        virtual void init();
        virtual void render(SDL_Surface* screen);
        virtual void userInput();
        virtual void update();
        virtual void onPause();
        virtual void onResume();
        virtual void pauseScreen(SDL_Surface* screen);

    private:
        enum DIRECTION
        {
            UP=0,
            DOWN,
            LEFT,
            RIGHT,
        };
        enum NUMBERS
        {
            NUM_CANDLES = 6
        };
        class Candle
        {
            public:
                Candle()
                {
                    lit = true;
                    candle.setCurrentFrame( Random::nextInt(0,3));
                    flame.loadFrames("images/BirthdayCake/flame.png",4,1);
                    candle.loadFrames("images/BirthdayCake/candles.png",4,1);
                    setPosition(Vector2df(50,50));
                    timer.setMode(MILLI_SECONDS);
                }
                void setPosition(Vector2df pos){position = pos;candle.setPosition(pos);flame.setPosition(pos.x+6,pos.y-12);}
                Vector2df getPosition()const{return position;}
                void render(SDL_Surface* screen){candle.render(screen);if(lit)flame.render(screen);}
                void update()
                {
                    timer.update();
                    if(lit)
                        flame.update();
                    if(timer.hasFinished())
                        light();
                }
                void blow(){lit = false;timer.start();}
                void light(){lit = true;}
                bool isLit()const{return lit;}
                void setRelightLimit(CRint ms){timer.setLimit(ms);}
            private:
                bool lit;
                Vector2df position;
                AnimatedSprite flame;
                AnimatedSprite candle;
                CountDown timer;
        };
        class Player
        {
            public:
                Player()
                {
                    direction = DOWN;
                    stand = true;
                    blowing = false;
                    position = Vector2df(400-((192/4)/2),240-42);
                    anim[UP].loadFrames("images/BirthdayCake/walkup.png",4,1);
                    anim[DOWN].loadFrames("images/BirthdayCake/walkdown.png",4,1);
                    anim[LEFT].loadFrames("images/BirthdayCake/walkleft.png",4,1);
                    anim[RIGHT].loadFrames("images/BirthdayCake/walkright.png",4,1);
                    anim[direction].setPosition(position);
                    anim[UP].setFrameRate(DECI_SECONDS);
                    anim[DOWN].setFrameRate(DECI_SECONDS);
                    anim[LEFT].setFrameRate(DECI_SECONDS);
                    anim[RIGHT].setFrameRate(DECI_SECONDS);
                    blowSpr.loadFrames("images/BirthdayCake/blow.png",4,1);
                    blowSpr.setAlpha(240);
                    blowSpr.update();
                    map.loadImage("images/BirthdayCake/cakecollision.png");
                    shadow.loadSprite("images/BirthdayCake/shadow.png");
                    shadow.setAlpha(70);
                    hitRegion = NULL;
                    hitRegion = new HitRegion;
                    hitRegion->init(position.x + 80, position.y + 128, 32, 32);
                    speed = Vector2df(0,0);
                }
                ~Player()
                {
                    if(hitRegion)
                        delete hitRegion;
                }
                void render(SDL_Surface* screen)
                {
                    shadow.render(screen);
                    anim[direction].render(screen);
                    if(blowing)
                    {
                        blowSpr.render(screen);
                        if(direction == UP)
                            anim[direction].render(screen);
                    }

                #ifdef _DEBUG
                    hitRegion->render();
                #endif
                }
                void update()
                {
                    if (speed.x < 0)
                        speed.x -= max(-0.5f,speed.x);
                    else if (speed.x > 0)
                        speed.x -= min(0.5f,speed.x);

                    if (speed.y < 0)
                        speed.y -= max(-0.5f,speed.y);
                    else if (speed.y > 0)
                        speed.y -= min(0.5f,speed.y);

                    hitRegion->moveTo(position.x + 8.5f + speed.x,position.y + 64.5f + speed.y);
                    if(hitRegion->hitTest(&map,0,0))
                    {
                        speed = Vector2df(0,0);
                        hitRegion->moveTo(position.x + 8.5f, position.y + 64.5f);
                    }

                    position += speed;
                    anim[direction].setPosition(position);
                    shadow.setPosition(position.x+8,position.y + anim[direction].getHeight()-18);

                    if (direction == LEFT)
                        blowSpr.setPosition(position.x-6,position.y +12);
                    else if (direction == RIGHT)
                        blowSpr.setPosition(position.x+38,position.y +12);
                    else if (direction == UP)
                        blowSpr.setPosition(position.x+16,position.y - 2);
                    else
                        blowSpr.setPosition(position.x+16,position.y +24);
                    blowSpr.setCurrentFrame(direction);

                    if(stand)
                        return;
                    anim[direction].update();
                }
                Vector2df getPosition()const{return position;}
                int getX()const{return hitRegion->getX();}
                int getY()const{return hitRegion->getY();}
                void left()
                {
                    direction = LEFT;
                    if (speed.x >= -5)
                        speed.x -= 1.5;
                    stand = false;
                    blowing = false;
                    canBlow = false;
                }
                void right()
                {
                    direction = RIGHT;
                    if (speed.x <= 5)
                        speed.x += 1.5;
                    stand = false;
                    blowing = false;
                    canBlow = false;
                }
                void up()
                {
                    direction = UP;
                    if (speed.y >= -5)
                        speed.y -= 1.5;
                    stand = false;
                    blowing = false;
                    canBlow = false;
                }
                void down()
                {
                    direction = DOWN;
                    if (speed.y <= 5)
                        speed.y += 1.5;
                    stand = false;
                    blowing = false;
                    canBlow = false;
                }
                void stop(){anim[direction].setCurrentFrame(0);stand = true;canBlow = blowing =false;}
                void blow()
                {
                    blowing = true;
                    if(!canBlow)
                    {
                        if(hitRegion->colourTest(&map) == Colour(0,0,(uchar)255))
                        {
                            canBlow = true;
                        }
                    }
                }
                bool canBlowCandle()const{return canBlow;}
                HitRegion* getHitRegion(){return hitRegion;}
            private:
                AnimatedSprite anim[4];
                Sprite shadow;
                AnimatedSprite blowSpr;
                CollisionMap map;
                HitRegion* hitRegion;
                DIRECTION direction;
                Vector2df position;
                Vector2df oldPos;
                Vector2df speed;
                bool stand;
                bool blowing;
                bool canBlow;
        };

        Background back;
        Player mariela;
        Candle candles[NUM_CANDLES];
        CountDown timer;
        Text text;
        AnimatedSprite buttonSheet;
        int limit;
        Music mus;
};

#endif // STATECAKE_H
