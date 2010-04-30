#include "StateSpellingContest.h"
StateSpellingContest::StateSpellingContest()
{
    nullify = false;

    doc.load("scripts/SpellingContest/words.txt");  //didn't know where else to put it

    text.loadFont("font/bip.ttf", 48);
    text.setColour(Colour(BLUE));
    text.setRelativity(true);

    counter.setMode(SECONDS);

    background.loadBackground("images/SpellingContest/background.png");
    boy.loadFrames("images/SpellingContest/boy_stand.png",2,1);
    girl.loadFrames("images/SpellingContest/girl_stand.png",2,1);
    boy.setFrameRate(SECONDS); //one frame per second
    girl.setFrameRate(SECONDS); //this doesn't actually work - Penjin just defaults too scaler = 1000;
    //it would be nice if AnimatedSprite used Timer::setScaler(float) so that you could use any framerate
    pauseText.loadFont("font/bip.ttf", 32);
    pauseText.setColour(WHITE);
}


StateSpellingContest::~StateSpellingContest()
{

}

void StateSpellingContest::init()
{
    SDL_Rect bounds;
    bounds.x = bounds.y = 0;
    bounds.w = GFX::getXResolution();
    bounds.h = GFX::getYResolution();
    text.setBoundaries(bounds);

    boy.setPosition(350,200);
    girl.setPosition(550,200);

    uint words;
    for(int i = doc.size()-1; i>= 0; --i)
        doc.editLine(i, StringUtility::winToLin(doc.getLine(i)));
    word = doc.getLine(0);
    words = atoi(word.c_str());
    word = doc.getLine(rand()%words + 1);

    wordLength = word.length() - 1;
    correctChars = 0;
    sTime = 2 + wordLength / 2;

    //pv = SDL_EnableUNICODE(1); //enables unicode and saves the previous state

    status = 0; //show info
    counter.start();
}

void StateSpellingContest::userInput()
{
    if(status==1)
    {
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {

                #ifdef PLATFORM_PC
                    case SDL_QUIT: nullifyState(); break;
                    case SDL_KEYDOWN:
                        if(event.key.keysym.sym == SDLK_RETURN)
                            pauseToggle();
                #elif PLATFORM_PANDORA
                    case SDL_KEYDOWN:
                        if(event.key.keysym.sym == SDLK_LALT)
                            pauseToggle();
                #endif


                    if (((event.key.keysym.sym > 64 && event.key.keysym.sym < 91)
                    ||(event.key.keysym.sym > 96 && event.key.keysym.sym < 123))
                    && status == 1)
                    {
                        if(word[correctChars] == (char)StringUtility::lower(event.key.keysym.sym))
                        {
                            correctChars++;
                        }
                        else
                        {
                            variables[0].setInt(0); //failed
                            status++;
                            text.setColour(Colour(BLACK));
                            boy.clearFrames();
                            girl.clearFrames();
                            boy.loadFrames("images/SpellingContest/boy_loose.png",2,1);
                            girl.loadFrames("images/SpellingContest/girl_win.png",2,1);
                            sound.loadMusic("music/SpellingContest/laughing.mp3");
                            sound.setLooping(false);
                            sound.play();
                            counter.start();
                        }
                        break;
                    }
            }
        }

        if(correctChars == wordLength)
        {
            variables[0].setInt(1); //passed
            status++;
            text.setColour(Colour(YELLOW));
            boy.clearFrames();
            girl.clearFrames();
            boy.loadFrames("images/SpellingContest/boy_win.png",2,1);
            girl.loadFrames("images/SpellingContest/girl_loose.png",2,1);
            sound.loadMusic("music/SpellingContest/applause.mp3");
            sound.setLooping(false);
            sound.play();
            counter.start();
        }
    }
}

#ifdef PENJIN_SDL
void StateSpellingContest::renderInfo(SDL_Surface *screen)
{
    text.setPosition(100,200);
    text.print(screen,"spell:");
}

void StateSpellingContest::renderGame(SDL_Surface *screen)
{
    text.setColour(Colour(BLUE));
    text.setPosition(10,10);
	text.print(screen,"time: ");
	//text.setPosition(100,0);
	text.print(screen,(int)sTime - counter.getScaledTicks());
    if (correctChars != wordLength)
    {
        text.setPosition(100,200);
        text.setColour(Colour(YELLOW));
        text.print(screen,word.substr(0,correctChars));
        text.setColour(Colour(WHITE));
        text.print(screen,word.substr(correctChars,wordLength-correctChars));
    }
}

void StateSpellingContest::renderEnd(SDL_Surface *screen)
{
    text.setPosition(100,200);

    if(correctChars == wordLength)
        text.print(screen,"Winner");
    else
        text.print(screen,"Loser");
}

void StateSpellingContest::render(SDL_Surface *screen)
{
	background.render(screen);

	boy.render(screen);
	girl.render(screen);

    switch(status)
    {
        case 0: renderInfo(screen); break;
        case 1: renderGame(screen); break;
        case 2: renderEnd(screen); break;
    }
}

void StateSpellingContest::pauseScreen(SDL_Surface* screen)
{
    // Pause screen
    if(!isSubState())
        pauseSymbol(screen);
    pauseText.setPosition(50,320);
    pauseText.print(screen, "Type the word in the time limit!");
    pauseText.setPosition(50,360);
    pauseText.print(screen, "Don't press the wrong key!");
    buttonsOverlay(screen);
}
#else
    void StateSpellingContest::renderInfo()
    {

    }
    void StateSpellingContest::renderGame()
    {
        text.setColour(Colour(BLUE));
        text.setPosition(10,10);
        text.print("time: ");
        //text.setPosition(100,0);
        text.print((int)sTime - counter.getScaledTicks());
        if (correctChars != wordLength)
        {
            text.setPosition(100,200);
            text.setColour(Colour(YELLOW));
            text.print(word.substr(0,correctChars));
            text.setColour(Colour(WHITE));
            text.print(word.substr(correctChars,wordLength-correctChars));
        }
    }
    void StateSpellingContest::renderEnd()
    {
        text.setPosition(100,200);

        if(correctChars == wordLength)
            text.print("Winner");
        else
            text.print("Loser");
    }
    void StateSpellingContest::render()
    {
        background.render();

        boy.render();
        girl.render();

        switch(status)
        {
            case 0: renderInfo(); break;
            case 1: renderGame(); break;
            case 2: renderEnd(); break;

        }
    }
#endif

void StateSpellingContest::onPause()
{
    counter.pause();
}

void StateSpellingContest::onResume()
{
    counter.unpause();
}

void StateSpellingContest::updateInfo()
{
    if(counter.getScaledTicks() > 0) //wait 1 second
    {
        status++;
        counter.start();
    }
}

void StateSpellingContest::updateGame()
{
    if((uint)counter.getScaledTicks() > sTime - 1 && status == 1) //if times up
    {
        variables[0].setInt(0); //failed
        status++;
        text.setColour(Colour(BLACK));
        boy.clearFrames();
        girl.clearFrames();
        boy.loadFrames("images/SpellingContest/boy_loose.png",2,1);
        girl.loadFrames("images/SpellingContest/girl_win.png",2,1);
        sound.loadMusic("music/SpellingContest/laughing.mp3");
        sound.setLooping(false);
        sound.play();
        counter.start();
    }
}

void StateSpellingContest::updateEnd()
{
    if(counter.getScaledTicks() > 2) //wait 2 seconds
    {
        if(sound.isPlaying())
            sound.stop();
        //SDL_EnableUNICODE(pv); //restore previous configuration
        setNextState(STATE_MAIN);
    }
}

void StateSpellingContest::update()
{
    switch(status)
    {
        case 0: updateInfo(); break;
        case 1: updateGame(); break;
        case 2: updateEnd(); break;
    }

    boy.update();
    girl.update();
}
