#include "AchievementWinAll.h"

#include "AchievementSystem.h"

#define PARENT_SYSTEM ((AchievementSystem*)parent)

///------------------------------
/// Constructor / Destructor
///------------------------------

AchievementWinAll::AchievementWinAll() : AchievementList()
{
    type = atCUSTOM;
    showProgress = true;
    doneCount = 0;
    eCount = 0;
}


///------------------------------
/// Public
///------------------------------
void AchievementWinAll::setAmount(CRint value)
{
    for (int I = value - doneEvents.size(); I > 0; --I)
    {
        doneEvents.push_back(false);
        eCount++;
    }
}

///------------------------------
/// Private
///------------------------------

void AchievementWinAll::changeCount(const vector<Event*>& changeEvents)
{
    int game = 0;

    // determine kind of this achievement
    string ident = "";
    if ((*changeEvents.begin())->name == "GAME_WIN")
        ident = "MINIGAME";  // win all games
    else
        ident = "INDEX";    // unlock all achievements

    // find game number
    vector<SpecialProperty>::iterator I;
    for (I = (*changeEvents.begin())->special->begin(); I < (*changeEvents.begin())->special->end(); ++I)
    {
        if (I->property == ident)
            game = I->value;
    }

    // check whether already done
    if (!doneEvents.at(game-1))
    {
        doneEvents.at(game-1) = true;
        ++doneCount;
    }

    if (doneCount >= eCount)
    {
        ++count;
    }
}

