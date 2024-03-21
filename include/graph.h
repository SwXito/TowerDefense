/**
 * @file graph.h
 * @author TOUATI Damien, GUNEY David
 * @brief This file is the header for the graphics part
 * @version 0.1
 * @date 2024-01-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __GRAPH__
#define __GRAPH__

#include "monster.h"
#include "gem.h"
#include <MLV/MLV_all.h>

/**
 * @brief The width shift for the graphics part
 * 
 */
#define WIDTH_SHIFT 200

/**
 * @brief The height shift for the graphics part
 * 
 */
#define HEIGHT_SHIFT 50

/**
 * @brief The window width
 * 
 */
#define WINDOW_WIDTH 1320

/**
 * @brief The window height
 * 
 */
#define WINDOW_HEIGHT 930

/**
 * @brief Definition of PI because we can't the one of math.h
 * 
 */
#define M_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679

/**
 * Draw a gem on the screen
 * @param x
 * @param y
 * @param size
 * @param color
 */
void draw_gem(int x, int y, int size, MLV_Color color);

/**
 * Manage the display of all gems
 * @param gems
 */
void draw_all_gems(GemList *gems);

/**
 * Draw all towers on the screen
 * @param list
 * @param gem_tower
 */
void draw_all_tower_gems(Tower list[], int gem_tower);

/**
 * Display all monsters on the screen
 * @param list
 * @param wave_number
 */
void display_monster(Wave list[], unsigned int wave_number);

/**
 * Draw every shot on the screen
 * @param shot
 * @param towers
 * @param gem_tower
 */
void draw_shots(Pos_monster shot[], Tower towers[], int gem_tower);

/**
 * Gives the RGB value of a hue
 * @param hue
 * @param r
 * @param g
 * @param b
 */
void hueToRgb(double hue, int *r, int *g, int *b);

/**
 * Calls all needed functions to display the game
 * @param grid
 */
void display_game(enum Grid grid[WIDTH][LENGHT]);

/**
 * Display the player infos on the screen
 * @param player
 */
void player_infos(Player *player);

/**
 * Display the mana on the screen
 * @param player
 */
void display_mana(Player *player);

/**
 * Display all infos of the current wave on the screen
 * @param list
 * @param wave_number
 * @param wave_duration
 * @param monsters
 */
void display_wave_infos(Wave list[], unsigned int wave_number, int wave_duration, unsigned int *monsters);

/**
 * Calls all functions to display all infos on the screen
 * @param list
 * @param player
 * @param wave_number
 * @param gem_level
 * @param wave_duration
 * @param monsters
 * @param font
 */
void display_infos(Wave list[], Player *player, unsigned int wave_number, int *gem_level, int wave_duration, unsigned int *monsters, MLV_Font *font);

/**
 * Display the window of the game that never change
 * @param grass
 * @param road
 * @param grid
 * @param font
 */
void window(MLV_Image *grass, enum Grid grid[WIDTH][LENGHT], MLV_Font *font);

/**
 * Draw a simple box to display text on the screen
 * @param x
 * @param y
 * @param text
 * @param font
 */
void draw_box(int x, int y, char * text, MLV_Font *font);

/**
 * Draw the box for select the level wanted of a gem
 * @param level
 */
void draw_level_box(const int *level);

/**
 * Call all functions to display the game
 * @param list
 * @param grass
 * @param player
 * @param gems
 * @param wave_number
 * @param gem_level
 * @param wave_duration
 * @param monsters
 * @param grid
 * @param towers
 * @param gem_tower
 * @param shot
 * @param font
 */
void display_window(Wave list[], MLV_Image *grass, Player *player,
                    GemList *gems, unsigned int wave_number, int *gem_level, int wave_duration, unsigned int *monsters,
                    enum Grid grid[WIDTH][LENGHT], Tower towers[], int gem_tower, Pos_monster shot[], MLV_Font *font);

/**
 * Make place a gem on a tower
 * @param gem
 * @param mouse_x
 * @param mouse_y
 * @param grid
 * @param tower_list
 * @param tower_number
 * @param gems
 * @param gem_number
 * @param player
 * @param shot
 */
void placeGem(Gem gem, int mouse_x, int mouse_y, enum Grid grid[WIDTH][LENGHT], Tower tower_list[], int *tower_number, GemList *gems, int gem_number, Player *player, Pos_monster shot[]);

/**
 * Returns the gem number in the list depending on the mouse click
 * @param mouse_x
 * @param mouse_y
 * @return
 */
int get_gem_number(int mouse_x, int mouse_y);

/**
 * Move every monster of a wave
 * @param wave
 * @param road
 * @param player
 * @param monster_on_road
 * @param turns
 * @param turn_direction
 * @param size_turns
 * @return
 */
int makeMoveMonster(Wave *wave, Road *road, Player * player, int monster_on_road, Position turns[], enum Direction turn_direction[], int size_turns);

/**
 * Make a monster do every actions it needs to do
 * @param i
 * @param speed
 * @param wave
 * @param size_turns
 * @param turns
 * @param turn_direction
 */
void manage_monster(int i, double *speed, Wave *wave, int size_turns, Position turns[], enum Direction turn_direction[]);

/**
 * Returns the distance between a monster and a shot
 * @param monster
 * @param shot
 * @return
 */
double dist_shot(Pos_monster monster, Pos_monster shot);

#endif
