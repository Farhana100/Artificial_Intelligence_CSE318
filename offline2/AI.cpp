class AI
{
    std::stack < vector < vector <int> > > states;

    /**
            0             1       2       3       4       5       6
        0   home_p1     1_p1    2_p1    3_p1    4_p1    5_p1    6_p1
        1   1_p2        2_p2    3_p2    4_p2    5_p2    6_p2    home_p2


        all move count starts from 1
    */

    int heuristic;
    int numberOfCells;
    bool iamplayer1;
    bool maximizeplayer1;                           // start state player
    int first_valid_move;                           // furthest valid bin ( a bin on my side which is not empty) from my storage
    int my_stone_count;                             // The total number of stones residing in the six bins of my side
    int opp_stone_count;                            // The total number of stones residing in the six bins of opponent's side
    int stone_count_diff;                           // How far ahead of my opponent I am now [the difference in stone count between my storage and opponent's storage
    int my_storage_stones;                          // How close I am to winning (If my storage is already close to containing half of total number of stones)
    int opp_storage_stones;                         // How close opponent is to winning
    int stones_close_to_my_storage;                 /// NOT CLEAR -> Number of stones close to my storage ( a stone, although residing in a bin on my side, is not close to my storage, if it is going to be overflowed to my opponent's side )
    int stones_close_to_opp_storage;                // Number of stones close to my opponent's storage
    int my_extra_move_count;                           // Have I earned an extra move -> only counted for max player
    int my_steal_move_count;                           // bleh -> only counted for max player
    int opp_extra_move_count;                           // Have I earned an extra move -> only counted for max player
    int opp_steal_move_count;                           // bleh -> only counted for max player
    int stones_captured;                            // Have I captured any stone

public:

    AI()
    {
        first_valid_move = -1;
        my_stone_count = -1;
        opp_stone_count = -1;
        stone_count_diff = 0;
        my_storage_stones = 0;
        opp_storage_stones = 0;
        stones_close_to_my_storage = 0;         // left
        stones_close_to_opp_storage = 0;        // left
        my_extra_move_count = 0;
        opp_extra_move_count = 0;
        my_steal_move_count = 0;
        opp_steal_move_count = 0;
        stones_captured = 0;
        numberOfCells = 0;
        iamplayer1 = 1;
    }

    void setStartstate(vector < vector <int> > startstate, bool maximizeplayer1)
    {
        while(!states.empty())states.pop();

        this->states.push(startstate);
        this->maximizeplayer1 = maximizeplayer1;
        this->numberOfCells = startstate[0].size()-1;
        this->iamplayer1 = maximizeplayer1;

    }

    void analyse()
    {
        auto state = states.top();

        int i;
        if(maximizeplayer1){
            /// my_storage_stones
            my_storage_stones = state[0][0];


            /// opp_storage_stones
            opp_storage_stones = state[1][state[1].size()-1];

            /// my_stone_count      first_valid_move
            my_stone_count = 0;
            first_valid_move = 0;
            for(i = 1; i < state[0].size(); i++){
                my_stone_count += state[0][i];
                if(first_valid_move == 0 and state[0][i] > 0){
                    first_valid_move = i;
                }
            }

            /// opp_stone_count
            opp_stone_count = 0;
            for(i = 0; i < state[1].size()-1; i++){
                opp_stone_count += state[1][i];
            }

        }
        else{
            /// my_storage_stones
            my_storage_stones = state[1][numberOfCells];


            /// opp_storage_stones
            opp_storage_stones = state[0][0];

            /// my_stone_count first_valid_move
            my_stone_count = 0;
            first_valid_move = 0;
            for(i = 0; i < state[1].size()-1; i++){
                my_stone_count += state[1][i];
                if(first_valid_move == 0 and state[1][i] > 0){
                    first_valid_move = i+1;
                }
            }

            /// opp_stone_count
            opp_stone_count = 0;
            for(i = 1; i < state[0].size(); i++){
                opp_stone_count += state[0][i];
            }

        }

        /// stone_count_diff
        stone_count_diff = my_storage_stones - opp_storage_stones;
    }

    int generateNextState(int move)
    {
        /**
            returns
                -> -1 if invalid move
                -> 1 if extra move
                -> 2 if steal
                -> 0 else
        */
        auto state = states.top();

        if(iamplayer1){
            // player 1 moves

            if(state[0][move] == 0) return -1;

            int temp = state[0][move];
            state[0][move] = 0;
            move--;
            while(temp--){
                if(move == 0){
                    state[0][move]++;
                    if(temp == 0){
                        // last move in home -> extra move
                        if(maximizeplayer1) my_extra_move_count++;
                        else opp_extra_move_count++;
                        states.push(state);
                        return 1;
                    }
                }
                else if(move < -numberOfCells){
                    // enter own
                    move = numberOfCells-1;

                    if(state[0][move] == 0 and state[1][move-1] and temp == 0){
                        // steal
                        if(maximizeplayer1) my_steal_move_count++;
                        else opp_steal_move_count++;


                        state[0][move]++;
                        states.push(state);
                        return 2;
                    }

                    state[0][move]++;
                }
                else if(move < 0){
                    // opponents cell
                    state[1][-move-1]++;
                }
                else{

                    if(state[0][move] == 0 and state[1][move-1] and temp == 0){
                        // steal
                        if(maximizeplayer1) my_steal_move_count++;
                        else opp_steal_move_count++;

                        state[0][move]++;
                        states.push(state);
                        return 2;
                    }
                    state[0][move]++;
                }
                move--;
            }

        }
        else {


            // player 2 moves
            if(state[1][move-1] == 0){
                return -1;
            }


            int temp = state[1][move-1];
            state[1][move-1] = 0;

            while(temp--){
                if(move == numberOfCells){

                    state[1][move]++;
                    if(temp == 0){
                        // last move in home -> extra move
                        if(maximizeplayer1) this->opp_extra_move_count++;
                        else this->my_extra_move_count++;
                        states.push(state);
                        return 1;
                    }
                }
                else if(move > 2*numberOfCells){
                    // enter own
                    move = 0;

                    if(state[1][move] == 0 and state[0][move+1] and temp == 0){
                        // steal
                        if(maximizeplayer1) opp_steal_move_count++;
                        else my_steal_move_count++;

                        state[1][move]++;
                        states.push(state);
                        return 2;
                    }

                    state[1][move]++;
                }
                else if(move > numberOfCells){
                    // opponents cell
                    state[0][2*numberOfCells - move +1]++;
                }
                else{
                    if(state[1][move] == 0 and state[0][move+1] and temp == 0){
                        // steal
                        if(maximizeplayer1) opp_steal_move_count++;
                        else my_steal_move_count++;

                        state[1][move]++;
                        states.push(state);
                        return 2;
                    }

                    state[1][move]++;
                }
                move++;
            }
        }

        states.push(state);
        return 0;
    }


    void setHeuristic(int i)
    {
        heuristic = i;
    }

    int h1(){ return stone_count_diff;}
    int h2(int w1, int w2)
    {
        return w1*stone_count_diff + w2*(my_stone_count - opp_stone_count);
    }

    int h3(int w1, int w2, int w3)
    {
        return w1*stone_count_diff + w2*(my_stone_count - opp_stone_count) + w3*my_extra_move_count;
    }

    int h4(int w1, int w2, int w3)
    {
        return w1*stone_count_diff + w2*(my_stone_count - opp_stone_count) + w3*(my_extra_move_count - opp_extra_move_count);
    }

    int h5(int w1, int w2, int w3, int w4)
    {
        return w1*stone_count_diff + w2*(my_stone_count - opp_stone_count) + w3*my_extra_move_count + w4*my_steal_move_count;
    }

    int h6(int w1, int w2, int w3, int w4)
    {
        return w1*stone_count_diff + w2*(my_stone_count - opp_stone_count) + w3*(my_extra_move_count - opp_extra_move_count) + w4*(my_steal_move_count - opp_steal_move_count);
    }


    int h()
    {
        switch(heuristic)
        {
        case 1:
            return h1();
        case 2:
            return h2(5, 2);
        case 3:
            return h3(5, 2, 8);
        case 4:
            return h4(5, 2, 5);
        case 5:
            return h5(5, 2, 1, 1);
        default:
            return h6(20, 2, 4, 9);
        }
    }



    int minimax(int depth, bool maxplayer, int already_found_val)
    {
        if(depth){
            auto g = std::default_random_engine {};
            vector <int > moves;
            for(int i = 1; i <= numberOfCells; i++) moves.push_back(i);
            shuffle(moves.begin(), moves.end() , g);

            if(maxplayer){
                int val  = INT_MIN;
                int opt_move = 0;
                for(int i: moves){
                    int f = generateNextState(i);   /* this will,
                                                        -> state state
                                                        -> report if extra moves
                                                        -> report if steal moves
                                                */
                    if(f == -1){
                        continue;
                    }
                    // -> count extra moves and steal moves
                    iamplayer1 = !iamplayer1;
                    int tempval = minimax(depth-1, false, val);

                    if(tempval > val){
                        val = tempval;
                        opt_move = i;
                    }

                    // -> undo count extra moves and steal moves
                    iamplayer1 = !iamplayer1;
                    if(f == 1){
                        if(iamplayer1){
                            if(maximizeplayer1) my_extra_move_count--;
                            else opp_extra_move_count--;
                        }
                        else{
                            if(maximizeplayer1) opp_extra_move_count--;
                            else my_extra_move_count--;
                        }
                    }

                    if(f == 2){
                        if(iamplayer1){
                            if(maximizeplayer1) my_steal_move_count--;
                            else opp_steal_move_count--;
                        }
                        else{
                            if(maximizeplayer1) opp_steal_move_count--;
                            else my_steal_move_count--;
                        }
                    }

                    states.pop();
                    if(val >= already_found_val){
                         return val;
                    }

                }

                if(val == INT_MIN){
                    // no valid move
                    analyse();
                    return h();
                }

                if(states.size() == 1)return opt_move;
                return val;
            }
            else{
                int val  = INT_MAX;
                int opt_move = 0;
                for(int i: moves){
                    int f = generateNextState(i);   /* this will,
                                                        -> state state
                                                        -> report if extra moves
                                                        -> report if steal moves
                                            */

                    if(f == -1){
                        continue;
                    }
                    // -> count extra moves and steal moves
                    iamplayer1 = !iamplayer1;
                    int tempval = minimax(depth-1, true, val);

                    if(tempval < val){
                        val = tempval;
                        opt_move = i;
                    }

                    // -> undo count extra moves and steal moves
                    iamplayer1 = !iamplayer1;
                    if(f == 1){
                        if(iamplayer1){
                            if(maximizeplayer1) my_extra_move_count--;
                            else opp_extra_move_count--;
                        }
                        else{
                            if(maximizeplayer1) opp_extra_move_count--;
                            else my_extra_move_count--;
                        }
                    }

                    if(f == 2){
                        if(iamplayer1){
                            if(maximizeplayer1) my_steal_move_count--;
                            else opp_steal_move_count--;
                        }
                        else{
                            if(maximizeplayer1) opp_steal_move_count--;
                            else my_steal_move_count--;
                        }
                    }
                    states.pop();

                    if(val <= already_found_val) return val;
                }


                if(val == INT_MAX){
                    // no valid move
                    analyse();
                    return h();
                }

                if(states.size() == 1)return opt_move;
                return val;
            }
        }

        analyse();
        return h();
    }


};
