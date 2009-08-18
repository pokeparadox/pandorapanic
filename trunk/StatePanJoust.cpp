#include "StatePanJoust.h"

StatePanJoust::StatePanJoust()
{
}

StatePanJoust::~StatePanJoust()
{
//    clear();
}

void StatePanJoust::init()
{
    background.loadBackground("images/PanJoust/background.png");
    if(variables.size()<SUBSTATE_TRIGGER)
    {
        joustmusic.loadMusic("music/PanJoust/joustmusic.ogg");
        joustmusic.setLooping(true);
        joustmusic.play();
        onenemy.loadSound("sounds/PanJoust/enemydead.ogg");
    }
    panic = false;
    text.loadFont("font/bip.ttf",32);
    text.setColour(Colour(GREEN));
    inputlimiter.setMode(MILLI_SECONDS);
    enemynumber = (variables[2].getInt()/8) + 2;
    teller.setMode(SECONDS);
    teller.start();
    killcount = 0;
    gamelength = (variables[2].getInt()/12) + 9;

    jet.setMax(840);
    //jet.setInvisible(400);
    jet.setColour(YELLOW);
    jet.setGravity(RIGHT_HOOK);
    jet.setLifeTime(150);

    hero1.setPosition(380,380);
    platformnumber = 10 - (variables[2].getInt()/10);
    youdead = 0;
    youwin = 0;
    //platformnumber = 4;  // FOR  TESTING
    //enemynumber = 3; // FOR  TESTING
    //gamelength = 10;  // FOR  TESTING
    ground.loadSprite("images/PanJoust/ground.png");
    ground.setTransparentColour(MAGENTA);

    platform = new Sprite[platformnumber];

    for (int i = 0; i < platformnumber; i++)
    {
        platform[i].loadSprite("images/PanJoust/platform.png");
        platform[i].setTransparentColour(MAGENTA);

        enemyonplatform[i] = 0;
        enemybelowplatform[i] = 0;
        enemy[i].update();
    }

    platform[0].setPosition(420,460);
    platform[1].setPosition(180,460);
    platform[2].setPosition(205,340);
    platform[3].setPosition(620,280);
    platform[4].setPosition(-185,280);
    platform[5].setPosition(300,150);
    platform[6].setPosition(-150,110);
    platform[7].setPosition(700,110);
    platform[8].setPosition(660,460);
    platform[9].setPosition(-50,460);





    for (int i = 0; i < enemynumber; i++)
    {
        int j = (rand()%5) + 2;
        temprand = rand()%2;
        enemydead[i] = 0;
        if (temprand == 0)
        {
            enemy[i].loadFrames("images/PanJoust/birdrideranim.png", 8, 1);
            enemy[i].setTransparentColour(MAGENTA);
            enemyvel[i].x = rand()%11 + 2;
            enemy[i].setCurrentFrame(i);
        }
        else
        {
            enemy[i].loadFrames("images/PanJoust/birdrideranimright.png", 8, 1);
            enemy[i].setTransparentColour(MAGENTA);
            enemyvel[i].x = - rand()%11 + 2;
            enemy[i].setCurrentFrame(i);
        }
        enemy[i].setFrameRate(DECI_SECONDS);
        enemy[i].setPosition(platform[j].getX() + 100, platform[j].getY()-42);
    }
    enemy[1].setCurrentFrame(15);
    enemyvel[1].x = 2;
    enemyvel[3].x = -2;
    enemyvel[8].x = 2;
    inputlimiter.start();

}


void StatePanJoust::userInput()
{
    input->update();
    if (input->isStart())
    {
        isPaused = !isPaused;
        input->resetKeys();
    }
    if(input->isA() && youdead == 0)
    {
        if (inputlimiter.getScaledTicks() > 40)
        {
            hero1.wingFlap();
            if (input->isLeft())
            {
                hero1.setDirection(0);
                hero1.changeXVelocity(-50);
            }
            if (input->isRight())
            {
                hero1.setDirection(1);
                hero1.changeXVelocity(+50);
            }
        }
        inputlimiter.start();
    }
    else
    {
        if (hero1.getOnPlatform() == true)
        {
            if (input->isLeft())
            {
                hero1.setDirection(0);
                hero1.changeXVelocity(-1.25f);
            }
            if (input->isRight())
            {
                 hero1.setDirection(1);
                hero1.changeXVelocity(+1.25f);
           }
        }
        else
        {
                if (input->isLeft())
                {
                    hero1.setDirection(0);
                    hero1.changeXVelocity(-0.50f);
                }
                if (input->isRight())
                {
                    hero1.setDirection(1);
                    hero1.changeXVelocity(+0.50f);
                }
        }
    }
}

#ifdef PENJIN_SDL
void StatePanJoust::pauseScreen(SDL_Surface* screen)
{
    pauseSymbol(screen);
    text.setPosition(50,200);
    text.setColour(WHITE);
    text.print(screen, "Fly! And land on the other birds to kill them");
    //buttonPrompter.render(screen);
}
#else
void StatePanJoust::pauseScreen()
{

}
#endif

void StatePanJoust::onPause()
{
    teller.pause();
}

void StatePanJoust::onResume()
{
    teller.unpause();
}

#ifdef PENJIN_SDL
void StatePanJoust::render(SDL_Surface *screen)
{
    background.render(screen);
    for (int i = 0; i < platformnumber; ++i)
    {
        if (i > 1 && i < 8)
        {
            platform[i].render(screen);
        }
        else
        {
            ground.setPosition(platform[i].getPosition());
            ground.render(screen);
        }

    }
    /* Particle Thingies
    jet.setPosition(Vector2df(hero1.getX() + 20, hero1.getY()+40));
    if (hero1.getDirection() == 0)
    {jet.setGravity(Vector2df(1,0));}
    else
    {jet.setGravity(Vector2df(-1,0));}

    jet.render(screen);
    */

    hero1.render(screen);

    for (int i = 0; i < enemynumber; ++i)
    {
        enemy[i].clearFrames();
        if (enemyvel[i].x > 0)
        {
            enemy[i].loadFrames("images/PanJoust/birdrideranim.png", 8, 1);
            enemy[i].setTransparentColour(MAGENTA);
        }
        else
        {
            enemy[i].loadFrames("images/PanJoust/birdrideranimright.png", 8, 1);
            enemy[i].setTransparentColour(MAGENTA);
        }
        enemy[i].render(screen);
    }
    if (youdead == 0 && youwin == 0)
    {
        text.setPosition(50,30);
        //text.print(screen, gamelength - teller.getScaledTicks());
        //text.print(screen, panic); //testing
    }
    if (youdead && !youwin)
    {
        text.setPosition(320,210);
        text.print(screen, "Too Bad");
    }
    if (youwin)
    {
        text.setPosition(320,210);
        text.print(screen, "Nice 1!");
    }
    //buttonPrompter.render(screen);
}
#else
    void StatePanJoust::render()
    {
        background.render();
        for (int i = 0; i < platformnumber; ++i)
        {
            if (i > 1 && i < 8)
            {
                platform[i].render();
            }
            else
            {
                ground.setPosition(platform[i].getPosition());
                ground.render();
            }

        }

        jet.setPosition(Vector2df(hero1.getX() + 20, hero1.getY()+40));
        if (hero1.getDirection() == 0)
        {jet.setGravity(Vector2df(1,0));}
        else
        {jet.setGravity(Vector2df(-1,0));}

        jet.render();

        hero1.render();

        for (int i = 0; i < enemynumber; ++i)
        {
            enemy[i].clearFrames();
            if (enemyvel[i].x > 0)
            {
                enemy[i].loadFrames("images/PanJoust/birdrideranim.png", 8, 1);
                enemy[i].setTransparentColour(MAGENTA);
            }
            else
            {
                enemy[i].loadFrames("images/PanJoust/birdrideranimright.png", 8, 1);
                enemy[i].setTransparentColour(MAGENTA);
            }
            enemy[i].render();
        }
        if (youdead == 0 && youwin == 0)
        {
            text.setPosition(50,30);
            //text.print(screen, gamelength - teller.getScaledTicks());
            //text.print(screen, panic); //testing
        }
        if (youdead && !youwin)
        {
            text.setPosition(320,210);
            text.print("Too Bad");
        }
        if (youwin)
        {
            text.setPosition(320,210);
            text.print("Nice 1!");
        }
        //buttonPrompter.render(screen);
    }
#endif

void StatePanJoust::update()
{
    jet.update();
    #ifdef PLATFORM_PC
    if(input->isQuit())
    nullifyState();
    #endif
    // Hier alle actieve shit
    // set enemies and check for collision
    for (int i = 0; i < enemynumber; i++)
    {
        //if(teller.getScaledTicks() >= 1)
        //{
        if(enemy[i].getX() >= hero1.getX() - 50 && enemy[i].getX() <= hero1.getX() + 50
            && enemy[i].getY() >= hero1.getY() - 30 && enemy[i].getY() <= hero1.getY() + 40 && teller.getScaledTicks() >=1)
        {
            if (panic == false)
            {
                if (hero1.getY() < (enemy[i].getY()) )
                {
                    onenemy.play();
                    if (enemydead[i] == 0)
                    {
                    enemydead[i] = 1;
                    killcount = killcount + 1;
                    hero1.changeYVelocity(-50);
                    }
                }
                else if (!youwin)
                {
                    youdead = true;
                    teller.start();
                }
            }
            else
            {
                if (i < enemynumber)
                {
                    if (hero1.getY() < (enemy[i].getY()) && i < enemynumber - 1 )
                    {
                        onenemy.play();
                        if (enemydead[i] == 0 )
                        {
                            enemydead[i] = 1;
                            killcount = killcount + 1;
                            hero1.changeYVelocity(-50);
                        }

                    }
                    else
                    {
                        if (!youwin)
                        {
                        youdead = true;
                        teller.start();
                        }
                    }
                }
            }


            if (hero1.getY() > enemy[i].getY())
            {
                if (youdead == false && !youwin)
                {
                youdead = true;
                teller.start();
                }
            }
        }
        //}
        if (enemydead[i] == 1)
        {

            enemy[i].setX(enemy[i].getX() + 20);
        }
        if (youdead == 1)
        {
            hero1.setY(hero1.getY() + 1);
        }
        else
        {
            enemy[i].setX(enemy[i].getX() - enemyvel[i].x);
            if (enemyvel[i].x > 0 || enemyvel[i].x < 0)
            {
                enemyonplatform[i] = 0;
                enemybelowplatform[i] = 0;
                for (int j = 0; j< platformnumber; j++)
                {
                    if (enemy[i].getX() > platform[j].getX() - 64 && enemy[i].getX() < platform[j].getX() + 230
                        && enemy[i].getY() > platform[j].getY() - 42  && enemy[i].getY() < platform[j].getY() - 22)
                    {
                        enemyonplatform[i] = 1;

                    }

                    if (enemy[i].getX() > platform[j].getX() - 64 && enemy[i].getX() < platform[j].getX() + 250
                        && enemy[i].getY() > platform[j].getY()  && enemy[i].getY() < platform[j].getY() +30)
                    {
                        enemybelowplatform[i] = 1;
                    }



                }
                if (enemy[i].getY() > hero1.getY() && enemyonplatform[i] == 0 && enemybelowplatform[i] == 0)
                {
                    enemy[i].setY(enemy[i].getY() - 1);
                }


                if (enemy[i].getY() < hero1.getY() && enemyonplatform[i] == 0 )
                {
                     enemy[i].setY(enemy[i].getY() + 1);
                }
            }
        }
        if (enemy[i].getX() < -40)
        {
            enemy[i].setX(780);
        }

        if (enemy[i].getX() > 780)
        {
            enemy[i].setX(-40);
        }
        if (enemydead[i] == 1)
        {
            enemy[i].setY(enemy[i].getY() + 20);

        }
        if (enemyonplatform[i] == 0)
        {
            enemy[i].update();
        }
    }

    hero1.update();

    int temp = hero1.getOnPlatform();
    hero1.resetOnPlatform();
    for (int i = 0; i < platformnumber; i++)
    {
        if (hero1.getX() > platform[i].getX() - 64 && hero1.getX() < platform[i].getX() - 30
            && hero1.getY() > platform[i].getY() - 41 && hero1.getY() < platform[i].getY()&& hero1.getXVelocity() > 0)
        {
            hero1.setXVelocity(-30);
        }
        if (hero1.getX() > platform[i].getX() - 56 && hero1.getX() < platform[i].getX() + 242
            && hero1.getY() > platform[i].getY()-7  && hero1.getY() < platform[i].getY() +20)
        {
            hero1.setYVelocity(15);
        }
        if (hero1.getX() > platform[i].getX() - 32 && hero1.getX() < platform[i].getX() + 230
            && hero1.getY() >= platform[i].getY() - 64  && hero1.getY() < platform[i].getY() - 30 && youdead == false )
        {
            hero1.setYVelocity(0);
            hero1.setOnPlatform();
            hero1.setY(platform[i].getY() - 64);
        }

        if (hero1.getX() > platform[i].getX() + 220 && hero1.getX() < platform[i].getX() + 250
            && hero1.getY() > platform[i].getY() - 41 && hero1.getY() < platform[i].getY() && hero1.getXVelocity() < 0)
        {
            hero1.setXVelocity(30);
        }

        for (int j = 0; j < enemynumber; j++)
        {
            if (enemy[j].getX() > platform[i].getX() - 64 && enemy[j].getX() < platform[i].getX() - 30
                && enemy[j].getY() > platform[i].getY() - 42 && enemy[j].getY() < platform[i].getY() + 20 && enemyvel[j].x < 0)
            {
                enemyvel[j].x = - enemyvel[j].x;

            }
            if (enemy[j].getX() > platform[i].getX() + 220 && enemy[j].getX() < platform[i].getX() + 250
                && enemy[j].getY() > platform[i].getY() - 42 && enemy[j].getY() < platform[i].getY() + 20 && enemyvel[j].x > 0)
            {
                enemyvel[j].x = - enemyvel[j].x;
            }


        }
    }


    if (hero1.getOnPlatform() != temp)
    {
        hero1.setY(hero1.getY() + 7);
    }


    if (hero1.getY() > 450)
    {
    if (youdead == false)
    {
        youdead = true;
        teller.start();
    }
    }

    // Check if end of game for various reasons
    if (teller.getScaledTicks() == 2 && youdead == 1)
    {
        variables[0].setInt(0);
        setNextState(STATE_MAIN);
    }
    if (teller.getScaledTicks() == 2 && youwin == 1)
    {
        variables[0].setInt(1);
        setNextState(STATE_MAIN);
    }
    if (killcount == enemynumber && youdead == 0 && youwin == 0 && panic == false)
    {
        youwin = 1;
        teller.start();
    }
    if (killcount == enemynumber - 1 && youdead == 0 && youwin == 0 && panic == true)
    {
        youwin = 1;
        teller.start();
    }
    if(teller.getScaledTicks() > (gamelength) && panic == false)
    {
        enemynumber = enemynumber + 1;
        enemy[enemynumber-1].loadFrames("images/PanJoust/birdrideranim.png", 8, 1);
        enemy[enemynumber-1].setTransparentColour(MAGENTA);
        enemyvel[enemynumber-1].x = 25;
        enemy[enemynumber-1].setFrameRate(DECI_SECONDS);
        enemy[enemynumber-1].setX(800);
        enemy[enemynumber-1].setY(350);
        panic = true;
    }
}
