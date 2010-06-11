#include "StateTestSight.h"

StateTestSight::StateTestSight()
{
    nullify = false;
}


StateTestSight::~StateTestSight()
{

}

void StateTestSight::init()
{
    // vars
    for (int I = CUPS_AMOUNT-1; I >= 0; --I)
    {
        cups[I][ciCurrentPos] = I;
        cups[I][ciDestination] = I;
        cups[I][ciDifference] = 0;
        cups[I][ciShowCounter] = 0;
    }
    diffLevel = variables[2].getInt(); // affects speed of shuffling and number of swaps
    phase = gpInit; // phase of the game
    correctCup = rand()%5; // the correct cup - duh
    numShuffles = 0; // nummber of swaps already done
    selection = 2; // the current selected cup (in answer phase)

    // graphic
    GFX::setClearColour(WHITE);
    img_cup.loadSprite("images/TestYourSight/cup.png");
    hand.loadSprite("images/TestYourSight/hand.png");
    hand.setPosition(LEFT_CUP_X-25,480-232);
    coin.loadSprite("images/TestYourSight/coin.png");
    coin.setPosition(LEFT_CUP_X+11 + correctCup * CUP_OFFSET,CUPS_Y + 80);
    bg.loadBackground("images/TestYourSight/bg.png");
    pauseText.loadFont("font/bip.ttf", 32);
    pauseText.setColour(BLACK);
    buttonSheet.loadFrames("images/ButtonPrompter/ButtonsSheet.png",10,2);
}

void StateTestSight::userInput()
{

    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif

    if (phase == gpAnswer)
    {
        // move arm in answer phase
        if(input->isLeft())
        {
            if (selection > 0)
                selection--;
        }
        if(input->isRight())
        {
            if (selection < 4)
                selection ++;
        }

        // select a cup
        if(input->isA())
        {
            phase = gpConclusion;
            showCup(selection);
        }
    }

    if(input->isStart())
    {
        pauseToggle();
    }
    input->resetKeys();
}

#ifdef PENJIN_SDL
void StateTestSight::render(SDL_Surface *screen)
{
    GFX::clearScreen(screen);
    bg.render(screen);

    // show coin only when cups are not being shuffled
    if (not (phase == gpShuffle))
    {
        coin.setX(LEFT_CUP_X+11 + cups[correctCup][ciCurrentPos] * CUP_OFFSET);
        coin.render(screen);
    }

    // z-order sort (using the y-offset)
    int indexArray[CUPS_AMOUNT] = {0}; // array holding the ordered indexes (referring to yPos)
    float yPos[CUPS_AMOUNT] = {0.0}; // array for the y-offsets (used for the z-ordering)
    for (int I = CUPS_AMOUNT-1; I >= 0; --I)
    {
        yPos[I] = getYOffset(I);
    }
    NumberUtility::sort(yPos,indexArray,false); // sort descending as smaller yOffset means the cup is placed in the back and has to be rendered first

    // draw cups in sorted order
    for (int I = CUPS_AMOUNT-1; I >= 0; --I)
    {
        img_cup.setPosition(LEFT_CUP_X + CUP_OFFSET * cups[indexArray[I]][ciCurrentPos], CUPS_Y + yPos[indexArray[I]] - SHOW_HEIGHT * ( min(cups[indexArray[I]][ciShowCounter],float(SHOW_FRAMES_MOVE)) - max(float(0),cups[indexArray[I]][ciShowCounter] - SHOW_FRAMES_MOVE - SHOW_FRAMES_HOLD)) / SHOW_FRAMES_MOVE);
        img_cup.render(screen);
    }

    // draw hand in answer phase
    if (phase == gpAnswer)
    {
        hand.setX(LEFT_CUP_X-25 + selection * CUP_OFFSET);
        hand.render(screen);
    }
}

void StateTestSight::pauseScreen(SDL_Surface* screen)
{
    if(!isSubState())
        pauseSymbol(screen);
    buttonsOverlay(screen);

    pauseText.setPosition(50,180);
    pauseText.print(screen, "Test your sight!");
    pauseText.setPosition(50,220);
    pauseText.print(screen, "Follow the path of the cup hiding the coin!");
    pauseText.setPosition(50,260);
    pauseText.print(screen, "Press    to select your destiny!");
    buttonSheet.setCurrentFrame(10);
    buttonSheet.setPosition(134,260);
    buttonSheet.render(screen);
}
#else
    void StateTestSight::render()
    {

    }
    void StateTestSight::pauseScreen()
    {
        pauseSymbol();
    }
#endif

void StateTestSight::onPause()
{

}

void StateTestSight::onResume()
{

}

void StateTestSight::update()
{
    moveCups();

    // show initial correct cup
    if (phase == gpInit)
    {
        showCup(correctCup);
        phase = gpShow;
    }
    else if ((phase == gpShow) && not isMoving())
        phase = gpShuffle;
    // shuffle
    else if ((phase == gpShuffle) && not isMoving())
    {
        // amount check
        if (numShuffles == max(diffLevel,5)) // 5 - 20 shuffles
            phase = gpAnswer;
        else
        {
            // get two randomly chosen, different cups
            int cup1 = rand()%8;
            // check whether out of bounds -> set to cup with coin under it (higher probability to get swapped)
            if (cup1 >= 5)
                cup1 = correctCup;
            int cup2 = rand()%5;
            while (cup1 == cup2)
                cup2 = rand()%5;

            // do the shuffle
            swapCups(cup1,cup2);

            numShuffles++;
        }
    }
    // answer pahse
    else if ((phase == gpAnswer) && not isMoving())
    {
        // wait for user input
    }
    // conclusion phase
    else if ((phase == gpConclusion) && not isMoving())
    {
        // answer correct - goto next state as cup has already been shown after user input
        if (selection == cups[correctCup][ciCurrentPos])
        {
            variables[0].setInt(1);
            phase = gpExit;
        }
        // answer incorrect - show correct cup, then go to next state
        else
        {
            showCup(cups[correctCup][ciCurrentPos]);
            variables[0].setInt(0);
            phase = gpExit;
        }
    }
    // Exit game - goto next state
    else if ((phase == gpExit) && not isMoving())
        setNextState(STATE_MAIN);
}

/// Private implementation

// Swaps two cup's positions and sets velocity
void StateTestSight::swapCups(CRuint index1, CRuint index2)
{
    if (index1 < CUPS_AMOUNT && index2 < CUPS_AMOUNT && index1 != index2)
    {
        cups[index1][ciDestination] = round(cups[index2][ciCurrentPos]);
        cups[index2][ciDestination] = round(cups[index1][ciCurrentPos]);
        cups[index1][ciDifference] = round(cups[index2][ciCurrentPos] - cups[index1][ciCurrentPos]);
        cups[index2][ciDifference] = round(cups[index1][ciCurrentPos] - cups[index2][ciCurrentPos]);
    }
}

// Show what's under a specific cup
void StateTestSight::showCup(CRuint index)
{
    for (int I = CUPS_AMOUNT-1; I >= 0; --I)
        if (cups[I][ciCurrentPos] == index)
            cups[I][ciShowCounter] = 1;
}

// Move a cup over to its new position using velocity in percent
void StateTestSight::moveCups()
{
    for (int I = CUPS_AMOUNT-1; I >= 0; --I)
    {
        // Check whether velocity is greater than difference left and use that in case
        cups[I][ciCurrentPos] += min((float)(4.0f * sqrt(min(diffLevel,20))) / (float)CUP_OFFSET, // Normal movement between two spots
                                     abs(cups[I][ciCurrentPos]-cups[I][ciDestination])) * // Difference left between two spots
                                     NumberUtility::sign(cups[I][ciDifference]); // Direction of movement
        if (hasFinished(I))
        {
            cups[I][ciDifference] = 0;
        }
        if (cups[I][ciShowCounter] > 0)
        {
            if (cups[I][ciShowCounter] < SHOW_FRAMES_MOVE * 2 + SHOW_FRAMES_HOLD)
                cups[I][ciShowCounter]++;
            else
                cups[I][ciShowCounter] = 0;
        }
    }
}

// Calculates the y offset of a single cup so that cups moved left are moved in front of the other cups and rightwards moving cups in the back
float StateTestSight::getYOffset(CRuint index)
{
    const float x = abs(cups[index][ciCurrentPos]-cups[index][ciDestination]);
    const float vel = abs(cups[index][ciDifference]);
    if (vel != 0.0)
    {
        const float y = pow( 2.0/vel * (x - (vel/2.0) ), 2.0) - 1.0;
        return NumberUtility::sign(cups[index][ciDifference]) * y * MAX_Y_OFFSET;
    }
    else
        return 0.0;
}

// Checks whether a cup has reached its destination
bool StateTestSight::hasFinished(CRuint index) const
{
    if (index < CUPS_AMOUNT)
    {
        if (cups[index][ciDifference] >= 0)
            return (cups[index][ciCurrentPos] >= cups[index][ciDestination]);
        else
            return (cups[index][ciCurrentPos] <= cups[index][ciDestination]);
    }
    else
    {
        return false;
    }
}

// Returns whether one specific cup is moving
bool StateTestSight::isMoving(CRuint index) const
{
    if (index < CUPS_AMOUNT)
    {
        return ((cups[index][ciDifference] != 0) || (cups[index][ciShowCounter] != 0));
    }
    else
    {
        return false;
    }
}

// Returns whether at least one cup is moving
bool StateTestSight::isMoving() const
{
    for (int I = CUPS_AMOUNT-1; I >= 0; --I)
    {
        if ((cups[I][ciDifference] != 0) || (cups[I][ciShowCounter] != 0))
        {
            return true;
        }
    }
    return false;
};
