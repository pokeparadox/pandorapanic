#ifndef STATEJUMPER_H_INCLUDED
#define STATEJUMPER_H_INCLUDED

#include "PenjinStates.h"     // Make sure your state is listed in this include.

#include "Background.h"
#include "Sprite.h"
#include "TextDoc.h"
#include "Random.h"
#include "Music.h"
#include "Jumper/Tile.h"
#include "ButtonPrompter.h"
//#include "StatePanJoust/HeroBird.h

class Character : public Sprite
{

    public:
        Character();
        void setXvel(CRfloat x){xvel = x;}
        void setYvel(CRfloat y){yvel = y;}
        void setHeight(CRint x){height = x;}
        void setWidth(CRint y){width = y;}
        void setDimensions(CRint x, CRint y){height = x; width = y;}
        float getXvel(){return xvel;}
        float getYvel(){return yvel;}
        int getHeight(){return height;}
        int getWidth(){return width;}
        int offset;
        int ontile;
        int connecttotile;
        int type;

        //Character(CRint x,CRint y);

    private:
//ToDo: blah
        float xvel;
        float yvel;
        int width;
        int height;
        Sprite blobsprite;

};



class StateJumper : public BaseState
{
    public:
        StateJumper();
        virtual void init();
        #ifdef PENJIN_SDL
            virtual void render(SDL_Surface *screen);
            virtual void pauseScreen(SDL_Surface* screen);
        #else
            virtual void render();
            virtual void pauseScreen();
        #endif
        virtual void update();
        virtual void onPause();
        virtual void onResume();

        virtual void userInput();
        virtual void pauseInput();
        virtual ~StateJumper();

    private:
        ButtonPrompter buttonPrompter;
        Background background0;
        Timer teller;
        Timer inputlimiter;
        const char* filename;
        Sprite mysprite;
        Sprite tilesprite;
        Sprite blobevil;
        Sprite cloud;
        AnimatedSprite robotright;
        AnimatedSprite robotleft;
        Character hero;
        Character blob[100];
        Tile blok[200];

        //string filename;
        string testingx;
        string testingy;
        string withblob;
        string tiletype;
        int testx;
        int testy;
        int jumpcount;
        Text text;
        Tile goal;
        int tilenumber;
        int blobtimer[100];

        int blobjump[100];
        int blobnumber;
        Music music;
};
//The tile



#endif // STATEJUMPER_H_INCLUDED
