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
    background.loadBackground("images/RotDef/battlefield.png");
    LUT::init();
    buttonSheet.loadFrames("images/ButtonPrompter/ButtonsSheet.png",10,2);
    pauseText.loadFont("font/bip.ttf", 32);
    pauseText.setColour(WHITE);
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
        rotateSound.loadSound("sounds/RotDef/ROTATE.ogg");
    }
    turret.setPosition(400 - turret.getWidth()*0.5f,
                       240 - turret.getHeight()*0.5f);
    turretBase.setPosition(turret.getX(), turret.getY());

    shooting = false;

    float enemyDirection = rand() % 360;
    uchar enemDir = degreeToBrad(enemyDirection);
    enemyPosition = Vector2df(400 + 360*Lcos(enemDir),240 + 200*Lsin(enemDir));
    Vector2df turretVec = Vector2df(turret.getX() + turret.getWidth()*0.5f,
                                  turret.getY() + turret.getHeight()*0.5f);
    enemyVelocity = (turretVec - enemyPosition);
    enemyVelocity.normalise();
    enemyVelocity *= 2.0f + ((turretVec - enemyPosition).length()-240.0f)/120.0f;
    enemy.setPosition(enemyPosition.x, enemyPosition.y);

    turretDirection = rand() % (5*360);

    counter.start();
}

void StateRotDef::userInput()
{

    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif

    if(input->isLeft())
    {
        ++turretDirection;
        if(!rotateSound.isPlaying())
            rotateSound.play();
    }
    else if(input->isRight())
    {
        --turretDirection;
        if(!rotateSound.isPlaying())
            rotateSound.play();
    }
    else
        rotateSound.stop();


    if(input->isA() && !shooting)
    {
        shotSound.play();
        uchar turrDir = degreeToBrad(turretDirection/5.0 - 90);
        float shotDirX = -Lcos(turrDir);
        float shotDirY = Lsin(turrDir);

        shooting = true;
        shotPosition = Vector2df(static_cast<int> (turret.getX() + turret.getWidth()*0.5f + turret.getHeight()*0.5f * shotDirX - shot.getWidth()*0.5f),
                         static_cast<int> (turret.getY() + turret.getHeight()*0.5f + turret.getHeight()*0.5f * shotDirY - shot.getHeight()*0.5f));
        shot.setPosition(shotPosition.x, shotPosition.y);
        shot.setRotation(turretDirection/5.0);
        shotVelocity = Vector2df(-Lcos(turrDir),
                                Lsin(turrDir));
        shotVelocity *= 10.0f;
    }

    if(input->isStart())
    {
        isPaused=!isPaused;
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

    enemy.render(screen);
    turretBase.render(screen);
    turret.setRotation(turretDirection/5.0);
    turret.render(screen);

    if(shooting)
    {
        shot.render(screen);
    }

    if(counter.getTicks() <= 500)
    {
        command.print(screen, "Shoot!");
    }
}
void StateRotDef::pauseScreen(SDL_Surface* screen)
{
    // Pause screen
    pauseSymbol(screen);
    pauseText.setPosition(220,60);
    pauseText.print(screen, "Rotate the turret using    and");
    buttonSheet.setCurrentFrame(14);
    buttonSheet.setPosition(556,60);
    buttonSheet.render(screen);
    buttonSheet.setCurrentFrame(15);
    buttonSheet.setPosition(648,60);
    buttonSheet.render(screen);
    pauseText.setPosition(220,100);
    pauseText.print(screen, "Press     to shoot the bomb!");
    buttonSheet.setCurrentFrame(10);
    buttonSheet.setPosition(300,100);
    buttonSheet.render(screen);
}
#else
void StateRotDef::render()
{
	background.render();

    enemy.render();
    turretBase.render();
    turret.setRotation(turretDirection/5.0f);
    turret.render();

    if(shooting)
    {
        shot.render();
    }

    if(counter.getTicks() <= 500)
    {
        command.print("Shoot!");
    }
}
#endif

void StateRotDef::update()
{
    enemyPosition += enemyVelocity;
    enemy.setPosition(enemyPosition.x, enemyPosition.y);
    if(shooting)
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
            endSound.loadSound("sounds/RotDef/WIN.ogg");
            endSound.play();
            SDL_Delay(1000);
            variables[0].setInt(1);
            setNextState(STATE_MAIN);
        }

    }

    if(enemy.getX()+(int)enemy.getWidth()*0.5f >= turretBase.getX() &&
       enemy.getX()+(int)enemy.getWidth()*0.5f <= turretBase.getX()+(int)turretBase.getWidth() &&
       enemy.getY()+(int)enemy.getHeight()*0.5f >= turretBase.getY() &&
       enemy.getY()+(int)enemy.getHeight()*0.5f <= turretBase.getY()+(int)turretBase.getHeight() )
    {
            endSound.loadSound("sounds/RotDef/LOSE.ogg");
            endSound.play();
            SDL_Delay(1000);
        variables[0].setInt(0);
        setNextState(STATE_MAIN);
    }
}
