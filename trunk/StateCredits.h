#ifndef STATECREDITS_H
#define STATECREDITS_H
#include "PenjinStates.h"     // Make sure your state is listed in this include.
/*
Staff Roll/credits
*/
#include "version.h"
#include "Background.h"
#include "AnimatedSprite.h"
#include "Sprite.h"
#include "Music.h"
#include "Text.h"
#include "Timer.h"
#include "Random.h"
#include "Colour.h"
#include "BackBuffer.h"
#include "Emitter.h"
#include "NumberUtility.h"

const static string CREDITS[]=
{
    "PandoraPanic! [VERSION]",
    "Games by the OpenHandhelds and GP32X community",
    "",
    "Music:",
    "Title Music - Pat Ferguson",
    "Losing Music - Pat Ferguson",
    "Winning Music - Pat Ferguson",
    "Nervous (DistractedMaths) - Rooster",
    "Mojo (Credits) - Rooster",
    "Windows (Badvista) - Gruso",
    "Whatever :P (Panic Attack) - Gruso",
    "Quickdraw (SnatchABeer) - Gruso",
    "Birthday - Dragons_Slayer",
    "",
    "Graphics:",
    "GUI Concept and Art - Kagato",
    "BirdRiders/Platforms, Panjoust - Dragons_Slayer",
    "Rocket and Pad, Plander - Dragons_Slayer",
    "Birthday Cake - Dragons_Slayer",
    "Robot, Jumper - Niqk_Dude",
    "Forum Posts - Chip",
    "Snazzy buttons, PanicAttack - TheBrainSquid",
    "Test Your Sight - Cyrax",
    "BadVista - badvista.org",
    "",
    "MiniGames:",
    "BallDrop - Zacaj, billykater",
    //"Barrel Roll - Cameleon",
    "Space Invaders - pstudio",
    "Spelling Contest - pstudio",
    "BadVista - Todd",
    "Panic Attack - Todd",
    "Punch Wrestler - Trabitboy",
    "PuttPutt - DasFool",
    "Pong - Miggles",
    "Pang - Rockthesmurf",
    "One Button Bandit - Miggles",
    "Distracted Maths - MarkoeZ",
    "Jumper - MarkoeZ",
    "Maze - MarkoeZ",
    //"DoubleNubTrouble - MarkoeZ",
    "PanJoust - MarkoeZ",
    "Reflex - oclbdk",
    "Rotating Turret - B-Zar",
    "Memory Blocks - B-Zar",
    "Plander - PokeParadox",
    "Birthday Cake - PokeParadox",
    "Arena - Dragons_Slayer",
    "Snatch A Beer - foxblock",
    "The Count Job - foxblock",
    "Test Your Sight - foxblock",
    "Fire - Pickle",
    "",
    "Special Thanks:",
    "Ben Monk - getting the ball rolling!",
    "Pickle - debugging on devboard",
    "SDL - easier cross-platform support",
    "OpenPandora - awesome OSS handheld.",
    "Mariela - I love youuuuuuuuuuuuuu!",
    "foxblock - for making me laugh",
    "Gruso - writing Manual and Blogging",
    "Anyone else that hass been missed...",
    "You also rock!",
    "PS let me know if you're missing.",
    "",
    "PandoraPanic! Engine - PokeParadox",
    "",
    "Penjin Development - Pirate Games",
    "Pirate Games 2008-2010",
    "pirate-games.co.uk",
    ""
};

class StateCredits : public BaseState
{
    public:
        StateCredits();
        virtual void init();
    #ifdef PENJIN_SDL
        virtual void render(SDL_Surface *screen);
    #else
        virtual void render();
    #endif
        virtual void update();
        virtual void unlimitedUpdate();
        virtual ~StateCredits();
        virtual void userInput();

	private:
        void initPandora(const Vector2di& start);
        void movePandora(const Vector2di& move);
        #ifdef PENJIN_SDL
            void renderPandora(SDL_Surface* screen);
        #else
            void renderPandora();
        #endif
        void updatePandora();

		Music music;
		Text text;
		Timer timer;
		uint currentLine;

		vector <Emitter> emit;
		Sprite logo;
		BackBuffer buff;
		AnimatedSprite pand;
		Vector2di pandStartPos;
		Vector3df pandPos[9];
		Vector3df pandVel[9];
		uchar pandPrio[9];
};
#endif	//	STATECREDITS_H

