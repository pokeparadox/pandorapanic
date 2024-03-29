#ifndef ACHIEVEMENTSSETUP_H_INCLUDED
#define ACHIEVEMENTSSETUP_H_INCLUDED

#include "AchievementSystem.h"

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

    tempB = new AchievementBoolean;
    tempB->setName("Hi-hi-hiiiighscore");
    tempB->setDescription("Get a score of at least 50 points");
    tempB->setIcon("images/Achievements/highscore.png");
    tempB->addEvent("GAME_SCORE",49,coGREATER,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempR = new AchievementReset;
    tempR->setName("Only lucky");
    tempR->setDescription("Win %l games in a row in Panic or Ordered gamemode");
    tempR->setIcon("images/Achievements/only-lucky.png");
    prop = new vector<SpecialProperty>;
    prop->push_back(SpecialProperty("GAMEMODE",2,coSMALLER));
    tempR->addEventSpecial("GAME_WIN",prop,1,coEQUAL,acINCREASE);
    tempR->addEvent("GAME_FAIL",1,coEQUAL,acRESET);
    tempR->setLimit(5);
    ACHIEVEMENTS->addAchievement(tempR);

    tempR = new AchievementReset;
    tempR->setName("Getting better");
    tempR->setDescription("Win %l games in a row in Panic or Ordered gamemode");
    tempR->setIcon("images/Achievements/getting-better.png");
    prop = new vector<SpecialProperty>;
    prop->push_back(SpecialProperty("GAMEMODE",2,coSMALLER));
    tempR->addEventSpecial("GAME_WIN",prop,1,coEQUAL,acINCREASE);
    tempR->addEvent("GAME_FAIL",1,coEQUAL,acRESET);
    tempR->setLimit(10);
    ACHIEVEMENTS->addAchievement(tempR);

    tempR = new AchievementReset;
    tempR->setName("AWESOME!");
    tempR->setDescription("Win %l games in a row in Panic or Ordered gamemode");
    tempR->setIcon("images/Achievements/awesome.png");
    prop = new vector<SpecialProperty>;
    prop->push_back(SpecialProperty("GAMEMODE",2,coSMALLER));
    tempR->addEventSpecial("GAME_WIN",prop,1,coEQUAL,acINCREASE);
    tempR->addEvent("GAME_FAIL",1,coEQUAL,acRESET);
    tempR->setLimit(20);
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
    tempB->setIcon("images/Achievements/sneakthrough.png");
    tempB->addEvent("MAZE_SNEAKTHROUGH",1,coEQUAL,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("A-Maze-ing");
    tempB->setDescription("Finish Maze in under 10 seconds");
    tempB->setIcon("images/Achievements/amazing.png");
    tempB->addEvent("MAZE_WIN",10,coSMALLER,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("First Jumper Goal");
    tempB->setDescription("Finish the first map in minimal jumps");
    tempB->setIcon("images/Achievements/jumper-1.png");
    tempB->addEvent("JUMPER1",1,coEQUAL,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("Second Jumper Goal");
    tempB->setDescription("Finish the second map in minimal jumps");
    tempB->setIcon("images/Achievements/jumper-1.png");
    tempB->addEvent("JUMPER2",1,coEQUAL,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("Third Jumper Goal");
    tempB->setDescription("Finish the third map in minimal jumps");
    tempB->setIcon("images/Achievements/jumper-1.png");
    tempB->addEvent("JUMPER3",1,coEQUAL,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("Fourth Jumper Goal");
    tempB->setDescription("Finish the fourth map in minimal jumps");
    tempB->setIcon("images/Achievements/jumper-1.png");
    tempB->addEvent("JUMPER4",1,coEQUAL,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("Fifth Jumper Goal");
    tempB->setDescription("Finish the last map in minimal jumps");
    tempB->setIcon("images/Achievements/jumper-1.png");
    tempB->addEvent("JUMPER5",1,coEQUAL,acINCREASE);
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
    tempC->setIcon("images/Achievements/firefighter.png");
    tempC->addEvent("FIRE_SHOOT",0,coGREATER,acINCREASE_COUNT);
    tempC->setLimit(100000);
    tempC->addMilestone(10000);
    tempC->addMilestone(20000);
    tempC->addMilestone(30000);
    tempC->addMilestone(40000);
    tempC->addMilestone(50000);
    tempC->addMilestone(60000);
    tempC->addMilestone(70000);
    tempC->addMilestone(80000);
    tempC->addMilestone(90000);
    ACHIEVEMENTS->addAchievement(tempC);

    tempB = new AchievementBoolean;
    tempB->setName("Eagle-eye");
    tempB->setDescription("Spot the correct cup after 50 or more shuffles!");
    tempB->setIcon("images/Achievements/eagle-eye.png");
    prop = new vector<SpecialProperty>;
    prop->push_back(SpecialProperty("LEVELNUMBER",49,coGREATER));
    prop->push_back(SpecialProperty("MINIGAME",24,coEQUAL));
    tempB->addEventSpecial("GAME_WIN",prop,1,coEQUAL,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("B/W-Frenzy");
    tempB->setDescription("Win TheCountJob %l times in highest level (>30) and colourblind mode");
    tempB->setIcon("images/Achievements/bw-frenzy.png");
    prop = new vector<SpecialProperty>;
    prop->push_back(SpecialProperty("CIRCLELEVEL",5,coEQUAL));
    prop->push_back(SpecialProperty("DIFFLEVEL",10,coEQUAL));
    prop->push_back(SpecialProperty("TIMELEVEL",30,coEQUAL));
    prop->push_back(SpecialProperty("COLOURBLIND",1,coEQUAL));
    tempB->addEventSpecial("COUNT_WIN",prop,1,coEQUAL,acINCREASE);
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

    tempL = new AchievementList;
    tempL->setName("Second hand beer");
    tempL->setDescription("Snatch the beer in the second chance (level 20 minimum)");
    tempL->setIcon("images/Achievements/second-hand-snatch.png");
    prop = new vector<SpecialProperty>;
    prop->push_back(SpecialProperty("LEVELNUMBER",19,coGREATER));
    prop->push_back(SpecialProperty("MINIGAME",20,coEQUAL));
    tempL->addEventSpecial("GAME_START",prop,1,coEQUAL,acINCREASE);
    tempL->addEvent("SNATCH_SECOND",1,coEQUAL,acINCREASE);
    tempL->addEvent("GAME_WIN",1,coEQUAL,acRESET);
    tempL->addEvent("GAME_FAIL",1,coEQUAL,acRESET);
    ACHIEVEMENTS->addAchievement(tempL);

    tempB = new AchievementBoolean;
    tempB->setName("Perfect Putt");
    tempB->setDescription("Shoot exactly at the center of the green block");
    tempB->setIcon("images/Achievements/perfect-putt.png");
    tempB->addEvent("PERFECT_PUTT",1,coEQUAL,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("Superior species?");
    tempB->setDescription("Shoot the alien before it can advance a row");
    tempB->setIcon("images/Achievements/superior-species.png");
    tempB->addEvent("SPACE_WIN",1,coEQUAL,acRESET);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("Mastermind");
    tempB->setDescription("Enter the correct code with still 5 seconds left");
    tempB->setIcon("images/Achievements/mastermind.png");
    prop = new vector<SpecialProperty>;
    prop->push_back(SpecialProperty("TIME",4999,coGREATER));
    prop->push_back(SpecialProperty("WIN",1,coEQUAL));
    tempB->addEventSpecial("MEMORY_END",prop,1,coEQUAL,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    AchievementWinAll* winAllA = new AchievementWinAll;
    winAllA->setName("OCD");
    winAllA->setDescription("Win every minigame at least once!");
    winAllA->setIcon("images/Achievements/win-all.png");
    winAllA->addEvent("GAME_WIN",1,coEQUAL,acINCREASE);
    winAllA->setAmount(24);
    ACHIEVEMENTS->addAchievement(winAllA);

    winAllA = new AchievementWinAll;
    winAllA->setName("Achievement whore");
    winAllA->setDescription("Unlock every achievement (except this one)!");
    winAllA->setIcon("images/Achievements/achievement-whore.png");
    winAllA->addEvent("ACHIEVEMENT_UNLOCK",1,coEQUAL,acINCREASE);
    winAllA->setAmount(29);
    ACHIEVEMENTS->addAchievement(winAllA);

    // secret achievements

    tempL = new AchievementList;
    tempL->setName("Collectible-love");
    tempL->setDescription("Get the collectible, but lose the level");
    tempL->setIcon("images/Achievements/collectible-love.png");
    tempL->setSecret(true);
    tempL->addEvent("PLANDER_COLLECTIBLE_GET",1,coEQUAL,acINCREASE);
    tempL->addEvent("GAME_FAIL",1,coEQUAL,acINCREASE);
    tempL->addEvent("GAME_START",1,coEQUAL,acRESET);
    ACHIEVEMENTS->addAchievement(tempL);

    tempB = new AchievementBoolean;
    tempB->setName("Close call");
    tempB->setDescription("Land on the pad with no fuel left");
    tempB->setIcon("images/Achievements/close-call.png");
    tempB->setSecret(true);
    tempB->addEvent("PLANDER_FUEL_WIN",0,coEQUAL,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("In the nick of time");
    tempB->setDescription("In RotatingTurrent shoot the bomb very close to your turret");
    tempB->setIcon("images/Achievements/nick-of-time.png");
    tempB->setSecret(true);
    tempB->addEvent("ROTDEF_WIN",95,coSMALLER,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("Rocket science");
    tempB->setDescription("Overfill the fuel-meter and get an extra life");
    tempB->setIcon("images/Achievements/rocket-science.png");
    tempB->setSecret(true);
    tempB->addEvent("PLANDER_OVERFILL",1,coEQUAL,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);

    tempB = new AchievementBoolean;
    tempB->setName("Love is in the air");
    tempB->setDescription("Form a code only consisting of hearts");
    tempB->setIcon("images/Achievements/love-air.png");
    tempB->setSecret(true);
    prop = new vector<SpecialProperty>;
    prop->push_back(SpecialProperty("HEARTS",1,coEQUAL));
    tempB->addEventSpecial("MEMORY_END",prop,1,coEQUAL,acINCREASE);
    ACHIEVEMENTS->addAchievement(tempB);


    // first try to load achievment file
    if(ACHIEVEMENTS->load() != PENJIN_OK)
        ACHIEVEMENTS->save();

#endif //ACHIEVEMENTSSETUP_H_INCLUDED
