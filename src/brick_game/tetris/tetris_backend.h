#ifndef TETRIS_BACKEND
#define TETRIS_BACKEND

#include <ncurses.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define ROWS 20
#define COLS 10
#define NEXT_WIDTH 4
#define EMPTY 0
#define NUM_BLOCKS 7
#define NUM_ROTS 4

#define SPACE_KEY 32
#define ESCAPE_KEY 27
#define ENTER_KEY 10

#define LINE_SCORE 100
#define LAST_LEVEL 10
#define SCORE_LEVELUP 600
#define INIT_SPEED 1000000
#define SPEED_LEVELUP 100000

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action,
  NOSIG,
} UserAction_t;

typedef enum {
  START = 0,
  SPAWN,
  MOVING,
  REACH,
  GAMEOVER,
  EXIT_STATE
} game_state;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef struct {
  int **block_array;
  int block_width;
  int block_row;
  int block_col;
} Block_t;

void gameLoop();
int getRandBlockNum();
int getRandBlockRot();
void initGameInfo();
GameInfo_t updateCurrentState();
int isTimerOver(struct timeval start_timer, struct timeval finish_timer,
                suseconds_t timer);
void userInput(Block_t *current_block, UserAction_t action,
               game_state *tetris_state);
UserAction_t get_signal(int user_input, bool *hold);
int moveBlockDown(Block_t *current_block, Block_t *temp);
void processFullRows();
void writeLevelAndSpeed();
void writeHighScore();
int checkCollide(Block_t block);
void rotateBlock(Block_t *block);
Block_t copyBlock(Block_t *current_block);
void delBlock(Block_t *block);
void delBlockFromField(Block_t *current_block);
void initMemory();
void freeMemory();
void initState();
void addBlockToField(Block_t *current_block);
int getBlockWidth(int block_num);
int getFigureNum();
void initNextBlock();
void rotateNextBlock(int block_num, int block_rot, int w);
void delUpEmptyLines(int w);
void initBlockInfo(Block_t *current_block);
void initHighScoreFromFile();
void writeHighScoreToFile();

#endif
