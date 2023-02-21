package com.company;

import org.chocosolver.solver.Model;
import org.chocosolver.solver.Solution;
import org.chocosolver.solver.variables.IntVar;

public class Main {

    public static void main(String[] args) {
	// write your code here
        Model model = new Model("sudoku 1");


        IntVar[][] vars = model.intVarMatrix(9, 9, 1, 9);

        // constraints

        // row
        for (int i = 0; i < 9; i++)
        {
            model.allDifferent(vars[i]).post();
        }



        // col
        for (int i = 0; i < 9; i++)
        {
            IntVar [] temp = new IntVar[9];
            for (int j = 0; j < 9; j++)
            {
                temp[j] = vars[j][i];
            }
            model.allDifferent(temp).post();
        }

        // 3x3 box

        int [][] translate1 = {{0, 0}, {3, 0}, {6, 0}, {6, 3}, {3, 3}, {0, 3}, {0, 6}, {3, 6}, {6, 6}};
        int [][] translate2 = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};

        int i = 1, j = 1;

        for (int k = 0; k < 9; k++)
        {
            IntVar [] temp = new IntVar[9];
            for (int l = 0; l < 9; l++)
            {
                temp[l] = vars[i + translate1[k][0] + translate2[l][0]][j + translate1[k][1] + translate2[l][1]];
            }
            model.allDifferent(temp).post();
        }

        // puzzle
        model.arithm(vars[0][0].add(vars[1][0]).intVar(), "=", 13).post();
        model.arithm(vars[0][1].add(vars[0][2]).add(vars[0][3]).add(vars[0][4]).add(vars[1][4]).intVar(), "=", 21).post();
        model.arithm(vars[0][5].add(vars[1][5]).intVar(), "=", 12).post();
        model.arithm(vars[0][6].add(vars[1][6]).add(vars[1][7]).add(vars[2][7]).intVar(), "=", 24).post();
        model.arithm(vars[0][7].add(vars[0][8]).add(vars[1][8]).add(vars[2][8]).intVar(), "=", 17).post();
        model.arithm(vars[1][1].add(vars[2][1]).intVar(), "=", 9).post();
        model.arithm(vars[1][2].add(vars[2][2]).intVar(), "=", 11).post();
        model.arithm(vars[1][3].add(vars[2][3]).add(vars[3][3]).intVar(), "=", 13).post();
        model.arithm(vars[2][0].add(vars[3][0]).intVar(), "=", 5).post();
        model.arithm(vars[2][4].add(vars[3][4]).intVar(), "=", 11).post();
        model.arithm(vars[2][5].add(vars[2][6]).intVar(), "=", 11).post();
        model.arithm(vars[3][1].add(vars[3][2]).add(vars[4][2]).add(vars[4][3]).add(vars[5][2]).add(vars[5][1]).intVar(), "=", 33).post();
        model.arithm(vars[3][5].add(vars[3][6]).intVar(), "=", 15).post();
        model.arithm(vars[3][7].add(vars[3][8]).intVar(), "=", 9).post();
        model.arithm(vars[4][0].add(vars[4][1]).intVar(), "=", 8).post();
        model.arithm(vars[4][4].add(vars[4][5]).add(vars[4][6]).add(vars[4][7]).add(vars[4][8]).intVar(), "=", 20).post();
        model.arithm(vars[5][0].add(vars[6][0]).intVar(), "=", 17).post();
        model.arithm(vars[5][3].add(vars[6][3]).add(vars[7][3]).intVar(), "=", 13).post();
        model.arithm(vars[5][4].add(vars[6][4]).intVar(), "=", 10).post();
        model.arithm(vars[5][5].add(vars[5][6]).intVar(), "=", 9).post();
        model.arithm(vars[5][7].add(vars[5][8]).intVar(), "=", 8).post();
        model.arithm(vars[6][1].add(vars[7][1]).intVar(), "=", 13).post();
        model.arithm(vars[6][2].add(vars[7][2]).intVar(), "=", 5).post();
        model.arithm(vars[6][5].add(vars[6][6]).intVar(), "=", 10).post();
        model.arithm(vars[6][7].add(vars[7][7]).add(vars[7][6]).add(vars[8][6]).intVar(), "=", 18).post();
        model.arithm(vars[6][8].add(vars[7][8]).add(vars[8][8]).add(vars[8][7]).intVar(), "=", 25).post();
        model.arithm(vars[7][0].add(vars[8][0]).intVar(), "=", 5).post();
        model.arithm(vars[7][4].add(vars[8][4]).add(vars[8][3]).add(vars[8][2]).add(vars[8][1]).intVar(), "=", 31).post();
        model.arithm(vars[7][5].add(vars[8][5]).intVar(), "=", 9).post();


        System.out.println(model.toString());
        Solution solution = model.getSolver().findSolution();

        if(solution != null)
        {
            for (i = 0; i < 9; i++)
            {
                for (j = 0; j < 9; j++)
                {
                    System.out.print(vars[i][j].getValue() + " ");
                }
                System.out.println();
            }
            //System.out.println(solution.toString());
        }
        else {
            System.out.println("no solution");
        }
    }
}
