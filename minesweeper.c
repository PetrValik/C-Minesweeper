#include "minesweeper.h"
#include <stdio.h>
#include <stdbool.h>

#define UNUSED(A) (void) (A)

/* ************************************************************** *
 *                         HELP FUNCTIONS                         *
 * ************************************************************** */

#define NO_MINE 1
#define ONE_MINE 2
#define TWO_MINES 4
#define THREE_MINES 8
#define FOUR_MINES 16
#define FIVE_MINES 32
#define SIX_MINES 64
#define SEVEN_MINES 128
#define EIGHT_MINES 256
#define REVEALED_TILE 512
#define UNREVEALED_TILE 1024
#define REVEALED_NO_SPECIFIC_MINES 2048
#define MINE 4096
#define GOOD_FLAG 8192
#define BAD_FLAG 16384
#define FLAG 32768
#define ROW_IN_ARRAY(ROW, ROWS, I) ((I == -1 && ROW == 0) || (ROW + 1 == ROWS && I == 1))
#define COL_IN_ARRAY(COL, COLS, J) ((J == -1 && COL == 0) || (COL + 1 == COLS && J == 1))
#define IS_ANOTHER_CELL(I, J) (I == 0 && J == 0)

bool is_flag(uint16_t cell)
{
    if ((cell & FLAG) == FLAG) {
        return true;
    }
    return false;
}

bool is_mine(uint16_t cell)
{
    if ((cell & MINE) == MINE) {
        return true;
    }
    return false;
}

bool is_revealed(uint16_t cell)
{
    if ((cell & REVEALED_TILE) == REVEALED_TILE) {
        return true;
    }
    return false;
}

int get_number(uint16_t cell)
{
    if (is_mine(cell) == true) {
        return 0;
    }
    switch (cell & ((1 << 9)-1)) {
        case ONE_MINE:
            return 1;
        case TWO_MINES:
            return 2;
        case THREE_MINES:
            return 3;
        case FOUR_MINES:
            return 4;
        case FIVE_MINES:
            return 5;
        case SIX_MINES:
            return 6;
        case SEVEN_MINES:
            return 7;
        case EIGHT_MINES:
            return 8;
        default:
            return -1;
    }
}

/* ************************************************************** *
 *                         INPUT FUNCTIONS                        *
 * ************************************************************** */


bool set_cell(uint16_t *cell, char val)
{
    if (cell == NULL) {
        return -1;
    }
    switch(val) {
        case 'X':
        case 'x':
            *cell += UNREVEALED_TILE;
            break;
        case 'M':
        case 'm':
            *cell += MINE;
            *cell += UNREVEALED_TILE;
            break;
        case 'F':
        case 'f':
            *cell += FLAG;
            *cell += GOOD_FLAG;
            *cell += UNREVEALED_TILE;
            *cell += MINE;
            break;
        case 'w':
        case 'W':
            *cell += FLAG;
            *cell += BAD_FLAG;
            *cell += UNREVEALED_TILE;
            break;
        case '0':
            *cell += NO_MINE;
            *cell += REVEALED_TILE;
            break;
        case '.':
            *cell += REVEALED_NO_SPECIFIC_MINES;
            *cell += REVEALED_TILE;
            break;
        case '1':
            *cell += ONE_MINE;
            *cell += REVEALED_TILE;
            break;
        case '2':
            *cell += TWO_MINES;
            *cell += REVEALED_TILE;
            break;
        case '3':
            *cell += THREE_MINES;
            *cell += REVEALED_TILE;
            break;
        case '4':
            *cell += FOUR_MINES;
            *cell += REVEALED_TILE;
            break;
        case '5':
            *cell += FIVE_MINES;
            *cell += REVEALED_TILE;
            break;
        case '6':
            *cell += SIX_MINES;
            *cell += REVEALED_TILE;
            break;
        case '7':
            *cell += SEVEN_MINES;
            *cell += REVEALED_TILE;
            break;
        case '8':
            *cell += EIGHT_MINES;
            *cell += REVEALED_TILE;
            break;
        default:
            return false;
    }

    return true;
}

int load_board(size_t rows, size_t cols, uint16_t board[rows][cols])
{
    if (board == NULL || rows < 3 || cols < 3 || cols > 99 || rows > 99) {
        return -1;
    }
    int input = getchar();
    size_t row = 0;
    size_t col = 0;
    while (input != EOF) {
        board[row][col] = 0;
        if (set_cell(&board[row][col], (char) input) == true) {
            col++;
        }
        if (col == cols) {
            row++;
            col = 0;
        }
        if (rows == row) {
            break;
        }
        input = getchar();
    }
    if (rows != row) {
        return -1;
    }
    return postprocess(rows, cols, board);
}

int get_mines(size_t rows, size_t cols, uint16_t board[rows][cols])
{
    int mines = 0;
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (is_mine(board[i][j]) == true) {
                mines++;
            }
        }
    }
    return mines;
}

int is_correct_clue(size_t rows, size_t  cols, uint16_t board[rows][cols], size_t row, size_t col)
{
    int near_mines = get_number(board[row][col]);
    if (near_mines == -1) {
        near_mines = 0;
    }

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (!(ROW_IN_ARRAY(row, rows, i) || COL_IN_ARRAY(col, cols, j) || IS_ANOTHER_CELL(i, j))) {
                if (is_mine(board[row + i][col + j]) == true) {
                    near_mines--;
                }
            }
        }
    }
    return near_mines;
}

int get_incorect_mines_clues(size_t rows, size_t cols, uint16_t board[rows][cols]) {
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (is_mine(board[i][j]) == false) {
                if (is_correct_clue(rows, cols, board, i, j) != 0) {
                    return -1;
                }
            }
        }
    }
    return 0;
}

int get_corner_mines(size_t rows, size_t cols, uint16_t board[rows][cols])
{
    if (is_mine(board[0][0]) == true) {
        return -1;
    }
    if (is_mine(board[rows - 1][0]) == true) {
        return -1;
    }
    if (is_mine(board[0][cols - 1]) == true) {
        return -1;
    }
    if (is_mine(board[rows - 1][cols - 1]) == true) {
        return -1;
    }
    return 0;
}

void count_mines(size_t row, size_t col, size_t rows, size_t cols, uint16_t board[rows][cols])
{
    int counter = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (!(ROW_IN_ARRAY(row, rows, i) || COL_IN_ARRAY(col, cols, j) || IS_ANOTHER_CELL(i, j))) {
                if (is_mine(board[row + i][col + j]) == true) {
                    counter++;
                }
            }
        }
    }
    size_t mines_count = 1;
    mines_count = mines_count << counter;
    board[row][col] += mines_count;
}

void calculate_mines(size_t rows, size_t cols, uint16_t board[rows][cols])
{
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (!(is_mine(board[i][j])) && ((is_revealed(board[i][j]) == false) ||
                    (board[i][j] & REVEALED_NO_SPECIFIC_MINES) == REVEALED_NO_SPECIFIC_MINES
            || (is_flag(board[i][j]) && is_mine(board[i][j]) == false))) {
                count_mines(i, j, rows, cols, board);
            }
        }
    }
}

int postprocess(size_t rows, size_t cols, uint16_t board[rows][cols])
{
    if (rows < 3 || cols < 3 || rows > 99 || cols > 99) {
        return -1;
    }
    calculate_mines(rows, cols, board);
    if (get_corner_mines(rows, cols, board) != 0) {
        return -1;
    }
    if (get_incorect_mines_clues(rows, cols, board) != 0) {
        return -1;
    }
    int mines = get_mines(rows, cols, board);
    if (mines == 0) {
        return -1;
    }
    return mines;
}

/* ************************************************************** *
 *                        OUTPUT FUNCTIONS                        *
 * ************************************************************** */

void print_cols(size_t cols)
{
    printf("   ");
    for (size_t i = 0; i < cols; i++) {
        if (i >= 10) {
            printf(" %zu", i);
        } else {
            printf(" ");
            printf(" %zu", i);
        }
        printf(" ");
    }
    putchar('\n');
}

void print_separator(size_t cols)
{
    printf("   ");
    for (size_t i = 0; i < cols; i++) {
        printf("+---");
    }
    printf("+\n");
}

void print_row(size_t index, size_t cols, uint16_t row[cols])
{
    if (index >= 10) {
        printf("%zu ", index);
    } else {
        printf(" %zu ", index);
    }
    for (size_t i = 0; i < cols; i++) {
        putchar('|');
        if (is_flag(row[i]) == true) {
            printf("_F_");
        } else if (is_revealed(row[i]) == false) {
            printf("XXX");
        } else if (is_mine(row[i])) {
            printf(" M ");
        } else if ((row[i] & NO_MINE) == NO_MINE) {
            printf("   ");
        } else {
            printf(" %d ", get_number(row[i]));
            }
    }
    printf("|\n");
}

int print_board(size_t rows, size_t cols, uint16_t board[rows][cols])
{
    print_cols(cols);
    for (size_t i = 0; i < rows; i++) {
        print_separator(cols);
        print_row(i, cols, board[i]);
    }
    print_separator(cols);
    return 0;
}

char show_cell(uint16_t cell)
{
    if ((is_flag(cell)) == true) {
        return 'F';
    } else if ((cell & UNREVEALED_TILE) == UNREVEALED_TILE) {
        return 'X';
    } else if ((is_mine(cell)) == true) {
        return 'M';
    }
    int number = get_number(cell);
    if (number == -1) {
        if ((cell & NO_MINE) == NO_MINE) {
            return ' ';
        }
    }
    if (number >= 0 && number <= 8) {
        return '0' + number;
    }
    return -1;
}

/* ************************************************************** *
 *                    GAME MECHANIC FUNCTIONS                     *
 * ************************************************************** */

int reveal_cell(size_t rows, size_t cols, uint16_t board[rows][cols], size_t row, size_t col)
{
    if (row >= rows || col >= cols) {
        return -1;
    }
    int result = reveal_single(&board[row][col]);
    if (result != 0) {
        return result;
    }
    if ((board[row][col] & NO_MINE) == NO_MINE) {
        reveal_floodfill(rows, cols, board, row, col);
    }
    return result;
}

int reveal_single(uint16_t *cell)
{
    if (cell == NULL || is_revealed(*cell) == true || is_flag(*cell) == true) {
        return -1;
    }
    *cell += REVEALED_TILE;
    *cell -= UNREVEALED_TILE;
    if (is_mine(*cell) == true) {
        return 1;
    }
    return 0;
}

void reveal_floodfill_cell(size_t rows, size_t cols, uint16_t board[rows][cols], size_t row, size_t col)
{
    if (is_revealed(board[row][col]) == true) {
        return;
    }
    board[row][col] += REVEALED_TILE;
    board[row][col] -= UNREVEALED_TILE;
    if (is_flag(board[row][col]) == true) {
        board[row][col] -= FLAG;
        board[row][col] -= BAD_FLAG;
    }

    if ((board[row][col] & NO_MINE) == NO_MINE) {
        reveal_floodfill(rows, cols, board, row, col);
    }
}

void reveal_floodfill(size_t rows, size_t cols, uint16_t board[rows][cols], size_t row, size_t col)
{
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (!(ROW_IN_ARRAY(row, rows, i) || COL_IN_ARRAY(col, cols, j) || IS_ANOTHER_CELL(i, j))) {
                reveal_floodfill_cell(rows, cols, board, row + i, col + j);
            }
        }
    }
}

int get_number_of_flags(size_t rows, size_t cols, uint16_t board[rows][cols])
{
    int flags = 0;
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (is_flag(board[i][j]) == true) {
                flags += 1;
            }
        }
    }
    return flags;
}

int flag_cell(size_t rows, size_t cols, uint16_t board[rows][cols], size_t row, size_t col)
{
    if (is_revealed(board[row][col]) == true && is_flag(board[row][col]) == false) {
        return -1;
    }
    if (is_flag(board[row][col]) == true) {
        board[row][col] -= FLAG;
        if (is_mine(board[row][col]) == true) {
            board[row][col] -= GOOD_FLAG;
        } else {
            board[row][col] -= BAD_FLAG;
        }
    } else {
        board[row][col] += FLAG;
        if (is_mine(board[row][col]) == true) {
            board[row][col] += GOOD_FLAG;
        } else {
            board[row][col] += BAD_FLAG;
        }
    }
    return get_mines(rows, cols, board) - get_number_of_flags(rows, cols, board);
}

int get_unrevealed_tiles(size_t rows, size_t cols, uint16_t board[rows][cols])
{
    int unrevealed_tiles = 0;
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (is_revealed(board[i][j]) == false) {
                unrevealed_tiles += 1;
            }
        }
    }
    return unrevealed_tiles;
}

bool is_solved(size_t rows, size_t cols, uint16_t board[rows][cols])
{
    return ((get_mines(rows, cols, board) == get_unrevealed_tiles(rows, cols, board)) == true);
}

/* ************************************************************** *
 *                         BONUS FUNCTIONS                        *
 * ************************************************************** */

int generate_random_board(size_t rows, size_t cols, uint16_t board[rows][cols], size_t mines)
{
    // TODO: Implement me
    UNUSED(mines);
    UNUSED(board);
    return -1;
}

int find_mines_set_cell(uint16_t *cell, char val)
{
    if (val == 'X' || val == 'x') {
        *cell += UNREVEALED_TILE;
    } else if (val == '0') {
        *cell += NO_MINE;
        *cell += REVEALED_TILE;
    } else if (val == '1') {
        *cell += ONE_MINE;
        *cell += REVEALED_TILE;
    } else if (val == '2') {
        *cell += TWO_MINES;
        *cell += REVEALED_TILE;
    } else if (val == '3') {
        *cell += THREE_MINES;
        *cell += REVEALED_TILE;
    } else if (val == '4') {
        *cell += FOUR_MINES;
        *cell += REVEALED_TILE;
    } else if (val == '5') {
        *cell += FIVE_MINES;
        *cell += REVEALED_TILE;
    } else if (val == '6') {
        *cell += SIX_MINES;
        *cell += REVEALED_TILE;
    } else if (val == '7') {
        *cell += SEVEN_MINES;
        *cell += REVEALED_TILE;
    } else if (val == '8') {
        *cell += EIGHT_MINES;
        *cell += REVEALED_TILE;
    } else {
        return false;
    }
    return true;
}

int find_mines(size_t rows, size_t cols, uint16_t board[rows][cols])
{
    // TODO: Implement me
    UNUSED(rows);
    UNUSED(cols);
    UNUSED(board);
    int input = getchar();
    size_t row = 0;
    size_t col = 0;
    while (input != EOF || row != rows) {
        if (find_mines_set_cell(&board[row][col], (char) input) == true) {
            col++;
        }
        if (col == cols) {
            row++;
            col = 0;
        }
        input = getchar();
    }
    return 0;
}
