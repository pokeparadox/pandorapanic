#include "StateRotDef.h"
using LUT::Lsin;
using LUT::Lcos;
using LUT::degreeToBrad;

StateRotDef::StateRotDef()
{
    nullify = false;

    command.loadFont("font/origami.ttf",48);
    command.setRelativity(false);
    command.setPosition(315,216);
    counter.setMode(SECONDS);
    enemy.loadSprite("images/RotDef/bomb.png");
    turret.loadSprite("images/RotDef/turret_top.png");
    turretBase.loadSprite("images/RotDef/turret_base.png");
    shot.loadSprite("images/RotDef/bullet.png");
    explosion.loadFrames("images/RotDef/explosion.png", 5, 1);
    explosion.setFrameRate(DECI_SECONDS);
    explosion.setLooping(false);
    background.loadBackground("images/RotDef/battlefield.png");
    LUT::init();
    buttonSheet.loadFrames("images/ButtonPrompter/ButtonsSheet.png",10,2);
    pauseText.loadFont("font/bip.ttf", 32);
    pauseText.setColour(WHITE);

#if ROTDEF_DEBUG
    debugText.loadFont("font/unispace.ttf", 12);
    debugText.setColour(WHITE);
#endif
}


StateRotDef::~StateRotDef()
{
    LUT::deInit();
}

void StateRotDef::init()
{
    if(variables.size()<SUBSTATE_TRIGGER)
    {
        shotSound.loadSound("sounds/RotDef/FIRE.ogg");
        //rotateSound.loadSound("sounds/RotDef/ROTATE.ogg");
        winSound.loadSound("sounds/RotDef/WIN.ogg");
        loseSound.loadSound("sounds/RotDef/LOSE.ogg");
    }
    turret.setPosition(400 - turret.getWidth()*0.5f,
                       240 - turret.getHeight()*0.5f);
    turretBase.setPosition(turret.getX(), turret.getY());

    shooting = false;
    enemyKilled = false;
    playerKilled = false;
    gameEnd = false;

    uchar enemDir = rand()%256;
    enemyPosition = Vector2df(400 + 359*Lcos(enemDir),240 + 200*Lsin(enemDir));
    Vector2df turretVec = Vector2df(turret.getX() + turret.getWidth()*0.5f,
                                  turret.getY() + turret.getHeight()*0.5f);
    enemyVelocity = (turretVec - enemyPosition);
    enemyVelocity.normalise();
    int levelNumber = variables[2].getInt();
    turnSpeed = 10;
    if(levelNumber>30)
        turnSpeed = 20;
    if(levelNumber>60)
    {
        levelNumber = 61;
    }
    levelNumber+=5;
    enemyVelocity *= (2.0f +  ((turretVec - enemyPosition).length()-240.0f)/120.0f);
    enemyVelocity *= (levelNumber * 0.04f);
    enemy.setPosition(enemyPosition.x, enemyPosition.y);

    turretDirection = rand() % (5*359);

    shotVelocity.x = 0;
    shotVelocity.y = 0;
    shotPosition = shotVelocity;
    counter.start();
}

void StateRotDef::userInput()
{
    float angle;

    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif

    if(input->isLeft())
    {
        turretDirection+=turnSpeed;

        if(turretDirection>(359*5))
            turretDirection = (turretDirection-(359*5));

        //if(!rotateSound.isPlaying())
            //rotateSound.play();
    }
    else if(input->isRight())
    {
        turretDirection-=turnSpeed;

        if(turretDirection<0)
            turretDirection = ((359*5)+turretDirection);

        //if(!rotateSound.isPlaying())
            //rotateSound.play();
    }
    //else if (rotateSound.isPlaying())
        //rotateSound.stop();


    if(input->isA() && !shooting)
    {
        shotSound.play();

        angle = (turretDirection*0.2f) - 90;

        if( angle<0)
             angle = 359 + angle;

        uchar turrDir = degreeToBrad(angle);
        float shotDirX = -Lcos(turrDir);
        float shotDirY = Lsin(turrDir);

        shooting = true;
                shotPosition = Vector2df((turret.getX() + turret.getWidth()*0.5f + turret.getHeight()*0.5f * shotDirX - shot.getWidth()*0.5f),
                                 (turret.getY() + turret.getHeight()*0.5f + turret.getHeight()*0.5f * shotDirY - shot.getHeight()*0.5f));
        shot.setPosition(shotPosition.x, shotPosition.y);
        shot.setRotation(turretDirection*0.2f);
        shotVelocity = Vector2df(-Lcos(turrDir),
                                Lsin(turrDir));
        shotVelocity *= 10.0f;
    }

    if(input->isStart())
    {
        pauseToggle();
        input->resetKeys();
    }
}

void StateRotDef::onPause()
{
    counter.pause();
}

void StateRotDef::onResume()
{
    counter.unpause();
}

#ifdef PENJIN_SDL
void StateRotDef::render(SDL_Surface *screen)
{
	background.render(screen);

    if(!enemyKilled && !playerKilled) enemy.render(screen);
    turretBase.render(screen);
    turret.setRotation(turretDirection*0.2f);
    turret.render(screen);

    if(shooting && !enemyKilled)
    {
        shot.render(screen);
    }

    if(counter.getTicks() <= 500)
    {
        command.print(screen, "Shoot!");
    }

    if((enemyKilled || playerKilled) && !explosion.hasFinished()) explosion.render(screen);
    if(explosion.hasFinished())
    {
        gameEnd = true;
    }

#if ROTDEF_DEBUG
    RenderDebug(screen);
#endif
}

void StateRotDef::pauseScreen(SDL_Surface* screen)
{
    // Pause screen
    if(!isSubState())
        pauseSymbol(screen);
    pauseText.setPosition(220,60);
    pauseText.print(screen, "Rotate the turret using    and");
    buttonSheet.setCurrentFrame(14);
    buttonSheet.setPosition(560,60);
    buttonSheet.render(screen);
    buttonSheet.setCurrentFrame(15);
    buttonSheet.setPosition(666,60);
    buttonSheet.render(screen);
    pauseText.setPosition(220,100);
    pauseText.print(screen, "Press     to shoot the bomb!");
    buttonSheet.setCurrentFrame(10);
    buttonSheet.setPosition(304,100);
    buttonSheet.render(screen);
    buttonsOverlay(screen);
}
#else
void StateRotDef::render()
{
	background.render();

    if(!enemyKilled) enemy.render();
    turretBase.render();
    turret.setRotation(turretDirection/5.0f);
    turret.render();

    if(shooting && !enemyKilled)
    {
        shot.render();
    }

    if(counter.getTicks() <= 500)
    {
        command.print("Shoot!");
    }

    if((enemyKilled || playerKilled) && !explosion.hasFinished()) explosion.render(screen);
    if(explosion.hasFinished())
    {
        gameEnd = true;
    }

#if ROTDEF_DEBUG
    RenderDebug();
#endif
}
#endif

void StateRotDef::update()
{
    if(gameEnd)
    {
        if(!winSound.isPlaying() && !loseSound.isPlaying());
        {
            if(enemyKilled) variables[0].setInt(1);
            else variables[0].setInt(0);
            setNextState(STATE_MAIN);
        }
    }

    if(enemyKilled || playerKilled) explosion.update();

    enemyPosition += enemyVelocity;
    enemy.setPosition(enemyPosition.x, enemyPosition.y);
    if(shooting && !enemyKilled)
    {
        shotPosition += shotVelocity;
        shot.setPosition(shotPosition.x, shotPosition.y);
        if(shot.getX() < 0 || shot.getX() >= 800 ||
           shot.getY() < 0 || shot.getY() >= 480)
        {
            shooting = false;
        }


        if(shot.getX()+(int)shot.getWidth()*0.5f >= enemy.getX() &&
           shot.getX()+(int)shot.getWidth()*0.5f <= enemy.getX()+(int)enemy.getWidth() &&
           shot.getY()+(int)shot.getHeight()*0.5f >= enemy.getY() &&
           shot.getY()+(int)shot.getHeight()*0.5f <= enemy.getY()+(int)enemy.getHeight() )
        {
            if(!enemyKilled)
            {
                if (!winSound.isPlaying())
                    winSound.play();
                explosion.setPosition(enemy.getX() - 84, enemy.getY() - 84);
                enemyKilled = true;
                ACHIEVEMENTS->logEvent("ROTDEF_WIN",round(sqrt(pow(turretBase.getX() + turretBase.getWidth() - enemy.getX(),2)+pow(turretBase.getY() + turretBase.getHeight() - enemy.getY(),2))));
            }
        }

    }

    if(enemy.getX()+(int)enemy.getWidth()*0.5f >= turretBase.getX() &&
       enemy.getX()+(int)enemy.getWidth()*0.5f <= turretBase.getX()+(int)turretBase.getWidth() &&
       enemy.getY()+(int)enemy.getHeight()*0.5f >= turretBase.getY() &&
       enemy.getY()+(int)enemy.getHeight()*0.5f <= turretBase.getY()+(int)turretBase.getHeight() )
    {
        if(!playerKilled)
        {
            if (!loseSound.isPlaying())
                loseSound.play();
            explosion.setPosition(turretBase.getX() - 29, turretBase.getY() - 29);
            playerKilled = true;
        }
    }
}

#if ROTDEF_DEBUG
void StateRotDef::RenderDebug( SDL_Surface* screen )
{
    #define DEBUG_TEXT_X1       10
    #define DEBUG_TEXT_Y1       0

    sprintf( debug_bulletstats, "Turn %f X %.3f Y %.3f Xv %.3f Yv %.3f \n", turretDirection, shotPosition.x, shotPosition.y, shotVelocity.x, shotVelocity.y );
    debugText.setPosition(DEBUG_TEXT_X1,DEBUG_TEXT_Y1);
    debugText.print(screen, debug_bulletstats);
}
#endif
