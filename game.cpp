#include "game.h"
#include "console/console.h"
#include <iostream>
#include <random>
#include <string>
#include <chrono>

using namespace console;

// 게임의 한 프레임을 처리
void Game::update()
{
  int s = currentTetromino->size();

  // 키 입력에 따른 처리
  if (true)
  {
    if (console::key(Key::K_UP)) // 하드드롭
    {
      for (int i = 0; i < 20; i++)
        if (board_[x][i] == true)
        {
          y = i + 1;
          break;
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
    else if (console::key(Key::K_ESC)) // 종료
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
  }

  // 블록 위에 쌓기
  if (board_[x][y - 1] == true)
  {
    for (int k = s - 1; k >= 0; k--)
    {
      int z = 0; // for문 탈출용
      for (int t = 1; t <= s; t++)
      {
        if (currentTetromino->check(s - t, k))
          if (board_[x + (s - t)][y + k] == true)
          {
            for (int i = s - 1; i >= 0; i--)
              for (int j = s - 1; j >= 0; j--)
                if (currentTetromino->check(i, j))
                  board_[x + i][y + j] = true;

            currentTetromino = nextTetromino;
            nextTetromino = ar[rand() % 7];
            check = 0;
            z = -1;
            break;
          }
      }
      if (z == -1)
        break;
    }
  }
  // 맨 바닥에 쌓기
  else
  { 
    for (int k = s - 1; k >= 0; k--)
    {
      int z = 0; // for문 탈출용
      for (int t = 1; t <= s; t++)
      {
        if (currentTetromino->check(s - t, k))
        {
            for (int i = s - 1; i >= 0; i--)
              for (int j = s - 1; j >= 0; j--)
                if (currentTetromino->check(i, j))
                  board_[x + i][y + j] = true;

        }
      }
    }
  }

  // 한 줄 채우면 ~
  for (int i = BOARD_HEIGHT - 1; i >= 0; i--)
  {
    int count = 0;
    for (int j = 0; j < BOARD_WIDTH; j++)
      if (board_[j][i] == true)
        count++;

    if (count == 10)
    {
      for (int k = 0; k < BOARD_WIDTH; k++)
        board_[k][i] = false;
      score--;

      // 칸 내리기
      for (int a = i; a > 0; a--)
      {
        for (int b = 0; b < BOARD_WIDTH; b++)
          board_[b][a] = board_[b][a - 1];
      }

      // 맨 윗줄
      for (int k = 0; k < BOARD_WIDTH; k++)
        board_[k][0] = false;
    }
  }
}

// 게임 화면을 그린다.
void Game::draw()
{
  // 시간 출력
  static auto lastTime = std::chrono::steady_clock::now();

  auto currentTime = std::chrono::steady_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime);
  auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
  duration -= minutes;
  auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
  duration -= seconds;
  auto milliseconds = duration;

  console::draw(4, 23, std::to_string(minutes.count()) + ":" + std::to_string(seconds.count()) + ":" + std::to_string(milliseconds.count()));

  lastTime = currentTime;

  console::drawBox(0, 0, 12, 22);
  console::drawBox(14, 0, 20, 8);
  console::drawBox(22, 0, 28, 8);
  console::draw(15, 0, "Next");
  console::draw(23, 0, "Hold");
  console::draw(BOARD_WIDTH / 2, BOARD_HEIGHT + 2, score + "lines left");

  currentTetromino->drawAt(BLOCK_STRING, x, y++);
  nextTetromino->drawAt(BLOCK_STRING, 15, 1);
  if (holdTetromino != nullptr)
    holdTetromino->drawAt(BLOCK_STRING, 24, 1);

  for (int i = 0; i < 20; i++)
    if (board_[x][i] == true)
    {
      currentTetromino->drawAt(SHADOW_STRING, x, i + 1);
      break;
    }

  for (int i = 0; i < BOARD_WIDTH; i++)
    for (int j = 0; j < BOARD_HEIGHT; j++)
      if (board_[i][j] == true)
        console::draw(i, j, BLOCK_STRING);

  if (score == 0)
  {
    console::draw(4, 10, "You Win");
    console::draw(4, 11, std::to_string(minutes.count()) + ":" + std::to_string(seconds.count()) + ":" + std::to_string(milliseconds.count()));
  }
}

// 게임 루프가 종료되어야 하는지 여부를 반환한다.
bool Game::shouldExit()
{
  if (board_[4][0] == true || board_[5][0])
  {
    console::draw(4, 10, "You Lost");
    return true;
  }

  if (score == 0)
  {
    return true;
  }

  return false;
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
