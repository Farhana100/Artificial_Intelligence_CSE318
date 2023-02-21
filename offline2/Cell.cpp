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
