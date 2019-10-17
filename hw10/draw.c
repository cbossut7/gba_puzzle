#include "draw.h"
#include "gba.h"

// TA-TODO: Include any header files for images generated by nin10kit.
// Example for the provided garbage image:
 #include "images/StartScreen.h"
 #include "images/character.h"
 #include "images/characterback.h"
 #include "images/characterright.h"
 #include "images/characterleft.h"
 #include "images/Xbackground.h"
 #include "images/Obackground.h"
 #include "images/Gameover.h"
 #include "images/Gamewon.h"
 #include "images/Win_Animation.h"
 #include "images/Instructions.h"
 #include "images/Temp_gameover.h"
 #include "images/WinScreen.h"
 #include <stdio.h>

// TA-TODO: Add any draw/undraw functions for sub-elements of your app here.
// For example, for a snake game, you could have a drawSnake function
// or a drawFood function
//
// e.g.:
// static void drawSnake(Snake* snake);
// static void drawFood(Food* food);

// This function will be used to draw everything about the app
// including the background and whatnot.
void fullDrawAppState(AppState *state) {
    // TA-TODO: IMPLEMENT.
    //UNUSED(state);
    //waitForVBlank();

    //struct XO *exo = state -> exos;

    // for(int i = 0; i < state -> exoSize; i++){
    //     if (exo -> draw && exo -> isX) {
    //         drawSprite(exo -> row, exo -> col, 'X', COLOR(0, 0, 31));
    //         //drawSprite(exo -> row, exo -> col, 'O', COLOR(2, 31, 0));
    //     }
    //     exo = exo + 1;
    // }

    // exo = state -> exos ;
    // for(int i = 0; i < state -> exoSize ; i++){
    //     if (exo -> draw && !(exo -> isX)) { //somehow exo -> isX is returning tru for 1st NULL
    //         drawSprite(exo -> row, exo -> col, 'O', COLOR(2, 31, 0));
    //     }
    //     exo = exo + 1;
    // }



    drawFullScreenImageDMA(Xbackground);
    drawString(151, 151, "Lives:", COLOR(0, 10, 31));
    //drawSprite(52, 217, 'O', COLOR(2, 31, 0));
    drawString(150, 150, "Lives:", COLOR(31, 31, 31));
    char hearts = 3;
    for(int i = 0; i < state -> lives; i++){
        drawChar(190 + i*12, 151, hearts, COLOR(0,10,31));
    }
    for(int i = 0; i < state -> lives; i++){
        drawChar(190 + i*12, 150, hearts, COLOR(20, 0, 0));
    }

    saveRectangleDMA(state -> mainChar -> row, state -> mainChar -> col,
        16, 16, state -> saveArr);

    drawMainCharDMA(state -> mainChar -> isFront, state -> mainChar -> isSide,
        state -> mainChar -> row, state -> mainChar -> col, 16, 16, character);

}

// This function will be used to undraw (i.e. erase) things that might
// move in a frame. E.g. in a Snake game, erase the Snake, the food & the score.
void undrawAppState(AppState *state) {
    //redraw background image OR:
    drawSavedRectangleDMA(state -> mainChar -> prevRow, state -> mainChar -> prevCol,
        16, 16, state -> saveArr);
}

// This function will be used to draw things that might have moved in a frame.
// For example, in a Snake game, draw the snake, the food, the score.
void drawAppState(AppState *state) {
    // TA-TODO: IMPLEMENT.
    if(state -> exoUpdate && state -> mainChar -> sliding) {
        (state -> mainChar -> prevTile) -> isX
        = !((state -> mainChar -> prevTile) -> isX);

        if (state -> mainChar -> prevTile -> isX) { //if need to draw X
            drawPieceDMA(state -> mainChar -> prevTile -> row+1, state -> mainChar -> prevTile -> col - 1,
            22, 20, Xbackground + OFFSET(state -> mainChar -> prevTile -> row+1,
                state -> mainChar -> prevTile -> col - 1, 240));
        } else { //if need to draw O
            drawPieceDMA(state -> mainChar -> prevTile -> row+1, state -> mainChar -> prevTile -> col - 1,
            22, 20, Obackground + OFFSET(state -> mainChar -> prevTile -> row+1,
                state -> mainChar -> prevTile -> col - 1, 240));
        }

        if (state -> mainChar -> prevTile -> isButton && !(state -> gameWon)) { //fix button if not won
            drawPieceDMA(state -> mainChar -> prevTile -> row+1, state -> mainChar -> prevTile -> col - 1,
            22, 20, Xbackground + OFFSET(state -> mainChar -> prevTile -> row+1,
                state -> mainChar -> prevTile -> col - 1, 240));
        }

        state -> exoUpdate = 0;
    }//end if ontile

    //save what I'm about to cover
    saveRectangleDMA(state -> mainChar -> row, state -> mainChar -> col,
        16, 16, state -> saveArr);

    //redraw character
    if(!(state -> mainChar -> isSide)) {
        if(state -> mainChar -> isFront) {
            drawMainCharDMA(state -> mainChar -> isFront, state -> mainChar -> isSide,
                state -> mainChar -> row, state -> mainChar -> col, 16, 16, character);
        } else {
            drawMainCharDMA(state -> mainChar -> isFront, state -> mainChar -> isSide,
                state -> mainChar -> row, state -> mainChar -> col, 16, 16, characterback);
        }
    } else {
        if(state -> mainChar -> isFront) { //right
            drawMainCharDMA(state -> mainChar -> isFront, state -> mainChar -> isSide,
            state -> mainChar -> row, state -> mainChar -> col, 16, 16, characterleft);
        } else { //left
            drawMainCharDMA(state -> mainChar -> isFront, state -> mainChar -> isSide,
                state -> mainChar -> row, state -> mainChar -> col, 16, 16, characterright);
        }
    }

    //cover character if wraparound
    if((state -> mainChar -> col) >= 216) {
        drawPieceDMA(state -> mainChar -> row , 0,
            24, 24, Xbackground + OFFSET(state -> mainChar -> row, 0, 240));
    }
}


void drawWinState(AppState *state) {
    saveRectangleDMA(state -> mainChar -> row, state -> mainChar -> col,
        16, 16, state -> saveArr);

    //redraw character
    if(!(state -> mainChar -> isSide)) {
        if(state -> mainChar -> isFront) {
            drawMainCharDMA(state -> mainChar -> isFront, state -> mainChar -> isSide,
                state -> mainChar -> row, state -> mainChar -> col, 16, 16, character);
        } else {
            drawMainCharDMA(state -> mainChar -> isFront, state -> mainChar -> isSide,
                state -> mainChar -> row, state -> mainChar -> col, 16, 16, characterback);
        }
    } else {
        if(state -> mainChar -> isFront) { //right
            drawMainCharDMA(state -> mainChar -> isFront, state -> mainChar -> isSide,
            state -> mainChar -> row, state -> mainChar -> col, 16, 16, characterleft);
        } else { //left
            drawMainCharDMA(state -> mainChar -> isFront, state -> mainChar -> isSide,
                state -> mainChar -> row, state -> mainChar -> col, 16, 16, characterright);
        }
    }

    //cover character if wraparound
    if((state -> mainChar -> col) >= 216) {
        drawPieceDMA(state -> mainChar -> row , 0,
            24, 24, Xbackground + OFFSET(state -> mainChar -> row, 0, 240));
    }


}
