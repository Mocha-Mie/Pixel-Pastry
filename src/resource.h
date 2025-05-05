// resource.h - Khai báo tài nguyên SDL: ảnh, nhạc, font, cửa sổ

#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>

// Biến toàn cục dùng toàn chương trình
extern SDL_Window* window;              // Cửa sổ game
extern SDL_Renderer* renderer;          // Renderer để vẽ
extern SDL_Texture* candyTextures[5];   // Mảng ảnh kẹo
extern Mix_Music* bgMusic;              // Nhạc nền
extern TTF_Font* font;                  // Font chữ

// Tải ảnh từ đường dẫn thành texture
SDL_Texture* loadTexture(const std::string& path);

// Tạo texture từ văn bản
SDL_Texture* renderText(const std::string& message, SDL_Color color);

// Khởi tạo tài nguyên: cửa sổ, renderer, ảnh, font, nhạc
bool initResources();

// Giải phóng tài nguyên khi thoát
void cleanupResources();