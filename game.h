
#ifndef GAME_H
#define GAME_H

#include "tetromino.h"
#include "iostream"
#include <chrono>
#include <sstream>
#include <iomanip>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define LINES 40

#define DROP_DELAY 60

class Game {
private:
  // 게임 판을 나타내는 배열
  // board[x][y]가 true 인 경우 x, y 위치에 고정된 블록이 존재하는 것을 의미한다
  bool board_[BOARD_WIDTH][BOARD_HEIGHT];
  
  int score;
  int x,y;
  int check = 0;

  int moveTimer = DROP_DELAY;

  std::chrono::steady_clock::time_point startTime_ = std::chrono::steady_clock::now(); // 게임 시작 시간

  Tetromino currentTetromino = Tetromino::T;
  Tetromino nextTetromino = Tetromino::T;
  Tetromino holdTetromino = Tetromino::T;

  Tetromino ar[7] = {Tetromino::I,Tetromino::O,Tetromino::T,
                    Tetromino::S,Tetromino::Z,Tetromino::J,Tetromino::L};

  bool leftWall();
  bool rightWall();
  bool floorWall();
  
  int underBlock();

  void drawShadow();

  std::string formatPlayTime();

public:
  // 게임의 한 프레임을 처리한다.
  void update();

  // 게임 화면을 그린다.
  void draw();

  // 게임 루프가 종료되어야 하는지 여부를 반환한다.
  bool shouldExit();

  Game();
};
#endif