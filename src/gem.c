/**
 * @file gem.c
 * @author TOUATI Damien, GUNEY David
 * @brief This file handles gems and towers
 * @version 0.1
 * @date 2024-01-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "gem.h"

#include <graph.h>

void apply_element(Gem gem, Monster *monster) {
    switch (monster->elemental_residue) {
        case NO_SHADE:
            monster->elemental_residue = gem.color;
            break;
        case RED:
        case GREEN:
        case BLUE:
            monster->elemental_residue = NO_SHADE;
            break;
        case BLUE_RED:
        case RED_GREEN:
        case GREEN_BLUE:
        default:
            break;
    }
}

void vaporization_reaction(unsigned int wave_number, Wave list[], Gem gem, int gem_tower, Monster *monster) {
    for(int i = 0; i < gem_tower; ++i)
        for(int j = 0; j < wave_number; ++j)
            for(int k = list[j].nb_monster - 1; k >= 0; --k)
                if(list[j].monster_list != NULL && dist_shot(list[j].monster_list[k].position, monster->position) <= 3.5)
                    list[j].monster_list[k].hp -= DAMAGE * pow(2, gem.level) * (1 - cos(gem.shade - list[j].monster_list[k].shade) / 2) * 0.05;
    monster->debuff.speed_modifier = 4.0 / 5;
    monster->debuff.time_left = 5 * 60;
}

void combustion_reaction(double *damage) {
    *damage *= 3;
}

void rooted_reaction(Monster *monster) {
    monster->debuff.speed_modifier = 0;
    monster->debuff.time_left = 3 * 60;
}

void pyro_reaction(unsigned int wave_number, Wave list[], Gem gem, int gem_tower, Monster *monster) {
    for(int i = 0; i < gem_tower; ++i)
        for(int j = 0; j < wave_number; ++j)
            for(int k = list[j].nb_monster - 1; k >= 0; --k)
                if(list[j].monster_list != NULL && dist_shot(list[j].monster_list[k].position, monster->position) <= 2)
                    list[j].monster_list[k].hp -= DAMAGE * pow(2, gem.level) * (1 - cos(gem.shade - list[j].monster_list[k].shade) / 2) * 0.15;
}

void hydro_reaction(Monster *monster) {
    monster->debuff.speed_modifier = 2.0 / 3;
    monster->debuff.time_left = 10 * 60;
}

void dendro_reaction(Monster *monster, double *damage) {
    monster->debuff.parasite_damage = 0.025 * *damage;
    monster->debuff.time_left = 10 * 60;
}

void elemental_reaction(double *damage, Monster *monster, Gem gem, Wave list[], int gem_tower, unsigned int wave_number) {
    switch (monster->elemental_residue) {
        case RED:
            if(gem.color == GREEN)
                combustion_reaction(damage);
            else if(gem.color == BLUE)
                vaporization_reaction(wave_number, list, gem, gem_tower, monster);
            break;
        case BLUE:
            if(gem.color == RED)
                vaporization_reaction(wave_number, list, gem, gem_tower, monster);
            else if(gem.color == GREEN)
                rooted_reaction(monster);
            break;
        case GREEN:
            if(gem.color == RED)
                combustion_reaction(damage);
            else if(gem.color == BLUE)
                rooted_reaction(monster);
            break;
        case NO_SHADE:
            if(gem.color == RED)
                pyro_reaction(wave_number, list, gem, gem_tower, monster);
            else if(gem.color == GREEN)
                dendro_reaction(monster, damage);
            else if(gem.color == BLUE)
                hydro_reaction(monster);
            break;
        case RED_GREEN:
        case GREEN_BLUE:
        case BLUE_RED:
            break;
    }
}

double initial_damage(Gem gem, int index, Monster* monster[], Wave list[], int gem_tower, unsigned int wave_number){
    double damage = DAMAGE * pow(2, gem.level) * (1 - cos(gem.shade - monster[index]->shade)/2);
    elemental_reaction(&damage, monster[index], gem, list, gem_tower, wave_number);
    apply_element(gem, monster[index]);
    return damage;
}

int pick_random_shade(void){
    int random_type = rand() % 3; //0 for red shade, 1 for green shade, 2 for blue shade;
    if(random_type == 0){
        int res = rand() % 61;
        if(res > 30)
            return 299 + res;
        return res;
    }
    if(random_type == 1)
        return 90 + rand() % 61;
    return 210 + rand() % 61;
}

enum Shade colorOfShade(int shade){
    if(shade >= 90 && shade <= 150)
        return GREEN;
    if(shade >= 210 && shade <= 270)
        return BLUE;
    return RED;
}

Gem create_gem(int gem_level){
    Gem gem;
    gem.shade = pick_random_shade();
    gem.level = gem_level;
    gem.type = PURE;
    gem.color = colorOfShade(gem.shade);
    return gem;
}

Gem merge_gem(Gem first, Gem second, Player * player){
    Gem gem;
    if(first.type == PURE && second.type == PURE && (first.color == second.color)){
        gem.type = PURE;
        gem.level = first.level+1;
        gem.shade = (first.shade + second.shade)/2;
        gem.color = first.color;
    }
    else{
        gem.type = MIXED;
        gem.level = first.level+1;
        gem.shade = (first.shade + second.shade)/2;
        gem.color = first.color + second.color;
    }
    player->current_mana -= 100;
    return gem;
}

GemList init_gem_list(void){
    GemList list;
    list.size = 0;
    return list;
}

void right_shift(GemList *list){
    if(list->size + 1 <= MAX_GEM){
        for(int i = list->size; i > 0; i--){
            list->tab[i] = list->tab[i-1];
        }
        list->size++;
    }
}

void left_shift(GemList *list, int index){
    if(index <= list->size) {
        for(int i = index; i < list->size; i++)
            list->tab[i-1] = list->tab[i];
        list->size--;
    }
    else
        fprintf(stderr, "Bad index, leftshift function\n");
}

void add_gem_list(GemList* list, int gem_level, Player *player){
    if(player->current_mana >= 100 * pow(2, gem_level)){
        if(list->size + 1 <= MAX_GEM){
            player->current_mana -= 100 * pow(2, gem_level);
            right_shift(list);
            list->tab[0] = create_gem(gem_level);
        }
    }
}

void print_gem_list(GemList list){
    for(int i = 0; i < list.size; i++){
        printf("Gem : %d\n", i + 1);
        gem_infos(list.tab[i]);
    }
}

int gem_equal(Gem gem1, Gem gem2) {
    return gem1.color == gem2.color
    && gem1.level == gem2.level
    && gem1.shade == gem2.shade
    && gem1.type == gem2.type
    && isPositionEquals(gem1.position, gem2.position);
}

void gem_infos(Gem gem){
    printf("Gem infos :\n");
    printf("color : %d\n", gem.color);
    printf("level : %d\n", gem.level);
    printf("shade : %d\n", gem.shade);
    printf("type : %d\n", gem.type);
    printf("\n");
}

