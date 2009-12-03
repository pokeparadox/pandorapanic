#include "Rocket.h"

Rocket::Rocket()
{
    //ctor
    landed = false;
    angle = 0.0f;
    thrust = 0.0f;
    startPos.x = startPos.y = 0;
    acceleration = velocity = Vector2df(0,0);
    /// Setup particles
    leftThruster.resize(3);
    rightThruster.resize(3);
    int t = 0;
    const int INVISIBLE = 1;
    const int MAX_ONE = 500;
    const int MAX_TWO = MAX_ONE * 0.66f;
    const int MAX_THREE = MAX_ONE * 0.33f;
    //  TODO: remove hardcoded bounds workaround.
    Vector2di b;
    b.x = 800;
    b.y = 480;
    leftThruster[t].setMax(MAX_ONE);
    leftThruster[t].setInvisible(MAX_ONE-INVISIBLE);
    leftThruster[t].setColour(YELLOW);
    leftThruster[t].setMaxVelocity(Vector2df(MAX_VEL_DN/3.0f,MAX_VEL_DN));
    leftThruster[t].setLifeTime(100);
    leftThruster[t].setGravity(GRAVITY);
    //leftThruster[t].setPosition(Vector2di(startPos.x + 16, startPos.y+64));
    leftThruster[t].setPosition(Vector2di(-10000, -10000));
    leftThruster[t].setBoundaries(Vector2di(0,0),b);
    leftThruster[t].advanceUpdate(1);

    leftThruster[++t].setMax(MAX_TWO);
    leftThruster[t].setInvisible(MAX_TWO-INVISIBLE);
    leftThruster[t].setColour(RED);
    leftThruster[t].setMaxVelocity(Vector2df(MAX_VEL_DN/3.0f,MAX_VEL_DN));
    leftThruster[t].setLifeTime(90);
    leftThruster[t].setGravity(GRAVITY);
    //leftThruster[t].setPosition(Vector2di(startPos.x + 16, startPos.y+64));
    leftThruster[t].setPosition(Vector2di(-10000, -10000));
    leftThruster[t].setBoundaries(Vector2di(0,0),b);
    leftThruster[t].advanceUpdate(1);

    leftThruster[++t].setMax(MAX_THREE);
    leftThruster[t].setInvisible(MAX_THREE-INVISIBLE);
    leftThruster[t].setColour(BLUE);
    leftThruster[t].setMaxVelocity(Vector2df(MAX_VEL_DN/3.0f,MAX_VEL_DN));
    leftThruster[t].setLifeTime(80);
    leftThruster[t].setGravity(GRAVITY);
    //leftThruster[t].setPosition(Vector2di(startPos.x + 16, startPos.y+64));
    leftThruster[t].setPosition(Vector2di(-10000, -10000));
    leftThruster[t].setBoundaries(Vector2di(0,0),b);
    leftThruster[t].advanceUpdate(1);

    t = 0;
    rightThruster[t].setMax(MAX_ONE);
    rightThruster[t].setInvisible(MAX_ONE-INVISIBLE);
    rightThruster[t].setColour(YELLOW);
    rightThruster[t].setMaxVelocity(Vector2df(MAX_VEL_DN/3.0f,MAX_VEL_DN));
    rightThruster[t].setLifeTime(100);
    rightThruster[t].setGravity(GRAVITY);
    //rightThruster[t].setPosition(Vector2di(startPos.x + 48, startPos.y+64));
    rightThruster[t].setPosition(Vector2di(-10000, -10000));
    rightThruster[t].setBoundaries(Vector2di(0,0),b);
    rightThruster[t].advanceUpdate(1);

    rightThruster[++t].setMax(MAX_TWO);
    rightThruster[t].setInvisible(MAX_TWO-INVISIBLE);
    rightThruster[t].setColour(RED);
    rightThruster[t].setMaxVelocity(Vector2df(MAX_VEL_DN/3.0f,MAX_VEL_DN));
    rightThruster[t].setLifeTime(90);
    rightThruster[t].setGravity(GRAVITY);
    //rightThruster[t].setPosition(Vector2di(startPos.x + 48, startPos.y+64));
    rightThruster[t].setPosition(Vector2di(-10000, -10000));
    rightThruster[t].setBoundaries(Vector2di(0,0),b);
    rightThruster[t].advanceUpdate(1);

    rightThruster[++t].setMax(MAX_THREE);
    rightThruster[t].setInvisible(MAX_THREE-INVISIBLE);
    rightThruster[t].setColour(BLUE);
    rightThruster[t].setMaxVelocity(Vector2df(MAX_VEL_DN/3.0f,MAX_VEL_DN));
    rightThruster[t].setLifeTime(80);
    rightThruster[t].setGravity(GRAVITY);
    //rightThruster[t].setPosition(Vector2di(startPos.x + 48, startPos.y+64));
    rightThruster[t].setPosition(Vector2di(-10000, -10000));
    rightThruster[t].setBoundaries(Vector2di(0,0),b);
    rightThruster[t].advanceUpdate(1);

    startThrusters(false);

    /// Setup Ship Sprite
    sprite.loadSprite("images/RocketEscape/rocket.png");
    #ifdef PENJIN_SDL
        sprite.setUseHardware(false);
    #endif
    LUT::init();
}

Rocket::~Rocket()
{
    //dtor
    LUT::deInit();
}

void Rocket::rotateZero()
{
    if(angle>0)
        angle-=1.5f;
    else if(angle<0)
        angle+=1.5f;
}

#ifdef PENJIN_SDL
    void Rocket::render(SDL_Surface* screen)
    {
        for(int i = leftThruster.size()-1;i>=0; --i)
        {
            leftThruster[i].render(screen);
            rightThruster[i].render(screen);
        }
        sprite.setRotation(angle);
        sprite.render(screen);
    }
#else
    void Rocket::render()
    {
        for(int i = leftThruster.size()-1;i>=0; --i)
        {
            leftThruster[i].render();
            rightThruster[i].render();
        }
        sprite.setRotation(angle);
        sprite.render();
    }
#endif

void Rocket::startThrusters(CRbool start)
{
    for(int i = leftThruster.size()-1; i >=0; --i)
    {
        leftThruster[i].setShouldStopNew(!start);
        rightThruster[i].setShouldStopNew(!start);
    }
}

void Rocket::update()
{
    #ifdef PENJIN_FIXED
        oldPos.x = fixedpoint::fix2int(sprite.getX());
        oldPos.y = fixedpoint::fix2int(sprite.getY());
    #else
        oldPos = sprite.getPosition();
    #endif
    Vector2df pos(sprite.getX(),sprite.getY());
    if(fuel > 0.0f)
        fuel -= (thrust*0.33f);
    if(pos.y <0 )
        thrustKill();
    if(thrust > 0.01f)
        startThrusters(true);
    else
        startThrusters(false);
    if(wrapSides)
    {
        if(pos.x < -64)
            pos.x = bounds.w;
        else if(pos.x > bounds.w)
            pos.x = 0;
    }
    else
    {
        if(pos.x < 0)
        {
            pos.x = 0;
            invertVelocityX();
        }
        else if(pos.x + 64 > bounds.w)
        {
            pos.x = bounds.w - 64;
            invertVelocityX();
        }
    }
    if(pos.y < startPos.y -64)
    {
        acceleration.y = GRAVITY;
    }
    else
    {
        pos.y = startPos.y-65;
        velocity.x *= 0.9f; //Friction
        invertVelocityY();
        if(fuel<=0.0f)
            landed = true;
    }
    #ifdef PENJIN_FIXED
        acceleration.x = thrust * LUT::Lsin(-fixedpoint::fix2int(angle));
    #else
        acceleration.x = thrust * LUT::Lsin(-angle);
    #endif
    if(!(fuel <= 0.0f))
        #ifdef PENJIN_FIXED
            acceleration.y += -thrust * LUT::Lcos(fixedpoint::fix2int(angle));
        #else
            acceleration.y += -thrust * LUT::Lcos(angle);
        #endif

    velocity+=acceleration;
    limitVelocity();
    for(int i = leftThruster.size()-1;i>=0; --i)
    {
        //if(acceleration.x<=-0.5f)
            leftThruster[i].setAcceleration(Vector2df(-acceleration.x,abs(acceleration.y)));
        //if(acceleration.x>=0.5f)
            rightThruster[i].setAcceleration(Vector2df(-acceleration.x,abs(acceleration.y)));
        Vector2df centre = pos;
        centre.x+=32;centre.y+=32;
        leftRotated = pos;
        leftRotated.x += 23;leftRotated.y+=64;
        leftRotated = rotatePoint(leftRotated,centre,-angle);
        leftThruster[i].setPosition(leftRotated);
        rightRotated = pos;
        rightRotated.x += 41;rightRotated.y+=64;
        rightRotated = rotatePoint(rightRotated,centre,-angle);
        rightThruster[i].setPosition(rightRotated);
        leftThruster[i].update();
        rightThruster[i].update();
    }
    sprite.setPosition(Vector2di(pos.x, pos.y) + Vector2di(velocity.x+0.5f,velocity.y+0.5f));
}

Vector2df Rocket::rotatePoint(const Vector2df& p, const Vector2df& c, CRfloat a)
{
    uchar angle = LUT::degreeToBrad(a);
    Vector2df out;
    out.x = c.x + (LUT::Lcos(angle) * (p.x - c.x) - (LUT::Lsin(angle)) * (p.y - c.y));
    out.y = c.y + (LUT::Lsin(angle) * (p.x - c.x) + (LUT::Lcos(angle)) * (p.y - c.y));
    return out;
}

void Rocket::limitVelocity()
{
    if(velocity.x < -MAX_VEL_DN)
        velocity.x = -MAX_VEL_DN;
    else if(velocity.x > MAX_VEL_DN)
        velocity.x = MAX_VEL_DN;

    if(velocity.y < MAX_VEL_UP)
        velocity.y = MAX_VEL_UP;
    else if(velocity.y > MAX_VEL_DN)
        velocity.y = MAX_VEL_DN;
}

bool Rocket::checkCollectible()
{
    float left[2];
    float right[2];
    float top[2];
    float bottom[2];
    bool collision = true;
    int xOffset = 4;
    left[0] = sprite.getX() + xOffset;
    left[1] = collect->getX();
    right[0] = left[0] + 64 - xOffset;
    right[1] = left[1] + collect->getWidth();
    top[0] = sprite.getY();
    top[1] = collect->getY();
    bottom[0] = top[0] + 64;
    bottom[1] = top[1] + collect->getHeight();

    if (bottom[0] < top[1])
        collision = false;
    else if (top[0] > bottom[1])
        collision = false;
    else if (right[0] < left[1])
        collision = false;
    else if (left[0] > right[1])
        collision = false;
    return collision;
}

void Rocket::setBounds(SDL_Rect bounds)
{
    this->bounds = bounds;
    for(int i = 2; i >=0; --i)
    {
        leftThruster[i].setBoundaries(bounds);
        rightThruster[i].setBoundaries(bounds);
    }
}
