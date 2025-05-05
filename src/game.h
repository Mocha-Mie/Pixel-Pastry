// game.h - Khai báo các hàm xử lý logic trò chơi

#pragma once

// Khởi tạo lưới kẹo ngẫu nhiên
void initGrid();

// Vẽ toàn bộ lưới kẹo lên màn hình
void renderGrid();

// Kiểm tra và đánh dấu các ô match
bool findAndClearMatches();

// Làm rơi kẹo xuống và sinh kẹo mới
void collapseGrid();

// Kiểm tra hai ô có kề nhau không
bool isAdjacent(int x1, int y1, int x2, int y2);

// Hoán đổi vị trí 2 ô
void swap(int x1, int y1, int x2, int y2);

// Biến toàn cục
extern int grid[8][8];
extern int score;
extern int movesLeft;
extern bool candySelected;
extern int selectedX, selectedY;
extern bool showMatches[8][8];