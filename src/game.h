#pragma once
#include <SDL.h>

// Khai báo lưới game
extern int grid[8][8];
extern int score;
extern int movesLeft;
extern bool candySelected;
extern int selectedX, selectedY;

// Sự kiện SDL toàn cục
extern SDL_Event e;

// Các hàm xử lý chính trong game
void initGrid();                                 // Tạo lưới kẹo ban đầu
void renderGrid();                               // Vẽ lưới kẹo
bool isAdjacent(int x1, int y1, int x2, int y2); // Kiểm tra ô kề
void swap(int x1, int y1, int x2, int y2);       // Hoán đổi kẹo
bool findAndClearMatches();                      // Tìm match và cộng điểm
void collapseGrid();                             // Làm kẹo rơi và sinh kẹo mới
void handleMouse(SDL_Event& e);                  // Xử lý click chuột
void renderUI(int score, int movesLeft);         // Vẽ giao diện
void showGameOverScreenWithButtons(SDL_Event &e, bool &playing);
