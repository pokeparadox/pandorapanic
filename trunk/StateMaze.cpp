#include "StateMaze.h"
StateMaze::StateMaze()
{
}

Charac::Charac()
{
    xvel = 0;
    yvel = 0;
}

/*Tile::Tile()
{
}*/



StateMaze::~StateMaze()
{
//    clear();
}



void StateMaze::init()
{

    if(variables.size()<SUBSTATE_TRIGGER)
    {
        music.loadMusic("music/Maze/background.ogg");
        music.setLooping(false);
        music.play();
    }



    text.loadFont("font/bip.ttf", 32);
    text.setColour(Colour(RED));
    text.setPosition(40,40);


    hero.loadSprite("images/Maze/hero.png");
    hero.setPosition(64,180);
    hero.setTransparentColour(MAGENTA);

    buttonPrompter.display(ButtonPrompter::BUTTON_A, 200, 300);




    /*blobevil.loadSprite("images/Jumper/blobevil.png");
    blobevil.setTransparentColour(MAGENTA);
    */

    cloudnumber = variables[2].getInt()/15  + 3 ;

    for (int i = 0; i< cloudnumber;i++)
    {
    downhit[i] = 0;
    uphit[i] = 0;
    righthit[i] = 0;
    lefthit[i] = 0;
    downtrigger[i] = 0;
    willekeur = rand()%5;
    if (willekeur == 0){cloud[i].loadSprite("images/Maze/purplecloud.png");}
    else if (willekeur == 1){cloud[i].loadSprite("images/Maze/cloud.png");}
    else if (willekeur == 2){cloud[i].loadSprite("images/Maze/redcloud.png");}
    else if (willekeur == 3){cloud[i].loadSprite("images/Maze/greencloud.png");}
    else if (willekeur == 4){cloud[i].loadSprite("images/Maze/yellowcloud.png");}


    cloud[i].setTransparentColour(MAGENTA);


    cloud[i].setYvel(10);



    }
     cloud[0].setPosition(252,420);
     cloud[1].setPosition(512,400);
     cloud[2].setPosition(712,220);
     cloud[3].setPosition(150,64);
     cloud[4].setPosition(452,64);
     cloud[5].setPosition(306,128);
     cloud[6].setPosition(452,420);
     cloud[7].setPosition(712,264);
     cloud[8].setPosition(452,128);
     cloud[9].setPosition(452,420);

    goal.loadSprite("images/Maze/goal.png");
    goal.setTransparentColour(MAGENTA);

        background0.loadBackground("images/Maze/background.png");



    inputlimiter.setMode(MILLI_SECONDS);
    inputlimiter.start();
    teller.setMode(SECONDS);
    teller.start();
    achievementtimer.setMode(SECONDS);


        tilesprite[0].loadSprite("images/Maze/mazewire.png");
        tilesprite[0].setTransparentColour(MAGENTA);
        tilesprite[1].loadSprite("images/Maze/mazewire2.png");
        tilesprite[1].setTransparentColour(MAGENTA);
        tilesprite[2].loadSprite("images/Maze/mazewire3.png");
        tilesprite[2].setTransparentColour(MAGENTA);
        tilesprite[3].loadSprite("images/Maze/mazewire4.png");
        tilesprite[3].setTransparentColour(MAGENTA);


    youdead = false;

    gamelength = 20;

    willekeur = rand()%2;
    if (willekeur == 0){filename = "scripts/Maze/maze.map";}
    else {filename = "scripts/Maze/maze2.map";}

    ifstream tilemap (filename);

    //if ((variables[2].getInt()/10) == 1)
     //   ifstream tilemap ("scripts/Jumper/bla.map");
    getline(tilemap,testingx);
    getline(tilemap,testingy);
    getline(tilemap,tiletype);
    getline(tilemap,withblob);

    goal.setPosition(stringToInt(testingx),stringToInt(testingy));

    tilenumber = 0;


    int j = 0;
    while (! tilemap.eof() )
    {

        getline(tilemap,testingx);
        getline(tilemap,testingy);
        getline(tilemap,tiletype);
        getline(tilemap,withblob);

        bloksprite[j] = rand()%4;

        blok[j].setDimensions(32,32);
        blok[j].setPosition(stringToInt(testingx),stringToInt(testingy));
        blok[j].setMovement(0,0);
        blok[j].moving = stringToInt(tiletype);
    /*
        if (blok[j].moving == 1){blok[j].setYMovement(rand()%50);}
        if (blok[j].moving == 2){blok[j].setYMovement(50);}
        if (blok[j].moving == 3){blok[j].setXMovement(rand()%40);}
        if (blok[j].moving == 4){blok[j].setXMovement(40);}*/
        tilenumber = j;
        j++;


    }
    //blobnumber = blobnumber - 1;
    tilemap.close();

    /*
    for (int k = 0; k < 100;k++)
    {
        blok[k].setPosition(rand()%26 * 32,rand()%15 * 32);
    }
    tilenumber = 100;*/


}



void StateMaze::userInput()
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

        if (input->isRight())
        {
            hero.setXvel(hero.getXvel() + 45);
            /*if (hero.getYvel() > 0)
            {
                hero.setYvel(hero.getYvel() - 1);
            }
            else
            {
                if (hero.getYvel() < 0){hero.setYvel(hero.getYvel() + 1);}
            }*/
        }
        if (input->isLeft())
        {
            hero.setXvel(hero.getXvel() - 45);
            /*
            if (hero.getYvel() < 0){hero.setYvel(hero.getYvel() - 1);}
            else
            {
                if (hero.getYvel() < 0){hero.setYvel(hero.getYvel() + 1);}
            }*/

        }

        if (input->isDown())
        {
            hero.setYvel(hero.getYvel() + 45);
            /*
            if (hero.getXvel() > 0){hero.setXvel(hero.getXvel() - 1);}
            else
            {
                if (hero.getXvel() < 0){hero.setXvel(hero.getXvel() + 1);}
            }*/

        }
        if (input->isUp())
        {
            hero.setYvel(hero.getYvel() - 45);
            /*
            if (hero.getXvel() > 0){hero.setXvel(hero.getXvel() - 1);}
            else
            {
                if (hero.getXvel() < 0){hero.setXvel(hero.getXvel() + 1);}
            }*/


        }

    /*  DISABLE JUMP
    if (inputlimiter.getScaledTicks() > 15)
    {
        if (input->isA() && hero.ontile > 0 && hero.getYvel() == 0)
        {
            hero.setYvel(- 180);
            inputlimiter.start();
        }
    }
    */
}

#ifdef PENJIN_SDL
    void StateMaze::pauseScreen(SDL_Surface* screen)
    {
        if(!isSubState())
            pauseSymbol(screen);
        text.setColour(WHITE);
        text.setPosition(50,180);
        text.print(screen, "Reach the door to safety and escape the maze!");
        text.setPosition(50,220);
        text.print(screen, "Watch out for the blobs!");
        buttonsOverlay(screen);
    }
#else
    void StateMaze::pauseScreen()
    {

    }
#endif
void StateMaze::onPause()
{
    teller.pause();
}

void StateMaze::onResume()
{
    teller.unpause();
}

#ifdef PENJIN_SDL
    void StateMaze::render(SDL_Surface *screen)
    {
        background0.render(screen);

        for (int i = 0; i < tilenumber; i++)
        {

           if (bloksprite[i] == 0)
           {
                tilesprite[0].setX(blok[i].getX() + blok[i].getXMovement());
                tilesprite[0].setY(blok[i].getY() + blok[i].getYMovement());
                tilesprite[0].render(screen);
           }
           else if (bloksprite[i] == 1)
           {
               tilesprite[1].setX(blok[i].getX() + blok[i].getXMovement());
                tilesprite[1].setY(blok[i].getY() + blok[i].getYMovement());
               tilesprite[1].render(screen);
               }
           else if (bloksprite[i] == 2)
           {
               tilesprite[2].setX(blok[i].getX() + blok[i].getXMovement());
                tilesprite[2].setY(blok[i].getY() + blok[i].getYMovement());
               tilesprite[2].render(screen);
            }
            else if (bloksprite[i] == 3)
           {
               tilesprite[3].setX(blok[i].getX() + blok[i].getXMovement());
                tilesprite[3].setY(blok[i].getY() + blok[i].getYMovement());
               tilesprite[3].render(screen);
            }
        }

        goal.render(screen);
        /*
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
        {*/

        hero.render(screen);

        //}
        if (teller.getScaledTicks() < 3)
        {
            text.setPosition(80,200);


        }

            for (int i = 0;i< cloudnumber;i++)
            {
            cloud[i].render(screen);

            }


        //text.print(screen, blobjump);
        //buttonPrompter.render();
        text.setColour(RED);
        text.setPosition(30,30);

        if (hero.getX() > 96 || hero.getY() > 64){text.setColour(Colour(RED));text.print(screen, (gamelength - teller.getScaledTicks()));}
    }
#else
    void StateMaze::render()
    {
        text.render()
        background0.render();
        for (int i = 1; i < tilenumber; ++i)
        {
           tilesprite.setX(blok[i].getX() + blok[i].getXMovement());
           tilesprite.setY(blok[i].getY() + blok[i].getYMovement());
           tilesprite.render();
        }

        goal.render();

        /*
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
        {*/

        hero.render();


        if (teller.getScaledTicks() < 3)
        {
            text.setPosition(80,200);
        }

            for (int i = 1;i < cloudnumber;i++)
            {
            cloud[i].render();

            }

        //text.print(screen, blobjump);
        //buttonPrompter.render();
        if (hero.getX() > 96 || hero.getY() > 64){text.render(gamelength - teller.getScaledTicks());}

    }
#endif

void StateMaze::update()
{
        //hero.update();
        //if (hero.getYvel() > 0
        //hero.setYvel(hero.getYvel() + 15); Undo gravity
        hero.setX(hero.getX() + (hero.getXvel()*0.11f));
        hero.setY(hero.getY() + (hero.getYvel()*0.11f));

        if (hero.getXvel() > 20 + variables[2].getInt()/4)
        {
            hero.setXvel(20 + variables[2].getInt()/4);
        }
        if (hero.getXvel() < -20 - variables[2].getInt()/4)
        {
            hero.setXvel(-20 - variables[2].getInt()/4);
        }
        if (hero.getYvel() > 20 + variables[2].getInt()/4) {hero.setYvel(18 + + variables[2].getInt()/4);}
        if (hero.getYvel() < -20 - variables[2].getInt()/4) {hero.setYvel(-18 - variables[2].getInt()/4);}


        for (int i = 0;i< cloudnumber;i++)
        {
        cloud[i].setX(cloud[i].getX() + (cloud[i].getXvel()*0.2f));
        cloud[i].setY(cloud[i].getY() + (cloud[i].getYvel()*0.22f));
        }
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

            if (hero.getY() + 28 >= blok[i].getY() + blok[i].getYMovement() && hero.getY() + 12 < blok[i].getY() + blok[i].getYMovement()
                && hero.getX()  > blok[i].getX() + blok[i].getXMovement() - 16 && hero.getX()  < blok[i].getX() + blok[i].getXMovement()+ 24 )
            {
                if (hero.getYvel() > 0){hero.setYvel(0);hero.setY(blok[i].getY() + blok[i].getYMovement() - 30);}

                //hero.ontile = hero.ontile + 1;
            }
            if (hero.getX() > blok[i].getX() + blok[i].getXMovement() - 28 && hero.getX() < blok[i].getX() + blok[i].getXMovement() - 12
                && hero.getY() > blok[i].getY() + blok[i].getYMovement() - 24 && hero.getY() < blok[i].getY() + blok[i].getYMovement() + blok[i].getHeight() - 8)
            {
                if (hero.getXvel() > 0)
                {
                    {hero.setXvel(0);}
                    hero.setX(blok[i].getX() - 30);
                }
                //hero.setXvel(-22);
            }
            if (hero.getX() > blok[i].getX() + blok[i].getXMovement() + blok[i].getWidth() - 20 && hero.getX() < blok[i].getX() + blok[i].getXMovement() + blok[i].getWidth() - 4
                && hero.getY() > blok[i].getY() + blok[i].getYMovement() - 24 && hero.getY() < blok[i].getY() + blok[i].getYMovement() + blok[i].getHeight() - 8)
            {
                if (hero.getXvel() < 0)
                {
                    {hero.setXvel(0);}
                    hero.setX(blok[i].getX() + 30);
                }
                //hero.setXvel();
            }
            if (hero.getX() >  blok[i].getX() + blok[i].getXMovement() - 24 && hero.getX()  < blok[i].getX() + blok[i].getXMovement() + 24
                && hero.getY() > blok[i].getY() + blok[i].getYMovement() + 12 && hero.getY() < blok[i].getY() + blok[i].getYMovement() + blok[i].getHeight())
            {
                if (hero.getYvel() < 0){hero.setYvel(0);hero.setY(blok[i].getY() + blok[i].getYMovement() + 30);}

                //hero.ontile = hero.ontile + 1;

            }


            // Collision Checking with Tles And enemies and heroe and enemies
            for (int j = 0;j < cloudnumber; j++)
            {
            downhit[j] = 0;
            if (cloud[j].getY() + 28 >= blok[i].getY() + blok[i].getYMovement() && cloud[j].getY() + 16 < blok[i].getY() + blok[i].getYMovement()
                && cloud[j].getX()  > blok[i].getX() + blok[i].getXMovement() - 24 && cloud[j].getX()  < blok[i].getX() + blok[i].getXMovement()+ 24)
            {
                if (cloud[j].getYvel() > 0){cloud[j].setYvel(0);}
                cloud[j].setY(blok[i].getY() + blok[i].getYMovement() - 30);
                //cloud[j].ontile = cloud[j].ontile + 1;
                downhit[j] = 1;
            }
            righthit[j] = 0;
            if (cloud[j].getX() > blok[i].getX() + blok[i].getXMovement() - 28 && cloud[j].getX() < blok[i].getX() + blok[i].getXMovement() - 16
                && cloud[j].getY() > blok[i].getY() + blok[i].getYMovement() - 24 && cloud[j].getY() < blok[i].getY() + blok[i].getYMovement() + blok[i].getHeight() - 8)
            {
                if (cloud[j].getXvel() > 0)
                {
                    {cloud[j].setXvel(0);}
                    cloud[j].setX(blok[i].getX() - 30);
                    righthit[j] = 1;
                }
                //cloud[j].setXvel(-22);
            }
            lefthit[j] = 0;
            if (cloud[j].getX() > blok[i].getX() + blok[i].getXMovement() + blok[i].getWidth() - 16 && cloud[j].getX() < blok[i].getX() + blok[i].getXMovement() + blok[i].getWidth() - 4
                && cloud[j].getY() > blok[i].getY() + blok[i].getYMovement() - 24 && cloud[j].getY() < blok[i].getY() + blok[i].getYMovement() + blok[i].getHeight() - 8)
            {
                if (cloud[j].getXvel() < 0)
                {
                    {cloud[j].setXvel(0);}
                    cloud[j].setX(blok[i].getX() + 30);
                    lefthit[j] = 1;
                }
                //cloud[j].setXvel();
            }
            uphit[j] = 0;
            if (cloud[j].getX() >  blok[i].getX() + blok[i].getXMovement() - 24 && cloud[j].getX()  < blok[i].getX() + blok[i].getXMovement() + 24
                && cloud[j].getY() > blok[i].getY() + blok[i].getYMovement() + 16 && cloud[j].getY() < blok[i].getY() + blok[i].getYMovement() + 28)
            {
                if (cloud[j].getYvel() < 0){cloud[j].setYvel(0);}
                cloud[j].setY(blok[i].getY() + blok[i].getYMovement() + 30);
                uphit[j] = 1;
                //cloud[j].ontile = cloud[j].ontile + 1;

            }
            willekeur = rand()%3;
            if (downhit[j] == 1)
            {
                if (willekeur == 0){cloud[j].setXvel(10);}
                else if (willekeur == 1){cloud[j].setXvel(-10);}
                else {cloud[j].setYvel(-10);}
            }
            if (righthit[j] == 1)
            {
                if (willekeur == 0){cloud[j].setYvel(10);}
                else if (willekeur == 1){cloud[j].setYvel(-10);}
                else {cloud[j].setXvel(-10);}

            }
            if (lefthit[j] == 1)
            {
                if (willekeur == 0){cloud[j].setYvel(10);}
                else if (willekeur == 1){cloud[j].setYvel(-10);}
                else {cloud[j].setXvel(10);}
            }
            if (uphit[j] == 1)
            {
                if (willekeur == 0){cloud[j].setXvel(10);}
                else if (willekeur == 1){cloud[j].setXvel(-10);}
                else {cloud[j].setYvel(10);}
            }


                if (cloud[j].getXvel()> 10){cloud[j].setXvel(10);}
                if (cloud[j].getXvel()< -10){cloud[j].setXvel(-10);}
                if (cloud[j].getYvel()> 10){cloud[j].setYvel(10);}
                if (cloud[j].getYvel()< -10){cloud[j].setYvel(-10);}


            if (cloud[j].getX() > hero.getX() - 14 && cloud[j].getX() < hero.getX() + 14
            && cloud[j].getY() > hero.getY() - 14 && cloud[j].getY() < hero.getY() + 14)
            {
               youdead = true;
            }
            if (cloud[j].getX() > hero.getX() - 16 && cloud[j].getX() < hero.getX() + 16
            && cloud[j].getY() > hero.getY() - 16 && cloud[j].getY() < hero.getY() + 16 && youdead == false)
            {
                if (hero.getXvel() > 8 || hero.getXvel() < -8 || hero.getYvel() > 8 || hero.getYvel() < -8)
                {
                    achievementtimer.start();
                }
            }
            }
        }

        // various limiters
        if (hero.getXvel() < -3) {hero.setXvel(hero.getXvel() + 3);}
        if (hero.getXvel() > 3) {hero.setXvel(hero.getXvel() - 3);}
        if (hero.getXvel() <= 3 && hero.getXvel() >= -3){hero.setXvel(0);}

        if (hero.getYvel() < -3) {hero.setYvel(hero.getYvel() + 3);}
        if (hero.getYvel() > 3) {hero.setYvel(hero.getYvel() - 3);}
        if (hero.getYvel() <= 3 && hero.getYvel() >= -3){hero.setYvel(0);}


        // Check if goal is reached

        if (hero.getX() >  goal.getX() - 32 && hero.getX()  < goal.getX() + 32
           && hero.getY() > goal.getY() -32 && hero.getY() < goal.getY() + 32)
        {
            ACHIEVEMENTS->logEvent("MAZE_WIN",teller.getScaledTicks());
            variables[0].setInt(1);
            setNextState(STATE_MAIN);
        }

        // check for death

        if( youdead == true)
        {
            variables[0].setInt(0);
            setNextState(STATE_MAIN);
        }
        if(teller.getScaledTicks() > gamelength)
        {
            variables[0].setInt(0);
            setNextState(STATE_MAIN);
        }
        if(achievementtimer.getScaledTicks() > 1 && youdead == false)
        {
            ACHIEVEMENTS->logEvent("MAZE_SNEAKTHROUGH");
        }
}
