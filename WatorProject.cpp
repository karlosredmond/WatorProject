/* Copyright 2017 Karl Redmond, Ger Dobbs
   ProducerConsumerMain.cpp
   Author: Karl Redmond, Ger Dobbs
   Date:   Thursday,  27 November 2017.
   License:GNU General Public License v3.0
   Brief:Joint 4th Year Wator simulation Project*/
#include <iostream>
#include <random>

int const OCEANSIZEX = 10;
int const OCEANSIZEY = 10;
int numFish = 2;
int numShark = 5;
int fishBreedTime = 3;
int sharkBreedTime = 8;
int sharkStarveTime = 4;
char ocean[OCEANSIZEX][OCEANSIZEY][3];
int xPos, yPos;

void initOceanCube() {
        for (int i = 0; i < OCEANSIZEX; ++i) {
            for (int j = 0; j < OCEANSIZEY; ++j) {
                ocean[i][j][0] = ' ';
                ocean[i][j][1] = ' ';
                ocean[i][j][2] = ' ';
            }
        }
}

void fillOceanCubeWithFish() {
        for (int i = 0; i < numFish; ++i) {
            xPos = (int) random() % 10;
            yPos = (int) random() % 10;
            ocean[xPos][yPos][0] = 'f';
            ocean[xPos][yPos][1] = '0' + fishBreedTime;
        }
}

void fillOceanCubeWithShark() {
        for (int i = 0; i < numShark; ++i) {
            xPos = std::rand() % 10;
            yPos = std::rand() % 10;
            ocean[xPos][yPos][0] = 'S';
            ocean[xPos][yPos][1] = '0' + sharkBreedTime;
            ocean[xPos][yPos][2] = '0' + sharkStarveTime;
        }
}

void fillOcean() {
    initOceanCube();
    fillOceanCubeWithFish();
    fillOceanCubeWithShark();
}

void displayOcean() {
    for (int i = 0; i < OCEANSIZEX; ++i) {
        for (int j = 0; j < OCEANSIZEY; ++j) {
            std::cout << ocean[i][j][0];
        }
        std::cout << std::endl;
    }
}

void displayBreedTimes() {
    for (int i = 0; i < OCEANSIZEX; ++i) {
        for (int j = 0; j < OCEANSIZEY; ++j) {
            std::cout << ocean[i][j][1];
        }
        std::cout << std::endl;
    }
}

void displayStarveTimes() {
    for (int i = 0; i < OCEANSIZEX; ++i) {
        for (int j = 0; j < OCEANSIZEY; ++j) {
            std::cout << ocean[i][j][2];
        }
        std::cout << std::endl;
    }
}
int main() {
    fillOcean();
    displayOcean();
    std::cout << std::endl;
    displayBreedTimes();
    std::cout << std::endl;
    displayStarveTimes();

    return 0;
}
