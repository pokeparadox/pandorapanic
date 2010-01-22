#include "MyGame.h"

MyGame::MyGame()
{
	//	Default constructor
	Penjin::setApplicationName("PandoraPanic");
}

MyGame::~MyGame()
{
    ACHIEVEMENTS->save("achieve.ach");
}

PENJIN_ERRORS MyGame::init()
{
    GFX::setResolution(800,480);    // Pandora resolution on all platforms!
    //  We have to setup the screen beforehand since code in the h file below tries to get screen res.
    PENJIN_ERRORS e = GFX::resetScreen();
    #include "AchievementSetup.h"
    setInitialState(STATE_TITLE);
	return e;
}

void MyGame::stateManagement()
{
	//	Check if the state itself wants to change states
	uint next = state->getNextState();
	if(state)
	{
		delete state;
		state = NULL;
	}
    #include "MiniGameStates.h"
}

/*PENJIN_ERRORS MyGame::argHandler(int argc, char **argv)
{
	//	This is just an example of how to handle commandlines, you would override this depending on actual needs.
	if(argc <= 1)
		return	PENJIN_NO_COMMANDLINE;
	else
	{
		//	Do further CMD processing
		for (int arg = 1; arg < argc; ++arg)
		{
			//	Check for commandline escape chars
			if(argv[arg][0] == '-' || argv[arg][0] == '/')
			{
				//	Look for arguements
				switch(argv[arg][1])
				{
				    //  Load CMF - Custom controls
				    case 'C':
				    {
				        customControlMap = argv[arg+1];
				        break;
                    }
					//	Set Fullscreen
					case 'F':
					{
						fullScreen = true;
						break;
					}
					//	Set xRes
					case 'x':
					case 'X':
					{
						xRes = cStringToInt(argv[arg+1]);
						break;
					}
					//  Set yRes
					case 'y':
					case 'Y':
					{
                        yRes = cStringToInt(argv[arg+1]);
					    break;
					}
					default:
					{
						return PENJIN_INVALID_COMMANDLINE;
					}
				}
			}
		}
	}
	return	PENJIN_OK;
}*/
