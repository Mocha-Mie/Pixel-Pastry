// resource.cpp - Xử lý khởi tạo và giải phóng tài nguyên SDL

#include "resource.h"
#include <iostream>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* candyTextures[5];
Mix_Music* bgMusic = nullptr;
TTF_Font* font = nullptr;

// Tải ảnh từ đường dẫn vào texture
SDL_Texture* loadTexture(const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cout << "Lỗi tải ảnh: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// Tạo texture từ chuỗi văn bản
SDL_Texture* renderText(const std::string& message, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, message.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// Khởi tạo cửa sổ, renderer, ảnh, font và nhạc nền
bool initResources() {
    window = SDL_CreateWindow("Sweet Match", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Không tạo được cửa sổ: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Không tạo được renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    const char* paths[5] = {
        "res/candy_pink.png",
        "res/candy_brown.png",
        "res/candy_yellow.png",
        "res/candy_green.png",
        "res/candy_blue.png"
    };

    for (int i = 0; i < 5; ++i) {
        candyTextures[i] = loadTexture(paths[i]);
        if (!candyTextures[i]) return false;
    }

    font = TTF_OpenFont("res/LoveDays-2v7Oe.ttf", 24);
    if (!font) {
        std::cout << "Lỗi font: " << TTF_GetError() << std::endl;
        return false;
    }

    bgMusic = Mix_LoadMUS("res/bg_music.mp3");
    if (bgMusic) Mix_PlayMusic(bgMusic, -1);

    return true;
}

// Giải phóng toàn bộ tài nguyên khi thoát
void cleanupResources() {
    for (auto& tex : candyTextures)
        SDL_DestroyTexture(tex);
    Mix_FreeMusic(bgMusic);
    TTF_CloseFont(font);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}