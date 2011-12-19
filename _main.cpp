#include "MyGame.h"

int main(int argc, char** argv)
{
    Engine* game = NULL;
	ErrorHandler error;

	//	Setup game engine
	game = new MyGame;
    PenjinErrors::PENJIN_ERRORS e;
	cout << error.getErrorString(game->argHandler(argc,argv));
	e = game->penjinInit();
	cout << error.getErrorString(e);

    if(e == PENJIN_OK)
        while(game->stateLoop());	//	Perform main loop until shutdown


	cout << error.getErrorString(PENJIN_SHUTDOWN);

	//	Tidy up
    if(game)
    {
        delete game;
        game = NULL;
    }
    cout << error.getErrorString(PENJIN_GOODBYE);
	return 0;	//	Normal program termination.
}
