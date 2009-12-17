#ifndef ACHIEVEMENTSSETUP_H_INCLUDED
#define ACHIEVEMENTSSETUP_H_INCLUDED

#include "AchievementSystem.h"

    ACHIEVEMENTS->setOffset(250,35);
    ACHIEVEMENTS->setSpacing(3);
    ACHIEVEMENTS->setPopupPosition(ppBOTTOMCENTER);
    ACHIEVEMENTS->setPopupFadeTime(500);
    ACHIEVEMENTS->setPopupShowTime(2000);

    AchievementBoolean* tempB = NULL;
    AchievementReset* tempR = NULL;
    AchievementReset2* tempR2 = NULL;
    AchievementCount* tempC = NULL;
    vector<SpecialProperty>* prop = NULL;

    tempC = new AchievementCount;
    tempC->setName("Winner");
    tempC->setDescription("Win one thousand games!");
    tempC->setIcon("images/Achievements/test.png");
    tempC->addEvent("GAME_WIN",1,coEQUAL,acINCREASE);
    tempC->setLimit(1000);
    ACHIEVEMENTS->addAchievement(tempC);

    tempC = new AchievementCount;
    tempC->setName("Slasher");
    tempC->setDescription("Hit %l enemies at once in Arena");
    tempC->setIcon("images/Achievements/test.png");
    tempC->setTimeLimit(250);
    tempC->addEvent("ARENA_HIT_ENEMY",0,coGREATER,acINCREASE);
    tempC->setLimit(3);
    ACHIEVEMENTS->addAchievement(tempC);

    tempB = new AchievementBoolean;
    tempB->setName("Joust High Flyer");
    tempB->setDescription("Don't land on a platform and win");
    tempB->setIcon("images/Achievements/highflyer.png");
    tempB->addEvent("JOUST_FINISH",1,coEQUAL,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("Maze Sneakthrough");
    tempB->setDescription("Close ghost pass at high speed");
    tempB->setIcon("images/Achievements/test.png");
    tempB->addEvent("MAZE_SNEAKTHROUGH",1,coEQUAL,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempC = new AchievementCount;
    tempC->setName("First Jumper Goal");
    tempC->setDescription("first map in minimal jumps");
    tempC->setIcon("images/Achievements/test.png");
    tempC->addEvent("JUMPER1",1,coEQUAL,acINCREASE);
    tempC->setLimit(1);
    ACHIEVEMENTS->addAchievement(tempC);

    tempC = new AchievementCount;
    tempC->setName("Second Jumper Goal");
    tempC->setDescription("Second map in minimal jumps");
    tempC->setIcon("images/Achievements/test.png");
    tempC->addEvent("JUMPER2",1,coEQUAL,acINCREASE);
    tempC->setLimit(1);
    ACHIEVEMENTS->addAchievement(tempC);

    tempC = new AchievementCount;
    tempC->setName("Third Jumper Goal");
    tempC->setDescription("Third map in minimal jumps");
    tempC->setIcon("images/Achievements/test.png");
    tempC->addEvent("JUMPER3",1,coEQUAL,acINCREASE);
    tempC->setLimit(1);
    ACHIEVEMENTS->addAchievement(tempC);

    tempC = new AchievementCount;
    tempC->setName("Fourth Jumper Goal");
    tempC->setDescription("Fourth map in minimal jumps");
    tempC->setIcon("images/Achievements/test.png");
    tempC->addEvent("JUMPER4",1,coEQUAL,acINCREASE);
    tempC->setLimit(1);
    ACHIEVEMENTS->addAchievement(tempC);

    tempC = new AchievementCount;
    tempC->setName("Fifth Jumper Goal");
    tempC->setDescription("Last map in minimal jumps");
    tempC->setIcon("images/Achievements/test.png");
    tempC->addEvent("JUMPER5",1,coEQUAL,acINCREASE);
    tempC->setLimit(1);
    ACHIEVEMENTS->addAchievement(tempC);


    tempR = new AchievementReset;
    tempR->setName("Reflex-man!");
    tempR->setDescription("Get a %l hit-streak in Reflex");
    tempR->setIcon("images/Achievements/test.png");
    tempR->addEvent("REFLEX_HIT",0,coGREATER,acINCREASE_COUNT);
    tempR->addEvent("GAME_WIN",1,coEQUAL,acRESET);
    tempR->addEvent("GAME_FAIL",1,coEQUAL,acRESET);
    tempR->setLimit(12);
    ACHIEVEMENTS->addAchievement(tempR);

    tempR2 = new AchievementReset2;
    tempR2->setName("Collectible-love");
    tempR2->setDescription("Get the collectible, but lose the level");
    tempR2->setIcon("images/Achievements/test.png");
    tempR2->setSecret(true);
    tempR2->addEvent("ROCKET_COLLECTIBLE_GET",1,coEQUAL,acINCREASE);
    tempR2->addEvent("GAME_FAIL",1,coEQUAL,acINCREASE);
    tempR2->addEvent("GAME_START",1,coEQUAL,acRESET);
    tempR2->setLimit(1);
    ACHIEVEMENTS->addAchievement(tempR2);

    // first try to load achievment file
    if(ACHIEVEMENTS->load("achieve.ach") != PENJIN_OK)
        ACHIEVEMENTS->save("achieve.ach");

#endif //ACHIEVEMENTSSETUP_H_INCLUDED
