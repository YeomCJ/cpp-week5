#include "tetromino.h"
#include "console/console.h"

using namespace console;

// 사이즈와 shape 문자열을 통해 생성한다.
// 문자열은 size * size 길이의 문자열이 주어진다.

// 블록이 존재한다면 O를 통해 표시하고 아니라면 X를 통해 표시한다.
// 문자열은 개행 없이 일렬로 나열되어 주어진다.
Tetromino::Tetromino(std::string name, int size, std::string shape)
{
    name_ = name;
    size_ = size;

    for (int i = 0; i < MAX_SIZE; i++)
        for (int j = 0; j < MAX_SIZE; j++)
            shape_[i][j] = false;

    int a = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (shape[a] == 'O')
                shape_[i][j] = true;
            a++;
        }
    }
    // 회전되지 않은 원래 테트로미노 객체를 저장하는 포인터
    // Tetromino *original_;
    original_ = this;
}

// 시계 방향으로 회전한 모습의 테트로미노 객체를 반환한다.
Tetromino Tetromino::rotatedCW()
{
    // I 블록을 제외한 나머지 블록
    if (size_ != 4)
    {
        bool tmp1 = shape_[0][2], tmp2 = shape_[0][1];

        shape_[0][2] = shape_[0][0];
        shape_[0][1] = shape_[1][0];
        shape_[0][0] = shape_[2][0];
        shape_[1][0] = shape_[2][1];
        shape_[2][0] = shape_[2][2];
        shape_[2][1] = shape_[1][2];
        shape_[2][2] = tmp1;
        shape_[1][2] = tmp2;
    }
    else
    {
        // I 가로
        if (shape_[0][1] == false && shape_[0][2] == false)
        {
            // 2번째 줄이 가로 "xxxxOOOOxxxxxxxx"
            if (shape_[1][0] == true)
            {
                for (int i = 0; i < 4; i++)
                    shape_[1][i] = false;
                for (int i = 0; i < 4; i++)
                    shape_[i][2] = true;
            }
            else
            {
                for (int i = 0; i < 4; i++)
                    shape_[2][i] = false;
                for (int i = 0; i < 4; i++)
                    shape_[i][1] = true;
            }
        }
        // 세로
        else
        {
            // 2번째 ~ "xOxxxOxxxOxxxOxx"
            if (shape_[0][1] == true)
            {
                for (int i = 0; i < 4; i++)
                    shape_[i][1] = false;
                for (int i = 0; i < 4; i++)
                    shape_[1][i] = true;
            }
            else
            {
                for (int i = 0; i < 4; i++)
                    shape_[i][2] = false;
                for (int i = 0; i < 4; i++)
                    shape_[2][i] = true;
            }
        }
    }
}

// 반시계 방향으로 회전한 모습의 테트로미노 객체를 반환한다.
Tetromino Tetromino::rotatedCCW()
{
    // I 블록을 제외한 나머지 블록
    if (size_ != 4)
    {
        bool tmp1 = shape_[0][0], tmp2 = shape_[0][1];

        shape_[0][0] = shape_[0][2];
        shape_[0][2] = shape_[2][2];
        shape_[2][2] = shape_[2][0];
        shape_[2][0] = tmp1;

        shape_[0][1] = shape_[1][2];
        shape_[1][2] = shape_[2][1];
        shape_[2][1] = shape_[1][0];
        shape_[1][0] = tmp2;
    }
    else
    {
        // I 가로
        if (shape_[0][1] == false && shape_[0][2] == false)
        {
            // 2번째 줄이 가로 "xxxxOOOOxxxxxxxx"
            if (shape_[1][0] == true)
            {
                for (int i = 0; i < 4; i++)
                    shape_[1][i] = false;
                for (int i = 0; i < 4; i++)
                    shape_[i][1] = true;
            }
            else
            {
                for (int i = 0; i < 4; i++)
                    shape_[2][i] = false;
                for (int i = 0; i < 4; i++)
                    shape_[i][2] = true;
            }
        }
        // 세로
        else
        {
            // 2번째 ~ "xOxxxOxxxOxxxOxx"
            if (shape_[0][1] == true)
            {
                for (int i = 0; i < 4; i++)
                    shape_[i][1] = false;
                for (int i = 0; i < 4; i++)
                    shape_[2][i] = true;
            }
            else
            {
                for (int i = 0; i < 4; i++)
                    shape_[i][2] = false;
                for (int i = 0; i < 4; i++)
                    shape_[1][i] = true;
            }
        }
    }
}

// 화면의 x, y 위치에 s 문자열로  테트로미노를 그린다
void Tetromino::drawAt(std::string s, int x, int y)
{
    for (int i = 0; i < size_; i++)
    {
        for (int j = 0; j < size_; j++)
        {
            if (shape_[i][j] == true)
                draw(x + j, y + i, s);
        }
    }
}

Tetromino Tetromino::T("T", 3, "XOXOOOXXX");
Tetromino Tetromino::I("I", 4, "XXXXOOOOXXXXXXXX");
Tetromino Tetromino::O("O", 2, "OOOO");
Tetromino Tetromino::S("S", 3, "XOOOOXXXX");
Tetromino Tetromino::Z("Z", 3, "OOXXOOXXX");
Tetromino Tetromino::J("J", 3, "OXXOOOXXX");
Tetromino Tetromino::L("L", 3, "XXOOOOXXX");
