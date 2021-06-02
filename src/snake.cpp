#include <curses.h>

#include <iostream>


bool gameOver;
constexpr int width { 40 };
constexpr int height { 20 };
int x;
int y;
int fruitX;
int fruitY;
int score;
int tailX[100];
int tailY[100];
int nTail;

enum eDirecton
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};
eDirecton dir;

char* board = new char[width*height];
WINDOW * win;

void clearBoard()
{
    for (int col = 0; col < height; col++)
    {
        for (int row = 0; row < width; row++)
        {
            if (row == 0 || row == width-1 || col == 0 || col == height-1)
            {
                board[row + col*width] = '#';
            } else
            {
                board[row + col*width] = ' ';
            }
        }
    }
}

void setup() {
    // Curses Initialisations
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    win = newwin(height + 1, width, 0, 0);

    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;

    clearBoard();

    score = 0;
    gameOver = false;
}

void draw()
{
    clearBoard();
    board[fruitX + fruitY * width] = 'X';
    board[x + y * width] = 'O';
    for (int k { 0 }; k < nTail; k++)
    {
             board[tailX[k] + tailY[k] * width] = 'o';
    }

    mvwprintw(win, 0, 0, board);
    wrefresh(win);

    char const *scoreStr = std::to_string(score).c_str();
    mvwprintw(win, height, 0, scoreStr);
}

void input()
{
    cbreak();
    timeout(167); // wait 167 milliseconds for input
    int ch = getch();
    if (ch == ERR)
    {
        // ignore timeout expired
        return;
    }

    switch(ch)
    {
        case KEY_UP:
        case 'w':
            dir = UP;
            break;
        case KEY_DOWN:
        case 's':
            dir = DOWN;
            break;
        case KEY_LEFT:
        case 'a':
            dir = LEFT;
            break;
        case KEY_RIGHT:
        case 'd':
            dir = RIGHT;
            break;
        case 'q':
        case 'Q':
            gameOver = true;
            break;
    }
}

void logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir)
    {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }

    if (x >= width)
    {
        x = 0;
    } else if (x < 0)
    {
        x = width - 1;
    };

    if (y >= height)
    {
        y = 0;
    } else if (y < 0)
    {
        y = height - 1;
    }

    for (int i = 0; i < nTail; i++)
    {
        if (tailX[i] == x && tailY[i] == y)
        {
            gameOver = true;
        }
    }

    if (x == fruitX && y == fruitY)
    {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

int main()
{
    setup();
    while (!gameOver)
    {
        draw();
        input();
        logic();
    }
    endwin();

    return 0;
}

