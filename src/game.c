/**
 * @file game.c
 * @author TOUATI Damien, GUNEY David
 * @brief This file handles the game actions part
 * @version 0.1
 * @date 2024-01-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "game.h"

void left_click(Wave list[], int *gem_level, int *pressed, GemList *gems, Player *player, unsigned int *wave_number,
    Position monster_nest, int *wave_duration, int *tower_number, enum Grid grid[WIDTH][LENGHT],
    int *placing_tower, Road *road, int *gem_selected, Gem *gem, int *gem_number, Tower tower_list[], int *gem_tower, Pos_monster shot[]){
    //Manage left click
    int mouse_x = 0, mouse_y = 0;
    MLV_Button_state button_state = MLV_get_mouse_button_state(MLV_BUTTON_LEFT);
    if(button_state == MLV_RELEASED) {
        *pressed = 0;
        if(*gem_selected == 1) {
            MLV_draw_circle(gems->tab[*gem_number - 1].position.x, gems->tab[*gem_number - 1].position.y, 28, MLV_COLOR_RED);
            return;
        }
    }
    //if its a left click
    else if(button_state == MLV_PRESSED && !*pressed) {
        *pressed = 1;
        MLV_get_mouse_position(&mouse_x, &mouse_y);
        if(*placing_tower == 1) {
            placeTower(player, grid, tower_number, mouse_x, mouse_y);
            *placing_tower = 0;
            return;
        }
        if(*gem_selected == 1) {
            placeGem(*gem, mouse_x, mouse_y, grid, tower_list, gem_tower, gems, *gem_number, player, shot);
            *gem_selected = 0;
            return;
        }
    }
    interaction(player, mouse_x, mouse_y, wave_number, monster_nest, wave_duration, list, gems, gem_level, grid, placing_tower, road, gem_selected, gem, gem_number);
}

void interaction(Player *player, int mouse_x, int mouse_y, unsigned int *wave_number, Position monster_nest, int *wave_duration,
    Wave list[], GemList *gems, int *gem_level, enum Grid grid[WIDTH][LENGHT], int *placing_tower, Road *road, int *gem_selected, Gem *gem, int *gem_number) {
    //if the player wants to upgrade is mana reserve
    if(mouse_x >= 460 && mouse_x <= 471 && mouse_y >= 5 && mouse_y <= 22)
        increaseReserve(player);
    //if we can place a tower
    if(mouse_x >= 795 && mouse_x <= 945 && mouse_y >= 5 && mouse_y <= 45)
        *placing_tower = 1;
    //if have selected a gem
    if(mouse_x >= 1120 && mouse_x <= 1320 && mouse_y >= 0 && mouse_y <= WINDOW_HEIGHT)
        *gem_selected = select_gem(mouse_x, mouse_y, gems, gem, gem_number);
    //if the player wants to skip a wave
    if(mouse_x >= 1085 && mouse_x <= 1115 && mouse_y >= 30 && mouse_y <= 46) {
        if(player->current_mana + player->max_mana * (*wave_duration/100.0) > player->max_mana)
            player->current_mana = player->max_mana;
        else
            player->current_mana += player->max_mana * (*wave_duration/100.0);
        *wave_duration = WAVE_TIME;
        list[*wave_number] = pickRandomWave(*wave_number, monster_nest, road);
        (*wave_number)++;
    }
    build_gem_setting(gems, player, gem_level, mouse_x, mouse_y);
}

int select_gem(int mouse_x, int mouse_y, GemList *gems, Gem *gem, int *gem_number) {
    *gem_number = get_gem_number(mouse_x, mouse_y);
    //if there is not gem selected
    if(*gem_number == 0)
        return 0;
    if(gems->size > 0 && *gem_number <= gems->size) {
        *gem = gems->tab[*gem_number - 1];
        return 1;
    }
    return 0;
}

void build_gem_setting(GemList *gems, Player *player, int *gem_level, int mouse_x, int mouse_y) {
    //check clicks to add gems
    if(*gem_level - 1 >= 0 && mouse_x >= 350 && mouse_x <= 370 && mouse_y >= 12 && mouse_y <= 37)
        (*gem_level)--;
    if(mouse_x >= 430 && mouse_x <= 450 && mouse_y >= 12 && mouse_y <= 37)
        (*gem_level)++;
    if(mouse_x >= 195 && mouse_x <= 345 && mouse_y >= 5 && mouse_y <= 45)
        add_gem_list(gems, *gem_level, player);
}

int update_ennemies(Wave list[], unsigned int wave_number, unsigned int monster_on_road[], unsigned int frame[], Road *road, Player *player,
    unsigned int monsters_left, Position turns[], enum Direction turn_direction[], int size_turns) {
    int speed;
    for(int i = 0; i < wave_number; ++i) {
        if(list[i].monster_list != NULL){
            speed = list[i].monster_list->speed * 60 == 1 ? 60 : 30;
            //make spawn monsters one by one
            if(frame[i] % speed == 0) {
                if(monster_on_road[i] != list[i].nb_monster)
                    monster_on_road[i]++;
            }
            //move monsters that are on the road
            if(!makeMoveMonster(&list[i], road, player, monster_on_road[i] - 1, turns, turn_direction, size_turns)
                || (wave_number > 200 && monsters_left == 0))
                return 0;
            frame[i]++;
        }
    }
    return 1;
}

void manage_waves(Wave list[], unsigned int *frame, int *wave_duration, unsigned int *wave_number, Position monster_nest, Road *road) {
    (*frame)++;
    //reducing the chrono every seconds
    if(*frame % 60 == 0) {
        (*wave_duration)--;
    }
    //if its time to spawn a wave
    if(*wave_duration == 0) {
        list[*wave_number] = pickRandomWave(*wave_number, monster_nest, road);
        (*wave_number)++;
        *wave_duration = WAVE_TIME;
    }
}

void free_dead_waves(Wave list[], unsigned int wave_number) {
    for(int i = 0; i < wave_number; ++i) {
        if(list[i].monster_list != NULL && endWave(list[i])) {
            free(list[i].monster_list);
            list[i].monster_list = NULL;
        }
    }
}

void free_waves(Wave list[], unsigned int wave_number) {
    for(int i = 0; i < wave_number; ++i) {
        if(list[i].monster_list != NULL) {
            free(list[i].monster_list);
            list[i].monster_list = NULL;
        }
    }
}

void shot_on_target(Monster* target[], int index, Pos_monster shot[], Tower towers[], Player * player, int gem_tower, Wave list[], unsigned int wave_number) {
    shot[index].x = towers[index].position.x;
    shot[index].y = towers[index].position.y;
    //damaging the monster
    target[index]->hp -= initial_damage(towers[index].gem, index, target, list, gem_tower, wave_number);
    //if dead removing target
    if(target[index]->hp <= 0) {
        monsterDefeat(player, target[index]->max_hp);
        for(int i = 0; i < gem_tower; ++i) {
            if(i != index && target[i] == target[index]) {
                shot[i].x = towers[i].position.x;
                shot[i].y = towers[i].position.y;
                target[i] = NULL;
            }
        }
    }
    target[index] = NULL;
}

void shoot(Monster* target[], int index, Pos_monster shot[], double speed, Tower towers[], Player * player, int gem_tower, Wave list[], unsigned int wave_number) {
    Pos_monster vector;
    double distance = sqrt(pow(target[index]->position.x - shot[index].x, 2) + pow(target[index]->position.y - shot[index].y, 2));
    vector.x = (target[index]->position.x - shot[index].x) / distance;
    vector.y = (target[index]->position.y - shot[index].y) / distance;
    //the shoot isn't on the target
    if(distance > speed) {
        shot[index].x = shot[index].x + vector.x * speed;
        shot[index].y = shot[index].y + vector.y * speed;
    }
    else {
        shot_on_target(target, index, shot, towers, player, gem_tower, list, wave_number);
    }
}

void tower_shoot(Tower towers[], int gem_tower, Wave list[], unsigned int wave_number, Monster* target[], Pos_monster shot[], Player *player) {
    double speed = 1.0/20;
    //Indentifying the targets
    for(int i = 0; i < gem_tower; ++i) {
        for(int j = 0; j < wave_number; ++j)
            for(int k = list[j].nb_monster - 1; k >= 0; --k) {
                if(list[j].monster_list != NULL && dist_monster(list[j].monster_list[k].position, towers[i].position) <= 3) {
                    if(shot[i].x == towers[i].position.x && shot[i].y == towers[i].position.y && list[j].monster_list[k].hp > 0)
                        target[i] = &list[j].monster_list[k];
                }
            }
    }
    //Shooting the targets
    for(int i = 0; i < gem_tower; ++i) {
        if(target[i] != NULL)
            shoot(target, i, shot, speed, towers, player, gem_tower, list, wave_number);
    }
}

void game(Road *road, Player *player, MLV_Image *grass, Position monster_nest, enum Grid grid[WIDTH][LENGHT],
    Position turns[], enum Direction turn_direction[], int size_turns, MLV_Font *font){
    struct timespec new_time, end_time;
    double frametime, extratime;
    Wave list[MAX_WAVE];
    Tower tower_list[MAX_TOWER];
    Pos_monster shot[MAX_TOWER];
    Monster* target[MAX_TOWER];
    unsigned int monster_on_road[MAX_WAVE], frames[MAX_WAVE];
    Gem gem;
    GemList gems = init_gem_list();
    unsigned int wave_number = 1, monsters = 0, frame = 0;
    int quit = 0, gem_tower = 0, gem_number = 0, gem_selected = 0, placing_tower = 0, tower_number = 0, gem_level = 0,
    pressed = 0, wave_duration = WAVE_TIME;
    list[0] = pickRandomWave(wave_number, monster_nest, road);
    for(int i = 0; i < MAX_TOWER; ++i)
        target[i] = NULL;
    for(int i = 0; i < MAX_WAVE; ++i) {
        monster_on_road[i] = 0;
        frames[i] = 0;
    }
    //game loop
    while(!quit) {
        clock_gettime(CLOCK_REALTIME, &end_time);
        display_window(list, grass, player, &gems, wave_number, &gem_level, wave_duration, &monsters, grid, tower_list, gem_tower, shot, font);
        left_click(list, &gem_level, &pressed, &gems, player, &wave_number, monster_nest,
            &wave_duration, &tower_number, grid, &placing_tower, road, &gem_selected, &gem, &gem_number, tower_list, &gem_tower, shot);
        quit = !update_ennemies(list, wave_number, monster_on_road, frames, road, player, monsters, turns, turn_direction, size_turns);
        manage_waves(list, &frame, &wave_duration, &wave_number, monster_nest, road);
        tower_shoot(tower_list, gem_tower, list, wave_number, target, shot, player);
        free_dead_waves(list, wave_number);
        MLV_update_window();
        clock_gettime(CLOCK_REALTIME, &new_time);
        frametime = new_time.tv_sec - end_time.tv_sec;
        frametime += (new_time.tv_nsec - end_time.tv_nsec) / 1.0E9;
        extratime = 1.0 / 60 - frametime;
        if(extratime > 0)
            MLV_wait_milliseconds((int)(extratime * 1000));
    }
    free_waves(list, wave_number);
}
