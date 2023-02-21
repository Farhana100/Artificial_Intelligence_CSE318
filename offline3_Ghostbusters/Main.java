package com.company;

import java.awt.*;
import java.util.Scanner;
import java.util.Vector;

public class Main {

    public static void main(String[] args) {
        int rows, cols;
        double sensorAccurary, edgeBias;
        Vector <Point> obstacles = new Vector<>();
        Point casperPos = new Point();
        Scanner scanner = new Scanner(System.in);

        edgeBias = 0.9;
        sensorAccurary = 0.85;
        rows = scanner.nextInt();
        cols = scanner.nextInt();
        int k = scanner.nextInt();

        for (int i = 0; i < k; i++)
        {
            int x, y;
            x = scanner.nextInt();
            y = scanner.nextInt();
            obstacles.add(new Point(x, y));
        }

        GbAI ai = new GbAI(rows, cols, obstacles, sensorAccurary, edgeBias);
        // init
        ai.arrangeBoard();
        ai.setInitProbability();
        ai.calcTransitionProbability();

        System.out.println("Initial Probability:");
        ai.printProbabilities();

        int t = 0;
        while (true)
        {
            int u, v, b;
            char command;

            command = scanner.next(".").charAt(0);

            if(command == 'Q'){
                System.out.println("Bye Casper!");
                return;
            }
            if(command == 'C'){
                casperPos = ai.getCasper();
                System.out.println("Casper is most probably at ( "+ casperPos.x + " , " + casperPos.y + " )");
            }
            if(command == 'R'){
                t++;
                u = scanner.nextInt();
                v = scanner.nextInt();
                b = scanner.nextInt();

                System.out.println("Probability Update ( " + t + "th reading ):");
                System.out.println();

                System.out.println("Before sensor reading: ");
                ai.updateProbabilityforOneTimeStep();
                ai.printProbabilities();
                System.out.println();

                System.out.println("After sensor reading: ");
                ai.reweighProbabilityBasedOnEvidence(u, v, b);
                ai.printProbabilities();
                System.out.println();
            }
        }

    }
}
