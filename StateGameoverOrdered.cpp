#include "StateGameover.h"

StateGameoverOrdered::StateGameoverOrdered()
{
bgBuffer = NULL;
}

StateGameoverOrdered::~StateGameoverOrdered()
{
	clear();
}

void StateGameoverOrdered::clear()
{
	// Clear score data from variable cache
	variables[2].setInt(0);
    if(bgBuffer != NULL)
        SDL_FreeSurface(bgBuffer);
}
void StateGameoverOrdered::init()
{
	SDL_Rect bounds;
	bounds.x = bounds.y = 0;
	bounds.w = getStateXResolution();
	bounds.h = getStateYResolution();
	scoreScreen.setBoundaries(bounds);
	scoreScreen.setScoreFile("pandordered.scr");
	scoreScreen.setTableTitle("Pandordered! Pros!");
	scoreScreen.loadFont("font/foo.ttf", 28);
	scoreScreen.setFontColour(Colour(WHITE));
	scoreScreen.setNameEntry(true);
    const static uint SCORE = 2;
	//	The game over state is normally triggered by death in the main game
	//	Because of this we should check the variable cache for the player details
	if(variables.size()>SCORE)
	{
		if(variables[SCORE].hasInt())
		    if(variables[SCORE].getInt() != 0)
                scoreScreen.setScore(variables[SCORE].getInt());
	}
	scoreScreen.init();
	if(scoreScreen.inNameEntryScreen())
	{
        music.loadMusic("music/Rick Kelsall - PP Interlude.ogg");
        music.setLooping(true);
        music.play();
	}

	buff.update();

	#ifdef PENJIN_SDL
        bgBuffer = SDL_CreateRGBSurface(screen->flags,getStateXResolution(), getStateYResolution(), screen->format->BitsPerPixel, 0, 0, 0, 0);
        Random::setLimits(0,255);
        SDL_FillRect(bgBuffer, NULL, SDL_MapRGB(bgBuffer->format,Random::nextInt(),Random::nextInt(),Random::nextInt()));
        SDL_FillRect(screen, NULL, NULL);
    #endif

    back.loadBackground("images/pp_bg.png");
    #ifdef PENJIN_SDL
        back.setUseHardware(false);
    #endif
    logo.loadSprite("images/pp_logo_large.png");
    logo.setPosition(Vector2di(getStateXResolution()/4,getStateYResolution()/3));
    #ifdef PENJIN_SDL
        back.render(bgBuffer);
    #else
        back.render();
    #endif
    logo.setAlpha(32);
    #ifdef PENJIN_SDL
        logo.render(bgBuffer);
    #else
        logo.render();
    #endif
}

void StateGameoverOrdered::userInput()
{
	//	Refresh inputs
	input->update();
#ifdef PLATFORM_PC
    if(input->isQuit())
        nullifyState();
#endif

    if(input->isUp() || input->isLeftStickUp())
        scoreScreen.charUp();
    else if(input->isDown() || input->isLeftStickDown())
        scoreScreen.charDown();


    if(input->isA())
        scoreScreen.charInput();
    else if(input->isX())
        scoreScreen.charDelete();
/// TODO add Start button to confirm name in score screen.

    //	Check to return to main menu
    if((input->isA() || input->isStart() || input->isTouch()) && !scoreScreen.inNameEntryScreen())
    {
        //variables.resize(2);
        variables[2].setInt(0);
        setNextState(STATE_TITLE);
    }
    input->resetKeys();
}

#ifdef PENJIN_SDL
    void StateGameoverOrdered::render(SDL_Surface *screen)
    {
            buff.update();
            buff.setAlpha(128);
            SDL_BlitSurface(bgBuffer,NULL,screen,NULL);
            buff.render();
            scoreScreen.render();
    }
#else
    void StateGameoverOrdered::render()
    {
            //gfx.setBackBuffer(screen);
            //gfx.setBackBufferAlpha(128);
            //SDL_BlitSurface(bgBuffer,NULL,screen,NULL);
            //gfx.renderBackBuffer(screen);
            scoreScreen.render();
    }

#endif
void StateGameoverOrdered::update()
{
scoreScreen.update();
}

