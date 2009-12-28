#ifndef STATETILEEDITOR_H_INCLUDED
#define STATETILEEDITOR_H_INCLUDED

#include "userStates.h"     // Make sure your state is listed in this include.
/*
    nEW pROJECT

*/


#include "Background.h"
#include "Sprite.h"
#include "TextDoc.h"
#include "Random.h"
#include "Jumper/Tile.h"



class StateTileEditor : public BaseState
{
    public:
        StateTileEditor();
        virtual void init();
        #ifdef PENJIN_SDL
            virtual void render(SDL_Surface *screen);
        #else
            virtual void render();
        #endif
        virtual void update();
        virtual void userInput();
        virtual ~StateTileEditor();

    private:
        Timer inputlimiter;

        Background background;
        Sprite mysprite[10];
        Sprite tilesprite;
        Sprite tileup;
        Sprite tiledown;
        Sprite tileleft;
        Sprite tileright;
        Sprite blob;
        Sprite cloud;
        Vector2di tiles[200];
        Vector2di scope;
        int tilenumber;
        Sprite goal;
        bool goaldefined;
        int tiletype[200];
        int temptiletype;
        Tile blok[200];
        int offset;
        int withblob;
        bool del;
        string testingx;
        string testingy;
        string hasblob;
        string soort;
        Text test;
};

#endif // STATETILEEDITOR_H_INCLUDED
