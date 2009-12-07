#include "StateDistractedMath.h"
StateDistractedMath::StateDistractedMath()
{
    nullify = false;
    text.loadFont("font/AlphaMaleModern.ttf", 96);
    text.setColour(Colour(WHITE));
    command.loadFont("font/AlphaMaleModern.ttf",48);
    text.setColour(Colour(GREEN));
    command.setRelativity(false);
    command.setPosition(315,216);
    telfont.loadFont("font/AlphaMaleModern.ttf",180);
    telfont.setColour(Colour(RED));
    pauseText.loadFont("font/bip.ttf", 32);
    pauseText.setColour(WHITE);
    teller.setMode(SECONDS);

    rnd = rand()%4;
    difficulty = 0;

    background0.loadBackground("images/DistractedMaths/back0.png");
    background1.loadBackground("images/DistractedMaths/back1.png");
    background2.loadBackground("images/DistractedMaths/back2.png");
    background3.loadBackground("images/DistractedMaths/back3.png");
    hasWon = false;
    GFX::setClearColour(BLACK);
}


StateDistractedMath::~StateDistractedMath()
{

}

void StateDistractedMath::init()
{
    if(variables.size()<SUBSTATE_TRIGGER)
    {
        backsound.loadMusic("music/DistractedMaths/RickKelsall_Nervous.mp3");
        backsound.setLooping(false);
        backsound.play();
    }

    mathstate = 5;
    oper = 1;
    round = 1;
    tv = SDL_EnableUNICODE(1); //enables unicode and saves the previous state
    inputlimiter.setMode(MILLI_SECONDS);
    inputlimiter.start();
    teller.start();
}

void StateDistractedMath::onPause()
{
    teller.pause();
}

void StateDistractedMath::onResume()
{
    teller.unpause();
}

void StateDistractedMath::userInput()
{

        if (mathstate == 0)
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
                        SDL_JOYBUTTONDOWN:
                        if(event.jbutton.button == 5)// Pandora start button
                            pauseToggle();
                    #endif
                        MathInputUnicode =  (event.key.keysym.unicode); // unicode input
                        #ifdef PENJIN_FIXED
                            MathInputNumeric = MathInputUnicode.intValue<<16 - 48; // numeric is 48 ascii chars lower :)
                        #else
                            MathInputNumeric = MathInputUnicode - 48; // numeric is 48 ascii chars lower :)
                        #endif
                        if (MathInputNumeric == number)  // if entered number matches correct answer
                        {
                            round = round + 1;
                            if (level < 25)
                            {
                                difficulty = 0;
                                if (round == 6)    //  Adjust the value after round to adjust game difficulty. more rounds more difficult.
                                    hasWon = true;
                                if (oper == 0)      // oper is used to switch calculations
                                    oper = oper + 1;
                                else
                                    oper = oper - 1;
                            }
                            else if (level < 50)
                            {
                                difficulty = 1;
                                if (round == 7)    //  Adjust the value after round to adjust game difficulty. more rounds more difficult.
                                    hasWon = true;
                                if (oper == 0)      // oper is used to switch calculations
                                    oper = oper + 1;
                                else
                                    oper = oper - 1;
                            }
                            else
                            {
                                difficulty = 2;
                                if (round == 8)    //  Adjust the value after round to adjust game difficulty. more rounds more difficult.
                                    hasWon = true;
                                if (oper == 0)      // oper is used to switch calculations
                                    oper = oper + 1;
                                else
                                    oper = oper - 1;
                            }
                            teller.start();     //restart counter

                            MathInputNumeric = -1;  // reset input

                            number = rand()%10; // generate random again
                            if (oper == 0)
                            {
                                number2 = rand()%(10 + variables[2].getInt());           //
                                number3 = number2 - number;    // the calculations
                            }                                  //
                            if (oper == 1)                     //
                            {                                  //
                                number2 = (rand()%(int)((3 + variables[2].getInt())*0.5f)) + 1;           //
                                number3 = number2 * number;    //
                            }
                            rnd = rand()%4;     // background switch


                        }
                        else            // if input is incorrect
                        {
                            if (MathInputNumeric >= 10 && MathInputUnicode != 13)   // Generally this should include most
                            {                           // of the pandora's input outside the numbers. not sure
                                teller.start();         // about the buttons though

                                round = 6;
                                mathstate = 1;
                            }
                            else                        // or wrong number
                            {
                                if (MathInputUnicode != 13)
                                {
                                teller.start();
                                losestate = round;      // keep for the loser  message but round is used in closing
                                round = 6;
                                mathstate = 2;
                                }
                            }
                        }
                }
            }
        }


}

void StateDistractedMath::pauseInput()
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

#ifdef PENJIN_SDL
void StateDistractedMath::render(SDL_Surface *screen)
{
	GFX::clearScreen(screen);
    if (rnd == 0)
        background0.render(screen);
    if (rnd == 1)
        background1.render(screen);     // Backgrounds
    if (rnd == 2)
        background2.render(screen);
    if (rnd == 3)
        background3.render(screen);

    if (mathstate == 0)                 // in game
    {
        command.setPosition(430,30);
        command.print(screen,"Round: ");
        command.setPosition(600,30);
        command.print(screen, round);
        telfont.setPosition(460,240);
        telfont.print(screen,(int)9 - round - teller.getScaledTicks());
        quickrand = rand()%10;
        text.setPosition(600,110);
        text.print(screen,quickrand);

        if (oper == 0)
        {
            text.setPosition(80,110);
            text.print(screen,number2);
            text.setPosition(230,110);
            text.print(screen,"-");
            text.setPosition(330,110);
            text.print(screen,number3);
            text.setPosition(450,110);
            text.print(screen," = ");
        }
        if (oper == 1)
        {
            text.setPosition(80,110);
            text.print(screen,number3);
            text.setPosition(230,110);
            text.print(screen,"/");
            text.setPosition(330,110);
            text.print(screen,number2);
            text.setPosition(450,110);
            text.print(screen," = ");
        }
    }

    if (mathstate == 1)                 // text or weird input
    {
        text.setPosition(50,90);
        text.print(screen,"That's not even ");
         text.setPosition(150,230);
        text.print(screen,"a number...");
    }
    if (mathstate == 2)                 // wrong number
    {
        if (losestate > 3)
        {
            telfont.setPosition(110,60);
            telfont.print(screen,"ERROR");
            text.setPosition(60,250);
            text.print(screen,"much pressure...");
        }
        else
        {
            text.setPosition(100,150);
            text.print(screen,"Come On...");
        }
    }

    if (mathstate == 3)                 // out of time
    {
        text.setPosition(100,150);
        text.print(screen,"Time's Up... ");
    }
    if (mathstate == 4)
    {
        text.setPosition(100,150);
        text.print(screen,"Congratulations...");
    }
    if (mathstate == 5)
    {
        text.setPosition(100,150);
        text.print(screen,"ready...");
    }
}
void StateDistractedMath::pauseScreen(SDL_Surface* screen)
{
    if(variables.size()<SUBSTATE_TRIGGER)
        pauseSymbol(screen);
    pauseText.setPosition(50,180);
    pauseText.print(screen, "Use your math magic and the numeric keys");
    pauseText.setPosition(50,220);
    pauseText.print(screen, "to correctly solve the given problems.");
}
#else
void StateDistractedMath::render()
{
    if (rnd == 0)
        background0.render();
    if (rnd == 1)
        background1.render();     // Backgrounds
    if (rnd == 2)
        background2.render();
    if (rnd == 3)
        background3.render();

    if (mathstate == 0)                 // in game
    {
        command.setPosition(430,30);
        command.print("Round: ");
        command.setPosition(600,30);
        command.print(round);
        telfont.setPosition(460,240);
        telfont.print((int)9 - round - teller.getScaledTicks());
        quickrand = rand()%10;
        text.setPosition(600,110);
        text.print(quickrand);

        if (oper == 0)
        {
            text.setPosition(80,110);
            text.print(number2);
            text.setPosition(230,110);
            text.print("-");
            text.setPosition(330,110);
            text.print(number3);
            text.setPosition(450,110);
            text.print(" = ");
        }
        if (oper == 1)
        {
            text.setPosition(80,110);
            text.print(number3);
            text.setPosition(230,110);
            text.print("/");
            text.setPosition(330,110);
            text.print(number2);
            text.setPosition(450,110);
            text.print(" = ");
        }
    }

    if (mathstate == 1)                 // text or weird input
    {
        text.setPosition(50,90);
        text.print("That's not even ");
         text.setPosition(150,230);
        text.print("a number...");
    }
    if (mathstate == 2)                 // wrong number
    {
        if (losestate > 3)
        {
            telfont.setPosition(110,60);
            telfont.print("ERROR");
            text.setPosition(60,250);
            text.print("much pressure...");
        }
        else
        {
            text.setPosition(100,150);
            text.print("Come On...");
        }
    }

    if (mathstate == 3)                 // out of time
    {
        text.setPosition(100,150);
        text.print("Time's Up... ");
    }
    if (mathstate == 4)
    {
        text.setPosition(100,150);
        text.print("Congratulations...");
    }
    if (mathstate == 5)
    {
        text.setPosition(100,150);
        text.print("ready...");
    }
}
void StateDistractedMath::pauseScreen()
{
    pauseSymbol();
    pauseText.setPosition(50,180);
    pauseText.print( "Use your math magic and the numeric keys");
    pauseText.setPosition(50,220);
    pauseText.print( "to correctly solve the given problems.");
}
#endif

void StateDistractedMath::update()
{
        // Hier alle actieve shit
        if (mathstate == 5)
        {
            number = rand()%10;
            number2 = (rand()%9 + 1);
            number2 = rand()%40;

            if (teller.getScaledTicks() == 3)
            {
                input->resetKeys();
                teller.start();
                mathstate = 0;
                number = rand()%10;
                if (oper == 0)
                {
                    number2 = rand()%((5 + variables[2].getInt())*2);

                    number3 = (number2 - number);
                }
                if (oper == 1)
                {
                    number2 = (rand()%(int)((3 + variables[2].getInt())*0.5f)) + 1;
                            //

                    number3 = number2 * number;
                }
            }
        }


        if(teller.getScaledTicks() > (8 - round))
        {
            if (mathstate == 0 )
            {
                mathstate = 3;
                teller.start();
                round = 4;
            }
            else
            {
                variables[0].setInt(0);
                SDL_EnableUNICODE(tv);
                setNextState(STATE_MAIN);
            }
        }
        if(hasWon)
        {
            variables[0].setInt(1);
            SDL_EnableUNICODE(tv);
            setNextState(STATE_MAIN);
        }
}
