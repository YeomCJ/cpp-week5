#include "game.h"
#include "console/console.h"
#include <iostream>
#include <random>
#include <string>

using namespace console;

// 벽에서 회전했을 때 오류
bool Game::leftWall()
{
  int s = currentTetromino.size();
  for (int j = 0; j < s; j++)
  {
    if (currentTetromino.check(j, 0))
      if (x == 1)
        return false;

    if (currentTetromino.check(j, 1))
      if (x == 0)
        return false;

    if (currentTetromino.check(j, 2)) // I 블록
      if (x == -1)
        return false;
  }
  
  return true;
}

bool Game::rightWall()
{
  int s = currentTetromino.size();
  for (int j = 0; j < s; j++)
  {
    if (currentTetromino.check(j, s - 1))
      if (x + s == 11)
        return false;

    if (currentTetromino.check(j, s - 2))
      if (x + s == 12)
        return false;

    if (currentTetromino.check(j, s - 3)) // I 블록
      if (x + s == 13)
        return false;
  }
  return true;
}

// 바닥 수정 해야함 오류 있음
bool Game::floorWall()
{
  int s = currentTetromino.size();
  int count = 0;
  for (int j = 0; j < BOARD_WIDTH; j++)
    if (board_[j][19] == true)
      count++;
  if (count != 10)
  {
    for (int j = 0; j < s; j++)
    {
      if (currentTetromino.check(s - 1, j))
        if (y + s - 1 == 20)
          return false;

      if (currentTetromino.check(s - 2, j))
        if (y + s - 2 == 20)
          return false;

      if (currentTetromino.check(s - 3, j)) // I 블록
        if (y + s == 23)
          return false;
    }
  }
  return true;
}

int Game::underBlock() // up, shadow
{
  int s = currentTetromino.size();
  int minDistance = 19;
  for (int i = s - 1; i >= 0; i--)
  {
    for (int j = 0; j < s; j++)
    {
      if (currentTetromino.check(i, j))
      {
        for (int k = 19; k >= y; k--)
          if (board_[x + j - 1][k])
          {
            if (k - i <= minDistance) {
              minDistance = k - i;
            }
          }
      }
    }
  }
  // ----------- 바닥 ------------
  if (s == 3)
  {
    for (int j = 0; j < s; j++)

      if (currentTetromino.check(s - 1, j))
      {
        if (minDistance < 18)
          return minDistance;
        return 18;
      }
  }
  else if (s == 4)
  {
    for (int j = 0; j < s; j++)
    {
      if (currentTetromino.check(s - 1, j))
      {
        if (minDistance < 17)
          return minDistance;
        return 17;
      }
      if (currentTetromino.check(s - 2, j))
      {
        if (minDistance < 16)
          return minDistance;
        return 16;
      }
    }
  }
  return minDistance;
}

// 게임의 한 프레임을 처리
void Game::update()
{
  int s = currentTetromino.size();

  // 키 입력에 따른 처리
  if (console::key(Key::K_UP))
  {
    y = underBlock();
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
    exit(0);
  }

  else if (console::key(Key::K_SPACE)) // 홀드
  {
    if (check == 0)
    {
      if (holdTetromino.size() == 0)
      {
        holdTetromino = currentTetromino;
        currentTetromino = nextTetromino;
        nextTetromino = ar[rand() % 7];
      }
      else
      {
        Tetromino tmp = holdTetromino;
        holdTetromino = currentTetromino;
        currentTetromino = tmp;
      }
      x = 4;
      y = 1;
      check++;
    }
  }
  else if (console::key(Key::K_Z)) // 반시계 회전
  {

    if (!leftWall())
    {
      currentTetromino = currentTetromino.rotatedCCW();
      for (int j = 0; j < s; j++)
        if (currentTetromino.check(0, j))
        {
          currentTetromino = currentTetromino.rotatedCW();
          break;
        }
    }
    else if (!rightWall())
    {
      currentTetromino = currentTetromino.rotatedCCW();
      for (int j = 0; j < s; j++)
        if (currentTetromino.check(s - 1, j))
        {
          currentTetromino = currentTetromino.rotatedCW();
          break;
        }
    }
    else
      currentTetromino = currentTetromino.rotatedCCW();
  }
  else if (console::key(Key::K_X)) // 시계 회전
  {
    if (!leftWall())
    {
      currentTetromino = currentTetromino.rotatedCW();
      for (int j = 0; j < s; j++)
        if (currentTetromino.check(0, j))
        {
          currentTetromino = currentTetromino.rotatedCCW();
          break;
        }
    }
    else if (!rightWall())
    {
      currentTetromino = currentTetromino.rotatedCW();
      for (int j = 0; j < s; j++)
        if (currentTetromino.check(s - 1, j))
        {
          currentTetromino = currentTetromino.rotatedCCW();
          break;
        }
    }
    else
      currentTetromino = currentTetromino.rotatedCW();
  }

  // 프레임 속도 맞추기
  moveTimer--;
  if (moveTimer == 0)
  {
    moveTimer = DROP_DELAY;
    y++;
  }

  // 블록 위에 쌓기
  for (int i = s - 1; i >= 0; i--)
  {
    int z = 0;
    for (int j = 0; j < s; j++)
    {
      if (currentTetromino.check(i, j))
      {
        if (board_[x + j - 1][y + i])
        {
          for (int k = s - 1; k >= 0; k--)
            for (int l = 0; l < s; l++)
              if (currentTetromino.check(k, l))
                board_[x + l - 1][y + k - 1] = true;

          currentTetromino = nextTetromino;
          nextTetromino = ar[rand() % 7];
          x = 4;
          y = 1;
          check = 0;
          z = -1;
          break;
        }
      }
    }
    if (z != 0)
      break;
  }

  // 바닥에 쌓기
  if (!floorWall())
  {
    for (int i = 0; i < s; i++)
      for (int j = 0; j < s; j++)
        if (currentTetromino.check(i, j))
          board_[x + j - 1][y + i - 1] = true;

    currentTetromino = nextTetromino;
    nextTetromino = ar[rand() % 7];
    x = 4;
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

void Game::drawShadow()
{
  int s = currentTetromino.size();

  int shadowY = underBlock();
  for (int i = 0; i < s; ++i)
  {
    for (int j = 0; j < s; ++j)
    {
      if (currentTetromino.check(i, j))
      {
        currentTetromino.drawAt(SHADOW_STRING, x, shadowY);
        return;
      }
    }
  }
}

std::string Game::formatPlayTime()//플레이타임 계산
{
    auto currentTime = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime_);

    int minutes = std::chrono::duration_cast<std::chrono::minutes>(duration).count() % 60;
    int seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count() % 60;
    int milliseconds = duration.count() % 1000;

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << minutes << ":"//분
        << std::setfill('0') << std::setw(2) << seconds << "."//초
        << std::setfill('0') << std::setw(2) << milliseconds / 10;

    return oss.str();
}
// 게임 화면을 그린다.
void Game::draw()
{
  int s = currentTetromino.size();
  // 시간 출력
  console::draw(2,23,formatPlayTime());

  console::drawBox(0, 0, 11, 21);
  console::drawBox(12, 0, 17, 6);
  console::drawBox(18, 0, 23, 6);
  console::draw(13, 0, "Next");
  console::draw(19, 0, "Hold");
  console::draw(0, 22, std::to_string(score) + " lines left");

  drawShadow();

  currentTetromino.drawAt(BLOCK_STRING, x, y);
  nextTetromino.drawAt(BLOCK_STRING, 13, 1);
  if (holdTetromino.size() != 0)
    holdTetromino.drawAt(BLOCK_STRING, 19, 1);

  for (int i = 0; i < BOARD_WIDTH; i++)
    for (int j = 0; j < BOARD_HEIGHT; j++)
      if (board_[i][j] == true)
      {
        console::draw(i + 1, j + 1, BLOCK_STRING);
      }

  if (score == 0)
  {
    console::draw(2, 10, "You Win");
    console::draw(2,11,formatPlayTime());
    return;
  }

  if (board_[4][1] || board_[5][1])
  {
    console::draw(2, 10, "You Lost");
    return;
  }
}

// 게임 루프가 종료되어야 하는지 여부를 반환한다.
bool Game::shouldExit()
{
  if (board_[4][1] || board_[5][1])
  {

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
  score = LINES;
  x = 4, y = 1;

  for (int i = 0; i < BOARD_WIDTH; ++i)
  {
    for (int j = 0; j < BOARD_HEIGHT; ++j)
    {
      board_[i][j] = false;
    }
  }

  currentTetromino = ar[rand() % 7];
  nextTetromino = ar[rand() % 7];
  holdTetromino = Tetromino("Q", 0, "1");
}
