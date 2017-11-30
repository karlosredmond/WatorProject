#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <thread>
#include <time.h>
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>


int const OCEANSIZEX = 100;
int const OCEANSIZEY = 100;
int numFish = 20;
int numShark = 100;
int fishBreedTime = 3;
int sharkBreedTime = 8;
int sharkStarveTime = 4;
char ocean[OCEANSIZEX][OCEANSIZEY][3];
char oceanCopy[OCEANSIZEX][OCEANSIZEY][3];
int xPos, yPos;
std::vector<char> neighbourhood;
int const DISPLAYLAYER = 0;
int const BREEDLAYER = 1;
int const STARVELAYER = 2;
//int direction = 0;
typedef boost::mt19937 RNGType;
RNGType rng;
boost::uniform_int<> one_to_six( 0, 3 );
boost::variate_generator< RNGType, boost::uniform_int<> >
        dice(rng, one_to_six);
int direction = 0;


void initOceanCubes() {
    for (int i = 0; i < OCEANSIZEX; ++i) {
        for (int j = 0; j < OCEANSIZEY; ++j) {
            ocean[i][j][0] = ' ';
            ocean[i][j][1] = ' ';
            ocean[i][j][2] = ' ';
            oceanCopy[i][j][0] = ' ';
            oceanCopy[i][j][1] = ' ';
            oceanCopy[i][j][2] = ' ';
        }
    }
}

void copyOcean(char copyFrom[OCEANSIZEX][OCEANSIZEY][3], char copyTo[OCEANSIZEX][OCEANSIZEY][3] ){
    for (int k = 0; k < OCEANSIZEX; ++k) {
        for (int i = 0; i < OCEANSIZEY; ++i) {
            for (int j = 0; j < 3; ++j) {
                copyTo[k][i][j] = copyFrom[k][i][j];
            }
        }
    }
}

void fillOceanCubeWithFish() {
    for (int i = 0; i < numFish; ++i) {
        xPos = (int) random() % OCEANSIZEX;
        yPos = (int) random() % OCEANSIZEY;
        ocean[xPos][yPos][0] = 'F';
        ocean[xPos][yPos][1] = '0' + fishBreedTime;
    }
}

void fillOceanCubeWithShark() {
    for (int i = 0; i < numShark; ++i) {
        xPos = std::rand() % OCEANSIZEX;
        yPos = std::rand() % OCEANSIZEY;
        ocean[xPos][yPos][0] = 'S';
        ocean[xPos][yPos][1] = '0' + sharkBreedTime;
        ocean[xPos][yPos][2] = '0' + sharkStarveTime;
    }
}

void fillOcean() {
    initOceanCubes();
    fillOceanCubeWithFish();
    fillOceanCubeWithShark();
    copyOcean(ocean, oceanCopy);
}

std::string displayOcean(/*char ocean[OCEANSIZEX][OCEANSIZEY][3]*/) {
    std::string temp = "";
    for (int i = 0; i < OCEANSIZEX; ++i) {
        for (int j = 0; j < OCEANSIZEY; ++j) {
            temp = temp + ocean[i][j][DISPLAYLAYER];
        }
        temp = temp + "\n";
    }
    return temp;
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

char checkNeighbourhood(int i, int j){
    neighbourhood.clear();
    if(ocean[(i + OCEANSIZEX -1) % OCEANSIZEX][j][0] == ' '){
        neighbourhood.push_back('N');
    }
    if(ocean[(i + 1) % OCEANSIZEY][j][0] == ' '){
        neighbourhood.push_back('S');
    }
    if(ocean[i][(j + 1) % OCEANSIZEY][0] == ' '){
        neighbourhood.push_back('E');
    }
    if(ocean[i][(j + OCEANSIZEX -1) % OCEANSIZEX][0] == ' '){
        neighbourhood.push_back('W');
    }
    //std::cout << neighbourhood.size();

    //std::cout << neighbourhood[direction];
    //std::cout << direction;
    if(neighbourhood.empty()){
        return 'D';
    }
    else{
        direction++;
        //int direction = dice() % (neighbourhood.size()-1);
        return neighbourhood[dice() % neighbourhood.size() -1];
    }
        //srand(time(NULL));
        //int direction = (int) rand() % (neighbourhood.size() - 1) ;
        /*direction++;
        if(direction == 4){
            direction = 0;
        }*/


        //return neighbourhood[direction] ;
    //}
     //return 'W';
}

void move(){
    int direction;
    for (int i = 0; i < OCEANSIZEX; ++i) {
        for (int j = 0; j < OCEANSIZEY; ++j) {
            if (ocean[i][j][0] == 'F'){
                ocean[i][j][1]--;   //Found a fish so decrementing breed time
                direction = checkNeighbourhood(i,j);
                if(direction == 'N'){
                    if(ocean[i][j][1] == '0'){
                        oceanCopy[i][j][1] = '0' + fishBreedTime;
                        oceanCopy[(i + OCEANSIZEX -1) % OCEANSIZEX][j][1] = '0' + fishBreedTime;
                    }
                    else {
                        oceanCopy[i][j][0] = ' ';
                        oceanCopy[(i + OCEANSIZEX -1) % OCEANSIZEX][j][1] = ocean[i][j][1]; //copying instance of fish breed time to ocean copy's relative layer
                        oceanCopy[i][j][1] = ' ';
                    }
                    oceanCopy[(i + OCEANSIZEX -1) % OCEANSIZEX][j][0] = 'F';
                }
                else if(direction == 'S'){
                    if(ocean[i][j][1] == '0'){
                        oceanCopy[i][j][1] = '0' + fishBreedTime;
                        oceanCopy[(i + 1) % OCEANSIZEY][j][1] = '0' + fishBreedTime;
                        oceanCopy[(i + 1) % OCEANSIZEY][j][0] = 'F';
                    }
                    else{
                        oceanCopy[(i + 1) % OCEANSIZEY][j][1] = ocean[i][j][1]; //copying instance of fish breed time to ocean copy's relative layer
                        oceanCopy[i][j][1] = ' ';
                        oceanCopy[(i + 1) % OCEANSIZEY][j][0] = 'F';
                        oceanCopy[i][j][0] = ' ';
                    }
                }
                else if(direction == 'E'){
                    if(ocean[i][j][1] == '0'){
                        oceanCopy[i][(j + 1) % OCEANSIZEY][1] = '0' + fishBreedTime;
                        oceanCopy[i][j][1] = '0' + fishBreedTime;
                        oceanCopy[i][(j + 1) % OCEANSIZEY][0] = 'F';
                    }
                    else{
                        oceanCopy[i][(j + 1) % OCEANSIZEY][1] = ocean[i][j][1];
                        oceanCopy[i][j][1] = ' ';
                        oceanCopy[i][(j + 1) % OCEANSIZEY][0] = 'F';
                        oceanCopy[i][j][0] = ' ';
                    }
                }
                else if (direction == 'W'){
                    if(ocean[i][j][1] == '0'){
                        oceanCopy[i][j][1] = '0' + fishBreedTime;
                        oceanCopy[i][(j + OCEANSIZEX -1) % OCEANSIZEX][1] = '0' + fishBreedTime;
                    }
                    else {
                        oceanCopy[i][j][0] = ' ';
                        oceanCopy[i][(j + OCEANSIZEX -1) % OCEANSIZEX][1] = ocean[i][j][1]; //copying instance of fish breed time to ocean copy's relative layer
                        oceanCopy[i][j][1] = ' ';
                    }
                    oceanCopy[i][(j + OCEANSIZEX -1) % OCEANSIZEX][0] = 'F';
                }
            }
        }
    }
    copyOcean(oceanCopy, ocean);
}

int main() {
    std::string display = "Test";
    sf::RenderWindow window(sf::VideoMode(1500, 1500), "SFML works!");
    sf::Font font;
    font.loadFromFile("/Library/Fonts/AppleMyungjo.ttf");
    sf::Text text(display, font, 11);
    text.setCharacterSize(20);
    text.setPosition(0, 0);
    fillOcean();
    //std::cout << displayOcean();
    //move();
    //std::cout << displayOcean();
   // std::cout << displayOcean(oceanCopy);
   // display = displayOcean();
   while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        text.setString(display);
        window.clear();
        window.draw(text);
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        move();
        display = displayOcean();
    }
    return 0;
}
