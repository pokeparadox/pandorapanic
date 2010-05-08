#ifndef ACHIEVEMENTWINALL_H
#define ACHIEVEMENTWINALL_H

///------------------------------
/// Single. custom achievement for PandoraPanic!
/// Used for win all games and unlock all achievements
///------------------------------

#include "AchievementList.h"
using namespace std;

class AchievementWinAll : public AchievementList
{
    public:
        AchievementWinAll();

        void setAmount(CRint value);

    protected:
        // increase or decrease achievement count
        virtual void changeCount(const vector<Event*>& changeEvents);
};

#endif // ACHIEVEMENTWINALL_H


