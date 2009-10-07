#ifndef STATEBADVISTA_H_INCLUDED
#define STATEBADVISTA_H_INCLUDED

#include "userStates.h"     // Make sure your state is listed in this include.
/*
    BadVista minigame for Pandora Panic
    Release: 4 Oct 2009
    Authors: Todd Foster, code
             Gruso, soundtrack
    License: GPLv3
    Image adapted from John Sullivan's "No littering" logo, licensed under
      GNU FDL Version 1.2, available at www.badvista.org
*/

#include "Background.h"
#include "Sprite.h"
#include <vector>
#include "ButtonPrompter.h"
#include "Text.h"

class SimpleRegion {
  public:
    SimpleRegion(int x1, int y1, int x2, int y2);
    bool intersects(SimpleRegion obstacle);
    int x1() const{ return _x1; };
    int y1() const{ return _y1; };
    int x2() const{ return _x2; };
    int y2() const{ return _y2; };

  private:
    int _x1, _y1, _x2, _y2;
};



class StateBadVista : public BaseState
{
  public:
    StateBadVista();
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

  private:
    ButtonPrompter buttonPrompter;
    Background background;
    Sprite arm;
    Sprite logo;

    Sprite loserBackground;
    Timer counter;
    Timer inputDelay;

    Music backsound;
    Sound bounce;

    Text pauseText;

    const static int LOSER_DELAY = 4;
    const static int END_DELAY = 1;

    std::vector<SimpleRegion> horizontalObstacles;
    std::vector<SimpleRegion> verticalObstacles;
    std::vector<SimpleRegion> playerObstacle;
    std::vector<SimpleRegion> basketLeft;
    std::vector<SimpleRegion> basketRight;

    bool hitObstacle(std::vector<SimpleRegion> obstacles, SimpleRegion motion);
    bool ballStopped();
    bool inBasket();

    enum BV_STATE {
      BV_INIT,
      BV_WINDUP,
      BV_PITCHED,
      BV_LOST,
      BV_WON,
      BV_PWNED
    };

    int numBounces;
    const static int MAX_NUM_BOUNCES = 15;

    int bvState;

    int armAngle;
    int armIncrement;

    bool pwned;
    int  pwned_count;
    const static int PWNED_STOPPED = 4;
    const static int PWNED_PWNED = 6;

    const static int INPUT_DELAY = 25; // used to throttle input

    const static int ARM_ANGLE_MAX = 85;
    const static int ARM_INIT_X = 600;
    const static int ARM_INIT_Y = 170;

    int logoX, logoY;
    float logoVelocityX, logoVelocityY;
#ifdef PENJIN_FIXED
    Fixed ANGLE_TO_VELOCITY_MULTIPLIER;
    Fixed GRAVITY;
    Fixed BOUNCE_DECAY;
#else
    const static float ANGLE_TO_VELOCITY_MULTIPLIER = 0.3f;
    const static float GRAVITY = 1.0f;
    const static float BOUNCE_DECAY = 0.85f;
#endif
    const static int LOGO_INIT_X = 620;
    const static int LOGO_INIT_Y = 315;

    const static int SCREEN_MAX_Y=479;
    const static int SCREEN_MAX_X = 799;
    const static int WALL_LEFT = 20;
    const static int WALL_RIGHT = 780;
    const static int FLOOR = 460;
    const static int CEILING = 20;
    const static int BAD_WIDTH = 170;
    const static int BAD_BOTTOM = 100;
    const static int VISTA_WIDTH = 235;
    const static int VISTA_TOP = 115;
    const static int VISTA_BOT = 180;

    const static int BASKET_LEFT_OUTSIDE = 150;
    const static int BASKET_LEFT_INSIDE = 160;
    const static int BASKET_CENTER = 150;
    const static int BASKET_RIGHT_INSIDE = 240;
    const static int BASKET_RIGHT_OUTSIDE = 255;
    const static int BASKET_TOP = 375;
    const static int BASKET_BOT = 440;

    const static int PERSON_X = 700;
    const static int PERSON_Y = 270;
};

#endif // STATEBADVISTA_H_INCLUDED
