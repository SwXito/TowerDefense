/**
 * @file player.c
 * @author TOUATI Damien, GUNEY David
 * @brief This file handles the player
 * @version 0.1
 * @date 2024-01-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "player.h"

#include <graph.h>

Player initPlayer(void){
    Player player;
    player.max_mana = 2000;
    player.current_mana = 150;
    player.level = 0;
    return player;
}

void increaseReserve(Player * player){
    if(player->current_mana > 600 * pow(1.4, player->level) && player->level < 100){
        player->current_mana -= 600 * pow(1.4, player->level);
        player->level++;
        player->max_mana = 2000 * pow(1.4, player->level);
    }
}

void printPlayerInfos(Player player){
    printf("Player infos :\n");
    printf("Current mana : %lf\n", player.current_mana);
    printf("Max mana : %lf\n", player.max_mana);
    printf("Reserve level : %d\n", player.level);
    printf("\n");
}

void placeTower(Player * player, enum Grid grid[WIDTH][LENGHT], int *tower_number, int mouse_x, int mouse_y) {
    if(mouse_x >= 0 && mouse_x <= WINDOW_WIDTH - WIDTH_SHIFT && mouse_y >= HEIGHT_SHIFT && mouse_y <= WINDOW_HEIGHT) {
        Position pos;
        //getting the coordinates in the grid
        pos.y = mouse_x / ((WINDOW_WIDTH - WIDTH_SHIFT) / LENGHT);
        pos.x = (mouse_y - HEIGHT_SHIFT) / ((WINDOW_HEIGHT - HEIGHT_SHIFT) / WIDTH);
        //if the place is free
        if(grid[pos.x][pos.y] == EMPTY) {
            if(*tower_number >= 3){
                if(player->current_mana > 100 * pow(2, *tower_number-3))
                    player->current_mana -= 100 * pow(2, *tower_number-3);
                else
                    return;
            }
            grid[pos.x][pos.y] = TOWER;
            (*tower_number)++;
        }
    }
}
