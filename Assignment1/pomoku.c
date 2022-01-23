#include <assert.h>

#include "pomoku.h"
static void add_score(const color_t color, const int add_score);


#define MIN_BOARD_ROW_COUNT (10)
#define MAX_BOARD_ROW_COUNT (20)
#define MIN_BOARD_COL_COUNT (MIN_BOARD_ROW_COUNT)
#define MAX_BOARD_COL_COUNT (MAX_BOARD_ROW_COUNT)

static size_t s_row_count = 0;
static size_t s_col_count = 0;
static color_t s_board_color[MAX_BOARD_ROW_COUNT][MAX_BOARD_COL_COUNT];

static int s_black_score = 0;
static int s_white_score = 0;

void init_game(void)
{
    const size_t INIT_BOARD_ROW_COUNT = 15;
    const size_t INIT_BOARD_COL_COUNT = INIT_BOARD_ROW_COUNT;

    s_row_count = INIT_BOARD_ROW_COUNT;
    s_col_count = INIT_BOARD_COL_COUNT;
    s_black_score = 0;
    s_white_score = 0;

    {
        size_t row;
        size_t col;

        for (row = 0; row < MAX_BOARD_ROW_COUNT; ++row) {
            for (col = 0; col < MAX_BOARD_COL_COUNT; ++col) {
                s_board_color[row][col] = COLOR_EMPTY;
            }
        }
    }
}

size_t get_row_count(void)
{
    return s_row_count;
}

size_t get_column_count(void)
{
    return s_col_count;
}

int get_score(const color_t color)
{
    switch (color) {
    case COLOR_BLACK:
        return s_black_score;
        break;
    case COLOR_WHITE:
        return s_white_score;
        break;
    default:
        /* assert(0); */
        return -1;
        break;
    }
}

static void add_score(const color_t color, const int add_score)
{
    switch (color) {
    case COLOR_BLACK:
        s_black_score += add_score;
        break;
    case COLOR_WHITE:
        s_white_score += add_score;
        break;
    default:
        assert(0);
        break;
    }
}

int is_valid_color(const color_t color)
{
    switch (color) {
    case COLOR_EMPTY:
        return FALSE;
        break;

    case COLOR_BLACK:
        /* intentional fallthrough */
    case COLOR_WHITE:
        return TRUE;
        break;

    default:
        /* assert(0); */
        return FALSE;
        break;
    }
}

int get_color(const size_t row, const size_t col)
{
    if (is_in_board(row, col) == FALSE) {
        return -1;
    }

    switch (s_board_color[row][col]) {
    case COLOR_EMPTY:
        return -1;
        break;

    case COLOR_BLACK:
        /* intentional fallthrough */
    case COLOR_WHITE:
        return s_board_color[row][col];
        break;

    default:
        assert(0);
        return -1;
        break;
    }
}

int is_in_board(const size_t row, const size_t col)
{
    if (is_in_board_row(row) == FALSE || is_in_board_col(col) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

int is_in_board_row(const size_t row)
{
    if (s_row_count <= row) {
        return FALSE;
    }

    return TRUE;
}

int is_in_board_col(const size_t col)
{
    if (s_col_count <= col) {
        return FALSE;
    }

    return TRUE;
}

int is_placeable(const size_t row, const size_t col)
{
    if (is_in_board(row, col) == FALSE) {
        return FALSE;
    }

    switch (s_board_color[row][col]) {
    case COLOR_EMPTY:
        return TRUE;
        break;

    case COLOR_BLACK:
        /* intentional fallthrough */
    case COLOR_WHITE:
        return FALSE;
        break;

    default:
        assert(0);
        return FALSE;
        break;
    }
}

int place_stone(const color_t color, const size_t row, const size_t col)
{
    const int SCORE_START_COUNT = 5;

    if (is_placeable(row, col) == FALSE) {
        return FALSE;
    }

    if (is_valid_color(color) == FALSE) {
        return FALSE;
    }

    s_board_color[row][col] = color;

    /* calculate score */
    {
        /* horizontal */
        {
            int count = 0;

            size_t col_i;

            /* horizontal, right */
            for (col_i = col; col_i < s_col_count; ++col_i) {
                if (s_board_color[row][col_i] != color) {
                    break;
                }

                ++count;
            }

            /* horizontal, left */
            for (col_i = col - 1; col_i != (size_t)-1; --col_i) {
                if (s_board_color[row][col_i] != color) {
                    break;
                }

                ++count;
            }

            {
                const int score = count - (SCORE_START_COUNT - 1) > 0 ? count - (SCORE_START_COUNT - 1) : 0;
                add_score(color, score);
            }
        }

        /* vertical */
        {
            int count = 0;

            size_t row_i;

            /* vertical, top */
            for (row_i = row; row_i < s_row_count; ++row_i) {
                if (s_board_color[row_i][col] != color) {
                    break;
                }

                ++count;
            }

            /* vertical, bottom */
            for (row_i = row - 1; row_i != (size_t)-1; --row_i) {
                if (s_board_color[row_i][col] != color) {
                    break;
                }

                ++count;
            }

            {
                const int score = count - (SCORE_START_COUNT - 1) > 0 ? count - (SCORE_START_COUNT - 1) : 0;
                add_score(color, score);
            }
        }

        /* left bottom to right top cross */
        {
            int count = 0;

            size_t cross_i;

            /* left bottom to right top cross, top */
            cross_i = 0;
            while (is_in_board(row - cross_i, col + cross_i) == TRUE) {
                if (s_board_color[row - cross_i][col + cross_i] != color) {
                    break;
                }

                ++cross_i;
                ++count;
            }


            /* left bottom to right top cross, bottom */
            cross_i = 1;
            while (is_in_board(row + cross_i, col - cross_i) == TRUE) {
                if (s_board_color[row + cross_i][col - cross_i] != color) {
                    break;
                }

                ++cross_i;
                ++count;
            }

            {
                const int score = count - (SCORE_START_COUNT - 1) > 0 ? count - (SCORE_START_COUNT - 1) : 0;
                add_score(color, score);
            }
        }

        /* left top to right bottom cross cross */
        {
            int count = 0;

            size_t cross_i;

            /* left top to right bottom cross, top */
            cross_i = 0;
            while (is_in_board(row - cross_i, col - cross_i) == TRUE) {
                if (s_board_color[row - cross_i][col - cross_i] != color) {
                    break;
                }

                ++cross_i;
                ++count;
            }

            /* left top to right bottom cross, bottom */
            cross_i = 1;
            while (is_in_board(row + cross_i, col + cross_i) == TRUE) {
                if (s_board_color[row + cross_i][col + cross_i] != color) {
                    break;
                }

                ++cross_i;
                ++count;
            }

            {
                const int score = count - (SCORE_START_COUNT - 1) > 0 ? count - (SCORE_START_COUNT - 1) : 0;
                add_score(color, score);
            }
        }

    } /* calculate score */

    return TRUE;
}

/* special moves */
int insert_row(const color_t color, const size_t row)
{
    const int NEED_SCORE = 3;

    if (is_valid_color(color) == FALSE) {
        return FALSE;
    }

    if (s_row_count < row) {
        return FALSE;
    }

    if (get_score(color) < NEED_SCORE) {
        return FALSE;
    }

    if (get_row_count() >= MAX_BOARD_ROW_COUNT) {
        return FALSE;
    }

    {
        size_t row_i;
        size_t col_i;

        for (row_i = s_row_count - 1; row_i != (size_t)-1 && row_i >= row; --row_i) {
            for (col_i = 0; col_i < s_col_count; ++col_i) {
                s_board_color[row_i + 1][col_i] = s_board_color[row_i][col_i];
            }
        }
    }

    {
        size_t col_i;

        for (col_i = 0; col_i < s_col_count; ++col_i) {
            s_board_color[row][col_i] = COLOR_EMPTY;
        }
    }

    add_score(color, -NEED_SCORE);
    ++s_row_count;
    return TRUE;
}

int insert_column(const color_t color, const size_t col)
{
    const int NEED_SCORE = 3;

    if (is_valid_color(color) == FALSE) {
        return FALSE;
    }

    if (s_col_count < col) {
        return FALSE;
    }

    if (get_score(color) < NEED_SCORE) {
        return FALSE;
    }

    if (s_col_count >= MAX_BOARD_COL_COUNT) {
        return FALSE;
    }

    {
        size_t row_i;
        size_t col_i;

        for (row_i = 0; row_i < s_row_count; ++row_i) {
            for (col_i = s_col_count - 1; col_i != (size_t)-1 && col_i >= col; --col_i) {
                s_board_color[row_i][col_i + 1] = s_board_color[row_i][col_i];
            }
        }
    }

    {
        size_t row_i;

        for (row_i = 0; row_i < s_row_count; ++row_i) {
            s_board_color[row_i][col] = COLOR_EMPTY;
        }
    }

    add_score(color, -NEED_SCORE);
    ++s_col_count;
    return TRUE;
}

int remove_row(const color_t color, const size_t row)
{
    const int NEED_SCORE = 3;

    if (is_valid_color(color) == FALSE) {
        return FALSE;
    }

    if (is_in_board_row(row) == FALSE) {
        return FALSE;
    }

    if (get_score(color) < NEED_SCORE) {
        return FALSE;
    }

    if (s_row_count <= MIN_BOARD_ROW_COUNT) {
        return FALSE;
    }

    {
        size_t row_i;
        size_t col_i;

        for (row_i = row + 1; row_i < s_row_count; ++row_i) {
            for (col_i = 0; col_i < s_col_count; ++col_i) {
                s_board_color[row_i - 1][col_i] = s_board_color[row_i][col_i];
            }
        }
    }

    {
        const size_t row_i = s_row_count - 1;
        size_t col_i;

        for (col_i = 0; col_i < s_col_count; ++col_i) {
            s_board_color[row_i][col_i] = COLOR_EMPTY;
        }
    }

    add_score(color, -NEED_SCORE);
    --s_row_count;
    return TRUE;
}

int remove_column(const color_t color, const size_t col)
{
    const int NEED_SCORE = 3;

    if (is_valid_color(color) == FALSE) {
        return FALSE;
    }

    if (is_in_board_col(col) == FALSE) {
        return FALSE;
    }

    if (get_score(color) < NEED_SCORE) {
        return FALSE;
    }

    if (s_col_count <= MIN_BOARD_COL_COUNT) {
        return FALSE;
    }

    {
        size_t row_i;
        size_t col_i;

        for (row_i = 0; row_i < s_row_count; ++row_i) {
            for (col_i = col + 1; col_i < s_col_count; ++col_i) {
                s_board_color[row_i][col_i - 1] = s_board_color[row_i][col_i];
            }
        }
    }

    {
        size_t row_i;
        const size_t col_i = s_col_count - 1;

        for (row_i = 0; row_i < s_row_count; ++row_i) {
            s_board_color[row_i][col_i] = COLOR_EMPTY;
        }
    }

    add_score(color, -NEED_SCORE);
    --s_col_count;
    return TRUE;
}

int swap_rows(const color_t color, const size_t row0, const size_t row1)
{
    const int NEED_SCORE = 2;

    if (is_valid_color(color) == FALSE) {
        return FALSE;
    }

    if (is_in_board_row(row0) == FALSE) {
        return FALSE;
    }

    if (is_in_board_row(row1) == FALSE) {
        return FALSE;
    }

    if (get_score(color) < NEED_SCORE) {
        return FALSE;
    }

    {
        color_t temp_row0[MAX_BOARD_ROW_COUNT];
        size_t col_i;

        for (col_i = 0; col_i < s_col_count; ++col_i) {
            temp_row0[col_i] = s_board_color[row0][col_i];
        }

        for (col_i = 0; col_i < s_col_count; ++col_i) {
            s_board_color[row0][col_i] = s_board_color[row1][col_i];
        }

        for (col_i = 0; col_i < s_col_count; ++col_i) {
            s_board_color[row1][col_i] = temp_row0[col_i];
        }
    }

    add_score(color, -NEED_SCORE);
    return TRUE;
}

int swap_columns(const color_t color, const size_t col0, const size_t col1)
{
    const int NEED_SCORE = 2;

    if (is_valid_color(color) == FALSE) {
        return FALSE;
    }

    if (is_in_board_col(col0) == FALSE) {
        return FALSE;
    }

    if (is_in_board_col(col1) == FALSE) {
        return FALSE;
    }

    if (get_score(color) < NEED_SCORE) {
        return FALSE;
    }

    {
        color_t temp_col0[MAX_BOARD_ROW_COUNT];
        size_t row_i;

        for (row_i = 0; row_i < s_row_count; ++row_i) {
            temp_col0[row_i] = s_board_color[row_i][col0];
        }

        for (row_i = 0; row_i < s_row_count; ++row_i) {
            s_board_color[row_i][col0] = s_board_color[row_i][col1];
        }

        for (row_i = 0; row_i < s_row_count; ++row_i) {
            s_board_color[row_i][col1] = temp_col0[row_i];
        }
    }

    add_score(color, -NEED_SCORE);
    return TRUE;
}

int copy_row(const color_t color, const size_t src, const size_t dst)
{
    const int NEED_SCORE = 4;

    if (is_valid_color(color) == FALSE) {
        return FALSE;
    }

    if (is_in_board_row(src) == FALSE) {
        return FALSE;
    }

    if (is_in_board_row(dst) == FALSE) {
        return FALSE;
    }

    if (get_score(color) < NEED_SCORE) {
        return FALSE;
    }

    {
        size_t col_i;
        for (col_i = 0; col_i < s_col_count; ++col_i) {
            s_board_color[dst][col_i] = s_board_color[src][col_i];
        }
    }

    add_score(color, -NEED_SCORE);
    return TRUE;
}

int copy_column(const color_t color, const size_t src, const size_t dst)
{
    const int NEED_SCORE = 4;

    if (is_valid_color(color) == FALSE) {
        return FALSE;
    }

    if (is_in_board_col(src) == FALSE) {
        return FALSE;
    }

    if (is_in_board_col(dst) == FALSE) {
        return FALSE;
    }

    if (get_score(color) < NEED_SCORE) {
        return FALSE;
    }

    {
        size_t row_i;
        for (row_i = 0; row_i < s_row_count; ++row_i) {
            s_board_color[row_i][dst] = s_board_color[row_i][src];
        }
    }

    add_score(color, -NEED_SCORE);
    return TRUE;
}

