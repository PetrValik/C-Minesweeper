#include "cut.h"

#include "../minesweeper.h"
#include "utils_for_tests.h"

/* Our provided tests are testing:
 *
 * set_cell - subset of valid states
 * postprocess - on simple valid board
 * assisting functions - is_mine, is_revealed ...
 * show_cell - subset of valid cells/states
 *
 * These functions are necessary for us, to test your representation.
 *
 * NOTE: These tests are not complete at all, they just give you a rough idea,
 *       if you are working correctly. They should also motivate you to create
 *       some of your own.
 *
 * Additional tests:
 * - print_board (this test is not a part of the nanecisto tests)
 * - load_board (this test is not a part of the nanecisto tests)
 *
 * We have provided you with these additional tests to give you a simple example
 * how the Output and Input should look like.
 *
 * As it was said previously, you should extend these provided tests
 * with your own. */

TEST(set_cell_with_mine_uppercase)
{
    // GIVEN
    uint16_t dummy_cell = 0;
    // DO
    ASSERT(set_cell(&dummy_cell, 'M'));
    // THEN
    ASSERT(is_mine(dummy_cell));
}

TEST(set_cell_with_mine_lovercase)
{
    // GIVEN
    uint16_t dummy_cell = 0;
    // DO
    ASSERT(set_cell(&dummy_cell, 'm'));
    // THEN
    ASSERT(is_mine(dummy_cell));
}

TEST(set_cell_revealed)
{
    // GIVEN
    uint16_t dummy_cell = 0;
    // DO
    ASSERT(set_cell(&dummy_cell, '.'));
    // THEN
    ASSERT(is_revealed(dummy_cell));
}

TEST(postprocess_on_simple_board_3by3_board)
{
    // GIVEN
    const size_t size = 3;
    uint16_t board[3][3] = { 0 };
    fill_board_value(size, size, board, '.'); // function from test utils
    set_cell(&(board[1][1]), 'M');
    // THEN
    ASSERT(postprocess(size, size, board) == 1); // it has one mine
}

TEST(show_cell_on_3by3_board)
{
    // GIVEN
    const size_t size = 3;
    uint16_t board[3][3] = { 0 };
    const char *input = "XXX"
                        "XWF"
                        "XMX";
    // THEN
    fill_board(size, size, board, input); // function from test utils

    ASSERT(show_cell(board[0][0]) == 'X');
    ASSERT(show_cell(board[1][1]) == 'F'); // this is a wrong flag, but in print it should be shown as flag
    ASSERT(show_cell(board[1][2]) == 'F'); // this is a correct flag
    ASSERT(show_cell(board[2][1]) == 'X'); // this is mine, but it should not be revealed
    ASSERT(show_cell(board[2][2]) == 'X');
}

/* *** NOTE ***
 * This test it NOT part of the sanity (nanecisto) suite and will not
 * prevent you from submitting your solution.
 *
 * It is here only to show you the expected formatting of 'print_board()'. */
TEST(print_revealed_all_but_mine_board)
{
    // GIVEN
    const size_t size = 3;
    uint16_t board[3][3] = { 0 };
    const char *input = "..."
                        ".M."
                        "...";
    fill_board(size, size, board, input);

    // THEN
    ASSERT(print_board(size, size, board) == 0);

    ASSERT_FILE(stdout, "     0   1   2 \n"
                        "   +---+---+---+\n"
                        " 0 | 1 | 1 | 1 |\n"
                        "   +---+---+---+\n"
                        " 1 | 1 |XXX| 1 |\n"
                        "   +---+---+---+\n"
                        " 2 | 1 | 1 | 1 |\n"
                        "   +---+---+---+\n");
}

/* *** NOTE ***
 * This test it NOT part of the sanity (nanecisto) suite and will not
 * prevent you from submitting your solution.
 *
 * It is here only to show you the expected formatting of 'print_board()'. */
TEST(random)
{

    uint16_t cell = 0;
    set_cell(&cell ,'M');
    ASSERT(reveal_single(&cell) == 1);
    ASSERT(reveal_single(&cell) == -1);
    cell = 0;
    set_cell(&cell ,'F');
    ASSERT(reveal_single(&cell) == -1);
    ASSERT(reveal_single(NULL) == -1);
    cell = 0;
    set_cell(&cell ,'2');
    ASSERT(reveal_single(NULL) == -1);
}

TEST(show)
{

    uint16_t cell = 0;
    set_cell(&cell ,'M');
    reveal_single(&cell);
    ASSERT(show_cell(cell) == 'M');
    cell = 0;
    set_cell(&cell ,'F');
    ASSERT(show_cell(cell) == 'F');
    cell = 0;
    set_cell(&cell ,'f');
    ASSERT(show_cell(cell) == 'F');
    cell = 0;
    set_cell(&cell ,'m');
    reveal_single(&cell);
    ASSERT(show_cell(cell) == 'M');
    cell = 0;
    set_cell(&cell ,'W');
    ASSERT(show_cell(cell) == 'F');
    cell = 0;
    set_cell(&cell ,'w');
    ASSERT(show_cell(cell) == 'F');
    cell = 0;
    set_cell(&cell ,'0');
    ASSERT(show_cell(cell) == ' ');
    cell = 0;
    set_cell(&cell ,'1');
    ASSERT(show_cell(cell) == '1');
    cell = 0;
    set_cell(&cell ,'2');
    ASSERT(show_cell(cell) == '2');
    cell = 0;
    set_cell(&cell ,'3');
    ASSERT(show_cell(cell) == '3');
    cell = 0;
    set_cell(&cell ,'4');
    ASSERT(show_cell(cell) == '4');
    cell = 0;
    set_cell(&cell ,'5');
    ASSERT(show_cell(cell) == '5');
    cell = 0;
    set_cell(&cell ,'6');
    ASSERT(show_cell(cell) == '6');
    cell = 0;
    set_cell(&cell ,'7');
    ASSERT(show_cell(cell) == '7');
    cell = 0;
    set_cell(&cell ,'8');
    ASSERT(show_cell(cell) == '8');
    cell = 0;
    ASSERT(show_cell(cell) == -1);

}

TEST(load_unrevealed_3by3_board)
{
    // GIVEN
    const size_t size = 3;
    uint16_t board[3][3] = { 0 };
    INPUT_STRING("XXX"
                 "XMX"
                 "XXX"); // the content of STDIN for LOAD

    // THEN
    ASSERT(load_board(size, size, board) == 1); // it has one mine

    ASSERT(is_mine(board[1][1]));
    ASSERT(!is_mine(board[1][0]));
}

/* TODO: You should write your tests for specified other functions and values
 * in the similar manner. */

/* If you've made it down here I would also would like to tell you the meaning
 * of life is 42. */
