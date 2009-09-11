#include "StateTheCountJob.h"

StateTheCountJob::StateTheCountJob()
{
    nullify = false;
}


StateTheCountJob::~StateTheCountJob()
{

}

bool StateTheCountJob::checkPos(int curr) {
    // check if position of circle is already occupied by another one
    for (int I=0; I < curr; I++) {
        if (circle[curr].getX() >= circle[I].getX()-40 && circle[curr].getX() <= circle[I].getX()+40) {
            if (circle[curr].getY() >= circle[I].getY()-40 && circle[curr].getY() <= circle[I].getY()+40) {
                return false;
            }
		}
    }
    return true;
}

void StateTheCountJob::init()
{
    // vars
    counter.setMode(DECI_SECONDS);
    arr[0] = arr[1] = arr[2] = arr[3] = arr[4] = 0;
    colourBlind = false;
    inputMode = false;
    selected = 0;
    winTime = 0;

    // setting amount of circles
    int numCircles = 75;
    int maxLevel = min(variables[2].getInt(),30); // range: 1 - 30
    circleLevel = 2+ceil(maxLevel/10.0); // range: 3 - 5
    diffLevel = maxLevel-floor((maxLevel-1.0)/10.0)*10; // range: 1 - 10
    showTime = 50-round((20.0/9.0)*(diffLevel-1.0)); // range: 30 - 50 s/10
    numCircles -= round(75.0 / circleLevel) + ceil((11.0-diffLevel) / circleLevel); // right side ranges: 26 - 29; 20 - 22; 16 - 17
    arr[rand()%circleLevel] = 75 - numCircles;
    float I = 1;
    while (I < circleLevel) { // evaluate the reamining numers
        int num = rand()%circleLevel;
        if (arr[num] == 0) {
            arr[num] = round(numCircles / (circleLevel-I))-floor(1.0/I);
            numCircles -= arr[num];
            I++;
        }
    }

    // visuals
    selection.loadSprite("images/TheCountJob/selection.png");
    pauseText.loadFont("font/bip.ttf", 32);
    text.loadFont("font/bip.ttf", 48);
    text.setColour(Colour(BLACK));

    // sound
    sTime1.loadSound("sounds/TheCountJob/clock_slow.wav");
    sTime2.loadSound("sounds/TheCountJob/clock_fast.wav");
    sAlarm.loadSound("sounds/TheCountJob/alarm_clock.wav");
    sSelection.loadSound("sounds/TheCountJob/selection.wav");

    GFX::setClearColour(WHITE);
    // gogogo
    counter.start();
}

void StateTheCountJob::userInput()
{

    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif

    // selection/input mode keys
    if(input->isLeft() && inputMode)
    {
        selected--;
        if (selected < 0) {
            selected = circleLevel-1;
        }
        sSelection.stop();
        sSelection.play();
        input->resetKeys();
    }
    if(input->isRight() && inputMode)
    {
        selected++;
        if (selected >= circleLevel) {
            selected = 0;
        }
        sSelection.stop();
        sSelection.play();
        input->resetKeys();
    }
    if(input->isA())
    {
        // check win/lose
        if (inputMode) {
            if (arr[selected] >= arr[0] && arr[selected] >= arr[1] && arr[selected] >= arr[2] && arr[selected] >= arr[3] && arr[selected] >= arr[4]) {
                variables[0].setInt(1);
            } else {
                variables[0].setInt(0);
            }
            winTime = counter.getScaledTicks();
        // set colourblind mode
        } else if (counter.getScaledTicks() <= 10) {
            colourBlind = true;
        }
        input->resetKeys();
    }

    if(input->isStart())//I wasnt sure what exit key was...
    {
        isPaused = !isPaused;
        input->resetKeys();
    }
}

#ifdef PENJIN_SDL
void StateTheCountJob::render(SDL_Surface *screen)
{
	GFX::clearScreen(screen);
    if (counter.getScaledTicks() <= 10) { // draw info for 1 second
        text.setPosition(199,161);
        text.print(screen,"Count the circles!");
        text.setPosition(27,246);
        text.print(screen,"Press A-key for colourblind mode.");
    } else if (counter.getScaledTicks() < 10+showTime) { // render circles
        for (int I=0; I < 75; I++) {
            circle[I].render(screen);
        }
    } else {  // input/win mode
        text.setPosition(55,55);
        text.print(screen,"Which one was displayed most?");
        for (int I=0; I<circleLevel; I++) {
            circle[I].render(screen); // render selection circles
        }
        if (winTime == 0) {
            selection.render(screen); // render selector
        } else {
            if (counter.getScaledTicks() >= winTime+5) {
                for (int I=0; I<circleLevel; I++) {
                    text.setPosition(round((800.0-(circleLevel-1.0)*125.0)/2.0)+125*I-25,330); // render count text
                    text.print(screen,arr[I]);
                }
            }
            if (counter.getScaledTicks() == winTime+15) {
                setNextState(STATE_MAIN); // goto next game
            }
        }
    }
}

void StateTheCountJob::pauseScreen(SDL_Surface* screen)
{
    pauseSymbol(screen);
    pauseText.setPosition(20,180);
    pauseText.setColour(MAGENTA);
    pauseText.print(screen, "Count the circles and determine which color is displayed most.");
}
#else
void StateTheCountJob::render()
{
	GFX::clearScreen();
    if (counter.getScaledTicks() <= 10) { // draw info for 1 second
        text.setPosition(199,161);
        text.print("Count the circles!");
        text.setPosition(27,246);
        text.print("Press A-key for colourblind mode.");
    } else if (counter.getScaledTicks() < 10+showTime) { // render circles
        for (int I=0; I < 75; I++) {
            circle[I].render();
        }
    } else {  // input/win mode
        text.setPosition(55,55);
        text.print("Which one was displayed most?");
        for (int I=0; I<circleLevel; I++) {
            circle[I].render(); // render selection circles
        }
        if (winTime == 0) {
            selection.render(); // render selector
        } else {
            if (counter.getScaledTicks() >= winTime+5) {
                for (int I=0; I<circleLevel; I++) {
                    text.setPosition(round((800.0-(circleLevel-1.0)*125.0)/2.0)+125*I-25,330); // render count text
                    text.print(arr[I]);
                }
            }
            if (counter.getScaledTicks() == winTime+15) {
                setNextState(STATE_MAIN); // goto next game
            }
        }
    }
}

void StateTheCountJob::pauseScreen()
{
    pauseSymbol();
    pauseText.setPosition(20,180);
    pauseText.setColour(MAGENTA);
    pauseText.print("Count the circles and determine which color is displayed most.");
}
#endif
void StateTheCountJob::onPause()
{
    counter.pause();
}

void StateTheCountJob::onResume()
{
    counter.unpause();
}

void StateTheCountJob::update()
{
    if (counter.getScaledTicks() == 10) {
        // creating the circles
        //int pos[2];
        int count=0;
        for (int I=0; I<circleLevel; I++) {
            for (int K=0; K<arr[I]; K++) {
                if (I == 0) {
                    if (colourBlind) {
                        circle[count].loadSprite("images/TheCountJob/circle_bw1.png");
                    } else {
                        circle[count].loadSprite("images/TheCountJob/circle_y.png");
                    }
                } else if (I == 1) {
                    if (colourBlind) {
                        circle[count].loadSprite("images/TheCountJob/circle_bw3.png");
                    } else {
                        circle[count].loadSprite("images/TheCountJob/circle_r.png");
                    }
                } else if (I == 2) {
                    if (colourBlind) {
                        circle[count].loadSprite("images/TheCountJob/circle_bw5.png");
                    } else {
                        circle[count].loadSprite("images/TheCountJob/circle_b.png");
                    }
                } else if (I == 3) {
                    if (colourBlind) {
                        circle[count].loadSprite("images/TheCountJob/circle_bw2.png");
                    } else {
                        circle[count].loadSprite("images/TheCountJob/circle_o.png");
                    }
                } else if (I == 4) {
                    if (colourBlind) {
                        circle[count].loadSprite("images/TheCountJob/circle_bw4.png");
                    } else {
                        circle[count].loadSprite("images/TheCountJob/circle_g.png");
                    }
                }
                circle[count].setPosition(rand()%750,rand()%430);
                while (!checkPos(count)) {
                    circle[count].setPosition(rand()%750,rand()%430);
                }
                count++;
            }
        }
        // sound - phase 1
        sTime1.play();
    } else if (counter.getScaledTicks() == 10+round(showTime*3.0/5.0)) {
        // sound - phase 2 (phase 3 was cutted because of budget limits)
        sTime1.stop();
        sTime2.play();
    }
    if (counter.getScaledTicks() == 10+showTime) {
        sTime2.stop();
        sAlarm.play();
        // start input mode
        inputMode = true;
        // display 3 - 5 circles of different colour
        for (int I=0;I<circleLevel;I++) {
            circle[I].clear();
            if (I == 0) {
                if (colourBlind) {
                    circle[I].loadSprite("images/TheCountJob/circle_bw1.png");
                } else {
                    circle[I].loadSprite("images/TheCountJob/circle_y.png");
                }
            } else if (I == 1) {
                if (colourBlind) {
                    circle[I].loadSprite("images/TheCountJob/circle_bw3.png");
                } else {
                    circle[I].loadSprite("images/TheCountJob/circle_r.png");
                }
            } else if (I == 2) {
                if (colourBlind) {
                    circle[I].loadSprite("images/TheCountJob/circle_bw5.png");
                } else {
                    circle[I].loadSprite("images/TheCountJob/circle_b.png");
                }
            } else if (I == 3) {
                if (colourBlind) {
                    circle[I].loadSprite("images/TheCountJob/circle_bw2.png");
                } else {
                    circle[I].loadSprite("images/TheCountJob/circle_o.png");
                }
            } else if (I == 4) {
                if (colourBlind) {
                    circle[I].loadSprite("images/TheCountJob/circle_bw4.png");
                } else {
                    circle[I].loadSprite("images/TheCountJob/circle_g.png");
                }
            }
            circle[I].setPosition(round((800.0-(circleLevel-1.0)*125.0)/2.0)+125*I-25,275);
            selection.setPosition(round((800.0-(circleLevel-1.0)*125.0)/2.0)-25,210);
        }
    }
    // display selector
    if (counter.getScaledTicks() >= 10+showTime) {
        selection.setX(round((800.0-(circleLevel-1.0)*125.0)/2.0)+125*selected-8-25);
    }
}
