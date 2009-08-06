#include "Collectible.h"

Collectible::Collectible()
{
    //ctor
    reset();
}

void Collectible::reset()
{
    setPosition(Random::nextInt(20,780),Random::nextInt(20,350));
}
