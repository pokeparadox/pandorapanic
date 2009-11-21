#include "StateFire.h"

// Screen default values
#define     SCREEN_WIDTH_DEFAULT    800
#define     SCREEN_HEIGHT_DEFAULT   480

// Vel and Accel Parameters
#define     WATER_GRAVITY           1.0f
#define     SMOKE_GRAVITY           -0.05f
#define     SMOKE_MAX_VEL_UP        0.5f
#define     SMOKE_MAX_VEL_DN        0.5f
#define     PICKLE_VELOCITY         3
#define     PICKLE_FIRE_VELOCITY    5
#define     PICKLE_LEFT             0
#define     PICKLE_RIGHT            1

// Game Parameters
#ifdef PENJIN_FIXED
#define     CANNON_ANGLE_MAX        (Fixed)(45)
#else
#define     CANNON_ANGLE_MAX        45 // in both directions -45 to +45
#endif
#define     CANNON_CHGRATE_MAX      10
#define     CANNON_ANGLE_CHGRATE    8
#define     CANNON_POWER_CHGRATE    5
#define     CANNON_POWER_MIN		5
#define     CANNON_POWER_MAX		40
#define     WATER_MAX		        2000
#define     WATER_SHOT	            1
#define     WATER_DAMAGE	        2
#define     SMOKE_PARTICLES         100
#define     SMOKE_LIFETIME           500
#define     SMOKE_WHITE_EMITTIME    1000
#define     FIRE_SIZE_MAX	        100
#define     FIRE_DAMAGE_SMALL   	5
#define     FIRE_DAMAGE_MEDIUM	    10
#define     FIRE_DAMAGE_LARGE       15
#define     FIRE_REGEN_MAX	        5
#define     FIRE_REGENTIMER_MAX	    100
#define     FLOOR_HP_MAX	        100
#define     FLOOR_BURNTIMER_MAX	    50
#define     PICKLE_RAND_MIN         0
#define     PICKLE_RAND_MAX         20

// Sprite Parameters
#define     FIRE_SPRITE_ROWS        3
#define     FIRE_SPRITE_FRAMES      4
#define     FLOOR_SPRITE_ROWS       4
#define     FLOOR_SPRITE_FRAMES     1
#define     PICKLE_SPRITE_ROWS      1
#define     PICKLE_SPRITE_FRAMES    4
#define     PICKLE_SPRITE_LPF       5  // Loops per frame
#define     GROUND_PIXEL_HEIGHT     10
#define     CANNON_PIXEL_HEIGHT     50
#define     CANNON_PIXEL_WIDTH      20

Fire::Fire()
{
    //firstrun = true;

    LUT::init();
#if FIRE_DEBUG
    debugText.loadFont("font/unispace.ttf", 12);
    debugText.setColour(BLACK);
#endif
    pauseText.loadFont("font/bip.ttf", 24);
    pauseText.setColour(RED);

    text.loadFont("font/chromo.ttf", 48);
    text.setColour(Colour(YELLOW));

	background.loadBackground( "images/Fire/background.png"  );
	sprCannon.loadSprite(	   "images/Fire/cannon.png"		 );
    sprWater.loadSprite(       "images/Fire/water.png" );
	imgFire.loadImageSheet(	   "images/Fire/fire.png", FIRE_SPRITE_FRAMES, FIRE_SPRITE_ROWS );
	imgFloor.loadImageSheet(   "images/Fire/floor.png", FLOOR_SPRITE_FRAMES, FLOOR_SPRITE_ROWS );
	imgPickle.loadImageSheet(  "images/Fire/pickle.png", PICKLE_SPRITE_FRAMES, PICKLE_SPRITE_ROWS );

	printf( "Fire: Images loaded\n" );
}

Fire::~Fire()
{
    LUT::deInit();
}

void Fire::init()
{
    // use to get stages
    int8_t current_level = 0;
    SDL_Surface* screen;

    screen = SDL_GetVideoSurface();
    if (screen)
    {
        printf( "Fire: Screen size of %dx%d detected\n", screen->w, screen->h );
        screen_width  = screen->w;
        screen_height = screen->h;
    }
    else
    {
        printf( "Fire: Error getting screen\n" );
        screen_width  = SCREEN_WIDTH_DEFAULT;
        screen_height = SCREEN_HEIGHT_DEFAULT;
    }

    current_level = variables[2].getInt();
    SetupGame( current_level );

    sprCannon.setX((screen_width/2)-sprCannon.getWidth());
    sprCannon.setY(screen_height-CANNON_PIXEL_HEIGHT-sprCannon.getHeight());
	cannon.power		= CANNON_POWER_MAX/4;
	cannon.angle		= 0;
	cannon.chgrate      = 0;
	cannon.water		= WATER_MAX;
	cannon.shooting		= false;
	cannon.moving		= false;

	pickle.state = STATE_INACTIVE;

    water.clear();

    printf( "Fire: Init complete loading level %d\n", current_level );
}

void Fire::userInput()
{
    input->update();
#ifdef PLATFORM_PC
    if (input->isQuit())
        nullifyState();
#endif

    if (input->isStart())
    {
        pauseToggle();
        input->resetKeys();
    }

    if (!cannon.moving)
	{
        cannon.shooting = false;
        // Shoot water
        if (input->isA() || input->isLeftClick())
        {
            cannon.shooting = true;
            ShootWater();
			cannon.water -= WATER_SHOT;
			if (cannon.water < 0)
			{
				cannon.water = 0;
                printf( "Fire: Water depleted\n" );
			}
        }
	}

	if (!cannon.shooting)
	{
        cannon.moving = false;
        // Adjust Angle
        if (input->isLeft())
        {
			cannon.moving = true;
			cannon.chgrate++;
			if (cannon.chgrate > CANNON_CHGRATE_MAX)
			{
                cannon.chgrate = 0;
                cannon.angle += CANNON_ANGLE_CHGRATE;
			}
			if (cannon.angle > CANNON_ANGLE_MAX)
                cannon.angle = CANNON_ANGLE_MAX;

        }
        if (input->isRight())
        {
			cannon.moving = true;
			cannon.chgrate++;
			if (cannon.chgrate > CANNON_CHGRATE_MAX)
			{
                cannon.chgrate = 0;
                cannon.angle -= CANNON_ANGLE_CHGRATE;
			}
			if (cannon.angle < -CANNON_ANGLE_MAX)
                cannon.angle = -CANNON_ANGLE_MAX;
        }
        // Adjust Power
        if (input->isDown())
        {
			cannon.moving = true;
			cannon.chgrate++;
			if (cannon.chgrate > CANNON_CHGRATE_MAX)
			{
                cannon.chgrate = 0;
                cannon.power -= CANNON_POWER_CHGRATE;
			}
			if (cannon.power < CANNON_POWER_MIN)
                cannon.power = CANNON_POWER_MIN;
        }
        if (input->isUp())
        {
			cannon.moving = true;
			cannon.chgrate++;
			if (cannon.chgrate > CANNON_CHGRATE_MAX)
			{
                cannon.chgrate = 0;
                cannon.power += CANNON_POWER_CHGRATE;
			}
			if (cannon.power > CANNON_POWER_MAX)
                cannon.power = CANNON_POWER_MAX;
        }
	}
}

void Fire::render(SDL_Surface *screen)
{
	background.render(screen);
	RenderSprites(screen);
	RenderCannon(screen);
#if FIRE_DEBUG
	RenderDebug(screen);
#endif
}

void Fire::pauseScreen(SDL_Surface* screen)
{
    #define PAUSE_X         50
    #define PAUSE_Y         180
    #define PAUSE_DELTA_X   20
    #define PAUSE_DELTA_Y   30

    if(variables.size()<SUBSTATE_TRIGGER)
        pauseSymbol(screen);
    pauseText.setColour(BLACK);
    pauseText.setPosition(PAUSE_X,PAUSE_Y);
    pauseText.print(screen, "Put out all of the fires before any building burns down.");
    pauseText.setPosition(PAUSE_X,PAUSE_Y+PAUSE_DELTA_Y*1);
    pauseText.print(screen, "Controls:");
    pauseText.setPosition(PAUSE_X+PAUSE_DELTA_X,PAUSE_Y+PAUSE_DELTA_Y*2);
    pauseText.print(screen, "Left/Right Arrows or Dpad rotate cannon");
    pauseText.setPosition(PAUSE_X+PAUSE_DELTA_X,PAUSE_Y+PAUSE_DELTA_Y*3);
    pauseText.print(screen, "Up/Down Arrows or Dpad change cannon power");
    pauseText.setPosition(PAUSE_X+PAUSE_DELTA_X,PAUSE_Y+PAUSE_DELTA_Y*4);
    pauseText.print(screen, "Z key or A button shoot water");
    pauseText.setPosition(PAUSE_X+PAUSE_DELTA_X*8,PAUSE_Y+PAUSE_DELTA_Y*6);
    pauseText.print(screen, "Press Enter/Start to continue");
}

void Fire::update()
{
	results_t result = RESULT_NONE;

    FireRegen();
    FloorBurn();
    PickleAI();
    MoveParticles();

    result = CheckGameConditions();
    if (result != RESULT_NONE)
    {
        if (GameResult(result) == FIRE_PASS)
        {
            printf( "Fire: Player Pass\n" );
            variables[0].setInt(1);
        }
        else
        {
            printf( "Fire: Player Fail\n" );
            variables[0].setInt(0);
        }
        setNextState(STATE_MAIN);
    }

    /*if (firstrun)
    {
        isPaused=true;
        firstrun = false;
    }*/
}

/**
Fire Private Functions
**/

#define FMAX FIRE_MAX_FLOORS
void Fire::SetupGame( int8_t lvl )
{
	int8_t i;
	int8_t tower = 0;
	int8_t firelevel[2][FIRE_MAX_TOWERS] = {{ FMAX, FMAX, FMAX, FMAX, FMAX, FMAX, FMAX, FMAX, FMAX, FMAX },
                                            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                           };
	Vector2df smoke_position;

    // Build a level based on the current stage
    for (i=0; i<lvl; i++)
    {
        if (firelevel[0][tower] > 2)
        {
            firelevel[0][tower]--;
        }
        if (firelevel[1][tower] < FIRE_SIZE_MAX-(FIRE_SIZE_MAX/10))
        {
            firelevel[1][tower] += (FIRE_SIZE_MAX/10);
        }
        tower++;
        if (tower>FIRE_MAX_TOWERS)
        {
            tower = 0;
        }
    }

    for (i=0; i<FIRE_MAX_TOWERS; i++)
    {
        towers[i].x                 = i*screen_width/FIRE_MAX_TOWERS;
        if (i >= FIRE_MAX_TOWERS/2)
            towers[i].x += CANNON_PIXEL_WIDTH;
        towers[i].y                 = screen_height-GROUND_PIXEL_HEIGHT-imgFloor.getHeight();
        towers[i].floors.clear();
        towers[i].floors.resize(firelevel[0][i], FLOOR_HP_MAX);
        towers[i].burntimer         = FLOOR_BURNTIMER_MAX;
        towers[i].fire.x            = towers[i].x;
        towers[i].fire.y            = towers[i].y-((towers[i].floors.size()-1)*imgFloor.getHeight());
        towers[i].fire.size         = firelevel[1][i];
        towers[i].fire.regentimer   = FIRE_REGENTIMER_MAX;
        towers[i].fire.frame        = 0;

        towers[i].smokeblack.setShouldStopNew(true);
        towers[i].smokeblack.setMax(SMOKE_PARTICLES);
        towers[i].smokeblack.loadSprite( "images/Fire/bsmoke.png" );
        towers[i].smokeblack.setPosition(Vector2di(towers[i].fire.x+(imgFloor.getWidth()/2), towers[i].fire.y));
        towers[i].smokeblack.setInvisible(0);
        towers[i].smokeblack.setMaxVelocity(Vector2df(SMOKE_MAX_VEL_UP, SMOKE_MAX_VEL_DN));
        towers[i].smokeblack.setLifeTime(SMOKE_LIFETIME);
        towers[i].smokeblack.setGravity(SMOKE_GRAVITY);
        smoke_position = towers[i].smokeblack.getPosition();

        towers[i].smokewhite.setShouldStopNew(true);
        towers[i].smokewhite.setMax(SMOKE_PARTICLES);
        towers[i].smokewhite.loadSprite( "images/Fire/wsmoke.png" );
        towers[i].smokewhite.setPosition(Vector2di(towers[i].fire.x+(imgFloor.getWidth()/2), towers[i].fire.y));
        towers[i].smokewhite.setInvisible(0);
        towers[i].smokewhite.setMaxVelocity(Vector2df(SMOKE_MAX_VEL_UP, SMOKE_MAX_VEL_DN));
        towers[i].smokewhite.setLifeTime(SMOKE_LIFETIME);
        towers[i].smokewhite.setGravity(SMOKE_GRAVITY);
        towers[i].smoketimer = 0;

        printf( "Fire: Tower #%d with %d floors at %dx%d\n", i, towers[i].floors.size(), towers[i].x, towers[i].y );
        printf( "Fire:       with Fire at  %dx%d\n", towers[i].fire.x, towers[i].fire.y );
        printf( "Fire:       with Smoke at %fx%f\n", smoke_position.x, smoke_position.y );

#if FIRE_DEBUG
        strncpy( debug_firestats[i], "Empty\n", DEBUG_STRING_MAX );
        strncpy( debug_floorstats[i], "Empty\n", DEBUG_STRING_MAX );
#endif
    }
}

results_t Fire::CheckGameConditions( void )
{
	int8_t i;
	bool checkfires		= true;
	bool checkwater		= true;
	bool checkfloors	= true;

    // Check if there water left
	if (cannon.water > 0)
	{
		checkwater = false;
		for (i=0; i<FIRE_MAX_TOWERS; i++)
		{
		    // Check if fire is still burning
			if (towers[i].fire.size > 0)
			{
				checkfires = false;
			}

            // Check if bottom floor is still not destroyed
            if (towers[i].floors.at(0) > 0)
            {
                checkfloors = false;
            }
            else
            {
                checkfloors = true;
                break;
            }
		}
	}

	if (checkwater)  return RESULT_NOWATER;
	if (checkfires)  return RESULT_NOFIRES;
	if (checkfloors) return RESULT_NOFLOOR;
	return RESULT_NONE;
}

int8_t Fire::GameResult( results_t result )
{
	int8_t final_result;

	switch (result)
	{
		case RESULT_NOWATER:
            printf( "Fire: Fail no water left\n" );
			final_result = FIRE_FAIL;
			break;
		case RESULT_NOFIRES:
            printf( "Fire: Pass no fires left\n" );
			final_result = FIRE_PASS;
			break;
		case RESULT_NOFLOOR:
            printf( "Fire: Fail a building burned down\n" );
			final_result = FIRE_FAIL;
			break;
		default:
            printf( "Fire: Fail you shouldnt be here\n" );
			final_result = FIRE_FAIL;
	}
	return final_result;
}

void Fire::FireRegen( void )
{
	int8_t i;

	for (i=0; i<FIRE_MAX_TOWERS; i++)
	{
	    if (towers[i].fire.size > 0)
	    {
            if (towers[i].fire.regentimer <= 0)
            {
                towers[i].fire.regentimer	 = FIRE_REGENTIMER_MAX;
                towers[i].fire.size			+= FIRE_REGEN_MAX;
                if (towers[i].fire.size > FIRE_SIZE_MAX)
                {
                    towers[i].fire.size = FIRE_SIZE_MAX;
                }
            }
            else
            {
                towers[i].fire.regentimer--;
            }
	    }
#if FIRE_DEBUG
        char debug_temp[DEBUG_STRING_MAX];
        sprintf( debug_temp, "T %.3d S %.3d T %.3d\n", i, towers[i].fire.size, towers[i].fire.regentimer );
        strncpy( debug_firestats[i], debug_temp, DEBUG_STRING_MAX );
#endif
	}
}

void Fire::FloorBurn( void )
{
	uint8_t i, j;
	int8_t currentfloor;

	for (i=0; i<FIRE_MAX_TOWERS; i++)
	{
        currentfloor = -1;
		// Get the current floor on fire
        if (towers[i].fire.size > 0 && towers[i].floors.size() > 0)
        {
            for (j=0; j<towers[i].floors.size(); j++)
            {
                if (towers[i].floors.at(j) > 0)
                {
                    currentfloor = j;
                }
            }

            // Check the burn timer
            if (currentfloor > -1)
            {
                if (towers[i].burntimer <= 0)
                {
                    towers[i].burntimer = FLOOR_BURNTIMER_MAX;               // Reset the timer

                    // Take some of life from the floor
                    if (towers[i].fire.size > (FIRE_SIZE_MAX-(FIRE_SIZE_MAX/3)))
                    {
                        towers[i].floors.at(currentfloor) -= FIRE_DAMAGE_LARGE;
                    }
                    else if (towers[i].fire.size > (FIRE_SIZE_MAX/3) && \
                             towers[i].fire.size <= (FIRE_SIZE_MAX-(FIRE_SIZE_MAX/3)))
                    {
                        towers[i].floors.at(currentfloor) -= FIRE_DAMAGE_MEDIUM;
                    }
                    else if (towers[i].fire.size > 0 && \
                             towers[i].fire.size <= (FIRE_SIZE_MAX/3))
                    {
                        towers[i].floors.at(currentfloor) -= FIRE_DAMAGE_SMALL;
                    }
                    else
                    {
                        printf( "Fire: Fire size was out of range tower: %d value: %d\n", i, towers[i].fire.size );
                        towers[i].floors.at(currentfloor) -= FIRE_DAMAGE_SMALL;
                    }

                    // Floor has no more life
                    if (towers[i].floors.at(currentfloor) <= 0)
                    {
                        printf( "Fire: Tower #%d Floor #%d has burned out\n", i, currentfloor );
                        towers[i].floors.at(currentfloor) = 0;
                        // Move fire down
                        towers[i].fire.y = towers[i].y - (currentfloor-1) * imgFloor.getHeight();
                    }
                }
                else
                {
                    towers[i].burntimer--;
                }
            }
        }
#if FIRE_DEBUG
        char debug_temp[DEBUG_STRING_MAX];

        sprintf( debug_temp, "T %.3d F %.3d T %.3d", i, currentfloor, towers[i].burntimer );
        strncpy( debug_floorstats[i], debug_temp, DEBUG_STRING_MAX );

        if (currentfloor < 0)
        {
            sprintf( debug_temp, " No floors left" );
            strcat( debug_floorstats[i], debug_temp );
        }
        else
        {
            sprintf( debug_temp, " L %.3d", towers[i].floors.at(currentfloor) );
            strcat( debug_floorstats[i], debug_temp );

            if (towers[i].fire.size <= 0)
            {
                sprintf( debug_temp, " Fire is out" );
                strcat( debug_floorstats[i], debug_temp );
            }
        }
        strcat( debug_floorstats[i], "\n" );
#endif
	}
}

void Fire::PickleAI( void )
{
    uint8_t i, j;
    uint8_t random1, random2;
    int8_t tower, floor;

    switch (pickle.state)
    {
        case STATE_INACTIVE:
            random1 = RandomValue( PICKLE_RAND_MIN, PICKLE_RAND_MAX );
            random2 = RandomValue( PICKLE_RAND_MIN, PICKLE_RAND_MAX );

            if (random1 == random2)
            {
                // Pick a tower to burn
                tower = -1;
                floor = -1;
                for (i=0; i<FIRE_MAX_TOWERS; i++)
                {
                    if (towers[i].fire.size <= 0 && towers[i].floors.size() > 1)
                    {
                        for (j=0; j<towers[i].floors.size(); j++)
                        {
                            if (towers[i].floors.at(j) > 0 && j > floor)
                            {
                                floor = j;
                                tower = i;
                            }
                        }
                    }
                }
                // Now go and start the fire again
                if (tower > -1 && floor > -1)
                {
                    pickle.state    = STATE_TOWERTO;
                    pickle.target   = tower;
                    pickle.lpf      = 0;
                    pickle.frame    = 0;

                    if (towers[pickle.target].x > screen_width/2)
                    {
                        pickle.dir      = PICKLE_LEFT;
                        pickle.x        = screen_width+imgPickle.getWidth();
                    }
                    else
                    {
                        pickle.dir      = PICKLE_RIGHT;
                        pickle.x        = -imgPickle.getWidth();
                    }
                    pickle.y        = screen_height-imgPickle.getHeight();
                }
            }
            break;
        case STATE_TOWERTO:
            if (pickle.dir == PICKLE_LEFT)
            {
                if (pickle.x <= towers[pickle.target].x+(int16_t)imgFloor.getWidth())    // Pickle is at or past the tower
                {
                    pickle.x        = towers[pickle.target].x+imgFloor.getWidth();
                    pickle.state    = STATE_FIRE_START;
                }
                else                            // Pickle is not at the tower yet
                {
                    pickle.x        -= PICKLE_VELOCITY;
                }
            }
            else
            {
                if (pickle.x >= towers[pickle.target].x)    // Pickle is at or past the tower
                {
                    pickle.x        = towers[pickle.target].x;
                    pickle.state    = STATE_FIRE_START;
                }
                else                            // Pickle is not at the tower yet
                {
                    pickle.x        += PICKLE_VELOCITY;
                }
            }
            break;
        case STATE_FIRE_START:
            pickle.fire.size    = FIRE_SIZE_MAX;
            pickle.fire.frame   = 0;
            pickle.fire.x       = towers[pickle.target].x;
            pickle.fire.y       = towers[pickle.target].y;
            pickle.state        = STATE_FIRE_ACTIVE;
            break;
        case STATE_FIRE_ACTIVE:
            if (pickle.fire.y <= towers[pickle.target].y- \
                                 (int16_t)((towers[pickle.target].floors.size()-1)*imgFloor.getHeight()))
            {
                towers[pickle.target].fire.size = FIRE_SIZE_MAX/4;
                pickle.state    = STATE_TOWERFROM;
            }
            else
            {
                pickle.fire.y   -= PICKLE_FIRE_VELOCITY;
            }
            break;
        case STATE_TOWERFROM:
            if (pickle.dir == PICKLE_LEFT)
            {
                if (pickle.x > screen_width)    // Pickle is off the screen now
                {
                    pickle.state    = STATE_INACTIVE;
                }
                else                            // Pickle is not off the screen yet
                {
                    pickle.x        += PICKLE_VELOCITY;
                }
            }
            else
            {
                if (pickle.x < -(int16_t)imgPickle.getWidth())    // Pickle is off the screen now
                {
                    pickle.state    = STATE_INACTIVE;
                }
                else                            // Pickle is not off the screen yet
                {
                    pickle.x        -= PICKLE_VELOCITY;
                }
            }
            break;
        default:
            printf( "Fire: Unknown pickle state %d\n", pickle.state );
            pickle.state = STATE_INACTIVE;
            break;
    }
}

void Fire::MoveParticles( void )
{
    int8_t i;
	vector<sWater>::iterator iter;
	SDL_Rect water_box, fire_box, screen_box;

    screen_box.x = 0;
    screen_box.y = 0;
    screen_box.w = screen->w;
    screen_box.h = screen->h;

    water_box.w = sprWater.getWidth();
    water_box.h = sprWater.getHeight();
    fire_box.w = imgFire.getWidth();
    fire_box.h = imgFire.getHeight();

    if (water.size() > 0)
    {
        for (iter = water.end()-1; iter >= water.begin(); iter--)
        {
            iter->vel_y += WATER_GRAVITY;
            iter->x += iter->vel_x;
            iter->y += iter->vel_y;

            water_box.x = iter->x;
            water_box.y = iter->y;

            for (i=0; i<FIRE_MAX_TOWERS; i++)
            {
                if (towers[i].fire.size > 0)
                {
                    fire_box.x = towers[i].fire.x;
                    fire_box.y = towers[i].fire.y;

                    if (CheckRectCollision( &water_box, &fire_box ))
                    {
                        // Collision emit white smoke
                        towers[i].fire.size -= WATER_DAMAGE;
                        towers[i].smoketimer = SMOKE_WHITE_EMITTIME;
                        water.erase(iter);
                        break;
                    }
                    else // No collision with fire
                    {
                        // If water is off the screen then remove
                        if (false == CheckRectCollision( &water_box, &screen_box ))
                        {
                            water.erase(iter);
                            break;
                        }
                    }

                    if (towers[i].smoketimer > 0)
                    {
                        towers[i].smoketimer--;
                        towers[i].smokewhite.setShouldStopNew(false);
                    }
                    else
                    {
                        towers[i].smoketimer = 0;
                        towers[i].smokewhite.setShouldStopNew(true);
                    }
                }
                else
                {
                    // No fire no smoke
                    towers[i].smokeblack.setShouldStopNew(true);
                    towers[i].smokewhite.setShouldStopNew(true);
                }
            }
        }
    }

    for (i=0; i<FIRE_MAX_TOWERS; i++)
    {
        towers[i].smokeblack.update();
    }
}

void Fire::RenderSprites( SDL_Surface* screen )
{
    uint8_t i,j;
	int8_t tile;
	vector<sWater>::iterator iter;

	for (i=0; i<FIRE_MAX_TOWERS; i++)
	{
		// Render the floors
		for (j=0; j<towers[i].floors.size(); j++)
		{
			if (towers[i].floors.at(j) > (FLOOR_HP_MAX-(FLOOR_HP_MAX/3)))
			{
				tile = 0;
			}
			else if (towers[i].floors.at(j) > (FLOOR_HP_MAX/3) && \
                     towers[i].floors.at(j) <= (FLOOR_HP_MAX-(FLOOR_HP_MAX/3)))
			{
				tile = 1;
			}
			else if (towers[i].floors.at(j) > 0 && \
                     towers[i].floors.at(j) <= (FLOOR_HP_MAX/3))
			{
				tile = 2;
			}
			else if (towers[i].floors.at(j) == 0)
			{
				tile = 3;
			}
			else
			{
                printf( "Fire: Floor life was out of range tower: %d floor: %d value: %d\n", i, j, towers[i].floors.at(j) );
                tile = 0;
			}
			imgFloor.renderImage( tile, screen, towers[i].x, towers[i].y-imgFloor.getHeight()*j );
		}

		// Draw the fire
		if (towers[i].fire.size > 0 && towers[i].floors.at(0) > 0)
		{
		    towers[i].fire.frame++;
		    if (towers[i].fire.frame >= FIRE_SPRITE_FRAMES)
		    {
                towers[i].fire.frame = 0;
		    }

            if (towers[i].fire.size > (FIRE_SIZE_MAX-(FIRE_SIZE_MAX/3)))
            {
                tile = (FIRE_SPRITE_FRAMES*2)+towers[i].fire.frame;
            }
            else if (towers[i].fire.size > (FIRE_SIZE_MAX/3) && \
                     towers[i].fire.size <= (FIRE_SIZE_MAX-(FIRE_SIZE_MAX/3)))
            {
                tile = (FIRE_SPRITE_FRAMES*1)+towers[i].fire.frame;
            }
            else if (towers[i].fire.size > 0 && \
                     towers[i].fire.size <= (FIRE_SIZE_MAX/3))
            {
                tile = towers[i].fire.frame;
            }
			else
			{
                printf( "Fire: Fire size was out of range tower: %d value: %d\n", i, towers[i].fire.size );
                tile = 0;
			}
            imgFire.renderImage( tile, screen, towers[i].fire.x, towers[i].fire.y );
		}
        // Draw the smoke
        towers[i].smokeblack.render(screen);
        towers[i].smokewhite.render(screen);
	}

    // Draw water
	for (iter = water.begin(); iter != water.end(); iter++)
    {
        sprWater.setPosition(iter->x, iter->y );
        sprWater.render();
    }

    // Draw pickle
    if (pickle.state != STATE_INACTIVE)
    {
        pickle.lpf++;
        if (pickle.lpf >= PICKLE_SPRITE_LPF)
        {
            pickle.lpf = 0;

            pickle.frame++;
            if (pickle.frame >= PICKLE_SPRITE_FRAMES)
            {
                pickle.frame = 0;
            }
        }
        imgPickle.renderImage( pickle.frame, screen, pickle.x, pickle.y );

        // Draw some fire
        if (pickle.state==STATE_FIRE_ACTIVE)
        {
            pickle.fire.frame++;
            if (pickle.fire.frame >= FIRE_SPRITE_FRAMES)
            {
                pickle.fire.frame = 0;
            }
            tile = (FIRE_SPRITE_FRAMES*2)+pickle.fire.frame;
            imgFire.renderImage( tile, screen, pickle.fire.x, pickle.fire.y );
        }
    }
}

void Fire::RenderCannon( SDL_Surface* screen )
{
    char cannon_label[20];

	sprCannon.setRotation(cannon.angle);
	sprCannon.render(screen);

#ifdef PENJIN_FIXED
    cannon.tip_x = sprCannon.getX() + sprCannon.getWidth()/2  + (sprCannon.getHeight()/2 * LUT::Lsin(-fixedpoint::fix2int(cannon.angle) );
    cannon.tip_y = sprCannon.getY() + sprCannon.getHeight()/2 - (sprCannon.getHeight()/2 * LUT::Lcos( fixedpoint::fix2int(cannon.angle) );
#else
    cannon.tip_x = sprCannon.getX() + sprCannon.getWidth()/2  + (sprCannon.getHeight()/2 * LUT::Lsin(-cannon.angle) );
    cannon.tip_y = sprCannon.getY() + sprCannon.getHeight()/2 - (sprCannon.getHeight()/2 * LUT::Lcos( cannon.angle) );
#endif

    #define CANNON_LABEL_X      500
    #define CANNON_LABEL_Y      10
    #define CANNON_LABEL_DELTAY 30

    sprintf( cannon_label, "Angle: %2d deg",  cannon.angle );
    pauseText.setPosition(CANNON_LABEL_X,CANNON_LABEL_Y);
    pauseText.print(screen, cannon_label);

    sprintf( cannon_label, "Power: %2.1f%%",  ((float)cannon.power/(float)CANNON_POWER_MAX)*100.0f );
    pauseText.setPosition(CANNON_LABEL_X,CANNON_LABEL_Y+CANNON_LABEL_DELTAY*1);
    pauseText.print(screen, cannon_label);

    sprintf( cannon_label, "Water: %2.1f%%",  ((float)cannon.water/(float)WATER_MAX)*100.0f );
    pauseText.setPosition(CANNON_LABEL_X,CANNON_LABEL_Y+CANNON_LABEL_DELTAY*2);
    pauseText.print(screen, cannon_label);
}

#if FIRE_DEBUG
void Fire::RenderDebug( SDL_Surface* screen )
{
    #define DEBUG_TEXT_X1       10
    #define DEBUG_TEXT_X2       200
    #define DEBUG_TEXT_Y1       0
    #define DEBUG_TEXT_Y2       150
    #define DEBUG_TEXT_DELTAY   10

    for (int i=0; i<FIRE_MAX_TOWERS; i++)
    {
        debugText.setPosition(DEBUG_TEXT_X1,DEBUG_TEXT_Y1+i*DEBUG_TEXT_DELTAY);
        debugText.print(screen, debug_firestats[i]);

        debugText.setPosition(DEBUG_TEXT_X2,DEBUG_TEXT_Y1+i*DEBUG_TEXT_DELTAY);
        debugText.print(screen, debug_floorstats[i]);
    }

    sprintf( debug_cannonstats, "A %.3d P %.3d W %.3d\n", cannon.angle, cannon.power, cannon.water );
    debugText.setPosition(DEBUG_TEXT_X1,DEBUG_TEXT_Y2);
    debugText.print(screen, debug_cannonstats);
}
#endif

void Fire::ShootWater()
{
    sWater shot;
#ifdef PENJIN_FIXED
    cannon.acceleration.x =  cannon.power * LUT::Lsin(-fixedpoint::fix2int(cannon.angle));
    cannon.acceleration.y = -cannon.power * LUT::Lcos(fixedpoint::fix2int(cannon.angle));
#else
    cannon.acceleration.x =  cannon.power * LUT::Lsin(-cannon.angle);
    cannon.acceleration.y = -cannon.power * LUT::Lcos(cannon.angle);
#endif

    shot.x = cannon.tip_x;
    shot.y = cannon.tip_y;
    shot.vel_x = cannon.acceleration.x;
    shot.vel_y = -abs(cannon.acceleration.y);

    water.push_back(shot);
}

bool Fire::CheckRectCollision( SDL_Rect* boxA, SDL_Rect* boxB )
{
    // The sides of the SDL_Rects
    int16_t leftA, leftB;
    int16_t rightA, rightB;
    int16_t topA, topB;
    int16_t bottomA, bottomB;

    // Calculate the sides of rec mCollisionbox
    leftA   = boxB->x;
    rightA  = boxB->x + boxB->w;
    topA    = boxB->y;
    bottomA = boxB->y + boxB->h;

    // Calculate the sides of rec box
    leftB   = boxA->x;
    rightB  = boxA->x + boxA->w;
    topB    = boxA->y;
    bottomB = boxA->y + boxA->h;

    // If any of the sides from mCollisionbox are outside of box
    if (bottomA <= topB) return false;
    if (topA >= bottomB) return false;
    if (rightA <= leftB) return false;
    if (leftA >= rightB) return false;
    // If none of the sides from mCollisionbox are outside box
    return true;    // Collision has occured
}

int16_t Fire::RandomValue( int16_t low, int16_t high )
{
    int16_t num = 0;

    // Return a random number between low and high
    num = rand() % (high - low + 1) + low;
    return num;
}

void Fire::pauseInput()
{
    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif
    if (input->isStart())
    {
        pauseToggle();
        input->resetKeys();
    }
}
