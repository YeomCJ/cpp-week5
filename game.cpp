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
  int index = rand() % 7;

  Tetromino *currentTetromino;


  if (console::key(Key::K_UP))
  {
  }
  else if (console::key(Key::K_DOWN))
  {
  }
  else if (console::key(Key::K_LEFT))
  {
  }
  else if (console::key(Key::K_RIGHT))
  {
  }
  else if (console::key(Key::K_ENTER))
  {
  }
  else if (console::key(Key::K_ESC))
  {
  }
  else if (console::key(Key::K_SPACE))
  {
  }
  else if (console::key(Key::K_Z))
  {
  }
  else if (console::key(Key::K_X))
  {
  }

  // 한 줄 채우면 ~
  for (int i = BOARD_HEIGHT - 1; i >= 0; i--)
  {
    int count = 0;
    for (int j = 0; j < BOARD_WIDTH; j++)
      if (board_[i][j] == true)
        count++;

    if (count == 10)
    {
      for (int k = 0; k < BOARD_WIDTH; k++)
        board_[i][k] = false;

      for (int a = i; a > 0; a--)
      {
        for (int b = 0; b < BOARD_WIDTH; b++)
          board_[a][b] = board_[a - 1][b];
      }

      for (int k = 0; k < BOARD_WIDTH; k++)
        board_[0][k] = false;
    }

    count = 0;
    score--;
  }
}

// 게임 화면을 그린다.
void Game::draw()
{
  auto end = chrono::steady_clock::now();

  console::drawBox(0, 0, 12, 22);
  console::drawBox(14, 0, 20, 8);
  console::drawBox(22, 0, 28, 8);
  console::draw(15, 0, "Next");
  console::draw(23, 0, "Hold");
  console::draw(BOARD_WIDTH / 2, BOARD_HEIGHT + 2, score + "lines left");
  // 시간
}

// 게임 루프가 종료되어야 하는지 여부를 반환한다.
bool Game::shouldExit()
{
}

Game::Game()
{
  score = 40;

  auto start = chrono::steady_clock::now();
}
