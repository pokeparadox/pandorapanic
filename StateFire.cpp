#include "StateFire.h"

Fire::Fire()
{
    LUT::init();

    debugText.loadFont("font/unispace.ttf", 12);
    debugText.setColour(BLACK);

    pauseText.loadFont("font/bip.ttf", 32);
    pauseText.setColour(WHITE);

    text.loadFont("font/chromo.ttf", 48);
    text.setColour(Colour(YELLOW));

	background.loadBackground( "images/Fire/background.png"  );
	sprCannon.loadSprite(	   "images/Fire/cannon.png"		 );
	imgFire.loadImageSheet(	   "images/Fire/fire.png", FIRE_SPRITE_FIRE_FRAMES, FIRE_SPRITE_FIRE_ROWS );
	imgFloor.loadImageSheet(   "images/Fire/floor.png", FIRE_SPRITE_FLOOR_FRAMES, FIRE_SPRITE_FLOOR_ROWS );

	printf( "Fire: Images loaded\n" );



}


Fire::~Fire()
{
}

void Fire::init()
{
    int8_t current_level = 0;

    SetupGame( current_level );

    sprCannon.setX((FIRE_SCREEN_WIDTH/2)-sprCannon.getWidth());
    sprCannon.setY(FIRE_SCREEN_HEIGHT-FIRE_CANNON_HEIGHT-sprCannon.getHeight());
	cannon.power		= FIRE_MAX_POWER/4;
	cannon.angle		= 0;
	cannon.water		= FIRE_MAX_WATER;
	cannon.shooting		= false;
	cannon.moving		= false;

    water.setShouldStopNew(true);
    water.setPosition( Vector2di(sprCannon.getX(), sprCannon.getY()) );
    water.setMax(FIRE_MAX_WATERPARTS);
    water.setColour(BLUE);
    water.setInvisible(0);
    water.setMaxVelocity(Vector2df(FIRE_MAX_VEL_DN/3.0f,FIRE_MAX_VEL_DN));
    water.setLifeTime(10);
    water.setGravity(FIRE_GRAVITY);

    printf( "Fire: Init complete loading level %d\n", current_level );
}

void Fire::userInput()
{
    input->update();
#ifdef PLATFORM_PC
    if(input->isQuit())
        nullifyState();
#endif

    if (input->isStart())
    {
        isPaused = !isPaused;
        input->resetKeys();
    }

    if (!moving)
	{
        shooting = false;
        // Shoot water
        if(input->isA() || input->isLeftClick() )
        {
            printf( "Fire: Water shooting\n" );
            shooting = true;
            water.setShouldStopNew(false);
            ShootWater();
			cannon.water -= FIRE_MAX_WATERUSE;
			if (cannon.water < 0) {
				cannon.water = 0;
                printf( "Fire: Water depleted\n" );
			}
        }
        else {
            water.setShouldStopNew(true);
        }
	}

	if (!shooting)
	{
        moving = false;
        // Adjust Angle
        if(input->isLeft())
        {
            printf( "Fire: Move cannon right\n" );
			moving = true;
			cannon.angle += 1;
			if (cannon.angle > FIRE_MAX_ANGLE)
                cannon.angle = FIRE_MAX_ANGLE;

        }
        if(input->isRight())
        {
            printf( "Fire: Move cannon left\n" );
			moving = true;
			cannon.angle -= 1;
			if (cannon.angle < -FIRE_MAX_ANGLE)
                cannon.angle = -FIRE_MAX_ANGLE;
        }
        // Adjust Power
        if(input->isDown())
        {
            printf( "Fire: Decrease cannon power\n" );
			moving = true;
			cannon.power -= 1;
        }
        if(input->isUp())
        {
            printf( "Fire: Increase cannon power\n" );
			moving = true;
			cannon.power += 1;
        }
	}

}

#ifdef PENJIN_SDL
    void Fire::render(SDL_Surface *screen)
    {
        background.render(screen);
        RenderFloorsAndFires(screen);
        RenderCannon(screen);
    #if FIRE_DEBUG
        RenderDebug(screen);
    #endif
    }

    void Fire::pauseScreen(SDL_Surface* screen)
    {
        pauseSymbol(screen);
        pauseText.setPosition(50,180);
        pauseText.print(screen, "Put out the fires! press A to shoot");
    }
#else
    void Fire::render()
    {
        background.render();
        RenderFloorsAndFires();
        RenderCannon();
        /*
    #if FIRE_DEBUG
        RenderDebug(screen);
    #endif
    */
    }

    void Fire::pauseScreen()
    {
        pauseSymbol();
        pauseText.setPosition(50,180);
        pauseText.print("Put out the fires! press A to shoot");
    }

    void Fire::RenderFloorsAndFires()
    {

    }

    void Fire::RenderCannon()
    {

    }
#endif
void Fire::onPause()
{
}

void Fire::onResume()
{
}

void Fire::update()
{
	results_t result = RESULT_NONE;

    FireRegen();
    FloorBurn();
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
}

/**
Fire Private Functions
**/

void Fire::SetupGame( int8_t lvl )
{
	int8_t i;
	Vector2df smoke_position;

	int8_t FireLevels[2][2][FIRE_MAX_TOWERS] = {
                                 {{ 5, 4, 3, 2, 1, 1, 2, 3, 4, 5 },
                                  { 10, 10, 20, 20, 40, 40, 80, 80, 100, 100 }},
                                 {{ 1, 2, 3, 4, 5, 5, 4, 3, 2, 1 },
                                  { 0, 1, 2, 0, 1, 2, 0, 1, 2, 0 }}
                                 };

    for (i=0; i<FIRE_MAX_TOWERS; i++)
    {
        towers[i].x                 = i*FIRE_SCREEN_WIDTH/FIRE_MAX_TOWERS;
        if( i >= FIRE_MAX_TOWERS/2 )
            towers[i].x += FIRE_TRUCK_WIDTH;
        towers[i].y                 = FIRE_SCREEN_HEIGHT-FIRE_GROUND_HEIGHT-imgFloor.getHeight();
        towers[i].floors.clear();
        towers[i].floors.resize(FireLevels[lvl][0][i], FIRE_MAX_FLOORLIFE);
        towers[i].burntimer         = FIRE_MAX_BURNTIMER;
        towers[i].fire.x            = towers[i].x;
        towers[i].fire.y            = towers[i].y - ((towers[i].floors.size()-1) * imgFloor.getHeight());
        towers[i].fire.size         = FireLevels[lvl][1][i];
        towers[i].fire.regentimer   = FIRE_MAX_FIRETIMER;

        towers[i].smoke.setShouldStopNew(true);
        towers[i].smoke.setPosition(Vector2di(towers[i].fire.x+(imgFloor.getWidth()/2), towers[i].fire.y));
        towers[i].smoke.setMax(1000);
        towers[i].smoke.setColour(BLACK);
        towers[i].smoke.setInvisible(0);
        towers[i].smoke.setMaxVelocity(Vector2df(0.5f,0.5f));
        towers[i].smoke.setLifeTime(1000);
        towers[i].smoke.setGravity(-0.001f);
        smoke_position = towers[i].smoke.getPosition();

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
	uint8_t i,j;
	bool checkfires		= true;
	bool checkwater		= true;
	bool checkfloors	= true;

	if ( cannon.water > 0 )
	{
		checkwater = false;
		for ( i=0; i<FIRE_MAX_TOWERS; i++ )
		{
			if ( towers[i].fire.size != 0 ) {
				checkfires = false;
			}

			for ( j=0; j<towers[i].floors.size(); j++ )
			{
				if ( towers[i].floors.at(j) > 0 ) {
					checkfloors = false;
				}
			}
		}
	}

	if ( checkwater )  return RESULT_NOWATER;
	if ( checkfires )  return RESULT_NOFIRES;
	if ( checkfloors ) return RESULT_NOFLOORS;
	return RESULT_NONE;
}

int8_t Fire::GameResult( results_t result )
{
	int8_t final_result;

	switch (result) {
		case RESULT_NOWATER:			// Fail no water left
            printf( "Fire: Fail no water left\n" );
			final_result = FIRE_FAIL;
			break;
		case RESULT_NOFIRES:			// Pass no fires left
            printf( "Fire: Pass no fires left\n" );
			final_result = FIRE_PASS;
			break;
		case RESULT_NOFLOORS:			// Fail no floors left
            printf( "Fire: Fail no floors left\n" );
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

	for ( i=0; i<FIRE_MAX_TOWERS; i++ )
	{
		if ( towers[i].fire.regentimer <= 0 )
		{
			towers[i].fire.regentimer	 = FIRE_MAX_FIRETIMER;
			towers[i].fire.size			+= FIRE_MAX_FIREREGEN;
			if ( towers[i].fire.size > FIRE_MAX_FIRESIZE ) {
				towers[i].fire.size = FIRE_MAX_FIRESIZE;
			}
		}
		else {
			towers[i].fire.regentimer--;
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
	uint8_t i,j;
	int8_t currentfloor;

	for ( i=0; i<FIRE_MAX_TOWERS; i++ )
	{
		// Get the current floor on fire
		if( towers[i].floors.size() > 0 )
		{
            currentfloor = -1;
            for ( j=0; j<towers[i].floors.size(); j++ )
            {
                if ( towers[i].floors.at(j) > 0 ) {
                    currentfloor = j;
                }
            }

            // Check the burn timer
            if ( currentfloor > -1 )
            {
                if ( towers[i].burntimer <= 0 )
                {
                    towers[i].burntimer = FIRE_MAX_BURNTIMER;               // Reset the timer
                    towers[i].floors.at(currentfloor) -= FIRE_MAX_FIRESHOT; // Take some of life from the floor
                    // Floor has no more life
                    if ( towers[i].floors.at(currentfloor) <= 0 ) {
                        printf( "Fire: Tower #%d Floor #%d has burned out\n", i, currentfloor );
                        towers[i].floors.at(currentfloor) = 0;
                        // Move fire down
                        towers[i].fire.y = towers[i].y - (currentfloor-1) * imgFloor.getHeight();
                    }
                }
                else {
                    towers[i].burntimer--;
                }
            }
#if FIRE_DEBUG
            char debug_temp[DEBUG_STRING_MAX];
            if ( currentfloor > -1 ) {
                sprintf( debug_temp, "T %.3d F %.3d L %.3d T %.3d\n", i, currentfloor, towers[i].floors.at(currentfloor), towers[i].burntimer );
            }
            else {
                strncpy( debug_temp, "No floors left\n", DEBUG_STRING_MAX );
            }
            strncpy( debug_floorstats[i], debug_temp, DEBUG_STRING_MAX );
#endif
		}
	}
}

void Fire::MoveParticles( void )
{
	int8_t i;

    water.update();

    for ( i=0; i<FIRE_MAX_TOWERS; i++ )
    {
        if( towers[i].fire.size > 0 )
        {
            #if 0
            if ( water.collide(towers[i].fire) )    // TODO NEED A COLLLIDE FUNCTION
            {
                // Collision emit white smoke
                towers[i].fire.size -= FIRE_MAX_WATERSHOT;
                towers[i].smoke.setShouldStopNew(false);
                towers[i].smoke.setColour(WHITE);
            }
            else
            {
            #endif
                // No collision emit black smoke
                towers[i].smoke.setShouldStopNew(false);
                towers[i].smoke.setColour(BLACK);
            //}

        }
        else
        {
            // No fire no smoke
            towers[i].smoke.setShouldStopNew(true);
        }

        towers[i].smoke.update();
    }
}

#ifdef PENJIN_SDL
void Fire::RenderFloorsAndFires( SDL_Surface* screen )
{
    uint8_t i,j;
	uint8_t tile;

	for ( i=0; i<FIRE_MAX_TOWERS; i++ )
	{
		// Render the floors
		for ( j=0; j<towers[i].floors.size(); j++ )
		{
			if ( towers[i].floors.at(j) > (FIRE_MAX_FLOORLIFE-(FIRE_MAX_FLOORLIFE/3)) )
			{
				tile = 0;
			}
			else if ( towers[i].floors.at(j) > (FIRE_MAX_FLOORLIFE/3) && towers[i].floors.at(j) <= (FIRE_MAX_FLOORLIFE-(FIRE_MAX_FLOORLIFE/3)) )
			{
				tile = 1;
			}
			else if ( towers[i].floors.at(j) > 0 && towers[i].floors.at(j) <= (FIRE_MAX_FLOORLIFE/3) )
			{
				tile = 2;
			}
			else if ( towers[i].floors.at(j) == 0 )
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
		if ( towers[i].floors.at(0) > 0 )
		{
		    towers[i].fire.frame++;
		    if( towers[i].fire.frame > FIRE_SPRITE_FIRE_FRAMES )
                towers[i].fire.frame = 0;

            if ( towers[i].fire.size > (FIRE_MAX_FIRESIZE-(FIRE_MAX_FIRESIZE/3)) )
            {
                tile = FIRE_SPRITE_FIRE_FRAMES*2+towers[i].fire.frame;
            }
            else if ( towers[i].fire.size > (FIRE_MAX_FIRESIZE/3) && towers[i].fire.size <= (FIRE_MAX_FIRESIZE-(FIRE_MAX_FIRESIZE/3)) )
            {
                tile = FIRE_SPRITE_FIRE_FRAMES*1+towers[i].fire.frame;
            }
            else if ( towers[i].fire.size > 0 && towers[i].fire.size <= (FIRE_MAX_FIRESIZE/3) )
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
        towers[i].smoke.render(screen);
	}
}

void Fire::RenderCannon( SDL_Surface* screen )
{
	sprCannon.setRotation(cannon.angle);
	sprCannon.render(screen);
	water.render(screen);
}

#if FIRE_DEBUG
void Fire::RenderDebug( SDL_Surface* screen )
{
    for( int i=0; i<FIRE_MAX_TOWERS; i++ )
    {
        debugText.setPosition(10,i*10);
        debugText.print(screen, debug_firestats[i]);

        debugText.setPosition(200,i*10);
        debugText.print(screen, debug_floorstats[i]);
    }
}
#endif
#endif
void Fire::ShootWater()
{
    #ifdef PENJIN_FIXED
        cannon.acceleration.x = cannon.power * LUT::Lsin(-fixedpoint::fix2int(cannon.angle));
        cannon.acceleration.y = -cannon.power * LUT::Lcos(fixedpoint::fix2int(cannon.angle));
    #else
        cannon.acceleration.x = cannon.power * LUT::Lsin(-cannon.angle);
        cannon.acceleration.y = -cannon.power * LUT::Lcos(cannon.angle);
    #endif
    water.setAcceleration(Vector2df(-cannon.acceleration.x,abs(cannon.acceleration.y)));
}
