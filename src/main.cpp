// main.cpp - Hàm main quản lý khởi tạo, vòng lặp game và xử lý chuột
#include "game.h"
#include "resource.h"
#include <iostream>

// Hàm main khởi tạo SDL và bắt đầu game
int main(int argc, char* argv[]) {
    srand((unsigned int)time(nullptr));
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    TTF_Init();

    if (!initResources()) {
        std::cout << "Failed to initialize resources!" << std::endl;
        return 1;
    }

    initGrid();

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        // Bắt sự kiện thoát và click chuột
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x / 80;
                int y = e.button.y / 80;
                if (!candySelected) {
                    selectedX = x; selectedY = y;
                    candySelected = true;
                } else {
                    if (isAdjacent(x, y, selectedX, selectedY)) {
                        swap(x, y, selectedX, selectedY);
                        if (!findAndClearMatches()) swap(x, y, selectedX, selectedY);
                        else collapseGrid();
                    }
                    candySelected = false;
                }
            }
        }

        // Xử lý tự động match và vẽ
        if (findAndClearMatches()) collapseGrid();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        renderGrid();

        SDL_Color white = {255, 255, 255};
        SDL_Texture* scoreTexture = renderText("Score: " + std::to_string(score), white);
        SDL_Rect scoreRect = {10, 10, 180, 30};
        SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);
        SDL_DestroyTexture(scoreTexture);

        SDL_RenderPresent(renderer);
        SDL_Delay(150);
    }

    cleanupResources();
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    return 0;
}
