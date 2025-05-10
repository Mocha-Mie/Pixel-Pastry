#include "game.h"
#include <SDL_image.h>
#include <ctime>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "resource.h"

// Hàm main
int main(int argc, char* argv[]) {
    // Khởi tạo SDL và các thư viện cần thiết
    srand((unsigned int)time(nullptr));
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    TTF_Init();

    // Tạo cửa sổ và renderer
    window = SDL_CreateWindow("Pixel Pastry", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!initResources()) return 1;
    initGrid();

    bool quit = false;
    bool playing = true;
    SDL_Event e;  // Khai báo biến sự kiện chuột/bàn phím
    while (playing) {
        quit = false;
        while (!quit) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) quit = true;
                handleMouse(e); // Xử lý click chuột
            }

            if (findAndClearMatches()) collapseGrid();

            SDL_RenderClear(renderer);

            renderUI(score, movesLeft); // Vẽ khung giao diện
            renderGrid();               // Vẽ kẹo
            SDL_RenderPresent(renderer);

            SDL_Delay(150);

            // Khi hết lượt thì thoát vòng lặp
            if (movesLeft <= 0) quit = true;
        }
        // Hiện khung Game Over và lựa chọn
        showGameOverScreenWithButtons(e, playing);
    }

    // Giải phóng bộ nhớ
    for (auto& tex : candyTextures) SDL_DestroyTexture(tex);
    Mix_FreeMusic(bgMusic);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
