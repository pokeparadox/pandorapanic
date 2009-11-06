#include "StateCredits.h"

StateCredits::StateCredits()
{

}

StateCredits::~StateCredits(){}


void StateCredits::init()
{
	//	We will use this to progress the current credit line.
	currentLine = 0;

	//	Setup timer for credits scrolling
    timer.start();
	timer.setMode(MILLI_SECONDS);

	//	Setup randomNumbers for colours
	Random::setLimits(0,255);


	//	Load music
	string dir = "music/";
	music.loadMusic(dir+"credits.ogg");
	music.setLooping(false);
	music.play();

	//	Load sprites
	dir = "images/";
	string ext = ".png";
	logo.loadSprite(dir+"menu/pp_logo_small"+ext);
	//logo.setAlpha(0);
	pand.loadFrames(dir+"pandora"+ext,4,1);
    pand.setTransparentColour(Vector2di(0,0));
    #ifdef PENJIN_SDL
        pand.setUseHardware(true);
    #endif

    SDL_Rect bounds;
    bounds.x = bounds.y = 0;
    bounds.w = getStateXResolution();
    bounds.h = getStateYResolution();
    pandStartPos = Vector2di((bounds.w - 196) *0.5f,bounds.h * 0.1f);
    initPandora(pandStartPos);
    logo.setPosition((bounds.w - logo.getWidth()) *0.5f,bounds.h * 0.66f);

	//	Setup SDL Text Handler
	text.loadFont("font/foo.ttf", 28);
	text.setRelativity(false);	//	We will control all positions of text manually
	text.setBoundaries(bounds);
	text.setCentreText(true);
	text.setPosition(0,bounds.h*0.5f);
	text.setColour(Colour(WHITE));

    emit.resize(3);
    emit[0].setMax(800);
    emit[0].setInvisible(750);
	emit[0].setLooping(true);
	emit[0].setPosition(Vector2df(bounds.w*0.5f,bounds.h-1.0f));
	emit[0].setMaxVelocity(Vector2df(2.3f,5.9f));
	emit[0].setColour(WHITE);
	emit[0].setDeviation(1.2f);
	emit[0].setAcceleration(Vector2df(0.0f,-0.0009f));
	emit[0].setGravity(0.001f);
	emit[0].setBoundaries(bounds);

    emit[1].setMax(500);
    emit[1].setLifeTime(805);
    emit[1].setLooping(false);
	emit[1].setPosition(Vector2df(160,120));
	emit[1].setMaxVelocity(Vector2df(4.5f,4.5f));
	emit[1].setColour(Colour(ORANGE));
	emit[1].shouldFadeColour(true);
	emit[1].setDeviation(2.6f);
	emit[1].setGravity(Vector2df(0.0f,0.001f));
	emit[1].setBoundaries(bounds);

    emit[2].setMax(500);
    emit[2].setLifeTime(800);
	emit[2].setLooping(false);
	emit[2].setPosition(Vector2df(140,90));
	emit[2].setMaxVelocity(Vector2df(4.2f,4.2f));
	emit[2].setColour(Colour(BLUE));
	emit[2].shouldFadeColour(true);
	emit[2].setDeviation(2.5f);
	emit[2].setGravity(Vector2df(0.0f,0.001f));
	emit[2].setBoundaries(bounds);

	input->setDeadZone(Vector2di(4000,4000));
	input->setScaler(0.000001f);
	GFX::setClearColour(BLACK);
}

void StateCredits::userInput()
{
	//	Refresh inputs
	input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif
	//	Skip current Credits item
	if(input->isA() && currentLine < CREDITS_SIZE-1)
	{
		++currentLine;
		input->resetKeys();
	}
	//	skip credits
	if(input->isStart())
	{
		setNextState(STATE_TITLE);
		input->resetKeys();
	}
}

void StateCredits::movePandora(const Vector2di& move)
{
    for(uint i =0; i < 9; ++i)
        pandPos[i] += Vector3df(move.x,move.y,0);
}

void StateCredits::initPandora(const Vector2di& start)
{
    enum LOGO_POS
    {
        TL = 0,
        T,
        TR,
        L,
        C,
        R,
        DL,
        D,
        DR
    };
    pandPos[TL] = Vector3df(start.x, start.y,0);
    for(uint i =0; i < 8; ++i)
    {
        pandPos[i+1] = pandPos[i] + Vector3df(64,0,0);
        if(pandPos[i+1].x > start.x + 128)
        {
            pandPos[i+1].x = start.x;
            pandPos[i+1].y+= 64;
        }
    }
    pandPos[TL] += Vector3df(16,16,-16);
    pandPos[TR] += Vector3df(-48,48,48);
    pandPos[DL] += Vector3df(48,-48,-48);
    pandPos[DR] += Vector3df(-16,-16,16);
    pandPos[T].z = 0;
    pandPos[D].z = 0;
    pandPos[L] = Vector3df(pandPos[C].x,pandPos[C].y,64);
    pandPos[R] = Vector3df(pandPos[C].x,pandPos[C].y,-64);

    pandVel[TL] = Vector3df(1,1,-1);    pandVel[T] = Vector3df(0,1,1);      pandVel[TR] = Vector3df(-1,1,1);
    pandVel[L] = Vector3df(1,0,1);      pandVel[C] = Vector3df(0,0,0);      pandVel[R] = Vector3df(-1,0,-1);
    pandVel[DL] = Vector3df(1,-1,-1);    pandVel[D] = Vector3df(0,-1,-1);    pandVel[DR] = Vector3df(-1,-1,1);
    for(int i = 8; i >=0; --i)
        pandVel[i]*=4.2f;

}

void StateCredits::updatePandora()
{
    for(int i = 8; i >= 0; --i)
    {
        Vector3df t = pandPos[i] - pandPos[4];
        if(abs(t.x) > 64)
            pandVel[i].x = -pandVel[i].x;
        if(abs(t.y) > 64)
            pandVel[i].y = -pandVel[i].y;
        if(abs(t.z) > 64)
            pandVel[i].z = -pandVel[i].z;
        pandPos[i] += pandVel[i];
        #ifdef PENJIN_FIXED
        pandPrio[i] = ((((pandPos[i].z +64)/128.0f)*4)-0.5f).intValue<<16;
        #else
        pandPrio[i] = (((pandPos[i].z +64)/128.0f)*4)-0.5f;
        #endif
    }
}

#ifdef PENJIN_SDL
void StateCredits::renderPandora(SDL_Surface* screen)
{
    for(int i = 8; i >= 0; --i)
    {
        if(pandPrio[i] == 0)
        {
            pand.setPosition(pandPos[i].x, pandPos[i].y);
            pand.setCurrentFrame(0);
            if(i == 1 || i == 3 ||i == 5 ||i == 7)
                pand.render(screen);
        }
    }
    for(int i = 8; i >= 0; --i)
    {
        if(pandPrio[i] == 1)
        {
            pand.setPosition(pandPos[i].x, pandPos[i].y);
            pand.setCurrentFrame(1);
            if(i == 1 || i == 3 ||i == 5 ||i == 7)
                pand.render(screen);
        }
    }
    pand.setPosition(pandPos[4].x, pandPos[4].y);
    pand.setCurrentFrame(pandPrio[4]);
    pand.render(screen);
    for(int i = 8; i >= 0; --i)
    {
        if(pandPrio[i] == 2)
        {
            pand.setPosition(pandPos[i].x, pandPos[i].y);
            pand.setCurrentFrame(2);
            if(i == 1 || i == 3 ||i == 5 ||i == 7)
                pand.render(screen);
        }
    }
    for(int i = 8; i >= 0; --i)
    {
        if(pandPrio[i] == 3)
        {
            pand.setPosition(pandPos[i].x, pandPos[i].y);
            pand.setCurrentFrame(3);
            if(i == 1 || i == 3 ||i == 5 ||i == 7)
                pand.render(screen);
        }
    }
}

void StateCredits::render(SDL_Surface *screen)
{
	//  Render all particles
    for(int e = emit.size()-1; e >= 0; --e)
        emit[e].render(screen);

	string tempString = CREDITS[currentLine];

	if(strstr(tempString.c_str(),"[VERSION]"))
        tempString = "PandoraPanic! V" + (string)AutoVersion::FULLVERSION_STRING;
    logo.render(screen);
    renderPandora(screen);
    text.setPosition(0,getStateYResolution()*0.5f);
	text.print(screen,tempString);
	buff.update();
	buff.setAlpha(240);
	SDL_FillRect(screen,NULL, NULL);
	buff.render();
}
#else
void StateCredits::render()
{
	//  Render all particles
	GFX::clearScreen();
    for(int e = emit.size()-1; e >= 0; --e)
        emit[e].render();

	string tempString = CREDITS[currentLine];

	if(strstr(tempString.c_str(),"[VERSION]"))
        tempString = "PandoraPanic! V" + (string)AutoVersion::FULLVERSION_STRING;
    logo.render();
    renderPandora();
    text.setPosition(0,getStateYResolution()*0.5f);
	text.print(tempString);
	buff.update();
//	buff.setAlpha(240);
	//SDL_FillRect(screen,NULL, NULL);
	buff.render();
}
void StateCredits::renderPandora()
{
    for(int i = 8; i >= 0; --i)
    {
        if(pandPrio[i] == 0)
        {
            pand.setPosition(pandPos[i].x, pandPos[i].y);
            pand.setCurrentFrame(0);
            if(i == 1 || i == 3 ||i == 5 ||i == 7)
                pand.render();
        }
    }
    for(int i = 8; i >= 0; --i)
    {
        if(pandPrio[i] == 1)
        {
            pand.setPosition(pandPos[i].x, pandPos[i].y);
            pand.setCurrentFrame(1);
            if(i == 1 || i == 3 ||i == 5 ||i == 7)
                pand.render();
        }
    }
    pand.setPosition(pandPos[4].x, pandPos[4].y);
    pand.setCurrentFrame(pandPrio[4]);
    pand.render();
    for(int i = 8; i >= 0; --i)
    {
        if(pandPrio[i] == 2)
        {
            pand.setPosition(pandPos[i].x, pandPos[i].y);
            pand.setCurrentFrame(2);
            if(i == 1 || i == 3 ||i == 5 ||i == 7)
                pand.render();
        }
    }
    for(int i = 8; i >= 0; --i)
    {
        if(pandPrio[i] == 3)
        {
            pand.setPosition(pandPos[i].x, pandPos[i].y);
            pand.setCurrentFrame(3);
            if(i == 1 || i == 3 ||i == 5 ||i == 7)
                pand.render();
        }
    }
}
#endif
void StateCredits::unlimitedUpdate()
{
//	camSprite.update();
	if(timer.getScaledTicks() >= 120250/CREDITS_SIZE)
	{
		//	Get the latest credit line from the file by increasing currentLine
		if(currentLine < CREDITS_SIZE-1)
		{
			++currentLine;
			string ext = ".png";
			string path = "images/";

			logo.setAlpha(255);
            if(strstr(CREDITS[currentLine].c_str(), "PandoraPanic"))
            {
                logo.clear();
                logo.loadSprite(path+"menu/pp_logo_small"+ext);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "Penjin"))
            {
                logo.clear();
                logo.loadSprite(path+"penjin"+ext);
            }
			else if(strstr(CREDITS[currentLine].c_str(), "pirate-games") || strstr(CREDITS[currentLine].c_str(), "Pirate Games"))
            {
                logo.clear();
                logo.loadSprite(path+"pirategames"+ext);
            }
			else if(strstr(CREDITS[currentLine].c_str(), "GP32X"))
            {
                logo.clear();
                logo.loadSprite(path+"gp32x.gif");
            }
			else if(strstr(CREDITS[currentLine].c_str(), "GUI"))
            {
                logo.clear();
                logo.loadSprite(path+"pp_start"+ext);
            }
			else if(strstr(CREDITS[currentLine].c_str(), "Losing"))
            {
                logo.clear();
                logo.loadSprite(path+"pp_fail"+ext);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "Winning"))
            {
                logo.clear();
                logo.loadSprite(path+"pp_win"+ext);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "Invaders"))
            {
                logo.clear();
                logo.loadSprite(path+"SpaceInvaders/alien"+ext);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "Spelling"))
            {
                logo.clear();
                logo.loadSprite(path+"SpellingContest/girl_stand"+ext);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "BadVista"))
            {
                logo.clear();
                logo.loadSprite(path+"BadVista/bvLogo"+ext);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "Panic Attack"))
            {
                logo.clear();
                logo.loadSprite(path+"PanicAttack/bigRedX"+ext);
                logo.setTransparentColour(Vector2di(60,30));
            }
            else if(strstr(CREDITS[currentLine].c_str(), "Wrestler"))
            {
                logo.clear();
                logo.loadSprite(path+"MadWrestlerPunch/kick"+ext);
                logo.setTransparentColour(WHITE);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "Pong"))
            {
                logo.clear();
                logo.loadSprite(path+"Pong/ball"+ext);
                logo.setTransparentColour(BLACK);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "Pang"))
            {
                logo.clear();
                logo.loadSprite(path+"Pang/player"+ext);
                logo.setTransparentColour(BLACK);
            }
            /*else if(strstr(CREDITS[currentLine].c_str(), "DoubleNub"))
            {
                logo.clear();
                logo.loadSprite(path+"DoubleNubTrouble/target"+ext);
                logo.setTransparentColour(MAGENTA);
            }*/
            else if(strstr(CREDITS[currentLine].c_str(), "joust - Dragons_Slayer"))
            {
                logo.clear();
                logo.loadSprite(path+"PanJoust/heroframe"+ext);
                logo.setTransparentColour(MAGENTA);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "PanJoust"))
            {
                logo.clear();
                logo.loadSprite(path+"PanJoust/birdrider"+ext);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "Distracted Maths"))
            {
                logo.clear();
                logo.loadSprite(path+"DistractedMaths/4"+ext);
                logo.setTransparentColour(MAGENTA);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "Reflex"))
            {
                logo.clear();
                logo.loadSprite(path+"Reflex/circle"+ext);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "SDL"))
            {
                logo.clear();
                logo.loadSprite(path+"sdl.gif");
            }
            else if(strstr(CREDITS[currentLine].c_str(), "Rocket"))
            {
                logo.clear();
                logo.loadSprite(path+"RocketEscape/rocket"+ext);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "Niqk"))
            {
                logo.clear();
                logo.loadSprite(path+"Jumper/robot"+ext);
                logo.setTransparentColour(MAGENTA);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "Jumper - MarkoeZ"))
            {
                logo.clear();
                logo.loadSprite(path+"Jumper/blobevil"+ext);
                logo.setTransparentColour(MAGENTA);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "Maze"))
            {
                logo.clear();
                logo.loadSprite(path+"Maze/hero"+ext);
                logo.setTransparentColour(MAGENTA);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "Plander - Poke"))
            {
                logo.clear();
                logo.loadSprite(path+"RocketEscape/platform"+ext);
                //logo.setUseHardware(false);
            }
            else if(CREDITS[currentLine].find("Rotating Turret") != string::npos)
            {
                logo.clear();
                logo.loadSprite(path+"RotDef/bomb"+ext);
            }
            else if(CREDITS[currentLine].find("Memory Blocks") != string::npos)
            {
                logo.clear();
                logo.loadSprite(path+"MemoryBlocks/purple_block"+ext);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "PuttPutt"))
            {
                logo.clear();
                logo.loadSprite(path+"PuttPutt/Ball"+ext);
                //logo.setTransparentColour(MAGENTA);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "Birthday"))
            {
                logo.clear();
                logo.loadSprite(path+"BirthdayCake/candles"+ext);
                //logo.setTransparentColour(MAGENTA);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "Arena"))
            {
                logo.clear();
                logo.loadSprite(path+"Arena/Hero/HeroStandingDown"+ext);
                //logo.setTransparentColour(MAGENTA);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "Beer"))
            {
                logo.clear();
                logo.loadSprite(path+"SnatchABeer/beer"+ext);
                //logo.setTransparentColour(MAGENTA);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "Count"))
            {
                logo.clear();
                logo.loadSprite(path+"TheCountJob/circle_g"+ext);
                //logo.setTransparentColour(MAGENTA);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "Fire"))
            {
                logo.clear();
                logo.loadSprite(path+"Fire/fire"+ext);
                //logo.setTransparentColour(MAGENTA);
            }
            else if(strstr(CREDITS[currentLine].c_str(), "Bandit"))
            {
                logo.clear();
                logo.loadSprite(path+"OneButtonBandit/heart"+ext);
                //logo.setTransparentColour(MAGENTA);
            }
			else
                logo.setAlpha(0);
            logo.setPosition((getStateXResolution() - logo.getWidth())*0.5f,getStateYResolution() * 0.66f);
		}
		else
			setNextState(STATE_TITLE);

        //	Check if it is the last credit item and begin to fade the music.
        if(currentLine == CREDITS_SIZE-1)
        {
            music.fade(120250/CREDITS_SIZE);
        }
        timer.start();
	}
}

void StateCredits::update()
{
    updatePandora();
        //  Update all particles
    for(int e = emit.size()-1; e >= 0; --e)
    {
        emit[e].update();
        emit[e].setColour(Colour((uchar)Random::nextInt(),Random::nextInt(),Random::nextInt()));
        if(emit[1].isFinished())
        {
            emit[1].setPosition(Vector2df(Random::nextInt(50,750),Random::nextInt(50,430)));
            emit[1].start();
        }
        if(emit[2].isFinished())
        {
            emit[2].setPosition(Vector2df(Random::nextInt(50,750),Random::nextInt(50,430)));
            emit[2].start();
        }
    }
}
