#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 2560
#define HEIGHT 1440

int f(int x){return x*x;}

typedef struct Col{
    int r;
    int g;
    int b;
} Col;

Col *newCol(int r, int g, int b){
    Col* new_col = (Col*)malloc(sizeof(Col));
    new_col->r = r;
    new_col->g = g;
    new_col->b = b;
    return new_col;
}

Col* randCol(){
    int r, g, b;
    r = rand()%256;
    g = rand()%256;
    b = rand()%256;
    return newCol(r,g,b);
}

void draw(Uint32 *pixels, int x, int y, Col* c){
    pixels[y * WIDTH + x] = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), c->r, c->g, c->b);
}

int main() {
     srand(clock());
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Pixel Control", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        WIDTH, HEIGHT, 
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        WIDTH, HEIGHT
    );

    // Pixel buffer (RGBA format, 32 bits per pixel)
    Uint32* pixels = malloc(WIDTH * HEIGHT * sizeof(Uint32));

    // Main loop
    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
        }

        // Modify pixels here (pixel-level control)
        // Example: Set pixel at (100, 100) to red
        for(int i = 0; i < WIDTH; i++){
            for(int j = 0; j < HEIGHT; j++){
                Col* c = randCol();
                draw(pixels, i, j, c);
                free(c);
            }
        }
        


        // Update texture and render
        SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    free(pixels);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

//gcc -o pixel_demo pixel_demo.c -lSDL2