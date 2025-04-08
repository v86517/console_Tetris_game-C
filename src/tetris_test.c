#include <check.h>

#include "brick_game/tetris/tetris_backend.h"

extern GameInfo_t game_info;

START_TEST(initMemory1) {
  initMemory();
  initMemory();
  for (int i = 0; i < ROWS; ++i)
    for (int j = 0; j < COLS; ++j) ck_assert_int_eq(game_info.field[i][j], 0);

  for (int i = 0; i < NEXT_WIDTH; ++i)
    for (int j = 0; j < NEXT_WIDTH; ++j)
      ck_assert_int_eq(game_info.next[i][j], 0);
  freeMemory();
}
END_TEST

START_TEST(rotateNextBlock1) {
  int expected_res[4][4] = {
      {0, 8, 0, 0}, {0, 8, 0, 0}, {0, 8, 0, 0}, {0, 8, 0, 0}};
  int block_num = 0, block_rot = 0, width = 4;
  initMemory();
  rotateNextBlock(block_num, block_rot, width);

  for (int i = 0; i < width; ++i)
    for (int j = 0; j < width; ++j)
      ck_assert_int_eq(game_info.next[i][j], expected_res[i][j]);
  freeMemory();
}
END_TEST

START_TEST(rotateNextBlock2) {
  int expected_res[4][4] = {
      {8, 8, 8, 8}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  int block_num = 0, block_rot = 1, width = 4;
  initMemory();
  rotateNextBlock(block_num, block_rot, width);

  for (int i = 0; i < width; ++i)
    for (int j = 0; j < width; ++j)
      ck_assert_int_eq(game_info.next[i][j], expected_res[i][j]);
  freeMemory();
}
END_TEST

START_TEST(rotateNextBlock3) {
  int expected_res[4][4] = {
      {0, 8, 0, 0}, {0, 8, 0, 0}, {0, 8, 0, 0}, {0, 8, 0, 0}};
  int block_num = 0, block_rot = 2, width = 4;
  initMemory();
  rotateNextBlock(block_num, block_rot, width);

  for (int i = 0; i < width; ++i)
    for (int j = 0; j < width; ++j)
      ck_assert_int_eq(game_info.next[i][j], expected_res[i][j]);
  freeMemory();
}
END_TEST

START_TEST(rotateNextBlock4) {
  int expected_res[3][3] = {{0, 5, 0}, {5, 5, 0}, {0, 5, 0}};
  int block_num = 5, block_rot = 2, width = 3;
  initMemory();
  rotateNextBlock(block_num, block_rot, width);

  for (int i = 0; i < width; ++i)
    for (int j = 0; j < width; ++j)
      ck_assert_int_eq(game_info.next[i][j], expected_res[i][j]);
  freeMemory();
}
END_TEST

START_TEST(rotateNextBlock5) {
  int expected_res[3][3] = {{0, 5, 0}, {5, 5, 5}, {0, 0, 0}};
  int block_num = 5, block_rot = 3, width = 3;
  initMemory();
  rotateNextBlock(block_num, block_rot, width);

  for (int i = 0; i < width; ++i)
    for (int j = 0; j < width; ++j)
      ck_assert_int_eq(game_info.next[i][j], expected_res[i][j]);
  freeMemory();
}
END_TEST

START_TEST(initBlockInfo1) {
  int expected_res[4][4] = {
      {8, 8, 8, 8}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  int block_num = 0, block_rot = 3, width = 4;
  initMemory();
  rotateNextBlock(block_num, block_rot, width);
  Block_t current_block;
  initBlockInfo(&current_block);
  for (int i = 0; i < width; ++i)
    for (int j = 0; j < width; ++j)
      ck_assert_int_eq(current_block.block_array[i][j], expected_res[i][j]);
  ck_assert_int_eq(current_block.block_width, getBlockWidth(getFigureNum()));
  ck_assert_int_eq(current_block.block_row, 0);
  ck_assert_int_eq(current_block.block_col, 4);

  freeMemory();
  delBlock(&current_block);
}
END_TEST

START_TEST(initBlockInfo2) {
  int expected_res[2][2] = {{3, 3}, {3, 3}};
  int block_num = 3, block_rot = 3, width = 2;
  initMemory();
  rotateNextBlock(block_num, block_rot, width);
  Block_t current_block;
  initBlockInfo(&current_block);
  for (int i = 0; i < width; ++i)
    for (int j = 0; j < width; ++j)
      ck_assert_int_eq(current_block.block_array[i][j], expected_res[i][j]);
  ck_assert_int_eq(current_block.block_width, getBlockWidth(getFigureNum()));

  freeMemory();
  delBlock(&current_block);
}
END_TEST

START_TEST(initBlockInfo3) {
  int expected_res[3][3] = {{0, 5, 0}, {0, 5, 5}, {0, 5, 0}};
  int block_num = 5, block_rot = 0, width = 3;
  initMemory();
  rotateNextBlock(block_num, block_rot, width);
  Block_t current_block;
  initBlockInfo(&current_block);
  for (int i = 0; i < width; ++i)
    for (int j = 0; j < width; ++j)
      ck_assert_int_eq(current_block.block_array[i][j], expected_res[i][j]);
  ck_assert_int_eq(current_block.block_width, getBlockWidth(getFigureNum()));

  freeMemory();
  delBlock(&current_block);
}
END_TEST

START_TEST(addBlockToField1) {
  int expected_res[ROWS][COLS] = {0};
  int block_num = 0, block_rot = 1, width = 4;
  initMemory();
  rotateNextBlock(block_num, block_rot, width);
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < width; j++) {
      expected_res[i][j + 4] = game_info.next[i][j];
    }
  }

  Block_t current_block;
  initBlockInfo(&current_block);
  addBlockToField(&current_block);
  for (int i = 0; i < ROWS; ++i)
    for (int j = 0; j < COLS; ++j)
      ck_assert_int_eq(game_info.field[i][j], expected_res[i][j]);

  freeMemory();
  delBlock(&current_block);
}
END_TEST

START_TEST(delBlockFromField1) {
  int expected_res[ROWS][COLS] = {0};
  int block_num = 0, block_rot = 1, width = 4;
  initMemory();
  rotateNextBlock(block_num, block_rot, width);

  Block_t current_block;
  initBlockInfo(&current_block);
  addBlockToField(&current_block);
  delBlockFromField(&current_block);
  updateCurrentState();

  for (int i = 0; i < ROWS; ++i)
    for (int j = 0; j < COLS; ++j)
      ck_assert_int_eq(game_info.field[i][j], expected_res[i][j]);

  freeMemory();
  delBlock(&current_block);
}
END_TEST

START_TEST(rotateBlock1) {
  int expected_res[4][4] = {
      {0, 0, 0, 0}, {8, 8, 8, 8}, {0, 0, 0, 0}, {0, 0, 0, 0}};

  int block_num = 0, block_rot = 0, width = 4;
  initMemory();
  rotateNextBlock(block_num, block_rot, width);

  Block_t current_block;
  initBlockInfo(&current_block);
  rotateBlock(&current_block);
  for (int i = 0; i < width; ++i)
    for (int j = 0; j < width; ++j)
      ck_assert_int_eq(current_block.block_array[i][j], expected_res[i][j]);

  freeMemory();
  delBlock(&current_block);
}
END_TEST

START_TEST(moveBlockDown1) {
  int expected_res = 1;
  int block_num = 0, block_rot = 1, width = 4;
  initMemory();
  rotateNextBlock(block_num, block_rot, width);

  Block_t current_block;
  initBlockInfo(&current_block);
  Block_t temp;
  temp = copyBlock(&current_block);
  ck_assert_int_eq(moveBlockDown(&current_block, &temp), expected_res);
  ck_assert_int_eq(current_block.block_row, expected_res);

  freeMemory();
  delBlock(&current_block);
  delBlock(&temp);
}
END_TEST

START_TEST(moveBlockDown2) {
  int expected_res = 0;
  int block_num = 0, block_rot = 1, width = 4;
  initMemory();
  rotateNextBlock(block_num, block_rot, width);
  game_info.field[1][6] = 1;
  Block_t current_block;
  initBlockInfo(&current_block);
  Block_t temp;
  temp = copyBlock(&current_block);
  ck_assert_int_eq(moveBlockDown(&current_block, &temp), expected_res);
  ck_assert_int_eq(current_block.block_row, expected_res);

  freeMemory();
  delBlock(&current_block);
  delBlock(&temp);
}
END_TEST

START_TEST(initNextBlock1) {
  int block_num = -1;
  initMemory();
  initNextBlock();
  for (int i = 0; i < NEXT_WIDTH; i++) {
    if (game_info.next[0][i] != 0) block_num = game_info.next[0][i];
  }
  ck_assert_int_eq(block_num, 0 || 1 || 2 || 3 || 4 || 5 || 6);

  freeMemory();
}
END_TEST

START_TEST(processFullRows1) {
  initMemory();
  initGameInfo();
  for (int i = 0; i < COLS; i++) game_info.field[10][i] = 1;
  processFullRows();
  ck_assert_int_eq(game_info.score, 100);

  freeMemory();
}
END_TEST

START_TEST(get_signal1) {
  UserAction_t result = Action;
  get_signal(SPACE_KEY, NULL);
  ck_assert_int_eq(result, Action);
}
END_TEST

START_TEST(get_signal2) {
  UserAction_t result = Pause;
  get_signal('p', NULL);
  ck_assert_int_eq(result, Pause);
}
END_TEST

START_TEST(userInput1) {
  game_state tetris_state = GAMEOVER;
  int block_num = 0, block_rot = 1, width = 4;

  initMemory();
  rotateNextBlock(block_num, block_rot, width);
  Block_t current_block;
  initBlockInfo(&current_block);

  userInput(&current_block, get_signal(ENTER_KEY, NULL), &tetris_state);

  ck_assert_int_eq(tetris_state, MOVING);

  freeMemory();
  delBlock(&current_block);
}
END_TEST

START_TEST(userInput2) {
  game_state tetris_state = GAMEOVER;
  int block_num = 0, block_rot = 1, width = 4;

  initMemory();
  rotateNextBlock(block_num, block_rot, width);
  Block_t current_block;
  initBlockInfo(&current_block);

  userInput(&current_block, get_signal(ESCAPE_KEY, NULL), &tetris_state);

  ck_assert_int_eq(tetris_state, EXIT_STATE);

  freeMemory();
  delBlock(&current_block);
}
END_TEST

START_TEST(userInput3) {
  game_state tetris_state = GAMEOVER;
  int block_num = 0, block_rot = 1, width = 4;

  initMemory();
  rotateNextBlock(block_num, block_rot, width);
  Block_t current_block;
  initBlockInfo(&current_block);

  userInput(&current_block, get_signal(KEY_UP, NULL), &tetris_state);

  ck_assert_int_eq(tetris_state, GAMEOVER);

  freeMemory();
  delBlock(&current_block);
}
END_TEST

START_TEST(userInput4) {
  game_state tetris_state = START;
  int block_num = 0, block_rot = 1, width = 4;

  initMemory();
  rotateNextBlock(block_num, block_rot, width);
  Block_t current_block;
  initBlockInfo(&current_block);

  userInput(&current_block, get_signal(ENTER_KEY, NULL), &tetris_state);

  ck_assert_int_eq(tetris_state, MOVING);

  freeMemory();
  delBlock(&current_block);
}
END_TEST

START_TEST(userInput5) {
  game_state tetris_state = START;
  int block_num = 0, block_rot = 1, width = 4;

  initMemory();
  rotateNextBlock(block_num, block_rot, width);
  Block_t current_block;
  initBlockInfo(&current_block);

  userInput(&current_block, get_signal(ESCAPE_KEY, NULL), &tetris_state);

  ck_assert_int_eq(tetris_state, EXIT_STATE);

  freeMemory();
  delBlock(&current_block);
}
END_TEST

START_TEST(userInput6) {
  game_state tetris_state = START;
  int block_num = 0, block_rot = 1, width = 4;

  initMemory();
  rotateNextBlock(block_num, block_rot, width);
  Block_t current_block;
  initBlockInfo(&current_block);

  userInput(&current_block, get_signal(KEY_UP, NULL), &tetris_state);

  ck_assert_int_eq(tetris_state, START);

  freeMemory();
  delBlock(&current_block);
}
END_TEST

START_TEST(userInput7) {
  game_state tetris_state = MOVING;
  int block_num = 0, block_rot = 1, width = 4;

  initMemory();
  rotateNextBlock(block_num, block_rot, width);
  Block_t current_block;
  initBlockInfo(&current_block);

  userInput(&current_block, get_signal(KEY_LEFT, NULL), &tetris_state);

  ck_assert_int_eq(current_block.block_col, 3);

  freeMemory();
  delBlock(&current_block);
}
END_TEST

START_TEST(userInput8) {
  game_state tetris_state = MOVING;
  int block_num = 0, block_rot = 1, width = 4;

  initMemory();
  rotateNextBlock(block_num, block_rot, width);
  Block_t current_block;
  initBlockInfo(&current_block);

  userInput(&current_block, get_signal(KEY_RIGHT, NULL), &tetris_state);

  ck_assert_int_eq(current_block.block_col, 5);

  freeMemory();
  delBlock(&current_block);
}
END_TEST

START_TEST(userInput9) {
  game_state tetris_state = MOVING;
  int block_num = 0, block_rot = 1, width = 4;

  initMemory();
  rotateNextBlock(block_num, block_rot, width);
  Block_t current_block;
  initBlockInfo(&current_block);

  userInput(&current_block, get_signal(KEY_DOWN, NULL), &tetris_state);

  ck_assert_int_eq(current_block.block_row, 1);

  freeMemory();
  delBlock(&current_block);
}
END_TEST

START_TEST(userInput10) {
  game_state tetris_state = MOVING;
  int block_num = 0, block_rot = 1, width = 4;

  initMemory();
  rotateNextBlock(block_num, block_rot, width);
  Block_t current_block;
  initBlockInfo(&current_block);

  userInput(&current_block, get_signal(ESCAPE_KEY, NULL), &tetris_state);

  ck_assert_int_eq(tetris_state, GAMEOVER);

  freeMemory();
}
END_TEST

////////////////////////////////////////////////////////////////////////////
Suite *initMemory_suite(void) {
  Suite *s = suite_create("************initMemory**********");
  TCase *tc = tcase_create("tetris");

  tcase_add_test(tc, initMemory1);

  suite_add_tcase(s, tc);

  return s;
}

Suite *rotateNextBlock_suite(void) {
  Suite *s = suite_create("************rotateNextBlock**********");
  TCase *tc = tcase_create("tetris");

  tcase_add_test(tc, rotateNextBlock1);
  tcase_add_test(tc, rotateNextBlock2);
  tcase_add_test(tc, rotateNextBlock3);
  tcase_add_test(tc, rotateNextBlock4);
  tcase_add_test(tc, rotateNextBlock5);

  suite_add_tcase(s, tc);

  return s;
}

Suite *initBlockInfo_suite(void) {
  Suite *s = suite_create("************initBlockInfo**********");
  TCase *tc = tcase_create("tetris");

  tcase_add_test(tc, initBlockInfo1);
  tcase_add_test(tc, initBlockInfo2);
  tcase_add_test(tc, initBlockInfo3);

  suite_add_tcase(s, tc);

  return s;
}

Suite *addBlockToField_suite(void) {
  Suite *s = suite_create("************addBlockToField**********");
  TCase *tc = tcase_create("tetris");

  tcase_add_test(tc, addBlockToField1);

  suite_add_tcase(s, tc);

  return s;
}

Suite *delBlockFromField_suite(void) {
  Suite *s = suite_create("************delBlockFromField**********");
  TCase *tc = tcase_create("tetris");

  tcase_add_test(tc, delBlockFromField1);

  suite_add_tcase(s, tc);

  return s;
}

Suite *rotateBlock_suite(void) {
  Suite *s = suite_create("************rotateBlock**********");
  TCase *tc = tcase_create("tetris");

  tcase_add_test(tc, rotateBlock1);

  suite_add_tcase(s, tc);

  return s;
}

Suite *moveBlockDown_suite(void) {
  Suite *s = suite_create("************moveBlockDown**********");
  TCase *tc = tcase_create("tetris");

  tcase_add_test(tc, moveBlockDown1);
  tcase_add_test(tc, moveBlockDown2);

  suite_add_tcase(s, tc);

  return s;
}

Suite *initNextBlock_suite(void) {
  Suite *s = suite_create("************initNextBlock**********");
  TCase *tc = tcase_create("tetris");

  tcase_add_test(tc, initNextBlock1);

  suite_add_tcase(s, tc);

  return s;
}

Suite *processFullRows_suite(void) {
  Suite *s = suite_create("************processFullRows**********");
  TCase *tc = tcase_create("tetris");

  tcase_add_test(tc, processFullRows1);

  suite_add_tcase(s, tc);

  return s;
}

Suite *get_signal_suite(void) {
  Suite *s = suite_create("************get_signal**********");
  TCase *tc = tcase_create("tetris");

  tcase_add_test(tc, get_signal1);
  tcase_add_test(tc, get_signal2);

  suite_add_tcase(s, tc);

  return s;
}

Suite *userInput_suite(void) {
  Suite *s = suite_create("************userInput**********");
  TCase *tc = tcase_create("tetris");

  tcase_add_test(tc, userInput1);
  tcase_add_test(tc, userInput2);
  tcase_add_test(tc, userInput3);
  tcase_add_test(tc, userInput4);
  tcase_add_test(tc, userInput5);
  tcase_add_test(tc, userInput6);
  tcase_add_test(tc, userInput7);
  tcase_add_test(tc, userInput8);
  tcase_add_test(tc, userInput9);
  tcase_add_test(tc, userInput10);

  suite_add_tcase(s, tc);

  return s;
}

/////////////////////////////////////////////////////////////////////////////
int tetris_tests(Suite *s) {
  int failed_count;
  SRunner *runner = srunner_create(s);

  srunner_run_all(runner, CK_NORMAL);

  failed_count = srunner_ntests_failed(runner);
  srunner_free(runner);

  if (failed_count != 0) {
    failed_count = 1;
  }

  return failed_count;
}

int main(void) {
  int result = 0;

  result += tetris_tests(initMemory_suite());
  result += tetris_tests(rotateNextBlock_suite());
  result += tetris_tests(initBlockInfo_suite());
  result += tetris_tests(addBlockToField_suite());
  result += tetris_tests(delBlockFromField_suite());
  result += tetris_tests(rotateBlock_suite());
  result += tetris_tests(moveBlockDown_suite());
  result += tetris_tests(initNextBlock_suite());
  result += tetris_tests(processFullRows_suite());
  result += tetris_tests(get_signal_suite());
  result += tetris_tests(userInput_suite());

  printf("Failed tests: %d\n", result);

  return 0;
}
