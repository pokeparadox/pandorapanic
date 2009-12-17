#ifndef ACHIEVEMENTSSETUP_H_INCLUDED
#define ACHIEVEMENTSSETUP_H_INCLUDED

#include "AchievementSystem.h"

    ACHIEVEMENTS->setOffset(250,10);
    ACHIEVEMENTS->setPopupPosition(ppBOTTOMCENTER);
    ACHIEVEMENTS->setPopupFadeTime(500);
    ACHIEVEMENTS->setPopupShowTime(2000);

    AchievementReset* tempR = NULL;
    AchievementCount* tempC = NULL;
    AchievementBoolean* tempB = NULL;


    tempC = new AchievementCount;
    tempC->setName("Winner");
    tempC->setDescription("Win one thousand games!");
    tempC->setIcon("images/Achievements/test.png");
    tempC->addEvent("WIN",0,coGREATER,acINCREASE);
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
    tempB->setDescription("Don't land on  a platform and win");
    tempB->setIcon("images/Achievements/highflyer.png");
    tempB->setTimeLimit(250);
    tempB->addEvent("JOUST_FINISH",0,coGREATER,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("Maze Sneakthrough");
    tempB->setDescription("Close ghost pass at high speed");
    tempB->setIcon("images/Achievements/test.png");
    tempB->setTimeLimit(250);
    tempB->addEvent("MAZE_SNEAKTHROUGH",0,coGREATER,acINCREASE);
    //tempC->setSecret(true);
    ACHIEVEMENTS->addAchievement(tempB);

    tempC = new AchievementCount;
    tempC->setName("First Jumper Goal");
    tempC->setDescription("first map in minimal jumps");
    tempC->setIcon("images/Achievements/test.png");
    tempC->setTimeLimit(250);
    tempC->addEvent("JUMPER1",0,coGREATER,acINCREASE);
    tempC->setLimit(1);
    ACHIEVEMENTS->addAchievement(tempC);

    tempC = new AchievementCount;
    tempC->setName("Second Jumper Goal");
    tempC->setDescription("Second map in minimal jumps");
    tempC->setIcon("images/Achievements/test.png");
    tempC->setTimeLimit(250);
    tempC->addEvent("JUMPER2",0,coGREATER,acINCREASE);
    tempC->setLimit(1);
    ACHIEVEMENTS->addAchievement(tempC);

    tempC = new AchievementCount;
    tempC->setName("Third Jumper Goal");
    tempC->setDescription("Third map in minimal jumps");
    tempC->setIcon("images/Achievements/test.png");
    tempC->setTimeLimit(250);
    tempC->addEvent("JUMPER3",0,coGREATER,acINCREASE);
    tempC->setLimit(1);
    ACHIEVEMENTS->addAchievement(tempC);

    tempC = new AchievementCount;
    tempC->setName("Fourth Jumper Goal");
    tempC->setDescription("Fourth map in minimal jumps");
    tempC->setIcon("images/Achievements/test.png");
    tempC->setTimeLimit(250);
    tempC->addEvent("JUMPER4",0,coGREATER,acINCREASE);
    tempC->setLimit(1);
    ACHIEVEMENTS->addAchievement(tempC);

    tempC = new AchievementCount;
    tempC->setName("Fifth Jumper Goal");
    tempC->setDescription("Last map in minimal jumps");
    tempC->setIcon("images/Achievements/test.png");
    tempC->setTimeLimit(250);
    tempC->addEvent("JUMPER5",0,coGREATER,acINCREASE);
    tempC->setLimit(1);
    ACHIEVEMENTS->addAchievement(tempC);


    tempR = new AchievementReset;
    tempR->setName("Reflex-man!");
    tempR->setDescription("Get a %l hit-streak in Reflex");
    tempR->setIcon("images/Achievements/test.png");
    tempR->addEvent("REFLEX_HIT",0,coGREATER,acINCREASE_COUNT);
    tempR->addEvent("WIN",0,coGREATER,acRESET);
    tempR->addEvent("FAIL",0,coGREATER,acRESET);
    tempR->setLimit(12);
    ACHIEVEMENTS->addAchievement(tempR);

    // first try to load achievment file
    if(ACHIEVEMENTS->load("achieve.ach") != PENJIN_OK)
        ACHIEVEMENTS->save("achieve.ach");

#endif //ACHIEVEMENTSSETUP_H_INCLUDED
