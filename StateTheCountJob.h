#ifndef STATETHECOUNTJOB_H_INCLUDED
#define STATETHECOUNTJOB_H_INCLUDED

#include "userStates.h"     // Make sure your state is listed in this include.
/*
    foxblock got interested in C++ programming, so fear his second creation
*/


#include "Sprite.h"
#include "NumberUtility.h"
#include "Vector2df.h"

/// const
#define INSTRUCTIONS_TIME 10

class StateTheCountJob : public BaseState
{
    public:
        StateTheCountJob();
        virtual ~StateTheCountJob();
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

        virtual bool checkPos(vector<Vector2df>::iterator curr, int arr);

        /// visuals
        Text pauseText;
        Text text;
        Timer counter;
        Sprite circle[5];
        Sprite selection;

        /// sounds
        Sound sTime1;
        Sound sTime2;
        Sound sAlarm;
        Sound sSelection;

        /// vars
        vector<Vector2df> pos[5];
        bool colourBlind;
        bool inputMode;
        int circleLevel;
        int diffLevel;
        int showTime;
        int winTime;
        int selected;
        int pauseMayhem;
};

#endif // STATETHECOUNTJOB_H_INCLUDED
