#include "StateAchievements.h"
#include "TextDoc.h"
StateAchievements::StateAchievements()
{
    bgBuffer = NULL;
}

StateAchievements::~StateAchievements()
{
    TextDoc doc;
    if (ACHIEVEMENTS->showingPopups())
        doc.append("ShowPopup = true");
    else
        doc.append("ShowPopup = false");
    doc.save("config.conf");
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
        //buff.update();
        SDL_Surface* screen = GFX::getVideoSurface();
        bgBuffer = SDL_CreateRGBSurface(screen->flags,GFX::getXResolution(), GFX::getYResolution(), screen->format->BitsPerPixel, 0, 0, 0, 0);
        Random::setLimits(0,255);
        SDL_FillRect(bgBuffer, NULL, SDL_MapRGB(bgBuffer->format,Random::nextInt(),Random::nextInt(),Random::nextInt()));
        SDL_FillRect(screen, NULL, NULL);
    #endif
    text.loadFont("font/foo.ttf",30);
    text.setColour(Colour(WHITE));
    text.setRelativity(true);
    text.setBoundaries(Vector2di(0,0),Vector2di(screen->w-360,screen->h));
    buttonSheet.loadFrames("images/ButtonPrompter/ButtonsSheet.png",10,2);
    back.loadBackground("images/pp_bg.png");
    #ifdef PENJIN_SDL
        back.render(bgBuffer);
    #else
        back.render();
    #endif

    offset = 0;
    speed = 0.1f;
    timer.init(100,MILLI_SECONDS,this,&StateAchievements::increaseSpeed);
    timer.setRewind(REWIND);
    ACHIEVEMENTS->setOffset(450,0);
    TextDoc doc;
    doc.load("config.conf");
    string line;
    for(uint i = 0; i < doc.size(); ++i)
    {
        line = StringUtility::lower(doc.getLine(i));
        if(line.find("showpopup") != string::npos)
        {
            if(line.find("yes") != string::npos || line.find("y") != string::npos || line.find("t") != string::npos || line.find("true") != string::npos )
                ACHIEVEMENTS->enablePopups();
            else
                ACHIEVEMENTS->disablePopups();
        }
    }
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
        speed = 0.1f;
    }

    if (input->isY())
    {
        if (ACHIEVEMENTS->showingPopups())
            ACHIEVEMENTS->disablePopups();
        else
            ACHIEVEMENTS->enablePopups();
        input->resetKeys();
    }

    if(input->isX())
    {
        setNextState(STATE_TITLE);
        input->resetKeys();
    }

}

#ifdef PENJIN_SDL
    void StateAchievements::render(SDL_Surface *screen)
    {
        //buff.update();
        //buff.setAlpha(128);
        SDL_BlitSurface(bgBuffer,NULL,screen,NULL);
        //buff.render();
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
        text.setFontSize(30);
        text.setAlignment(TextClass::LEFT_JUSTIFIED);

        buttonSheet.setCurrentFrame(13);
        buttonSheet.setPosition(0,400);
        buttonSheet.render(screen);
        text.setPosition(50,400);
        if (ACHIEVEMENTS->showingPopups())
            text.print(screen,"Hide popups");
        else
            text.print(screen,"Show popups");

        buttonSheet.setCurrentFrame(12);
        buttonSheet.setPosition(0,440);
        buttonSheet.render(screen);
        text.setPosition(50,440);
        text.print(screen,"Go Back");
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
    ((StateAchievements*) object)->speed *= 1.2f;
}
