#include "StateTheCountJob.h"

StateTheCountJob::StateTheCountJob()
{
    //
}


StateTheCountJob::~StateTheCountJob()
{
    //
}

bool StateTheCountJob::checkPos(vector<Vector2df>::iterator curr, int arr)
{
    // check if position of circle is already occupied by another one
    for (int I=0; I < circleLevel; I++)
    {
        vector<Vector2df>::iterator end;
        if (I == arr)
            end = curr;
        else
            end = pos[I].end();
        vector<Vector2df>::iterator iter;
        for (iter=pos[I].begin(); iter < end; iter++)
        {
            if (curr->x >= iter->x-40 && curr->x <= iter->x+40)
            {
                if (curr->y >= iter->y-40 && curr->y <= iter->y+40)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

void StateTheCountJob::init()
{
    text.loadFont("font/bip.ttf", 48);
    text.setColour(Colour(BLACK));
    pauseText.loadFont("font/bip.ttf", 32);
    pauseText.setColour(MAGENTA);
    pauseMayhem = rand()%100;

    GFX::setClearColour(WHITE);

    // The following gets only triggert in a real game
    if(variables.size()<SUBSTATE_TRIGGER) {

    /// vars
    counter.setMode(DECI_SECONDS);
    colourBlind = false;
    inputMode = false;
    selected = 0;
    winTime = 0;

    // setting amount of circles
    int numCircles = 75;
    // limits level number to 30
    int maxLevel = min(variables[2].getInt(),30); // range: 1 - 30
    // number of different colours
    circleLevel = 2+ceil(maxLevel/10.0); // range: 3 - 5
    // speed difficulty level
    diffLevel = maxLevel-floor((maxLevel-1.0)/10.0)*10; // range: 1 - 10
    // amount of time in which circles are shown (depending on difficulty level)
    showTime = 50-round((20.0/9.0)*(diffLevel-1.0)); // range: 30 - 50 s/10
    // calculates number of circles for biggest group
    numCircles -= round(75.0 / circleLevel) + ceil((11.0-diffLevel) / circleLevel); // right side ranges: 26 - 29; 20 - 22; 16 - 17
    pos[rand()%circleLevel].resize(75 - numCircles);
    // calculates number of circles for rest of the groups
    float I = 1;
    while (I < circleLevel)
    {
        int num = rand()%circleLevel;
        if (pos[num].size() == 0)
        {
            pos[num].resize(round(numCircles / (circleLevel-I))-floor(1.0/I));
            numCircles -= pos[num].size();
            I++;
        }
    }
    // set circle positions
    for (int I=0; I<circleLevel; I++)
    {
        vector<Vector2df>::iterator iter;
        for (iter = pos[I].begin(); iter < pos[I].end(); iter++)
        {
            *iter = Vector2df(rand()%750,rand()%430);
            // check if position is already occupied
            // max 100 checks to prevent infinite loop
            int max = 100;
            while (!checkPos(iter,I) && max > 0)
            {
                *iter = Vector2df(rand()%750,rand()%430);
                max--;
            }
        }
    }

    /// visuals
    selection.loadSprite("images/TheCountJob/selection.png");
    circle[0].loadSprite("images/TheCountJob/circle_y.png");
    circle[1].loadSprite("images/TheCountJob/circle_r.png");
    circle[2].loadSprite("images/TheCountJob/circle_b.png");
    circle[3].loadSprite("images/TheCountJob/circle_o.png");
    circle[4].loadSprite("images/TheCountJob/circle_g.png");

    /// sound
    sTime1.loadSound("sounds/TheCountJob/clock_slow.ogg");
    sTime2.loadSound("sounds/TheCountJob/clock_fast.ogg");
    sAlarm.loadSound("sounds/TheCountJob/alarm_clock.ogg");
    sSelection.loadSound("sounds/TheCountJob/selection.ogg");

    // gogogo
    counter.start();

    } // end sel box check
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
        if (selected < 0)
        {
            selected = circleLevel-1;
        }
        sSelection.stop();
        sSelection.play();
        input->resetKeys();
    }
    if(input->isRight() && inputMode)
    {
        selected++;
        if (selected >= circleLevel)
        {
            selected = 0;
        }
        sSelection.stop();
        sSelection.play();
        input->resetKeys();
    }
    if(input->isA())
    {
        // check win/lose
        if (inputMode)
        {
            int arr[5] = {pos[0].size(), pos[1].size(), pos[2].size(), pos[3].size(), pos[4].size()};
            if (NumberUtility::isGreatest(arr,selected))
                variables[0].setInt(1);
            else
                variables[0].setInt(0);

            winTime = counter.getScaledTicks();
        }
        // set colourblind mode
        else if (counter.getScaledTicks() <= INSTRUCTIONS_TIME && !colourBlind)
        {
            colourBlind = true;
            circle[0].clear();
            circle[0].loadSprite("images/TheCountJob/circle_bw1.png");
            circle[1].clear();
            circle[1].loadSprite("images/TheCountJob/circle_bw3.png");
            circle[2].clear();
            circle[2].loadSprite("images/TheCountJob/circle_bw5.png");
            circle[3].clear();
            circle[3].loadSprite("images/TheCountJob/circle_bw2.png");
            circle[4].clear();
            circle[4].loadSprite("images/TheCountJob/circle_bw4.png");
        }
        input->resetKeys();
    }
    // pause
    if(input->isStart() && counter.getScaledTicks() <= INSTRUCTIONS_TIME)
    {
        isPaused = !isPaused;
        input->resetKeys();
    }
}

#ifdef PENJIN_SDL
void StateTheCountJob::render(SDL_Surface *screen)
{
	GFX::clearScreen(screen);
    if (counter.getScaledTicks() <= INSTRUCTIONS_TIME) // draw info
    {
        text.setPosition(199,161);
        text.print(screen,"Count the circles!");
        text.setPosition(27,246);
        text.print(screen,"Press A-key for colourblind mode.");
    }
    else if (counter.getScaledTicks() < INSTRUCTIONS_TIME+showTime) // render circles
    {
        for (int I=0; I < circleLevel; I++)
        {
            vector<Vector2df>::iterator iter;
            for (iter = pos[I].begin(); iter < pos[I].end(); iter++)
            {
                circle[I].setPosition(*iter);
                circle[I].render(screen);
            }
        }
    }
    else // input/win mode
    {
        text.setPosition(15,100);
        text.print(screen,"Which one was displayed the most?");
        for (int I=0; I<circleLevel; I++)
        {
            circle[I].render(screen); // render selection circles
        }
        if (winTime == 0)
        {
            selection.render(screen); // render selector
        }
        else
        {
            if (counter.getScaledTicks() >= winTime+5) // render count text for one second
            {
                for (int I=0; I<circleLevel; I++)
                {
                    text.setPosition(round((800.0 - (circleLevel - 1.0) * 125.0) / 2.0) + 125 * I - 25, 330); // set position (don't ask me about this formula!)
                    text.print(screen,int(pos[I].size()));
                }
            }
            if (counter.getScaledTicks() == winTime+15) // goto next game
            {
                setNextState(STATE_MAIN);
            }
        }
    }
}

void StateTheCountJob::pauseScreen(SDL_Surface* screen)
{

    pauseText.setPosition(50,180);
    if (pauseMayhem <= 95)
    {
        pauseText.print(screen, "Read this!");
    }
    else if (pauseMayhem == 96)
    {
        pauseText.print(screen, "There once was an instruction text here.");
        pauseText.setPosition(50,220);
        pauseText.print(screen, "It's gone now.");
    }
    else if (pauseMayhem == 97)
    {
        pauseText.print(screen, "Everyone reading this text is a dumb prick!");
    }
    else if (pauseMayhem == 98)
    {
        pauseText.print(screen, "There is nothing to look at here.");
        pauseText.setPosition(50,220);
        pauseText.print(screen, "Would you kindly move on to the game.");
    }
    else if (pauseMayhem == 99)
    {
        pauseText.loadFont("font/bip.ttf", 24);
        pauseText.setPosition(200,40);
        pauseText.print(screen, "Wi nøt trei a høliday in Sweden this yer?");
        pauseText.setPosition(200,70);
        pauseText.print(screen, "See the løveli lakes");
        pauseText.setPosition(200,100);
        pauseText.print(screen, "The wonderful telephøne system");
        pauseText.setPosition(200,130);
        pauseText.print(screen, "And mani interesting furry animals");
        pauseText.setPosition(50,160);
        pauseText.print(screen, "Including the majestic møøse");
        pauseText.setPosition(50,190);
        pauseText.print(screen, "A Møøse once bit my sister ...");
        pauseText.setPosition(50,220);
        pauseText.print(screen, "No realli!  She was Karving her initials on the møøse");
        pauseText.setPosition(50,250);
        pauseText.print(screen, "with the sharpened end of an interspace tøøthbrush given");
        pauseText.setPosition(50,280);
        pauseText.print(screen, "her by Svenge - her brother-in-law - an Oslo dentist and");
        pauseText.setPosition(50,310);
        pauseText.print(screen, "star of many Norwegian møvies:  \"The Høt Hands of an Oslo");
        pauseText.setPosition(50,340);
        pauseText.print(screen, "Dentist\", \"Fillings of Passion\", \"The Huge Mølars of Horst");
        pauseText.setPosition(50,370);
        pauseText.print(screen, "Nordfink\".");
        pauseText.setPosition(80,400);
        pauseText.print(screen, "We apologise for the fault in the instructions.");
        pauseText.setPosition(80,430);
        pauseText.print(screen, "Those responsible have been sacked.");
    }
}
#else
void StateTheCountJob::render()
{
	GFX::clearScreen();
    if (counter.getScaledTicks() <= 10) // draw info for 1 second
    {
        text.setPosition(199,161);
        text.print("Count the circles!");
        text.setPosition(27,246);
        text.print("Press A-key for colourblind mode.");
    }
    else if (counter.getScaledTicks() < 10+showTime) // render circles
    {
        for (int I=0; I < circleLevel; I++)
        {
            vector<Vector2df>::iterator iter;
            for (iter = pos[I].begin(); iter < pos[I].end(); iter++)
            {
                circle[I].setPosition(*iter);
                circle[I].render(screen);
            }
        }
    }
    else // input/win mode
    {
        text.setPosition(15,100);
        text.print("Which one was displayed the most?");
        for (int I=0; I<circleLevel; I++)
        {
            circle[I].render(); // render selection circles
        }
        if (winTime == 0)
        {
            selection.render(); // render selector
        }
        else
        {
            if (counter.getScaledTicks() >= winTime+5)
            {
                for (int I=0; I<circleLevel; I++) // render count text for one second
                {
                    text.setPosition(round((800.0 - (circleLevel - 1.0) * 125.0) / 2.0) + 125 * I - 25, 330);// set position (don't ask me about this formula!)
                    text.print(int(pos[I].size()));
                }
            }
            if (counter.getScaledTicks() == winTime+15)
            {
                setNextState(STATE_MAIN); // goto next game
            }
        }
    }
}

void StateTheCountJob::pauseScreen()
{
    pauseSymbol();
    pauseText.setPosition(50,180);
    pauseText.setColour(MAGENTA);
    if (pauseMayhem <= 95)
    {
        pauseText.print("Read this!");
    }
    else if (pauseMayhem == 96)
    {
        pauseText.print("There once was an instruction text here.");
        pauseText.setPosition(50,220);
        pauseText.print("It's gone now.");
    }
    else if (pauseMayhem == 97)
    {
        pauseText.print("Everyone reading this text is a dumb prick!");
    }
    else if (pauseMayhem == 98)
    {
        pauseText.print("There is nothing to look at here.");
        pauseText.setPosition(50,220);
        pauseText.print("Would you kindly move on to the game.");
    }
    else if (pauseMayhem == 99)
    {
        pauseText.loadFont("font/bip.ttf", 24);
        pauseText.setPosition(200,40);
        pauseText.print("Wi nøt trei a høliday in Sweden this yer?");
        pauseText.setPosition(200,70);
        pauseText.print("See the løveli lakes");
        pauseText.setPosition(200,100);
        pauseText.print("The wonderful telephøne system");
        pauseText.setPosition(200,130);
        pauseText.print("And mani interesting furry animals");
        pauseText.setPosition(50,160);
        pauseText.print("Including the majestic møøse");
        pauseText.setPosition(50,190);
        pauseText.print("A Møøse once bit my sister ...");
        pauseText.setPosition(50,220);
        pauseText.print("No realli!  She was Karving her initials on the møøse");
        pauseText.setPosition(50,250);
        pauseText.print("with the sharpened end of an interspace tøøthbrush given");
        pauseText.setPosition(50,280);
        pauseText.print("her by Svenge - her brother-in-law - an Oslo dentist and");
        pauseText.setPosition(50,310);
        pauseText.print("star of many Norwegian møvies:  \"The Høt Hands of an Oslo");
        pauseText.setPosition(50,340);
        pauseText.print("Dentist\", \"Fillings of Passion\", \"The Huge Mølars of Horst");
        pauseText.setPosition(50,370);
        pauseText.print("Nordfink\".");
        pauseText.setPosition(80,400);
        pauseText.print("We apologise for the fault in the instructions.");
        pauseText.setPosition(80,430);
        pauseText.print("Those responsible have been sacked.");
    }
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
    if (counter.getScaledTicks() == 10) // clock ticking
    {
        // sound - phase 1
        sTime1.play();
    }
    else if (counter.getScaledTicks() == 10 + round(showTime * 3.0 / 5.0)) // faster
    {
        // sound - phase 2 (phase 3 was cutted because of budget limits)
        sTime1.stop();
        sTime2.play();
    }
    if (counter.getScaledTicks() == 10+showTime) // ring!!
    {
        sTime2.stop();
        sAlarm.play();
        // start input mode
        inputMode = true;
        // display 3 - 5 circles of different colour
        for (int I=0;I<circleLevel;I++)
        {
            circle[I].setPosition(round((800.0 - (circleLevel - 1.0) * 125.0) / 2.0) + 125 * I - 25, 275);
            //selection.setPosition(round((800.0-(circleLevel-1.0)*125.0)/2.0)-25,210);
            selection.setPosition(0,210);
        }
    }
    // display selector
    if (counter.getScaledTicks() >= 10+showTime)
        selection.setX(round((800.0 - (circleLevel - 1.0) * 125.0) / 2.0) + 125 * selected - 8 - 25); // it works... somehow
}
