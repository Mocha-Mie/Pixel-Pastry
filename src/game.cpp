#include "game.h"
#include "resource.h"
#include <SDL.h>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

// Lưới game và các biến toàn cục
int grid[GRID_SIZE][GRID_SIZE];
int score = 0;
int movesLeft = 15;
bool candySelected = false;
int selectedX = -1, selectedY = -1;
bool showMatches[GRID_SIZE][GRID_SIZE] = {}; // Đánh dấu các ô được match để vẽ hiệu ứng
int matchType[8][8] = {};

// Tạo lưới ban đầu
void initGrid() {
    for (int y = 0; y < GRID_SIZE; ++y)
        for (int x = 0; x < GRID_SIZE; ++x)
            grid[y][x] = rand() % NUM_CANDIES;
}

// Kiểm tra hai ô có kề nhau không
bool isAdjacent(int x1, int y1, int x2, int y2) {
    return (abs(x1 - x2) + abs(y1 - y2)) == 1;
}

// Hoán đổi hai ô
void swap(int x1, int y1, int x2, int y2) {
    swap(grid[y1][x1], grid[y2][x2]);
}

// Vẽ lưới kẹo và hiệu ứng
void renderGrid() {
    SDL_SetRenderDrawColor(renderer, 201, 234, 242, 255);
    SDL_Rect gameArea = {0, 80, 640, 660};
    SDL_RenderFillRect(renderer, &gameArea);

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            SDL_Rect dst = {x * 80, y * 80 + 90, 80, 80};
            SDL_RenderCopy(renderer, candyTextures[grid[y][x]], nullptr, &dst);

            if (showMatches[y][x]) {
                if (matchType[y][x] == 3) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // vàng
                } else if (matchType[y][x] == 4) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // trắng
                } else if (matchType[y][x] == 5) {
                    SDL_SetRenderDrawColor(renderer, 160, 32, 240, 255); // tím
                }
                SDL_RenderDrawRect(renderer, &dst);
            }
        }
    }

    if (candySelected) {
        SDL_Rect rect = {selectedX * 80, selectedY * 80 + 90, 80, 80};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &rect);
    }
}

// Tìm các ô match, phân loại match 3/4/5 và tạo hiệu ứng tương ứng
bool findAndClearMatches() {
    bool matched = false;
    bool marked[8][8] = {};
    int localType[8][8] = {};

    // Kiểm tra hàng ngang
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x <= 5;) {
            int start = x, type = grid[y][x];
            while (x < 8 && grid[y][x] == type) ++x;
            int len = x - start;
            if (len >= 3) {
                matched = true;
                int matchScore = (len == 3 ? 10 : (len == 4 ? 20 : 50));
                score += matchScore;
                for (int i = start; i < x; ++i) {
                    marked[y][i] = true;
                    localType[y][i] = len;
                }
            }
        }
    }

    // Kiểm tra cột dọc
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y <= 5;) {
            int start = y, type = grid[y][x];
            while (y < 8 && grid[y][x] == type) ++y;
            int len = y - start;
            if (len >= 3) {
                matched = true;
                score += (len == 3 ? 10 : (len == 4 ? 20 : 50));
                for (int i = start; i < y; ++i) {
                    marked[i][x] = true;
                    localType[i][x] = len;
                }
            }
        }
    }

    // Gán hiệu ứng để vẽ
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x) {
            showMatches[y][x] = marked[y][x];
            matchType[y][x] = localType[y][x];
        }

    // Hiệu ứng flash/sparkle
    if (matched) {
        for (int flash = 0; flash < 3; ++flash) {
            renderGrid();
            SDL_RenderPresent(renderer);
            SDL_Delay(100);
        }
    }

    // Xóa ô bị match
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
            if (marked[y][x]) grid[y][x] = -1;

    return matched;
}

// Làm kẹo rơi xuống
void collapseGrid() {
    bool falling = true;

    while (falling) {
        falling = false;

        for (int x = 0; x < 8; ++x) {
            for (int y = 7; y > 0; --y) {
                if (grid[y][x] == -1 && grid[y - 1][x] != -1) {
                    grid[y][x] = grid[y - 1][x];
                    grid[y - 1][x] = -1;
                    falling = true;
                }
            }
        }

        // Vẽ từng bước rơi
        renderGrid();
        SDL_RenderPresent(renderer);
        SDL_Delay(60); // delay ngắn để tạo hiệu ứng rơi
    }

    // Sau khi rơi xong → tạo kẹo mới ở vị trí rỗng
    for (int x = 0; x < 8; ++x)
        for (int y = 0; y < 8; ++y)
            if (grid[y][x] == -1)
                grid[y][x] = rand() % 5;

    renderGrid();
    SDL_RenderPresent(renderer);
    SDL_Delay(60);

    // Xóa hiệu ứng sau khi rơi hoàn tất
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x) {
            showMatches[y][x] = false;
            matchType[y][x] = 0;
        }
}

// Xử lý click chuột
void handleMouse(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int x = e.button.x / TILE_SIZE;
        int y = (e.button.y - 90) / TILE_SIZE;
        if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
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
}
void renderUI(int score, int movesLeft) {
    // Vẽ thanh trên màu tím nhạt
    SDL_SetRenderDrawColor(renderer, 216, 191, 216, 255);
    SDL_Rect topBar = {0, 0, SCREEN_WIDTH, 80};
    SDL_RenderFillRect(renderer, &topBar);

    // Vẽ khung điểm (hồng đậm)
    SDL_SetRenderDrawColor(renderer, 252, 66, 168, 255); 
    SDL_Rect scoreRect = {30, 20, 180, 40};
    SDL_RenderFillRect(renderer, &scoreRect);

    // Vẽ khung lượt đi (be nhạt)
    SDL_SetRenderDrawColor(renderer, 245, 222, 179, 255);
    SDL_Rect moveRect = {SCREEN_WIDTH - 250, 20, 220, 40};
    SDL_RenderFillRect(renderer, &moveRect);

    // Hiển thị điểm (chữ trắng, căn giữa)
    string scoreStr = "Scores: " + to_string(score);
    SDL_Surface* scoreSurface = TTF_RenderUTF8_Blended(font, scoreStr.c_str(), {255, 255, 255});
    SDL_Texture* scoreTex = SDL_CreateTextureFromSurface(renderer, scoreSurface);

    int textW, textH;
    TTF_SizeUTF8(font, scoreStr.c_str(), &textW, &textH);
    SDL_Rect scoreTextRect = {
        scoreRect.x + (scoreRect.w - textW) / 2,
        scoreRect.y + (scoreRect.h - textH) / 2,
        textW, textH
    };
    SDL_RenderCopy(renderer, scoreTex, nullptr, &scoreTextRect);
    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(scoreTex);

    // Hiển thị lượt đi (chữ nâu)
    string moveStr = "Move left: " + to_string(movesLeft);
    SDL_Surface* moveSurface = TTF_RenderUTF8_Blended(font, moveStr.c_str(), {139, 69, 19});
    SDL_Texture* moveTex = SDL_CreateTextureFromSurface(renderer, moveSurface);

    TTF_SizeUTF8(font, moveStr.c_str(), &textW, &textH);
    SDL_Rect moveTextRect = {
        moveRect.x + (moveRect.w - textW) / 2,
        moveRect.y + (moveRect.h - textH) / 2,
        textW, textH
    };
    SDL_RenderCopy(renderer, moveTex, nullptr, &moveTextRect);
    SDL_FreeSurface(moveSurface);
    SDL_DestroyTexture(moveTex);
}

void showGameOverScreenWithButtons(SDL_Event &e, bool &playing) {
        bool inMenu = true;
    
        SDL_Rect restartBtn = { SCREEN_WIDTH / 2 - 110, SCREEN_HEIGHT / 2 + 40, 100, 40 };
        SDL_Rect exitBtn = { SCREEN_WIDTH / 2 + 10, SCREEN_HEIGHT / 2 + 40, 100, 40 };
    
        while (inMenu) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    playing = false;
                    inMenu = false;
                } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                    int x = e.button.x;
                    int y = e.button.y;
    
                    if (x >= restartBtn.x && x <= restartBtn.x + restartBtn.w &&
                        y >= restartBtn.y && y <= restartBtn.y + restartBtn.h) {
                        // Chơi lại
                        score = 0;
                        movesLeft = 15;
                        initGrid();
                        Mix_PlayMusic(bgMusic, -1); // Phát lại nhạc
                        inMenu = false;
                    }
                    else if (x >= exitBtn.x && x <= exitBtn.x + exitBtn.w &&
                             y >= exitBtn.y && y <= exitBtn.y + exitBtn.h) {
                        playing = false;
                        inMenu = false;
                    }
                }
            }
    
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
    
            // Vẽ chữ Game Over
            SDL_Color white = { 255, 255, 255 };
            SDL_Surface* surface = TTF_RenderText_Solid(font, "Game Over", white);
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect textRect = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 80, 200, 40 };
            SDL_RenderCopy(renderer, tex, nullptr, &textRect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(tex);
    
            // Hiển thị điểm
            string scoreStr = "Your score: " + to_string(score);
            surface = TTF_RenderText_Solid(font, scoreStr.c_str(), white);
            tex = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect scoreRect = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 30, 200, 30 };
            SDL_RenderCopy(renderer, tex, nullptr, &scoreRect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(tex);
    
            // Vẽ nút Restartt
            SDL_RenderDrawRect(renderer, &restartBtn);
            string restartText = "Restart";
            surface = TTF_RenderText_Solid(font, restartText.c_str(), white);
            tex = SDL_CreateTextureFromSurface(renderer, surface);
            int w, h;
            TTF_SizeText(font, restartText.c_str(), &w, &h);
            SDL_Rect restartTextRect = {
                restartBtn.x + (restartBtn.w - w) / 2,
                restartBtn.y + (restartBtn.h - h) / 2,
                w, h
            };
            SDL_RenderCopy(renderer, tex, nullptr, &restartTextRect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(tex);
    
            // Vẽ nút Exit
            SDL_RenderDrawRect(renderer, &exitBtn);
            string exitText = "Exit";
            surface = TTF_RenderText_Solid(font, exitText.c_str(), white);
            tex = SDL_CreateTextureFromSurface(renderer, surface);
            TTF_SizeText(font, exitText.c_str(), &w, &h);
            SDL_Rect exitTextRect = {
                exitBtn.x + (exitBtn.w - w) / 2 + 20,
                exitBtn.y + (exitBtn.h - h) / 2,
                w, h
            };
            SDL_RenderCopy(renderer, tex, nullptr, &exitTextRect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(tex);
    
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }
    }
