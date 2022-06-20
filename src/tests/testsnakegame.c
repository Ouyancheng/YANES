#include "sdk.h"
#include "SDL2/SDL.h"
#include "system.h"
static uint8_t snake_game[] = {
    0x20, 0x06, 0x06, 0x20, 0x38, 0x06, 0x20, 0x0d, 0x06, 0x20, 0x2a, 0x06, 0x60, 0xa9, 0x02, 0x85,
    0x02, 0xa9, 0x04, 0x85, 0x03, 0xa9, 0x11, 0x85, 0x10, 0xa9, 0x10, 0x85, 0x12, 0xa9, 0x0f, 0x85,
    0x14, 0xa9, 0x04, 0x85, 0x11, 0x85, 0x13, 0x85, 0x15, 0x60, 0xa5, 0xfe, 0x85, 0x00, 0xa5, 0xfe,
    0x29, 0x03, 0x18, 0x69, 0x02, 0x85, 0x01, 0x60, 0x20, 0x4d, 0x06, 0x20, 0x8d, 0x06, 0x20, 0xc3,
    0x06, 0x20, 0x19, 0x07, 0x20, 0x20, 0x07, 0x20, 0x2d, 0x07, 0x4c, 0x38, 0x06, 
    0xa5, 
    0xff, 
    0xc9, 0x77, 
    0xf0, 0x0d, 
    0xc9, 0x64, 
    0xf0, 0x14, 
    0xc9, 0x73, 
    0xf0, 0x1b, 
    0xc9, 0x61, 
    0xf0, 0x22, 
    0x60,
    0xa9, 0x04, 0x24, 0x02, 0xd0, 0x26, 0xa9, 0x01, 0x85, 0x02, 0x60, 0xa9, 0x08, 0x24, 0x02, 0xd0,
    0x1b, 0xa9, 0x02, 0x85, 0x02, 0x60, 0xa9, 0x01, 0x24, 0x02, 0xd0, 0x10, 0xa9, 0x04, 0x85, 0x02,
    0x60, 0xa9, 0x02, 0x24, 0x02, 0xd0, 0x05, 0xa9, 0x08, 0x85, 0x02, 0x60, 0x60, 0x20, 0x94, 0x06,
    0x20, 0xa8, 0x06, 0x60, 0xa5, 0x00, 0xc5, 0x10, 0xd0, 0x0d, 0xa5, 0x01, 0xc5, 0x11, 0xd0, 0x07,
    0xe6, 0x03, 0xe6, 0x03, 0x20, 0x2a, 0x06, 0x60, 0xa2, 0x02, 0xb5, 0x10, 0xc5, 0x10, 0xd0, 0x06,
    0xb5, 0x11, 0xc5, 0x11, 0xf0, 0x09, 0xe8, 0xe8, 0xe4, 0x03, 0xf0, 0x06, 0x4c, 0xaa, 0x06, 0x4c,
    0x35, 0x07, 0x60, 0xa6, 0x03, 0xca, 0x8a, 0xb5, 0x10, 0x95, 0x12, 0xca, 0x10, 0xf9, 0xa5, 0x02,
    0x4a, 0xb0, 0x09, 0x4a, 0xb0, 0x19, 0x4a, 0xb0, 0x1f, 0x4a, 0xb0, 0x2f, 0xa5, 0x10, 0x38, 0xe9,
    0x20, 0x85, 0x10, 0x90, 0x01, 0x60, 0xc6, 0x11, 0xa9, 0x01, 0xc5, 0x11, 0xf0, 0x28, 0x60, 0xe6,
    0x10, 0xa9, 0x1f, 0x24, 0x10, 0xf0, 0x1f, 0x60, 0xa5, 0x10, 0x18, 0x69, 0x20, 0x85, 0x10, 0xb0,
    0x01, 0x60, 0xe6, 0x11, 0xa9, 0x06, 0xc5, 0x11, 0xf0, 0x0c, 0x60, 0xc6, 0x10, 0xa5, 0x10, 0x29,
    0x1f, 0xc9, 0x1f, 0xf0, 0x01, 0x60, 0x4c, 0x35, 0x07, 0xa0, 0x00, 0xa5, 0xfe, 0x91, 0x00, 0x60,
    0xa6, 0x03, 0xa9, 0x00, 0x81, 0x10, 0xa2, 0x00, 0xa9, 0x01, 0x81, 0x10, 0x60, 0xa2, 0x00, 0xea,
    0xea, 0xca, 0xd0, 0xfb, 0x60
};


static SDL_Window* win;
static SDL_Renderer *renderer; 
static SDL_Surface *surface; 
static SDL_Texture *texture; 
static uint8_t *pixels;
static int pitch;
static void cleanup_and_exit(int code) {
    SDL_DestroyTexture(texture); 
    SDL_DestroyRenderer(renderer); 
    SDL_DestroyWindow(win);
    SDL_Quit();
    exit(code); 
}
static void handle_input(struct nescpu *pcpu) {
    SDL_Event e;
    bool quit = false;
    int ret = SDL_PollEvent(&e); 
    
    if (!ret) { return; }
    if (e.type == SDL_QUIT) {
        cleanup_and_exit(0); 
    }
    if (e.type == SDL_KEYDOWN) {
        SDL_Keycode keycode = e.key.keysym.sym; 
        switch (keycode) {
            case SDLK_w: cpu_write8(pcpu, 0xff, 0x77); break; 
            case SDLK_a: cpu_write8(pcpu, 0xff, 0x61); break; 
            case SDLK_s: cpu_write8(pcpu, 0xff, 0x73); break; 
            case SDLK_d: cpu_write8(pcpu, 0xff, 0x64); break; 
            case SDLK_ESCAPE: 
            case SDLK_q: cleanup_and_exit(0); break; 
            default: break; 
        }
    }
}

static struct rgb get_color(uint8_t b) {
    switch (b) {
    case 0: return (struct rgb){.r=0,.g=0,.b=0,.a=SDL_ALPHA_OPAQUE};
    case 1: return (struct rgb){.r=0xff,.g=0xff,.b=0xff,.a=SDL_ALPHA_OPAQUE};
    case 2: 
    case 9: return (struct rgb){.r=0x6f,.g=0x6f,.b=0x6f,.a=SDL_ALPHA_OPAQUE};
    case 3: 
    case 10: return (struct rgb){.r=0xff,.g=0,.b=0,.a=SDL_ALPHA_OPAQUE};
    case 4: 
    case 11: return (struct rgb){.r=0,.g=0xff,.b=0,.a=SDL_ALPHA_OPAQUE};
    case 5: 
    case 12: return (struct rgb){.r=0,.g=0,.b=0xff,.a=SDL_ALPHA_OPAQUE};
    case 6: 
    case 13: return (struct rgb){.r=0xff,.g=0,.b=0xff,.a=SDL_ALPHA_OPAQUE};
    case 7: 
    case 14: return (struct rgb){.r=0xff,.g=0xff,.b=0,.a=SDL_ALPHA_OPAQUE};
    default: return (struct rgb){.r=0,.g=0xff,.b=0xff,.a=SDL_ALPHA_OPAQUE};
    }
}

static struct rgb framebuffer[32*32]; 
static int read_screen_state(struct nescpu *pcpu) {
    int update = 0; 
    unsigned x; 
    unsigned y; 
    unsigned idx; 
    for (uint16_t i = 0x0200; i < 0x0600; ++i) {
        // 512 - 1535 
        // 1536 - 512 = 1024 
        uint8_t color_idx = cpu_read8(pcpu, i); 
        struct rgb color = get_color(color_idx); 
        idx = i-0x0200; 
        y = idx / 32; 
        x = idx - (32 * y); 
        framebuffer[idx] = color; 
    }
    return update; 
}
static int cnt = 0; 
static uint8_t random_number = 0; 
static void callback(struct nescpu *cpu) {
    cnt += 1; 
    if (cnt % 600 == 0) {
        handle_input(cpu); 
        random_number = (random_number + 1) % 16; 
        cpu_write8(cpu, 0xfe, random_number);
        read_screen_state(cpu); 
        if (cnt % 600000 == 0) printf("cnt=%d\n", cnt); 
        if (SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch) != 0) {
            panic("failed to lock texture!\n"); 
        } 
        for (int y = 0; y < 32; ++y) {
            for (int x = 0; x < 32; ++x) {
                pixels[y*pitch + 4*x]   = framebuffer[y*32+x].a;
                pixels[y*pitch + 4*x+1] = framebuffer[y*32+x].b;
                pixels[y*pitch + 4*x+2] = framebuffer[y*32+x].g;
                pixels[y*pitch + 4*x+3] = framebuffer[y*32+x].r;
            }
        }
        SDL_UnlockTexture(texture); 
        if (SDL_RenderCopy(renderer, texture, NULL, NULL) != 0) {
            panic("failed to copy texture\n"); 
        }
        SDL_RenderPresent(renderer); 
        
        SDL_Delay(42); 
    }
}
struct nessystem snakegame_system;
uint8_t empty_romdata[0x6010];
void test_snake(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        panic("error initializing SDL: %s\n", SDL_GetError());
    }
    win = SDL_CreateWindow(
        "GAME",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        32*10, 32*10, 0
    );
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal"); 
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderSetScale(renderer, 10.0, 10.0); 
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 32, 32); 
    if (!texture) {
        panic("failed to get texture!\n"); 
    }
    SDL_SetRenderTarget(renderer, texture);
    if (SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch) != 0) {
        panic("failed to lock texture!\n"); 
    } 
    empty_romdata[0] = 'N';
    empty_romdata[1] = 'E';
    empty_romdata[2] = 'S';
    empty_romdata[3] = 0x1A;
    empty_romdata[4] = 0x01; // PRG = 1 * 16KB
    empty_romdata[5] = 0x01; // CHR = 1 * 8KB
    system_init(&snakegame_system);
    int ret = system_load_rom_ines(&snakegame_system, empty_romdata, 0x6010);
    if (ret < 0) {
        panic("failed to load rom!\n");
    }
    
    printf("PRG offset=%u PRG size=%u CHR size=%u\n", snakegame_system.rom->PRG_rom_offset, snakegame_system.rom->PRG_rom_size, snakegame_system.rom->CHR_rom_size);
    for (size_t i = 0; i < sizeof(snake_game); ++i) {
        cpu_write8(&(snakegame_system.cpu), UINT16_C(0x0600) + (uint16_t)i, snake_game[i]); 
    }
    
    system_test_run(&snakegame_system, 0x0600, callback);
    /// NOTE: the PRG is NOT writable
    // cpu_write16(&(snakegame_system.cpu), CPU_RESET_VECTOR, 0x0600);
    // cpu_reset(&(snakegame_system.cpu)); 
    // printf("reset vector = %04X cpu.pc = %04X\n", cpu_peek16(&(snakegame_system.cpu), CPU_RESET_VECTOR), snakegame_system.cpu.pc);
    // cpu_run(&(snakegame_system.cpu), callback); 
    // cpu_write16(&(snakegame_system.cpu), 0xFFFC, 0x0600);
    // cpu_reset(&(snakegame_system.cpu)); 
    // void dump_cpu(struct nescpu *cpu);
    // cpu_run(&(snakegame_system.cpu), dump_cpu); 
}


