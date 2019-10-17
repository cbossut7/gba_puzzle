#ifndef LOGIC_H
#define LOGIC_H

#include "gba.h"



/*
* TA-TODO: Add any additional structs that you need for your app.
*
* For example, for a Snake game, one could be:
*
* typedef struct {
*   int heading;
*   int length;
*   int x;
*   int y;
* } Snake;
*
*/

typedef struct XO {
    int row;
    int col;
    int isX;
    int isButton;
    int draw;
} XO;

typedef struct Personnage {
    int row;
    int col;
    int prevRow;
    int prevCol;
    int isSide; //isSide & isFront = face right, isSide & !isFront = face left.
    int isFront;
    XO *prevTile;
    int onTile;
    int sliding;
} psnage;

typedef struct winImage {
    int row;
    int col;
} wimage;


typedef struct {
    // Store whether or not the game is over in this member:
    wimage *w;
    int wimageDraw;
    int gameOver;
    int gameWon;
    int lives;
    int exoSize;
    int exoUpdate;
    int tilesHit;
    int onSnowUpdate;
    struct XO *exos;
    psnage* mainChar;
    unsigned short *saveArr;
} AppState;

// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);
void RE_initializeAppState(AppState* appState);

// This function will be used to process app frames.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);
AppState *processAppStateMove (AppState *currentAppState, u32 keysPressedNow);
AppState *processAppStateSlide (AppState *currentAppState);
AppState *processAppStateWin (AppState *state, u32 keysPressedNow);
AppState *processAppStateWinScroll (AppState *state);
AppState *processAppStateWinReset (AppState *state);
void ResetXO(AppState* appState);
int checkTiles(AppState *state);
XO *getCurrTile(AppState *currentAppState);

// If you have anything else you need accessible from outside the logic.c
// file, you can add them here. You likely won't.
void makeXO(int row, int col, int isX, XO *exo);

#endif
