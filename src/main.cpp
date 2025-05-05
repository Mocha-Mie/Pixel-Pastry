#include "game.h"
#include "resource.h"
#include <iostream>

// Hiện màn hình Game Over
void showGameOverScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color white = {255, 255, 255};
    SDL_Texture* gameOverText = renderText("Game Over!", white);
    SDL_Rect overRect = {280, 220, 240, 40};
    SDL_RenderCopy(renderer, gameOverText, nullptr, &overRect);

    SDL_Texture* scoreText = renderText("Your Score: " + std::to_string(score), white);
    SDL_Rect scoreRect = {270, 280, 260, 40};
    SDL_RenderCopy(renderer, scoreText, nullptr, &scoreRect);

    SDL_RenderPresent(renderer);
    SDL_Delay(2500);

    SDL_DestroyTexture(gameOverText);
    SDL_DestroyTexture(scoreText);
}

int main(int argc, char* argv[]) {
    srand((unsigned int)time(nullptr));
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    TTF_Init();

    if (!initResources()) return 1;

restart_game:
    score = 0;
    movesLeft = 20;
    initGrid();

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;

            if (e.type == SDL_MOUSEBUTTONDOWN && movesLeft > 0) {
                int x = e.button.x / 80;
                int y = e.button.y / 80;

                if (!candySelected) {
                    selectedX = x;
                    selectedY = y;
                    candySelected = true;
                } else {
                    if (isAdjacent(x, y, selectedX, selectedY)) {
                        swap(x, y, selectedX, selectedY);
                        if (!findAndClearMatches())
                            swap(x, y, selectedX, selectedY);
                        else {
                            collapseGrid();
                            movesLeft--;
                        }
                    }
                    candySelected = false;
                }
            }
        }

        if (findAndClearMatches()) collapseGrid();

        renderGrid();

        // Vẽ khung điểm
        SDL_Rect scorePanel = {640, 0, 160, 320};
        SDL_SetRenderDrawColor(renderer, 255, 105, 180, 255); // hồng
        SDL_RenderFillRect(renderer, &scorePanel);

        // Vẽ khung lượt
        SDL_Rect movePanel = {640, 320, 160, 320};
        SDL_SetRenderDrawColor(renderer, 245, 245, 220, 255); // be nhạt
        SDL_RenderFillRect(renderer, &movePanel);

        SDL_Color white = {255, 255, 255};
        SDL_Texture* scoreText = renderText("Score: " + std::to_string(score), white);
        SDL_Rect scoreRect = {650, 50, 140, 30};
        SDL_RenderCopy(renderer, scoreText, nullptr, &scoreRect);
        SDL_DestroyTexture(scoreText);

        SDL_Color brown = {139, 69, 19};
        SDL_Texture* moveText = renderText("Moves: " + std::to_string(movesLeft), brown);
        SDL_Rect moveRect = {650, 370, 140, 30};
        SDL_RenderCopy(renderer, moveText, nullptr, &moveRect);
        SDL_DestroyTexture(moveText);

        SDL_RenderPresent(renderer);
        SDL_Delay(100);

        if (movesLeft <= 0) {
            showGameOverScreen();

            std::cout << "\n1. Restart game\n2. Exit\nNumber: ";
            int choice;
            std::cin >> choice;
            std::cin.ignore();

            if (choice == 1) {
                goto restart_game; // chơi lại từ đầu
            } else {
                quit = true;
            }
        }
    }

    cleanupResources();
    SDL_Quit(); IMG_Quit(); TTF_Quit();
    return 0;
}