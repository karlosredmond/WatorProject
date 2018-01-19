/* Copyright 2017 Karl Redmond, Ger Dobbs
 WatorProjectSFML.cpp
 Author:   Karl Redmond, Ger Dobbs
 Date:     Thursday,  27 November 2017.
 License:  GNU General Public License v3.0
 Brief:    Joint 4th Year Wator simulation Project. We implemented this project using two 3-dimensional arrays, 
            the first of which (ocean[][][]) is used to display the current representaion of the denizens living in Wa-tor,
            the second (oceanCopy[][][])is used to keep track of the moves for the current iteration. 
*/

#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>

int const OCEANSIZEX = 20;
int const OCEANSIZEY = 20;
int numFish = 200;
int numShark = 50;
char const FISHY = '.';
char const SHARKY = '0';
int fishBreedTime = 5;
int sharkBreedTime = 5;
int sharkStarveTime = 5;
char ocean[OCEANSIZEX][OCEANSIZEY][3]; /** Used for displaying ocean **/
char oceanCopy[OCEANSIZEX][OCEANSIZEY][3]; /** Used for keeping track of moves**/
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
 * \brief Initialize 3 Dimensional Cube. 
 * details The first layer [0] represents what will be displayed.
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

/**
    *\brief This method copies any of the moves from the latest oceanCopy update, into the ocean. 
    * \details This will be used to display
    * the moves on screen. All layers of the cube are copied.
    * param The oceanCopy Layer to be copied from and the ocean layer to be copied to
**/
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
 * \brief Fill the ocean with fish.
 * \details Populate our cube with fish placed at random positions within the first layer,
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
 * \brief Populate our cube with sharks.
 * \details Put the sharks at random positions within the first layer,
 *  and set the corresponding second and third layer to the breed time of the shark, and the starve time of the shark respectively.
 **/
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
 * \brief Fill Ocean with sharks and fish.
 * details First Initializes the ocean with ' ' characters, subsequently populating the ocean with fish and sharks,
 * and storing the breed/starve time.This is then copied to oceanCopy.
 */
void fillOcean() {
    initOceanCubes();
    fillOceanCubeWithFish();
    fillOceanCubeWithShark();
    copyOcean(ocean, oceanCopy);
}

/**
 * \brief Writes the string to be displayed, showing the positions of the fish and sharks. Also counts number of fish/sharks for
 this iteration.
 */
std::string displayOcean() {
    std::string temp;
    numFish = 0;
    numShark = 0;
    for (int i = 0; i < OCEANSIZEX; ++i) {
        for (int j = 0; j < OCEANSIZEY; ++j) {
            if(ocean[i][j][0] == FISHY){
                numFish++;
            }
            else if(ocean[i][j][0] == SHARKY){
                numShark++;
            }
            temp += ocean[i][j][DISPLAYLAYER];
        }
        temp += "\n";
    }
    return temp;
}

/**
    \*brief If the shark has starved(sharkStarveTime == 0), this method removes the shark from both the copy and the main cube.
**/
void removeShark(int i, int j){
    ocean[i][j][0] = ' ';
    ocean[i][j][1] = ' ';
    ocean[i][j][2] = ' ';
    oceanCopy[i][j][0] = ' ';
    oceanCopy[i][j][1] = ' ';
    oceanCopy[i][j][2] = ' ';
}

/**
    Eats a fish in the given direction
**/
char eatFish(int i, int j, char direction){
    if(direction == 'N'){
        oceanCopy[(i + OCEANSIZEX - 1) % OCEANSIZEX][j][0] = SHARKY;
        oceanCopy[(i + OCEANSIZEX - 1) % OCEANSIZEX][j][1] = ocean[i][j][1];
        oceanCopy[(i + OCEANSIZEX - 1) % OCEANSIZEX][j][2] = '0' + sharkStarveTime;
    }
    else if(direction == 'S') {
        oceanCopy[(i + 1) % OCEANSIZEY][j][0] = SHARKY;
        oceanCopy[(i + 1) % OCEANSIZEY][j][1] = ocean[i][j][1];
        oceanCopy[(i + 1) % OCEANSIZEY][j][2] = '0' + sharkStarveTime;
    }
    else if(direction == 'E'){
        oceanCopy[i][(j + 1) % OCEANSIZEY][0] = SHARKY;
        oceanCopy[i][(j + 1) % OCEANSIZEY][1] = ocean[i][j][1];
        oceanCopy[i][(j + 1) % OCEANSIZEY][2] = '0' + sharkStarveTime;
    }
    else if(direction == 'W'){
        oceanCopy[i][(j + OCEANSIZEX -1) % OCEANSIZEX][0] = SHARKY;
        oceanCopy[i][(j + OCEANSIZEX -1) % OCEANSIZEX][1] = ocean[i][j][1];
        oceanCopy[i][(j + OCEANSIZEX -1) % OCEANSIZEX][2] = '0' + sharkStarveTime;
    }
    oceanCopy[i][j][0] = ' ';
    oceanCopy[i][j][1] = ' ';
    oceanCopy[i][j][2] = ' ';
    return direction;
}

/**
    Looks for empty space or fish in surrounding index positions. These positions are pushed onto a character vector, and later used to 
    determine a random direction in which the shark should move.
**/
char checkNeighbourhoodShark(int i, int j){
    neighbourhoodFishForShark.clear();
    neighbourhoodEmptyForShark.clear();
    if(ocean[(i + OCEANSIZEX -1) % OCEANSIZEX][j][0] == FISHY || oceanCopy[(i + OCEANSIZEX -1) % OCEANSIZEX][j][0] == FISHY){
        neighbourhoodFishForShark.push_back('N');
    }
    else if(oceanCopy[(i + OCEANSIZEX -1) % OCEANSIZEX][j][0] == ' '){
        neighbourhoodEmptyForShark.push_back('N');
    }
    if(ocean[(i + 1) % OCEANSIZEY][j][0] == FISHY || oceanCopy[(i + 1) % OCEANSIZEY][j][0] == FISHY){
        neighbourhoodFishForShark.push_back('S');
    }
    else if(oceanCopy[(i + 1) % OCEANSIZEY][j][0] == ' '){
        neighbourhoodEmptyForShark.push_back('S');
    }
    if(ocean[i][(j + 1) % OCEANSIZEY][0] == FISHY || oceanCopy[i][(j + 1) % OCEANSIZEY][0] == FISHY){
        neighbourhoodFishForShark.push_back('E');
    }
    else if(oceanCopy[i][(j + 1) % OCEANSIZEY][0] == ' '){
        neighbourhoodEmptyForShark.push_back('E');
    }
    if(ocean[i][(j + OCEANSIZEX -1) % OCEANSIZEX][0] == FISHY || oceanCopy[i][(j + OCEANSIZEX -1) % OCEANSIZEX][0] == FISHY){
        neighbourhoodFishForShark.push_back('W');
    }
    else if(oceanCopy[i][(j + OCEANSIZEX -1) % OCEANSIZEX][0] == ' '){
        neighbourhoodEmptyForShark.push_back('W');
    }
    if(!neighbourhoodFishForShark.empty()){
        return eatFish(i, j, neighbourhoodFishForShark[(dice() + neighbourhoodFishForShark.size() -1) % neighbourhoodFishForShark.size()]);
    }
    else if(!neighbourhoodEmptyForShark.empty()){
        return neighbourhoodEmptyForShark[(dice() + neighbourhoodEmptyForShark.size() -1) % neighbourhoodEmptyForShark.size()];
    }
    else{
        return 'D';
    }
}
/**
    Looks for empty space in surrounding index'. These positions are pushed onto a vector and later used to determine a random direction
    in which the fish should move.
**/
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
        return neighbourhood[(dice() + neighbourhood.size() -1) % neighbourhood.size()];
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
        if(type == SHARKY){
            oceanCopy[i][j][0] = type;
        }
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
        ocean[i][j][0] = ' ';
        ocean[i][j][1] = ' ';
        ocean[i][j][2] = ' ';
        ocean[x][y][0] = ' ';
        ocean[x][y][1] = ' ';
        ocean[x][y][2] = ' ';
    }
    oceanCopy[x][y][0] = type;
}

void move() {
    char direction;
    for (int i = 0; i < OCEANSIZEX; ++i) {
        for (int j = 0; j < OCEANSIZEY; ++j) {
            if (ocean[i][j][0] == SHARKY) {
                direction = checkNeighbourhoodShark(i, j);
                if (direction != 'D') { // If the shark can move(Not surrounded by sharks)
                    ocean[i][j][1]--;
                    ocean[i][j][2]--;
                }
                if (ocean[i][j][2] == '0') { //If Shark has Starved
                        removeShark(i, j);
                }
                else if (direction == 'N') {
                    moveCreature(i, j, (i + OCEANSIZEX - 1) % OCEANSIZEX, 'N', sharkBreedTime, sharkStarveTime,
                                     SHARKY);
                } else if (direction == 'S') {
                    moveCreature(i, j, (i + 1) % OCEANSIZEY, 'S', sharkBreedTime, sharkStarveTime, SHARKY);
                } else if (direction == 'E') {
                    moveCreature(i, j, (j + 1) % OCEANSIZEY, 'E', sharkBreedTime, sharkStarveTime, SHARKY);
                } else if (direction == 'W') {
                    moveCreature(i, j, (j + OCEANSIZEX - 1) % OCEANSIZEX, 'W', sharkBreedTime, sharkStarveTime,
                                 SHARKY);
                }
            }
            else if (ocean[i][j][0] == FISHY) {
                direction = checkNeighbourhoodFish(i, j);
                if (direction != 'D') { // If the fish can move (not surrounded by shark or fish)
                    ocean[i][j][1]--;
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
    sf::Text text(display, font, 2);
    sf::Text numFishyWishy(std::to_string(numFish), font, 11);
    sf::Text numSharkyWarky(std::to_string(numShark), font, 11);
    text.setCharacterSize(25);
    text.setPosition(0, 0);
    numFishyWishy.setCharacterSize(40);
    numFishyWishy.setPosition(0, 350);
    numSharkyWarky.setCharacterSize(40);
    numSharkyWarky.setPosition(0, 400);
    fillOcean();
    window.display();
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        text.setString(display);
        numFishyWishy.setString("\t\t\t\tNum Fish = " + std::to_string(numFish));
        numSharkyWarky.setString("\t\t\t\tNum Shark = " + std::to_string(numShark));
        window.clear();
        window.draw(text);
        window.draw(numFishyWishy);
        window.draw(numSharkyWarky);
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        move();
        display = displayOcean();
    }
    return 0;
}
