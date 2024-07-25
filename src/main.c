#include "SDL2/SDL.h"
#include "SDL2/SDL_timer.h"

#define WINDOW_WIDTH  LOW_RES_WIDTH
#define WINDOW_HEIGHT LOW_RES_HEIGHT
#define PIXEL_SCALE   (20)

struct sdl
{
    SDL_Window       *window;
    SDL_Renderer     *renderer;
    SDL_Texture      *texture;
    SDL_AudioDeviceID audio_device;
};

static inline void sdl_cleanup(struct sdl *sdl)
{
    if (sdl->audio_device) SDL_CloseAudioDevice(sdl->audio_device);
    if (sdl->texture) SDL_DestroyTexture(sdl->texture);
    if (sdl->renderer) SDL_DestroyRenderer(sdl->renderer);
    if (sdl->window) SDL_DestroyWindow(sdl->window);

    SDL_Quit();
}

static bool sdl_init(struct sdl *sdl)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
    {
        LOG_ERROR("Failed to initialise SDL subsystems : %s", SDL_GetError());
        return false;
    }

    if (sdl == NULL)
    {
        LOG_ERROR("sdl is NULL\n");
        return false;
    }

    sdl->window = SDL_CreateWindow(
        "Chip8 Graphics",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH * PIXEL_SCALE,
        WINDOW_HEIGHT * PIXEL_SCALE,
        0);
    if (!sdl->window)
    {
        LOG_ERROR("Failed to create window : %s", SDL_GetError());
        sdl_cleanup(sdl);
        return false;
    }

    // TODO : remove this and go software only
    sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED);
    if (!sdl->renderer)
    {
        LOG_ERROR("Failed to create window : %s", SDL_GetError());
        sdl_cleanup(sdl);
        return false;
    }

    // Create texture
    sdl->texture = SDL_CreateTexture(sdl->renderer,
                                     SDL_PIXELFORMAT_RGBA8888,
                                     SDL_TEXTUREACCESS_STREAMING,
                                     HIGH_RES_WIDTH,
                                     HIGH_RES_HEIGHT);
    if (!sdl->texture)
    {
        LOG_ERROR("Texture could not be created : %s", SDL_GetError());
        sdl_cleanup(sdl);
        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
    {
        LOG_ERROR("Failed to initialise SDL subsystems : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    struct sdl sdl = {0};
    if (sdl_init(&sdl) == false)
        return EXIT_FAILURE;

    sdl_cleanup(&sdl);

    LOG_INFO("Exit success");

    return 0;
