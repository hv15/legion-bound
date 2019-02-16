#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>

// Screen dimension constants
static const uint32_t SCREEN_WIDTH = 1280;
static const uint32_t SCREEN_HEIGHT = 720;

// FPS control
static const uint32_t SCREEN_TICKS_PER_FRAME = 1000; // 1 FPS

// legion end-credit colors
typedef struct color {
    uint32_t r;
    uint32_t g;
    uint32_t b;
} color_t;

static const color_t bg   = {203, 203, 203}; // gray
static const color_t box1 = {35, 110, 25}; // green
static const color_t box2 = {131, 80, 136}; // purple

static
void add_rand_rect (SDL_Renderer * renderer, const color_t * color)
{
    uint32_t pos_y = rand () % SCREEN_HEIGHT;
    uint32_t pos_x = rand () % SCREEN_WIDTH;
    uint32_t size_y = rand () % SCREEN_HEIGHT/2;
    uint32_t size_x = rand () % SCREEN_WIDTH/2;

    SDL_Rect rect;
    rect.x = pos_x;
    rect.y = pos_y;
    rect.w = size_x;
    rect.h = size_y;

    SDL_SetRenderDrawColor (renderer, color->r, color->g, color->b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect (renderer, &rect);
}

static
uint32_t draw (uint32_t interval, void * param)
{
    // get renderer
    SDL_Renderer * renderer = param;

    // set background black
    SDL_SetRenderDrawColor (renderer, bg.r, bg.g, bg.b, SDL_ALPHA_OPAQUE);
    SDL_RenderClear (renderer);

    // create two rectangles
    add_rand_rect (renderer, &box1);
    add_rand_rect (renderer, &box2);

    SDL_RenderPresent (renderer);

    return interval;
}

int main ()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window = SDL_CreateWindow(
            "Legion Bound",
            // put into centre of screen (windowed/boxed)
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            0
            );
    if (window == NULL) {
        SDL_Log("Unable to create SDL window: %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_Log ("Unable to create SDL renderer: %s", SDL_GetError());
        SDL_DestroyWindow (window);
        SDL_Quit ();
        return EXIT_FAILURE;
    }

    /* Asynchronously update the window */
    SDL_TimerID timer = SDL_AddTimer (SCREEN_TICKS_PER_FRAME,
                                      draw,
                                      renderer);
    if (timer == 0) {
        SDL_Log ("Unable to create SDL timer: %s", SDL_GetError());
        SDL_DestroyRenderer (renderer);
        SDL_DestroyWindow (window);
        SDL_Quit ();
        return EXIT_FAILURE;
    }

    /* Draw initial view */
    draw (0, (void *)renderer);

    /* Event loop; we wait on next re-draw event through timer or user quit */
    SDL_Event event;
    while (SDL_WaitEvent (&event) && event.type != SDL_QUIT);

    SDL_RemoveTimer (timer);
    SDL_DestroyRenderer (renderer);
    SDL_DestroyWindow (window);
    SDL_Quit ();

    return EXIT_SUCCESS;
}
