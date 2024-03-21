/**
 * @file grid.h
 * @author TOUATI Damien, GUNEY David
 * @brief This file is the header for the grid of the game
 * @version 0.1
 * @date 2024-01-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __GRID__
#define __GRID__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

/**
 * @brief Grid width
 * 
 */
#define LENGHT 28

/**
 * @brief Grid height
 * 
 */
#define WIDTH 22

/**
 * @brief Minimal number of turns in the path generated
 * 
 */
#define MINIMAL_TURNS 7

/**
 * @brief Minimal lenght of the generating path
 * 
 */
#define MINIMAL_ROAD_LENGHT 75

/**
 * @brief Defines a position in the grid
 * 
 */
typedef struct{
    int x;
    int y;
}Position;

/**
 * @brief Defines the type of a grid cell
 * 
 */
enum Grid{
    EMPTY,
    PLAYER_SIDE,
    MONSTER_SIDE,
    ROAD,
    TOWER,
    GEM_TOWER
};

/**
 * @brief Defines the cardinals directions
 * 
 */
enum Direction{
    NORTH, 
    EAST, 
    SOUTH, 
    WEST,
    NONE
};

/**
 * @brief Defines a grid road that is compose with a position linked with the direction of the next position
 * 
 */
typedef struct road{
    Position *positionArray;
    enum Direction *directionArray;
    int size;
}Road;

/**
 * @brief print the grid
 * 
 * @param tab
*/
void affiche_tab_2D(enum Grid tab[WIDTH][LENGHT]);

/**
 * @brief initialize an empty grid
 * 
 * @param grid
*/
void init_grid(enum Grid grid[WIDTH][LENGHT]);

/**
 * @brief Returns 1 if the positions are equals otherwise 0
 * 
 * @param a 
 * @param b 
 * @return int 
 */
int isPositionEquals(Position a, Position b);

/**
 * @brief Inits a road
 * 
 * @return Road 
 */
Road initRoad();

/**
 * @brief returns the Manhattan_Distance between two places in the grid
 * 
 * @param A
 * @param B
 * 
 * @return int
*/
int Manhattan_Distance(Position A, Position B);

/**
 * @brief returns a "Position" more than 3 of distance with the edge
 * 
 * @return Position
*/
Position random_case();

/**
 * @brief returns the extent in one direction from a given point
 * 
 * @param A
 * @param direction
 * @param grid
 * 
 * @return int
*/
int calc_extent(Position A, enum Direction direction, enum Grid grid[WIDTH][LENGHT]);

/**
 * @brief returns the index of the array where we stop
 * 
 * @param array
 * @param lenght
 * 
 * @return int
*/
int pickRandom(int *array, int lenght);

/**
 * @brief Returns the max value between a and b.
 * 
 * @param a
 * @param b
 * 
 * @return int
*/
int maximum(int a, int b);

/**
 * @brief Build the road in a given direction into the grid from a given "Position" and returns the last "Position" of the road
 * 
 * @param grid
 * @param startingPoint
 * @param direction
 * @param road_lenght
 * 
 * @return Position
*/
Position buildRoad(enum Grid grid[WIDTH][LENGHT], Position startingPoint ,enum Direction direction, int road_lenght);

/**
 * @brief returns the right direction of a given direction
 * 
 * @param direction
 * 
 * @return Direction
*/
enum Direction rightDirection(enum Direction direction);

/**
 * @brief returns the left direction of a given direction
 * 
 * @param direction
 * 
 * @return Direction
*/
enum Direction leftDirection(enum Direction direction);

/**
 * Returns the opposite direction of the given one
 * @param direction
 * @return Direction
 */
enum Direction oppositeDirection(enum Direction direction);

/**
 * Returns direction to the path
 * @param road
 * @param position
 * @return Direction
 */
enum Direction findDirection(Road *road, Position position);

/**
 * Changes position by moving from one position in the given direction
 * @param position
 * @param direction
 */
void applyDirection(Position *position, enum Direction direction);

/**
 * Returns the last position on the grid of a straight path from a position in a certain direction
 * @param grid
 * @param direction
 * @param startingPoint
 * @param road
 * @return Position
 */
Position parcoursStraight(enum Grid grid[WIDTH][LENGHT], enum Direction direction, Position startingPoint, Road *road);

/**
 * Returns LEFT or RIGHT depending on the next turn direction otherwise NONE.
 * @param grid
 * @param direction
 * @param startingPoint
 * @return Direction
 */
enum Direction findLeftOrRight(enum Grid grid[WIDTH][LENGHT], enum Direction direction, Position startingPoint);

/**
 * Completing the Road struct in parameters
 * @param grid
 * @param monsterSide
 * @param road
 */
void completeRoad(enum Grid grid[WIDTH][LENGHT], Position monsterSide, Road *road);

/**
 * @brief returns 1 if the grid[i][j] is a turn otherwise 0.
 * 
 * @param grid
 * @param i
 * @param j
 * 
 * @return int
*/
int isTurn(enum Grid grid[WIDTH][LENGHT], int i, int j);

/**
 * @brief returns 1 if the road build on the grid if valid otherwise 0.
 * 
 * @param grid
 * 
 * @return int
*/
int checkValidRoad(enum Grid grid[WIDTH][LENGHT]);

/**
 * @brief Building the road of the grid and returns 1 if the generation is fine
 *  and get the position of the monster side otherwise 0.
 * 
 * @param grid
 * @param monsterSide
 * 
 * @return int
*/
int generation(enum Grid grid[WIDTH][LENGHT], Position *monsterSide);

#endif
