#include <iostream>
#include <curses.h>

using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirecton dir;
char** board = new char*[height];
WINDOW * win;

void clearBoard() {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (i == 0 || i == height-1 || j == 0 || j == width-1) {
        board[i][j] = '#';
      } else {
        board[i][j] = ' ';
      }
    }
  }
}
void setupBoard() {
  for (int i = 0; i < height; i++)
    board[i] = new char[width];

  clearBoard();
}

void setup() {
  /* Curses Initialisations */
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  win  = newwin(height+1, width+1, 0, 0);

  dir = STOP;
  x = width / 2;
  y = height / 2;
  fruitX = rand() % width;
  fruitY = rand() % height;

  setupBoard();

  score = 0;
  gameOver = false;
}
 
void draw() {
  clearBoard();
  board[y][x] = 'O';
  board[fruitY][fruitX] = 'F';
  for (int k = 0; k < nTail; k++) {
       board[tailY[k]][tailX[k]] = 'o';
  }

  for (int i = 0; i < height; i++) {
    mvwprintw(win, i, 0, board[i]);
    wrefresh(win);
  }
}

void input() {
  cbreak();
  timeout(167); // wait 167 milliseconds for input
  int ch = getch();
  if (ch == ERR)
    return;  // ignore timeout expired
  switch(ch) {
    case KEY_UP:
      dir = UP;
      break;
    case KEY_DOWN:
      dir = DOWN;
      break;
    case KEY_LEFT:
      dir = LEFT;
      break;
    case KEY_RIGHT:
      dir = RIGHT;
      break;
    case 'x':
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
  switch (dir) {
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

  if (x >= width) x = 0; else if (x < 0) x = width - 1;
  if (y >= height) y = 0; else if (y < 0) y = height - 1;

  for (int i = 0; i < nTail; i++)
    if (tailX[i] == x && tailY[i] == y)
      gameOver = true;

  if (x == fruitX && y == fruitY) {
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

