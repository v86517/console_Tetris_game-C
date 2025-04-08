#ifndef TETRIS_FRONTEND
#define TETRIS_FRONTEND

#include "../../brick_game/tetris/tetris_backend.h"

#define MY_COLOR_RED_I (12)
#define MY_COLOR_ORANGE_J (13)
#define MY_COLOR_YELLOW_L (14)
#define MY_COLOR_PINK_O (15)
#define MY_COLOR_GREEN_S (16)
#define MY_COLOR_BLUE_T (17)
#define MY_COLOR_MAGENTA_Z (18)

#define EMPTY_CELL 'E'
#define FULL_CELL 'F'

void printRectangle(WINDOW *w, int topR, int bottomR, int leftC, int rightC);
void initColor();
void initWindows(WINDOW *field, WINDOW *panel);
void printGame(GameInfo_t actual_game_info, game_state tetris_state,
               WINDOW *field, WINDOW *panel);
void printGameInfo(GameInfo_t actual_game_info, WINDOW *panel);
void clearNext(WINDOW *panel);
void printStartState(WINDOW *field);
void printPauseState(WINDOW *field);
void printGameOverState(WINDOW *field);
void printFieldState(GameInfo_t actual_game_info, WINDOW *field);

#endif