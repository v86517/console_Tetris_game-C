#include "../../gui/cli/tetris_frontend.h"
#include "tetris_backend.h"

extern GameInfo_t game_info;

int main() {
  initscr();             //
  noecho();              //
  curs_set(0);           //
  keypad(stdscr, TRUE);  //
  start_color();
  srand(time(NULL));
  initColor();
  gameLoop();
  endwin();
  return 0;
}

void gameLoop() {
  Block_t current_block;
  UserAction_t action;
  game_state tetris_state = START;

  initState();

  WINDOW *field, *panel;
  field = newwin(ROWS + 2, COLS * 2 + 2, 0, 0);
  panel = newwin(22, 14, 0, COLS * 2 + 2);
  initWindows(field, panel);

  nodelay(stdscr, true);
  bool hold = false;

  struct timeval start_timer, finish_timer;
  gettimeofday(&start_timer, NULL);

  while (tetris_state != EXIT_STATE) {
    action = get_signal(getch(), &hold);

    if (action == Pause && tetris_state == MOVING) {
      game_info.pause = game_info.pause == TRUE ? FALSE : TRUE;
    }

    if (game_info.pause == FALSE) {
      userInput(&current_block, action, &tetris_state);

      suseconds_t timer = game_info.speed;

      gettimeofday(&finish_timer, NULL);
      if (isTimerOver(start_timer, finish_timer, timer)) {
        userInput(&current_block, action = Down, &tetris_state);
        gettimeofday(&start_timer, NULL);
      }
    }
    GameInfo_t actual_game_info = updateCurrentState();

    //  printGame(actual_game_info, tetris_state, field, panel);
    printGame(actual_game_info, tetris_state, field, panel);
  }
  freeMemory(&current_block);  //////нужен ли &current_block ????
  clear();
  delwin(field);
  delwin(panel);
}
