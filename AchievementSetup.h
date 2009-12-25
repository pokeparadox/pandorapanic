#ifndef ACHIEVEMENTSSETUP_H_INCLUDED
#define ACHIEVEMENTSSETUP_H_INCLUDED

#include "AchievementSystem.h"

    ACHIEVEMENTS->setOffset(250,50);
    ACHIEVEMENTS->setSpacing(3);
    ACHIEVEMENTS->setPopupPosition(ppBOTTOMCENTER);
    ACHIEVEMENTS->setPopupFadeTime(500);
    ACHIEVEMENTS->setPopupShowTime(2000);

    AchievementBoolean* tempB = NULL;
    AchievementReset* tempR = NULL;
    AchievementList* tempL = NULL;
    AchievementCount* tempC = NULL;
    vector<SpecialProperty>* prop = NULL;

    tempC = new AchievementCount;
    tempC->setName("Winner");
    tempC->setDescription("Win one thousand games!");
    tempC->setIcon("images/Achievements/win1000.png");
    tempC->addEvent("GAME_WIN",1,coEQUAL,acINCREASE);
    tempC->setLimit(1000);
    tempC->addMilestone(250);
    tempC->addMilestone(500);
    tempC->addMilestone(750);
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
    tempB->setIcon("images/Achievements/slasher.png");
    tempB->setTimeLimit(250);
    tempB->addEvent("ARENA_HIT_ENEMY",0,coGREATER,acINCREASE);
    tempB->setLimit(3);
    tempB->setShowProgress(false);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("High Flyer");
    tempB->setDescription("Don't land on a platform and win Joust");
    tempB->setIcon("images/Achievements/highflyer.png");
    tempB->addEvent("JOUST_FINISH",1,coEQUAL,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("Sneakthrough");
    tempB->setDescription("Close ghost pass at high speed in Maze");
    tempB->setIcon("images/Achievements/test.png");
    tempB->addEvent("MAZE_SNEAKTHROUGH",1,coEQUAL,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("First Jumper Goal");
    tempB->setDescription("Finish the first map in minimal jumps");
    tempB->setIcon("images/Achievements/test.png");
    tempB->addEvent("JUMPER1",1,coEQUAL,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("Second Jumper Goal");
    tempB->setDescription("Finish the second map in minimal jumps");
    tempB->setIcon("images/Achievements/test.png");
    tempB->addEvent("JUMPER2",1,coEQUAL,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("Third Jumper Goal");
    tempB->setDescription("Finish the third map in minimal jumps");
    tempB->setIcon("images/Achievements/test.png");
    tempB->addEvent("JUMPER3",1,coEQUAL,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("Fourth Jumper Goal");
    tempB->setDescription("Finish the fourth map in minimal jumps");
    tempB->setIcon("images/Achievements/test.png");
    tempB->addEvent("JUMPER4",1,coEQUAL,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("Fifth Jumper Goal");
    tempB->setDescription("Finish the last map in minimal jumps");
    tempB->setIcon("images/Achievements/test.png");
    tempB->addEvent("JUMPER5",1,coEQUAL,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("Close call");
    tempB->setDescription("Land on the pad with no fuel left (Plander)");
    tempB->setIcon("images/Achievements/test.png");
    tempB->addEvent("PLANDER_FUEL_WIN",0,coEQUAL,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempR = new AchievementReset;
    tempR->setName("Reflex-man!");
    tempR->setDescription("Get a %l hit-streak in Reflex");
    tempR->setIcon("images/Achievements/reflex-man.png");
    tempR->addEvent("REFLEX_HIT",0,coGREATER,acINCREASE_COUNT);
    tempR->addEvent("GAME_START",1,coEQUAL,acRESET);
    tempR->setLimit(12);
    ACHIEVEMENTS->addAchievement(tempR);

    tempC = new AchievementCount;
    tempC->setName("Firefighter");
    tempC->setDescription("Shoot %l litres of water in Fire");
    tempC->setIcon("images/Achievements/test.png");
    tempC->addEvent("FIRE_SHOOT",0,coGREATER,acINCREASE_COUNT);
    tempC->setLimit(20000);
    tempC->addMilestone(5000);
    tempC->addMilestone(10000);
    tempC->addMilestone(15000);
    ACHIEVEMENTS->addAchievement(tempC);

    tempB = new AchievementBoolean;
    tempB->setName("Eagle-eye");
    tempB->setDescription("Win TheCountJob %l times in highest level and colourblind mode");
    tempB->setIcon("images/Achievements/eagle-eye.png");
    prop = new vector<SpecialProperty>;
    prop->push_back(special("CIRCLELEVEL",5,coEQUAL));
    prop->push_back(special("DIFFLEVEL",10,coEQUAL));
    prop->push_back(special("TIMELEVEL",30,coEQUAL));
    prop->push_back(special("COLOURBLIND",1,coEQUAL));
    tempB->addEventSpecial("COUNT_WIN",*prop,1,coEQUAL,acINCREASE);
    tempB->setLimit(10);
    tempB->addMilestone(5);
    ACHIEVEMENTS->addAchievement(tempB);

    tempR = new AchievementReset;
    tempR->setName("Pong forever");
    tempR->setDescription("Return the ball %l times in one game of Pong");
    tempR->setIcon("images/Achievements/pong-forever.png");
    tempR->addEvent("PONG_HIT",1,coEQUAL,acINCREASE);
    tempR->addEvent("GAME_START",1,coEQUAL,acRESET);
    tempR->setLimit(25);
    tempR->addMilestone(10);
    tempR->addMilestone(20);
    ACHIEVEMENTS->addAchievement(tempR);

    tempB = new AchievementBoolean;
    tempB->setName("In the nick of time");
    tempB->setDescription("In RotatingTurrent shoot the bomb very close to your turret");
    tempB->setIcon("images/Achievements/nick-of-time.png");
    tempB->addEvent("ROTDEF_WIN",140,coSMALLER,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempL = new AchievementList;
    tempL->setName("Second hand beer");
    tempL->setDescription("Snatch the beer in the second chance (level 20 minimum)");
    tempL->setIcon("images/Achievements/second-hand-snatch.png");
    prop = new vector<SpecialProperty>;
    prop->push_back(special("LEVELNUMBER",19,coGREATER));
    prop->push_back(special("MINIGAME",20,coEQUAL));
    tempL->addEventSpecial("GAME_START",*prop,1,coEQUAL,acINCREASE);
    tempL->addEvent("SNATCH_SECOND",1,coEQUAL,acINCREASE);
    tempL->addEvent("GAME_WIN",1,coEQUAL,acRESET);
    tempL->addEvent("GAME_FAIL",1,coEQUAL,acRESET);
    ACHIEVEMENTS->addAchievement(tempL);

    tempB = new AchievementBoolean;
    tempB->setName("Perfect Putt");
    tempB->setDescription("Shoot exactly at the center of the green block");
    tempB->setIcon("images/Achievements/test.png");
    tempB->addEvent("PERFECT_PUTT",1,coEQUAL,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);


    // secret achievements

    tempL = new AchievementList;
    tempL->setName("Collectible-love");
    tempL->setDescription("Get the collectible, but lose the level");
    tempL->setIcon("images/Achievements/collectible-love.png");
    tempL->setSecret(true);
    tempL->addEvent("PLANDER_COLLECTIBLE_GET",1,coEQUAL,acINCREASE);
    tempL->addEvent("GAME_FAIL",1,coEQUAL,acINCREASE);
    tempL->addEvent("GAME_START",1,coEQUAL,acRESET);
    tempL->setLimit(1);
    ACHIEVEMENTS->addAchievement(tempL);



    // first try to load achievment file
    if(ACHIEVEMENTS->load("achieve.ach") != PENJIN_OK)
        ACHIEVEMENTS->save("achieve.ach");

#endif //ACHIEVEMENTSSETUP_H_INCLUDED
