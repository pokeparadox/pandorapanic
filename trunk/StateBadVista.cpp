#include "StateBadVista.h"
/*
    BadVista minigame for Pandora Panic
    Release: 4 Oct 2009
    Authors: Todd Foster, code
             Gruso, soundtrack
    License: GPLv3
    Image adapted from John Sullivan's "No littering" logo, licensed under
      GNU FDL Version 1.2, available at www.badvista.org
    Music by Gruso
    Sound effects found on the web someplace

    TODO: Alt. backdoor for pwned screen: perhaps using keys "C A Delete" ?

*/


SimpleRegion::SimpleRegion(int x1, int y1, int x2, int y2) {
  _x1 = min(x1, x2);
  _x2 = max(x1, x2);
  _y1 = min(y1, y2);
  _y2 = max(y1, y2);
}

bool SimpleRegion::intersects(SimpleRegion obstacle) {
  return (
    (
      (x1() <= obstacle.x1() && obstacle.x1() <= x2())
      ||
      (x1() <= obstacle.x2() && obstacle.x2() <= x2())
    )
    &&
    (
      (y1() <= obstacle.y1() && obstacle.y1() <= y2())
      ||
      (y1() <= obstacle.y2() && obstacle.y2() <= y2())
    )
  );
}


StateBadVista::StateBadVista()
{

#ifdef PENJIN_FIXED
    ANGLE_TO_VELOCITY_MULTIPLIER = 0.3f;
    GRAVITY = 1.0f;
    BOUNCE_DECAY = 0.85f;
#endif
    arm.loadSprite("images/BadVista/bvArm.png");
    logo.loadSprite("images/BadVista/bvLogo.png");
    background.loadBackground("images/BadVista/bvBkgnd.png");

    counter.setMode(SECONDS);
    inputDelay.setMode(MILLI_SECONDS);
    inputDelay.start();

    horizontalObstacles.push_back(SimpleRegion(0, 0, SCREEN_MAX_X, CEILING));
    // leave some extra buffer below the floor; else the logo accidentally gets stuck down there!
    horizontalObstacles.push_back(SimpleRegion(0, FLOOR, SCREEN_MAX_X, SCREEN_MAX_Y + 50));
    horizontalObstacles.push_back(SimpleRegion(0, VISTA_TOP, VISTA_WIDTH, VISTA_BOT));
    horizontalObstacles.push_back(SimpleRegion(BASKET_LEFT_INSIDE, BASKET_BOT, BASKET_RIGHT_INSIDE, FLOOR));

    verticalObstacles.push_back(SimpleRegion(0, 0, WALL_LEFT, SCREEN_MAX_Y));
    verticalObstacles.push_back(SimpleRegion(WALL_RIGHT, 0, SCREEN_MAX_X, SCREEN_MAX_Y));
    verticalObstacles.push_back(SimpleRegion(0, 0, BAD_WIDTH, BAD_BOTTOM));
    verticalObstacles.push_back(SimpleRegion(0, VISTA_TOP, VISTA_WIDTH, VISTA_BOT));

    basketLeft.push_back(SimpleRegion(BASKET_LEFT_OUTSIDE, BASKET_TOP - logo.getHeight() / 4, BASKET_LEFT_INSIDE, FLOOR));
    basketRight.push_back(SimpleRegion(BASKET_RIGHT_OUTSIDE, BASKET_TOP - logo.getHeight() / 4, BASKET_RIGHT_INSIDE, FLOOR));

    playerObstacle.push_back(SimpleRegion(PERSON_X, PERSON_Y, SCREEN_MAX_X, SCREEN_MAX_Y));

    pauseText.loadFont("font/bip.ttf", 32);
    pauseText.setColour(BLACK);
}


void StateBadVista::init()
{
  bvState = BV_INIT;
  pwned = false;
  pwned_count = 0;
  armAngle = 1;
  armIncrement = 4;
  numBounces = 0;
  arm.setPosition((int)ARM_INIT_X, (int)ARM_INIT_Y);
  logo.setPosition((int)LOGO_INIT_X, (int)LOGO_INIT_Y);
  buttonPrompter.display(ButtonPrompter::BUTTON_A, LOGO_INIT_X - 10, LOGO_INIT_Y - 28);
  if(variables.size()<SUBSTATE_TRIGGER)
  {
          backsound.loadMusic("music/BadVista/pp.bv.windows.single.ogg");
    backsound.setLooping(true);
    bounce.loadSound("music/BadVista/ding.ogg");
    backsound.play();
  }
}

void StateBadVista::userInput()
{
  if (inputDelay.getTicks() < INPUT_DELAY)
    return;
  inputDelay.start();

  input->update();
  #ifdef PLATFORM_PC
    if(input->isQuit())
      nullifyState();
  #endif

  if(input->isA() && bvState < BV_PITCHED) {
    bvState = BV_WINDUP;
    armAngle += armIncrement;
    if (armAngle >= ARM_ANGLE_MAX || armAngle <= 1) {
      armIncrement = -armIncrement;
      ++pwned_count;
      pwned =  PWNED_STOPPED <= pwned_count && pwned_count <= PWNED_PWNED + 1;

    }

    logo.setPosition((int)ARM_INIT_X, (int)ARM_INIT_Y + armAngle);
  }

  if (bvState == BV_WINDUP && !input->isA()) {
    bvState = BV_PITCHED;

    logoX = LOGO_INIT_X;
    logoY = LOGO_INIT_Y;
    float initVelocity = (float) armAngle * ANGLE_TO_VELOCITY_MULTIPLIER;
    logoVelocityX = -initVelocity;
    logoVelocityY = initVelocity;
    armAngle = 1;
  }

  if(bvState > BV_PITCHED) {
    if(input->isA())
      setNextState(STATE_MAIN);
  }

  if(input->isStart())//I wasnt sure what exit key was...
    {
        isPaused = !isPaused;
        input->resetKeys();
    }
}

bool StateBadVista::hitObstacle(std::vector<SimpleRegion> obstacles, SimpleRegion motion){
  for(std::vector<SimpleRegion>::iterator obstacleIt=obstacles.begin(); obstacleIt!=obstacles.end(); obstacleIt++) {
    SimpleRegion region = *(obstacleIt);
    if (region.intersects(motion)) {
        return true;
    }
  }
  return false;
}

bool StateBadVista::ballStopped(){
  const int LOGO_TOP  = logo.getHeight() / 2;
  //cout << "ballStopped? y=" << logoY + LOGO_TOP << "\t" << FLOOR << "/" << BASKET_BOT << "\n";
  return
    (
        (
            (
                (inBasket() && abs(logoY + LOGO_TOP - BASKET_BOT) < 5)
                || (!inBasket() && abs(logoY + LOGO_TOP - FLOOR) < 5)
            )
        && abs(logoVelocityX) < 0.5f
        && abs(logoVelocityY) < 0.5f
        )
    || numBounces > MAX_NUM_BOUNCES);
}


bool StateBadVista::inBasket() {
    return
        BASKET_LEFT_OUTSIDE <= logoX && logoX <= BASKET_RIGHT_OUTSIDE
        && BASKET_TOP <= logoY;
}

void StateBadVista::update()
{

  const int LOGO_SIDE = logo.getWidth()  / 2;
  const int LOGO_TOP  = logo.getHeight() / 2;

  arm.setScaleX(1);
  arm.setScaleY(1);
  arm.setRotation(-armAngle);

  logo.setScaleX(1);
  logo.setScaleY(1);
  logo.setRotation(-armAngle * 0.5f);


  if(bvState == BV_WINDUP) {
    float radians = armAngle * 3.14159f / 180.0f;
    logo.setPosition(
        (int) LOGO_INIT_X + (armAngle * 0.25f) - (50 * sin(radians)),
        (int) LOGO_INIT_Y - 100 + (100 * cos(radians)));
    return;
  }


  if(bvState == BV_PITCHED) {
    // Calculate motion region
    #ifdef PENJIN_FIXED
        SimpleRegion tempRegion(logoX, logoY, fixedpoint::fix2int(logoX + logoVelocityX), fixedpoint::fix2int(logoY + logoVelocityY));
    #else
        SimpleRegion tempRegion(logoX, logoY, logoX + logoVelocityX, logoY + logoVelocityY);
    #endif
    SimpleRegion motion(
      tempRegion.x1() - LOGO_SIDE,
      tempRegion.y1() - LOGO_TOP,
      tempRegion.x2() + LOGO_SIDE,
      tempRegion.y2() + LOGO_TOP);

    if (hitObstacle(horizontalObstacles, motion)) {
      if (bounce.isPlaying()) bounce.stop();
      bounce.play();
      logoVelocityX *= BOUNCE_DECAY;
      logoVelocityY *= BOUNCE_DECAY;
      logoVelocityY *= -1;
      numBounces++;
    }

    if (hitObstacle(verticalObstacles, motion)) {
      if (bounce.isPlaying()) bounce.stop();
      bounce.play();
      logoVelocityX *= BOUNCE_DECAY;
      logoVelocityY *= BOUNCE_DECAY;
      logoVelocityX *= -1;
    }

    // Sometimes the ball gets caught inside the side of the litter can:
    // neither inside nor out. The following two paragraphs seek to prevent
    // that.
    if (hitObstacle(basketLeft, motion)) {
      bounce.play();
      logoVelocityX *= BOUNCE_DECAY;
      logoVelocityY *= BOUNCE_DECAY;
      if (logoX >= BASKET_LEFT_INSIDE && logoVelocityX < 0)
        logoVelocityX *= -1;
      if (logoX < BASKET_LEFT_INSIDE && logoVelocityX > 0)
        logoVelocityX *= -1;
    }

    if (hitObstacle(basketRight, motion)) {
      bounce.play();
      logoVelocityX *= BOUNCE_DECAY;
      logoVelocityY *= BOUNCE_DECAY;
      if (logoX >= BASKET_RIGHT_INSIDE && logoVelocityX < 0)
        logoVelocityX *= -1;
      if (logoX < BASKET_RIGHT_INSIDE && logoVelocityX > 0)
        logoVelocityX *= -1;
    }

    if (hitObstacle(playerObstacle, motion)) {
      init();
      return;
    }

    SimpleRegion logoPosition(logoX - LOGO_SIDE, logoY - LOGO_TOP, logoX + LOGO_SIDE, logoY + LOGO_TOP);

    // Check for end of game
    if (ballStopped()) {
      bool won = inBasket();
      variables[0].setValue(won ? 1 : 0);
      if (won)
        bvState = BV_WON;
      else
        bvState = BV_LOST;
      counter.start();
    }

    // Ensure gravity doesn't overcome the bounce decay
    if (inBasket()) {
        // Once the logo enters the basket, don't let it bounce out
        float newYPos = GRAVITY + logoVelocityY + logoY + LOGO_TOP;
        if (logoVelocityY < 1 && BASKET_BOT - logoY < LOGO_TOP + 5) {
            logoVelocityY = 0;
            logoVelocityX = 0;
            logoY = BASKET_BOT - LOGO_TOP;
        }
        else if (BASKET_TOP < newYPos && newYPos < BASKET_BOT)
            logoVelocityY += GRAVITY;
        else if (newYPos  > BASKET_BOT)
            logoVelocityY = BASKET_BOT - logoY - LOGO_TOP;
        else
            logoVelocityY = BASKET_TOP - logoY - LOGO_TOP;
    }
    else {
        if (GRAVITY + logoVelocityY + logoY + LOGO_TOP < FLOOR)
            logoVelocityY += GRAVITY;
        else
            logoVelocityY = FLOOR - logoY - LOGO_TOP;
    }


    #ifdef PENJIN_FIXED
        logoY += fixedpoint::fix2int(logoVelocityY);
        logoX += fixedpoint::fix2int(logoVelocityX);
    #else
        logoY += logoVelocityY;
        logoX += logoVelocityX;
    #endif

    logo.setPosition(logoX - LOGO_SIDE, logoY - LOGO_TOP);
    return;
  }


  if(bvState == BV_LOST) {
    if(pwned) {
        bounce.loadSound("music/BadVista/chimes.ogg");
        bounce.play();

        backsound.stop();
        backsound.loadMusic("music/BadVista/pp.bv.windows.e.egg.ogg");
        backsound.setLooping(false);
        backsound.play();

        if (pwned_count < PWNED_PWNED)
            loserBackground.loadSprite("images/BadVista/stopped.jpg");
        else
            loserBackground.loadSprite("images/BadVista/lost.png");

        bvState = BV_PWNED;
    }
    else
        if(counter.getScaledTicks() > END_DELAY)
            setNextState(STATE_MAIN);
  }

  if(bvState == BV_WON) {
    if(counter.getScaledTicks() > END_DELAY)
        setNextState(STATE_MAIN);
  }

  if(bvState == BV_PWNED) {
    if(counter.getScaledTicks() > LOSER_DELAY && !backsound.isPlaying())
      setNextState(STATE_MAIN);
  }
}

#ifdef PENJIN_SDL
void StateBadVista::render(SDL_Surface *screen) {
    background.render(screen);

  if (bvState == BV_PWNED)
    loserBackground.render(screen);
  else {
    arm.render(screen);
    logo.render(screen);
    buttonPrompter.render();
  }
}
void StateBadVista::pauseScreen(SDL_Surface* screen)
{
    // Pause screen
    if(variables.size()<SUBSTATE_TRIGGER)
        pauseSymbol(screen);
    pauseText.setPosition(50,180);
    pauseText.print(screen, "Put Vista where it belongs!");
    pauseText.setPosition(50,220);
    pauseText.print(screen, "Press and hold     to throw!");
    buttonPrompter.renderImage(10,260,220);
}
#else
void StateBadVista::render()
{
	background.render();

  if (bvState == BV_PWNED)
    loserBackground.render();
  else {
    arm.render();
    logo.render();
    buttonPrompter.render();
  }
}
void StateBadVista::pauseScreen()
{
    // Pause screen
    pauseSymbol();
    pauseText.setPosition(50,180);
    pauseText.print("Put Vista where it belongs!");
    pauseText.setPosition(50,220);
    pauseText.print("Press and hold     to throw!");
    buttonPrompter.renderImage(10,260,220);
}
#endif
