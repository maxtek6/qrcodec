#ifdef _WIN32
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif
#include <qrcodec.h>

#include <stdio.h>

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;

static void initialize();
static void main_loop();
static void shutdown();

int main()
{
    initialize();
    main_loop();
    shutdown();
    return 0;
}

static void initialize()
{
    const char *payload = "https://github.com/maxtek6/qrcodec";
    const qrcodec_data qrdata = {
        .ecc = QRCODEC_ECC_MEDIUM,
        .mask = QRCODEC_MASK_AUTO,
        .min_version = 1,
        .max_version = 40,
        .type = QRCODEC_TYPE_ALPHANUMERIC,
        .payload_data = (uint8_t *)payload,
        .payload_size = strlen(payload),
    };
    SDL_Rect pixel = {
        .x = 0,
        .y = 0,
        .w = 1,
        .h = 1,
    };
    SDL_Rect draw_area = {
        .x = 0,
        .y = 0,
    };
    uint8_t output[QRCODEC_MAX_BUFFER_SIZE];
    qrcodec_qr qrcode = {
        .bitmap_data = &output[0],
        .bitmap_size = QRCODEC_MAX_BUFFER_SIZE,
    };
    size_t qrsize;
    bool value;
    SDL_DisplayMode display_mode;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    SDL_GetCurrentDisplayMode(0, &display_mode);

    window = SDL_CreateWindow("QRCodec Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, display_mode.w / 2, display_mode.h / 2, 0);
    draw_area.w = display_mode.h / 4;
    draw_area.h = display_mode.h / 4;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    
    qrcodec_encode(&qrdata, &qrcode);
    qrcodec_qr_get_size(&qrcode, &qrsize);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, qrsize, qrsize);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, texture);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
    for (size_t x = 0; x < qrsize; x++)
    {
        for (size_t y = 0; y < qrsize; y++)
        {
            qrcodec_qr_get_value(&qrcode, x, y, &value);
            if (value)
            {
                pixel.x = x;
                pixel.y = y;
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, &draw_area);
    SDL_RenderPresent(renderer);
}

static void main_loop()
{
    bool running;
    SDL_Event event;

    running = true;
    while(running)
    {
        if(SDL_PollEvent(&event) > 0)
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDLK_ESCAPE)
            {
                running = false;
            }
        }
        else
        {
            SDL_Delay(10);
        }
    }
}

static void shutdown()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}