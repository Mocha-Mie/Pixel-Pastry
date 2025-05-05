// game.cpp - Xử lý logic game: tạo lưới, kiểm tra match, hoán đổi, vẽ
#include "game.h"
#include "resource.h"
#include <cstdlib>
#include <cmath>

// Biến toàn cục
int grid[8][8];
int score = 0;
bool candySelected = false;
int selectedX = -1, selectedY = -1;

// Khởi tạo lưới kẹo ngẫu nhiên
void initGrid() {
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
            grid[y][x] = rand() % 5;
}

// Vẽ toàn bộ lưới kẹo và viền đỏ nếu đang chọn
void renderGrid() {
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x) {
            SDL_Rect dst = { x * 80, y * 80, 80, 80 };
            SDL_RenderCopy(renderer, candyTextures[grid[y][x]], nullptr, &dst);
        }

    if (candySelected) {
        SDL_Rect rect = { selectedX * 80, selectedY * 80, 80, 80 };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &rect);
    }
}

// Kiểm tra và đánh dấu các hàng/cột match, cộng điểm
bool findAndClearMatches() {
    bool matched = false;
    bool marked[8][8] = {};

    // Kiểm tra hàng ngang
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x <= 5;) {
            int start = x, type = grid[y][x];
            while (x < 8 && grid[y][x] == type) ++x;
            int len = x - start;
            if (len >= 3) {
                matched = true;
                score += (len == 3 ? 10 : (len == 4 ? 20 : 50));
                for (int i = start; i < x; ++i) marked[y][i] = true;
            }
        }

    // Kiểm tra cột dọc
    for (int x = 0; x < 8; ++x)
        for (int y = 0; y <= 5;) {
            int start = y, type = grid[y][x];
            while (y < 8 && grid[y][x] == type) ++y;
            int len = y - start;
            if (len >= 3) {
                matched = true;
                score += (len == 3 ? 10 : (len == 4 ? 20 : 50));
                for (int i = start; i < y; ++i) marked[i][x] = true;
            }
        }

    // Đánh dấu xóa
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
            if (marked[y][x]) grid[y][x] = -1;

    return matched;
}

// Làm kẹo rơi xuống và tạo mới
void collapseGrid() {
    for (int x = 0; x < 8; ++x) {
        int writeRow = 7;
        for (int y = 7; y >= 0; --y)
            if (grid[y][x] != -1)
                grid[writeRow--][x] = grid[y][x];
        while (writeRow >= 0)
            grid[writeRow--][x] = rand() % 5;
    }
}

// Kiểm tra 2 ô có kề nhau không
bool isAdjacent(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2) == 1;
}

// Hoán đổi 2 ô
void swap(int x1, int y1, int x2, int y2) {
    std::swap(grid[y1][x1], grid[y2][x2]);
}
