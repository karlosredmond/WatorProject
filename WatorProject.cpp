/* Copyright 2017 Karl Redmond, Ger Dobbs
   ProducerConsumerMain.cpp
   Author: Karl Redmond, Ger Dobbs
   Date:   Thursday,  27 November 2017.
   License:GNU General Public License v3.0
   Brief:Joint 4th Year Wator simulation Project*/
#include <iostream>
#include <random>

int const OCEANSIZEX = 100;
int const OCEANSIZEY = 100;
int numFish = 200;
int numShark = 20;
int fishBreedTime = 3;
int sharkBreedTime = 10;
int sharkStarveTime = 4;
char ocean[OCEANSIZEX][OCEANSIZEY][3];
int xPos, yPos;

void fillOcean() {
    for (int i = 0; i < OCEANSIZEX; ++i) {
        for (int j = 0; j < OCEANSIZEY; ++j) {
            ocean[i][j][0] = ' ';
        }
    }

    for (int i = 0; i < numFish; ++i) {
        xPos = random() % 100;
        yPos = random() % 100;
        ocean[xPos][yPos][0] = 70;
    }

    for (int i = 0; i < numShark; ++i) {
      xPos = random() % 100;
        yPos = random() % 100;
        ocean[xPos][yPos][0] = 83;
    }
}

void displayOcean() {
    for (int i = 0; i < OCEANSIZEX; ++i) {
        for (int j = 0; j < OCEANSIZEY; ++j) {
            std::cout << ocean[i][j];
        }
        std::cout << std::endl;
    }
}
int main() {
    fillOcean();
    displayOcean();
    return 0;
}
