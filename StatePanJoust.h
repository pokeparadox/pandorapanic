#ifndef STATEPANJOUST_H_INCLUDED
#define STATEPANJOUST_H_INCLUDED

#include "mgBaseState.h"
#include "PenjinStates.h"     // Make sure your state is listed in this include.

#include "Background.h"
#include "Sprite.h"
#include "TextDoc.h"
#include "Random.h"
#include "ButtonPrompter.h"
#include "PanJoust/HeroBird.h"
#include "Emitter.h"

class StatePanJoust : public mgBaseState
{
    public:
        StatePanJoust();
        virtual void init();
        #ifdef PENJIN_SDL
            virtual void render(SDL_Surface *screen);
            virtual void pauseScreen(SDL_Surface* screen);
        #else
            virtual void render();
            virtual void pauseScreen();
        #endif
        virtual void update();
        virtual void onPause();
        virtual void onResume();
        virtual void userInput();
        virtual ~StatePanJoust();

    private:
        AnimatedSprite enemy[40];
        Text text;
        Timer teller;
        Timer inputlimiter;
        Background background;
        Sound onenemy;
        Music joustmusic;
        Sprite *platform;
        Sprite ground;
        HeroBird hero1;

        int enemynumber;
        int killcount;
        int gamelength;
        int temprand;
        int enemydead[40];
        int enemyonplatform[20];
        int enemybelowplatform[20];
        Vector2di enemyvel[40];

        bool youdead;
        bool youwin;
        bool panic;
        bool touchedplatform;
        Emitter jet;


        int platformnumber;
        //ButtonPrompter buttonPrompter;

};
//The tile



#endif // STATEPANJOUST_H_INCLUDED
