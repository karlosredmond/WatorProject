/** Copyright 2017 Karl Redmond, Ger Dobbs
 *ProducerConsumerMain.cpp
 *Author:   Karl Redmond, Ger Dobbs
 *Date:     Thursday,  27 November 2017.
 *License:  GNU General Public License v3.0
 *Brief:    Joint 4th Year Wator simulation Project
**/

#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>

int const OCEANSIZEX = 10;
int const OCEANSIZEY = 10;
int numFish = 0;
int numShark = 1;
char const FISHY = 'F';
char const SHARKY = 'S';
int fishBreedTime = 4;
int sharkBreedTime = 20;
int sharkStarveTime = 7;
char ocean[OCEANSIZEX][OCEANSIZEY][3];
char oceanCopy[OCEANSIZEX][OCEANSIZEY][3];
int xPos, yPos;
std::vector<char> neighbourhood;
std::vector<char> neighbourhoodFishForShark;
std::vector<char> neighbourhoodEmptyForShark;
int const DISPLAYLAYER = 0;
int const BREEDLAYER = 1;
int const STARVELAYER = 2;
typedef boost::mt19937 RNGType;
RNGType rng;
boost::uniform_int<> one_to_six( 0, 20000 );
boost::variate_generator< RNGType, boost::uniform_int<> > dice(rng, one_to_six);

/**
 * \brief Initialize 3 Dimensional Cube. The first layer [0] represents what will be displayed.
 * The second layer [1] represents the breed time for the fish and sharks that exist in layer [0].
 * The third layer[2] represents the starve time for the sharks that exist in layer[0].
 */
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

/**
 * \brief Populate our cube with fish placed at random positions within the first layer,
 * and set the corresponding second layer to the breed time of the fish.
 */
void fillOceanCubeWithFish() {
    for (int i = 0; i < numFish; ++i) {
        xPos = (int) random() % OCEANSIZEX;
        yPos = (int) random() % OCEANSIZEY;
        ocean[xPos][yPos][0] = FISHY;
        ocean[xPos][yPos][1] = '0' + fishBreedTime;
    }
}

/**
 * \brief Populate our cube with sharks placed at random positions within the first layer,
 * and set the corresponding second and third layer to the breed time of the shark, and the starve time of the shark respectively.
 */
void fillOceanCubeWithShark() {
    for (int i = 0; i < numShark; ++i) {
        xPos = dice() % OCEANSIZEX;
        yPos = dice() % OCEANSIZEY;
        ocean[xPos][yPos][0] = SHARKY;
        ocean[xPos][yPos][1] = '0' + sharkBreedTime;
        ocean[xPos][yPos][2] = '0' + sharkStarveTime;
    }
}

/**
 * \brief Initializes ocean with ' ' characters, subsequently populating the ocean with fish and sharks, and storing the breed/starve time.
 */
void fillOcean() {
    initOceanCubes();
    fillOceanCubeWithFish();
    fillOceanCubeWithShark();
    copyOcean(ocean, oceanCopy);
}

/**
 * \brief Displays the first layer of the ocean, showing the positions of the fish and sharks.
 */
std::string displayOcean() {
    std::string temp;
    for (int i = 0; i < OCEANSIZEX; ++i) {
        for (int j = 0; j < OCEANSIZEY; ++j) {
            temp += ocean[i][j][DISPLAYLAYER];
        }
        temp += "\n";
    }
    return temp;
}

void removeShark(int i, int j){
    ocean[i][j][0] = ' ';
    ocean[i][j][1] = ' ';
    ocean[i][j][2] = ' ';
    oceanCopy[i][j][0] = ' ';
    oceanCopy[i][j][1] = ' ';
    oceanCopy[i][j][2] = ' ';
    numShark--;
}

void removeFish(int i, int j){
    oceanCopy[i][j][0] = SHARKY;
    oceanCopy[i][j][1] = '0' + sharkBreedTime;
    oceanCopy[i][j][2] = '0' + sharkStarveTime;
    ocean[i][j][0] = ' ';
    ocean[i][j][1] = ' ';
    ocean[i][j][2] = ' ';
    removeShark(i,j);
    numFish--;
}

char checkNeighbourhoodShark(int i, int j){
    neighbourhoodFishForShark.clear();
    neighbourhoodEmptyForShark.clear();
    if(ocean[(i + OCEANSIZEX -1) % OCEANSIZEX][j][0] == FISHY){
        removeFish((i + OCEANSIZEX -1) % OCEANSIZEX,j);
        return 'D';
    }
    else if(oceanCopy[(i + OCEANSIZEX -1) % OCEANSIZEX][j][0] == ' '){
        neighbourhoodEmptyForShark.push_back('N');
    }
    if(ocean[(i + 1) % OCEANSIZEY][j][0] == FISHY){
        removeFish((i + 1) % OCEANSIZEY, j);
        return 'D';
    }
    else if(oceanCopy[(i + 1) % OCEANSIZEY][j][0] == ' '){
        neighbourhoodEmptyForShark.push_back('S');
    }
    if(ocean[i][(j + 1) % OCEANSIZEY][0] == FISHY){
        removeFish(i, (j + 1) % OCEANSIZEY);
        return 'D';
    }
    else if(oceanCopy[i][(j + 1) % OCEANSIZEY][0] == ' '){
        neighbourhoodEmptyForShark.push_back('E');
    }
    if(ocean[i][(j + OCEANSIZEX -1) % OCEANSIZEX][0] == FISHY){
        removeFish(i, (j + OCEANSIZEX -1) % OCEANSIZEX);
        return 'D';
    }
    else if(oceanCopy[i][(j + OCEANSIZEX -1) % OCEANSIZEX][0] == ' '){
        neighbourhoodEmptyForShark.push_back('W');
    }
    if(!neighbourhoodFishForShark.empty()){
        return neighbourhoodFishForShark[(dice() + neighbourhoodFishForShark.size() -1) % neighbourhoodFishForShark.size()];
    }
    else if(!neighbourhoodEmptyForShark.empty()){
        int testDirectionTwo = (dice() + neighbourhoodEmptyForShark.size() -1) % neighbourhoodEmptyForShark.size();
        return neighbourhoodEmptyForShark[testDirectionTwo];
    }
    else{
        return 'D';
    }
}

char checkNeighbourhoodFish(int i, int j){
    neighbourhood.clear();
    if(oceanCopy[(i + OCEANSIZEX -1) % OCEANSIZEX][j][0] == ' '){
        neighbourhood.push_back('N');
    }
    if(oceanCopy[(i + 1) % OCEANSIZEY][j][0] == ' '){
        neighbourhood.push_back('S');
    }
    if(oceanCopy[i][(j + 1) % OCEANSIZEY][0] == ' '){
        neighbourhood.push_back('E');
    }
    if(oceanCopy[i][(j + OCEANSIZEX -1) % OCEANSIZEX][0] == ' '){
        neighbourhood.push_back('W');
    }
    if(neighbourhood.empty()){
        return 'D';
    }
    else {
        int testDirectionTwo = (dice() + neighbourhood.size() -1) % neighbourhood.size();
        return neighbourhood[testDirectionTwo];
    }
}

void moveCreature(int i, int j, int direction, char charDirection, int breedTime, int starveTime, char type){
    int x = i,y = j;
    if (charDirection == 'N' || charDirection == 'S') {
        x = direction;
    }
    else{
        y = direction;
    }
    if (ocean[i][j][1] == '0') {
        oceanCopy[i][j][1] = '0' + breedTime;
        oceanCopy[i][j][2] = '0' + starveTime;
        oceanCopy[x][y][1] = '0' + breedTime;
        oceanCopy[x][y][2] = '0' + starveTime;
    } else {
        oceanCopy[i][j][0] = ' ';
        oceanCopy[i][j][1] = ' ';
        oceanCopy[i][j][2] = ' ';
        oceanCopy[x][y][1] = ocean[i][j][1]; //copying instance of fish breed time to ocean copy's relative layer
        oceanCopy[x][y][2] = ocean[i][j][2];
    }
    oceanCopy[x][y][0] = type;
}

void move() {
    char direction;
    for (int i = 0; i < OCEANSIZEX; ++i) {
        for (int j = 0; j < OCEANSIZEY; ++j) {
            if (ocean[i][j][0] == FISHY) {
                direction = checkNeighbourhoodFish(i, j);
                if (direction != 'D') {
                    ocean[i][j][1]--;
                    if (ocean[i][j][1] == '0') {
                        numFish++;
                    }
                }
                if (direction == 'N') {
                    moveCreature(i, j, (i + OCEANSIZEX - 1) % OCEANSIZEX, 'N', fishBreedTime, 0, FISHY);
                } else if (direction == 'S') {
                    moveCreature(i, j, (i + 1) % OCEANSIZEY, 'S', fishBreedTime, 0, FISHY);
                } else if (direction == 'E') {
                    moveCreature(i, j, (j + 1) % OCEANSIZEY, 'E', fishBreedTime, 0, FISHY);
                } else if (direction == 'W') {
                    moveCreature(i, j, (j + OCEANSIZEX - 1) % OCEANSIZEX, 'W', fishBreedTime, 0, FISHY);
                }
            }else if (ocean[i][j][0] == SHARKY) {
                direction = checkNeighbourhoodShark(i, j);
                if (direction != 'D') {
                    ocean[i][j][1]--;
                    ocean[i][j][2]--;
                }
                if (ocean[i][j][2] == '0') {
                    removeShark(i,j);
                }
                else if (direction == 'N') {
                    moveCreature(i, j, (i + OCEANSIZEX - 1) % OCEANSIZEX, 'N', sharkBreedTime, sharkStarveTime, SHARKY);
                } else if (direction == 'S') {
                    moveCreature(i, j, (i + 1) % OCEANSIZEY, 'S', sharkBreedTime, sharkStarveTime, SHARKY);
                } else if (direction == 'E') {
                    moveCreature(i, j, (j + 1) % OCEANSIZEY, 'E', sharkBreedTime, sharkStarveTime, SHARKY);
                } else if (direction == 'W') {
                    moveCreature(i, j, (j + OCEANSIZEX - 1) % OCEANSIZEX, 'W', sharkBreedTime, sharkStarveTime, SHARKY);
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
    font.loadFromFile("/Library/Fonts/AppleMyungjo.ttf" /*"/Applications/Microsoft Word.app/Contents/Resources/Fonts/Wingdings 2.ttf"*/);
    sf::Text text(display, font, 11);
    sf::Text numFishyWishy(std::to_string(numFish), font, 11);
    sf::Text numSharkyWarky(std::to_string(numShark), font, 11);
    text.setCharacterSize(25);
    text.setPosition(0, 0);
    numFishyWishy.setCharacterSize(40);
    numFishyWishy.setPosition(0, 350);
    numSharkyWarky.setCharacterSize(40);
    numSharkyWarky.setPosition(0, 400);
    fillOcean();
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        text.setString(display);
        numFishyWishy.setString(std::to_string(numFish));
        numSharkyWarky.setString(std::to_string(numShark));
        window.clear();
        window.draw(text);
        window.draw(numFishyWishy);
        window.draw(numSharkyWarky);
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        move();
        display = displayOcean();
    }
    return 0;
}