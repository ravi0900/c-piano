#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 400

#define NUM_KEYS 26 // Number of alphabet keys
#define NUM_WHITE_KEYS 7
#define NUM_BLACK_KEYS 5

#define WHITE_KEY_WIDTH (SCREEN_WIDTH / NUM_WHITE_KEYS)
#define WHITE_KEY_HEIGHT (SCREEN_HEIGHT / 2)
#define BLACK_KEY_WIDTH (WHITE_KEY_WIDTH / 2)
#define BLACK_KEY_HEIGHT (WHITE_KEY_HEIGHT / 2)

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

Mix_Chunk* pianoSound[NUM_KEYS];

int initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 0;
    }
    
    window = SDL_CreateWindow("Virtual Piano", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window creation failed: %s\n", SDL_GetError());
        return 0;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        return 0;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return 0;
    }

    return 1;
}

void loadSounds() {
    for (int i = 1; i < NUM_KEYS; i++) {
        char filename[23];
        sprintf(filename, "key0%d.ogg", i);
        pianoSound[i] = Mix_LoadWAV(filename);
        if (pianoSound[i] == NULL) {
            printf("Failed to load sound %d: %s\n", i, Mix_GetError());
        }
    }
}

void renderKeyboard() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Rect whiteKeyRect = {0, 0, WHITE_KEY_WIDTH, WHITE_KEY_HEIGHT};
    SDL_Rect blackKeyRect = {0, 0, BLACK_KEY_WIDTH, BLACK_KEY_HEIGHT};

    // Render white keys
    for (int i = 0; i < NUM_WHITE_KEYS; i++) {
        whiteKeyRect.x = i * WHITE_KEY_WIDTH;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &whiteKeyRect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &whiteKeyRect);
    }

    // Render black keys
    for (int i = 0; i < NUM_BLACK_KEYS; i++) {
        if (i != 2) { // Skip drawing black key in the middle
            blackKeyRect.x = (i + 1) * WHITE_KEY_WIDTH - (BLACK_KEY_WIDTH / 2);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, &blackKeyRect);
        }
    }

    SDL_RenderPresent(renderer);
}

void playSound(int key) {
    if (key >= 0 && key < NUM_KEYS && pianoSound[key] != NULL) {
        Mix_PlayChannel(-1, pianoSound[key], 0);
    }
}

void closeSDL() {
    for (int i = 0; i < NUM_KEYS; i++) {
        if (pianoSound[i] != NULL) {
            Mix_FreeChunk(pianoSound[i]);
        }
    }
    Mix_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() {
    if (!initSDL()) {
        return 1;
    }

    loadSounds();

    renderKeyboard();

    SDL_Event event;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym >= SDLK_a && event.key.keysym.sym <= SDLK_z) {
                        int key = event.key.keysym.sym - SDLK_a;
                        playSound(key);
                    }
                    break;
                default:
                    break;
            }
        }
    }

    closeSDL();

    return 0;
}
