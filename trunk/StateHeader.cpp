#include "StateHeader.h"
#include "StateHeaderCurve.h"

#include "Random.h"

StateHeader::StateHeader()
{
    // Background
    background.loadBackground("images/Header/bkg.png");

    // Sprites
    ball.loadSprite( "images/Header/ball.png");
    ball.setTransparentColour(MAGENTA);
    playerL.loadFrames( "images/Header/anim_spriteL.png", 3, 1);
    playerL.setTransparentColour(MAGENTA);
    playerR.loadFrames( "images/Header/anim_spriteR.png", 3, 1);
    playerR.setTransparentColour(MAGENTA);
    numb0.loadFrames( "images/Header/numbers.png", 10, 1);
    numb0.setTransparentColour(MAGENTA);
    numb1.loadFrames( "images/Header/numbers.png", 10, 1);
    numb1.setTransparentColour(MAGENTA);

    if(this->isSubState()) {
        // Sound effects
		sndKick.loadSound(  "sounds/Header/kick.ogg");
		sndHitG.loadSound(  "sounds/Header/hitG.ogg");
    }
}


StateHeader::~StateHeader()
{
}

void StateHeader::init()
{
	done	= fail	= false;
	count   = 15;                        // 20 FTW!
	level	= variables[2].getInt();
	if ( level > 25)	level = 25;

	// states for the animations and movements
	statePL	= 0;
	statePR	= 0;
	ball_t	= 500;
	ball_dt	= 5+1*level;
	if ( Random::nextBool())
		ball_dt	*=-1;

	yposPL	= yposPR =300;
	xposPL	= 105-37;
	xposPR	= 710-27;
	ball_x	= xposPL+(xposPR - xposPL)/2;
	ball_y	= 250;

	// Put the players on screen
	playerL.setCurrentFrame( 0);
	playerR.setCurrentFrame( 2-0);
	playerL.setPosition( xposPL, yposPL);
	playerR.setPosition( xposPR, yposPR);

	numb0.setCurrentFrame( count % 10);
	numb0.setPosition( 448, 92);
	numb1.setCurrentFrame( count / 10);
	numb1.setPosition( 428, 92);
	// Set ball position
	ball.setPosition( ball_x, ball_y);

	pauseText.loadFont("font/bip.ttf", 32);
	pauseText.setColour(WHITE);

}

void StateHeader::userInput()
{

    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit()) {
            nullifyState();
        }
    #endif

    if (input->isStart())
    {
        pauseToggle();
        input->resetKeys();
    }


    // User mashing the buttons?
    if ( statePL == 0  &&  input->isX())
		statePL	= 1;			// start jump up.
	if ( statePR == 0  &&  input->isB())
		statePR	= 1;			// start jump up right sprite.

	//sndKick.setGlobalVolume(50);
	if ( input->isA())
		sndKick.play();
}

#ifdef PENJIN_SDL
    void StateHeader::render(SDL_Surface *screen)
    {
        // Render the background
        background.render(screen);

        // Add the user paddle
        playerL.render(screen);

        // Add the cpu paddle
        playerR.render(screen);

        // Add the ball
        ball.render(screen);

	// The score
	numb0.render(screen);
	numb1.render(screen);
    }

    void StateHeader::pauseScreen(SDL_Surface* screen)
    {
        // Pause screen
        if(!isSubState())
            pauseSymbol(screen);
        pauseText.setPosition(200,125);
        pauseText.print(screen, "pre-release (V.90)");
        pauseText.setPosition(105+50,175);
        pauseText.print(screen, "Idea taken from Superstar Saga!");
        pauseText.setPosition(110+50,215);
        pauseText.print(screen, "Press X and B at the right time!");
        buttonsOverlay(screen);
    }
#else
    void StateHeader::render()
    {
        // Render the background
        background.render();

        // Add the user paddle
        playerL.render();

        // Add the cpu paddle
        playerR.render();

        // Add the ball
        ball.render();

	// The score
	numb0.render();
	numb1.render();
    }

    void StateHeader::pauseScreen()
    {
        // Pause screen
        pauseSymbol();
        pauseText.setPosition(50,180);
        pauseText.print("Idea taken from Superstar Saga!");
        pauseText.setPosition(50,220);
        pauseText.print("Mash da buttons at the right time!");
    }
#endif
void StateHeader::unlimitedUpdate()
{
    // Are we in the end game?
	if ( fail && done) {
		variables[0].setInt(0);
		setNextState( STATE_MAIN);
	}

	// Done it!
	if ( count == 0) {
		variables[0].setInt(1);
		setNextState( STATE_MAIN);
	}
}

void StateHeader::update()
{
	switch ( statePL) {
		case 0:
			playerL.setCurrentFrame( 0);
			yposPL	= 300;
			break;
		case 1:
			playerL.setCurrentFrame( 1);
			yposPL	-=1;
			if ( yposPL < 300-10)
				statePL	= 2;
		case 2:
			playerL.setCurrentFrame( 2);
			yposPL	-=4;
			if ( yposPL < 300-50)
				statePL	= 3;		// player goes down
			break;
		case 3:
			playerL.setCurrentFrame( 0);
			yposPL	+=5;
			if ( yposPL > 300)
				statePL	= 0;
			break;
	}

	switch ( statePR) {
		case 0:
			playerR.setCurrentFrame( 2-0);
			yposPR	= 300;
			break;
		case 1:
			playerR.setCurrentFrame( 2-1);
			yposPR	-=1;
			if ( yposPR < 300-20)
				statePR	= 2;
		case 2:
			playerR.setCurrentFrame( 2-2);
			yposPR	-=4;
			if ( yposPR < 300-50)
				statePR	= 3;		// player goes down
			break;
		case 3:
			yposPR	+=5;
			if ( yposPR > 300)
				statePR	= 0;
			break;
	}

	// Update score board
	numb0.setCurrentFrame( count % 10);
	numb1.setCurrentFrame( count / 10);

	// Advance ball
	ball_t	+=ball_dt;


	// Put the players on screen
	playerL.setPosition(xposPL, yposPL);
	playerR.setPosition(xposPR, yposPR);

	// Put ball on screen
	ball.setPosition( HeaderCurve[ 2*ball_t],HeaderCurve[2*ball_t+1] - 85);

	// Check if we hit da ball
	if ( !fail) {
		if ( ball_t >= 820) {
			if ( yposPR < (300-35)) {
				sndKick.play();
				ball_dt *=-1;
				count--;
			} else {
				fail	= true;
			}
		}
		if ( ball_t < 170) {
			if ( yposPL < (300-35)) {
				sndKick.play();
				ball_dt = -ball_dt+2;		// Maybe *1.15 ?
				count--;
			} else {
				fail	= true;
			}
		}
	} else {
		if ( ball_t >= 1000  ||  ball_t <= 0) {
			done	= true;
		} else {
			if ( (ball_t > 930)  ||  (ball_t < 70))
				sndHitG.play();
		}
	}

	if ( ball_dt > 0)
		ball_ang	+=ball_dt;
	else
		ball_ang	+=ball_dt;

	ball_ang	%=360;
	ball.setRotation( ball_ang);
}

