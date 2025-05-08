#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>

using namespace std;

// Kích thước lưới và cửa sổ game
const int GRID_SIZE = 8;
const int TILE_SIZE = 80;
const int SCREEN_WIDTH = GRID_SIZE * TILE_SIZE;
const int UI_HEIGHT = 100; // Chiều cao thanh UI
const int SCREEN_HEIGHT = GRID_SIZE * TILE_SIZE + UI_HEIGHT;
const int NUM_CANDIES = 5;

// Khai báo tài nguyên toàn cục
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* candyTextures[NUM_CANDIES];
extern TTF_Font* font;
extern Mix_Music* bgMusic;

// Hàm load texture từ file ảnh
SDL_Texture* loadTexture(const string& path);

// Hàm render chữ ra texture
SDL_Texture* renderText(const string& text, SDL_Color color);

// Hàm khởi tạo toàn bộ tài nguyên game (ảnh, font, nhạc)
bool initResources();
