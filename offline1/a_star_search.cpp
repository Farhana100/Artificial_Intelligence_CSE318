#include <bits/stdc++.h>

using namespace std;

struct n_puzzle{
    int board[10][10];
    int parentID;
    int ID;
    int cost;                   // cost to reach node from the initial state
    int estm_cost;              // estimated cost from node to the goal state, heuristic
};

class comparePriority
{
    public:
    bool operator()(n_puzzle const& b1, n_puzzle const& b2)
    {
        return b1.cost + b1.estm_cost > b2.cost + b2.estm_cost;
    }
};


priority_queue <n_puzzle, vector<n_puzzle>, comparePriority> openlist;
set <string> closedlist;
map <int, n_puzzle> idMap;
int explored, goalID, expanded;

int goal[10][10];
int n;  // nxn


string toString(int board[10][10])
{
    int i, j;
    string str = "";

    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            str += to_string(board[i][j]) + " ";
        }
        str += "\n";
    }
    return str;
}

bool up(n_puzzle &puz)
{
    int i, j;

    for(i = 0; i < n-1; i++){
        for(j = 0; j < n; j++){
            if(!puz.board[i][j]){
                swap(puz.board[i][j], puz.board[i+1][j]);
                return true;
            }
        }
    }

    return false;
}

bool down(n_puzzle &puz)
{
    int i, j;

    for(i = 1; i < n; i++){
        for(j = 0; j < n; j++){
            if(!puz.board[i][j]){
                swap(puz.board[i][j], puz.board[i-1][j]);
                return true;
            }
        }
    }
    return false;
}

bool left(n_puzzle &puz)
{

    int i, j;

    for(i = 0; i < n; i++){
        for(j = 0; j < n-1; j++){
            if(!puz.board[i][j]){
                swap(puz.board[i][j], puz.board[i][j+1]);
                return true;
            }
        }
    }
    return false;
}

bool right(n_puzzle &puz)
{
    int i, j;

    for(i = 0; i < n; i++){
        for(j = 1; j < n; j++){
            if(!puz.board[i][j]){
                swap(puz.board[i][j], puz.board[i][j-1]);
                return true;
            }
        }
    }

    return false;
}

int hammingDistance(n_puzzle &puz)
{
    /**
        The number of blocks in the wrong position (not including the blank)
    */

    int i, j;
    puz.estm_cost = 0;

    for(i = 0;  i < n; i++){
        for(j  = 0; j < n; j++){
            if(!puz.board[i][j])continue;
            if(puz.board[i][j] != goal[i][j]){
                puz.estm_cost++;
            }
        }
    }

    return puz.estm_cost;
}

int manDistance(n_puzzle &puz)
{
    /**
        The sum of the Manhattan distances (sum of the vertical and horizontal distance) of the blocks to their goal positions
    */

    int i, j;
    puz.estm_cost = 0;
    map < int, pair <int, int> > g;

    for(i = 0;  i < n; i++){
        for(j = 0; j < n; j++){
            if(goal[i][j]){
                g[goal[i][j]] = {i, j};
            }
        }
    }

    for(i = 0;  i< n; i++){
        for(j = 0; j < n; j++){
            if(puz.board[i][j]){
                puz.estm_cost += abs(i - g[puz.board[i][j]].first) + abs(j - g[puz.board[i][j]].second);
            }
        }
    }

    return puz.estm_cost;
}

int linearConflict(n_puzzle &puz)
{
    int i, j, k;
    manDistance(puz);
    map < int, pair <int, int> > temp;

    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            if(!puz.board[i][j])continue;
            temp[puz.board[i][j]] = {i,j};
        }
    }

    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            if(!goal[i][j])continue;
            if(i != temp[goal[i][j]].first)continue;
            for(k = 0; k < n; k++){
                if(j == k)continue;
                if(i != temp[goal[i][k]].first)continue;
                if((j - k)*(temp[goal[i][j]].second - temp[goal[i][k]].second) < 0)puz.estm_cost++;
            }
        }
    }

    return puz.estm_cost;
}

void showboard(n_puzzle puz)
{
    int i, j;
    cout << puz.ID << endl;
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            cout << puz.board[i][j] << ' ';
        }cout << endl;
    }

    cout << endl;
}


void bestfs(n_puzzle init_state, int (*h)(n_puzzle &))
{
    int i, j, k, l;
    string goalstr = toString(goal);
    openlist = priority_queue <n_puzzle, vector<n_puzzle>, comparePriority>();
    closedlist.clear();
    idMap.clear();
    goalID = -1;
    explored = expanded = 0;

    init_state.ID = explored;
    init_state.parentID = -1;
    init_state.cost = 0;
    init_state.estm_cost = 0;
    idMap[explored] = init_state;
    explored++;

    openlist.push(init_state);

    n_puzzle next_State;
    while(!openlist.empty()){
        n_puzzle curr_state = openlist.top();
        openlist.pop();
        string temp = toString(curr_state.board);
        if(temp == goalstr){
            cout << "goal reached" << endl;
            goalID = curr_state.ID;
            break;
        }
        closedlist.insert(temp);
        expanded++;

        /// up
        next_State = curr_state;
        if(up(next_State) and !closedlist.count(toString(next_State.board))){
            next_State.cost++;
            next_State.ID = explored;
            next_State.parentID = curr_state.ID;
            h(next_State);

            idMap[explored] = next_State;
            explored++;
            openlist.push(next_State);
            //closedlist.insert(toString(next_State.board));
        }

        /// down
        next_State = curr_state;
        if(down(next_State) and !closedlist.count(toString(next_State.board))){
            next_State.cost++;
            next_State.ID = explored;
            next_State.parentID = curr_state.ID;
            h(next_State);

            idMap[explored] = next_State;
            explored++;
            openlist.push(next_State);
            //closedlist.insert(toString(next_State.board));
        }

        /// left
        next_State = curr_state;
        if(left(next_State) and !closedlist.count(toString(next_State.board))){
            next_State.cost++;
            next_State.ID = explored;
            next_State.parentID = curr_state.ID;
            h(next_State);

            idMap[explored] = next_State;
            explored++;
            openlist.push(next_State);
            //closedlist.insert(toString(next_State.board));
        }

        /// right
        next_State = curr_state;
        if(right(next_State) and !closedlist.count(toString(next_State.board))){
            next_State.cost++;
            next_State.ID = explored;
            next_State.parentID = curr_state.ID;
            h(next_State);

            idMap[explored] = next_State;
            explored++;
            openlist.push(next_State);
            //closedlist.insert(toString(next_State.board));
        }
    }
}


int inversions(n_puzzle &state)
{
    int i, j, inv = 0;
    vector <int > v;
    for(i = 0; i <n; i++){
        for(j = 0; j < n; j++){
            if(!state.board[i][j])continue;
            v.push_back(state.board[i][j]);
        }
    }

    for(i = 0; i < v.size(); i++){
        for(j = i+1; j< v.size(); j++){
            if(v[i] > v[j])inv++;
        }
    }

    return inv;
}

bool is_solvable(n_puzzle &state)
{
    int inv = inversions(state);

    if(n&1) return !(inv&1); // n odd

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(state.board[i][j])continue;
            return (i&1) != (inv&1);
        }
    }

    return false;
}

int main()
{
    int i, j, k, l;
    n_puzzle init_state, next_State;
    cin >> n;

    for(i =0, l = 1; i < n; i++){
        for(j = 0; j < n; j++, l++){
            cin >> init_state.board[i][j];
            goal[i][j] = l%(n*n);
        }
    }

    if(!is_solvable(init_state)){
        cout << "Not solvable" << endl; return 0;
    }

    cout << "Solvable " << endl;

    stack <string> stk;

    bestfs(init_state, linearConflict);
    cout << "---------------------------Linear Conflict----------------------------------" << endl;
    cout << "Total states explored: " << explored << endl;
    cout << "Total states expanded: " << expanded << endl;
    if(goalID != -1)    cout << "Cost: " << idMap[goalID].cost << endl << endl;
    while(goalID != -1){
        next_State = idMap[goalID];
        stk.push(toString(next_State.board));
        goalID = next_State.parentID;
    }

    while(!stk.empty()){
        cout << stk.top() << endl;
        stk.pop();
    }
    cout << "-----------------------------------------------------------------------------" << endl;


    bestfs(init_state, manDistance);
    cout << "---------------------------Manhattan distance----------------------------------" << endl;
    cout << "Total states explored: " << explored << endl;
    cout << "Total states expanded: " << expanded << endl;
    if(goalID != -1)    cout << "Cost: " << idMap[goalID].cost << endl << endl;
    while(goalID != -1){
        next_State = idMap[goalID];
        stk.push(toString(next_State.board));
        goalID = next_State.parentID;
    }

    while(!stk.empty()){
        cout << stk.top() << endl;
        stk.pop();
    }
    cout << "-----------------------------------------------------------------------------" << endl;

    bestfs(init_state, hammingDistance);
    cout << "---------------------------Hamming distance----------------------------------" << endl;
    cout << "Total states explored: " << explored << endl;
    cout << "Total states expanded: " << expanded << endl;
    if(goalID != -1)    cout << "Cost: " << idMap[goalID].cost << endl << endl;
    while(goalID != -1){
        next_State = idMap[goalID];
        stk.push(toString(next_State.board));
        goalID = next_State.parentID;
    }

    while(!stk.empty()){
        cout << stk.top() << endl;
        stk.pop();
    }
    cout << "-----------------------------------------------------------------------------" << endl;

}

