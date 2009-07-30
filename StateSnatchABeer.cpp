#include "StateSnatchABeer.h"
StateSnatchABeer::StateSnatchABeer()
{
    nullify = false;
}

StateSnatchABeer::~StateSnatchABeer()
{

}

void StateSnatchABeer::init()
{
    // Check if game is started and not viewed in the level selection then play music
    if(variables.size()<SUBSTATE_TRIGGER) {
        barmusic.loadMusic("music/SnatchABeer/bgmusic.ogg");
        barmusic.setLooping(true);
        barmusic.play();
    }

    // Initialize variables
        //sprites
    enemy.loadFrames("images/SnatchABeer/enemyidle.png",1,1);
    enemy.setCurrentFrame(0);
    enemy.setLooping(true);
    enemy.setFrameRate(DECI_SECONDS);
    enemy.setPosition(256,9);
    player.loadFrames("images/SnatchABeer/playeridle.png",1,1);
    player.setCurrentFrame(0);
    player.setLooping(true);
    player.setFrameRate(DECI_SECONDS);
    player.setPosition(202,63);
    bar.loadSprite("images/SnatchABeer/bar.png");
    bar.setPosition(-2,304);
    beer.loadSprite("images/SnatchABeer/beer.png");
    beer.setPosition(-70,238);
    action.loadSprite("images/SnatchABeer/action.png");
    action.setPosition(278,62);
    action.setScale(0);
    background.loadBackground("images/SnatchABeer/bg.png");
    pauseText.loadFont("font/bip.ttf", 32);

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

    //GO!
    counter.start();
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
        isPaused = !isPaused;
        input->resetKeys();
    }
}

#ifdef PENJIN_SDL
void StateSnatchABeer::render(SDL_Surface *screen)
{
	// Render things
	background.render(screen);
    enemy.render(screen);
    player.render(screen);
    beer.render(screen);
    bar.render(screen);
    action.render(screen);
}

void StateSnatchABeer::pauseScreen(SDL_Surface* screen)
{
    // Pause screen
    pauseSymbol(screen);
    pauseText.setPosition(50,180);
    pauseText.setColour(WHITE);
    pauseText.print(screen, "Press the 'A'-key at the right time to get the beer!");
}
#else
void StateSnatchABeer::render()
{
	// Render things
	background.render();
    enemy.render();
    player.render();
    beer.render();
    bar.render();
    action.render();
}

void StateSnatchABeer::pauseScreen()
{
    // Pause screen
    pauseSymbol();
    pauseText.setPosition(50,180);
    pauseText.setColour(WHITE);
    pauseText.print("Press the 'A'-key at the right time to get the beer!");
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
    player.update();
    enemy.update();

    // start beer
    if(counter.getScaledTicks() >= delay && !started) {
        started = true;
        sGlass.loadSound("sounds/SnatchABeer/glass_slide.wav");
        sGlass.play();
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
        //-------------------------
        // wee, working!, well kind of
        // if I use sGlass instead of sPlayer which would be much more logic
        // the enemy laughing sound (played by sEnemy) stops playing
        // EXPLAIN THIS EINSTEIN!
        // no, seriously, this is wierd
        // but I don't care it is working this way...
        if (!playOnce) {
            sPlayer.loadSound("sounds/SnatchABeer/glass_break.wav");
            sPlayer.play();
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
        action.setScale(min((winZones[0][1] - beer.getX()) / 20.0,1.0));
    } else if (secondChance && status == MISSCLOSE) {
        action.setScale(min(min(max((winZones[1][1] - beer.getX()) / 30.0,0.0), max((winZones[2][1] - beer.getY()) / 30.0,0.0)),1.0));
    } else {
        action.setScale(0);
    }

    // Animation stuff - and (NEW) sounds
    if (status == WIN) {
        if (animFrame == 0) {
            player.clearFrames();
            player.loadFrames("images/SnatchABeer/playerwin.png",1,1);
            player.setCurrentFrame(0);
            player.setLooping(true);
            enemy.clearFrames();
            enemy.loadFrames("images/SnatchABeer/enemyangry.png",3,1);
            enemy.setCurrentFrame(0);
            enemy.setLooping(true);
            beer.clear();
        }
        if (animFrame == 10) {
            temprandom = rand()%7;
           if (temprandom < 4) {
                if (temprandom == 0) {
                    sPlayer.loadSound("sounds/SnatchABeer/nice.wav");
                } else if (temprandom == 1) {
                    sPlayer.loadSound("sounds/SnatchABeer/alright.wav");
                } else if (temprandom == 2){
                    sPlayer.loadSound("sounds/SnatchABeer/yeah.wav");
                } else {
                    sPlayer.loadSound("sounds/SnatchABeer/chuckle.wav");
                }
                sPlayer.play();
            }
        }
        if (animFrame > 40) {
            variables[0].setInt(1);
            setNextState(STATE_MAIN);
        }
    } else if (status == WIN2) {
        if (animFrame == 0) {
            player.clearFrames();
            player.loadFrames("images/SnatchABeer/playerwin2.png",1,1);
            player.setCurrentFrame(0);
            player.setLooping(true);
            player.setPosition(618,150);
            enemy.clearFrames();
            enemy.loadFrames("images/SnatchABeer/enemyangry.png",3,1);
            enemy.setCurrentFrame(0);
            enemy.setLooping(true);
            beer.clear();
        }
        if (animFrame == 10) {
            temprandom = rand()%7;
            if (temprandom < 4) {
                if (temprandom == 0) {
                    sPlayer.loadSound("sounds/SnatchABeer/nice.wav");
                } else if (temprandom == 1) {
                    sPlayer.loadSound("sounds/SnatchABeer/alright.wav");
                } else if (temprandom == 2){
                    sPlayer.loadSound("sounds/SnatchABeer/yeah.wav");
                } else {
                    sPlayer.loadSound("sounds/SnatchABeer/knuckle.wav");
                }
                sPlayer.play();
            }
        }
        if (animFrame == 20 && rand()%20 == 0) {
            player.clearFrames();
            player.loadFrames("images/SnatchABeer/playerwin2_2.png",1,1);
            player.setCurrentFrame(0);
            player.setLooping(true);
        }
        if (animFrame > 40) {
            variables[0].setInt(1);
            setNextState(STATE_MAIN);
        }
    } else if (status == NOTRY) {
        if (animFrame == 0) {
            player.clearFrames();
            player.loadFrames("images/SnatchABeer/playerlose.png",2,1);
            player.setCurrentFrame(0);
            player.setLooping(true);
            enemy.clearFrames();
            enemy.loadFrames("images/SnatchABeer/enemywin.png",4,1);
            enemy.setCurrentFrame(0);
            enemy.setLooping(true);
            beer.clear();
        }
        if (animFrame > 40) {
            variables[0].setInt(0);
            setNextState(STATE_MAIN);
        }
    } else if (status == MISS) {
        if (animFrame == 0) {
            player.clearFrames();
            player.loadFrames("images/SnatchABeer/playersnatch.png",1,1);
            player.setCurrentFrame(0);
            player.setLooping(true);
            enemy.clearFrames();
            enemy.loadFrames("images/SnatchABeer/enemypunch.png",4,1);
            enemy.setCurrentFrame(0);
            enemy.setLooping(false);
        }
        if (enemy.getCurrentFrame() == 3 && !animationStarted) {
            player.clearFrames();
            player.loadFrames("images/SnatchABeer/playerhit.png",4,1);
            player.setCurrentFrame(0);
            player.setLooping(true);
            animationStarted = true;
            sEnemy.loadSound("sounds/SnatchABeer/punch.wav");
            sEnemy.play();
        }
        if (animFrame == 20) {
            enemy.clearFrames();
            enemy.loadFrames("images/SnatchABeer/enemylaughing.png",4,1);
            enemy.setCurrentFrame(0);
            enemy.setLooping(true);
            sEnemy.loadSound("sounds/SnatchABeer/laugh.wav");
            sEnemy.play();
        }
        if (animFrame > 70) {
            variables[0].setInt(0);
            setNextState(STATE_MAIN);
        }
    } else if (status == MISS2) {
        if (animFrame == 0) {
            player.clearFrames();
            player.loadFrames("images/SnatchABeer/playerlose2.png",1,1);
            player.setCurrentFrame(0);
            player.setLooping(true);
            player.setPosition(618,150);
            enemy.clearFrames();
            enemy.loadFrames("images/SnatchABeer/enemylaughing.png",4,1);
            enemy.setCurrentFrame(0);
            enemy.setLooping(true);
            sEnemy.loadSound("sounds/SnatchABeer/laugh.wav");
            sEnemy.play();
        }
        if (animFrame > 40) {
            variables[0].setInt(0);
            setNextState(STATE_MAIN);
        }
    } else if (status == NOTRY2) {
        if (animFrame == 10) {
            player.clearFrames();
            player.loadFrames("images/SnatchABeer/playerlose.png",2,1);
            player.setCurrentFrame(0);
            player.setLooping(true);
            enemy.clearFrames();
            enemy.loadFrames("images/SnatchABeer/enemyangry.png",3,1);
            enemy.setCurrentFrame(0);
            enemy.setLooping(true);
        }
        if (animFrame > 40) {
            variables[0].setInt(0);
            setNextState(STATE_MAIN);
        }
    } else if (status == MISSCLOSE) {
        if (animFrame == 0) {
            player.clearFrames();
            player.loadFrames("images/SnatchABeer/playersnatch.png",1,1);
            player.setCurrentFrame(0);
            player.setLooping(true);
            enemy.clearFrames();
            enemy.loadFrames("images/SnatchABeer/enemywhat.png",4,1);
            enemy.setCurrentFrame(0);
            enemy.setLooping(true);
            sEnemy.loadSound("sounds/SnatchABeer/what.wav");
            sEnemy.play();
            action.clear();
            action.loadSprite("images/SnatchABeer/action2.png");
            action.setPosition(558,-16);
            action.setScale(0);
        }
        if (animFrame == 10) {
            player.clearFrames();
            player.loadFrames("images/SnatchABeer/playeridle.png",1,1);
            player.setCurrentFrame(0);
            player.setLooping(true);
        }
    }
    if (status != IDLE) {
        animFrame++;
    }
}
