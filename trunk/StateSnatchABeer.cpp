#include "StateSnatchABeer.h"
StateSnatchABeer::StateSnatchABeer()
{
    //
}

StateSnatchABeer::~StateSnatchABeer()
{
    //
}

void StateSnatchABeer::init()
{
    enemyIdle.loadFrames("images/SnatchABeer/enemyidle.png",1,1);
    enemyIdle.setCurrentFrame(0);
    enemyIdle.setLooping(true);
    enemyIdle.setFrameRate(DECI_SECONDS);
    enemyIdle.setPosition(256,9);

    enemy = &enemyIdle;

    playerIdle.loadFrames("images/SnatchABeer/playeridle.png",1,1);
    playerIdle.setCurrentFrame(0);
    playerIdle.setLooping(true);
    playerIdle.setFrameRate(DECI_SECONDS);
    playerIdle.setPosition(202,63);

    player = &playerIdle;

    bar.loadSprite("images/SnatchABeer/bar.png");
    bar.setPosition(-2,304);

    beer.loadSprite("images/SnatchABeer/beer.png");
    beer.setPosition(-70,238);

    action1.loadSprite("images/SnatchABeer/action.png");
    action1.setPosition(278,62);
    action1.setScale(0);

    action2.loadSprite("images/SnatchABeer/action2.png");
    action2.setPosition(558,-16);
    action2.setScale(0);

    background.loadBackground("images/SnatchABeer/bg.png");
    buttonSheet.loadFrames("images/ButtonPrompter/ButtonsSheet.png",10,2);
    pauseText.loadFont("font/bip.ttf", 32);
    pauseText.setColour(WHITE);

        //time
    counter.setMode(SECONDS);
        //bounds
    SDL_Rect bounds;
    bounds.x = bounds.y = 0;
    bounds.w = 800;
    bounds.h = 480;
        //vars
    started = false;
    animationStarted = false;
    playOnce = false;
    secondChance = false;
    delay = rand()%5 + 3;
    status = IDLE;
    animFrame = 0;
    angle = 0.0;
    gravity = 0;
        //win zones - added for easier difficulty tweaking
    winZones[0][0] = 225;
    winZones[0][1] = 265;
    winZones[1][0] = 640;
    winZones[1][1] = 690;
    winZones[2][0] = 320;
    winZones[2][1] = 370;


    // The following are only loaded when the game is not viewed in selection box
    if(variables.size()<SUBSTATE_TRIGGER) {

    barmusic.loadMusic("music/SnatchABeer/bgmusic.ogg");
    barmusic.setLooping(true);
    barmusic.play();
        //sound
    sGlassSlide.loadSound("sounds/SnatchABeer/glass_slide.ogg");
    sGlassBreak.loadSound("sounds/SnatchABeer/glass_break.ogg");
    sPlayer[0].loadSound("sounds/SnatchABeer/nice.ogg");
    sPlayer[1].loadSound("sounds/SnatchABeer/alright.ogg");
    sPlayer[2].loadSound("sounds/SnatchABeer/yeah.ogg");
    sPlayer[3].loadSound("sounds/SnatchABeer/chuckle.ogg");
    sEnemyPunch.loadSound("sounds/SnatchABeer/punch.ogg");
    sEnemyLaugh.loadSound("sounds/SnatchABeer/laugh.ogg");
    sEnemyWhat.loadSound("sounds/SnatchABeer/what.ogg");

        //sprites
    enemyAngry.loadFrames("images/SnatchABeer/enemyangry.png",3,1);
    enemyAngry.setCurrentFrame(0);
    enemyAngry.setLooping(true);
    enemyAngry.setFrameRate(DECI_SECONDS);
    enemyAngry.setPosition(256,9);

    enemyWin.loadFrames("images/SnatchABeer/enemywin.png",4,1);
    enemyWin.setCurrentFrame(0);
    enemyWin.setLooping(true);
    enemyWin.setFrameRate(DECI_SECONDS);
    enemyWin.setPosition(256,9);

    enemyPunch.loadFrames("images/SnatchABeer/enemypunch.png",4,1);
    enemyPunch.setCurrentFrame(0);
    enemyPunch.setLooping(false);
    enemyPunch.setFrameRate(DECI_SECONDS);
    enemyPunch.setPosition(256,9);

    enemyLaugh.loadFrames("images/SnatchABeer/enemylaughing.png",4,1);
    enemyLaugh.setCurrentFrame(0);
    enemyLaugh.setLooping(true);
    enemyLaugh.setFrameRate(DECI_SECONDS);
    enemyLaugh.setPosition(256,9);

    enemyWhat.loadFrames("images/SnatchABeer/enemywhat.png",4,1);
    enemyWhat.setCurrentFrame(0);
    enemyWhat.setLooping(true);
    enemyWhat.setFrameRate(DECI_SECONDS);
    enemyWhat.setPosition(256,9);

    playerWin.loadFrames("images/SnatchABeer/playerwin.png",1,1);
    playerWin.setCurrentFrame(0);
    playerWin.setLooping(true);
    playerWin.setFrameRate(DECI_SECONDS);
    playerWin.setPosition(202,63);

    playerWin2.loadFrames("images/SnatchABeer/playerwin2.png",1,1);
    playerWin2.setCurrentFrame(0);
    playerWin2.setLooping(true);
    playerWin2.setFrameRate(DECI_SECONDS);
    playerWin2.setPosition(618,150);

    playerWin3.loadFrames("images/SnatchABeer/playerwin2_2.png",1,1);
    playerWin3.setCurrentFrame(0);
    playerWin3.setLooping(true);
    playerWin3.setFrameRate(DECI_SECONDS);
    playerWin3.setPosition(618,150);

    playerLose.loadFrames("images/SnatchABeer/playerlose.png",2,1);
    playerLose.setCurrentFrame(0);
    playerLose.setLooping(true);
    playerLose.setFrameRate(DECI_SECONDS);
    playerLose.setPosition(202,63);

    playerLose2.loadFrames("images/SnatchABeer/playerlose2.png",1,1);
    playerLose2.setCurrentFrame(0);
    playerLose2.setLooping(true);
    playerLose2.setFrameRate(DECI_SECONDS);
    playerLose2.setPosition(618,150);

    playerSnatch.loadFrames("images/SnatchABeer/playersnatch.png",1,1);
    playerSnatch.setCurrentFrame(0);
    playerSnatch.setLooping(true);
    playerSnatch.setFrameRate(DECI_SECONDS);
    playerSnatch.setPosition(202,63);

    playerHit.loadFrames("images/SnatchABeer/playerhit.png",4,1);
    playerHit.setCurrentFrame(0);
    playerHit.setLooping(true);
    playerHit.setFrameRate(DECI_SECONDS);
    playerHit.setPosition(202,63);

    //GO!
    counter.start();

    } // sel box check
}

void StateSnatchABeer::userInput()
{

    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif

    // A-Key is pressed
    if(input->isA() && started) {
        if (status == MISSCLOSE) {
            if (secondChance) {
                animFrame = 0;
                status = WIN2;
                playOnce = true;
                input->resetKeys();
            } else {
                animFrame = 0;
                status = MISS2;
                input->resetKeys();
            }
        } else if (status == IDLE) {
            if (beer.getX() > winZones[0][0] && beer.getX() < winZones[0][1]) {
                animFrame = 0;
                status = WIN;
                playOnce = true;
                input->resetKeys();
            } else if ((beer.getX() > winZones[0][0]-20 && beer.getX() <= winZones[0][0]) || (beer.getX() >= winZones[0][1] && beer.getX() < winZones[0][1]+20)) {
                animFrame = 0;
                status = MISSCLOSE;
                input->resetKeys();
            } else {
                animFrame = 0;
                status = MISS;
                input->resetKeys();
            }
        }
    }

    if(input->isStart())//I wasnt sure what exit key was... - me neither
    {
        pauseToggle();
        input->resetKeys();
    }
}

#ifdef PENJIN_SDL
void StateSnatchABeer::render(SDL_Surface *screen)
{
	// Render things
	background.render(screen);
    enemy->render(screen);
    player->render(screen);
    beer.render(screen);
    bar.render(screen);
    action1.render(screen);
    action2.render(screen);
}

void StateSnatchABeer::pauseScreen(SDL_Surface* screen)
{
    // Pause screen
    if(variables.size()<SUBSTATE_TRIGGER)
        pauseSymbol(screen);
    pauseText.setPosition(50,180);
    pauseText.print(screen, "Press     at the right time to get the beer!");
    buttonSheet.setCurrentFrame(10);
    buttonSheet.setPosition(130,180);
    buttonSheet.render(screen);
    pauseText.setPosition(50,220);
    pauseText.print(screen, "Hint: You might get a 2nd chance");
    pauseText.setPosition(50,260);
    pauseText.print(screen, "if you miss the first time.");
}
#else
void StateSnatchABeer::render()
{
	// Render things
	background.render();
    enemy->render();
    player->render();
    beer.render();
    bar.render();
    action1.render();
    action2.render();
}

void StateSnatchABeer::pauseScreen()
{
    // Pause screen
    pauseSymbol();
    pauseText.setPosition(50,180);
    pauseText.print("Press     at the right time to get the beer!");
    buttonSheet.setCurrentFrame(10);
    buttonSheet.setPosition(130,180);
    buttonSheet.render();
    pauseText.setPosition(50,220);
    pauseText.print("Hint: You might get a 2nd chance");
    pauseText.setPosition(50,260);
    pauseText.print("if you miss the first time.");
}
#endif

void StateSnatchABeer::onPause()
{
    counter.pause();
}

void StateSnatchABeer::onResume()
{
    counter.unpause();
}

void StateSnatchABeer::update()
{
    player->update();
    enemy->update();

    // start beer
    if(counter.getScaledTicks() >= delay && !started) {
        started = true;
        sGlassSlide.play();
    }

    int temprandom;

    // move beer
    if (started){
        beer.setX(beer.getX() + 3 + min(variables[2].getInt(),27));
        if (beer.getX() > 600) {
            if (angle > -90) {
                angle -= 6;
            }
            gravity += 3;
            beer.setRotation(angle);
            beer.setY(beer.getY() + gravity);
        }
    }
    if (beer.getX() > 420 && status == IDLE) {
        animFrame = 0;
        status = NOTRY;
        playOnce = true;
    }
    if (beer.getX() > winZones[1][0] || beer.getY() > winZones[2][0]) {
        if (beer.getX() < winZones[1][1] || beer.getY() < winZones[2][1]) {
            secondChance = true;
        } else {
            secondChance = false;
        }
    }

    if (beer.getX() > 800 || beer.getY() > 480) {
        if (!playOnce) {
            sGlassBreak.play();
            playOnce = true;
        }
        //-------------------------
        if (status == MISSCLOSE) {
            animFrame = 0;
            status = NOTRY2;
        }
    }

    // Action notifier
    if (beer.getX() >= winZones[0][0] && beer.getX() <= winZones[0][1] && status == IDLE) {
        action1.setScale(min((winZones[0][1] - beer.getX()) / 20.0,1.0));
    } else if (secondChance && status == MISSCLOSE) {
        action2.setScale(min(min(max((winZones[1][1] - beer.getX()) / 30.0,0.0), max((winZones[2][1] - beer.getY()) / 30.0,0.0)),1.0));
    } else {
        action1.setScale(0);
        action2.setScale(0);
    }

    // Animation stuff - and (NEW) sounds
    if (status == WIN) {
        if (animFrame == 0) {
            player = &playerWin;
            enemy = &enemyAngry;
            beer.clear();
        }
        if (animFrame == 10) {
            temprandom = rand()%7;
           if (temprandom < 4) {
                sPlayer[temprandom].play();
            }
        }
        if (animFrame > 40) {
            variables[0].setInt(1);
            setNextState(STATE_MAIN);
        }
    } else if (status == WIN2) {
        if (animFrame == 0) {
            player = &playerWin2;
            enemy = &enemyAngry;
            beer.clear();
        }
        if (animFrame == 10) {
            temprandom = rand()%7;
            if (temprandom < 4) {
                sPlayer[temprandom].play();
            }
        }
        if (animFrame == 20 && rand()%20 == 0) {
            player = &playerWin3;
        }
        if (animFrame > 40) {
            variables[0].setInt(1);
            setNextState(STATE_MAIN);
        }
    } else if (status == NOTRY) {
        if (animFrame == 0) {
            player = &playerLose;
            enemy = &enemyWin;
            beer.clear();
        }
        if (animFrame > 40) {
            variables[0].setInt(0);
            setNextState(STATE_MAIN);
        }
    } else if (status == MISS) {
        if (animFrame == 0) {
            player = &playerSnatch;
            enemy = &enemyPunch;
        }
        if (enemy->getCurrentFrame() == 3 && !animationStarted) {
            player = &playerHit;
            animationStarted = true;
            sEnemyPunch.play();
        }
        if (animFrame == 20) {
            enemy = &enemyLaugh;
            sEnemyLaugh.play();
        }
        if (animFrame > 70) {
            variables[0].setInt(0);
            setNextState(STATE_MAIN);
        }
    } else if (status == MISS2) {
        if (animFrame == 0) {
            player = &playerLose2;
            enemy = &enemyLaugh;
            sEnemyLaugh.play();
        }
        if (animFrame > 40) {
            variables[0].setInt(0);
            setNextState(STATE_MAIN);
        }
    } else if (status == NOTRY2) {
        if (animFrame == 10) {
            player = &playerLose;
            enemy = &enemyAngry;
        }
        if (animFrame > 40) {
            variables[0].setInt(0);
            setNextState(STATE_MAIN);
        }
    } else if (status == MISSCLOSE) {
        if (animFrame == 0) {
            player = &playerSnatch;
            enemy = &enemyWhat;
            sEnemyWhat.play();
        }
        if (animFrame == 10) {
            player = &playerIdle;
        }
    }
    if (status != IDLE) {
        animFrame++;
    }
}

void StateSnatchABeer::pauseInput()
{
    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif
    if (input->isStart())
    {
        pauseToggle();
        input->resetKeys();
    }
}
