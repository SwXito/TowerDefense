/**
 * @file player.h
 * @author TOUATI Damien, GUNEY David
 * @brief This file is the header for a Player
 * @version 0.1
 * @date 2024-01-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __PLAYER__
#define __PLAYER__

#include "grid.h"

/**
 * @brief Defines a Player
 * 
 */
typedef struct player{
    double current_mana;
    double max_mana;
    int level;
}Player;

/**
 *
 * @return Player
 */
Player initPlayer(void);

/**
 *
 * @param player
 */
void increaseReserve(Player * player);

/**
 *
 * @param player
 */
void printPlayerInfos(Player player);

/**
 *
 * @param player
 * @param grid
 * @param tower_number
 * @param mouse_x
 * @param mouse_y
 */
void placeTower(Player * player, enum Grid grid[WIDTH][LENGHT], int *tower_number, int mouse_x, int mouse_y);

#endif
