#include "gba.h"

volatile unsigned short *videoBuffer = (volatile unsigned short *) 0x6000000;
u32 vBlankCounter = 0;

void waitForVBlank(void) {
    // TA-TODO: IMPLEMENT

    // Write a while loop that loops until we're NOT in vBlank anymore:
    // (This prevents counting one VBlank more than once if your app is too fast)
    while(*SCANLINECOUNTER > 160);
    while(*SCANLINECOUNTER < 160);
    // Write a while loop that keeps going until we're in vBlank:

    // Finally, increment the vBlank counter:
    vBlankCounter++;
}

static int __qran_seed= 42;
static int qran(void) {
    __qran_seed= 1664525*__qran_seed+1013904223;
    return (__qran_seed>>16) & 0x7FFF;
}

int randint(int min, int max) {
    return (qran()*(max-min)>>15)+min;
}

void setPixel(int x, int y, u16 color) {
    // TA-TODO: IMPLEMENT
    videoBuffer[x*WIDTH + y] = color;
}

void drawRectDMA(int x, int y, int width, int height, volatile u16 color) {
    // TA-TODO: IMPLEMENT
    for(int i = 0; i < height; i++) {
        DMA[3].src = &color;
        DMA[3].dst = videoBuffer + OFFSET(x+i, y, 240);
        DMA[3].cnt = width | DMA_16 | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT
            |DMA_ON;
    }
}



void drawFullScreenImageDMA(const u16 *image) {
    // TA-TODO: IMPLEMENT
    DMA[3].src = image;
    DMA[3].dst = videoBuffer;
    DMA[3].cnt = WIDTH * HEIGHT | DMA_16 | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT
        |DMA_ON;
}

void drawImageDMA(int x, int y, int width, int height, const u16 *image) {
    // TA-TODO: IMPLEMENT
    for(int i = 0; i < height; i++) {
        DMA[3].src = image + OFFSET(i, 0, width);
        DMA[3].dst = videoBuffer + OFFSET(x+i, y, 240);
        DMA[3].cnt = width | DMA_16 | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT
            |DMA_ON;
    }
}

void drawMainCharDMA(int isFront, int isSide, int x, int y, int width, int height,
    const u16 *image) {
    // TA-TODO: IMPLEMENT

    if (!(isSide)) {
        int margin = 4;
        int tempwidth = width-(margin * 2);
        for(int i = 0; i < height; i++) {
            DMA[3].src = image + OFFSET(i, margin, width);
            DMA[3].dst = videoBuffer + OFFSET(x+i, y+margin, 240);
            DMA[3].cnt = tempwidth | DMA_16 | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT
                |DMA_ON;
            if(i > 2){
               margin = 1;
            } else {
               margin = 3 - i%5;
            }
            tempwidth = width-(margin * 2);

            //tempwidth = width-margin;

        }
    } else if (isFront) {
        int margin = 5;
        int tempwidth = width-(margin * 2);
        for(int i = 0; i < height; i++) {
            DMA[3].src = image + OFFSET(i, margin, width);
            DMA[3].dst = videoBuffer + OFFSET(x+i, y+margin, 240);
            DMA[3].cnt = tempwidth | DMA_16 | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT
                |DMA_ON;
            if(i > 2){
               margin = 2;
               tempwidth = width - 5;
            } else {
               margin = 3 - i/3;
               tempwidth = width-(margin * 2);
            }


            //tempwidth = width-margin;

        }

    } else {
        int margin = 5;
        int tempwidth = width-(margin * 2);
        for(int i = 0; i < height; i++) {
            DMA[3].src = image + OFFSET(i, margin, width);
            DMA[3].dst = videoBuffer + OFFSET(x+i, y+margin, 240);
            DMA[3].cnt = tempwidth | DMA_16 | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT
                |DMA_ON;
            if(i > 2){
               margin = 3;
               tempwidth = width - 5;
            } else {
               margin = 3 - i/3;
               tempwidth = width-(margin * 2);
            }

            //tempwidth = width-margin;

        }
    }
}

void saveRectangleDMA(int row, int col, int width, int height, unsigned short *savearr)
{
    for(int r = 0; r < height; r++) {
        for(int c = 0; c < width; c++) {
            savearr[OFFSET(r, c, width)] = videoBuffer[OFFSET(row + r, col + c, 240)];
        }
    }
}


void drawSavedRectangleDMA(int row, int col, int width, int height, unsigned short* savearr)
{

    for(int i = 0; i < height; i++) {
        DMA[3].src = savearr + OFFSET(i, 0, width);
        DMA[3].dst = videoBuffer + OFFSET(row+i, col, 240);
        DMA[3].cnt = width | DMA_16 | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT
            |DMA_ON;
    }

}



void fillScreenDMA(volatile u16 color) {
    // TA-TODO: IMPLEMENT
    DMA[3].src = &color;
    DMA[3].dst = videoBuffer;
    DMA[3].cnt = WIDTH * 160 | DMA_16 | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT
        |DMA_ON;
}

void drawPieceDMA(int x, int y, int width, int height, const u16 *image) {
    for(int i = 0; i < height; i++) {
        DMA[3].src = image + OFFSET(i, 0, 240);
        DMA[3].dst = videoBuffer + OFFSET(x+i, y, 240);
        DMA[3].cnt = width | DMA_16 | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT
            |DMA_ON;
    }
}

void drawScrollDMA(int x, int y, int width, int height, int widthPic, const u16 *image) {
    for(int i = 0; i < height; i++) {
        DMA[3].src = image + OFFSET(i + x, y, widthPic);
        DMA[3].dst = videoBuffer + OFFSET(i, 0, 240);
        DMA[3].cnt = width | DMA_16 | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT
            |DMA_ON;
    }
}

void drawChar(int col, int row, char ch, u16 color) {
    for(int r = 0; r<8; r++) {
        for(int c=0; c<6; c++) {
            if(fontdata_6x8[OFFSET(r, c, 6) + ch*48]) {
                setPixel(row+r, col+c, color);
            }
        }
    }
}

void drawSprite(int col, int row, char ch, u16 color) {
    for(int r = 0; r<24; r++) {
        for(int c=0; c<18; c++) {
            if(fontdata_6x8[OFFSET(r/3, c/3, 6) + ch*48]) {
                setPixel(col+c, row+r, color);
            }
        }
    }
}

void drawSpriteRight(int col, int row, char ch, u16 color) {
    for(int r = 0; r<24; r++) {
        for(int c=0; c<18; c++) {
            if(fontdata_6x8[OFFSET(r/3, c/3, 6) + ch*48]) {
                setPixel(row+r, col+c, color);
            }
        }
    }
}

void drawString(int col, int row, char *str, u16 color) {
    while(*str) {
        drawChar(col, row, *str++, color);
        col += 6;
    }
}

void drawCenteredString(int x, int y, int width, int height, char *str, u16 color) {
    u32 len = 0;
    char *strCpy = str;
    while (*strCpy) {
        len++;
        strCpy++;
    }

    u32 strWidth = 6 * len;
    u32 strHeight = 8;

    int col = x + ((width - strWidth) >> 1);
    int row = y + ((height - strHeight) >> 1);
    drawString(col, row, str, color);
}
