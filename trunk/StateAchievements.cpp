#include "StateAchievements.h"

StateAchievements::StateAchievements()
{
    bgBuffer = NULL;
}

StateAchievements::~StateAchievements()
{
	clear();
}

void StateAchievements::clear()
{
	// Clear score data from variable cache
	variables[2].setInt(0);
}
void StateAchievements::init()
{
	#ifdef PENJIN_SDL
        buff.update();
        SDL_Surface* screen = GFX::getVideoSurface();
        bgBuffer = SDL_CreateRGBSurface(screen->flags,GFX::getXResolution(), GFX::getYResolution(), screen->format->BitsPerPixel, 0, 0, 0, 0);
        Random::setLimits(0,255);
        SDL_FillRect(bgBuffer, NULL, SDL_MapRGB(bgBuffer->format,Random::nextInt(),Random::nextInt(),Random::nextInt()));
        SDL_FillRect(screen, NULL, NULL);
    #endif
    text.loadFont("font/foo.ttf",30);
    text.setColour(Colour(WHITE));
    buttonSheet.loadFrames("images/ButtonPrompter/ButtonsSheet.png",10,2);
    back.loadBackground("images/pp_bg.png");
    #ifdef PENJIN_SDL
        back.setUseHardware(false);
        back.render(bgBuffer);
    #else
        back.render();
    #endif

    offset = 0;
}

void StateAchievements::userInput()
{
	input->update();
#ifdef PLATFORM_PC
    if(input->isQuit())
        nullifyState();
#endif

    if(input->isUp() || input->isLeftStickUp())
    {
        if (offset > 0)
            --offset;
    }
    else if(input->isDown() || input->isLeftStickDown())
    {
        if (offset < ACHIEVEMENTS->achievementCount()-ACHIEVEMENTS->getListSize())
            ++offset;
    }

    else if(input->isB())
    {
        setNextState(STATE_TITLE);
    }

    input->resetKeys();
}

#ifdef PENJIN_SDL
    void StateAchievements::render(SDL_Surface *screen)
    {
        buff.update();
        buff.setAlpha(128);
        SDL_BlitSurface(bgBuffer,NULL,screen,NULL);
        buff.render();
        if (offset > 0)
        {
            buttonSheet.setCurrentFrame(16);
            buttonSheet.setPosition(540,50);
            buttonSheet.render(screen);
        }
        if (offset < ACHIEVEMENTS->achievementCount()-ACHIEVEMENTS->getListSize())
        {
            buttonSheet.setCurrentFrame(17);
            buttonSheet.setPosition(540,403);
            buttonSheet.render(screen);
        }
        buttonSheet.setCurrentFrame(11);
        buttonSheet.setPosition(0,440);
        buttonSheet.render(screen);
        text.setBoundaries(Vector2di(0,0),Vector2di(screen->w,screen->h));
        text.setPosition(0,5);
        text.setAlignment(TextClass::CENTRED);
        string temp = "Achievements unlocked: " + StringUtility::intToString(ACHIEVEMENTS->unlockedCount()) + "/" + StringUtility::intToString(ACHIEVEMENTS->achievementCount());
        text.print(screen,temp);
        text.setPosition(50,440);
        text.setAlignment(TextClass::LEFT_JUSTIFIED);
        text.print(screen,"Go Back");
        ACHIEVEMENTS->renderList(screen,offset);
    }
#else
    void StateAchievements::render()
    {
        back.render();
        ACHIEVEMENTS->renderList(offset);
    }
#endif

void StateAchievements::update()
{

}