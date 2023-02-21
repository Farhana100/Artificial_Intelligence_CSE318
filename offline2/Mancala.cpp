class Mancala{
    vector <vector <char> > board;
    vector <string> moveStack;
    int numberOfCells;
    int numberOfStones;

    // player 1
    vector <Cell> player1cells;
    int player1score;
    Cell *homeCell1;

    // player 2
    vector <Cell> player2cells;
    int player2score;
    Cell *homeCell2;

    bool gameState;     // true -> game on, false -> game ended
    void drawBoard();
public:
    Mancala();
    void showBoard();
    void gameInit(int n, int s);
    void gameStart_2player();
    void gameStart_withAI(int heuristic);
    void gameStart_withAI2(int heuristic);
    void gameStart_AIvsAI(int heuristic1, int heuristic2);
    bool player1move(int cellNo);
    bool player2move(int cellNo);
    vector <vector <int > > getState();
    bool gameover();
    int winner;
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
    winner = 0;
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
    return;
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

   // system("cls");
    for(auto v:tempboard){
        for(auto x:v){
            cout << x;
        }cout << endl;
    }
/*
    for(string s: moveStack){
        cout << s << endl;
    }
    cout << endl;
    */
}

void Mancala::gameInit(int n, int s){
    this->numberOfCells = n;
    this->numberOfStones = s;
    gameState = 1;


    winner = 0;
    player1cells.clear();
    moveStack.clear();
    player1score = 0;


    player2cells.clear();
    moveStack.clear();
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
            moveStack.push_back("steal!!");
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
            moveStack.push_back("steal!!");
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
        winner = 1;
    }
    else if(count1 + homeCell1->getNumberOfStones() < count2 + homeCell2->getNumberOfStones()){
        cout << "Player 2 Won" << endl;
        winner = 2;
    }
    else{
        cout << "Draw" << endl;
        winner = 0;
    }

    gameState = 0;
    return true;
}

void Mancala::gameStart_2player()
{
    int mv, skip1, skip2;
    showBoard();
    skip1 = skip2 = 0;
    while(gameState){

        if(!skip1 and !gameover()){
            cout << "player 1 move: ";
            cin >> mv;
            cout << endl;
            moveStack.push_back("player 1 move: " + to_string(mv));
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
            moveStack.push_back("player 2 move: " + to_string(mv));
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


void Mancala::gameStart_withAI(int heuristic)
{
    AI ai;
    ai.setHeuristic(heuristic);

    int mv, skip1, skip2;
    showBoard();
    skip1 = skip2 = 0;
    while(gameState){

        if(!skip1 and !gameover()){
            ai.setStartstate(getState(), true);
            mv = ai.minimax(DEPTH, true, INT_MAX);
            moveStack.push_back("player 1 move: " + to_string(mv));
            if(!player1move(mv)){
                skip2 = 1;
                int dum;
                cin >> dum;
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
            moveStack.push_back("player 2 move: " + to_string(mv));
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


void Mancala::gameStart_withAI2(int heuristic)
{
    AI ai;
    ai.setHeuristic(heuristic);

    int mv, skip1, skip2;
    showBoard();
    skip1 = skip2 = 0;
    while(gameState){

        if(!skip1 and !gameover()){
            cout << "player 1 move: ";
            cin >> mv;
            cout << endl;
            moveStack.push_back("player 1 move: " + to_string(mv));

            if(!player1move(mv)){
                skip2 = 1;
            }
            else{
                skip2 = 0;
                showBoard();
            }
        }
        if(!skip2 and !gameover()){
            ai.setStartstate(getState(), false);
            mv = ai.minimax(DEPTH2, true, INT_MAX);
            moveStack.push_back("player 2 move: " + to_string(mv));
            cout << "player 2 move: " << mv << endl;

            if(!player2move(mv)){
                skip1 = 1;
                int dum;
                cin >> dum;
            }
            else{
                skip1 = 0;
                showBoard();
            }
        }
    }
}

void Mancala::gameStart_AIvsAI(int heuristic1, int heuristic2)
{
    AI ai1, ai2;
    ai1.setHeuristic(heuristic1);
    ai2.setHeuristic(heuristic2);

    int mv, skip1, skip2;
    showBoard();
    skip1 = skip2 = 0;
    while(gameState){

        if(!skip1 and !gameover()){
            ai1.setStartstate(getState(), true);
            mv = ai1.minimax(DEPTH1, true, INT_MAX);

//            moveStack.push_back("player 1 move: " + to_string(mv));
//            cout << "player 1 move: " << mv << endl;
            if(!player1move(mv)){
                skip2 = 1;
            }
            else{
                skip2 = 0;
                //showBoard();
            }
        }

        if(!skip2 and !gameover()){
            ai2.setStartstate(getState(), false);
            mv = ai2.minimax(DEPTH2, true, INT_MAX);

//            moveStack.push_back("player 2 move: " + to_string(mv));
//
//            cout << "player 2 move: " << mv << endl;
            if(!player2move(mv)){
                skip1 = 1;
            }
            else{
                skip1 = 0;
                //showBoard();
            }
        }
    }
}

vector <vector <int > > Mancala::getState()
{
    vector <vector <int > > state;

    vector <int> p;

    p.push_back(homeCell1->getNumberOfStones());
    for(Cell c:player1cells){
        p.push_back(c.getNumberOfStones());
    }
    state.push_back(p);
    p.clear();

    for(Cell c:player2cells){
        p.push_back(c.getNumberOfStones());
    }
    p.push_back(homeCell2->getNumberOfStones());
    state.push_back(p);

    return state;
}
