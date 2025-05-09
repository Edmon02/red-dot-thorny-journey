#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define NUM_CIRCLES 28  // Length of rList (30 to 300 by 10)
#define TRAIL_LEN 33
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700
#define FPS 30
#define CENTER_X 350
#define CENTER_Y 350

typedef struct {
    double r;          // Distance from center
    double delta;      // Angular speed modifier
    double radius;     // Circle radius
    double angle;      // Current angle
} Circle;

typedef struct {
    int *data;
    int count;
    int capacity;
} IntArray;

// Initialize array with capacity
IntArray *createIntArray(int capacity) {
    IntArray *arr = malloc(sizeof(IntArray));
    arr->data = malloc(sizeof(int) * capacity);
    arr->count = 0;
    arr->capacity = capacity;
    return arr;
}

// Free array memory
void freeIntArray(IntArray *arr) {
    free(arr->data);
    free(arr);
}

// Add element to array
void appendIntArray(IntArray *arr, int value) {
    if (arr->count >= arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, sizeof(int) * arr->capacity);
    }
    arr->data[arr->count++] = value;
}

// Check if array contains value
bool containsIntArray(IntArray *arr, int value) {
    for (int i = 0; i < arr->count; i++) {
        if (arr->data[i] == value) return true;
    }
    return false;
}

// Calculate Euclidean distance between two points
double euclideanDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create window
    SDL_Window *window = SDL_CreateWindow("Red dot thorny journey through the realms of time",
                                         SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                         WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Seed random number generator
    srand(5);  // SeedRandom[5] equivalent

    // Initialize circles
    Circle circles[NUM_CIRCLES];
    for (int i = 0; i < NUM_CIRCLES; i++) {
        circles[i].r = 30 + i * 10;  // Range[30, 300, 10]
        circles[i].delta = ((double)rand() / RAND_MAX - 0.5) / 60.0;  // RandomReal[{-0.5, 0.5}]/60
        circles[i].radius = (30 + pow(circles[i].r, 2) / 10000) / 2;  // (30 + rList^2/10000)/2
        circles[i].angle = 0;
    }

    // Animation variables
    int redIdx = 0;  // Start with the first circle
    IntArray *inactiveSet = createIntArray(NUM_CIRCLES);
    int frame = 0;

    bool quit = false;
    SDL_Event e;
    Uint32 frameStart;
    int frameTime;

    // Main loop
    while (!quit) {
        frameStart = SDL_GetTicks();

        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Update circle angles
        for (int i = 0; i < NUM_CIRCLES; i++) {
            circles[i].angle = (frame + 9000) * circles[i].delta;
        }

        // Calculate red circle position
        double posRedX = CENTER_X + circles[redIdx].r * cos(circles[redIdx].angle);
        double posRedY = CENTER_Y + circles[redIdx].r * sin(circles[redIdx].angle);

        // Find overlapping circles
        IntArray *overlaps = createIntArray(NUM_CIRCLES);
        for (int i = 0; i < NUM_CIRCLES; i++) {
            if (i == redIdx) continue;

            double posX = CENTER_X + circles[i].r * cos(circles[i].angle);
            double posY = CENTER_Y + circles[i].r * sin(circles[i].angle);

            if (euclideanDistance(posRedX, posRedY, posX, posY) <
                circles[redIdx].radius + circles[i].radius) {
                appendIntArray(overlaps, i);
            }
        }

        // Handle overlaps and red circle transfers
        bool foundAvailable = false;
        for (int i = 0; i < overlaps->count; i++) {
            if (!containsIntArray(inactiveSet, overlaps->data[i])) {
                // Transfer to first new overlap
                int newIdx = overlaps->data[i];
                appendIntArray(inactiveSet, redIdx);
                redIdx = newIdx;
                foundAvailable = true;
                break;
            }
        }

        // Reset inactive set if no overlaps
        if (overlaps->count == 0) {
            inactiveSet->count = 0;
        }

        freeIntArray(overlaps);

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw trails
        for (int t = 1; t <= TRAIL_LEN; t++) {
            Uint8 alpha = 255 * (TRAIL_LEN - t) / TRAIL_LEN;
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);

            for (int i = 0; i < NUM_CIRCLES; i++) {
                double trailAngle = circles[i].angle - t * circles[i].delta;
                int centerX = CENTER_X + circles[i].r * cos(trailAngle);
                int centerY = CENTER_Y + circles[i].r * sin(trailAngle);

                // Draw circle outline (approximate with points)
                for (double a = 0; a < 2 * M_PI; a += 0.05) {
                    int x = centerX + circles[i].radius * cos(a);
                    int y = centerY + circles[i].radius * sin(a);
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
        }

        // Draw spokes
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int i = 0; i < NUM_CIRCLES; i++) {
            int endX = CENTER_X + circles[i].r * cos(circles[i].angle);
            int endY = CENTER_Y + circles[i].r * sin(circles[i].angle);
            SDL_RenderDrawLine(renderer, CENTER_X, CENTER_Y, endX, endY);
        }

        // Draw black disks
        for (int i = 0; i < NUM_CIRCLES; i++) {
            if (i == redIdx) continue;

            int centerX = CENTER_X + circles[i].r * cos(circles[i].angle);
            int centerY = CENTER_Y + circles[i].r * sin(circles[i].angle);
            int radius = (int)circles[i].radius;

            // Fill the circle with black
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            for (int y = centerY - radius; y <= centerY + radius; y++) {
                for (int x = centerX - radius; x <= centerX + radius; x++) {
                    if (pow(x - centerX, 2) + pow(y - centerY, 2) <= pow(radius, 2)) {
                        SDL_RenderDrawPoint(renderer, x, y);
                    }
                }
            }

            // Draw white outline
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            for (double a = 0; a < 2 * M_PI; a += 0.01) {
                int x = centerX + radius * cos(a);
                int y = centerY + radius * sin(a);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }

        // Draw red disk
        {
            int centerX = CENTER_X + circles[redIdx].r * cos(circles[redIdx].angle);
            int centerY = CENTER_Y + circles[redIdx].r * sin(circles[redIdx].angle);
            int radius = (int)circles[redIdx].radius;

            // Fill the circle with red
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            for (int y = centerY - radius; y <= centerY + radius; y++) {
                for (int x = centerX - radius; x <= centerX + radius; x++) {
                    if (pow(x - centerX, 2) + pow(y - centerY, 2) <= pow(radius, 2)) {
                        SDL_RenderDrawPoint(renderer, x, y);
                    }
                }
            }

            // Draw white outline
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            for (double a = 0; a < 2 * M_PI; a += 0.01) {
                int x = centerX + radius * cos(a);
                int y = centerY + radius * sin(a);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }

        // Update screen
        SDL_RenderPresent(renderer);

        // Frame timing
        frame++;
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 1000 / FPS) {
            SDL_Delay(1000 / FPS - frameTime);
        }
    }

    // Clean up
    freeIntArray(inactiveSet);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
