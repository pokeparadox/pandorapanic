/*
    PanicAttack minigame for Pandora Panic
    Release: 18 Feb 2009
    Authors: Todd Foster, code
             TheBrainSquid, buttons graphics
             Gruso, soundtrack
    License: GPLv3
*/

#include "StatePanicAttack.h"


int StatePanicAttack::level = 1;
int StatePanicAttack::gameInstance = -1;


StatePanicAttack::StatePanicAttack()
{
    instructionsBackground.loadBackground("images/PanicAttack/paInstructions.png");
    playBackground.loadBackground("images/PanicAttack/paHellollo.png");
    dontPanic.loadImageSheet("images/PanicAttack/dontpanic.png", 10, 1);
    panicsIndex.setMin(0);
    panicsIndex.setMax(23);

    events = new StatePanicAttackButtonEvents();

    panicsTimer.setMode(MILLI_SECONDS);
    timer.setMode(MILLI_SECONDS);
    gameLength = 10;
    pauseText.loadFont("font/bip.ttf", 32);
    text.loadFont("font/atrox.ttf", 60);
    text.setColour(Colour(RED));

    debounce = new Debounce(PA_NUM_BUTTONS ,100);
}

StatePanicAttack::~StatePanicAttack() {
    delete events;
    delete debounce;
}


void StatePanicAttack::init()
{
    if(variables.size()<SUBSTATE_TRIGGER)
    {
        backMusic.loadMusic("music/PanicAttack/panicattack16s.ogg");
        backMusic.setLooping(true);
        buttonSound.loadSound("music/PanicAttack/heartbeat.ogg");
        buzzer.loadSound("music/PanicAttack/buzzer2.ogg");

    }
    if (variables[7] != StatePanicAttack::gameInstance) {
      StatePanicAttack::level = 1;
      StatePanicAttack::gameInstance = variables[7].getInt();
    }

    //StatePanicAttack::level=7; // for testing

    // Parse out level
    buttonsAvailable = PA_BUTTON_SHOULDERLEFT;
    buttonsConcurrent = 0;
    buttonsMinSpacing = 1000;
    nextButtonAdded = 1000;
    events->setResponseTime(1500);
    if (StatePanicAttack::level > 1) {
        buttonsAvailable = PA_BUTTON_Y;
        buttonsConcurrent++;
    }
    if (StatePanicAttack::level > 2) {
        buttonsAvailable = PA_BUTTON_DOWN;
        buttonsMinSpacing = 750;
    }
    if (StatePanicAttack::level > 3) {
        events->setResponseTime(1250);
    }
    if (StatePanicAttack::level > 4) {
        buttonsConcurrent++;
        buttonsMinSpacing = 500;
    }
    if (StatePanicAttack::level > 5)
        events->setResponseTime(1000);

    if (StatePanicAttack::level > 6) {
        buttonsConcurrent = StatePanicAttack::level - 4;
        buttonsMinSpacing = 250;
    }

    if(StatePanicAttack::level % 2 == 1)
        panics.loadImageSheetNoKey("images/PanicAttack/panicsa.png", 24, 1);
    else
        panics.loadImageSheetNoKey("images/PanicAttack/panicsb_new.png", 24, 1);

    events->setPreview(buttonsAvailable);
    randomTime.setMin(buttonsMinSpacing);
    randomTime.setMax(buttonsMinSpacing * 2);
    randomButton.setMin(0);
    randomButton.setMax(buttonsAvailable);
    panicAttackState = PANICATTACK_INIT;
    timer.start();
    beats = 0;
}


void StatePanicAttack::userInput()
{
  // Framework stuff ---------------------------------
  input->update();
  #ifdef PLATFORM_PC
    if(input->isQuit())
      nullifyState();
  #endif

  if(input->isStart())//I wasnt sure what exit key was...
    {
        isPaused = !isPaused;
        input->resetKeys();
    }
  // End Framework stuff -----------------------------

  if(panicAttackState == PANICATTACK_INIT) {
      if(input->isLeft()  || // D-Pad buttons
         input->isRight() ||
         input->isUp()    ||
         input->isDown()  ||
         input->isA()     || // Other buttons
         input->isB()     ||
         input->isX()     ||
         input->isY()     ||
         input->isL()     || // Left shoulder button
         input->isR()        // Right shoulder button
         ) {
            finishedInit();
            debounce->block();
         }
    return;
  }

  if(panicAttackState == PANICATTACK_PLAYING) {
      if(!backMusic.isPlaying())
          backMusic.play();

      // For each key we're going to use in our game, we write a clause:
      if(
        (input->isA() && debounce->ready(PA_BUTTON_A) && events->rm(PA_BUTTON_A)) ||
        (input->isB() && debounce->ready(PA_BUTTON_B) && events->rm(PA_BUTTON_B)) ||
        (input->isX() && debounce->ready(PA_BUTTON_X) && events->rm(PA_BUTTON_X)) ||
        (input->isY() && debounce->ready(PA_BUTTON_Y) && events->rm(PA_BUTTON_Y)) ||
        (input->isUp() && debounce->ready(PA_BUTTON_UP) && events->rm(PA_BUTTON_UP)) ||
        (input->isDown() && debounce->ready(PA_BUTTON_DOWN) && events->rm(PA_BUTTON_DOWN)) ||
        (input->isRight() && debounce->ready(PA_BUTTON_RIGHT) && events->rm(PA_BUTTON_RIGHT)) ||
        (input->isLeft() && debounce->ready(PA_BUTTON_LEFT) && events->rm(PA_BUTTON_LEFT)) ||
        ((input->isL() || input->isR()) && debounce->ready(PA_BUTTON_SHOULDERLEFT) && events->rm(PA_BUTTON_SHOULDERLEFT))
        )
            buttonSound.play();

      if (events->lost()) {
          timer.start();
          buzzer.play();
          panicAttackState = PANICATTACK_LOST;
      }
  }
}

void StatePanicAttack::finishedInit() {
    panicAttackState++;
    events->setPreview(-1);
    timer.start();
}

void StatePanicAttack::onPause()
{
    events->pause();
    timer.pause();
}

void StatePanicAttack::onResume()
{
    events->unpause();
    timer.unpause();
}

void StatePanicAttack::update()
{
    int currentTime = timer.getScaledTicks();


    if (panicAttackState == PANICATTACK_INIT) {
        if (currentTime > INIT_DELAY)
            finishedInit();
        return;
    }

    if (panicAttackState == PANICATTACK_PLAYING) {
        if (beats >= gameLength && events->buttonCount(true) == 0) {
            panicAttackState = PANICATTACK_WON;
            timer.start();
            return;
        }

        // Add heartbeat
        if (beats < gameLength && currentTime > (beats * StatePanicAttack::HEARTBEAT_SPACING)) {
            events->expect(PA_BUTTON_SHOULDERLEFT);
            beats++;
        }

        // Add other buttons
        if (beats < gameLength
            && events->buttonCount(false) < buttonsConcurrent
            && nextButtonAdded < currentTime) {
                nextButtonAdded = currentTime + randomTime.nextInt();
                events->expect((PA_BUTTON) randomButton.nextInt());
            }

        return;
    }

    if (panicAttackState == PANICATTACK_WON) {
        if (currentTime > LOSER_DELAY) {
            ++StatePanicAttack::level;
            variables[0].setInt(1);
            setNextState(STATE_MAIN);
            return;
        }
    }

    if (panicAttackState == PANICATTACK_LOST) {
        if (currentTime > LOSER_DELAY) {
            variables[0].setInt(0);
            setNextState(STATE_MAIN);
        }
        return;
    }
}

#ifdef PENJIN_SDL
void StatePanicAttack::pauseScreen(SDL_Surface* screen)
{
    pauseSymbol(screen);
    pauseText.setPosition(50,180);
    pauseText.setColour(RED);
    pauseText.print(screen, "Press the prompted buttons within the timelimit");
}

void StatePanicAttack::render(SDL_Surface *screen) {
    if (panicAttackState == PANICATTACK_INIT) {
        instructionsBackground.render(screen);
        text.setPosition(10,10);
        ostringstream buffer1;
        buffer1 << "Level: " << StatePanicAttack::level;
        text.print(screen, buffer1.str());
    }
    else {
        playBackground.render(screen);
        renderPanics(screen);
    }

    events->render(screen);
    renderProgress(screen);
}


void StatePanicAttack::renderProgress (SDL_Surface *screen) {
    if (beats < 10)
        dontPanic.renderImage(beats, screen, 0, 0);
    else
        if ((int)(timer.getScaledTicks() * 0.01f) % 2)
            dontPanic.renderImage(9, screen, 0, 0);
    /*
    int progressBlockSeparation = 760 / gameLength;
    int progressBlockWidth = progressBlockSeparation / 3 * 2;
    for (int i=0; i<beats; i++)
      putRectangle(
        screen,
        20 +  (i * progressBlockSeparation),
        480 - progressBlockWidth - 5,
        progressBlockWidth,
        progressBlockWidth,
        0x0000ff
        );
        */
}

void StatePanicAttack::renderPanics(SDL_Surface *screen) {
    if (!panicsTimer.isStarted() || panicsTimer.getScaledTicks() > PANICS_DELAY) {
        panicsCurrent = panicsIndex.nextInt();
        panicsTimer.start();
    }
    panics.renderImage(panicsCurrent, screen, 302, 28);
}
#else
    void StatePanicAttack::pauseScreen()
    {
        pauseSymbol();
        pauseText.setPosition(50,180);
        pauseText.setColour(RED);
        pauseText.print("Press the prompted buttons within the timelimit");
    }
    void StatePanicAttack::render()
    {
        if (panicAttackState == PANICATTACK_INIT) {
            instructionsBackground.render();
            text.setPosition(10,10);
            ostringstream buffer1;
            buffer1 << "Level: " << StatePanicAttack::level;
            text.print(buffer1.str());
        }
        else {
            playBackground.render();
            renderPanics();
        }

        events->render();
        renderProgress();
    }
    void StatePanicAttack::renderProgress()
    {
        if (beats < 10)
            dontPanic.renderImage(beats, 0, 0);
        else
            if ((int)(timer.getScaledTicks() * 0.01f) % 2)
                dontPanic.renderImage(9, 0, 0);
    }
    void StatePanicAttack::renderPanics()
    {
 if (!panicsTimer.isStarted() || panicsTimer.getScaledTicks() > PANICS_DELAY) {
            panicsCurrent = panicsIndex.nextInt();
            panicsTimer.start();
        }
        panics.renderImage(panicsCurrent, 302, 28);
    }
#endif
