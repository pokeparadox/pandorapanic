#include "StateFire.h"

Fire::Fire()
{
    pauseText.loadFont("font/bip.ttf", 32);
    text.loadFont("font/chromo.ttf", 48);
    text.setColour(Colour(YELLOW));

	background.loadBackground( "images/Fire/background.png"  );
	sprCannon.loadSprite(	   "images/Fire/cannon.png"		 );
	imgFire.loadImageSheet(	   "images/Fire/fire.png", FIRE_SPRITE_FIRE_FRAMES, FIRE_SPRITE_FIRE_ROWS );
	imgFloor.loadImageSheet(   "images/Fire/floor.png", FIRE_SPRITE_FLOOR_FRAMES, FIRE_SPRITE_FLOOR_ROWS );
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

    water.setMax(FIRE_MAX_WATERPARTS);
    water.setInvisible(0);
    water.setColour(BLUE);
    water.setMaxVelocity(Vector2df(FIRE_MAX_VEL_DN/3.0f,FIRE_MAX_VEL_DN));
    water.setLifeTime(10000);
    water.setGravity(FIRE_GRAVITY);
    water.setPosition( Vector2di(sprCannon.getX(), sprCannon.getY()) );
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
            shooting = true;
            ShootWater();
			cannon.water -= FIRE_MAX_WATERUSE;
			if (cannon.water < 0)
				cannon.water = 0;
        }
	}

	if (!shooting)
	{
        moving = false;
        // Adjust Angle
        if(input->isLeft())
        {
			moving = true;
			cannon.angle -= 1;
			if (cannon.angle < -FIRE_MAX_ANGLE)
                cannon.angle = -FIRE_MAX_ANGLE;
        }
        if(input->isRight())
        {
			moving = true;
			cannon.angle += 1;
			if (cannon.angle > FIRE_MAX_ANGLE)
                cannon.angle = FIRE_MAX_ANGLE;
        }
        // Adjust Power
        if(input->isDown())
        {
			moving = true;
			cannon.power -= 1;
        }
        if(input->isUp())
        {
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
    }

    void Fire::pauseScreen(SDL_Surface* screen)
    {
        pauseSymbol(screen);
        pauseText.setPosition(50,180);
        pauseText.setColour(WHITE);
        pauseText.print(screen, "Put out the fires! press A to shoot");
    }
#else
    void Fire::render()
    {
        background.render();
        RenderFloorsAndFires();
        RenderCannon();
    }

    void Fire::pauseScreen()
    {
        pauseSymbol();
        pauseText.setPosition(50,180);
        pauseText.setColour(WHITE);
        pauseText.print("Put out the fires! press A to shoot");
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
    //MovePlayer();
    result = CheckGameConditions();
    if (result != RESULT_NONE)
    {
        if (GameResult(result) == FIRE_PASS)
        {
            variables[0].setInt(1);
        }
        else
        {
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

	int8_t FireLevels[2][2][FIRE_MAX_TOWERS] = {
                                 {{ 5, 4, 3, 2, 1, 1, 2, 3, 4, 5 },
                                  { 0, 1, 2, 0, 1, 2, 0, 1, 2, 0 }},
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
        towers[i].fire.y            = towers[i].y - towers[i].floors.size() * imgFloor.getHeight();
        towers[i].fire.size         = FireLevels[lvl][1][i];
        towers[i].fire.regentimer   = FIRE_MAX_FIRETIMER;

        towers[i].smokeblack.setMax(FIRE_MAX_SMOKEPARTS);
        towers[i].smokeblack.setInvisible(0);
        towers[i].smokeblack.setColour(BLACK);
        towers[i].smokeblack.setMaxVelocity(Vector2df(FIRE_MAX_VEL_DN/3.0f,FIRE_MAX_VEL_DN));
        towers[i].smokeblack.setLifeTime(5000);
        towers[i].smokeblack.setGravity(0);
        towers[i].smokeblack.setPosition(Vector2di(towers[i].fire.x, towers[i].fire.y));

        towers[i].smokewhite.setMax(FIRE_MAX_SMOKEPARTS);
        towers[i].smokewhite.setInvisible(0);
        towers[i].smokewhite.setColour(WHITE);
        towers[i].smokewhite.setMaxVelocity(Vector2df(FIRE_MAX_VEL_DN/3.0f,FIRE_MAX_VEL_DN));
        towers[i].smokewhite.setLifeTime(5000);
        towers[i].smokewhite.setGravity(0);
        towers[i].smokewhite.setPosition(Vector2di(towers[i].fire.x, towers[i].fire.y));
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
			if ( towers[i].fire.size != 0 )
				checkfires = false;

			for ( j=0; j<towers[i].floors.size(); j++ )
			{
				if ( towers[i].floors.at(j) > 0 )
					checkfloors = false;
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
			final_result = FIRE_FAIL;
			break;
		case RESULT_NOFIRES:			// Pass no fires left
			final_result = FIRE_PASS;
			break;
		case RESULT_NOFLOORS:			// Fail no floors left
			final_result = FIRE_FAIL;
			break;
		default:
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
	}
}

void Fire::FloorBurn( void )
{
	uint8_t i,j;
	uint8_t currentfloor;

	for ( i=0; i<FIRE_MAX_TOWERS; i++ )
	{
		// Get the current floor on fire
		for ( j=towers[i].floors.size()-1; j>=0; j-- )
		{
			if ( towers[i].floors.at(j) > 0 ) {
				currentfloor = j;
				break;
			}
		}

		// Check the burn timer
		if ( towers[i].burntimer <= 0 )
		{
			towers[i].burntimer		 = FIRE_MAX_BURNTIMER;
			towers[i].floors.at(currentfloor)	-= FIRE_MAX_FIRESHOT;
			if ( towers[i].floors.at(currentfloor) < 0 )
				towers[i].floors.at(currentfloor) = 0;
		}
		else {
			towers[i].burntimer--;
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
                towers[i].smokeblack.setShouldStopNew(true);
                towers[i].smokewhite.setShouldStopNew(false);
            }
            else
            {
            #endif
                // No collision emit black smoke
                towers[i].smokeblack.setShouldStopNew(false);
                towers[i].smokewhite.setShouldStopNew(true);
            //}

        }
        else
        {
            // No fire no smoke
            towers[i].smokeblack.setShouldStopNew(true);
            towers[i].smokewhite.setShouldStopNew(true);
        }

        towers[i].smokeblack.update();
        towers[i].smokewhite.update();
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
                tile = 0;
                if ( towers[i].floors.at(j) >= FIRE_MAX_FLOORLIFE-(FIRE_MAX_FLOORLIFE/3) )
                {
                    tile = 0;
                }
                else if ( towers[i].floors.at(j) > FIRE_MAX_FLOORLIFE/3 && towers[i].floors.at(j) < FIRE_MAX_FLOORLIFE-(FIRE_MAX_FLOORLIFE/3) )
                {
                    tile = 1;
                }
                else if ( towers[i].floors.at(j) > 0 && towers[i].floors.at(j) < (FIRE_MAX_FLOORLIFE/3) )
                {
                    tile = 2;
                }
                else if ( towers[i].floors.at(j) == 0 )
                {
                    tile = 3;
                }
                imgFloor.renderImage( tile, screen, towers[i].x, towers[i].y-imgFloor.getHeight()*j );
            }

            // Draw the fire
            tile = 0;
            if ( towers[i].fire.size >= FIRE_MAX_FIRESIZE-(FIRE_MAX_FIRESIZE/3) )
            {
                tile = 0;
            }
            else if ( towers[i].fire.size > (FIRE_MAX_FIRESIZE/3) && towers[i].fire.size < FIRE_MAX_FIRESIZE-(FIRE_MAX_FIRESIZE/3) )
            {
                tile = 1;
            }
            else if ( towers[i].fire.size > 0 && towers[i].fire.size < (FIRE_MAX_FIRESIZE/3) )
            {
                tile = 2;
            }
            imgFire.renderImage( tile, screen, towers[i].fire.x, towers[i].fire.y );

            // Draw the smoke
            towers[i].smokeblack.render(screen);
            towers[i].smokewhite.render(screen);
        }
    }

    void Fire::RenderCannon( SDL_Surface* screen )
    {
        sprCannon.setRotation(cannon.angle);
        sprCannon.render(screen);
        water.render(screen);
    }
#else
    void Fire::RenderFloorsAndFires()
    {
        uint8_t i,j;
        uint8_t tile;

        for ( i=0; i<FIRE_MAX_TOWERS; i++ )
        {
            // Render the floors
            for ( j=0; j<towers[i].floors.size(); j++ )
            {
                tile = 0;
                if ( towers[i].floors.at(j) >= FIRE_MAX_FLOORLIFE-(FIRE_MAX_FLOORLIFE/3) )
                {
                    tile = 0;
                }
                else if ( towers[i].floors.at(j) > FIRE_MAX_FLOORLIFE/3 && towers[i].floors.at(j) < FIRE_MAX_FLOORLIFE-(FIRE_MAX_FLOORLIFE/3) )
                {
                    tile = 1;
                }
                else if ( towers[i].floors.at(j) > 0 && towers[i].floors.at(j) < (FIRE_MAX_FLOORLIFE/3) )
                {
                    tile = 2;
                }
                else if ( towers[i].floors.at(j) == 0 )
                {
                    tile = 3;
                }
                imgFloor.renderImage( tile, towers[i].x, towers[i].y-imgFloor.getHeight()*j );
            }

            // Draw the fire
            tile = 0;
            if ( towers[i].fire.size >= FIRE_MAX_FIRESIZE-(FIRE_MAX_FIRESIZE/3) )
            {
                tile = 0;
            }
            else if ( towers[i].fire.size > (FIRE_MAX_FIRESIZE/3) && towers[i].fire.size < FIRE_MAX_FIRESIZE-(FIRE_MAX_FIRESIZE/3) )
            {
                tile = 1;
            }
            else if ( towers[i].fire.size > 0 && towers[i].fire.size < (FIRE_MAX_FIRESIZE/3) )
            {
                tile = 2;
            }
            imgFire.renderImage( tile, towers[i].fire.x, towers[i].fire.y );

            // Draw the smoke
            towers[i].smokeblack.render();
            towers[i].smokewhite.render();
        }
    }

    void Fire::RenderCannon(  )
    {
        sprCannon.setRotation(cannon.angle);
        sprCannon.render();
        water.render();
    }

#endif

void Fire::ShootWater()
{
    //acceleration.x  =  power * LUT::Lsin(-angle);
    //acceleration.y += -power * LUT::Lcos( angle);
    //velocity+=acceleration;
}
