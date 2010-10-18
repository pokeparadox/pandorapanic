#include "StateTitle.h"

StateTitle::StateTitle()
{
    //ctor
    nullify = false;
    needInit = true;
    returnToCentre = true;
    nextState = 0;
    Random::setLimits(0,255);
    timer.setMode(MILLI_SECONDS);
    timer.start();
    startTimer.setMode(MILLI_SECONDS);
    startTimer.start();
    splashDone = false;
    showStart = 0;
    gradual = Random::nextBool();
    gradualScaler = Random::nextInt(4,16);
    #ifdef PLATFORM_GP2X
        spinLogo = false;
        scaleLogo = false;
    #else
        spinLogo = Random::nextBool();
        scaleLogo = Random::nextBool();
    #endif
    angle = 0.0f;
    angleDelta = Random::nextFloat(0,15)*0.1f;
    scaleDelta.x = 0.1f;
    scaleDelta.y = 0.1f;
    scale.x = 1.0f;
    scale.y = 1.0f;
    bgBuffer = NULL;
}

StateTitle::~StateTitle()
{
    //dtor
    if(bgBuffer)
        SDL_FreeSurface(bgBuffer);
    bgBuffer = NULL;
}

void StateTitle::init()
{
    uint xRes = GFX::getXResolution();
    uint yRes = GFX::getYResolution();
    prompt.setDefaultX(xRes/8 * 7);//700);
    prompt.setDefaultY(yRes * 0.833333333f);//400);
    prompt.setFlashQuantity(4);
    backColour.setColour((uchar)Random::nextInt(),Random::nextInt(),Random::nextInt());
    muteSprite.setCurrentFrame(SoundClass::getGlobalVolume() == 0);

    GFX::setClearColour(backColour);
    newColour = backColour;
    emit.setMax(200);
    emit.setInvisible(150);
    const string path = "images/";
    const string ext = ".png";
    emit.loadSprite(path+"pp_star"+ext);
    emit.shouldFadeColour(false);
    #ifdef PENJIN_GL
        GFX::init2DRendering(*xRes,*yRes);
    #endif
    emit.setPosition(Vector2di(xRes*0.5f,yRes*0.5f));
    emit.setBoundaries(Vector2di(-64,-64),Vector2di(xRes,yRes));
    emit.setMaxVelocity(Vector2df(6,6));
    emit.setLifeTime(6000);
    emit.shouldFadeColour(false);
    emit.setDeviation(0.5f);

    back.loadBackground(path+"pp_bg"+ext);
    #ifdef PENJIN_SDL
        SDL_Surface* t = SDL_GetVideoSurface();
        bgBuffer = SDL_CreateRGBSurface(t->flags,GFX::getXResolution(), GFX::getYResolution(), t->format->BitsPerPixel, 0, 0, 0, 0);
        GFX::clearScreen(bgBuffer);
    #endif

    #ifdef PENJIN_SDL
        back.render(bgBuffer);
    #else
        back.render();
    #endif
    splash.loadSprite(path+"pp_logo_large"+ext);
    splash.setPosition(Vector2di(GFX::getXResolution()*0.25f,GFX::getYResolution()*0.33f));
    //splash.setUseHardware(true);
    startButton.loadSprite(path+"pp_start"+ext);
    startButton.setPosition(Vector2di(GFX::getXResolution()/2.8f,GFX::getYResolution()/1.5f));
    #ifdef PENJIN_SDL
        startButton.setUseHardware(true);
    #endif

    music.loadMusic("music/PandoraPanic_Title.ogg");
    music.setLooping(true);
    music.play();

    input->setScaler(0.0006f);
    input->setDeadZone(Vector2di(4,4));

    // The game needs to pass the following variables
    /// 0 - PASS / FAIL  0 is fail 1 is pass
    /// 1 - Game mode
    /// 2 - CurrentStageNo for score purposes
    /// 3 - Next stage for ordered progression
    /// 4 - RemainingLives
    variables.resize(8);
    variables[0].setInt(1);    //  Pass/fail
    variables[1].setInt(0);     //  0 random, 1 ordered,2 selection
    variables[2].setInt(0);     // currentstage 0 :)
    variables[3].setInt(0);     //  Start at beginning
    variables[4].setInt(3);     //  Start with 4 lives
    variables[5].setInt(0);     //  MenuSelection
    variables[6].setInt(1);     //  First run?
    variables[7].setInt(time(NULL)); // timestamp per run
    setupMainMenu();
#ifdef PLATFORM_PC
    SDL_ShowCursor(SDL_ENABLE);
#endif
}

void StateTitle::setupMainMenu()
{
    choice = -1;
    showPrompt = false;
    menu.clear();
    menu.setMenuStart(GFX::getXResolution()*0.25f - 15,12);
    menu.setSelection(1);
    menu.loadFont("font/foo.ttf", 28);
    menu.setTextColour(Colour(YELLOW));
    menu.setTextSelectionColour(Colour(WHITE));
    menu.setTextBgColour(RED);
    menu.addItem(MENU_IMAGE_ITEM);
    menu.setIsSelectable(false);
    const string path = "images/menu/pp_";
    const string ext =".png";
    menu.loadImage(path+"logo_small"+ext);
    const int spacing = 5;
    menu.setSpacing(spacing);
    menu.setHorizontalOffset(110);
    menu.setTextCenteringCorrection(-42);

    menu.addItem(MENU_IMAGE_STRING_ITEM);
    menu.setIsSelectable(true);
    menu.setMenuItemText("PandoraPanic!");
    menu.loadImage(path+"menu"+ext);
    menu.loadSelectionImage(path+"menu_selected"+ext);
    menu.setSpacing(spacing);
    menu.addItem(MENU_IMAGE_STRING_ITEM);
    menu.setIsSelectable(true);
    menu.setMenuItemText("Pandordered!");
    menu.loadImage(path+"menu"+ext);
    menu.loadSelectionImage(path+"menu_selected"+ext);
    menu.setSpacing(spacing);
    menu.addItem(MENU_IMAGE_STRING_ITEM);
    menu.setIsSelectable(true);
    menu.setMenuItemText("SelectionBox!");
    menu.loadImage(path+"menu"+ext);
    menu.loadSelectionImage(path+"menu_selected"+ext);
    menu.setSpacing(spacing);
    menu.addItem(MENU_IMAGE_STRING_ITEM);
    menu.setIsSelectable(true);
    menu.setMenuItemText("Scores!");
    menu.loadImage(path+"menu"+ext);
    menu.loadSelectionImage(path+"menu_selected"+ext);
    menu.setSpacing(spacing);
    menu.addItem(MENU_IMAGE_STRING_ITEM);
    menu.setIsSelectable(true);
    menu.setMenuItemText("Achievements!");
    menu.loadImage(path+"menu"+ext);
    menu.loadSelectionImage(path+"menu_selected"+ext);
    menu.setSpacing(spacing);

    menu.addItem(MENU_IMAGE_STRING_ITEM);
    menu.setIsSelectable(true);
    menu.setMenuItemText("Credits!");
    menu.loadImage(path+"menu"+ext);
    menu.loadSelectionImage(path+"menu_selected"+ext);
    menu.setSpacing(spacing);

    menu.addItem(MENU_IMAGE_STRING_ITEM);
    menu.setIsSelectable(true);
    menu.setMenuItemText("Quit!");
    menu.loadImage(path+"menu"+ext);
    menu.loadSelectionImage(path+"menu_selected"+ext);
    menu.setSpacing(spacing);
    menu.centreText();
}

void StateTitle::mainMenu()
{
    // The game needs to pass the following variables
    /// 0 - PASS / FAIL  0 is fail 1 is pass
    /// 1 - Game mode
    /// 2 - CurrentStageNo for score purposes
    /// 3 - Next stage for ordered progression
    /// 4 - RemainingLives

    if (choice == 1)        /// PandoraPanic
    {
        variables[1].setInt(0); //  random
        setNextState(STATE_MAIN);
        getReady();
    }
    else if (choice == 2)   /// Pandoredered
    {
        variables[1].setInt(1);
        setNextState(STATE_MAIN);
        getReady();
    }
    else if (choice == 3)   /// SelectionBox
    {
        variables[1].setInt(2);
        setNextState(STATE_MAIN);
    }
    else if (choice == 4)   /// HiScores screen
        setNextState(STATE_GAMEOVER);
    else if (choice == 5)   /// Achievements screen
        setNextState(STATE_ACHIEVEMENTS);
    else if (choice == 6)   /// Credits screen
        setNextState(STATE_CREDITS);
    else if(choice == 7)    /// Quit
        nullifyState();
}

void StateTitle::update()
{
    emit.update();
    if(backColour != newColour && !splashDone)
    {
        if(gradual)
        {
            int step = 1;
            if(backColour.red < newColour.red)
                backColour.red+=step;
            else if(backColour.red > newColour.red)
                backColour.red-=step;
            if(backColour.green < newColour.green)
                backColour.green+=step;
            else if(backColour.green > newColour.green)
                backColour.green-=step;
            if(backColour.blue < newColour.blue)
                backColour.blue+=step;
            else if(backColour.blue > newColour.blue)
                backColour.blue-=step;
            if(showStart%gradualScaler == 0)
            {
                GFX::setClearColour(backColour);
                #ifdef PENJIN_SDL
                    GFX::clearScreen(bgBuffer);
                    back.render(bgBuffer);
                #else
                    GFX::clearScreen();
                    back.render();
                #endif
            }
        }
        else
        {
            backColour = newColour;
            angleDelta = -angleDelta;
            GFX::setClearColour(backColour);
            #ifdef PENJIN_SDL
                GFX::clearScreen(bgBuffer);
                back.render(bgBuffer);
            #else
                back.render();
            #endif
        }
    }
    else if(splashDone)
    {
        menu.update();
        mainMenu();
        if(timer.getScaledTicks() > 500 && showPrompt == false)
        {
            showPrompt = true;
            prompt.display();
        }
    }
    else
    {
        if(scaleLogo)
        {
            if(scale.x > 1.4f)
                scaleDelta.x = -((Random::nextFloat(0,1)/15)-0.01f);
            else if(scale.x < 0)
                scaleDelta.x = (Random::nextFloat(0,1)*0.1f)-0.01f;
            else if(scale.x == 0)
            {
                scale.x =0.001f;
                scaleDelta.x = (Random::nextFloat(0,1)*0.1f)-0.01f;
            }
            /*if(scale.y > 2.5f)
                scaleDelta.y = -rand.nextFloat(0,1)/10;
            if(scale.y <= 0)
                scaleDelta.y = rand.nextFloat(0,1)/10;
*/
            scale.x = scale.x + scaleDelta.x;
            splash.setScale(scale.x);
        }
        if(spinLogo)
        {
            angle+=angleDelta;
            splash.setRotation(angle);
        }
        if(startTimer.getScaledTicks() > 451)
        {
            startTimer.start();
            ++showStart;

            if(showStart > 91)
                splashDone = true;
        }
    }
    if(timer.getScaledTicks() > 902)
    {
        timer.start();
        newColour.setColour((uchar)Random::nextInt(),Random::nextInt(),Random::nextInt());
    }
}

#ifdef PENJIN_SDL
void StateTitle::render(SDL_Surface* screen)
{
    /// Clear Screen with prerendered background
    SDL_BlitSurface(bgBuffer,NULL,screen,NULL);
    emit.render(screen);
    if(splashDone)
    {
        menu.render(screen);
        prompt.render();
        muteSprite.render();
    }
    else
    {
        splash.render(screen);
        if(showStart%2 == 0)
        {
            startButton.render(screen);
        }
    }
}
#else
void StateTitle::render()
{
    GFX::clearScreen();
    back.render();
    emit.render();
    if(splashDone)
    {
        menu.render();
    }
    else
    {
        splash.render();
        if(showStart%2 == 0)
        {
            startButton.render();
        }
    }
}
#endif
void StateTitle::getReady()
{
    SDL_Surface* screen = SDL_GetVideoSurface();
    SDL_BlitSurface(bgBuffer,NULL,screen,NULL);
    splash.setRotation(0.0f);
    splash.setScaleX(1.0f);
    splash.setScaleY(1.0f);
    #ifdef PENJIN_SDL
        splash.render(screen);
    #else
        splash.render();
    #endif
    Text t;
    t.loadFont("font/foo.ttf", 28);
    SDL_Rect r;
    r.x = r.y = 0;
    r.w = GFX::getXResolution();
    r.h = GFX::getYResolution();
    t.setBoundaries(r);
    t.setRelativity(false);
    t.setPosition(0,0.7f*r.h);
    t.setAlignment(TextClass::CENTRED);
    t.print("Get Ready!");
    GFX::forceBlit();
    SDL_Delay(1500);
}

void StateTitle::userInput()
{
    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif
    if(splashDone)
    {
        menu.setMouseSelection(input->getTouch());
        if(input->isTouch() || input->isA() || input->isStart())
        {
            choice = menu.getSelection();
            input->resetKeys();
        }
        if(input->isUp())
        {
            menu.menuUp();
            input->resetKeys();
            timer.start();
            showPrompt = false;
        }
        else if(input->isDown())
        {
            menu.menuDown();
            input->resetKeys();
            timer.start();
            showPrompt = false;
        }
        if(returnToCentre)
        {
            if(input->isLeftStickUp())
            {
                menu.menuUp();
                input->resetKeys();
            }
            else if(input->isLeftStickDown())
            {
                menu.menuDown();
                input->resetKeys();
            }
        }
        else if(!input->isLeftStick())
            returnToCentre = true;

        //  Mute button
        if(input->isR())
        {
            muteToggle();
            input->resetR();
        }
    }
    else
    {
        if(input->isTouch() || input->isA() || input->isStart())
        {
            splashDone = true;
            input->resetKeys();
        }
    }
}
