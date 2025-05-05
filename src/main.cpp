#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <cmath>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;
const int GRID_SIZE = 8;
const int TILE_SIZE = 80;
const int NUM_CANDIES = 5;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* candyTextures[NUM_CANDIES];
Mix_Music* bgMusic = nullptr;
TTF_Font* font = nullptr;

int grid[GRID_SIZE][GRID_SIZE];
int score = 0;

// Ô kẹo được chọn
bool candySelected = false;
int selectedX = -1, selectedY = -1;

// Load ảnh thành SDL_Texture
SDL_Texture* loadTexture(const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cout << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// Hiển thị văn bản điểm số
SDL_Texture* renderText(const std::string& message, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, message.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// Tạo lưới kẹo ngẫu nhiên
void initGrid() {
    for (int y = 0; y < GRID_SIZE; ++y)
        for (int x = 0; x < GRID_SIZE; ++x)
            grid[y][x] = rand() % NUM_CANDIES;
}

// Vẽ toàn bộ lưới
void renderGrid() {
    for (int y = 0; y < GRID_SIZE; ++y)
        for (int x = 0; x < GRID_SIZE; ++x) {
            SDL_Rect dst = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            SDL_RenderCopy(renderer, candyTextures[grid[y][x]], nullptr, &dst);
        }

    // Vẽ viền đỏ cho ô đang chọn
    if (candySelected) {
        SDL_Rect rect = { selectedX * TILE_SIZE, selectedY * TILE_SIZE, TILE_SIZE, TILE_SIZE };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &rect);
    }
}

// Kiểm tra và đánh dấu match
bool findAndClearMatches() {
    bool matched = false;
    bool marked[GRID_SIZE][GRID_SIZE] = {};

    // Match hàng
    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x <= GRID_SIZE - 3; ) {
            int start = x;
            int type = grid[y][x];
            while (x < GRID_SIZE && grid[y][x] == type) ++x;
            int len = x - start;
            if (len >= 3) {
                matched = true;
                score += (len == 3 ? 10 : (len == 4 ? 20 : 50));
                for (int i = start; i < x; ++i) marked[y][i] = true;
            }
        }
    }

    // Match cột
    for (int x = 0; x < GRID_SIZE; ++x) {
        for (int y = 0; y <= GRID_SIZE - 3; ) {
            int start = y;
            int type = grid[y][x];
            while (y < GRID_SIZE && grid[y][x] == type) ++y;
            int len = y - start;
            if (len >= 3) {
                matched = true;
                score += (len == 3 ? 10 : (len == 4 ? 20 : 50));
                for (int i = start; i < y; ++i) marked[i][x] = true;
            }
        }
    }

    for (int y = 0; y < GRID_SIZE; ++y)
        for (int x = 0; x < GRID_SIZE; ++x)
            if (marked[y][x]) grid[y][x] = -1;

    return matched;
}

// Rơi xuống và sinh mới
void collapseGrid() {
    for (int x = 0; x < GRID_SIZE; ++x) {
        int writeRow = GRID_SIZE - 1;
        for (int y = GRID_SIZE - 1; y >= 0; --y) {
            if (grid[y][x] != -1) {
                grid[writeRow--][x] = grid[y][x];
            }
        }
        while (writeRow >= 0)
            grid[writeRow--][x] = rand() % NUM_CANDIES;
    }
}

// Kiểm tra có phải kề cạnh không
bool isAdjacent(int x1, int y1, int x2, int y2) {
    return (std::abs(x1 - x2) + std::abs(y1 - y2)) == 1;
}

// Swap 2 viên kẹo
void swap(int x1, int y1, int x2, int y2) {
    std::swap(grid[y1][x1], grid[y2][x2]);
}

int main(int argc, char* argv[]) {
    srand((unsigned int)time(nullptr));
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    TTF_Init();

    window = SDL_CreateWindow("Candy Crush Clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load ảnh kẹo
    candyTextures[0] = loadTexture("res/kẹo hồng-01.png");
    candyTextures[1] = loadTexture("res/kẹo nâu-01.png");
    candyTextures[2] = loadTexture("res/kẹo vàng-01.png");
    candyTextures[3] = loadTexture("res/kẹo xanh lá-01.png");
    candyTextures[4] = loadTexture("res/kẹo xanh-01.png");

    font = TTF_OpenFont("res/LoveDays-2v7Oe.ttf", 24);
    if (!font) {
        std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        return 1;
    }

    bgMusic = Mix_LoadMUS("res/bg_music.mp3");
    if (bgMusic) Mix_PlayMusic(bgMusic, -1);

    initGrid();

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;

            // Xử lý chuột click
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x / TILE_SIZE;
                int y = e.button.y / TILE_SIZE;

                if (!candySelected) {
                    selectedX = x;
                    selectedY = y;
                    candySelected = true;
                } else {
                    if (isAdjacent(x, y, selectedX, selectedY)) {
                        swap(x, y, selectedX, selectedY);
                        if (!findAndClearMatches()) {
                            // Không match → swap lại
                            swap(x, y, selectedX, selectedY);
                        } else {
                            // Nếu match → collapse
                            collapseGrid();
                        }
                    }
                    candySelected = false;
                }
            }
        }

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

    for (auto& tex : candyTextures) SDL_DestroyTexture(tex);
    Mix_FreeMusic(bgMusic);
    TTF_CloseFont(font);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}