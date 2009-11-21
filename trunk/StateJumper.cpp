#include "StateJumper.h"
StateJumper::StateJumper()
{
}

Character::Character()
{
    xvel = 0;
    yvel = 0;
}

Tile::Tile()
{
}



StateJumper::~StateJumper()
{
//    clear();
}



void StateJumper::init()
{
    text.loadFont("font/bip.ttf", 32);
    text.setColour(Colour(WHITE));
    text.setPosition(40,40);

    robotleft.loadFrames("images/Jumper/robotwalkright.png",4,1);
    robotleft.setPosition(64,180);
    robotleft.setTransparentColour(MAGENTA);
    robotleft.setFrameRate(DECI_SECONDS);

    hero.loadSprite("images/Jumper/robot.png");
    hero.setPosition(64,180);
    hero.setTransparentColour(MAGENTA);

    robotright.loadFrames("images/Jumper/robotwalkleft.png",4,1);
    robotright.setPosition(200,180);
    robotright.setTransparentColour(MAGENTA);
    robotright.setFrameRate(DECI_SECONDS);

    buttonPrompter.display(ButtonPrompter::BUTTON_A, 200, 300);




    blobevil.loadSprite("images/Jumper/blobevil.png");
    blobevil.setTransparentColour(MAGENTA);

    cloud.loadSprite("images/Jumper/cloud.png");
    cloud.setTransparentColour(MAGENTA);

    goal.loadSprite("images/pp_life.png");
    goal.setTransparentColour(MAGENTA);
    int randommy = rand()%4;
    if (randommy == 0)
        background0.loadBackground("images/Jumper/blocks.png");
    if (randommy == 1)
        background0.loadBackground("images/Jumper/jumperback.png");
    if (randommy == 2)
        background0.loadBackground("images/Jumper/jumperbackcolour.png");
    if (randommy == 3)
        background0.loadBackground("images/Jumper/jumperbackstripes.png");

    //background0.setTransparentColour(MAGENTA);


    inputlimiter.setMode(MILLI_SECONDS);
    inputlimiter.start();
    teller.setMode(SECONDS);
    teller.start();
    tilesprite.loadSprite("images/Jumper/tile1.png");
    if ((variables[2].getInt()/10) == 0)
        filename = "scripts/Jumper/testing.map";
    else if ((variables[2].getInt()/10) == 1)
        filename = "scripts/Jumper/third.map";
    else if ((variables[2].getInt()/10) >= 2)
        filename = "scripts/Jumper/nogeen.map";

    ifstream tilemap (filename);

    //if ((variables[2].getInt()/10) == 1)
     //   ifstream tilemap ("scripts/Jumper/bla.map");
    getline(tilemap,testingx);
    getline(tilemap,testingy);
    getline(tilemap,tiletype);
    getline(tilemap,withblob);

    goal.setPosition(stringToInt(testingx),stringToInt(testingy));

    tilenumber = 0;
    for (int i = 0; i < 100; i++)
    {
        blobjump[i] = 0;
        blob[i].loadSprite("images/Jumper/blob.png");
        blob[i].setTransparentColour(MAGENTA);
    }
    blobnumber = 0;

    int j = 0;
    while (! tilemap.eof() )
    {

        getline(tilemap,testingx);
        getline(tilemap,testingy);
        getline(tilemap,tiletype);
        getline(tilemap,withblob);

        blok[j].setDimensions(32,32);
        blok[j].setPosition(stringToInt(testingx),stringToInt(testingy));
        blok[j].setMovement(0,0);
        blok[j].moving = stringToInt(tiletype);
        if (stringToInt(withblob) == 1)
        {
           blob[blobnumber].setPosition(stringToInt(testingx),stringToInt(testingy) - 32);
           blob[blobnumber].connecttotile = j;
           blobjump[blobnumber] = rand()%100;
           blobtimer[blobnumber] = 50 + rand()%50;
           blob[blobnumber].type = 1;

           blobnumber++;
        }
        if (stringToInt(withblob) == 2)
        {
           blob[blobnumber].setPosition(stringToInt(testingx),stringToInt(testingy) - 32);
           blob[blobnumber].connecttotile = j;
           blobjump[blobnumber] = rand()%100;
           blobtimer[blobnumber] = 50 + rand()%50;
           blob[blobnumber].type = 2;
           blob[blobnumber].offset = rand()%60 + 10;
           blob[blobnumber].setYvel(1);
           blobnumber++;
        }

        if (blok[j].moving == 1){blok[j].setYMovement(rand()%50);}
        if (blok[j].moving == 2){blok[j].setYMovement(50);}
        if (blok[j].moving == 3){blok[j].setXMovement(rand()%40);}
        if (blok[j].moving == 4){blok[j].setXMovement(40);}
        tilenumber = j;
        j++;




    }
    blobnumber = blobnumber - 1;
    tilemap.close();
    if(variables.size()<SUBSTATE_TRIGGER)
    {
        music.loadMusic("music/Jumper/PP-Jumper.ogg");
        music.setLooping(true);
        music.play();
    }
}



void StateJumper::userInput()
{
    input->update();
    if (input->isStart())
    {
        pauseToggle();
        input->resetKeys();
    }

        if (input->isRight())
        {
            hero.setXvel(hero.getXvel() + 15);

        }
        if (input->isLeft())
        {
            hero.setXvel(hero.getXvel() - 15);

        }
    if (inputlimiter.getScaledTicks() > 15)
    {
        if (input->isA() && hero.ontile > 0 && hero.getYvel() == 0)
        {
            hero.setYvel(- 180);
            inputlimiter.start();
        }
    }
}

#ifdef PENJIN_SDL
    void StateJumper::pauseScreen(SDL_Surface* screen)
    {
        if(variables.size()<SUBSTATE_TRIGGER)
            pauseSymbol(screen);
        text.setColour(WHITE);
        text.setPosition(50,180);
        text.print(screen, "Jump towards the star to win");
        text.setPosition(50,220);
        text.print(screen, "Green blobs only kill you when they jump");
    }
#else
    void StateJumper::pauseScreen()
    {

    }
#endif
void StateJumper::onPause()
{
    teller.pause();
}

void StateJumper::onResume()
{
    teller.unpause();
}

#ifdef PENJIN_SDL
    void StateJumper::render(SDL_Surface *screen)
    {
        background0.render(screen);
        for (int i = 1; i < tilenumber; i++)
        {
           tilesprite.setX(blok[i].getX() + blok[i].getXMovement());
           tilesprite.setY(blok[i].getY() + blok[i].getYMovement());
           tilesprite.render(screen);
        }

        goal.render(screen);
        if (hero.getXvel() > 4)
        {
            robotleft.setPosition(hero.getPosition());
            robotleft.render(screen);
        }
        else if (hero.getXvel() < -4)
        {
            robotright.setPosition(hero.getPosition());
            robotright.render(screen);
        }
        else
        {
        hero.render(screen);
        }
        if (teller.getScaledTicks() < 3)
        {
            text.setPosition(80,200);


        }
        for (int i = 0; i < blobnumber; i++)
        {
            if (blob[i].type == 1)
            {


                if (blob[i].getYvel() == 0)
                {
                    blob[i].render(screen);
                }
                else
                {
                    blobevil.setPosition(blob[i].getPosition());
                    blobevil.render(screen);
                }
            }
            else
            {
                cloud.setPosition(blob[i].getPosition());
                cloud.render(screen);
            }
        }

        //text.print(screen, blobjump);
        buttonPrompter.render();
    }
#else
    void StateJumper::render()
    {
        background0.render();
        for (int i = 1; i < tilenumber; ++i)
        {
           tilesprite.setX(blok[i].getX() + blok[i].getXMovement());
           tilesprite.setY(blok[i].getY() + blok[i].getYMovement());
           tilesprite.render();
        }

        goal.render();
        if (hero.getXvel() > 4)
        {
            robotleft.setPosition(hero.getPosition());
            robotleft.render();
        }
        else if (hero.getXvel() < -4)
        {
            robotright.setPosition(hero.getPosition());
            robotright.render();
        }
        else
        {
            hero.render();
        }
        if (teller.getScaledTicks() < 3)
        {
            text.setPosition(80,200);
        }
        for (int i = 0; i < blobnumber; ++i)
        {
            if (blob[i].type == 1)
            {
                if (blob[i].getYvel() == 0)
                {
                    blob[i].render();
                }
                else
                {
                    blobevil.setPosition(blob[i].getPosition());
                    blobevil.render();
                }
            }
            else
            {
                cloud.setPosition(blob[i].getPosition());
                cloud.render();
            }
        }

        //text.print(screen, blobjump);
        buttonPrompter.render();
    }
#endif

void StateJumper::update()
{
    #ifdef PLATFORM_PC
    if(input->isQuit())
    nullifyState();
    #endif
    if (!isPaused)
    {
        //hero.update();
        //if (hero.getYvel() > 0
        hero.setYvel(hero.getYvel() + 15);
        hero.setX(hero.getX() + (hero.getXvel()*0.1f));
        hero.setY(hero.getY() + (hero.getYvel()*0.11f));
        hero.ontile = 0;

        // The code for tile movement

        for (int i = 1; i < tilenumber; i++)
        {
            if (blok[i].moving == 1)
            {
                blok[i].setYMovement(blok[i].getYMovement() + 1);
                if (blok[i].getYMovement() == 100){blok[i].moving = 2;}
            }
            if (blok[i].moving == 2)
            {
                blok[i].setYMovement(blok[i].getYMovement() - 1);
                if (blok[i].getYMovement() == 0){blok[i].moving = 1;}
            }
            if (blok[i].moving == 3)
            {
                blok[i].setXMovement(blok[i].getXMovement() - 1);
                if (blok[i].getXMovement() == 0){blok[i].moving = 4;}
            }
            if (blok[i].moving == 4)
            {
                blok[i].setXMovement(blok[i].getXMovement() + 1);
                if (blok[i].getXMovement() == 50){blok[i].moving = 3;}
            }

            // collision checking with tile and hero

            if (hero.getY() + 64 >= blok[i].getY() + blok[i].getYMovement() && hero.getY() + 48 < blok[i].getY() + blok[i].getYMovement()
                && hero.getX()  > blok[i].getX() + blok[i].getXMovement() - 50 && hero.getX()  < blok[i].getX() + blok[i].getXMovement()+ 13)
            {
                if (hero.getYvel() > 0){hero.setYvel(0);}
                hero.setY(blok[i].getY() + blok[i].getYMovement() - 64);
                hero.ontile = hero.ontile + 1;
            }
            if (hero.getX() > blok[i].getX() + blok[i].getXMovement() - 56 && hero.getX() < blok[i].getX() + blok[i].getXMovement() - 35
                && hero.getY() > blok[i].getY() + blok[i].getYMovement() - 64 && hero.getY() < blok[i].getY() + blok[i].getYMovement() + blok[i].getHeight())
            {
                hero.setXvel(0);
                hero.setXvel(-22);
            }
            if (hero.getX() > blok[i].getX() + blok[i].getXMovement() + blok[i].getWidth() - 30 && hero.getX() < blok[i].getX() + blok[i].getXMovement() + blok[i].getWidth() - 20
                && hero.getY() > blok[i].getY() + blok[i].getYMovement() - 64 && hero.getY() < blok[i].getY() + blok[i].getYMovement() + blok[i].getHeight() )
            {
                hero.setXvel(0);
                hero.setXvel(22);
            }
            if (hero.getX() >  blok[i].getX() + blok[i].getXMovement() - 56 && hero.getX()  < blok[i].getX() + blok[i].getXMovement() + 16
                && hero.getY() > blok[i].getY() + blok[i].getYMovement() + 20 && hero.getY() < blok[i].getY() + blok[i].getYMovement() + blok[i].getHeight())
            {
                if (hero.getYvel() < 0)
                    hero.setYvel(15);
                //hero.setX(hero.getX() - 8);

            }

        }



        // various limiters

        if (hero.getXvel() > 80)
        {
            hero.setXvel(80);
        }
        if (hero.getXvel() < -80)
        {
            hero.setXvel(-80);
        }
        if (hero.getYvel() > 130) {hero.setYvel(130);}
        if (hero.getXvel() < -3) {hero.setXvel(hero.getXvel() + 6);}
        if (hero.getXvel() > 3) {hero.setXvel(hero.getXvel() - 6);}
        if (hero.getXvel() <= 3 && hero.getXvel() >= -3){hero.setXvel(0);}

        // The blob updates

        for (int i = 0; i < blobnumber; i++)
        {
            if (blob[i].type == 1)
            {
                blobjump[i] = blobjump[i] + 1;

                if (blobjump[i] == blobtimer[i]){blob[i].setYvel(-35);blobjump[i] = 0;}
                blob[i].setYvel(blob[i].getYvel() + 4);
                blob[i].setY(blob[i].getY() + blob[i].getYvel());
                if (blob[i].getY() >= blok[blob[i].connecttotile].getY() + blok[blob[i].connecttotile].getYMovement() -32 )
                {
                    blob[i].setY(blok[blob[i].connecttotile].getY() + blok[blob[i].connecttotile].getYMovement() - 32);
                    blob[i].setYvel(0);
                }

                blob[i].setX(blok[blob[i].connecttotile].getX() + blok[blob[i].connecttotile].getXMovement());
                if (hero.getX() > blob[i].getX() - 48 && hero.getX() < blob[i].getX() + 16
                && hero.getY() > blob[i].getY() - 64 && hero.getY() < blob[i].getY() + 16 && blob[i].getYvel() != 0)
                {
                variables[0].setInt(0);
                setNextState(STATE_MAIN);
                }

            }
            else
            {
                if (blob[i].getYvel() == 0)
                {
                    blob[i].offset = blob[i].offset + 1;
                    blob[i].setY(blok[blob[i].connecttotile].getY() + blok[blob[i].connecttotile].getYMovement() - blob[i].offset);
                    if (blob[i].offset == 80){blob[i].setYvel(1);}
                    blob[i].setX(blok[blob[i].connecttotile].getX() + blok[blob[i].connecttotile].getXMovement());

                }
                if (blob[i].getYvel() == 1)
                {
                    blob[i].offset = blob[i].offset - 1;
                    blob[i].setY(blok[blob[i].connecttotile].getY() + blok[blob[i].connecttotile].getYMovement() - blob[i].offset);
                    if (blob[i].offset == 0){blob[i].setYvel(0);}
                    blob[i].setX(blok[blob[i].connecttotile].getX() + blok[blob[i].connecttotile].getXMovement());
                }
                if (hero.getX() > blob[i].getX() - 48 && hero.getX() < blob[i].getX() + 16
                && hero.getY() > blob[i].getY() - 64 && hero.getY() < blob[i].getY() + 16)
            {
                variables[0].setInt(0);
                setNextState(STATE_MAIN);
            }

            }



        }

        robotright.update();
        robotleft.update();


        // Check if goal is reached

        if (hero.getX() >  goal.getX() - 64 && hero.getX()  < goal.getX() + 64
           && hero.getY() > goal.getY() -64 && hero.getY() < goal.getY() + 64)
        {
            variables[0].setInt(1);
            setNextState(STATE_MAIN);
        }

        // check for death

        if(hero.getY() > 490)
        {
            variables[0].setInt(0);
            setNextState(STATE_MAIN);
        }
        /*if(teller.getScaledTicks() > 15)
        {
            variables[0].setInt(0);
            setNextState(STATE_MAIN);
        }*/
    }
}

void StateJumper::pauseInput()
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
