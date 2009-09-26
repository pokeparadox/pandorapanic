#include "StateMain.h"

StateMain::StateMain()
{
    //ctor
    nullify = false;
    needInit = true;
    nextState = 0;
    gotoNext = false;
    playFail = false;
    playWin = false;
    text.setRelativity(true);
    bgBuffer = NULL;
    preview = NULL;
}

StateMain::~StateMain()
{
    if(bgBuffer)
        SDL_FreeSurface(bgBuffer);
    bgBuffer = NULL;
    if(preview)
        SDL_FreeSurface(preview);
    preview = NULL;
}

void StateMain::loadCommon()
{
    logo.loadSprite("images/menu/pp_logo_small.png");
    logo.setPosition(Vector2di(getStateXResolution()/2.5f,getStateYResolution()/14));
    #ifdef PENJIN_SDL
        logo.setUseHardware(true);
    #endif
    text.loadFont("font/foo.ttf",30);
    text.setColour(Colour(WHITE));

    SDL_Rect bounds;
    bounds.h = getStateYResolution();
    bounds.w = getStateXResolution();
    bounds.x = bounds.y = 0;
    text.setBoundaries(bounds);
    text.setPosition(15,20);

    back.loadBackground("images/pp_bg.png");
    #ifdef PENJIN_SDL
        back.setUseHardware(false);
    #endif
    SDL_Surface* t = SDL_GetVideoSurface();
    bgBuffer = SDL_CreateRGBSurface(t->flags,getStateXResolution(), getStateYResolution(), t->format->BitsPerPixel, 0, 0, 0, 0);
    Random::setLimits(0,255);
    backColour.setColour((uchar)Random::nextInt(),Random::nextInt(),Random::nextInt());
    SDL_FillRect(bgBuffer, NULL, SDL_MapRGB(bgBuffer->format,backColour.red,backColour.green,backColour.blue));
    #ifdef PENJIN_SDL
        back.render(bgBuffer);
        logo.render(bgBuffer);
    #else
        back.render();
        logo.render();
    #endif
    prompt.setDefaultX(650);
    prompt.setDefaultY(400);
}

void StateMain::winLoseLoad()
{
    music.setLooping(false);
    music.play();
    life.loadSprite("images/pp_life.png");
    #ifdef PENJIN_SDL
        life.setUseHardware(true);
    #endif
    lostLife.loadSprite("images/pp_life_lost.png");
    #ifdef PENJIN_SDL
        lostLife.setUseHardware(true);
    #endif
    bar.loadSprite("images/pp_level_info.png");
    bar.setPosition(getStateXResolution()/3.6f,getStateYResolution()*0.5f);
    #ifdef PENJIN_SDL
        bar.setUseHardware(true);
    #endif
}

void StateMain::loadWin()
{
    music.loadMusic("music/PandoraPanic_Win.ogg");
    loseWin.loadSprite("images/pp_win.png");
    loseWin.setPosition(getStateXResolution()/2.5f,getStateYResolution()/1.5f);
    winLoseLoad();
}

void StateMain::loadLose()
{
    music.loadMusic("music/PandoraPanic_Loss.ogg");
    loseWin.loadSprite("images/pp_fail.png");
    loseWin.setPosition(getStateXResolution()/2.5f,getStateYResolution()/1.5f);
    #ifdef PENJIN_SDL
        loseWin.setUseHardware(true);
    #endif
    winLoseLoad();
}

void StateMain::init()
{
    // The game needs to pass the following variables
    /// 0 - PASS / FAIL  0 is fail 1 is pass
    /// 1 - Game mode
    /// 2 - CurrentStageNo for score purposes
    /// 3 - Next stage for ordered progression
    /// 4 - RemainingLives
    /// 5 - menu selection storage
    selection = variables[5].getInt();
    /// Did the previous stage pass or fail
    timer.setMode(SECONDS);
    timer.start();
    loadCommon();
    if(variables[0].getInt() == 0)//    FAIL
    {
        loadLose();
        playFail = true;
        int mode = variables[1].getInt();
        if(mode == 0)   //  pick a random next stage
        {
            Random::setLimits(1,MAX_GAMES);
            int current = variables[3].getInt();
            int next = current;
            while(next == current)
            {
                next = Random::nextInt();
            }
            variables[3].setInt(next);
            gotoNext = true;
        }
        else if (mode == 1)
        {
            if(variables[3].getInt() < MAX_GAMES)
                variables[3].setInt(variables[3].getInt()+1);
            else
                variables[3].setInt(1);
            gotoNext = true;
        }
        if(mode == 2)
        {
            gotoNext = false;
            genPreview(selection);
            return;
        }
        variables[4].setInt(variables[4].getInt()-1);
        gotoNext = variables[4].getInt() >= 0; // Check remaining lives
        if (!gotoNext)
        {
            int mode = variables[1].getInt();
            if(mode == 0)   //  go to pandorapanic hiscores
                variables[3].setInt(STATE_GAMEOVER-3);
            else if (mode == 1) // go to pandordered hiscores
                variables[3].setInt(STATE_GAMEOVERORDERED-3);
            gotoNext=true;
        }

    }
    else if (variables[0].getInt() == 1)// PASS
    {
        if(variables[6].getInt() != 1)
            playWin = true;

        if(playWin)
            loadWin();

        int stage = variables[2].getInt();
        variables[2].setInt(stage+1);

        /// Check if extra life awarded
        if(stage%EXTRA_LIFE_STAGES == 0)
        {
            int lives = variables[4].getInt();
            if(lives<3)
            {
                ++lives;
                variables[4].setInt(lives);
            }
        }

        /// Check game mode to go to next stage
        int mode = variables[1].getInt();
        if(mode == 0)   //  pick a random next stage
        {
            Random::setLimits(1,MAX_GAMES);
            int current = variables[3].getInt();
            int next = current;
            while(next == current)
            {
                next = Random::nextInt();
            }
            variables[3].setInt(next);
            gotoNext = true;
        }
        else if (mode == 1)
        {
            if(variables[3].getInt() < MAX_GAMES)
                variables[3].setInt(variables[3].getInt()+1);
            else
                variables[3].setInt(1);
            gotoNext = true;
        }
        else if(mode == 2)
        {
            genPreview(selection);
            gotoNext = false;
        }
    }
}

void StateMain::genPreview(uint next)
{
    next+=4;
    /// Prepare a temporary state
    BaseState* state = NULL;
    //Handle the states.
    #include "MiniGameStates.h"
    /// Create a compatible suface to render to
    if(preview)
        SDL_FreeSurface(preview);
    preview = NULL;

    /// Render and scale down image
    state->variables = this->variables;
    state->variables.resize(SUBSTATE_TRIGGER);
    state->setStateResolution(xRes,yRes);
    state->init();

#ifdef PENJIN_SDL
    SDL_Surface* t = SDL_GetVideoSurface();
    state->render(t);
    preview = rotozoomSurfaceXY(t, 0, 0.6f, 0.6f, SMOOTHING_ON);
    SDL_Rect trect;
    trect.x = *xRes*0.23f;
    trect.y = *yRes*0.3f;
    trect.w=preview->w;
    trect.h=preview->h;
    SDL_BlitSurface(preview,NULL,bgBuffer,&trect);
#else

#endif
    /// Tidy up
    if(state)
        delete state;
    state=NULL;
    prompt.display();
}

void StateMain::userInput()
{
    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif
    if(!gotoNext)
    {
        if (input->isX())
        {
            //input.resetKeys();
            if (input->isR())
            {
                variables[3].setInt(STATE_TILEEDITOR-3);
                gotoNext=true;
            }
        }
        if(input->isDown() || input->isLeftStickDown())
        {
            ++selection;
            if(selection >= MAX_GAMES)
                selection = 0;
            genPreview(selection);
            input->resetKeys();
        }
        else if(input->isUp() || input->isLeftStickUp())
        {
            --selection;
            if(selection < 0)
                selection = MAX_GAMES-1;
            genPreview(selection);
            input->resetKeys();
        }
        if(input->isLeft() || input->isLeftStickLeft())
        {
            if(variables[2].getInt() > 1)
                variables[2].setInt(variables[2].getInt()-1);
            genPreview(selection);
            input->resetKeys();
        }
        else if(input->isRight() || input->isLeftStickRight())
        {
            variables[2].setInt(variables[2].getInt()+1);
            genPreview(selection);
            input->resetKeys();
        }
        if(input->isA() || input->isStart() || input->isTouch())
        {
            variables[3].setInt(selection+1);
            gotoNext = true;
        }
        else if (input->isB())
        {
            variables[3].setInt(-1);
            gotoNext = true;
        }
    }
}

void StateMain::update()
{
    if(timer.getScaledTicks() > 2)
    {
        if(playFail)
            playFail = false;
        if(playWin)
            playWin = false;
    }
    variables[5].setInt(selection);
    if(gotoNext && !playFail && !playWin)
    {
        if(variables[6].getInt() == 1)
            variables[6].setInt(0);
        input->resetKeys();
        setNextState(variables[3].getInt()+3);
    }
}

#ifdef PENJIN_SDL
void StateMain::render(SDL_Surface* screen)
{
    /// Clear Screen
    SDL_BlitSurface(bgBuffer,NULL,screen,NULL);
    text.setPosition(15,20);
    if(!gotoNext)
    {
        text.print(screen,(string)MINIGAMES_NAMES[selection]);
        text.print(screen," \n");text.print(screen,"Level: ");text.print(screen,variables[2].getInt());
        prompt.render();
    }
    else
        text.print(screen, "Loading...");

    if((playFail || playWin) && variables[1].getInt() != 2)
    {
        bar.render(screen);
        Vector2di pos(bar.getX(),bar.getY()+3);
        text.setPosition(pos.x+40,pos.y);
        text.print(screen, "Stage: ");text.print(screen, variables[2].getInt());
        text.setPosition(pos.x+230,pos.y);
        text.print(screen, "Lives: ");text.print(screen, variables[4].getInt()+1);

        SDL_BlitSurface(preview,NULL,screen,NULL);

        Vector2di lifeStart(245,130);
        /// Render alive lives

        int lives = variables[4].getInt();
        for(int i=0; i<=lives; i++)
        {
            life.setPosition(lifeStart + Vector2di(i*96,0));
            life.render(screen);
        }
        /// Render lost lives
        for(int i=lives+1; i<=3; i++) //TODO: DRY constant for number of lives
        {
            lostLife.setPosition(lifeStart + Vector2di(i*96,0));
            lostLife.render(screen);
        }
    }

    if(playFail || playWin)
        loseWin.render(screen);
}
#else
void StateMain::render()
{
    /// Clear Screen
    //SDL_BlitSurface(bgBuffer,NULL,screen,NULL);
    text.setPosition(15,20);
    if(!gotoNext)
    {
        text.print((string)MINIGAMES_NAMES[selection]);
        text.print(" \n");text.print("Level: ");text.print(variables[2].getInt());
    }
    else
        text.print("Loading...");

    if((playFail || playWin) && variables[1].getInt() != 2)
    {
        bar.render();
        Vector2di pos(bar.getX(),bar.getY()+3);
        text.setPosition(pos.x+40,pos.y);
        text.print("Stage: ");text.print(variables[2].getInt());
        text.setPosition(pos.x+230,pos.y);
        text.print("Lives: ");text.print(variables[4].getInt()+1);

        //SDL_BlitSurface(preview,NULL,screen,NULL);

        Vector2di lifeStart(245,130);
        /// Render alive lives

        int lives = variables[4].getInt();
        for(int i=0; i<=lives; i++)
        {
            life.setPosition(lifeStart + Vector2di(i*96,0));
            life.render();
        }
        /// Render lost lives
        for(int i=lives+1; i<=3; i++) //TODO: DRY constant for number of lives
        {
            lostLife.setPosition(lifeStart + Vector2di(i*96,0));
            lostLife.render();
        }
    }

    if(playFail || playWin)
        loseWin.render();
}
#endif
