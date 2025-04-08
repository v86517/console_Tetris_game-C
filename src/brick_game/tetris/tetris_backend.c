#include "tetris_backend.h"

GameInfo_t game_info;

int getRandBlockNum() { return rand() % NUM_BLOCKS; }

int getRandBlockRot() { return rand() % NUM_ROTS; }

void initGameInfo() {
  initNextBlock();
  game_info.score = 0;
  initHighScoreFromFile();
  game_info.level = 1;
  game_info.speed = INIT_SPEED;
  game_info.pause = 0;
}

GameInfo_t updateCurrentState() { return game_info; }

int isTimerOver(struct timeval start_timer, struct timeval finish_timer,
                suseconds_t timer) {
  suseconds_t finish_time =
      finish_timer.tv_sec * 1000000 + finish_timer.tv_usec;
  suseconds_t start_time = start_timer.tv_sec * 1000000 + start_timer.tv_usec;
  int res = finish_time - start_time > timer ? TRUE : FALSE;
  return res;
}

void userInput(Block_t *current_block, UserAction_t action,
               game_state *tetris_state) {
  if (*tetris_state == GAMEOVER) {
    switch (action) {
      case Start:
        initState();
        *tetris_state = SPAWN;
        break;
      case Terminate:
        *tetris_state = EXIT_STATE;
        break;
      default:
        break;
    }
  }

  if (*tetris_state == START) {
    switch (action) {
      case Start:
        *tetris_state = SPAWN;
        break;
      case Terminate:
        *tetris_state = EXIT_STATE;
        break;
      default:
        break;
    }
  }

  if (*tetris_state == SPAWN) {
    initBlockInfo(current_block);
    if (checkCollide(*current_block) == TRUE) {
      addBlockToField(current_block);
      initNextBlock();
      *tetris_state = MOVING;
    } else {
      delBlock(current_block);
      *tetris_state = GAMEOVER;
    }
  }

  if (*tetris_state == MOVING) {
    Block_t temp = copyBlock(current_block);
    switch (action) {
      case Action:
        delBlockFromField(current_block);
        rotateBlock(&temp);
        if (checkCollide(temp) == TRUE) rotateBlock(current_block);
        addBlockToField(current_block);
        break;
      case Down:
        if (!moveBlockDown(current_block, &temp)) *tetris_state = REACH;
        break;
      case Left:
        delBlockFromField(current_block);
        temp.block_col--;
        if (checkCollide(temp) == TRUE) current_block->block_col--;
        addBlockToField(current_block);
        break;
      case Right:
        delBlockFromField(current_block);
        temp.block_col++;
        if (checkCollide(temp) == TRUE) current_block->block_col++;
        addBlockToField(current_block);
        break;
      case Terminate:
        delBlock(current_block);
        *tetris_state = GAMEOVER;
        break;
      default:
        break;
    }
    delBlock(&temp);
  }

  if (*tetris_state == REACH) {
    processFullRows();
    delBlock(current_block);
    if (game_info.level > LAST_LEVEL)
      *tetris_state = GAMEOVER;
    else
      *tetris_state = SPAWN;
  }
}

UserAction_t get_signal(int user_input, bool *hold) {
  UserAction_t rc = NOSIG;

  if (user_input == ENTER_KEY)
    rc = Start;
  else if (user_input == KEY_UP)
    rc = Up;
  else if (user_input == KEY_DOWN)
    rc = Down;
  else if (user_input == KEY_LEFT)
    rc = Left;
  else if (user_input == KEY_RIGHT)
    rc = Right;
  else if (user_input == ESCAPE_KEY)
    rc = Terminate;
  else if (user_input == SPACE_KEY)
    rc = Action;
  else if (user_input == 'p')
    rc = Pause;
  else if (user_input == 'd')
    *hold = true;

  return rc;
}

int moveBlockDown(Block_t *current_block, Block_t *temp) {
  int res = TRUE;
  delBlockFromField(current_block);
  temp->block_row++;
  if (checkCollide(*temp) == TRUE) {
    current_block->block_row++;
  } else {
    res = FALSE;
  }
  addBlockToField(current_block);
  return res;
}

void processFullRows() {
  int score_count = 0;
  for (int r = 0; r < ROWS; r++) {
    int flag = 1;
    for (int c = 0; c < COLS; c++) {
      if (game_info.field[r][c] == EMPTY) {
        flag = 0;
      }
    }
    if (flag == 1) {
      for (int k = r; k > 0; k--)
        for (int l = 0; l < COLS; l++)
          game_info.field[k][l] = game_info.field[k - 1][l];
      r--;
      score_count = score_count * 2 + LINE_SCORE;
    }
  }
  game_info.score += score_count;
  writeLevelAndSpeed();
  writeHighScore();
}

void writeLevelAndSpeed() {
  game_info.level = game_info.score / SCORE_LEVELUP + 1;
  game_info.speed = INIT_SPEED - (SPEED_LEVELUP * (game_info.level - 1));
}

void writeHighScore() {
  if (game_info.score > game_info.high_score) {
    game_info.high_score = game_info.score;
    writeHighScoreToFile();
  }
}

int checkCollide(Block_t block) {
  for (int r = 0; r < block.block_width; r++) {
    for (int c = 0; c < block.block_width; c++) {
      if ((block.block_col + c < 0 || block.block_col + c >= COLS ||
           block.block_row + r >= ROWS ||
           (game_info.field[block.block_row + r][block.block_col + c] != 0)) &&
          (block.block_array[r][c] != 0)) {
        return FALSE;
      }
    }
  }
  return TRUE;
}

void rotateBlock(Block_t *block) {
  int width = block->block_width;
  int temp[width][width];

  for (int r = 0; r < width; r++) {
    for (int c = 0; c < width; c++) {
      temp[r][c] = block->block_array[r][c];
    }
  }
  for (int c = 0; c < width; c++) {
    for (int r = width - 1; r >= 0; r--) {
      block->block_array[c][width - 1 - r] = temp[r][c];
    }
  }
}

Block_t copyBlock(Block_t *current_block) {
  Block_t copy;
  copy.block_array = (int **)malloc(sizeof(int *) * current_block->block_width);

  for (int r = 0; r < current_block->block_width; r++) {
    copy.block_array[r] =
        (int *)calloc(sizeof(int), current_block->block_width);
    for (int c = 0; c < current_block->block_width; c++) {
      copy.block_array[r][c] = current_block->block_array[r][c];
    }
  }
  copy.block_width = current_block->block_width;
  copy.block_row = current_block->block_row;
  copy.block_col = current_block->block_col;
  return copy;
}

void delBlock(Block_t *block) {
  for (int r = 0; r < block->block_width; r++) {
    free(block->block_array[r]);
  }
  free(block->block_array);
}

void delBlockFromField(Block_t *current_block) {
  for (int r = 0; r < current_block->block_width; r++) {
    for (int c = 0; c < current_block->block_width; c++) {
      if (current_block->block_array[r][c] != 0) {
        game_info
            .field[r + current_block->block_row][c + current_block->block_col] =
            EMPTY;
      }
    }
  }
}

void initMemory() {
  if (game_info.field != NULL) {
    freeMemory();
  }
  game_info.field = (int **)malloc(sizeof(int *) * ROWS);
  for (int r = 0; r < ROWS; r++) {
    game_info.field[r] = (int *)calloc(sizeof(int), COLS);
  }
  game_info.next = (int **)malloc(sizeof(int *) * NEXT_WIDTH);
  for (int r = 0; r < NEXT_WIDTH; r++) {
    game_info.next[r] = (int *)calloc(sizeof(int), NEXT_WIDTH);
  }
}

void freeMemory() {
  for (int i = 0; i < NEXT_WIDTH; i++) free(game_info.next[i]);
  free(game_info.next);
  for (int i = 0; i < ROWS; i++) free(game_info.field[i]);
  free(game_info.field);
}

void initState() {
  initMemory();
  initGameInfo();
}

void addBlockToField(Block_t *current_block) {
  for (int r = 0; r < current_block->block_width; r++) {
    for (int c = 0; c < current_block->block_width; c++) {
      if (current_block->block_array[r][c] != 0) {
        game_info
            .field[r + current_block->block_row][c + current_block->block_col] =
            current_block->block_array[r][c];
      }
    }
  }
}

int getBlockWidth(int block_num) {
  int width = 0;
  switch (block_num) {
    case 0:
      width = 4;
      break;
    case 3:
      width = 2;
      break;
    default:
      width = 3;
      break;
  }
  return width;
}

int getFigureNum() {
  int figureNum = 0;
  for (int r = 0; r < NEXT_WIDTH; r++) {
    for (int c = 0; c < NEXT_WIDTH; c++) {
      if (game_info.next[r][c] != 0) {
        figureNum = game_info.next[r][c];
        break;
      }
    }
    break;
  }
  if (figureNum == 8) figureNum = 0;
  return figureNum;
}

void initNextBlock() {
  int block_num = getRandBlockNum();
  int width = getBlockWidth(block_num);
  int block_rot = getRandBlockRot();
  for (int r = 0; r < NEXT_WIDTH; r++) {
    for (int c = 0; c < NEXT_WIDTH; c++) {
      game_info.next[r][c] = 0;
    }
  }
  rotateNextBlock(block_num, block_rot, width);
}

void rotateNextBlock(int block_num, int block_rot, int w) {
  int blocks[7][16] = {{0, 8, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0},   // I
                       {0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},   // J
                       {2, 2, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0},   // L
                       {3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // O
                       {4, 0, 0, 4, 4, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0},   // S
                       {0, 5, 0, 0, 5, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0},   // T
                       {0, 6, 0, 6, 6, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0}};  // Z

  for (int r = 0; r < w; r++) {
    for (int c = 0; c < w; c++) {
      switch (block_rot) {
        case 0:
          game_info.next[r][c] = blocks[block_num][w * r + c];
          break;
        case 1:
          game_info.next[r][c] = blocks[block_num][w * (w - 1 - c) + r];
          break;
        case 2:
          if (block_num == 0 || block_num == 3 || block_num == 4 ||
              block_num == 6) {
            game_info.next[r][c] = blocks[block_num][w * r + c];
          } else {
            game_info.next[r][c] = blocks[block_num][w * (w - r) - 1 - c];
          }
          break;
        case 3:
          if (block_num == 0 || block_num == 3 || block_num == 4 ||
              block_num == 6) {
            game_info.next[r][c] = blocks[block_num][w * (w - 1 - c) + r];
          } else {
            game_info.next[r][c] = blocks[block_num][w - 1 + w * c - r];
          }
          break;
      }
    }
  }
  delUpEmptyLines(w);
}

void delUpEmptyLines(int w) {
  int flag = 0;
  for (int c = 0; c < w; c++) {
    if (game_info.next[0][c] != 0) flag = 1;
  }
  if (flag == 0) {
    int *temp = game_info.next[0];
    for (int r = 0; r < w - 1; r++) {
      game_info.next[r] = game_info.next[r + 1];
    }
    game_info.next[w - 1] = temp;
  }
}

void initBlockInfo(Block_t *current_block) {
  int width = getBlockWidth(getFigureNum());
  current_block->block_array = (int **)malloc(sizeof(int *) * width);
  for (int r = 0; r < width; r++) {
    current_block->block_array[r] = (int *)calloc(sizeof(int), width);
    for (int c = 0; c < width; c++) {
      current_block->block_array[r][c] = game_info.next[r][c];
    }
  }
  current_block->block_width = width;
  current_block->block_row = 0;
  current_block->block_col = 4;
}

void initHighScoreFromFile() {
  FILE *file = fopen("high_score.txt", "r");
  if (file) {
    fscanf(file, "%d", &(game_info.high_score));
    fclose(file);
  } else {
    game_info.high_score = 0;
  }
}

void writeHighScoreToFile() {
  FILE *file = fopen("high_score.txt", "w");
  if (file) {
    fprintf(file, "%d", game_info.high_score);
    fclose(file);
  }
}
