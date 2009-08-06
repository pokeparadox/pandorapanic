#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H
#include "AnimatedSprite.h"
#include "Random.h"

class Collectible : public AnimatedSprite
{
    public:
        Collectible();
        int getReward()const{return reward;}
        void setReward(CRint r){reward = r;}

    private:
        void reset();
        int reward;
};

#endif // COLLECTIBLE_H
