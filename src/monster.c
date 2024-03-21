/**
 * @file monster.c
 * @author TOUATI GUNEY
 * @brief This files handles the monsters
 * @version 0.1
 * @date 2024-01-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "monster.h"

double giveHP(enum Type type, int current_wave){
    if(type == BOSS)
        return BASE_HP * 12 * pow(1.2, current_wave);
    return BASE_HP * pow(1.2, current_wave);
}

Wave initWave(enum Type type, int wave_number, Position monster_nest, Road *road){
    Wave wave;
    Monster monster;
    Debuff debuff = {1.0, 0.0, 0};
    monster.debuff = debuff;
    monster.speed = 1.0 / 60;
    monster.direction = road->directionArray[0];
    monster.elemental_residue = NO_SHADE;
    wave.nb_monster = 12;
    //managing parameters depending on the wave type
    switch (type){
    case NORMAL:
        break;
    case AGILE:
        monster.speed = 2.0 / 60;
        break;
    case CROWD:
        monster.speed = 2.0 / 60;
        wave.nb_monster = 24;
        break;
    case BOSS:
        wave.nb_monster = 2;
        break;
    }
    wave.type = type;
    monster.max_hp = giveHP(wave.type, wave_number);
    monster.hp = monster.max_hp;
    monster.position.x = monster_nest.x;
    monster.position.y = monster_nest.y;
    //adding the monsters in the wave
    wave.monster_list = (Monster*)malloc(sizeof(Monster) * wave.nb_monster);
    if(wave.monster_list == NULL) {
        fprintf(stderr, "error while malloc a wave");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < wave.nb_monster; i++){
        monster.shade = rand() % 360;
        wave.monster_list[i] = monster;
    }
    return wave;
}

Wave pickRandomWave(unsigned int current_wave, Position monster_nest, Road *road){
    const int random_value = current_wave <= 5 ? rand() % 9: rand() % 10;
    if(random_value < 5)
        return initWave(NORMAL, current_wave, monster_nest, road);
    if(random_value < 7)
        return initWave(CROWD, current_wave, monster_nest, road);
    if(random_value < 9)
        return initWave(AGILE, current_wave, monster_nest, road);
    return initWave(BOSS, current_wave, monster_nest, road);
}

void printWaveInfos(Wave wave){
    printf("The number of monsters is : %d\n", wave.nb_monster);
    printf("The type of the wave is : %d\n", wave.type);
    printf("Monsters infos :\n");
    for(int i = 0; i < wave.nb_monster; i++)
        printf("Monster %d : hp = %f, speed = %f, shade = %d, direction = %d, position = %f.%f\n",
               i, wave.monster_list[i].hp, wave.monster_list[i].speed, wave.monster_list[i].shade, wave.monster_list[i].direction,
               wave.monster_list[i].position.x, wave.monster_list[i].position.y);
    printf("\n");     
}

void monsterDefeat(Player * player, double monsterHP){
    if((monsterHP * 0.1) * pow(1.3, player->level) + player->current_mana <= player->max_mana)
        player->current_mana += (monsterHP * 0.1) * pow(1.3, player->level);
    else
        player->current_mana = player->max_mana;
}

int banMonster(Player * player, int monsterHP, Wave *wave, Road *road, int monster_index){
    if(player->current_mana - (monsterHP * 0.15 * pow(1.3, player->level)) >= 0){
        player->current_mana -= monsterHP * 0.15 * pow(1.3, player->level);
        wave->monster_list[monster_index].position.x = road->positionArray[0].x;
        wave->monster_list[monster_index].position.y = road->positionArray[0].y;
        wave->monster_list[monster_index].direction = road->directionArray[0];
        return 1;
    }
    return 0;
}

void move_monster(Monster *monster, double variable_speed){
    static int tabx[] = {-1, 0, 1, 0};
    static int taby[] = {0, 1, 0, -1};
    //If there is a debuff
    if(monster->debuff.time_left > 0) {
        monster->position.x += variable_speed * monster->debuff.speed_modifier * tabx[monster->direction];
        monster->position.y += variable_speed * monster->debuff.speed_modifier * taby[monster->direction];
    }
    else {
        monster->position.x += variable_speed * tabx[monster->direction];
        monster->position.y += variable_speed * taby[monster->direction];
    }
}

int endWave(Wave wave){
    for(int i = 0; i < wave.nb_monster; i++){
        if(wave.monster_list[i].hp > 0)
            return 0;
    }
    return 1;
}

double dist_monster(Pos_monster A, Position B){
    return fabs(A.x - B.x) + fabs(A.y - B.y);
}

