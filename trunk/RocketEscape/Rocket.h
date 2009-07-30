#ifndef ROCKET_H
#define ROCKET_H

#include "LUT.h"
#include "Sprite.h"
#include "Sound.h"
#include "Emitter.h"
#include <vector>

namespace RocketConstants
{
    const float MAX_ACCEL = 0.7f;
    const float GRAVITY = 0.3f;
    const float MAX_VEL_UP = -10.5f;
    const float MAX_VEL_DN = 8.2f;
#ifdef PENJIN_FIXED
    const Fixed MAX_ANGLE = 45;
#else
    const uint MAX_ANGLE = 45;
#endif
}
using namespace RocketConstants;

class Rocket
{
    public:
        Rocket();
        ~Rocket();

        #ifdef PENJIN_SDL
            void render(SDL_Surface* screen);
        #else
            void render();
        #endif

        void update();

        PENJIN_ERRORS loadSound(CRstring sound){PENJIN_ERRORS e = hit.loadSound(sound);hit.setVolume(66);return e;}

        void setBounds(SDL_Rect bounds);
        void setPosition(Vector2di pos)
        {
            sprite.setPosition(pos);
            for(int i = leftThruster.size()-1; i>=0; --i)
            {
                 leftThruster[i].setPosition(Vector2di(startPos.x + 16, startPos.y+64));
                 rightThruster[i].setPosition(Vector2di(startPos.x + 48, startPos.y+64));
            }
        }
        void setStartPosition(const Vector2di& pos){startPos = pos;}
        void setFuel(CRfloat f){fuel=f;}
        void setWrapSides(CRbool shouldWrap){wrapSides = shouldWrap;}
        float getFuel()const{return fuel;}
        bool hasLanded()const{return landed;}
        Vector2df getPosition(){return sprite.getPosition();}
        Vector2di getOldPosition(){return oldPos;}
        Vector2df getVelocity(){return velocity;}
        void invertVelocityX(){velocity.x = -velocity.x*0.6f;angle = 0-angle;acceleration.x = 0;hit.play();}
        void invertVelocityY(){velocity.y = -velocity.y*0.4f;angle = 0;acceleration.y = 0;if(abs(velocity.y)>0.4f){hit.play();}}

        void rotateLeft(){if(angle<MAX_ANGLE){angle+=0.1f;}}
        void rotateRight(){if(angle+45> 0){angle-=0.1f;}}
        void rotateZero();
        void thrustIncrease(){if(thrust < MAX_ACCEL){thrust+=0.025f;}}
        void thrustDecrease(){if(thrust > 0){thrust-=0.025f;}}
        void thrustKill(){thrust = 0.0f;}

        Emitter* getLeftThruster(CRuint i){if(i<=leftThruster.size()-1){return &leftThruster[i];}return NULL;}
        Emitter* getRightThruster(CRuint i){if(i<=rightThruster.size()-1){return &rightThruster[i];}return NULL;}
    private:
        Vector2df rotatePoint(const Vector2df& toRot,const Vector2df& centre, CRfloat angle);
        void limitVelocity();
        void startThrusters(CRbool start);
        vector <Emitter> leftThruster;
        vector <Emitter> rightThruster;
        Vector2df leftRotated;
        Vector2df rightRotated;
        Sprite sprite;
        Sound hit;
        Vector2df velocity;
        Vector2df acceleration;
        Vector2di startPos;
        Vector2di oldPos;
        float angle;
        float thrust;
        float fuel;
        bool landed;
        bool wrapSides;
        SDL_Rect bounds;
};

#endif // ROCKET_H
