#ifndef STATEMAZE_H_INCLUDED
#define STATEMAZE_H_INCLUDED

#include "PenjinStates.h"     // Make sure your state is listed in this include.

#include "Background.h"
#include "Sprite.h"
#include "TextDoc.h"
#include "Random.h"

#include "Jumper/Tile.h"
#include "ButtonPrompter.h"

//#include "StatePanJoust/HeroBird.h

class Charac : public Sprite
{

    public:
        Charac();
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

        float xvel;
        float yvel;
        int width;
        int height;
        Sprite blobsprite;

};



class StateMaze : public BaseState
{
    public:
        StateMaze();
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
        virtual ~StateMaze();

    private:
        ButtonPrompter buttonPrompter;
        Background background0;
        Timer teller;
        Timer inputlimiter;
        const char* filename;
        Sprite mysprite;
        Sprite tilesprite[10];
        Sprite blobevil;
        Charac cloud[32];
        //AnimatedSprite robotright;
        //AnimatedSprite robotleft;
        Charac hero;

        Tile blok[300];
        int bloksprite[300];

        Music music;
        int downhit[30];
        int uphit[30];
        int righthit[30];
        int lefthit[30];
        int direction[30];
        int gamelength;

        int downtrigger[30];
        int uptrigger[30];
        int lefttrigger[30];
        int righttrigger[30];
        int willekeur;
        //string filename;
        string testingx;
        string testingy;
        string withblob;
        string tiletype;
        int testx;
        int testy;
        Text text;
        Tile goal;
        int tilenumber;
        bool youdead;


        int cloudnumber;
};
//The tile



#endif // STATEMAZE_H_INCLUDED
