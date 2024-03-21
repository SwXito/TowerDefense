/**
 * @file grid.c
 * @author TOUATI Damien, GUNEY David
 * @brief This file handles the grid of the game
 * @version 0.1
 * @date 2024-01-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "grid.h"

void affiche_tab_2D(enum Grid tab[WIDTH][LENGHT]){
    for (int i = 0; i < WIDTH; i++){
        for (int j = 0; j < LENGHT; j++)
            printf("%d ", tab[i][j]);
        printf("\n");
    }
    printf("\n");
}

int isPositionEquals(Position a, Position b){
    if(a.x == b.x && a.y == b.y)
        return 1;
    return 0;
}

void init_grid(enum Grid grid[WIDTH][LENGHT]){
    for(int i = 0; i < WIDTH; i++)
        for(int j = 0; j < LENGHT; j++)
            grid[i][j] = EMPTY;
}

Road initRoad(){
    Road road;
    road.directionArray = (enum Direction *)malloc(sizeof(enum Direction) * 200);
    road.positionArray = (Position *)malloc(sizeof(Position) * 200);
    road.size = 0;
    return road;
}

int Manhattan_Distance(Position A, Position B){
    return abs(A.x - B.x) + abs(A.y - B.y);
}

Position random_case(){
    int distance_from_edge = 3;
    
    Position res;
    res.x = 0;
    res.y = 0;
    while(res.x < distance_from_edge || res.x > WIDTH - distance_from_edge || res.y < distance_from_edge || res.y > LENGHT - distance_from_edge){
        res.x = rand() % LENGHT;
        res.y = rand() % WIDTH; 
    }
    return res;
}

int calc_extent(Position A, enum Direction direction, enum Grid grid[WIDTH][LENGHT]){
    int res = 0;
    switch (direction){
    case NORTH:
        while((A.x > 2) && (grid[A.x-1][A.y-2] == EMPTY) && (grid[A.x-2][A.y-1] == EMPTY) && (grid[A.x-3][A.y] == EMPTY) && (grid[A.x-2][A.y+1] == EMPTY) && (grid[A.x-1][A.y+2] == EMPTY)){
            A.x--;
            res++;
        }
        break;
    case EAST:
        while((A.y < LENGHT-3) && (grid[A.x-2][A.y+1] == EMPTY) && (grid[A.x-1][A.y+2] == EMPTY) && (grid[A.x][A.y+3] == EMPTY) && (grid[A.x+1][A.y+2] == EMPTY) && (grid[A.x+2][A.y+1] == EMPTY)){
            A.y++;
            res++;
        }
        break;
    case SOUTH:
        while((A.x < WIDTH-3) && (grid[A.x+1][A.y-2] == EMPTY) && (grid[A.x+2][A.y-1] == EMPTY) && (grid[A.x+3][A.y] == EMPTY) && (grid[A.x+2][A.y+1] == EMPTY) && (grid[A.x+1][A.y+2] == EMPTY)){
            A.x++;
            res++;
        }
        break;
    case WEST:
        while((A.y > 2) && (grid[A.x-2][A.y-1] == EMPTY) && (grid[A.x-1][A.y-2] == EMPTY) && (grid[A.x][A.y-3] == EMPTY) && (grid[A.x+1][A.y-2] == EMPTY) && (grid[A.x+2][A.y-1] == EMPTY)){
            A.y--;
            res++;
        }
        break;
    case NONE:
        return -1;
    }
    return res;
}

int pickRandom(int *array, int lenght){
    int index = 0, random_value, sum = 0;
    for(int i = 0; i < lenght; i++)
        sum += array[i];
    if(sum == 0)
        return -1;
    random_value = rand() % sum;
    for(int i = 0; i < lenght; i++){
        if(random_value < array[i]){
            index = i;
            break;
        }
        random_value -= array[i];
    }
    return index;
}

int pickValues(){
    int randomValue = rand() % 4;
    if(randomValue == 0)
        return 0;
    return 1;
}

int maximum(int a, int b){
    if(a > b)
        return a;
    return b;
}

Position buildRoad(enum Grid grid[WIDTH][LENGHT], Position startingPoint ,enum Direction direction, int road_size){
    //build a straight path in the direction
    switch (direction){
    case NORTH:
        for(int i = 0; i < road_size; i++)
            grid[--startingPoint.x][startingPoint.y] = ROAD;
        break;
    case EAST:
        for(int i = 0; i < road_size; i++)
            grid[startingPoint.x][++startingPoint.y] = ROAD;
        break;
    case SOUTH:
        for(int i = 0; i < road_size; i++)
            grid[++startingPoint.x][startingPoint.y] = ROAD;
        break;
    case WEST:
        for(int i = 0; i < road_size; i++)
            grid[startingPoint.x][--startingPoint.y] = ROAD;
        break;
    case NONE:
        break;
    }
    return startingPoint;
}

enum Direction rightDirection(enum Direction direction){
    if(direction == WEST)
        return NORTH;
    if(direction == NONE)
        return NONE;
    return direction + 1;
}

enum Direction leftDirection(enum Direction direction){
    if(direction == NORTH)
        return WEST;
    if(direction == NONE)
        return NONE;
    return direction - 1;
}

enum Direction oppositeDirection(enum Direction direction){
    switch (direction){
    case NORTH:
        return SOUTH;
    case SOUTH:
        return NORTH;
    case EAST:
        return WEST;
    case WEST:
        return EAST;
    default: return NONE;
    }
}

enum Direction findDirection(Road *road, Position position){
    for(int i = 0; i < road->size; i++)
        if(road->positionArray[i].x == position.x && road->positionArray[i].y == position.y)
            return road->directionArray[i];
    return NONE;
}


void applyDirection(Position *position, enum Direction direction){
    if(direction == NONE){
        fprintf(stderr, "Error applyDirection NONE\n");
        return;
    }
    static int tabx[] = {-1, 0, 1, 0};
    static int taby[] = {0, 1, 0, -1};
    position->x += tabx[direction];
    position->y += taby[direction];
}

enum Direction findLeftOrRight(enum Grid grid[WIDTH][LENGHT], enum Direction direction, Position startingPoint){
    //looking for left and right cell
    Position leftPosition = startingPoint;
    Position rightPosition = startingPoint;
    applyDirection(&leftPosition, leftDirection(direction));
    applyDirection(&rightPosition, rightDirection(direction));
    //cheking thoses cells
    if(grid[leftPosition.x][leftPosition.y] == ROAD)
        return leftDirection(direction);
    if(grid[rightPosition.x][rightPosition.y] == ROAD)
        return rightDirection(direction);
    return NONE;
}

Position parcoursStraight(enum Grid grid[WIDTH][LENGHT], enum Direction direction, Position startingPoint, Road *road){
    int end = 1;
    //goes straight the path until its an empty grid cell
    while(end){
        applyDirection(&startingPoint, direction);
        if(grid[startingPoint.x][startingPoint.y] == EMPTY)
            end = 0; 
        else{
            road->directionArray[road->size] = direction;
            road->size++;
            road->positionArray[road->size] = startingPoint;
        }
    }
    //goes back on the last cell of the path
    applyDirection(&startingPoint, oppositeDirection(direction));
    return startingPoint;
}

void completeRoad(enum Grid grid[WIDTH][LENGHT], Position monsterSide, Road *road){
    enum Direction direction = NONE;
    static int tabx[] = {-1, 0, 1, 0};
    static int taby[] = {0, 1, 0, -1};
    road->positionArray[0] = monsterSide;
    //finding the first direction
    for(enum Direction i = NORTH; i < NONE; i++)
        if(grid[monsterSide.x + tabx[i]][monsterSide.y + taby[i]] == ROAD)
            direction = i;
    monsterSide = parcoursStraight(grid, direction, monsterSide, road);
    direction = findLeftOrRight(grid, direction, monsterSide);
    //reads straight paths to the last
    while(direction != NONE){
        monsterSide = parcoursStraight(grid, direction, monsterSide, road);
        direction = findLeftOrRight(grid, direction, monsterSide);
    }
}

int isTurn(enum Grid grid[WIDTH][LENGHT], int i, int j){
    if(grid[i-1][j] == ROAD && grid[i][j+1] == ROAD) //checking from NORTH to EAST
        return 1;
    if(grid[i-1][j] == ROAD && grid[i][j-1] == ROAD) //checking from NORTH to WEST
        return 1;
    if(grid[i+1][j] == ROAD && grid[i][j+1] == ROAD) //checking from SOUTH to EAST
        return 1;
    if(grid[i+1][j] == ROAD && grid[i][j-1] == ROAD) //checking from SOUTH to WEST
        return 1;
    return 0;
}

int checkValidRoad(enum Grid grid[WIDTH][LENGHT]){
    int turns_count = 0;
    int road_lenght = 0;
    //couting turns and the lenght of the road
    for(int i = 0; i < WIDTH; i++)
        for(int j = 0; j < LENGHT; j++)
            if(grid[i][j] == ROAD){
                road_lenght ++;
                if(isTurn(grid, i, j))
                    turns_count++;
            }
    //checking if the road is valid
    if(turns_count == MINIMAL_TURNS && road_lenght > MINIMAL_ROAD_LENGHT)
        return 1;
    return 0;
}

int generation(enum Grid grid[WIDTH][LENGHT], Position *monsterSide){
    init_grid(grid);
    Position start_road = random_case(); //It's the starting point of the road and where the player side will be.
    enum Direction direction;
    int extent;
    int road_size;
    int index;
    int cardinal_ranges[4] = {calc_extent(start_road, NORTH, grid),
        calc_extent(start_road, EAST, grid),
        calc_extent(start_road, SOUTH, grid),
        calc_extent(start_road, WEST, grid)};
    int destinations[4] = {NORTH, EAST, SOUTH, WEST};
    int new_cardinal_ranges[2];
    grid[start_road.x][start_road.y] = PLAYER_SIDE;
    index = pickRandom(cardinal_ranges, 4);
    direction = destinations[index];
    extent = cardinal_ranges[index];
    while(extent > 2){
        road_size = 0;
        for(int i = 0; i < extent; i++)
            road_size += pickValues();
        road_size = maximum(road_size, 3);
        start_road = buildRoad(grid, start_road, direction, road_size); //Building the road
        enum Direction new_destinations[2] = {leftDirection(direction), rightDirection(direction)};
        new_cardinal_ranges[0] = calc_extent(start_road, new_destinations[0], grid); //Replacing only the first and second value and from now every manipulations will be on theses two only 
        new_cardinal_ranges[1] = calc_extent(start_road, new_destinations[1], grid);
        index = pickRandom(new_cardinal_ranges, 2); //Selecting a direction between the two new ones.
        if(index == -1)
            return 0;
        extent = new_cardinal_ranges[index];
        direction = new_destinations[index];
    }
    if(checkValidRoad(grid)){ //Checking if the grid is fine.
        grid[start_road.x][start_road.y] = MONSTER_SIDE;
        *monsterSide = start_road;
        return 1;
    }
    return 0;
}