#ifndef LANDINGPAD_H
#define LANDINGPAD_H

#include "RocketEscape/Rocket.h"
#include "ParticleMirror.h"
#include "AnimatedSprite.h"

class LandingPad
{
    public:
        LandingPad();
        ~LandingPad();

        /// Setup
        void setPosition(Vector2di pos);
        void linkEmitter(Emitter* e){mirror.linkEmitter(e);}
        void linkRocket(Rocket* r){rocket = r;}
        void setWidth(CRuint w){width = w;}
        void setHeight(CRuint h){height = h;}

        /// Realtime
        #ifdef PENJIN_SDL
            void render(SDL_Surface* screen);
        #else
            void render();
        #endif
        void update();
        bool hasRocketLanded()const;

    private:
        void rocketCollision();

        Vector2di position;
        // centre segment
        Sprite centre;
        ParticleMirror mirror;
        uint width;             //  Num of centre segments in middle of platform
        uint height;            //  Num of stand segments to support the platform
        Vector2di platformDims;
        //  Light segments
        AnimatedSprite lights[2];
        Rocket* rocket;
        bool rocketLanded;
};

#endif // LANDINGPAD_H
