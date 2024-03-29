#ifndef MINIGAMES_H_INCLUDED
#define MINIGAMES_H_INCLUDED
#include <string>
using std::string;
enum
{
    MAX_GAMES = 25,              //  The number of minigames we have
    EXTRA_LIFE_STAGES = 10,      //  The number of stages we need to pass for an extra life to be added.
    SUBSTATE_TRIGGER = 9         //  A number that signifies that a state is being loaded within another state.
};
enum MINIGAMES_LIST
{
    GAME_PANG=2,
    GAME_ARENA,
    GAME_BALLDROP,
    GAME_SPACEINVADERS,
    GAME_SPELLINGCONTEST,
    GAME_BADVISTA,
    GAME_DISTRACTEDMATH,
    GAME_REFLEX,
    //GAME_DOUBLENUBTROUBLE,
    GAME_PUNCHWRESTLER,
    GAME_PONG,
    GAME_ONEBUTTONBANDIT,
    GAME_PANICATTACK,
    GAME_PANJOUST,
    GAME_JUMPER,
    GAME_MAZE,
    GAME_PUTTPUTT,
    GAME_ROT_DEF,
    GAME_MEMORY_BLOCKS,
    GAME_PLANDER,
    GAME_SNATCHABEER,
    GAME_COUNTJOB,
    GAME_FIRE,
    GAME_CAKE,
    GAME_TESTSIGHT,
    GAME_HEADER,
    //GAME_DARKTREASURE,
    //GAME_PEEPEESBIGADVENTURE,
    //GAME_BARRELROLL,
    //GAME_PANDORAPANIC,
    GAME_TILEEDITOR,
};

static string MINIGAMES_NAMES[]=
{
    "Pang",
    "Arena",
    "BallDrop",
    "Space Invaders",
    "Spelling Contest",
    "BadVista",
    "Distracted Maths",
    "Reflex",
    //"Double Nub Trouble",
    "PunchWrestler",
    "Pong",
    "One Button Bandit",
    "Panic Attack",
    "PanJoust",
    "Jumper",
    "Maze",
    "PuttPutt",
    "Rotating Turret",
    "Memory Blocks",
    "Plander",
    "Snatch A Beer",
    "The Count Job",
    "Fire",
    "Birthday Cake",
    "Test Your Sight",
    "Header",
    //"Pee Pee's Big Adventure",
    //"Dark Treasure",
    //"Do A Barrel Roll!",
    //"Pandora Panic!",
    "Tile Editor",
};

#endif // MINIGAMES_H_INCLUDED
