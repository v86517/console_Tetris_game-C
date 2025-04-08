#include "tetris_frontend.h"

void printRectangle(WINDOW *w, int topR, int bottomR, int leftC, int rightC) {
  mvwaddch(w, topR, leftC, ACS_ULCORNER);
  int i = leftC + 1;
  for (; i < rightC; i++) mvwaddch(w, topR, i, ACS_HLINE);
  mvwaddch(w, topR, i, ACS_URCORNER);
  for (int j = topR + 1; j < bottomR; j++) {
    mvwaddch(w, j, leftC, ACS_VLINE);
    mvwaddch(w, j, rightC, ACS_VLINE);
  }
  mvwaddch(w, bottomR, leftC, ACS_LLCORNER);
  i = leftC + 1;
  for (; i < rightC; i++) mvwaddch(w, bottomR, i, ACS_HLINE);
  mvwaddch(w, bottomR, i, ACS_LRCORNER);
}

void initColor() {
  init_color(MY_COLOR_RED_I, 1000, 0, 0);
  init_color(MY_COLOR_ORANGE_J, 1000, 450, 0);
  init_color(MY_COLOR_YELLOW_L, 1000, 1000, 0);
  init_color(MY_COLOR_PINK_O, 1000, 600, 800);
  init_color(MY_COLOR_GREEN_S, 0, 500, 0);
  init_color(MY_COLOR_BLUE_T, 0, 0, 1000);
  init_color(MY_COLOR_MAGENTA_Z, 550, 0, 550);

  init_pair(0, MY_COLOR_RED_I, MY_COLOR_RED_I);
  init_pair(1, MY_COLOR_ORANGE_J, MY_COLOR_ORANGE_J);
  init_pair(2, MY_COLOR_YELLOW_L, MY_COLOR_YELLOW_L);
  init_pair(3, MY_COLOR_PINK_O, MY_COLOR_PINK_O);
  init_pair(4, MY_COLOR_GREEN_S, MY_COLOR_GREEN_S);
  init_pair(5, MY_COLOR_BLUE_T, MY_COLOR_BLUE_T);
  init_pair(6, MY_COLOR_MAGENTA_Z, MY_COLOR_MAGENTA_Z);
  init_pair(7, COLOR_BLACK, COLOR_BLACK);
  init_pair(8, MY_COLOR_RED_I, MY_COLOR_RED_I);
}

void initWindows(WINDOW *field, WINDOW *panel) {
  box(field, 0, 0);
  box(panel, 0, 0);
  for (int i = 1; i < 13; i++) {
    mvwaddch(panel, 4, i, ACS_HLINE);
    mvwaddch(panel, 8, i, ACS_HLINE);
    mvwaddch(panel, 12, i, ACS_HLINE);
    mvwprintw(panel, 1, 2, "Highscore");
    mvwprintw(panel, 5, 4, "Score");
    mvwprintw(panel, 9, 4, "Level");
    mvwprintw(panel, 13, 4, "Next");
  }
}

// void printGame(GameInfo_t actual_game_info, game_state tetris_state,
//                WINDOW *field, WINDOW *panel) {
void printGame(GameInfo_t actual_game_info, game_state tetris_state,
               WINDOW *field, WINDOW *panel) {
  printGameInfo(actual_game_info, panel);

  if (actual_game_info.pause == TRUE)
    printPauseState(field);
  else if (tetris_state == START)
    printStartState(field);
  else if (tetris_state == MOVING && actual_game_info.pause == FALSE)
    printFieldState(actual_game_info, field);
  else if (tetris_state == GAMEOVER)
    printGameOverState(field);
}

void printGameInfo(GameInfo_t actual_game_info, WINDOW *panel) {
  mvwprintw(panel, 3, 1, "%d", actual_game_info.high_score);
  mvwprintw(panel, 7, 1, "%d", actual_game_info.score);
  mvwprintw(panel, 11, 1, "%d", actual_game_info.level);
  clearNext(panel);
  for (int r = 0; r < 4; r++) {
    for (int c = 0; c < 4; c++) {
      if (actual_game_info.next[r][c] != 0) {
        mvwaddch(panel, r + 15, c * 2 + 3,
                 FULL_CELL | COLOR_PAIR(actual_game_info.next[r][c]));
        mvwaddch(panel, r + 15, c * 2 + 4,
                 FULL_CELL | COLOR_PAIR(actual_game_info.next[r][c]));
      }
    }
  }
  wrefresh(panel);
}

void clearNext(WINDOW *panel) {
  for (int y = 15; y < 19; y++) {
    for (int x = 3; x < 11; x++) {
      mvwaddch(panel, y, x, EMPTY_CELL | COLOR_PAIR(7));
    }
  }
}

void printStartState(WINDOW *field) {
  printRectangle(field, 7, 16, 1, 20);
  wattron(field, WA_BOLD);
  mvwprintw(field, 8, 7, "NEW GAME");
  wattroff(field, WA_BOLD);
  mvwprintw(field, 13, 2, "ENTER - start game");
  mvwprintw(field, 15, 6, "ESC - exit");
  wrefresh(field);
}

void printPauseState(WINDOW *field) {
  printRectangle(field, 7, 16, 1, 20);
  for (int y = 8; y < 16; y++) {
    mvwprintw(field, y, 2, "                  ");
  }
  wattron(field, WA_BOLD);
  mvwprintw(field, 8, 8, "PAUSED");
  wattroff(field, WA_BOLD);
  mvwprintw(field, 13, 5, "p - continue");
  wrefresh(field);
}

void printGameOverState(WINDOW *field) {
  printRectangle(field, 7, 16, 1, 20);
  for (int y = 8; y < 16; y++) {
    mvwprintw(field, y, 2, "                  ");
  }
  wattron(field, WA_BOLD);
  mvwprintw(field, 8, 6, "GAME OVER");
  wattroff(field, WA_BOLD);
  mvwprintw(field, 13, 3, "ENTER - new game");
  mvwprintw(field, 15, 6, "ESC - exit");
  wrefresh(field);
}

void printFieldState(GameInfo_t actual_game_info, WINDOW *field) {
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      if (actual_game_info.field[r][c] == 0) {
        mvwaddch(field, r + 1, c * 2 + 1, EMPTY_CELL | COLOR_PAIR(7));
        mvwaddch(field, r + 1, c * 2 + 1 + 1, EMPTY_CELL | COLOR_PAIR(7));
      }
      if (actual_game_info.field[r][c] != 0) {
        mvwaddch(field, r + 1, c * 2 + 1,
                 FULL_CELL | COLOR_PAIR(actual_game_info.field[r][c]));
        mvwaddch(field, r + 1, c * 2 + 1 + 1,
                 FULL_CELL | COLOR_PAIR(actual_game_info.field[r][c]));
      }
    }
  }
  wrefresh(field);
}
