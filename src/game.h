// game.h - Khai báo các hàm và biến toàn cục của game logic
#pragma once

void initGrid();
void renderGrid();
bool findAndClearMatches();
void collapseGrid();
bool isAdjacent(int x1, int y1, int x2, int y2);
void swap(int x1, int y1, int x2, int y2);

// Biến toàn cục
extern int score;
extern int grid[8][8];
extern bool candySelected;
extern int selectedX, selectedY;
