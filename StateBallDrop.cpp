#include "StateBallDrop.h"
StateBallDrop::StateBallDrop()
{
    nullify = false;
    cnt=0;
    //srand(time(NULL)); // I seed random numbers on app start - Poke
    ballx=rand()%600+100;
    bally=40;
    ballz=25;
    x=400;
    dir=rand()%4-2;
    text.loadFont("font/origami.ttf", 48);
    text.setColour(Colour(BLACK));
    yadd=-8;
    buttonPrompter.display(ButtonPrompter::BUTTON_RIGHT, 385, 370);
    buttonPrompter.display(ButtonPrompter::BUTTON_LEFT, 350, 370);
    gotoNext = false;
    GFX::setClearColour(WHITE);
}

void StateBallDrop::init()
{
    if(variables.size()<SUBSTATE_TRIGGER)
    {
        music.loadMusic("music/Balldrop/bd_shortfall.ogg");
        music.setLooping(true);
        music.play();

        input->setScaler(0.00007f);
        input->setDeadZoneX(2);
    }
}

#ifdef PENJIN_SDL
    //Here follows a badly hacked copy of SDL_gfx, because it wouldnt link right for me
    int pixelColorNolock(SDL_Surface * dst, Sint16 x, Sint16 y, Uint32 color)
    {
        SDL_Rect r;
        r.x=x;
        r.y=y;
        r.w=r.h=1;
        SDL_FillRect(dst,&r,color);
        return (0);
    }
    int pixelColorWeightNolock(SDL_Surface * dst, Sint16 x, Sint16 y, Uint32 color, Uint32 weight)
    {
        Uint32 a;
        a = (color & (Uint32) 0x000000ff);
        a = ((a * weight) >> 8);
        return (pixelColorNolock(dst, x, y, (color & (Uint32) 0xffffff00) | (Uint32) a));
    }

    int aaellipseColor(SDL_Surface * dst, Sint16 xc, Sint16 yc, Sint16 rx, Sint16 ry, Uint32 color)
    {
        Sint16 left, right, top, bottom;
        Sint16 x1,y1,x2,y2;
        int i;
        int a2, b2, ds, dt, dxt, t, s, d;
        Sint16 x, y, xs, ys, dyt, od, xx, yy, xc2, yc2;
        float cp;
        float sab;
        Uint8 weight, iweight;
        int result;
        if ((rx < 0) || (ry < 0)) {
        return (-1);
        }
        left = dst->clip_rect.x;
        right = dst->clip_rect.x + dst->clip_rect.w - 1;
        top = dst->clip_rect.y;
        bottom = dst->clip_rect.y + dst->clip_rect.h - 1;
        x1 = xc - rx;
        x2 = xc + rx;
        y1 = yc - ry;
        y2 = yc + ry;
        if ((x1<left) && (x2<left)) {
         return(0);
        }
        if ((x1>right) && (x2>right)) {
         return(0);
        }
        if ((y1<top) && (y2<top)) {
         return(0);
        }
        if ((y1>bottom) && (y2>bottom)) {
         return(0);
        }
        a2 = rx * rx;
        b2 = ry * ry;

        ds = 2 * a2;
        dt = 2 * b2;

        xc2 = 2 * xc;
        yc2 = 2 * yc;

        sab = sqrt(a2 + b2);
        #ifdef PENJIN_FIXED
            od = lrint(fixedpoint::fix2float(sab*0.01f)) + 1; /* introduce some overdraw */
            dxt = lrint(fixedpoint::fix2float((float)a2 / sab)) + od;
        #else
            od = lrint(sab*0.01f) + 1; /* introduce some overdraw */
            dxt = lrint((float)a2 / sab) + od;
        #endif
        t = 0;
        s = -2 * a2 * ry;
        d = 0;

        x = xc;
        y = yc - ry;

        /* Draw */
        result = 0;

        /* Lock surface */
        if (SDL_MUSTLOCK(dst)) {
        if (SDL_LockSurface(dst) < 0) {
            return (-1);
        }
        }

        /* "End points" */
        result |= pixelColorNolock(dst, x, y, color);
        result |= pixelColorNolock(dst, xc2 - x, y, color);
        result |= pixelColorNolock(dst, x, yc2 - y, color);
        result |= pixelColorNolock(dst, xc2 - x, yc2 - y, color);

        for (i = 1; i <= dxt; i++) {
        x--;
        d += t - b2;

        if (d >= 0)
            ys = y - 1;
        else if ((d - s - a2) > 0) {
            if ((2 * d - s - a2) >= 0)
            ys = y + 1;
            else {
            ys = y;
            y++;
            d -= s + a2;
            s += ds;
            }
        } else {
            y++;
            ys = y + 1;
            d -= s + a2;
            s += ds;
        }

        t -= dt;

        /* Calculate alpha */
        if (s != 0.0f) {
            cp = (float) abs(d) / (float) abs(s);
            if (cp > 1.0f) {
            cp = 1.0f;
            }
        } else {
            cp = 1.0f;
        }

        /* Calculate weights */
    #ifdef PENJIN_FIXED
        weight = (Uint8) (fixedpoint::fix2int(cp * 255));
    #else
        weight = (Uint8) (cp * 255);
    #endif
        iweight = 255 - weight;

        /* Upper half */
        xx = xc2 - x;
        result |= pixelColorWeightNolock(dst, x, y, color, iweight);
        result |= pixelColorWeightNolock(dst, xx, y, color, iweight);

        result |= pixelColorWeightNolock(dst, x, ys, color, weight);
        result |= pixelColorWeightNolock(dst, xx, ys, color, weight);

        /* Lower half */
        yy = yc2 - y;
        result |= pixelColorWeightNolock(dst, x, yy, color, iweight);
        result |= pixelColorWeightNolock(dst, xx, yy, color, iweight);

        yy = yc2 - ys;
        result |= pixelColorWeightNolock(dst, x, yy, color, weight);
        result |= pixelColorWeightNolock(dst, xx, yy, color, weight);
        }

        /* Replaces original approximation code dyt = abs(y - yc); */
        #ifdef PENJIN_FIXED
            dyt = lrint(fixedpoint::fix2float((float)b2 / sab)) + od;
        #else
            dyt = lrint((float)b2 / sab ) + od;
        #endif
        for (i = 1; i <= dyt; i++) {
        y++;
        d -= s + a2;

        if (d <= 0)
            xs = x + 1;
        else if ((d + t - b2) < 0) {
            if ((2 * d + t - b2) <= 0)
            xs = x - 1;
            else {
            xs = x;
            x--;
            d += t - b2;
            t -= dt;
            }
        } else {
            x--;
            xs = x - 1;
            d += t - b2;
            t -= dt;
        }

        s += ds;

        /* Calculate alpha */
        if (t != 0.0f) {
            cp = (float) abs(d) / (float) abs(t);
            if (cp > 1.0f) {
            cp = 1.0f;
            }
        } else {
            cp = 1.0f;
        }

        /* Calculate weight */
        #ifdef PENJIN_FIXED
        weight = (Uint8) (fixedpoint::fix2int(cp * 255));
        #else
        weight = (Uint8) (cp * 255);
        #endif
        iweight = 255 - weight;

        /* Left half */
        xx = xc2 - x;
        yy = yc2 - y;
        result |= pixelColorWeightNolock(dst, x, y, color, iweight);
        result |= pixelColorWeightNolock(dst, xx, y, color, iweight);

        result |= pixelColorWeightNolock(dst, x, yy, color, iweight);
        result |= pixelColorWeightNolock(dst, xx, yy, color, iweight);

        /* Right half */
        xx = 2 * xc - xs;
        result |= pixelColorWeightNolock(dst, xs, y, color, weight);
        result |= pixelColorWeightNolock(dst, xx, y, color, weight);

        result |= pixelColorWeightNolock(dst, xs, yy, color, weight);
        result |= pixelColorWeightNolock(dst, xx, yy, color, weight);

        }

        /* Unlock surface */
        if (SDL_MUSTLOCK(dst)) {
        SDL_UnlockSurface(dst);
        }

        return (result);
    }
#else
    //Here follows a badly hacked copy of SDL_gfx, because it wouldnt link right for me
    int pixelColorNolock(Sint16 x, Sint16 y, Uint32 color)
    {

        return (0);
    }
    int pixelColorWeightNolock(Sint16 x, Sint16 y, Uint32 color, Uint32 weight)
    {
        Uint32 a;
        a = (color & (Uint32) 0x000000ff);
        a = ((a * weight) >> 8);
        return (pixelColorNolock(x, y, (color & (Uint32) 0xffffff00) | (Uint32) a));
    }

    int aaellipseColor(Sint16 xc, Sint16 yc, Sint16 rx, Sint16 ry, Uint32 color)
    {

    }
#endif
StateBallDrop::~StateBallDrop()
{

}

void StateBallDrop::userInput()
{

    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif


    /////////////////////////////
    //Added by billykater
    if(input->isLeft())
    {
        x -= BALL_SPEED;
        if(x < 40)
            x = 40;
    }
    else if(input->isRight())
    {
        x += BALL_SPEED;
        if(x > (800 - 40))
            x = 800 - 40;
    }
    else
    {
        x+=input->getLeftStickX()+0.5f;
        if(x < 40)
            x = 40;
        if(x > (800 - 40))
            x = 800 - 40;
    }

    /////////////////////////////

    if(input->isStart())//I wasnt sure what exit key was...
    {
        isPaused = !isPaused;
        music.playPause();
        input->resetKeys();
    }
}

#ifdef PENJIN_SDL
void StateBallDrop::render(SDL_Surface *screen)
{
        GFX::clearScreen(screen);
        if(cnt<3)
        {
            text.setPosition(300,216);
            aaellipseColor(screen,(int)x,400,40,40,SDL_MapRGBA(screen->format,0,0,0,255));
            SDL_Rect r;
            r.x=0;
            r.y=350;
            r.w=800;
            r.h=50;
            SDL_FillRect(screen,&r,SDL_MapRGB(screen->format,255,255,255));
            #ifdef PENJIN_FIXED
            aaellipseColor(screen,(int)ballx.intValue<<16,(int)bally.intValue<<16,ballz,ballz,SDL_MapRGBA(screen->format,0,0,0,255));
            #else
            aaellipseColor(screen,(int)ballx,(int)bally,ballz,ballz,SDL_MapRGBA(screen->format,0,0,0,255));
            #endif
        }
        if(cnt<2)
        {
            bally+=8;
            if(bally>400)cnt++;
                text.print(screen,"CATCH!");
        }
        if(cnt==2)
        {
            if(ballx-6<x-40||ballx+6>x+40)
                cnt=3;
            else
                cnt=4;
            text.print(screen,"CATCH!");
        }
        if(cnt==3)
        {
            /*text.setPosition(375,216);
            text.print(screen,":(");*/
            variables[0].setInt(0);
            gotoNext = true;
            GFX::clearScreen();
        }
        if(cnt==4)
        {
            /*text.setPosition(375,216);
            text.print(screen,":)");*/
            variables[0].setInt(1);
            gotoNext = true;
            GFX::clearScreen();
        }
        //buttonPrompter.render(screen);
}
#else
void StateBallDrop::render()
{
    GFX::clearScreen();
        if(cnt<3)
        {
            text.setPosition(300,216);
            //aaellipseColor(screen,(int)x,400,40,40,SDL_MapRGBA(screen->format,0,0,0,255));
            SDL_Rect r;
            r.x=0;
            r.y=350;
            r.w=800;
            r.h=50;
            //SDL_FillRect(screen,&r,SDL_MapRGB(screen->format,255,255,255));
            #ifdef PENJIN_FIXED
            //aaellipseColor(screen,(int)ballx.intValue<<16,(int)bally.intValue<<16,ballz,ballz,SDL_MapRGBA(screen->format,0,0,0,255));
            #else
            //aaellipseColor(screen,(int)ballx,(int)bally,ballz,ballz,SDL_MapRGBA(screen->format,0,0,0,255));
            #endif
        }
        if(cnt<2)
        {
            bally+=8;
            if(bally>400)cnt++;
                text.print("CATCH!");
        }
        if(cnt==2)
        {
            if(ballx-6<x-40||ballx+6>x+40)
                cnt=3;
            else
                cnt=4;
            text.print("CATCH!");
        }
        if(cnt==3)
        {
            /*text.setPosition(375,216);
            text.print(screen,":(");*/
            variables[0].setInt(0);
            gotoNext = true;
            GFX::clearScreen();
        }
        if(cnt==4)
        {
            /*text.setPosition(375,216);
            text.print(screen,":)");*/
            variables[0].setInt(1);
            gotoNext = true;
            GFX::clearScreen();
        }
        //buttonPrompter.render(screen);
}
#endif

void StateBallDrop::update()
{
    if(gotoNext)
        setNextState(STATE_MAIN);
}
