#include "logic.h"
#include <stdio.h>
#include "gba.h"


void initializeAppState(AppState* appState) {
    // TA-TODO: Initialize everything that's part of this AppState struct here.
    // Suppose the struct contains random values, make sure everything gets
    // the value it should have when the app begins.

    //AppState vals = {0, 3, &exs, NULL};
    appState -> gameOver = 0;
    appState -> lives = 3;
    appState -> w -> row = 0;
    appState -> w -> col = 0;
    appState -> exoSize = 48;
    appState -> exoUpdate = 0;
    appState -> tilesHit = 0;
    appState -> gameWon = 0;
    appState -> wimageDraw = 0;

    appState -> onSnowUpdate = 0;
    appState -> mainChar -> row = 0;
    appState -> mainChar -> col = 0;
    appState -> mainChar -> isSide = 0;
    appState -> mainChar -> isFront = 1;
    appState -> mainChar -> onTile = 0;
    appState -> mainChar -> sliding = 0;
    appState -> mainChar -> prevTile = NULL;

    //init all exos to NULL
    struct XO *curr = appState -> exos;
    for(int i = 0; i < 48; i++) {
        curr -> draw = 0;
        curr = curr + 1;
    }

    ResetXO(appState);

}

void RE_initializeAppState(AppState* appState) {
    appState -> mainChar -> prevTile = NULL;
    appState -> mainChar -> onTile = 0;
    appState -> mainChar -> sliding = 0;
    appState -> onSnowUpdate = 0;
    appState -> exoUpdate = 0;
    appState -> gameOver = 0;
    appState -> gameWon = 0;
    appState -> tilesHit = 0;
    ResetXO(appState);
}

void ResetXO(AppState* appState) {
    makeXO(0, 0, 1, appState -> exos + OFFSET(0, 0, 6));
    makeXO(0, 4, 1, appState -> exos + OFFSET(0, 4, 6));
    makeXO(1, 0, 1, appState -> exos + OFFSET(1, 0, 6));
    makeXO(1, 1, 1, appState -> exos + OFFSET(1, 1, 6));
    makeXO(1, 3, 1, appState -> exos + OFFSET(1, 3, 6));
    makeXO(3, 2, 1, appState -> exos + OFFSET(3, 2, 6));
    makeXO(4, 1, 1, appState -> exos + OFFSET(4, 1, 6));
    makeXO(4, 2, 1, appState -> exos + OFFSET(4, 2, 6));
    makeXO(5, 1, 1, appState -> exos + OFFSET(5, 1, 6));
    makeXO(5, 3, 1, appState -> exos + OFFSET(5, 3, 6));
    makeXO(7, 0, 1, appState -> exos + OFFSET(7, 0, 6));
    makeXO(7, 4, 1, appState -> exos + OFFSET(7, 4, 6));
    makeXO(3, 5, 1, appState -> exos + OFFSET(3, 4, 6));
    (appState -> exos + OFFSET(3, 4, 6)) -> isButton = 1;
    (appState -> exos + OFFSET(3, 4, 6)) -> row = 52;
    (appState -> exos + OFFSET(3, 4, 6)) -> col = 217;
}

void makeXO(int row, int col, int isX, XO *exo) {
    exo -> row = row*18;
    exo -> col = 30 + col*40;
    exo -> isX = isX;
    exo -> draw = 1;
    exo -> isButton = 0;
}

// TA-TODO: Add any process functions for sub-elements of your app here.
// For example, for a snake game, you could have a processSnake function
// or a createRandomFood function or a processFoods function.
//
// e.g.:
// static Snake processSnake(Snake* currentSnake);
// static void generateRandomFoods(AppState* currentAppState, AppState* nextAppState);


// This function processes your current app state and returns the new (i.e. next)
// state of your application.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    /* TA-TODO: Do all of your app processing here. This function gets called
     * every frame.
     *
     * To check for key presses, use the KEY_JUST_PRESSED macro for cases where
     * you want to detect each key press once, or the KEY_DOWN macro for checking
     * if a button is still down.
     *
     * To count time, suppose that the GameBoy runs at a fixed FPS (60fps) and
     * that VBlank is processed once per frame. Use the vBlankCounter variable
     * and the modulus % operator to do things once every (n) frames. Note that
     * you want to process button every frame regardless (otherwise you will
     * miss inputs.)
     *
     * Do not do any drawing here.
     *
     * TA-TODO: VERY IMPORTANT! READ THIS PART.
     * You need to perform all calculations on the currentAppState passed to you,
     * and perform all state updates on the nextAppState state which we define below
     * and return at the end of the function. YOU SHOULD NOT MODIFY THE CURRENTSTATE.
     * Modifying the currentAppState will mean the undraw function will not be able
     * to undraw it later.
     */
    currentAppState -> mainChar -> prevRow = currentAppState -> mainChar -> row;
    currentAppState -> mainChar -> prevCol = currentAppState -> mainChar -> col;

    if(!(currentAppState -> mainChar -> sliding) || currentAppState -> mainChar -> onTile) {
        currentAppState = processAppStateMove(currentAppState, keysPressedNow);
    } else {
        currentAppState = processAppStateSlide(currentAppState);
    }

    AppState nextAppState = *currentAppState;

    UNUSED(keysPressedBefore);
    //UNUSED(keysPressedNow);

    return nextAppState;
}



AppState *processAppStateMove (AppState *currentAppState, u32 keysPressedNow) {
    int keyPressedCheck = 0;
    if (KEY_DOWN(BUTTON_RIGHT, keysPressedNow)) {
        currentAppState -> mainChar -> col++;
        currentAppState -> mainChar -> isFront = 1;
        currentAppState -> mainChar -> isSide = 1;
        keyPressedCheck++;
    }
    if (KEY_DOWN(BUTTON_LEFT, keysPressedNow)) {
        if(currentAppState -> mainChar -> col - 1 >= 0 ||
            currentAppState -> mainChar -> col - 1 > 25) {
            currentAppState -> mainChar -> col--;
            currentAppState -> mainChar -> isFront = 0;
            currentAppState -> mainChar -> isSide = 1;
            keyPressedCheck++;
        }
    }
    if (KEY_DOWN(BUTTON_UP, keysPressedNow)) {
        if(currentAppState -> mainChar -> row - 1 >= 0 ||
            currentAppState -> mainChar -> col - 1 > 25) {
            currentAppState -> mainChar -> row--;
            currentAppState -> mainChar -> isFront = 0;
            currentAppState -> mainChar -> isSide = 0;
            keyPressedCheck++;
        }
    }
    if (KEY_DOWN(BUTTON_DOWN, keysPressedNow)) {
        if(currentAppState -> mainChar -> row - 1 < 160-17 ||
            currentAppState -> mainChar -> col - 1 > 25) {
            currentAppState -> mainChar -> row++;
            currentAppState -> mainChar -> isFront = 1;
            currentAppState -> mainChar -> isSide = 0;
            keyPressedCheck++;
        }
    }



    if(currentAppState -> mainChar -> col > 20) {
        currentAppState -> mainChar -> sliding = 1;
    }

    if(currentAppState -> mainChar -> onTile) {
        if (currentAppState -> mainChar -> prevTile -> isButton
        && checkTiles(currentAppState)) {
            currentAppState -> gameWon = 1;
            currentAppState -> gameOver = 1;

        }  else if (keyPressedCheck != 0) {
            currentAppState -> mainChar -> onTile = 0;
        }
    }

    if (!(currentAppState -> mainChar -> sliding)) {
        currentAppState -> mainChar -> prevTile = NULL;
    }

    return currentAppState;
}

int checkTiles(AppState *state) {
    struct XO *curr = state -> exos;
    for(int i = 0; i < 48; i++) {
        if (curr -> draw && curr -> isX && !(curr -> isButton)) {
            return 0;
        }
        curr = curr + 1;
    }
    return 1;
}

AppState *processAppStateSlide (AppState *currentAppState) {
    if (currentAppState -> mainChar -> isSide) { //figure out which way he's facing
        if(currentAppState -> mainChar -> isFront) {
            currentAppState -> mainChar -> col++;
        } else {
            currentAppState -> mainChar -> col--;
        }

    } else { //figure out which way he's facing
        if(currentAppState -> mainChar -> isFront) {
            currentAppState -> mainChar -> row++;
        } else {
            currentAppState -> mainChar -> row--;
        }
    }

    //big boye if statement for if on tile
    if(currentAppState -> mainChar -> row < 140) {
        if((getCurrTile(currentAppState) -> draw && !(getCurrTile(currentAppState) -> isButton))
            &&
            (((currentAppState -> mainChar -> row ==
             getCurrTile(currentAppState) -> row
            &&
            currentAppState -> mainChar -> col <=
            getCurrTile(currentAppState) -> col + 10))
            ||
            (currentAppState -> mainChar -> col ==
            getCurrTile(currentAppState) -> col
            &&
            currentAppState -> mainChar -> row <=
            getCurrTile(currentAppState)-> row + 10))
            ) {

            currentAppState -> mainChar -> onTile = 1;

            if(getCurrTile(currentAppState)
                    != currentAppState -> mainChar -> prevTile) {
               currentAppState -> exoUpdate = 1;
            }

            currentAppState -> mainChar -> prevTile = getCurrTile(currentAppState);
        } else if ((getCurrTile(currentAppState) -> draw && getCurrTile(currentAppState) -> isButton)
            &&
            (((currentAppState -> mainChar -> row ==
             getCurrTile(currentAppState) -> row
            &&
            currentAppState -> mainChar -> col >=
            getCurrTile(currentAppState) -> col + 5))
            ||
            (currentAppState -> mainChar -> col ==
            getCurrTile(currentAppState) -> col
            &&
            currentAppState -> mainChar -> row <=
            getCurrTile(currentAppState)-> row + 10))
            ) {

            currentAppState -> mainChar -> onTile = 1;

            if(getCurrTile(currentAppState)
                    != currentAppState -> mainChar -> prevTile) {
               currentAppState -> exoUpdate = 1;
            }

            currentAppState -> mainChar -> prevTile = getCurrTile(currentAppState);

        }
    }

    //if on snow
    if(currentAppState -> mainChar -> col < 20) {
        currentAppState -> mainChar -> sliding = 0;
        currentAppState -> onSnowUpdate = 1;
    }

    //if col > 240 or row < 0 or row > 160 --> game over
    if (currentAppState -> mainChar -> col > 240 || currentAppState -> mainChar -> row + 16 < 0
        || currentAppState -> mainChar -> row >= 160) {
        currentAppState -> gameOver = 1;
    }

    return currentAppState;
}

XO *getCurrTile(AppState *currentAppState) {
    return (currentAppState -> exos + OFFSET((currentAppState -> mainChar -> row + 8)/18,
            (currentAppState -> mainChar -> col - 30 + 8)/40, 6));
}

AppState *processAppStateWinScroll (AppState *state) {
    state -> w -> col +=1;
    return state;

}

AppState *processAppStateWinReset (AppState *state) {
    state -> mainChar -> row = 52;
    state -> mainChar -> prevRow = 52;
    state -> mainChar -> col = 0;
    state -> mainChar -> prevCol = 0;
    state -> mainChar -> onTile = 0;
    state -> mainChar -> sliding = 0;
    saveRectangleDMA(state -> mainChar -> row, state -> mainChar -> col,
        16, 16, state -> saveArr);
    return state;
}


AppState *processAppStateWin (AppState *state, u32 keysPressedNow) {
    int keyPressedCheck = 0;

    state -> mainChar -> prevCol = state -> mainChar -> col;
    state -> mainChar -> prevRow = state -> mainChar -> row;

    if (KEY_DOWN(BUTTON_RIGHT, keysPressedNow)) {
        state -> mainChar -> col++;
        state -> mainChar -> isFront = 1;
        state -> mainChar -> isSide = 1;
        keyPressedCheck++;
    }
    if (KEY_DOWN(BUTTON_LEFT, keysPressedNow)) {
        if(state -> mainChar -> col - 1 >= 0 ||
            state -> mainChar -> col - 1 > 25) {
            state -> mainChar -> col--;
            state -> mainChar -> isFront = 0;
            state -> mainChar -> isSide = 1;
            keyPressedCheck++;
        }
    }
    if (KEY_DOWN(BUTTON_UP, keysPressedNow)) {
        if(state -> mainChar -> row - 1 >= 0 ||
            state -> mainChar -> col - 1 > 25) {
            state -> mainChar -> row--;
            state -> mainChar -> isFront = 0;
            state -> mainChar -> isSide = 0;
            keyPressedCheck++;
        }
    }
    if (KEY_DOWN(BUTTON_DOWN, keysPressedNow)) {
        if(state -> mainChar -> row - 1 < 160-17 ||
            state -> mainChar -> col - 1 > 25) {
            state -> mainChar -> row++;
            state -> mainChar -> isFront = 1;
            state -> mainChar -> isSide = 0;
            keyPressedCheck++;
        }
    }


    // state -> mainChar -> prevCol = state -> mainChar -> col;
    // state -> mainChar -> col += 1;
    if(state -> mainChar -> col > 240) {
        state -> wimageDraw = 1;
    }
    return state;
}

