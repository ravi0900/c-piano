#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <graphics.h>

#define NUM_KEYS 26 // Number of alphabet keys
#define NUM_WHITE_KEYS 7
#define NUM_BLACK_KEYS 5

#define WHITE_KEY_WIDTH 80
#define WHITE_KEY_HEIGHT 200
#define BLACK_KEY_WIDTH (WHITE_KEY_WIDTH / 2)
#define BLACK_KEY_HEIGHT (WHITE_KEY_HEIGHT / 2)

void playSound(int key) {
    if (key >= 0 && key < NUM_KEYS) {
        printf("Playing sound for key %c\n", 'a' + key);
        // Add your code here to play sound for the corresponding key
    }
}

void renderKeyboard() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TC\\BGI");

    int whiteKeyWidth = WHITE_KEY_WIDTH;
    int blackKeyWidth = BLACK_KEY_WIDTH;

    // Render white keys
    for (int i = 0; i < NUM_WHITE_KEYS; i++) {
        rectangle(i * whiteKeyWidth, 0, (i + 1) * whiteKeyWidth, WHITE_KEY_HEIGHT);
        floodfill(i * whiteKeyWidth + 10, WHITE_KEY_HEIGHT / 2, WHITE);
    }

    // Render black keys
    for (int i = 0; i < NUM_BLACK_KEYS; i++) {
        if (i != 2) { // Skip drawing black key in the middle
            rectangle((i + 1) * whiteKeyWidth - blackKeyWidth / 2, 0, (i + 1) * whiteKeyWidth + blackKeyWidth / 2, BLACK_KEY_HEIGHT);
            floodfill((i + 1) * whiteKeyWidth - blackKeyWidth / 2 + 10, BLACK_KEY_HEIGHT / 2, BLACK);
        }
    }

    getch();
    closegraph();
}

int main() {
    renderKeyboard();

    char ch;
    int quit = 0;
    while (!quit) {
        if (kbhit()) {
            ch = getch();
            if (ch == 27) {
                quit = 1;
            } else if (ch >= 'a' && ch <= 'z') {
                int key = ch - 'a';
                playSound(key);
            }
        }
    }

    return 0;
}
