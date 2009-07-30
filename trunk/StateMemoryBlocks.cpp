#include "StateMemoryBlocks.h"
#include "Image.h"
#include <cmath>
#include <string>

StateMemoryBlocks::StateMemoryBlocks() :
command(), timer(), counter(), background(), combination(),
combinationLength(4), blocks(), slot(), cursor(), currentSlot(0),
keyPressLimiter(false)
{
    nullify = false;

    command.loadFont("font/origami.ttf",48);
    command.setRelativity(false);
    command.setPosition(220,150);
    counter.setMode(SECONDS);

    timer.loadFont("font/chromo.ttf",60);
    timer.setRelativity(false);
    timer.setPosition(390,120);

    int const NUM_BLOCK_IMAGES = 5;
    std::string const BLOCK_IMAGES[] =
		    { "images/MemoryBlocks/red_block.png",
		      "images/MemoryBlocks/blue_block.png",
		      "images/MemoryBlocks/green_block.png",
		      "images/MemoryBlocks/purple_block.png",
		      "images/MemoryBlocks/yellow_block.png" };

    for( int i = 0; i < NUM_BLOCK_IMAGES; ++i )
    {
        Image* img = new Image();
        img->loadImage(BLOCK_IMAGES[i]);
        blocks.push_back(img);
    }

    background.loadBackground("images/MemoryBlocks/background.png");
    slot.loadImage("images/MemoryBlocks/slot.png");
    cursor.loadFrames("images/MemoryBlocks/cursor.png", 4, 1);
    cursor.setLooping(true);
    cursor.setFrameRate(DECI_SECONDS);
    /*
    enemy.loadSprite("images/RotDef/bomb.png");
    turret.loadSprite("images/RotDef/turret_top.png");
    turretBase.loadSprite("images/RotDef/turret_base.png");
    shot.loadSprite("images/RotDef/bullet.png");

    */
}


StateMemoryBlocks::~StateMemoryBlocks()
{
    for( int i = 0; i < (int)blocks.size(); ++i )
    {
        delete blocks.at(i);
        blocks.at(i) = 0;
    }
}

void StateMemoryBlocks::init()
{
    int levelNumber = variables[2].getInt();
    if( levelNumber < 10 )
    {
        combinationLength = 4;
    }
    else if( levelNumber < 20 )
    {
        combinationLength = 5;
    }
    else
    {
        combinationLength = 6;
    }

    combination.clear();
    for( int i = 0; i < (int)combinationLength; ++i )
    {
        combination.push_back( BlockSlot( rand() % blocks.size(),
                                          rand() % blocks.size() ) );
    }
    counter.start();
}

void StateMemoryBlocks::userInput()
{
    if( counter.getScaledTicks() < 3 )
    {
        return;
    }
    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif

    if(input->isLeft())
    {
        if( !keyPressLimiter )
        {
            if( currentSlot != 0 )
            {
                currentSlot -= 1;
            }
            else
            {
                currentSlot = combinationLength - 1;
            }
            keyPressLimiter = true;
        }
    }

    else if(input->isRight())
    {
        if( !keyPressLimiter )
        {
            currentSlot = (currentSlot + 1) % combinationLength;
            keyPressLimiter = true;
        }
    }

    else if(input->isUp())
    {
        if( !keyPressLimiter )
        {
            combination.at(currentSlot).current =
                (combination.at(currentSlot).current + 1) % blocks.size();
            keyPressLimiter = true;
        }
    }

    else if(input->isDown())
    {
        if( !keyPressLimiter )
        {
            if( combination.at(currentSlot).current != 0 )
            {
                combination.at(currentSlot).current -= 1;
            }
            else
            {
                combination.at(currentSlot).current = blocks.size() - 1;
            }

            keyPressLimiter = true;
        }
    }

    else
    {
        keyPressLimiter = false;
    }

    if(input->isStart())
    {
        isPaused = !isPaused;
        input->resetKeys();
    }
}

#ifdef PENJIN_SDL
void StateMemoryBlocks::pauseScreen(SDL_Surface* screen)
{
    SDL_Rect t;
    t.x = 0;
    t.y = getStateYResolution()*0.35f;
    t.w = getStateXResolution();
    t.h = getStateYResolution()*0.33f;
    SDL_FillRect(screen, &t,SDL_MapRGB(screen->format, 0,0,0));
    pauseSymbol(screen);
    command.print(screen, "Memorize!");
}
#else
void StateMemoryBlocks::pauseScreen()
{/*
    SDL_Rect t;
    t.x = 0;
    t.y = getStateYResolution()*0.35f;
    t.w = getStateXResolution();
    t.h = getStateYResolution()*0.33f;
    SDL_FillRect(screen, &t,SDL_MapRGB(screen->format, 0,0,0));
    pauseSymbol(screen);
    command.print(screen, "Memorize!");
    */
}
#endif

void StateMemoryBlocks::onPause()
{
    counter.pause();
}

void StateMemoryBlocks::onResume()
{
    counter.unpause();
}

#ifdef PENJIN_SDL
void StateMemoryBlocks::render(SDL_Surface *screen)
{
    background.render(screen);
    int yCenter = 480*0.5f;
    int xCenter = 800*0.5f;
    int gap = 20;
    int xStart = xCenter - (combination.size()*(slot.getWidth()+gap) - gap)*0.5f;

    for( int i = 0; i < (int)combination.size(); ++i )
    {
        if( counter.getScaledTicks() < 3 )
        {
            command.print(screen, "Memorize!");
        }

        int xSlot = xStart + i * (slot.getWidth()+gap);
        int ySlot = yCenter - slot.getHeight()*0.5f;
        slot.renderImage(screen, xSlot, ySlot);
        if( counter.getScaledTicks() < 3 )
        {
            blocks.at(combination.at(i).target)->renderImage(screen, xSlot+12, ySlot+12);
        }
        else
        {
            blocks.at(combination.at(i).current)->renderImage(screen, xSlot+12, ySlot+12);
        }

        if( i == currentSlot )
        {
            cursor.setPosition(xSlot - (cursor.getWidth() - slot.getWidth())*0.5f,
                               ySlot - (cursor.getHeight() - slot.getHeight())*0.5f);
            cursor.render(screen);
        }
    }
    if( counter.getScaledTicks() >= 3 )
    {
        timer.print( screen, 11 - counter.getScaledTicks() );
    }
}
#else
void StateMemoryBlocks::render()
{
   background.render();
    int yCenter = 480*0.5f;
    int xCenter = 800*0.5f;
    int gap = 20;
    int xStart = xCenter - (combination.size()*(slot.getWidth()+gap) - gap)*0.5f;

    for( int i = 0; i < (int)combination.size(); ++i )
    {
        if( counter.getScaledTicks() < 3 )
        {
            command.print( "Memorize!");
        }

        int xSlot = xStart + i * (slot.getWidth()+gap);
        int ySlot = yCenter - slot.getHeight()*0.5f;
        slot.renderImage(xSlot, ySlot);
        if( counter.getScaledTicks() < 3 )
        {
            blocks.at(combination.at(i).target)->renderImage(xSlot+12, ySlot+12);
        }
        else
        {
            blocks.at(combination.at(i).current)->renderImage( xSlot+12, ySlot+12);
        }

        if( i == currentSlot )
        {
            cursor.setPosition(xSlot - (cursor.getWidth() - slot.getWidth())*0.5f,
                               ySlot - (cursor.getHeight() - slot.getHeight())*0.5f);
            cursor.render();
        }
    }
    if( counter.getScaledTicks() >= 3 )
    {
        timer.print( 11 - counter.getScaledTicks() );
    }
}
#endif

void StateMemoryBlocks::update()
{
    bool ready = true;

    for( unsigned int i = 0; i < combination.size(); ++i )
    {
        if( combination.at(i).current != combination.at(i).target )
        {
            ready = false;
            break;
        }
    }
    if( ready )
    {
        variables[0].setInt(1);
        setNextState(STATE_MAIN);
    }

    if(counter.getScaledTicks() > 11)
    {
        variables[0].setInt(0);
        setNextState(STATE_MAIN);
    }
    cursor.update();
}
