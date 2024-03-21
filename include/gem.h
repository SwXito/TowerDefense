/**
 * @file gem.h
 * @author TOUATI Damien, GUNEY David
 * @brief This file is the header the generation of gems and towers
 * @version 0.1
 * @date 2024-01-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __GEM__
#define __GEM__

#include "monster.h"
#include "player.h"

/**
 * @brief Arbitrary value thats defines the base damage of a gem
 * 
 */
#define DAMAGE 7

/**
 * @brief Number maximum of gems
 * 
 */
#define MAX_GEM 33

/**
 * @brief Defines the type of a gem
 * 
 */
enum gemType{
    PURE,
    MIXED
};

/**
 * @brief Define a gem
 * 
 */
typedef struct gem{
    int shade;
    int level;
    Position position;
    enum gemType type;
    enum Shade color;
}Gem;

/**
 * @brief Define a list of gems
 * 
 */
typedef struct gemList{
    Gem tab[MAX_GEM];
    int size;
}GemList;

/**
 * @brief Define a tower
 * 
 */
typedef struct tower{
    Gem gem;
    Position position;
}Tower;

/**
 * \brief Applies the gem element on the monster elemental residue
 * \param gem
 * \param monster
 */
void apply_element(Gem gem, Monster *monster);

/**
 * \brief Makes a vaporization reaction on the monster
 * \param wave_number
 * \param list
 * \param gem
 * \param gem_tower
 * \param monster
 */
void vaporization_reaction(unsigned int wave_number, Wave list[], Gem gem, int gem_tower, Monster *monster);

/**
 * \brief Makes a combustion reaction on the monster
 * \param damage
 */
void combustion_reaction(double *damage);

/**
 * \brief Makes a rooted reaction on the monster
 * \param monster
 */
void rooted_reaction(Monster *monster);

/**
 * \brief Makes pyro reaction on the monster
 * \param wave_number
 * \param list
 * \param gem
 * \param gem_tower
 * \param monster
 */
void pyro_reaction(unsigned int wave_number, Wave list[], Gem gem, int gem_tower, Monster *monster);

/**
 * \brief Makes an hydro reaction on the monster
 * \param monster
 */
void hydro_reaction(Monster *monster);

/**
 * \brief Makes a dendro reaction on the monster
 * \param monster
 * \param damage
 */
void dendro_reaction(Monster *monster, double *damage);

/**
 * \brief Manages all elemental reaction on the target monster
 * \param damage
 * \param monster
 * \param gem
 * \param list
 * \param gem_tower
 * \param wave_number
 */
void elemental_reaction(double *damage, Monster *monster, Gem gem, Wave list[], int gem_tower, unsigned int wave_number);

/**
 * Returns the damage a monster should take
 * @param gem
 * @param index
 * @param monster
 * @return double
 */
double initial_damage(Gem gem, int index, Monster* monster[], Wave list[], int gem_tower, unsigned int wave_number);

/**
 * Returns a random shade
 * @return int
 */
int pick_random_shade(void);

/**
 * Creates a gem with the desired level
 * @param gem_level
 * @return Gem
 */
Gem create_gem(int gem_level);

/**
 * Returns the result of the merging two gems
 * @param first
 * @param second
 * @param player
 * @return Gem
 */
Gem merge_gem(Gem first, Gem second, Player * player);

/**
 * Inits a list of gems
 * @return GemList
 */
GemList init_gem_list(void);

/**
 * Prints the infos of all gems in the list of gems
 * @param list
 */
void print_gem_list(GemList list);

/**
 * Add a gem to the list of gems
 * @param list
 * @param gem_level
 * @param player
 */
void add_gem_list(GemList* list, int gem_level, Player *player);

/**
 * Returns 1 if two gems are equals otherwise 0
 * @param gem1
 * @param gem2
 * @return int
 */
int gem_equal(Gem gem1, Gem gem2);

/**
 * Make a left shit of the gem list
 * @param list
 * @param index
 */
void left_shift(GemList *list, int index);

/**
 * Prints all infos of a gem
 * @param gem
 */
void gem_infos(Gem gem);

#endif
