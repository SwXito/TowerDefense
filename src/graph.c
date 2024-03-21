/**
 * @file graph.c
 * @author TOUATI Damien, GUNEY David
 * @brief This file handles the graphics part of the program
 * @version 0.1
 * @date 2024-01-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "graph.h"

void draw_gem(int x, int y, int size, MLV_Color color) {
    int vertices = 6;
    double angle = 2.0 * M_PI / vertices;
    int vx[vertices];
    int vy[vertices];
    for (int i = 0; i < vertices; ++i) {
        vx[i] = x + size * cos(i * angle);
        vy[i] = y + size * sin(i * angle);
    }
    MLV_draw_filled_polygon(vx, vy, vertices, color);
    MLV_draw_polygon(vx, vy, vertices, MLV_COLOR_BLACK);
}

void draw_all_gems(GemList *gems){
    int r, g, b;
    int pos_x = 1160, pos_y = 40, size = 18;
    MLV_Color color;
    char level[10];
    //drawing gems in the right place
    for(int i = 0; i < gems->size; ++i) {
        switch(i % 3){
            case 0:
                pos_y += 75;
                pos_x = 1160;
                break;
            case 1:
                pos_x = 1220;
                break;
            case 2:
                pos_x = 1280;
                break;
            default:
                break;
        }
        gems->tab[i].position.x = pos_x;
        gems->tab[i].position.y = pos_y;
        hueToRgb(gems->tab[i].shade, &r, &g, &b);
        color = MLV_convert_rgba_to_color(r, g, b, 100);
        draw_gem(pos_x, pos_y - 5, size, color);
        sprintf(level, "level %d", gems->tab[i].level);
        MLV_draw_text(pos_x-20, pos_y+10, level, MLV_COLOR_BLACK);
    }
}

void draw_all_tower_gems(Tower list[], int gem_tower) {
    MLV_Color color;
    int r, g, b;
    for(int i = 0; i < gem_tower; ++i) {
        int screen_x = list[i].position.y * (WINDOW_WIDTH - WIDTH_SHIFT) / LENGHT + (WINDOW_WIDTH - WIDTH_SHIFT) / (2 * LENGHT);
        int screen_y = HEIGHT_SHIFT + list[i].position.x * (WINDOW_HEIGHT - HEIGHT_SHIFT) / WIDTH + (WINDOW_HEIGHT - HEIGHT_SHIFT) / (2 * WIDTH);
        hueToRgb(list[i].gem.shade, &r, &g, &b);
        color = MLV_convert_rgba_to_color(r, g, b, 100);
        draw_gem(screen_x, screen_y, 15, color);
        //drawing the range of the tower
        int range = 80;
        MLV_draw_circle(screen_x, screen_y, range, MLV_COLOR_RED);
        MLV_draw_filled_circle(screen_x, screen_y, range, MLV_convert_rgba_to_color(255, 0, 0, 40));
    }
}

void draw_shots(Pos_monster shot[], Tower towers[], int gem_tower) {
    for(int i = 0; i < gem_tower; ++i) {
        if(shot[i].x != towers[i].position.x || shot[i].y != towers[i].position.y)
            MLV_draw_filled_circle( shot[i].y * (WINDOW_HEIGHT - HEIGHT_SHIFT) / WIDTH + (WINDOW_HEIGHT - HEIGHT_SHIFT) / (2 * WIDTH),
            HEIGHT_SHIFT + shot[i].x * (WINDOW_WIDTH - WIDTH_SHIFT) / LENGHT + (WINDOW_WIDTH - WIDTH_SHIFT) / (2 * LENGHT), 2 ,MLV_COLOR_RED);
    }
}

void hueToRgb(double hue, int *r, int *g, int *b) {
    double saturation = 1.0;  // 100%
    double lightness = 0.5;   // 50%

    hue = fmod(hue, 360.0);
    if (hue < 0) {
        hue += 360.0;
    }
    if (saturation == 0.0) {
        *r = *g = *b = lightness * 255;
        return;
    }

    double C = (1.0 - fabs(2.0 * lightness - 1.0)) * saturation;
    double X = C * (1.0 - fabs(fmod(hue / 60.0, 2.0) - 1.0));
    double m = lightness - C / 2.0;

    double R, G, B;

    if (hue >= 0 && hue < 60) {
        R = C;
        G = X;
        B = 0;
    } else if (hue >= 60 && hue < 120) {
        R = X;
        G = C;
        B = 0;
    } else if (hue >= 120 && hue < 180) {
        R = 0;
        G = C;
        B = X;
    } else if (hue >= 180 && hue < 240) {
        R = 0;
        G = X;
        B = C;
    } else if (hue >= 240 && hue < 300) {
        R = X;
        G = 0;
        B = C;
    } else {
        R = C;
        G = 0;
        B = X;
    }

    *r = (int)((R + m) * 255);
    *g = (int)((G + m) * 255);
    *b = (int)((B + m) * 255);
}

void display_monster(Wave list[], unsigned int wave_number){
    int r, g, b;
    MLV_Color color;
    for(int i = 0; i < wave_number; ++i) {
        for(int j = 0; j < list[i].nb_monster; ++j) {
            if(list[i].monster_list != NULL && list[i].monster_list[j].hp > 0) {
                hueToRgb(list[i].monster_list[j].shade, &r, &g, &b);
                color = MLV_convert_rgba_to_color(r, g, b, 100);
                //drawing monster
                int screen_x = (int) (list[i].monster_list[j].position.y * (WINDOW_WIDTH - WIDTH_SHIFT) / LENGHT + (WINDOW_WIDTH - WIDTH_SHIFT) / (2 * LENGHT));
                int screen_y = (int) (list[i].monster_list[j].position.x * (WINDOW_HEIGHT - HEIGHT_SHIFT) / WIDTH + (WINDOW_HEIGHT - HEIGHT_SHIFT) / (2 * WIDTH));
                MLV_draw_filled_circle(screen_x, HEIGHT_SHIFT + screen_y, 5, color);

                //draw hp bar
                int health_bar_width = (int)(BASE_HP * list[i].monster_list[j].hp / list[i].monster_list[j].max_hp);
                int health_bar_height = 4; // ça c'est la hauteur de la barre de vie en terme d'épaisseur
                MLV_draw_filled_rectangle(screen_x - health_bar_width / 2, HEIGHT_SHIFT + screen_y - 10, health_bar_width, health_bar_height, MLV_COLOR_GREEN);
            }
        }
    }
}

void display_game(enum Grid grid[WIDTH][LENGHT]) {
        for (int i = 0; i < WIDTH; i++){
            for (int j = 0; j < LENGHT; j++) {
                switch(grid[i][j]) {
                    case ROAD:
                        MLV_draw_filled_rectangle(j*(WINDOW_HEIGHT - HEIGHT_SHIFT)/WIDTH, HEIGHT_SHIFT + i*(WINDOW_WIDTH - WIDTH_SHIFT)/LENGHT,
            (WINDOW_WIDTH - WIDTH_SHIFT)/LENGHT, (WINDOW_HEIGHT - HEIGHT_SHIFT)/WIDTH, MLV_convert_rgba_to_color(178, 180, 192, 232));
                        break;
                    case PLAYER_SIDE:
                        MLV_draw_filled_rectangle(j*(WINDOW_HEIGHT - HEIGHT_SHIFT)/WIDTH, HEIGHT_SHIFT + i*(WINDOW_WIDTH - WIDTH_SHIFT)/LENGHT,
            (WINDOW_WIDTH - WIDTH_SHIFT)/LENGHT, (WINDOW_HEIGHT - HEIGHT_SHIFT)/WIDTH, MLV_COLOR_GREEN);
                        break;
                    case MONSTER_SIDE:
                        MLV_draw_filled_rectangle(j*(WINDOW_HEIGHT - HEIGHT_SHIFT)/WIDTH, HEIGHT_SHIFT + i*(WINDOW_WIDTH - WIDTH_SHIFT)/LENGHT,
            (WINDOW_WIDTH - WIDTH_SHIFT)/LENGHT, (WINDOW_HEIGHT - HEIGHT_SHIFT)/WIDTH, MLV_COLOR_RED);
                        break;
                    case TOWER:
                    case GEM_TOWER:
                        MLV_draw_filled_rectangle(j*(WINDOW_HEIGHT - HEIGHT_SHIFT)/WIDTH, HEIGHT_SHIFT + i*(WINDOW_WIDTH - WIDTH_SHIFT)/LENGHT,
            (WINDOW_WIDTH - WIDTH_SHIFT)/LENGHT, (WINDOW_HEIGHT - HEIGHT_SHIFT)/WIDTH, MLV_COLOR_GREY50);
                        break;
                    default:
                        break;
                }
            }
        }
}

void player_infos(Player *player) {
    char level[4];
    sprintf(level, "%d", player->level);
    display_mana(player);
    MLV_draw_text(540, 30, "Player level :", MLV_COLOR_BLACK);
    MLV_draw_text(650, 30, level, MLV_COLOR_BLACK);
}

void display_mana(Player *player){
    char mana[20];
    char max_mana[20];
    sprintf(mana, "%.1lf", player->current_mana);
    sprintf(max_mana, "%.1lf", player->max_mana);
    MLV_draw_text(475, 5, "Mana:", MLV_COLOR_BLACK);
    MLV_draw_text(520, 5, mana, MLV_COLOR_BLACK);
    MLV_draw_text(655, 5, "/", MLV_COLOR_BLACK);
    MLV_draw_text(660, 5, max_mana, MLV_COLOR_BLACK);
    MLV_draw_adapted_text_box(460, 5, "+", 1,  MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GREY60, MLV_TEXT_CENTER);
}

void display_wave_infos(Wave list[], unsigned int wave_number, int wave_duration, unsigned int *monsters){
    int monster = 0;
    for(int i = 0; i < wave_number; ++i)
        for(int j = 0; j < list[i].nb_monster; ++j)
            if(list[i].monster_list != NULL && list[i].monster_list[j].hp > 0)
                monster++;
    *monsters = monster;
    char number[4];
    char monsters_left[10];
    char duration[3];
    sprintf(number, "%d", wave_number);
    sprintf(monsters_left, "%d", *monsters);
    sprintf(duration, "%d", wave_duration);
    MLV_draw_text(950, 30, "Wave number :", MLV_COLOR_BLACK);
    MLV_draw_text(1060, 30, number, MLV_COLOR_BLACK);
    MLV_draw_adapted_text_box(1085, 30, "skip", 1,  MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GREY60, MLV_TEXT_CENTER);
    MLV_draw_text(20, 10, "Monsters left : ", MLV_COLOR_BLACK);
    MLV_draw_text(150, 10, monsters_left, MLV_COLOR_BLACK);
    MLV_draw_text(950, 10, "next wave in :", MLV_COLOR_BLACK);
    MLV_draw_text(1090, 10, duration, MLV_COLOR_BLACK);

}

void draw_box(int x, int y, char * text, MLV_Font *font){
    MLV_draw_filled_rectangle(x-5, y-5, 150, 40, MLV_COLOR_GREY60);
    MLV_draw_text_box_with_font(x, y, 140, 30, text, font, 1, MLV_COLOR_GREY60,
                                MLV_COLOR_BLACK, MLV_COLOR_GREY85, MLV_TEXT_CENTER, MLV_TEXT_CENTER,
                                MLV_TEXT_CENTER);
}

void draw_level_box(const int *level){
    char lvl[4];
    sprintf(lvl, "%d", *level);
    MLV_draw_rectangle(350, 12, 100, 25, MLV_COLOR_GRAY48);
    MLV_draw_filled_rectangle(350, 12, 20, 25, MLV_COLOR_WHITE);
    MLV_draw_text(355, 15, "-", MLV_COLOR_BLACK);
    MLV_draw_text(395, 15, lvl, MLV_COLOR_BLACK);
    MLV_draw_filled_rectangle(430, 12, 20, 25, MLV_COLOR_WHITE);
    MLV_draw_text(435, 15, "+", MLV_COLOR_BLACK);
}

void display_infos(Wave list[], Player *player, unsigned int wave_number, int *gem_level, int wave_duration, unsigned int *monsters, MLV_Font *font){
    int w = 90, lines = MAX_GEM/3;
    MLV_draw_filled_rectangle(1130, w-12, 180, lines*75, MLV_COLOR_GRAY48);
    for (int i = 0; i < lines; i++, w += 75){
        MLV_draw_filled_rectangle(1135, w, 50, 50, MLV_COLOR_LIGHT_GREY);
        MLV_draw_filled_rectangle(1195, w, 50, 50, MLV_COLOR_LIGHT_GREY);
        MLV_draw_filled_rectangle(1255, w, 50, 50, MLV_COLOR_LIGHT_GREY);
    }
    draw_box(200, 10, "Build Gem", font);
    draw_level_box(gem_level);
    draw_box(800, 10, "Tower", font);
    player_infos(player);
    display_wave_infos(list, wave_number, wave_duration, monsters);
}

void window(MLV_Image *grass, enum Grid grid[WIDTH][LENGHT], MLV_Font *font){
    MLV_clear_window(MLV_COLOR_LIGHT_BLUE);
    MLV_draw_image(grass, 0, HEIGHT_SHIFT);
    MLV_draw_filled_rectangle(1120, 0, 200, WINDOW_HEIGHT, MLV_COLOR_GREY30);
    //drawing right part of the window
    draw_box(1150, 10, "Start", font);
    display_game(grid);
}

void display_window(Wave list[], MLV_Image *grass, Player *player,
    GemList *gems, unsigned int wave_number, int *gem_level, int wave_duration, unsigned int *monsters,
    enum Grid grid[WIDTH][LENGHT], Tower towers[], int gem_tower, Pos_monster shot[], MLV_Font *font){
    //Combine all displays for the final display
    window(grass, grid, font);
    display_infos(list, player, wave_number, gem_level, wave_duration, monsters, font);
    draw_all_gems(gems);
    draw_all_tower_gems(towers, gem_tower);
    draw_shots(shot, towers, gem_tower);
    display_monster(list, wave_number);
}

double dist_shot(Pos_monster monster, Pos_monster shot){
    return fabs(monster.x - shot.x) + fabs(monster.y - shot.y);
}

void manage_monster(int i, double *speed, Wave *wave, int size_turns, Position turns[], enum Direction turn_direction[]) {
    *speed = (0.9 + ((double) rand() / RAND_MAX) * 0.2) * wave->monster_list[i].speed;
    if(wave->monster_list[i].hp > 0) {
        for(int j = 0; j < size_turns; ++j) {
            //making const the monster speed in the corner
            if(dist_monster(wave->monster_list[i].position, turns[j]) <= 1)
                *speed = wave->monster_list[i].speed;
            //if we are in the corner we fix the position to the corner center
            if(dist_monster(wave->monster_list[i].position, turns[j]) <= *speed / 2) {
                if(wave->monster_list[i].position.x != turns[j].x || wave->monster_list[i].position.y != turns[j].y) {
                    wave->monster_list[i].position.x = turns[j].x;
                    wave->monster_list[i].position.y = turns[j].y;
                }
                wave->monster_list[i].direction = turn_direction[j];
            }
        }
        move_monster(&(wave->monster_list[i]), *speed);
        //manage the debuff time
        if(wave->monster_list[i].debuff.time_left > 0)
            wave->monster_list[i].debuff.time_left--;
        if(wave->monster_list[i].debuff.parasite_damage > 0 && wave->monster_list[i].debuff.time_left % 30 == 0)
            wave->monster_list[i].hp -= wave->monster_list[i].debuff.parasite_damage;
    }
}

int get_gem_number(int mouse_x, int mouse_y) {
    int index = 0;
    for(int i = 0; i < MAX_GEM; ++i) {
        if(mouse_x >= 1135 && mouse_x <= 1185 && mouse_y >= 90 + (i * 75) && mouse_y <= 140 + (i * 75))
            index = 1 + 3 * i;
        else if(mouse_x >= 1195 && mouse_x <= 1245 && mouse_y >= 90 + (i * 75) && mouse_y <= 140 + (i * 75))
            index = 2 + 3 * i;
        else if(mouse_x >= 1255 && mouse_x <= 1305 && mouse_y >= 90 + (i * 75) && mouse_y <= 140 + (i * 75))
            index = 3 + 3 * i;
    }
    return index;
}

void placeGem(Gem gem, int mouse_x, int mouse_y, enum Grid grid[WIDTH][LENGHT], Tower tower_list[], int *gem_tower, GemList *gems, int gem_number, Player *player, Pos_monster shot[]) {
    Position pos;
    int index = get_gem_number(mouse_x, mouse_y);
    //getting the grid coordinates
    pos.y = mouse_x / ((WINDOW_WIDTH - WIDTH_SHIFT) / LENGHT);
    pos.x = (mouse_y - HEIGHT_SHIFT) / ((WINDOW_HEIGHT - HEIGHT_SHIFT) / WIDTH);
    //if its a tower, it places a gem on it
    if(grid[pos.x][pos.y] == TOWER && gem_number > 0) {
        Tower tower = {gem, pos};
        grid[pos.x][pos.y] = GEM_TOWER;
        left_shift(gems, gem_number);
        tower_list[*gem_tower] = tower;
        shot[*gem_tower].x = tower.position.x;
        shot[*gem_tower].y = tower.position.y;
        (*gem_tower)++;
    }
    //if the click target is an other gem it merge them
    if(index > 0 && index <= gems->size && !gem_equal(gem, gems->tab[index - 1])) {
        int gem_index = get_gem_number(gem.position.x, gem.position.y);
        if(gem_index > 0 && gem.level == gems->tab[index - 1].level && player->current_mana >= 100) {
            Gem new_gem = merge_gem(gem, gems->tab[index - 1], player);
            gems->tab[gem_index - 1] = new_gem;
            left_shift(gems, index);
        }
        else
            fprintf(stderr, "Can't merge gems\n");
    }
}

int makeMoveMonster(Wave *wave, Road *road, Player * player, int monster_on_road, Position turns[], enum Direction turn_direction[], int size_turns){
    double speed;
    //spawning all monsters
    if(monster_on_road < wave->nb_monster - 1)
        for(int i = 0; i <= monster_on_road; i++)
            manage_monster(i, &speed, wave, size_turns, turns, turn_direction);
    //When they all spawned
    else
        for(int i = 0; i < wave->nb_monster; i++) {
            manage_monster(i, &speed, wave, size_turns, turns, turn_direction);;
            if(dist_monster(wave->monster_list[i].position, road->positionArray[road->size]) <= speed)
                if(!banMonster(player, wave->monster_list[i].hp, wave, road, i))
                    return 0;
        }
    return 1;
}
