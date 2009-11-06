#ifndef MINIGAMESTATES_H_INCLUDED
#define MINIGAMESTATES_H_INCLUDED
#include "PenjinStates.h"
/*
This file is just to stop reduncy in StateManagement and preview rendering.
You now just need to add your minigames here and it should work!
*/

    if(next == STATE_BASE)
		state = new BaseState;
    else if (next == STATE_TITLE)
        state = new StateTitle;
    else if (next == STATE_MAIN)
        state = new StateMain;

    else if (next == STATE_PANG)
        state = new StatePang;
    else if (next == STATE_ARENA)
        state = new StateArena;
    else if (next == STATE_BALLDROP)
        state = new StateBallDrop;
    else if (next == STATE_SPACEINVADERS)
        state = new StateSpaceInvaders;
    else if (next == STATE_SPELLINGCONTEST)
        state = new StateSpellingContest;
    else if (next == STATE_BADVISTA)
        state = new StateBadVista;
    else if (next == STATE_DISTRACTEDMATH)
        state = new StateDistractedMath;
    /*else if(next == STATE_DOUBLENUBTROUBLE)
        state = new StateDoubleNubTrouble;*/
    else if (next == STATE_REFLEX)
        state = new StateReflex;
    else if(next == STATE_PUNCHWRESTLER)
        state = new StatePunchWrestler;
    else if(next == STATE_PONG)
        state = new StatePong;
    else if(next == STATE_ONEBUTTONBANDIT)
        state = new StateOneButtonBandit;
    else if(next == STATE_PANICATTACK)
        state = new StatePanicAttack;
    else if(next == STATE_PANJOUST)
        state = new StatePanJoust;
    else if (next == STATE_JUMPER)
        state = new StateJumper;
    else if (next == STATE_TILEEDITOR)
        state = new StateTileEditor;
    else if (next == STATE_PUTTPUTT)
        state = new StatePuttPutt;
    else if (next == STATE_ROT_DEF)
        state = new StateRotDef;
    else if (next == STATE_MEMORY_BLOCKS)
        state = new StateMemoryBlocks;
    else if (next == STATE_PLANDER)
        state = new StatePlander;
    else if (next == STATE_SNATCHABEER)
        state = new StateSnatchABeer;
    else if(next == STATE_COUNTJOB)
        state = new StateTheCountJob;
    else if (next == STATE_FIRE)
        state = new Fire;
    else if (next == STATE_CAKE)
        state = new StateCake;


    /*else if(next == STATE_ROCKETESCAPE)
        state = new StateRocketEscape;*/
    else if (next == STATE_PUTTPUTT)
        state = new StatePuttPutt;

/*    else if (next == STATE_BARRELROLL)
        state = new StateBarrelRoll;*/

    else if(next == STATE_CREDITS)
        state = new StateCredits;
    else if(next == STATE_GAMEOVER)
        state = new StateGameover;
    else if(next == STATE_GAMEOVERORDERED)
        state = new StateGameoverOrdered;
    else
    {
        cout << ErrorHandler().getErrorString(PENJIN_UNDEFINED_STATE);
        exit(PENJIN_UNDEFINED_STATE);
    }
#endif // MINIGAMESTATES_H_INCLUDED
