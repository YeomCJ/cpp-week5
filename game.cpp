#include "game.h"
#include "console/console.h"
#include "tetromino.h"
#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <chrono>

using namespace std;
using namespace console;

// 게임의 한 프레임을 처리
void Game::update()
{
  int s = currentTetromino->size();

  if (console::key(Key::K_UP)) // 하드드롭
  {
    for (int i = 0; i < 20; i++)
      if (board_[x][i] == true)
      {
      }
  }
  else if (console::key(Key::K_DOWN))
  {
    y++;
  }
  else if (console::key(Key::K_LEFT))
  {
    x--;
  }
  else if (console::key(Key::K_RIGHT))
  {
    x++;
  }

  else if (console::key(Key::K_ENTER))
  { // ??
  }
  else if (console::key(Key::K_ESC)) // 정료
  {
    for (int i = 0; i < 7; i++)
      delete ar[i];
    delete holdTetromino;
    delete nextTetromino;
    delete currentTetromino;
    exit(0);
  }
  else if (console::key(Key::K_SPACE)) // 홀드
  {
    if (check == 0)
    {
      if (holdTetromino == nullptr)
        holdTetromino = currentTetromino->original();
      else
      {
        Tetromino *tmp = holdTetromino;
        holdTetromino = currentTetromino->original();
        currentTetromino = tmp;
        delete tmp;
      }
      x = 4;
      y = 0;
      check++;
    }
  }
  else if (console::key(Key::K_Z)) // 반시계 회전
  {
    currentTetromino->rotatedCCW();
  }
  else if (console::key(Key::K_X)) // 시계 회전
  {
    currentTetromino->rotatedCW();
  }

  for (int k = 0; k < s; k++)
  {
    for (int t = 1; t < s; t++)
    {
      if (currentTetromino->check(s - t, k))
      {
        if (board_[x + (s - t + 1)][y + k] == true)
        {
          for (int i = s - 1; i >= 0; i--)
            for (int j = s - 1; j >= 0; j--)
              if (currentTetromino->check(i, j))
                board_[x + i][y + j] = true;
          currentTetromino = nextTetromino;
          nextTetromino = ar[rand() % 7];
        }
      }
    }
  }

  // 한 줄 채우면 ~
  for (int i = BOARD_WIDTH - 1; i >= 0; i--)
  {
    int count = 0;
    for (int j = 0; j < BOARD_HEIGHT; j++)
      if (board_[i][j] == true)
        count++;

    if (count == 10)
    {
      for (int k = 0; k < BOARD_HEIGHT; k++)
        board_[i][k] = false;

      for (int a = i; a > 0; a--)
      {
        for (int b = 0; b < BOARD_HEIGHT; b++)
          board_[a][b] = board_[a - 1][b];
      }

      for (int k = 0; k < BOARD_HEIGHT; k++)
        board_[0][k] = false;
    }

    count = 0;
    score--;
  }
}

// 게임 화면을 그린다.
void Game::draw()
{
  console::drawBox(0, 0, 12, 22);
  console::drawBox(14, 0, 20, 8);
  console::drawBox(22, 0, 28, 8);
  console::draw(15, 0, "Next");
  console::draw(23, 0, "Hold");
  console::draw(BOARD_WIDTH / 2, BOARD_HEIGHT + 2, score + "lines left");

  currentTetromino->drawAt(BLOCK_STRING, x, y++);
  nextTetromino->drawAt(BLOCK_STRING, 15, 1);
  holdTetromino->drawAt(BLOCK_STRING, 24, 1);

  for (int i = 0; i < BOARD_WIDTH; i++)
    for (int j = 0; j < BOARD_HEIGHT; j++)
      if (board_[i][j] == true)
        console::draw(i,j,BLOCK_STRING);

  // 시간
}

// 게임 루프가 종료되어야 하는지 여부를 반환한다.
bool Game::shouldExit()
{
}

Game::Game()
{
  score = 40;
  x = 5, y = 0;

  for (int x = 0; x < BOARD_WIDTH; ++x)
  {
    for (int y = 0; y < BOARD_HEIGHT; ++y)
    {
      board_[x][y] = false;
    }
  }

  ar[0] = new Tetromino("T", 3, "XOXOOOXXX");
  ar[1] = new Tetromino("I", 4, "XXXXOOOOXXXXXXXX");
  ar[2] = new Tetromino("O", 2, "OOOO");
  ar[3] = new Tetromino("S", 3, "XOOOOXXXX");
  ar[4] = new Tetromino("Z", 3, "OOXXOOXXX");
  ar[5] = new Tetromino("J", 3, "OXXOOOXXX");
  ar[6] = new Tetromino("L", 3, "XXOOOOXXX");

  currentTetromino = ar[rand() % 7];
  nextTetromino = ar[rand() % 7];
  holdTetromino = nullptr;
}
