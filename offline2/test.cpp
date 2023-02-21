#include <bits/stdc++.h>
#include <stdlib.h>
#define BOARD_H 24
#define SCALE_X 2
#define CELL_B 5
#define CELL_H 10
#define DELAY for(long long i = 0; i < 10000000ll; i++);
#define EMPTY_SLOT  ' '
#define STONE 'o'
#define LAST_MOVED_STONE 178

using namespace std;

class Stone{
    int posX;
    int posY;
    int celli;
    int cellj;

public:
    char img;
    Stone()
    {
        this->posX = 0;
        this->posY = 0;
        this->celli = 0;
        this->cellj = 0;
        img = STONE;
    }

    void setPosX(int posX){this->posX = posX;}
    void setPosY(int posY){this->posY = posY;}
    int getPosX(){return posX;}
    int getPosY(){return posY;}

    void setCelli(int Celli){this->celli = celli;}
    void setCellj(int Cellj){this->cellj = cellj;}
    int getCelli(){return celli;}
    int getCellj(){return cellj;}

    void move(int x, int y, int celli, int cellj)
    {
        this->posX = x;
        this->posY = y;
        this->celli = celli;
        this->cellj = cellj;
    }
};

class Cell{

    int startX;
    int startY;
    int cell_h;
    int cell_b;
    vector <Stone> stones;
    vector < vector <bool> > grid;

public:

    Cell(int startX, int startY, int cell_h, int cell_b)
    {
        this->startX = startX;
        this->startY = startY;
        this->cell_h = cell_h;
        this->cell_b = cell_b;
        for(int i = 0; i < cell_h; i++){
            vector <bool> temp(cell_b, 0);
            grid.push_back(temp);
        }
    }

    bool insertStone(Stone st)
    {
        if(stones.size() == cell_b*cell_h) return false;

        for(int i = 0; i < grid.size(); i++){
            for(int j = 0; j < grid[i].size(); j++){
                if(!grid[i][j]){
                    // empty place
                    st.move(startX + j*(SCALE_X + 1), startY + i, i, j);
                    stones.push_back(st);
                    grid[i][j] = 1;
                    return true;
                }
            }
        }
    }

    bool takeStone(Stone &st)
    {
        if(stones.empty()) return false;
        st = stones.back();
        grid[st.getCelli()][st.getCellj()] = 0;
        stones.pop_back();
        return true;
    }

    int getNumberOfStones(){return stones.size();}
    vector <Stone>& getStones(){return stones;}
};


class Mancala{
    vector <vector <char> > board;
    int numberOfCells;
    int numberOfStones;

    // player 1
    vector <Cell> player1cells;
    vector <int> player1moveStack;
    int player1score;
    Cell *homeCell1;

    // player 2
    vector <Cell> player2cells;
    vector <int> player2moveStack;
    int player2score;
    Cell *homeCell2;

    bool gameState;     // true -> game on, false -> game ended
    void drawBoard();
public:
    Mancala();
    void showBoard();
    void gameInit(int n, int s);
    void gameStart();
    bool player1move(int cellNo);
    bool player2move(int cellNo);
    bool gameover();
};

Mancala::Mancala()
{
    numberOfCells = 6;
    numberOfStones = 4;
    gameState = 1;
    player1score = 0;
    player2score = 0;
    homeCell1 = NULL;
    homeCell2 = NULL;
}

void Mancala::drawBoard()
{
    board.clear();
    int i, j, k, l;
    vector < char> temp((numberOfCells+2)*CELL_B*(SCALE_X+1) + CELL_B + 3, '_');
    board.push_back(temp);
    for(int i = 0; i < BOARD_H; i++){
        vector < char > temp((numberOfCells+2)*CELL_B*(SCALE_X+1) + numberOfCells + 3, EMPTY_SLOT);
        for(j = k = 0; j < temp.size(); j++, k++){
            if(i >= CELL_H and i < BOARD_H-CELL_H and j >= CELL_B*(SCALE_X+1)+2 and j < temp.size()-(CELL_B*(SCALE_X+1)+2)){
                if(i == CELL_H or i == BOARD_H-CELL_H-1){
                    temp[j] = '_';
                }
                else if(i == BOARD_H/2 and j >= ((temp.size()/2) -3) and j < ((temp.size()/2) + 4 ) ){
                    if(temp[j] == EMPTY_SLOT){
                        temp[j] = 'M';
                        temp[j+1] = 'A';
                        temp[j+2] = 'N';
                        temp[j+3] = 'C';
                        temp[j+4] = 'A';
                        temp[j+5] = 'L';
                        temp[j+6] = 'A';
                    }
                }
                else temp[j] = ' ';
                continue;
            }
            if( j%( ( CELL_B*(SCALE_X + 1) ) + 1 ) == 0 ){
                temp[j] = '|';
                k--;
                continue;
            }
            if( (k% ( CELL_B*(SCALE_X + 1) ) ) %(SCALE_X + 1) == 1 )temp[j] = ' ';
            if( (k% ( CELL_B*(SCALE_X + 1) ) ) %(SCALE_X + 1) == 2 )temp[j] = ' ';
        }
        temp[temp.size()-1] = '|';
        board.push_back(temp);
    }
    board.push_back(temp);
}

void Mancala::showBoard()
{
    auto tempboard = board;

    // player 1
    for(Cell &c:player1cells){
        for(Stone &st:c.getStones()){
            tempboard[st.getPosY()][st.getPosX()] = st.img;
            st.img = STONE;
        }
    }

    for(Stone &st:homeCell1->getStones()){
        tempboard[st.getPosY()][st.getPosX()] = st.img;
        st.img = STONE;
    }

    // player 2
    for(Cell &c:player2cells){
        for(Stone &st:c.getStones()){
            tempboard[st.getPosY()][st.getPosX()] = st.img;
            st.img = STONE;
        }
    }
    for(Stone &st:homeCell2->getStones()){
        tempboard[st.getPosY()][st.getPosX()] = st.img;
        st.img = STONE;
    }

    system("cls");
    for(auto v:tempboard){
        for(auto x:v){
            cout << x;
        }cout << endl;
    }
}

void Mancala::gameInit(int n, int s){
    this->numberOfCells = n;
    this->numberOfStones = s;
    gameState = 1;

    player1cells.clear();
    player1moveStack.clear();
    player1score = 0;


    player2cells.clear();
    player2moveStack.clear();
    player2score = 0;

    /// fill stones
    // player 1
    for(int i = 0; i < numberOfCells; i++){
        Cell cell( ((SCALE_X+1)*CELL_B + 1)*(i+1) + 1, 1,CELL_H, CELL_B);
        for(int j = 0; j < numberOfStones; j++){
            Stone stone;
            cell.insertStone(stone);
        }
        player1cells.push_back(cell);
    }

    // player 2
    for(int i = 0; i < numberOfCells; i++){
        Cell cell( ((SCALE_X+1)*CELL_B + 1)*(i+1) + 1, BOARD_H-CELL_H+1,CELL_H, CELL_B);
        for(int j = 0; j < numberOfStones; j++){
            Stone stone;
            cell.insertStone(stone);
        }
        player2cells.push_back(cell);
    }

    homeCell1 = new Cell(1, 1, BOARD_H, CELL_B);
    homeCell2 = new Cell( ((SCALE_X+1)*CELL_B + 1)*(numberOfCells+1) + 1, 1, BOARD_H, CELL_B);

    drawBoard();
}


bool Mancala::player1move(int cellNo)
{
    if(cellNo < 1 or cellNo > numberOfCells){
        cout << "invalid move" << endl; return false;
    }
    if(player1cells[cellNo-1].getNumberOfStones() == 0){
        cout << "invalid move" << endl; return false;
    }
    player1moveStack.push_back(cellNo);

    Stone st;
    int next = cellNo-2;
    int i = player1cells[cellNo-1].getNumberOfStones();
    while(i--){
        player1cells[cellNo-1].takeStone(st);
        st.img = LAST_MOVED_STONE;
        if(next == -1){
            // home
            homeCell1->insertStone(st);
            if(!i){
                // last stone -> another chance
                showBoard();
                return false;
            }
            next--;
            continue;
        }
        else if(next <= -(numberOfCells+2)){
            // back to own cell
            next = numberOfCells-1;
            player1cells[next].insertStone(st);
        }
        else if(next < -1){
            // player 2's cells
            player2cells[-2-next].insertStone(st);
            next--;
            continue;
        }
        else{
            // own
            player1cells[next].insertStone(st);
        }
        /// check steal
        if(!i and player1cells[next].getNumberOfStones() == 1 and player2cells[next].getNumberOfStones()){
            // this was the last move
            cout << "steal!!" << endl;
            while(player1cells[next].takeStone(st)){
                st.img = LAST_MOVED_STONE;
                homeCell1->insertStone(st);
            }
            while(player2cells[next].takeStone(st)){
                st.img = LAST_MOVED_STONE;
                homeCell1->insertStone(st);
            }
        }

        next--;
    }

    return true;
}

bool Mancala::player2move(int cellNo)
{
    if(cellNo < 1 or cellNo > numberOfCells){
        cout << "invalid move" << endl; return false;
    }
    if(player2cells[cellNo-1].getNumberOfStones() == 0){
        cout << "invalid move" << endl; return false;
    }
    player2moveStack.push_back(cellNo);
//return;

    Stone st;
    int next = cellNo;
    int i = player2cells[cellNo-1].getNumberOfStones();
    while(i--){

        player2cells[cellNo-1].takeStone(st);
        st.img = LAST_MOVED_STONE;

        if(next == numberOfCells){
            // home
            homeCell2->insertStone(st);
            if(!i){
                // last stone -> another chance
                showBoard();
                return false;
            }
            next++;
            continue;
        }
        else if(next > 2*numberOfCells){
            // back to own cell
            next = 0;
            player2cells[next].insertStone(st);
        }
        else if(next > numberOfCells){
            // player 1's cells
            player1cells[numberOfCells - (next - numberOfCells)].insertStone(st);
            next++;
            continue;
        }
        else{
            // own
            player2cells[next].insertStone(st);
        }

        /// check steal
        if(!i and player2cells[next].getNumberOfStones() == 1 and player1cells[next].getNumberOfStones()){
            // this was the last move
            cout << "steal!!" << endl;
            while(player1cells[next].takeStone(st)){
                st.img = LAST_MOVED_STONE;
                homeCell2->insertStone(st);
            }
            while(player2cells[next].takeStone(st)){
                st.img = LAST_MOVED_STONE;
                homeCell2->insertStone(st);
            }
        }

        next++;
    }

    return true;
}

bool Mancala::gameover()
{
    int count1, count2;
    count1 = count2 = 0;

    for(Cell cell:player1cells){
        count1 += cell.getNumberOfStones();
    }

    for(Cell cell:player2cells){
        count2 += cell.getNumberOfStones();
    }

    if(count1 && count2)return false;

    cout << "Game Over" << endl;
    if(count1 + homeCell1->getNumberOfStones() > count2 + homeCell2->getNumberOfStones()){
        cout << "Player 1 Won" << endl;
    }
    else if(count1 + homeCell1->getNumberOfStones() < count2 + homeCell2->getNumberOfStones()){
        cout << "Player 2 Won" << endl;
    }
    else{
        cout << "Draw" << endl;
    }

    gameState = 0;
    return true;
}

void Mancala::gameStart()
{
    int mv, skip1, skip2;
    showBoard();
    skip1 = skip2 = 0;
    while(gameState){

        if(!skip1 and !gameover()){
            cout << "player 1 move: ";
            cin >> mv;
            cout << endl;
            if(!player1move(mv)){
                skip2 = 1;
            }
            else{
                skip2 = 0;
                showBoard();
            }
        }
        if(!skip2 and !gameover()){
            cout << "player 2 move: ";
            cin >> mv;
            cout << endl;
            if(!player2move(mv)){
                skip1 = 1;
            }
            else{
                skip1 = 0;
                showBoard();
            }
        }
    }
}

int main()
{
    Mancala mancala;
    mancala.gameInit(6, 4);
    mancala.gameStart();

    return 0;

}
