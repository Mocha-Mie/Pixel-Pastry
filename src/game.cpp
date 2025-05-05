#include "game.h"
#include "resource.h"
#include <cstdlib>
#include <cmath>

int grid[8][8];
int score = 0;
int movesLeft = 20;
bool candySelected = false;
int selectedX = -1, selectedY = -1;
bool showMatches[8][8] = {};
int matchType[8][8] = {}; // 0: không match, 3: match3, 4: match4, 5: match5

// Khởi tạo lưới ban đầu
void initGrid() {
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
            grid[y][x] = rand() % 5;
}

// Vẽ lưới kẹo và hiệu ứng
void renderGrid() {
    SDL_SetRenderDrawColor(renderer, 173, 216, 230, 255);
    SDL_Rect gameArea = {0, 0, 640, 640};
    SDL_RenderFillRect(renderer, &gameArea);

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            SDL_Rect dst = {x * 80, y * 80, 80, 80};
            SDL_RenderCopy(renderer, candyTextures[grid[y][x]], nullptr, &dst);

            if (showMatches[y][x]) {
                if (matchType[y][x] == 3) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // vàng
                } else if (matchType[y][x] == 4) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // trắng flash
                } else if (matchType[y][x] == 5) {
                    SDL_SetRenderDrawColor(renderer, 160, 32, 240, 255); // tím sparkle
                }
                SDL_RenderDrawRect(renderer, &dst);
            }
        }
    }

    if (candySelected) {
        SDL_Rect rect = {selectedX * 80, selectedY * 80, 80, 80};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &rect);
    }
}

// Tìm các ô match, phân loại match 3/4/5 và tạo hiệu ứng tương ứng
bool findAndClearMatches() {
    bool matched = false;
    bool marked[8][8] = {};
    int localType[8][8] = {}; // phân biệt 3 / 4 / 5 để vẽ hiệu ứng

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

// Làm kẹo rơi xuống từng pixel để tạo hiệu ứng mượt
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

// Kiểm tra hai ô có kề nhau không
bool isAdjacent(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2) == 1;
}

// Hoán đổi vị trí hai ô
void swap(int x1, int y1, int x2, int y2) {
    std::swap(grid[y1][x1], grid[y2][x2]);
}