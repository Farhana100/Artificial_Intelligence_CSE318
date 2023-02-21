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
#define DEPTH1 10
#define DEPTH2 3

using namespace std;

#include "Stone.cpp"
#include "Cell.cpp"
#include "AI.cpp"
#include "Mancala.cpp"

int main()
{
    int count1 = 0, count2 = 0, countdraw = 0;

    Mancala mancala;

    for(int i = 0; i < 100; i++){
        mancala.gameInit(6, 4);
        mancala.gameStart_AIvsAI(1, 6);
        if(mancala.winner == 1)count1++;
        else if(mancala.winner == 2)count2++;
        else countdraw++;
    }

    cout << 1 << " : " << 6 << " -> " << count1 << " : " << count2 << endl;
    cout << "Draw: " << countdraw << endl;
    return 0;

}


/**
depth -> 3:7
1 : 6 -> 0 : 100
Draw: 0

depth -> 10:3
1 : 6 -> 0 : 100
Draw: 0




*/
