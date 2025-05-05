// resource.cpp - Quản lý window, renderer, font, nhạc, ảnh kẹo
#include "resource.h"
#include <iostream>

// Biến toàn cục SDL
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* candyTextures[5];
Mix_Music* bgMusic = nullptr;
TTF_Font* font = nullptr;

// Load ảnh từ file thành texture
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

// Tạo texture từ chữ (hiển thị điểm)
SDL_Texture* renderText(const std::string& message, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, message.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// Khởi tạo tất cả tài nguyên SDL (ảnh, font, nhạc)
bool initResources() {
    window = SDL_CreateWindow("Candy Crush Clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "CreateWindow Error: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "CreateRenderer Error: " << SDL_GetError() << std::endl;
        return false;
    }

    candyTextures[0] = loadTexture("res/candy_pink.png");
    candyTextures[1] = loadTexture("res/candy_brown.png");
    candyTextures[2] = loadTexture("res/candy_yellow.png");
    candyTextures[3] = loadTexture("res/candy_green.png");
    candyTextures[4] = loadTexture("res/candy_blue.png");

    for (int i = 0; i < 5; ++i)
        if (!candyTextures[i]) return false;

    font = TTF_OpenFont("res/LoveDays-2v7Oe.ttf", 24);
    if (!font) {
        std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        return false;
    }

    bgMusic = Mix_LoadMUS("res/bg_music.mp3");
    if (bgMusic) Mix_PlayMusic(bgMusic, -1);

    return true;
}

// Giải phóng tài nguyên
void cleanupResources() {
    for (int i = 0; i < 5; ++i)
        SDL_DestroyTexture(candyTextures[i]);
    Mix_FreeMusic(bgMusic);
    TTF_CloseFont(font);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
