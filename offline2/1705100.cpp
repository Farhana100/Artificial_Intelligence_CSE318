#include <bits/stdc++.h>
#include <stdlib.h>
#define BOARD_H 24
#define SCALE_X 2
#define CELL_B 5
#define CELL_H 10
#define DELAY for(long long i = 0; i < 1000000000ll; i++);\
#define WAIT getchar();
#define EMPTY_SLOT  ' '
#define STONE 'o'
#define LAST_MOVED_STONE 178
#define DEPTH 2
#define DEPTH1 3
#define DEPTH2 7

using namespace std;

#include "Stone.cpp"
#include "Cell.cpp"
#include "testAI.cpp"
#include "Mancala.cpp"

int main()
{
    Mancala mancala;
    mancala.gameInit(6, 4);
   // mancala.gameStart_withAI2(1);
   // mancala.gameStart_withAI(1);
    mancala.gameStart_AIvsAI(1, 6);

    return 0;

}
