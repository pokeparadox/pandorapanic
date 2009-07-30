#ifndef STATETHECOUNTJOB_H_INCLUDED
#define STATETHECOUNTJOB_H_INCLUDED

#include "userStates.h"     // Make sure your state is listed in this include.
/*
    foxblock became interested in C++ programming, so fear his second creation
*/


#include "Background.h"
#include "Sprite.h"

class StateTheCountJob : public BaseState
{
    public:
        StateTheCountJob();
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


        virtual ~StateTheCountJob();
        virtual void userInput();

        virtual bool checkPos(int curr);

        Text pauseText;
        Text text;
        Timer counter;
        Sprite circle[75];
        Sprite selection;

        Sound sTime1;
        Sound sTime2;
        Sound sAlarm;
        Sound sSelection;

        int arr[5];
        bool colourBlind;
        bool inputMode;
        int circleLevel;
        int diffLevel;
        int showTime;
        int winTime;
        int selected;
};

#endif // STATETHECOUNTJOB_H_INCLUDED
