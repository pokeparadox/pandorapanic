/*
    Panic Attack minigame for Pandora Panic
    Release: 18 Feb 2009
    Authors: Todd Foster, code
             TheBrainSquid, buttons graphics
             Gruso, soundtrack
    License: GPLv3
*/


#ifndef STATEPANICATTACK_H_INCLUDED
#define STATEPANICATTACK_H_INCLUDED

#include "mgBaseState.h"
#include "PenjinStates.h"     // Make sure your state is listed in this include.

#include "Background.h"
#include "Sprite.h"
#include "Text.h"
#include "Random.h"
#include <vector>
#include "StatePanicAttackButtonsEvents.h"


class Debounce {
  public:
    Debounce(int numButtons, int delay) {
        _delay=delay;
        _timer.setMode(MILLI_SECONDS);
        _timer.start();
        _buttons.resize(numButtons); //init's elements to 0
    }
    void block() {
        int t = _timer.getScaledTicks() + _delay;
        for(int i=0; i<(int)_buttons.size(); i++)
          _buttons[i]=t;
    }
    bool ready(PA_BUTTON button) {
      int t = _timer.getScaledTicks();
      bool result = _buttons[button] < t;
      _buttons[button] = t + _delay;
      return result;
    }

  private:
    vector<int> _buttons;
    Timer _timer;
    int _delay;
};


class StatePanicAttack : public mgBaseState
{
  public:
    StatePanicAttack();
    ~StatePanicAttack();
    virtual void init();
    #ifdef PENJIN_SDL
        virtual void render(SDL_Surface *screen);
        virtual void pauseScreen(SDL_Surface* screen);
    #else
        virtual void render();
        virtual void pauseScreen();
    #endif

    virtual void update();
    virtual void userInput();
    virtual void onPause();
    virtual void onResume();

    static int level;
    static int gameInstance;

  private:
    Background instructionsBackground;
    Background playBackground;
    Image dontPanic;
    Image panics;
    Timer panicsTimer;
    int panicsCurrent;
    const static int PANICS_DELAY = 1500;
    RandomClass panicsIndex;

    Music backMusic;
    Sound buttonSound;
    Sound buzzer;

    StatePanicAttackButtonEvents *events;
    Timer timer;
    Debounce *debounce;

    int gameLength;
    int beats;
    PA_BUTTON buttonsAvailable;
    int buttonsConcurrent;
    int buttonsMinSpacing;
    int nextButtonAdded;
    RandomClass randomTime;
    RandomClass randomButton;

    const static int LOSER_DELAY = 1000;
    const static int INIT_DELAY = 1500;
    const static int HEARTBEAT_SPACING = 1600;

    Text text;
    Text pauseText;

    void finishedInit();

    #ifdef PENJIN_SDL
        void renderProgress (SDL_Surface *screen);
        void renderPanics(SDL_Surface *screen);
    #else
        void renderProgress();
        void renderPanics();
    #endif
    enum PANICATTACK_STATE {
      PANICATTACK_INIT,
      PANICATTACK_PLAYING,
      PANICATTACK_LOST,
      PANICATTACK_WON
    };
    int panicAttackState;
};

#endif // STATERHYTHM_H_INCLUDED
