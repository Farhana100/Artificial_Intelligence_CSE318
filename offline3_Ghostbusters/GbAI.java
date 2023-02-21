package com.company;

import java.awt.*;
import java.text.DecimalFormat;
import java.util.Vector;

import static java.lang.Math.abs;

public class GbAI
{
    int rows;
    int cols;

    Vector <Point> obstacles;   // x -> row, y -> col
    boolean[][] board;
    double[][] probability;

    double sensorAccurary;
    double edgeBias;
    double[][][] transitionProbability;     /*  [row][col][0] = edge transition
                                                [row][col][1] = diag transition
                                             */


    // helpers
    int[][] edge = {{0, -1}, {0, 1}, {1, 0}, {-1, 0}};
    int[][] diags = {{0, 0}, {1, 1}, {-1, -1}, {-1, 1}, {1, -1}};
    private static final DecimalFormat df = new DecimalFormat("0.0000");
    private static final double printWeight = 100.0;

    public GbAI(int rows, int cols, Vector<Point> obstacles, double sensorAccurary, double edgeBias)
    {
        this.rows = rows;
        this.cols = cols;
        this.obstacles = obstacles;
        this.sensorAccurary = sensorAccurary;
        this.edgeBias = edgeBias;

        board = new boolean[rows][cols];
        probability = new double[rows][cols];
        transitionProbability = new double[rows][cols][2];
    }

    public void arrangeBoard()
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                board[i][j] = true;
            }
        }
        for (Point obs: obstacles)
        {
            board[obs.x][obs.y] = false;
        }
    }

    public void setInitProbability()
    {
        // equally likely
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if(board[i][j]) probability[i][j] = 1.0/(rows*cols - obstacles.size());
                else probability[i][j] = 0;
            }
        }
    }

    public void calcTransitionProbability()
    {
        int count = 0;

        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                //------------------------------------------------->> EDGE start
                count = 0;
                for (int[] ints : edge)
                {
                    if (r + ints[0] < 0 || c + ints[1] < 0 || r + ints[0] >= rows || c + ints[1] >= cols) continue;
                    if(!board[r + ints[0]][c + ints[1]]) continue;
                    count++;
                }

                transitionProbability[r][c][0] = edgeBias/count;
                //------------------------------------------------->> EDGE end


                //------------------------------------------------->> DIAG start
                count = 0;
                for (int[] ints : diags)
                {
                    if (r + ints[0] < 0 || c + ints[1] < 0 || r + ints[0] >= rows || c + ints[1] >= cols) continue;
                    if(!board[r + ints[0]][c + ints[1]]) continue;
                    count++;
                }

                transitionProbability[r][c][1] = (1-edgeBias)/count;
                //------------------------------------------------->> DIAG end

                System.out.print("( " + transitionProbability[r][c][0] + ", " + transitionProbability[r][c][1] + " ) ");
            }
            System.out.println();
        }

        System.out.println();
        System.out.println();
    }


    public void printProbabilities()
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                System.out.print(df.format(probability[i][j] * printWeight));
                System.out.print(' ');
            }
            System.out.println();
        }
    }

    public Point getCasper()
    {
        Point p = new Point();
        p.setLocation(0, 0);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (probability[p.x][p.y] < probability[i][j]) p.setLocation(i, j);
            }
        }
        return p;
    }

    public void updateProbabilityforOneTimeStep()
    {
        double[][] probabilityT1 = new double[rows][cols];

        // calculate
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                if(!board[r][c]) continue;
                //------------------------------------------------->> EDGE start
                for (int[] ints : edge)
                {
                    if (r + ints[0] < 0 || c + ints[1] < 0 || r + ints[0] >= rows || c + ints[1] >= cols) continue;
                    if(!board[r + ints[0]][c + ints[1]]) continue;
                    probabilityT1[r][c] += probability[r + ints[0]][c + ints[1]]*transitionProbability[r + ints[0]][c + ints[1]][0];
                }
                //------------------------------------------------->> EDGE end


                //------------------------------------------------->> DIAG start
                for (int[] ints : diags)
                {
                    if (r + ints[0] < 0 || c + ints[1] < 0 || r + ints[0] >= rows || c + ints[1] >= cols) continue;
                    if(!board[r + ints[0]][c + ints[1]]) continue;
                    probabilityT1[r][c] += probability[r + ints[0]][c + ints[1]]*transitionProbability[r + ints[0]][c + ints[1]][1];
                }
                //------------------------------------------------->> DIAG end
            }
        }
        // update
        for (int i = 0; i < rows; i++)
            if (cols >= 0) System.arraycopy(probabilityT1[i], 0, probability[i], 0, cols);

        // normalize
        normalizeProbability();
    }


    public void reweighProbabilityBasedOnEvidence(int sensorRow, int sensorCol, int signal)
    {
        // calculate
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                probability[i][j] *= (( abs(i-sensorRow) < 2 && abs(j-sensorCol) < 2)? ((signal == 1)?sensorAccurary:(1-sensorAccurary)) : ((signal == 1)?(1-sensorAccurary):sensorAccurary));
            }
        }
        // normalize
        normalizeProbability();
    }

    public void normalizeProbability()
    {
        double sum = 0;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                sum += probability[i][j];
            }
        }

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                probability[i][j] /= sum;
            }
        }
    }

}
