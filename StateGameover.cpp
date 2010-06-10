#include "StateGameover.h"

StateGameover::StateGameover()
{
    bgBuffer = NULL;
}

StateGameover::~StateGameover()
{
	clear();
}

void StateGameover::clear()
{
	// Clear score data from variable cache
	variables[2].setInt(0);
    if(bgBuffer != NULL)
        SDL_FreeSurface(bgBuffer);
}
void StateGameover::init()
{
	SDL_Rect bounds;
	bounds.x = bounds.y = 0;
	bounds.w = GFX::getXResolution();
	bounds.h = GFX::getYResolution();
	scoreScreen.setBoundaries(bounds);
	scoreScreen.setScoreFile("panic.scr");
	scoreScreen.setTableTitle("PandoraPanic! Pros!");
	scoreScreen.loadFont("font/foo.ttf", 28);
	scoreScreen.setFontColour(Colour(WHITE));
	scoreScreen.setNameEntry(true);
    const static uint SCORE = 2;
	//	The game over state is normally triggered by death in the main game
	//	Because of this we should check the variable cache for the player details
	if(variables.size()>SCORE && variables[SCORE].hasInt() && variables[SCORE].getInt() != 0)
	{
        scoreScreen.setScore(variables[SCORE].getInt());
        ACHIEVEMENTS->logEvent("GAME_SCORE",variables[SCORE].getInt());
	}
	scoreScreen.init();
	if(scoreScreen.inNameEntryScreen())
	{
        music.loadMusic("music/Rick Kelsall - PP Interlude.ogg");
        music.setLooping(true);
        music.play();
        text.loadFont("font/foo.ttf", 28);
	}
	else
	{
        prompt.setDefaultX(700);
        prompt.setDefaultY(400);
        prompt.setFlashQuantity(4);
        prompt.display();
    }
	#ifdef PENJIN_SDL
        buff.update();
        SDL_Surface* screen = GFX::getVideoSurface();
        bgBuffer = SDL_CreateRGBSurface(screen->flags,GFX::getXResolution(), GFX::getYResolution(), screen->format->BitsPerPixel, 0, 0, 0, 0);
        Random::setLimits(0,255);
        SDL_FillRect(bgBuffer, NULL, SDL_MapRGB(bgBuffer->format,Random::nextInt(),Random::nextInt(),Random::nextInt()));
        SDL_FillRect(screen, NULL, NULL);
    #endif
    back.loadBackground("images/pp_bg.png");
    #ifdef PENJIN_SDL
        back.setUseHardware(false);
    #endif
    logo.loadSprite("images/pp_logo_large.png");
    logo.setPosition(Vector2di(GFX::getXResolution()/4,GFX::getYResolution()/3));
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
    input->resetKeys();
}

void StateGameover::userInput()
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

    if(input->isA())// || input->isKey("KP_ENTER") || input->isKey("RETURN"))
        scoreScreen.charInput();
    else if(input->isB())// || input->isKey("BACKSPACE"))
        scoreScreen.charDelete();

    //  Keyboard pass-through
    string t = input->isKeyLetter();
    if(t != "NULL")
    {
        scoreScreen.charInput(t.at(0));
        input->resetKeys();
    }

    //	Check to return to main menu
    if((input->isA() || input->isStart() || input->isTouch()) && !scoreScreen.inNameEntryScreen())
    {
        //variables.resize(2);
        variables[2].setInt(0);
        setNextState(STATE_GAMEOVERORDERED);
    }
    input->resetKeys();
}

#ifdef PENJIN_SDL
    void StateGameover::render(SDL_Surface *screen)
    {
        buff.update();
        buff.setAlpha(128);
        SDL_BlitSurface(bgBuffer,NULL,screen,NULL);
        buff.render();
        scoreScreen.render(screen);
        if(scoreScreen.inNameEntryScreen())
        {
            //  up and down arrows
            prompt.renderImage(16,360,350);prompt.renderImage(17,360,440);
            // a button
            prompt.renderImage(10,50,400);text.setPosition(100,400);text.print("Enter");
            //  b button
            prompt.renderImage(11,50,440);text.setPosition(100,440);text.print("Delete");
        }
        else
            prompt.render();
    }
#else
    void StateGameover::render()
    {
           // gfx.setBackBuffer();
            //gfx.setBackBufferAlpha(128);
            //SDL_BlitSurface(bgBuffer,NULL,screen,NULL);
            //gfx.renderBackBuffer(screen);
            scoreScreen.render();
    }
#endif

void StateGameover::update()
{
    scoreScreen.update();
}

