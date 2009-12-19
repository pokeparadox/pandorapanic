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

    tempR = new AchievementReset;
    tempR->setName("Only lucky");
    tempR->setDescription("Win %l games in a row in Panic or Ordered gamemode");
    tempR->setIcon("images/Achievements/test.png");
    prop = new vector<SpecialProperty>;
    prop->push_back(special("GAMEMODE",2,coSMALLER));
    tempR->addEventSpecial("GAME_WIN",*prop,1,coEQUAL,acINCREASE);
    tempR->addEvent("GAME_FAIL",1,coEQUAL,acRESET);
    tempR->setLimit(10);
    ACHIEVEMENTS->addAchievement(tempR);

    tempR = new AchievementReset;
    tempR->setName("Getting better");
    tempR->setDescription("Win %l games in a row in Panic or Ordered gamemode");
    tempR->setIcon("images/Achievements/test.png");
    prop = new vector<SpecialProperty>;
    prop->push_back(special("GAMEMODE",2,coSMALLER));
    tempR->addEventSpecial("GAME_WIN",*prop,1,coEQUAL,acINCREASE);
    tempR->addEvent("GAME_FAIL",1,coEQUAL,acRESET);
    tempR->setLimit(20);
    ACHIEVEMENTS->addAchievement(tempR);

    tempR = new AchievementReset;
    tempR->setName("AWESOME!");
    tempR->setDescription("Win %l games in a row in Panic or Ordered gamemode");
    tempR->setIcon("images/Achievements/test.png");
    prop = new vector<SpecialProperty>;
    prop->push_back(special("GAMEMODE",2,coSMALLER));
    tempR->addEventSpecial("GAME_WIN",*prop,1,coEQUAL,acINCREASE);
    tempR->addEvent("GAME_FAIL",1,coEQUAL,acRESET);
    tempR->setLimit(30);
    ACHIEVEMENTS->addAchievement(tempR);

    tempB = new AchievementBoolean;
    tempB->setName("Slasher");
    tempB->setDescription("Hit %l enemies at once in Arena");
    tempB->setIcon("images/Achievements/test.png");
    tempB->setTimeLimit(250);
    tempB->addEvent("ARENA_HIT_ENEMY",0,coGREATER,acINCREASE);
    tempB->setLimit(3);
    ACHIEVEMENTS->addAchievement(tempB);

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

    tempB = new AchievementBoolean;
    tempB->setName("First Jumper Goal");
    tempB->setDescription("first map in minimal jumps");
    tempB->setIcon("images/Achievements/test.png");
    tempB->addEvent("JUMPER1",1,coEQUAL,acINCREASE);
    tempB->setLimit(1);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("Second Jumper Goal");
    tempB->setDescription("Second map in minimal jumps");
    tempB->setIcon("images/Achievements/test.png");
    tempB->addEvent("JUMPER2",1,coEQUAL,acINCREASE);
    tempB->setLimit(1);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("Third Jumper Goal");
    tempB->setDescription("Third map in minimal jumps");
    tempB->setIcon("images/Achievements/test.png");
    tempB->addEvent("JUMPER3",1,coEQUAL,acINCREASE);
    tempB->setLimit(1);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("Fourth Jumper Goal");
    tempB->setDescription("Fourth map in minimal jumps");
    tempB->setIcon("images/Achievements/test.png");
    tempB->addEvent("JUMPER4",1,coEQUAL,acINCREASE);
    tempB->setLimit(1);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("Fifth Jumper Goal");
    tempB->setDescription("Last map in minimal jumps");
    tempB->setIcon("images/Achievements/test.png");
    tempB->addEvent("JUMPER5",1,coEQUAL,acINCREASE);
    tempB->setLimit(1);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("Close call");
    tempB->setDescription("Land on the pad with no fuel left");
    tempB->setIcon("images/Achievements/test.png");
    tempB->addEvent("PLANDER_FUEL_WIN",0,coEQUAL,acINCREASE);
    tempB->setLimit(1);
    ACHIEVEMENTS->addAchievement(tempB);

    tempR = new AchievementReset;
    tempR->setName("Reflex-man!");
    tempR->setDescription("Get a %l hit-streak in Reflex");
    tempR->setIcon("images/Achievements/test.png");
    tempR->addEvent("REFLEX_HIT",0,coGREATER,acINCREASE_COUNT);
    tempR->addEvent("GAME_START",1,coEQUAL,acRESET);
    tempR->setShowProgress(false);
    tempR->setLimit(12);
    ACHIEVEMENTS->addAchievement(tempR);

    // secret achievements

    tempR2 = new AchievementReset2;
    tempR2->setName("Collectible-love");
    tempR2->setDescription("Get the collectible, but lose the level");
    tempR2->setIcon("images/Achievements/test.png");
    tempR2->setSecret(true);
    tempR2->addEvent("PLANDER_COLLECTIBLE_GET",1,coEQUAL,acINCREASE);
    tempR2->addEvent("GAME_FAIL",1,coEQUAL,acINCREASE);
    tempR2->addEvent("GAME_START",1,coEQUAL,acRESET);
    tempR2->setLimit(1);
    ACHIEVEMENTS->addAchievement(tempR2);

    // first try to load achievment file
    if(ACHIEVEMENTS->load("achieve.ach") != PENJIN_OK)
        ACHIEVEMENTS->save("achieve.ach");

#endif //ACHIEVEMENTSSETUP_H_INCLUDED
