#include "StateTileEditor.h"
#include "StringUtility.h"

using namespace StringUtility;

StateTileEditor::StateTileEditor()
{

}

StateTileEditor::~StateTileEditor()
{
}

void StateTileEditor::init()
{
    blob.loadSprite("images/Jumper/blob.png");
    blob.setTransparentColour(MAGENTA);
    cloud.loadSprite("images/Jumper/cloud.png");
    cloud.setTransparentColour(MAGENTA);
    background.loadBackground("images/DistractedMaths/back0.png");
    background.setTransparentColour(MAGENTA);
    tilesprite.loadSprite("images/Jumper/tile1.png");
    tiledown.loadSprite("images/Jumper/tiledown.png");
    tileup.loadSprite("images/Jumper/tileup.png");
    tileleft.loadSprite("images/Jumper/tileleft.png");
    tileright.loadSprite("images/Jumper/tileright.png");
    goal.loadSprite("images/pp_life.png");
    goal.setTransparentColour(MAGENTA);
    scope.x = 280;
    scope.y = 220;
    inputlimiter.setMode(DECI_SECONDS);
    inputlimiter.start();
    tilenumber = 0;
    temptiletype = 0;
    goaldefined = false;
    test.loadFont("font/atrox.ttf",64);
    test.setColour(Colour(WHITE));
    withblob = 0;
    del = 0;

    // load level

    ifstream tilemap ("scripts/Jumper/fifth.map");
    tilenumber = 0;

    getline(tilemap,testingx);
    getline(tilemap,testingy);
    goal.setPosition(stringToInt(testingx),stringToInt(testingy));
    getline(tilemap,soort);
    getline(tilemap,hasblob);
    tilenumber++;

    while (! tilemap.eof() )
    {

        getline(tilemap,testingx);
        getline(tilemap,testingy);
        getline(tilemap,soort);
        getline(tilemap,hasblob);

        blok[tilenumber].setDimensions(32,32);
        blok[tilenumber].setPosition(stringToInt(testingx),stringToInt(testingy));
        blok[tilenumber].moving = stringToInt(soort);
        blok[tilenumber].hasblob = stringToInt(hasblob);

        tilenumber++;
    }
    if (tilenumber > 1){goaldefined = true;}

}

void StateTileEditor::userInput()
{
    input->update();
    #ifdef PLATFORM_PC
    if(input->isQuit())
    nullifyState();
    #endif

    if (inputlimiter.getScaledTicks() > 1)
    {
        // move the "cursor"
        if (input->isLeft()){scope.x = (tilesprite.getX() - 32);inputlimiter.start();}
        if (input->isRight()){scope.x  = (tilesprite.getX() + 32);inputlimiter.start();}
        if (input->isUp()){scope.y = (tilesprite.getY() - 32);inputlimiter.start();}
        if (input->isDown()){scope.y = (tilesprite.getY() + 32);inputlimiter.start();}

        // set tile placement and other attributes

        if (input->isB())
        {
            del = 0;
            if (temptiletype == -1)
            {
                goal.setPosition(tilesprite.getPosition());
                del = 3;

            }

            else
            {
                for (int i = 0; i < tilenumber;i++)
                {

                    if (blok[i].getX() == tilesprite.getX() && blok[i].getY() == tilesprite.getY()) // if tile exists, delete it
                    {
                        blok[i].setX(blok[tilenumber - 1].getX());
                        blok[i].setY(blok[tilenumber - 1].getY());
                        blok[i].hasblob = blok[tilenumber - 1].hasblob;
                        blok[i].moving = blok[tilenumber - 1].moving;
                        tilenumber = tilenumber - 1;
                        blok[tilenumber].setPosition(0,0);
                        blok[tilenumber].moving = 0;
                        blok[tilenumber].hasblob = 0;
                        del = 1;
                    }

                }
            }
            // otherwise set it

            if (del == 0)
            {
            if (goaldefined == false){goaldefined = true;}
            blok[tilenumber].setX(tilesprite.getX());
            blok[tilenumber].setY(tilesprite.getY());
            blok[tilenumber].hasblob = withblob;
            blok[tilenumber].moving = temptiletype;

            tilenumber = tilenumber + 1;


            }
            inputlimiter.start();
        }

        if (input->isX()) //change movement
        {

            if (temptiletype < 5){temptiletype = temptiletype + 1;}
            inputlimiter.start();
        }
        if (input->isY()) //change movement
        {
            if (temptiletype > -1){temptiletype = temptiletype - 1;}

            inputlimiter.start();
        }

        if (input->isR()) // Blob or not
        {
            if (withblob == 0){withblob = 1;}
            else if (withblob == 1){withblob = 2;}
            else if (withblob == 2){withblob = 0;}
            inputlimiter.start();
        }
        if (input->isL()) // quit
        {
            variables[0].setInt(1);
            setNextState(STATE_MAIN);
        }

        if (input->isStart()) // Save
        {

            ofstream tilemap ("scripts/Jumper/fifth.map");
            if (tilemap.is_open())
            {

                    tilemap << goal.getX();
                    tilemap << "\n";
                    tilemap << goal.getY();
                    tilemap << "\n";
                    tilemap << "0";
                    tilemap << "\n";
                    tilemap << "0";
                    tilemap << "\n";



                    for (int i = 0; i < tilenumber - 1; i++)
                    {
                        tilemap << blok[i].getX();
                        tilemap << "\n";
                        tilemap << blok[i].getY();
                        tilemap << "\n";
                        tilemap << blok[i].moving;
                        tilemap << "\n";
                        tilemap << blok[i].hasblob;
                        tilemap << "\n";
                    }
                    tilemap.close();

            }


        }
    }


}

#ifdef PENJIN_SDL
void StateTileEditor::render(SDL_Surface *screen)
{
    background.render(screen);

    for (int i = 0;i < tilenumber;i++)  //render tiles
    {
        if (i > 0)
        {
            if (blok[i].moving == 0)
            {
            tilesprite.setPosition(blok[i].getX(), blok[i].getY());
            tilesprite.render(screen);

            }
            if (blok[i].moving == 1)
            {
            tiledown.setPosition(blok[i].getX(), blok[i].getY());
            tiledown.render(screen);
            }
            if (blok[i].moving == 2)
            {
            tileup.setPosition(blok[i].getX(), blok[i].getY() );
            tileup.render(screen);
            }
            if (blok[i].moving == 3)
            {
            tileleft.setPosition(blok[i].getX() , blok[i].getY() );
            tileleft.render(screen);
            }
            if (blok[i].moving == 4)
            {
            tileright.setPosition(blok[i].getX() , blok[i].getY() );
            tileright.render(screen);
            }
            if (blok[i].hasblob == 1)
            {
                blob.setPosition(blok[i].getX(), blok[i].getY() - 32);
                blob.render(screen);
            }
            if (blok[i].hasblob == 2)
            {
                cloud.setPosition(blok[i].getX(), blok[i].getY() - 32);
                cloud.render(screen);
            }



        }
    }
    tilesprite.setPosition(scope.x, scope.y);
    if (goaldefined)
    {

        tilesprite.render(screen);
    }
    else
    {
        goal.setPosition(tilesprite.getPosition());


    }
    goal.render(screen);

    test.setPosition(20,20);
    test.print(screen,  temptiletype);
    if (temptiletype == 0)
    {
        test.print(screen, "still tile");
    }
    if (temptiletype == -1)
    {
        test.print(screen, "Goal");
    }
    if (temptiletype == 1)
    {
        test.print(screen, "Vertical moving tile type 1");
    }
    if (temptiletype == 2)
    {
        test.print(screen, "Vertical moving tile type 2");
    }
    if (temptiletype == 3)
    {
        test.print(screen, "Horizontal moving tile type 1");
    }
    if (temptiletype == 4)
    {
        test.print(screen, "Horizontal moving tile type 2");
    }
    test.setPosition(20,420);
    if (withblob == 0)
    {
        test.print(screen, "without enemy");
    }
    else if (withblob == 1)
    {
        test.print(screen, "with blob");
    }
    else if (withblob == 2)
    {
        test.print(screen, "with cloud");
    }
}
#else
    void StateTileEditor::render()
    {
        background.render();

        for (int i = 0;i < tilenumber;i++)  //render tiles
        {
            if (i > 0)
            {
                if (blok[i].moving == 0)
                {
                tilesprite.setPosition(blok[i].getX(), blok[i].getY());
                tilesprite.render();
                }
                if (blok[i].moving == 1)
                {
                tiledown.setPosition(blok[i].getX(), blok[i].getY());
                tiledown.render();
                }
                if (blok[i].moving == 2)
                {
                tileup.setPosition(blok[i].getX(), blok[i].getY() );
                tileup.render();
                }
                if (blok[i].moving == 3)
                {
                tileleft.setPosition(blok[i].getX() , blok[i].getY() );
                tileleft.render();
                }
                if (blok[i].moving == 4)
                {
                tileright.setPosition(blok[i].getX() , blok[i].getY() );
                tileright.render();
                }
                if (blok[i].hasblob == 1)
                {
                    blob.setPosition(blok[i].getX(), blok[i].getY() - 32);
                    blob.render();
                }
                if (blok[i].hasblob == 2)
                {
                    cloud.setPosition(blok[i].getX(), blok[i].getY() - 32);
                    cloud.render();
                }
            }
        }
        tilesprite.setPosition(scope.x, scope.y);
        if (goaldefined)
        {

            tilesprite.render();
        }
        else
        {
            goal.setPosition(tilesprite.getPosition());


        }
        goal.render();

        test.setPosition(20,20);
        test.print(temptiletype);
        if (temptiletype == 0)
        {
            test.print( "still tile");
        }
        if (temptiletype == -1)
        {
            test.print( "Goal");
        }
        if (temptiletype == 1)
        {
            test.print("Vertical Random moving tiles");
        }
        if (temptiletype == 2)
        {
            test.print("Vertical in sync tiles");
        }
        if (temptiletype == 3)
        {
            test.print("Horizontal Random moving tiles");
        }
        if (temptiletype == 4)
        {
            test.print("Horizontal platform tiles");
        }
        test.setPosition(20,420);
        if (withblob == 0)
        {
            test.print("without enemy");
        }
        else if (withblob == 1)
        {
            test.print("with blob");
        }
        else if (withblob == 2)
        {
            test.print("with cloud");
        }
    }
#endif

void StateTileEditor::update()
{


}
