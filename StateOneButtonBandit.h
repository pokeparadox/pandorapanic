#ifndef STATEONEBUTTONBANDIT_H_INCLUDED
#define STATEONEBUTTONBANDIT_H_INCLUDED

#include "PenjinStates.h"     // Make sure your state is listed in this include.
/*
    Miggles' Pokies game for Pandora Panic!
*/

struct OBBShapeNode {
    int shape, yPos;
    OBBShapeNode *next;
};

struct OBBColumnConfig {
    int screenX, screenY, shapeX, shapeY, shapeGapX, shapeGapY, numberOfShapes, moveY, matchShape, guideYPos;
};

class OBBShapes {
    public:
        OBBShapes();
        ~OBBShapes();
        void   addShape(string s);
        Sprite &getSprite(int n, bool guide);

    private:
        int shapeCount;
        Sprite *shapes[6];
        Sprite *guides[6];
};

class OBBColumn {
    public:
        ~OBBColumn();
        void initColumn(OBBShapes &s, OBBColumnConfig &c, float r, int x);
        void update(float &rate);
        #ifdef PENJIN_SDL
            void render(SDL_Surface *screen);
        #else
            void render();
        #endif
        bool isValid();
        void stopOnMatch();
        void instantStop();
        bool isStopped();
        bool active;

    private:
        void addShapeNode();
        void deleteLeadNode();
        bool nodeIsInValidPosition(OBBShapeNode *node);

        int shapes, xPos, alpha, currentState, moveY;
        float rateAdditive;

        Text text;
        Sound click;
        Sprite button;


        OBBShapes sprites;
        OBBColumnConfig config;
        OBBShapeNode *shapeNode, *lastShapeNode, *tempNode, *finalMatchNode;
};

class StateOneButtonBandit : public BaseState
{
    public:
//        StateOneButtonBandit();
//        ~StateOneButtonBandit();
        virtual void init();
        #ifdef PENJIN_SDL
            virtual void render(SDL_Surface *screen);
            virtual void pauseScreen(SDL_Surface* screen);
        #else
            virtual void render();
            virtual void pauseScreen();
        #endif
        virtual void update();
        virtual void userInput();
        virtual void pauseInput();
        virtual void unlimitedUpdate();
        virtual void onPause();
        virtual void onResume();


    private:

        Timer inputlimiter; //timer for the pause functionality

        Background background;
        Image shading;

        bool win, waitForButtonLift;
        int shapes, currentColumn, currentState;
        float startGlobalRate, maxGlobalRate, currentGlobalRate;

        Timer gameTimer;
        Sound coin, click, tumbler, chime, error;
        Text text;

        OBBShapes sprites;
        OBBColumnConfig config;
        OBBColumn leftColumn, centreColumn, rightColumn;
        Music music;
};



#endif // STATEONEBUTTONBANDIT_H_INCLUDED
