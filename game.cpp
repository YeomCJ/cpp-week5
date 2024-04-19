#include "game.h"
#include "console/console.h"
#include <iostream>
#include <random>
#include <string>
#include <chrono>

using namespace console;

// 벽에서 회전했을 때 오류
bool Game::leftWall()
{
  int s = currentTetromino->size();
  for (int j = 0; j < s; j++)
  {
    if (currentTetromino->check(j, 0))
      if (x == 1)
        return false;

    if (currentTetromino->check(j, 1))
      if (x == 0)
        return false;

    if (currentTetromino->check(j, 2)) // I 블록
      if (x == -1)
        return false;
  }
  return true;
}

bool Game::rightWall()
{
  int s = currentTetromino->size();
  for (int j = 0; j < s; j++)
  {
    if (currentTetromino->check(j, s - 1))
      if (x + s == 11)
        return false;

    if (currentTetromino->check(j, s - 2))
      if (x + s == 12)
        return false;

    if (currentTetromino->check(j, s - 3)) // I 블록
      if (x + s == 13)
        return false;
  }
  return true;
}

// 바닥 수정 해야함 오류 있음
bool Game::floorWall()
{
  int s = currentTetromino->size();
  int count = 0;
  for (int j = 0; j < BOARD_WIDTH; j++)
    if (board_[j][19] == true)
      count++;
  if (count != 10)
  {
    for (int j = 0; j < s; j++)
    {
      if (currentTetromino->check(s - 1, j))
        if (y + s - 1 == 20)
          return false;

      if (currentTetromino->check(s - 2, j))
        if (y + s - 2 == 20)
          return false;

      if (currentTetromino->check(s - 3, j)) // I 블록
        if (y + s == 23)
          return false;
    }
  }
  return true;
}

// 게임의 한 프레임을 처리
void Game::update()
{
  int s = currentTetromino->size();
  /*
   회전 판별
  int zc = 0;
  int xc = 0;
  */

  // 키 입력에 따른 처리
  if (console::key(Key::K_UP)) // 하드드롭
  {
  }

  else if (console::key(Key::K_DOWN)) // 연속으로 누르면 자동으로 내려가는 로직이 망가짐
  {
    if (floorWall())
      y++;
  }
  else if (console::key(Key::K_LEFT))
  {
    if (leftWall())
      x--;
  }
  else if (console::key(Key::K_RIGHT))
  {
    if (rightWall())
      x++;
  }

  else if (console::key(Key::K_ESC)) // 종료
  {
    for (int i = 0; i < 7; i++)
    {
      delete ar[i];
      delete nar[i];
    }
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
      {
        int i;
        for (i = 0; i < 7; i++)
          if (ar[i]->name()[0] == nextTetromino->name()[0])
            break;

        int j;
        for (j = 0; j < 7; j++)
          if (ar[j]->name()[0] == currentTetromino->name()[0])
            break;
        holdTetromino = nar[j];
        delete currentTetromino;
        currentTetromino = ar[i];
        delete nextTetromino;
        nextTetromino = nar[rand() % 7];
      }
      else
      {
        int i;
        for (i = 0; i < 7; i++)
          if (ar[i]->name()[0] == currentTetromino->name()[0])
            break;

        int j;
        for (j = 0; j < 7; j++)
          if (ar[j]->name()[0] == holdTetromino->name()[0])
            break;

        delete holdTetromino;
        holdTetromino = nar[i];
        delete currentTetromino;
        currentTetromino = ar[j];
      }
      x = 5;
      y = 1;
      check++;
    }
  }
  else if (console::key(Key::K_Z)) // 반시계 회전
  {
    currentTetromino->rotatedCCW();
    if (!leftWall())
    {
      currentTetromino->rotatedCCW();
      for (int j = 0; j < s; j++)
        if (currentTetromino->check(0, j))
        {
          currentTetromino->rotatedCW();
          break;
        }
    }
    else if (!rightWall())
    {
      currentTetromino->rotatedCCW();
      for (int j = 0; j < s; j++)
        if (currentTetromino->check(s - 1, j))
        {
          currentTetromino->rotatedCW();
          break;
        }
    }
    else
      currentTetromino->rotatedCCW();
  }
  else if (console::key(Key::K_X)) // 시계 회전
  {
    if (!leftWall())
    {
      currentTetromino->rotatedCW();
      for (int j = 0; j < s; j++)
        if (currentTetromino->check(0, j))
        {
          currentTetromino->rotatedCCW();
          break;
        }
    }
    else if (!rightWall())
    {
      currentTetromino->rotatedCW();
      for (int j = 0; j < s; j++)
        if (currentTetromino->check(s - 1, j))
        {
          currentTetromino->rotatedCCW();
          break;
        }
    }
    else
      currentTetromino->rotatedCW();
  }

  // 프레임 속도 맞추기
  moveTimer -= 2;
  if (moveTimer == 0)
  {
    moveTimer = DROP_DELAY;
    y++;
  }

  // 블록 위에 쌓기
  if (board_[x - 1][y - 1] == true)
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
                  board_[x + j][y + i] = true;

            int i;
            for (i = 0; i < 7; i++)
              if (ar[i]->name()[0] == nextTetromino->name()[0])
                break;
            delete currentTetromino;
            currentTetromino = ar[i];
            delete nextTetromino;
            nextTetromino = nar[rand() % 7];
            x = 5;
            y = 1;
            check = 0;
            z = -1;
            break;
          }
      }
      if (z == -1)
        break;
    }
  }

  // 바닥에 쌓기
  if (!floorWall())
  {
    for (int i = 0; i < s; i++)
      for (int j = 0; j < s; j++)
        if (currentTetromino->check(i, j))
          board_[x + j - 1][y + i - 1] = true;

    //-------------------------------------
    int q;
    for (q = 0; q < 7; q++)
      if (ar[q]->name()[0] == nextTetromino->name()[0])
        break;
    delete currentTetromino;
    currentTetromino = ar[q];
    delete nextTetromino;
    nextTetromino = nar[rand() % 7];
    x = 5;
    y = 1;
    check = 0;
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
  int s = currentTetromino->size();
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
  //

  console::drawBox(0, 0, 11, 21);
  console::drawBox(12, 0, 17, 6);
  console::drawBox(18, 0, 23, 6);
  console::draw(13, 0, "Next");
  console::draw(19, 0, "Hold");
  console::draw(0, 22, std::to_string(score) + " lines left");

  for (int i = 18; i >= 0; i--)
  {
    if (board_[x][19] == true)
    {
      if (board_[x][i] == false)
      {
        currentTetromino->drawAt(SHADOW_STRING, x, i);
        break;
      }
    }
    else
    {
      currentTetromino->drawAt(SHADOW_STRING, x, 19);
      break;
    }
  }

  currentTetromino->drawAt(BLOCK_STRING, x, y);
  nextTetromino->drawAt(BLOCK_STRING, 13, 1);
  if (holdTetromino != nullptr)
    holdTetromino->drawAt(BLOCK_STRING, 19, 1);

  for (int i = 0; i < BOARD_WIDTH; i++)
    for (int j = 0; j < BOARD_HEIGHT; j++)
      if (board_[i][j] == true)
      {
        console::draw(i + 1, j + 1, BLOCK_STRING);
      }

  if (score == 0)
  {
    console::draw(4, 10, "You Win");
    console::draw(4, 11, std::to_string(minutes.count()) + ":" + std::to_string(seconds.count()) + ":" + std::to_string(milliseconds.count()));
  }
}

// 게임 루프가 종료되어야 하는지 여부를 반환한다.
bool Game::shouldExit()
{

  if (score == 0)
  {
    for (int i = 0; i < 7; i++)
    {
      delete ar[i];
      delete nar[i];
    }
    delete holdTetromino;
    delete nextTetromino;
    delete currentTetromino;
    return true;
  }

  return false;
}

Game::Game()
{
  score = 40;
  x = 5, y = 1;

  for (int i = 0; i < BOARD_WIDTH; ++i)
  {
    for (int j = 0; j < BOARD_HEIGHT; ++j)
    {
      board_[i][j] = false;
    }
  }

  ar[0] = new Tetromino("T", 3, "XOXOOOXXX");
  ar[1] = new Tetromino("I", 4, "XXXXOOOOXXXXXXXX");
  ar[2] = new Tetromino("O", 2, "OOOO");
  ar[3] = new Tetromino("S", 3, "XOOOOXXXX");
  ar[4] = new Tetromino("Z", 3, "OOXXOOXXX");
  ar[5] = new Tetromino("J", 3, "OXXOOOXXX");
  ar[6] = new Tetromino("L", 3, "XXOOOOXXX");

  nar[0] = new Tetromino("Tn", 3, "XOXOOOXXX");
  nar[1] = new Tetromino("In", 4, "XXXXOOOOXXXXXXXX");
  nar[2] = new Tetromino("On", 2, "OOOO");
  nar[3] = new Tetromino("Sn", 3, "XOOOOXXXX");
  nar[4] = new Tetromino("Zn", 3, "OOXXOOXXX");
  nar[5] = new Tetromino("Jn", 3, "OXXOOOXXX");
  nar[6] = new Tetromino("Ln", 3, "XXOOOOXXX");

  currentTetromino = ar[rand() % 7];
  nextTetromino = nar[rand() % 7];
  holdTetromino = nullptr;
}
