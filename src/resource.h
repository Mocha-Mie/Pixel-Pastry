// resource.h - Quản lý các tài nguyên SDL: window, renderer, ảnh, font, nhạc
#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>

// Biến toàn cục SDL
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* candyTextures[5];
extern Mix_Music* bgMusic;
extern TTF_Font* font;

SDL_Texture* loadTexture(const std::string& path);
SDL_Texture* renderText(const std::string& message, SDL_Color color);
bool initResources();
void cleanupResources();
