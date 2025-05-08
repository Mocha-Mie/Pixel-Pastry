#include "resource.h"
#include <iostream>

using namespace std;

// Định nghĩa các biến tài nguyên toàn cục
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* candyTextures[NUM_CANDIES];
TTF_Font* font = nullptr;
Mix_Music* bgMusic = nullptr;

// Hàm load ảnh PNG thành texture
SDL_Texture* loadTexture(const string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        cout << "IMG_Load Error: " << IMG_GetError() << endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// Hàm tạo texture từ văn bản (dùng để hiển thị chữ điểm số, lượt...)
SDL_Texture* renderText(const string& text, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) return nullptr;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// Hàm khởi tạo toàn bộ tài nguyên game: ảnh, nhạc, font
bool initResources() {
    // Load 5 ảnh kẹo
    candyTextures[0] = loadTexture("res/candy_pink.png");
    candyTextures[1] = loadTexture("res/candy_brown.png");
    candyTextures[2] = loadTexture("res/candy_yellow.png");
    candyTextures[3] = loadTexture("res/candy_green.png");
    candyTextures[4] = loadTexture("res/candy_blue.png");

    // Kiểm tra load ảnh thành công
    for (int i = 0; i < NUM_CANDIES; ++i) {
        if (!candyTextures[i]) return false;
    }

    // Load font
    font = TTF_OpenFont("res/LoveDays-2v7Oe.ttf", 24);
    if (!font) {
        cout << "Font Error: " << TTF_GetError() << endl;
        return false;
    }

    // Load nhạc nền
    bgMusic = Mix_LoadMUS("res/bg_music.mp3");
    if (!bgMusic) {
        cout << "Music Error: " << Mix_GetError() << endl;
        return false;
    }

    // Phát nhạc
    Mix_PlayMusic(bgMusic, -1);

    return true;
}
