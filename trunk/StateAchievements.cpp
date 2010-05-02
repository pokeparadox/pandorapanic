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
    text.setRelativity(true);
    buttonSheet.loadFrames("images/ButtonPrompter/ButtonsSheet.png",10,2);
    back.loadBackground("images/pp_bg.png");
    #ifdef PENJIN_SDL
        back.setUseHardware(false);
        back.render(bgBuffer);
    #else
        back.render();
    #endif

    offset = 0;
    speed = 0.1;
    timer.init(100,MILLI_SECONDS,this,&StateAchievements::increaseSpeed);
    timer.setRewind(REWIND);
    ACHIEVEMENTS->setOffset(450,0);
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
        {
            offset -= min(speed,offset);
            if (!timer.isStarted())
                timer.start();
        }
    }
    else if(input->isDown() || input->isLeftStickDown())
    {
        if (offset < ACHIEVEMENTS->achievementCount()-ACHIEVEMENTS->getListSize())
        {
            offset += min(speed,ACHIEVEMENTS->achievementCount()-ACHIEVEMENTS->getListSize()-offset);
            if (!timer.isStarted())
                timer.start();
        }
    }
    else
    {
        timer.stop();
        speed = 0.1;
    }

    if(input->isB())
    {
        setNextState(STATE_TITLE);
        input->resetKeys();
    }

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
            buttonSheet.setPosition(400,10);
            buttonSheet.render(screen);
        }
        if (offset < ACHIEVEMENTS->achievementCount()-ACHIEVEMENTS->getListSize())
        {
            buttonSheet.setCurrentFrame(17);
            buttonSheet.setPosition(400,440);
            buttonSheet.render(screen);
        }
        buttonSheet.setCurrentFrame(11);
        buttonSheet.setPosition(0,440);
        buttonSheet.render(screen);
        text.setFontSize(30);
        text.setPosition(50,440);
        text.setAlignment(TextClass::LEFT_JUSTIFIED);
        text.print(screen,"Go Back");
        text.setBoundaries(Vector2di(0,0),Vector2di(screen->w-360,screen->h));
        text.setPosition(0,100);
        text.setAlignment(TextClass::CENTRED);
        text.print(screen, "Achievements unlocked:\n");
        text.setFontSize(48);
        string temp = StringUtility::intToString(ACHIEVEMENTS->unlockedCount()) + " of " + StringUtility::intToString(ACHIEVEMENTS->achievementCount()) + " ";
        text.print(screen,temp);
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
    timer.update();
}

void StateAchievements::increaseSpeed(void* object)
{
    if (((StateAchievements*) object)->speed >= 1)
        return;
    ((StateAchievements*) object)->speed *= 1.2;
}
