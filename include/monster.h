/**
 * @file monster.h
 * @author TOUATI Damien, GUNEY David
 * @brief This file is the header for the monsters
 * @version 0.1
 * @date 2024-01-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __MONSTER__
#define __MONSTER__

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "grid.h"
#include "player.h"

/**
 * @brief Arbitrary value choose for the monster's hp
 * 
 */
#define BASE_HP 15

/**
 * @brief Defines several shades
 * 
 */
enum Shade{
    RED = 50,
    GREEN = 100,
    BLUE = 200,
    RED_GREEN = RED + GREEN,
    GREEN_BLUE = GREEN + BLUE,
    BLUE_RED = BLUE + RED,
    NO_SHADE
};

/**
 * \brief Define the wave type
 */
enum Type {
    NORMAL,
    CROWD,
    AGILE,
    BOSS
};

/**
 * \brief Defines the position of a monster
 */
typedef struct {
    double x, y;
}Pos_monster;

/**
 * \brief Defines a debuff of a monster
 */
typedef struct debuff {
    double speed_modifier;
    double parasite_damage;
    int time_left; //in frame
} Debuff;

/**
 * \brief Defines a monster
 */
typedef struct monster{
    double hp;
    double max_hp;
    double speed;
    Debuff debuff;
    enum Shade elemental_residue;
    int shade;
    Pos_monster position;
    enum Direction direction;
}Monster;

/**
 * @brief Defines a wave of monsters
 * 
 */
typedef struct wave{
    int nb_monster;
    Monster* monster_list;
    enum Type type;
}Wave;

/**
 * Returns the hp give to a monster depending of the current wave number
 * @param type
 * @param current_wave
 * @return double
 */
double giveHP(enum Type type, int current_wave);

/**
 * Inits a wave and returns it
 * @param type
 * @param wave_number
 * @param monster_nest
 * @param road
 * @return Wave
 */
Wave initWave(enum Type type, int wave_number, Position monster_nest, Road *road);

/**
 * Returns a wave with a random type bases on probabilities
 * @param current_wave
 * @param monster_nest
 * @param road
 * @return Wave
 */
Wave pickRandomWave(unsigned int current_wave, Position monster_nest, Road *road);

/**
 * Prints all infos of a wave
 * @param wave
 */
void printWaveInfos(Wave wave);

/**
 * Gives mana to the player corresponding to the monster max hp
 * @param player
 * @param monsterHP
 */
void monsterDefeat(Player * player, double monsterHP);

/**
 * Returns 1 if the player can ban the monster otherwise 0
 * @param player
 * @param monsterHP
 * @param wave
 * @param road
 * @param monster_index
 * @return int
 */
int banMonster(Player * player, int monsterHP, Wave *wave, Road *road, int monster_index);

/**
 * Returns 1 if all monsters of a wave are dead otherwise 0.
 * @param wave
 * @return int
 */
int endWave(Wave wave);

/**
 * Move a monster with the given speed
 * @param monster
 * @param variable_speed
 */
void move_monster(Monster *monster, double variable_speed);

/**
 * Returns the distance between the monster and the grid position
 * @param A
 * @param B
 * @return double
 */
double dist_monster(Pos_monster A, Position B);

#endif
