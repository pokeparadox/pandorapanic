#include "StateReflex.h"
StateReflex::StateReflex()
{
    nullify = false;

    text.loadFont("font/atrox.ttf", 125);
    text.setColour(Colour(YELLOW));
    pauseText.loadFont("font/bip.ttf", 32);
    pauseText.setColour(WHITE);

    counter.setMode(SECONDS);
    circle.loadSprite("images/Reflex/circle.png");
    arrow.loadSprite("images/Reflex/arrow.png");

    leftSound.loadSound("sounds/Reflex/left.ogg");
    rightSound.loadSound("sounds/Reflex/right.ogg");
    downSound.loadSound("sounds/Reflex/down.ogg");
    upSound.loadSound("sounds/Reflex/up.ogg");
    GFX::setClearColour(BLACK);
}


StateReflex::~StateReflex()
{

}

void StateReflex::init()
{
    gameDuration=5;  // Seconds needed to play in order to win
    turnDuration=1000;  // Milliseconds of duration of each turn
    guide_xOffset=250;

    DOWN=1;
    LEFT=2;
    UP=3;
    RIGHT=4;
    currDirection=(rand()%4)+1;
    correct=false;
    circle.setPosition(guide_xOffset,4);
    arrow.setPosition(guide_xOffset,4);

    counter.start();
    correctStart=counter.getTicks();
}


void StateReflex::userInput()
{
    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif

    if (input->isStart())
    {

        isPaused = !isPaused;
        input->resetKeys();
    }

    // Tests if the input is in the right direction
    if(input->isLeft() && currDirection==LEFT)
    {
        correct=true;
        leftSound.play();
    }
    else if(input->isRight() && currDirection==RIGHT)
    {
        correct=true;
        rightSound.play();
    }
    else if(input->isDown() && currDirection==DOWN)
    {
        correct=true;
        downSound.play();
    }
    else if(input->isUp() && currDirection==UP)
    {
        correct=true;
        upSound.play();
    }
    else if(input->isLeft() || input->isRight() || input->isDown() || input->isUp())
    {
        variables[0].setInt(0);
        setNextState(STATE_MAIN);
    }
    input->resetKeys();
}

#ifdef PENJIN_SDL
void StateReflex::render(SDL_Surface *screen)
{
	GFX::clearScreen(screen);
    circle.render(screen);

    if(correct)
    {
        //  Flash the screen to break up the commands.
        GFX::setClearColour(WHITE);
        GFX::clearScreen(screen);
        GFX::setClearColour(BLACK);
        correctStart=counter.getTicks();
        currDirection=(rand()%4)+1;
        correct=false;
    }

    if(currDirection==DOWN)
    {
        arrow.setRotation(360);
    }
    else if(currDirection==LEFT)
    {
        arrow.setRotation(270);
    }
    else if(currDirection==UP)
    {
        arrow.setRotation(180);
    }
    else if(currDirection==RIGHT)
    {
        arrow.setRotation(90);
    }
    arrow.render(screen);

    // Displays remaining time
    text.setPosition(0,0);
    text.print(screen,"Game: ");
    text.setPosition(0,100);
    text.print(screen,(gameDuration)-counter.getScaledTicks());
    text.setPosition(0,200);
}
void StateReflex::pauseScreen(SDL_Surface* screen)
{
    // Pause screen
    pauseSymbol(screen);
    pauseText.setPosition(50,180);
    pauseText.print(screen, "Quickly press the D-Pad");
    pauseText.setPosition(50,220);
    pauseText.print(screen, "matching the direction on screen!");
}
#else
    void StateReflex::render()
    {
        GFX::clearScreen();
        circle.render();

        if(correct)
        {
            //  Flash the screen to break up the commands.
            GFX::setClearColour(WHITE);
            GFX::clearScreen();
            GFX::setClearColour(BLACK);
            correctStart=counter.getTicks();
            currDirection=(rand()%4)+1;
            correct=false;
        }

        if(currDirection==DOWN)
        {
            arrow.setRotation(360);
        }
        else if(currDirection==LEFT)
        {
            arrow.setRotation(270);
        }
        else if(currDirection==UP)
        {
            arrow.setRotation(180);
        }
        else if(currDirection==RIGHT)
        {
            arrow.setRotation(90);
        }
        arrow.render();

        // Displays remaining time
        text.setPosition(0,0);
        text.print("Game: ");
        text.setPosition(0,100);
        text.print((gameDuration)-counter.getScaledTicks());
        text.setPosition(0,200);
    }
    void StateReflex::pauseScreen()
    {
        // Pause screen
        pauseSymbol();
        pauseText.setPosition(50,180);
        pauseText.print("Quickly press the D-Pad");
        pauseText.setPosition(50,220);
        pauseText.print("matching the direction on screen!");
    }
#endif

void StateReflex::onPause()
{
    counter.pause();
}

void StateReflex::onResume()
{
    counter.unpause();
}

void StateReflex::update()
{
    // Tests if time ran out
    if(counter.getScaledTicks() >= gameDuration)
    {
        variables[0].setInt(1); // Pass
        setNextState(STATE_MAIN);
    }

    // Tests if turn was too long
    if((counter.getTicks()-correctStart)>turnDuration)
    {
        variables[0].setInt(0); // Fail
        setNextState(STATE_MAIN);
    }
}
