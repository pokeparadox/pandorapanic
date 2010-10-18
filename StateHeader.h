#ifndef STATEHEADER_H_INCLUDED
#define STATEHEADER_H_INCLUDED

#include "mgBaseState.h"
#include "PenjinStates.h"     // Make sure your state is listed in this include.
/*
   Miggles' Pong game for Pandora Panic!
   */


#include "Background.h"
#include "Sprite.h"

class StateHeader : public mgBaseState
{
	public:
		StateHeader();
		virtual void init();
#ifdef PENJIN_SDL
		virtual void render(SDL_Surface *screen);
		virtual void pauseScreen(SDL_Surface *screen);
#else
		virtual void render();
		virtual void pauseScreen();
#endif
		virtual void update();
		virtual void unlimitedUpdate();

		virtual ~StateHeader();
		virtual void userInput();

		Background background;
		AnimatedSprite	playerL, playerR;
		AnimatedSprite	numb1, numb0;
		Sprite 		ball;
		int		statePL, statePR;
		int		yposPL, yposPR, xposPL, xposPR;
		int		ball_x, ball_y, ball_dir;
		int		ball_t, ball_dt, ball_ang;
		bool		fail, done;
		int     	count;
		int		level;
		Timer		endGame;
		int		screenWidth, screenHeight, paddleWidth, paddleHeight, ballWidth, ballHeight;

		Sound	sndKick, sndHitG;
		Text text;
};

#endif // STATEHEADER_H_INCLUDED
