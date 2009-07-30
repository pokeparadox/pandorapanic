#ifndef STATEPONG_H_INCLUDED
#define STATEPONG_H_INCLUDED

#include "userStates.h"     // Make sure your state is listed in this include.
/*
    Miggles' Pong game for Pandora Panic!
*/


#include "Background.h"
#include "Sprite.h"

class StatePong : public BaseState
{
    public:
        StatePong();
        virtual void init();
        #ifdef PENJIN_SDL
            virtual void render(SDL_Surface *screen);
        #else
            virtual void render();
        #endif
        virtual void update();
        virtual void unlimitedUpdate();

        virtual ~StatePong();
        virtual void userInput();

        Background background;
        Sprite  playerPaddle, cpuPaddle, ball;
        Sound   bounceWallSound, bouncePaddleSound, bounceOutSound;
        Timer   endGame;
        bool    bounceWall, bouncePaddle, bounceOut, gameOver;
        float   ballUp, ballRight, ballVelocity, maxBallVelocity;
        int     paddleSpeed, cpuPaddleSpeed, cpuPaddleZone, playerPaddleVelocity, cpuPaddleVelocity, playerWin, collision, collisionDeflection;

        int     screenWidth, screenHeight, paddleWidth, paddleHeight, ballWidth, ballHeight;

        Text text;
};

#endif // STATEPONG_H_INCLUDED
