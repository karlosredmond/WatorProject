#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <cstdlib>
#include <thread>
#include <vector>



int const OCEANSIZEX = 100;
int const OCEANSIZEY = 100;
int numFish = 0;
int numShark = 10;
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
            temp = temp + ocean[i][j][STARVELAYER];
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

char checkNeighbourhood(int x, int y){
    neighbourhood.clear();
    if(ocean[x - 1][y][0] == ' '){
        neighbourhood.push_back('N');
    }
    if(ocean[x + 1][y][0] == ' '){
        neighbourhood.push_back('S');
    }
    if(ocean[x][y + 1][0] == ' '){
        neighbourhood.push_back('E');
    }
    if(ocean[x][y -1][0] == ' '){
        neighbourhood.push_back('W');
    }
    std::cout << neighbourhood.size();
    int direction = (int) random() % neighbourhood.size() ;

    //std::cout << neighbourhood[direction];
    //std::cout << direction;
    if(neighbourhood.size() == 0){
        return 'D';
    }
    else{
        return neighbourhood[direction] ;
    }

}


void move(){
    int direction;
    for (int i = 0; i < OCEANSIZEX; ++i) {
        for (int j = 0; j < OCEANSIZEY; ++j) {
            if (ocean[i][j][0] == 'F'){
                direction = checkNeighbourhood(i,j);
                if(direction == 'N'){
                    ocean[i][j][1]--;                    //Found a fish so decrementing breed time
                    oceanCopy[(i % OCEANSIZEX) - 1][j][0] = 'F';          //
                    oceanCopy[i][j][0] = ' ';
                    oceanCopy[(i % OCEANSIZEX) - 1][j][1] = ocean[i][j][1]; //copying instance of fish breed time to ocean copy's relative layer
                    oceanCopy[i][j][1] = ' ';
                }
                else if(direction == 'S'){
                    ocean[i][j][1]--;                    //Found a fish so decrementing breed time
                    oceanCopy[(i + 1) % OCEANSIZEX][j][0] = 'F';          //
                    oceanCopy[i][j][0] = ' ';
                    oceanCopy[(i + 1) % OCEANSIZEX][j][1] = ocean[i][j][1]; //copying instance of fish breed time to ocean copy's relative layer
                    oceanCopy[i][j][1] = ' ';
                }
                else if(direction == 'E'){
                    ocean[i][j][1]--;                    //Found a fish so decrementing breed time
                    oceanCopy[i][(j + 1) % OCEANSIZEX][0] = 'F';          //
                    oceanCopy[i][j][0] = ' ';
                    oceanCopy[i][(j + 1) % OCEANSIZEX][1] = ocean[i][j][1]; //copying instance of fish breed time to ocean copy's relative layer
                    oceanCopy[i][j][1] = ' ';
                }
                else if (direction == 'W'){
                    ocean[i][j][1]--;                    //Found a fish so decrementing breed time
                    oceanCopy[i][(j % OCEANSIZEX) - 1][0] = 'F';          //
                    oceanCopy[i][j][0] = ' ';
                    oceanCopy[i][(j % OCEANSIZEX) - 1][1] = ocean[i][j][1]; //copying instance of fish breed time to ocean copy's relative layer
                    oceanCopy[i][j][1] = ' ';
                }
                /*ocean[i][j][1]--;                    //Found a fish so decrementing breed time
                oceanCopy[i][j + 1][0] = 'F';          //
                oceanCopy[i][j][0] = ' ';
                oceanCopy[i][j+1][1] = ocean[i][j][1]; //copying instance of fish breed time to ocean copy's relative layer
                oceanCopy[i][j][1] = ' ';*/
            }
        }
    }
    copyOcean(oceanCopy, ocean);
}


int main() {
    std::string display = "Test";
    sf::RenderWindow window(sf::VideoMode(2000, 2000), "SFML works!");
    sf::Font font;
    font.loadFromFile("/Library/Fonts/AppleMyungjo.ttf");
    sf::Text text(display, font, 11);
    text.setCharacterSize(32);
    text.setPosition(50, 50);
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
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        move();
        display = displayOcean();
    }
    return 0;
}
