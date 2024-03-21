/**
 * @file main.c
 * @author TOUATI Damien, GUNEY David
 * @brief This file is the main, it regroups all files to build the project
 * @version 0.1
 * @date 2024-01-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "grid.h"
#include "monster.h"
#include "player.h"
#include "graph.h"
#include "game.h"

int main(int argc, char const *argv[]){
    MLV_create_window("Towerdefense", "", WINDOW_WIDTH, WINDOW_HEIGHT);
    MLV_Image *grass = MLV_load_image("./pictures/grass.jpg");
    MLV_Font *font = MLV_load_font("fonts/arial.ttf", 30);
    if(grass == NULL || font == NULL){
        fprintf(stderr, "Error while loading\n");
        return 1;
    }
    MLV_resize_image(grass, (WINDOW_WIDTH - 200), WINDOW_HEIGHT);

    srand(time(NULL));
    enum Grid grid[WIDTH][LENGHT];
    int mouse_x, mouse_y;
    int end_generation = 1;
    Player player = initPlayer();
    Road road = initRoad();
    Position monster_nest, turns[20];
    enum Direction turn_direction[20];

    //generating the grid
    while(end_generation){
        if(generation(grid, &monster_nest))
            end_generation = 0;
    }

    //getting the road path and corners
    completeRoad(grid, monster_nest, &road);
    int size_turns = 0;
    for(int i = 0; i < road.size; ++i)
        if(isTurn(grid, road.positionArray[i].x, road.positionArray[i].y)) {
            turn_direction[size_turns] = road.directionArray[i];
            turns[size_turns] = road.positionArray[i];
            size_turns++;
        }
    window(grass, grid, font);
    MLV_actualise_window();

    do{
        MLV_wait_mouse(&mouse_x, &mouse_y);
    }while(mouse_x < 1125 || mouse_x > 1315 || mouse_y > 75 || mouse_y < 5);

    //the game can begin
    game(&road, &player, grass, monster_nest, grid, turns, turn_direction, size_turns, font);
    
    free(road.directionArray);
    free(road.positionArray);
    MLV_free_image(grass);
    MLV_free_font(font);
    MLV_free_window();
    return 0;
}
