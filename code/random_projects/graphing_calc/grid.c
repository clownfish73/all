#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 800  // Reduced for better performance
#define HEIGHT 600

int f(int x) { return x * x; }

typedef struct Col {
    int r;
    int g;
    int b;
} Col;

Col *newCol(int r, int g, int b) {
    Col *new_col = (Col *)malloc(sizeof(Col));
    if (!new_col) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    new_col->r = r;
    new_col->g = g;
    new_col->b = b;
    return new_col;
}

void draw(Uint32 *pixels, int x, int y, Col *c, SDL_PixelFormat *format) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return; // Bounds check
    pixels[y * WIDTH + x] = SDL_MapRGB(format, c->r, c->g, c->b);
}

void initGrid(Uint32 *pixels){
    
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Initialization Failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Pixel Control",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture *texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT
    );

    if (!texture) {
        printf("Texture creation failed: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Allocate pixel buffer
    Uint32 *pixels = (Uint32 *)malloc(WIDTH * HEIGHT * sizeof(Uint32));
    if (!pixels) {
        printf("Memory allocation failed!\n");
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Get pixel format
    SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

    // Randomize pixels
    srand(time(NULL));

    for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                Col *color = newCol(rand() % 256, rand() % 256, rand() % 256);
                draw(pixels, x, y, color, format);
                free(color);
            }
        }
    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
        }

        // Modify pixels (Example: Random colored pixels)
        

        SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    free(pixels);
    SDL_FreeFormat(format);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
