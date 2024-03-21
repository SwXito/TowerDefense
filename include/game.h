/**
 * @file game.h
 * @author TOUATI Damien, GUNEY David
 * @brief This file is the header for the game actions
 * @version 0.1
 * @date 2024-01-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __GAME__
#define __GAME__

#define _POSIX_C_SOURCE 200809L
#define MAX_WAVE 200
#define WAVE_TIME 35
#define MAX_TOWER 100

#include "graph.h"

/**
 * @brief Handles the left click events in the game.
 *
 * This function is responsible for managing the left click events in the game. It checks the state of the left mouse button
 * and performs different actions based on the state. If the button is released, it resets the pressed state and draws a circle
 * if a gem is selected. If the button is pressed, it sets the pressed state, gets the mouse position and performs actions based
 * on whether a tower is being placed or a gem is selected. If neither of these conditions are met, it calls the interaction function.
 *
 * @param list An array of Wave structures representing the waves in the game.
 * @param gem_level A pointer to an integer representing the level of the gem.
 * @param pressed A pointer to an integer representing whether the mouse button is pressed.
 * @param gems A pointer to a GemList structure representing the list of gems.
 * @param player A pointer to a Player structure representing the player.
 * @param wave_number A pointer to an unsigned integer representing the wave number.
 * @param monster_nest A Position structure representing the position of the monster nest.
 * @param wave_duration A pointer to an integer representing the wave duration.
 * @param tower_number A pointer to an integer representing the number of towers.
 * @param grid A 2D array of Grid enums representing the game grid.
 * @param placing_tower A pointer to an integer representing whether a tower is being placed.
 * @param road A pointer to a Road structure representing the road.
 * @param gem_selected A pointer to an integer representing whether a gem is selected.
 * @param gem A pointer to a Gem structure representing the gem.
 * @param gem_number A pointer to an integer representing the number of gems.
 * @param tower_list An array of Tower structures representing the list of towers.
 * @param gem_tower A pointer to an integer representing the number of gem towers.
 * @param shot An array of Pos_monster structures representing the shots.
 */
void left_click(Wave list[], int *gem_level, int *pressed, GemList *gems, Player *player, unsigned int *wave_number, Position monster_nest,
    int *wave_duration, int *tower_number, enum Grid grid[WIDTH][LENGHT], int *placing_tower, Road *road,
    int *gem_selected,  Gem *gem, int *gem_number, Tower tower_list[], int *gem_tower, Pos_monster shot[]);

/**
 * Manage all interaction with the window
 * @param player
 * @param mouse_x
 * @param mouse_y
 * @param wave_number
 * @param monster_nest
 * @param wave_duration
 * @param list
 * @param gems
 * @param gem_level
 * @param grid
 * @param placing_tower
 * @param road
 * @param gem_selected
 * @param gem
 * @param gem_number
 */
void interaction(Player *player, int mouse_x, int mouse_y, unsigned int *wave_number, Position monster_nest, int *wave_duration,
    Wave list[], GemList *gems, int *gem_level, enum Grid grid[WIDTH][LENGHT], int *placing_tower, Road *road, int *gem_selected, Gem *gem, int *gem_number);

/**
 * Manage all settings to build a gem
 * @param gems
 * @param player
 * @param gem_level
 * @param mouse_x
 * @param mouse_y
 */
void build_gem_setting(GemList *gems, Player *player, int *gem_level, int mouse_x, int mouse_y);

/**
 * Update all monsters
 * @param list
 * @param wave_number
 * @param monster_on_road
 * @param frame
 * @param road
 * @param player
 * @param monsters_left
 * @param turns
 * @param turn_direction
 * @param size_turns
 * @return int
 */
int update_ennemies(Wave list[], unsigned int wave_number, unsigned int monster_on_road[], unsigned int frame[], Road *road, Player *player,
    unsigned int monsters_left, Position turns[], enum Direction turn_direction[], int size_turns);

/**
 * Make select a gem
 * @param mouse_x
 * @param mouse_y
 * @param gems
 * @param gem
 * @param gem_number
 * @return int
 */
int select_gem(int mouse_x, int mouse_y, GemList *gems, Gem *gem, int *gem_number);

/**
 * When the shot hit the target
 * @param target
 * @param index
 * @param shot
 * @param towers
 * @param player
 * @param gem_tower
 */
void shot_on_target(Monster* target[], int index, Pos_monster shot[], Tower towers[], Player * player, int gem_tower, Wave list[], unsigned int wave_number);

/**
 * Shoot a monster
 * @param target
 * @param index
 * @param shot
 * @param speed
 * @param towers
 * @param player
 * @param gem_tower
 */
void shoot(Monster* target[], int index, Pos_monster shot[], double speed, Tower towers[], Player * player, int gem_tower, Wave list[], unsigned int wave_number);

/**
 * Makes the towers shoot the monsters
 * @param towers
 * @param gem_tower
 * @param list
 * @param wave_number
 * @param target
 * @param shot
 * @param player
 */
void tower_shoot(Tower towers[], int gem_tower, Wave list[], unsigned int wave_number, Monster* target[], Pos_monster shot[], Player *player);

/**
 * Make spawn the waves
 * @param list
 * @param frame
 * @param wave_duration
 * @param wave_number
 * @param monster_nest
 * @param road
 */
void manage_waves(Wave list[], unsigned int *frame, int *wave_duration, unsigned int *wave_number, Position monster_nest, Road *road);

/**
 * \brief Free finished waves
 * \param list
 * \param wave_number
 */
void free_dead_waves(Wave list[], unsigned int wave_number);

/**
 * Free waves
 * @param list
 * @param wave_number
 */
void free_waves(Wave list[], unsigned int wave_number);

/**
 * The main function that makes the game working
 * @param road
 * @param player
 * @param grass
 * @param monster_nest
 * @param grid
 * @param turns
 * @param turn_direction
 * @param size_turns
 * @param font
 */
void game(Road *road, Player *player, MLV_Image *grass, Position monster_nest, enum Grid grid[WIDTH][LENGHT], Position turns[],
    enum Direction turn_direction[], int size_turns, MLV_Font *font);

#endif
