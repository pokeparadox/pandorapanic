#ifndef STATEPANICATTACKBUTTONEVENTS_H_INCLUDED
#define STATEPANICATTACKBUTTONEVENTS_H_INCLUDED

/*
    StatePanicAttackButtonEvents
    Release: 20 Dec 2008
    Author: Todd Foster
    License: GPLv3
*/


#include "Sprite.h"
#include "Timer.h"
#include "Music.h"
#include "ButtonPrompter.h"

#include <vector>


    enum PA_BUTTON {
      PA_BUTTON_A,
      PA_BUTTON_B,
      PA_BUTTON_X,
      PA_BUTTON_Y,
      PA_BUTTON_LEFT,
      PA_BUTTON_RIGHT,
      PA_BUTTON_UP,
      PA_BUTTON_DOWN,
      PA_BUTTON_SHOULDERLEFT,
      PA_BUTTON_SHOULDERRIGHT,
      PA_NUM_BUTTONS
    };

enum PA_ICON_COLOR { PA_GREEN_ICON=0, PA_YELLOW_ICON, PA_RED_ICON, PA_NUM_ICON_COLORS };

class PAButtonEvent {
  public:
    PAButtonEvent(PA_BUTTON button, int x, int y, int time) {
        _button=button;
        _x=x;
        _y=y;
        _time=time;
        }
    PA_BUTTON button() { return _button; };
    int x() { return _x; };
    int y() { return _y; };
    int time() { return _time; };

  private:
    PA_BUTTON _button;
    int _x, _y, _time;
};


class StatePanicAttackButtonEvents : public Image
{
  public:
    StatePanicAttackButtonEvents();
    virtual void expect(PA_BUTTON button);
    #ifdef PENJIN_SDL
        virtual void render(SDL_Surface *screen);
    #else
        virtual void render();
    #endif

    virtual bool rm(PA_BUTTON button);
    int buttonCount(bool countShoulderButtons);
    bool lost()  { return loserButton != PA_NUM_BUTTONS; };
    void setPreview(int b) { preview = b; };
    void setResponseTime(int t) { responseTime = t; }

    void pause() { timer.pause(); };
    void unpause() { timer.unpause(); };

  private:
    Timer timer;
    int responseTime;
    Image bigRedX;
    std::vector<PAButtonEvent> queue;
    std::vector<Vector2di> location;
    PA_BUTTON loserButton;
    int preview;


    SDL_Surface *screen;
    void draw(PA_BUTTON button, PA_ICON_COLOR color);
    PA_ICON_COLOR whichButton(int t, PAButtonEvent e);

};

#endif // STATEPANICATTACKBUTTONEVENTS_H_INCLUDED
