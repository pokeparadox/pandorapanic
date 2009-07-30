#ifndef BUTTONPROMPTER_H_INCLUDED
#define BUTTONPROMPTER_H_INCLUDED

/*
    ButtonPrompter utility for Pandora Panic
    Release: 6 Dec 2008
    Author: Todd Foster
    License: GPLv3
    Images by TheBrainSquid based on model by Miggles
    discussion at: http://www.gp32x.com/board/index.php?showtopic=44652&st=255&gopid=676340&#entry676340

    USAGE:

    1. #include "ButtonPrompter.h"
    2. Get an instance of ButtonPrompter.
    3. Put a "buttonPrompter.render(screen);" in your render method
    4. Place events in the queue with "buttonPrompter.display().

    ButtonPrompter assumes (and demands with the default constructor)
    a default spritesheet. You may specify your own in the other
    constructor (untested) or with:
    buttonPrompter.setImages("altLocation/backwards.png", 10);


    display can be passed all the relevant information in each call.
    Or you can set default values which will be filled in for missing
    parameters.

    Examples:

    buttonPrompter.setFlash(false);
    buttonPrompter.setFlashDelay(1000);
    buttonPrompter.setFlashQuantity(8);
    buttonPrompter.setDefaultX(400).setDefaultY(240).setDefaultButton(ButtonPrompter::BUTTON_SHOULDERLEFT).display();
    buttonPrompter.display(ButtonPrompter::BUTTON_A, LOGO_INIT_X - 10, LOGO_INIT_Y - 28);
*/


#include "Sprite.h"
#ifdef PLATFORM_GP2X
    #include "MMUHack.h"
#endif

#include "Timer.h"

#include <vector>

class ButtonEvent {
  public:
    ButtonEvent(int button, int x, int y, int startTime, int endTime) {
        _button=button;
        _x=x;
        _y=y;
        _startTime = startTime;
        _endTime=endTime;
        }
    int button() { return _button; };
    int x() { return _x; };
    int y() { return _y; };
    int startTime() { return _startTime; }
    int endTime() { return _endTime; };

  private:
    int _button, _x, _y, _startTime, _endTime;
};


class ButtonPrompter : public Image
{
  public:
    ButtonPrompter();
    ButtonPrompter(string filename, int cols);
    virtual ButtonPrompter& setImages(string filename, int cols);
    virtual ButtonPrompter& setFlash(bool on);
    virtual ButtonPrompter& setFlashDelay(int delay);
    virtual ButtonPrompter& setFlashQuantity(int quantity);
    virtual ButtonPrompter& setScreen(SDL_Surface *screen);
    virtual ButtonPrompter& setDefaultX(int x);
    virtual ButtonPrompter& setDefaultY(int y);
    virtual ButtonPrompter& setDefaultButton(int button);

    virtual void display(int button, int x, int y, bool useFlash, int flashDelay, int flashQuantity);
    virtual void display(int button, int x, int y);
    virtual void display(int x, int y);
    virtual void display(int button);
    virtual void display();

    virtual void render();


    enum BUTTON {
      BUTTON_A,
      BUTTON_B,
      BUTTON_X,
      BUTTON_Y,
      BUTTON_LEFT,
      BUTTON_RIGHT,
      BUTTON_UP,
      BUTTON_DOWN,
      BUTTON_SHOULDERLEFT,
      BUTTON_SHOULDERRIGHT
    };


  private:
    SDL_Surface* screen;
    virtual void initDefaults();

    int columns;

    Timer timer;

    bool defaultUseFlash;
    int defaultFlashDelay;
    int defaultFlashQuantity;
    SDL_Surface *defaultScreen;
    int defaultX;
    int defaultY;
    int defaultButton;

    std::vector<ButtonEvent> queue;


};

#endif // BUTTONPROMPTER_H_INCLUDED
