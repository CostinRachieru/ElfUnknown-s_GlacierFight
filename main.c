// Copyright 2019 Rachieru Costin Tudor <<costinrch46@gmail.com>>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Some functions for encoding and coding the given datas.
int encode_map(short height, short gloves) {
    int cell;
    cell = height;
    cell = cell << 16;
    cell = cell | gloves;
    return cell;
}

int encode_map_gloves(int cell, short gloves) {
    cell = cell >> 16;
    cell = cell << 16;
    return cell | gloves;
}

short decode_map_height(int cell) {
    cell = cell >> 16;
    return cell;
}

short decode_map_gloves(int cell) {
    cell = cell << 16;
    cell = cell >> 16;
    return cell;
}

void read_map(int radius, int **map, FILE *in_file) {
    short i, j, height, gloves;
    for (i = 0; i < (2 * radius + 1); i++) {
        for (j = 0; j < (2 * radius + 1); j++) {
            fscanf(in_file, "%hd %hd", &height, &gloves);
                map[i][j] = encode_map(height, gloves);
        }
    }
}

// Function that allocates map memory.
int** alloc_map(int **map, short radius) {
    short i;
    free(map);
    map = (int**)calloc((2 * radius + 1), sizeof(int*));
    for (i = 0; i <= radius * 2 ; i++) {
        map[i] = (int*)calloc(2 * radius + 1, sizeof(int));
    }
    return map;
}

// Function that allocates memory for the "Matrix" of datas about players.
long long **alloc_stats(long long **stats, short nr_players) {
    short i;
    free(stats);
    stats = (long long**)calloc(nr_players, sizeof(long long*));
    for (i = 0; i < nr_players; i++) {
        stats[i] = (long long*)calloc(1, sizeof(long long));
    }
    return stats;
}

// Function that allocates memory for the array of names.
char **alloc_names(char **names, short nr_players) {
    short i;
    free(names);
    names = (char**)calloc(nr_players, sizeof(char*));
    for (i = 0; i < nr_players; i++) {
        names[i] = (char*)calloc(30, sizeof(char));
    }
    return names;
}

long long encode_column(long long code, short column) {
    long long *aux1;
    aux1 = calloc(1, sizeof(long long));
    *aux1 = code >> 8;
    *aux1 = *aux1 << 8;
    code = column | *aux1;
    free(aux1);
    return code;
}

short decode_column(long long code) {
    short column = 0;
    code = code << 56;
    code = code >> 56;
    column = column | code;
    return column;
}

long long encode_line(long long code, short line) {
    long long *aux1, *aux2;
    aux1 = calloc(1, sizeof(long long));
    aux2 = calloc(1, sizeof(long long));
    *aux1 = code << 56;
    *aux1 = *aux1 >> 56;
    *aux2 = code >> 16;
    *aux2 = *aux2 << 16;
    code = line;
    code = code << 8;
    code = code | *aux1 | *aux2;
    free(aux1);
    free(aux2);
    return code;
}

short decode_line(long long code) {
    short line = 0;
    code = code << 48;
    code = code >> 56;
    line = line | code;
    return line;
}

long long encode_sta(long long code, short stamina) {
    long long *aux1, *aux2;
    aux1 = calloc(1, sizeof(long long));
    aux2 = calloc(1, sizeof(long long));
    *aux1 = code << 32;
    *aux1 = *aux1 >> 32;
    *aux2 = code >> 48;
    *aux2 = *aux2 << 48;
    code = stamina;
    code = code << 32;
    code = code | *aux1 | *aux2;
    free(aux1);
    free(aux2);
    return code;
}

short decode_sta(long long code) {
    code = code << 16;
    code = code >> 48;
    return code;
}
long long encode_hp(long long code, short hp) {
    long long *aux1;
    aux1 = calloc(1, sizeof(long long));
    *aux1 = code << 16;
    *aux1 = *aux1 >> 16;
    code = hp;
    code = code << 48;
    code = code | *aux1;
    free(aux1);
    return code;
}

short decode_hp(long long code) {
    code = code >> 48;
    return code;
}

long long encode_dmg(long long code, short dmg) {
    long long *aux1, *aux2;
    aux1 = calloc(1, sizeof(long long));
    aux2 = calloc(1, sizeof(long long));
    *aux1 = code << 48;
    *aux1 = *aux1 >> 48;
    *aux2 = code >> 32;
    *aux2 = *aux2 << 32;
    code = dmg;
    code = code << 16;
    code = code | *aux1 | *aux2;
    free(aux1);
    free(aux2);
    return code;
}

short decode_dmg(long long code) {
    short dmg = 0;
    code = code << 32;
    code = code >> 48;
    dmg = dmg | code;
    return dmg;
}

/*   Calculates the distance from the center of a circle (with "radius")
to the point (i,j). */
float distance(short i, short j, short radius) {
    return sqrt(pow(i - radius, 2) + pow(j - radius, 2));
}

short is_glacier(short i, short j, short radius, short centre) {
    if (distance(i, j, centre) <= radius) return 1;
    return 0;
}

short read_players(char **names, long long **stats, int **map,
short nr_players, short radius, FILE *in_file, FILE *out_file) {
    short i, *x, *y, *is_dead, dead = 0;

    x = calloc(1, sizeof(short));
    y = calloc(1, sizeof(short));
    is_dead = malloc(1 * sizeof(short));
    for (i = 0; i < nr_players; i++) {
        *is_dead = 0;
        fscanf(in_file, "%s", names[i]);
        fscanf(in_file, "%hd", x);
        stats[i][0] = encode_line(stats[i][0], *x);
        fscanf(in_file, "%hd", y);
        stats[i][0] = encode_column(stats[i][0], *y);
        if (is_glacier(*x, *y, radius, radius) == 0) {
            fprintf(out_file, "%s has missed the glacier.\n", names[i]);
            *is_dead = 1;
            dead += 1;
            stats[i][0] = (-1);
            }
        if (*is_dead == 0) {
            stats[i][0] = encode_dmg(stats[i][0],
                decode_map_gloves(map[*x][*y]));
            }
        map[*x][*y] = encode_map_gloves(map[*x][*y], 0);
        fscanf(in_file, "%hd", x);
        if (*is_dead == 0) {
            stats[i][0] = encode_hp(stats[i][0], *x);
            }
        fscanf(in_file, "%hd", x);
        if (*is_dead == 0) {
            stats[i][0] = encode_sta(stats[i][0], *x);
            }
    }
    free(x);
    free(y);
    free(is_dead);
    return dead;
}

// Verifies if there is any combat.
short is_combat(long long **stats, short nr_players, short *id, short *line,
    short *column, short *combatant_line, short *combatant_column,
    short *combatant_id) {
    short i;

    for (i = 0; i < nr_players; i++) {
        if (*id != i) {
            *combatant_line = decode_line(stats[i][0]);
            *combatant_column = decode_column(stats[i][0]);
            *combatant_id = i;
            if (*line == *combatant_line && *column == *combatant_column) {
                return 1;
            }
        }
    }
    return 0;
}

// Verifies if a player can move to another cell and swap gloves.
void moving(int **map,
    short *actual_height, short *line, short *column, short *next_height,
    short *next_line, short *next_column, short *alt_diff, short *sta,
    short *dmg, short radius, short *meltdowns) {
    short *new_gloves;
    if (*next_line >= 0 && *next_column >= 0 &&
        *next_line <= (radius + *meltdowns) * 2 &&
        *next_column <= (radius + *meltdowns) * 2) {
        new_gloves = calloc(1, sizeof(short));
        *actual_height = decode_map_height(map[*line][*column]);
        *next_height = decode_map_height(map[*next_line][*next_column]);
        *alt_diff = *actual_height - *next_height;
        if (*alt_diff < 0) {
            *alt_diff = (-1) * *alt_diff;
        }
        if (*alt_diff <= *sta) {
            *sta -= *alt_diff;
            } else {
                *next_column = *column;
                *next_line = *line;
            }
        *new_gloves = decode_map_gloves(map[*next_line][*next_column]);
        if (*new_gloves > *dmg) {
            map[*next_line][*next_column] =
            encode_map_gloves(map[*next_line][*next_column], *dmg);
            *dmg = *new_gloves;
        }
        free(new_gloves);
    }
}

// Simulates the combat between 2 players.
void combat(long long **stats, char **names, short nr_players, short *id,
    short *line, short *column, short *dead, short *kills, FILE *out_file) {
    short *combatant_line, *combatant_column, *combatant_id,
    *dmg, *combatant_dmg, *sta, *combatant_sta, *hp, *combatant_hp;

    combatant_line = calloc(1, sizeof(short));
    combatant_column = calloc(1, sizeof(short));
    combatant_id = calloc(1, sizeof(short));
    if (is_combat(stats, nr_players, id, line, column,
        combatant_line, combatant_column, combatant_id)) {
        *dead = *dead + 1;
        combatant_dmg = malloc(1 * sizeof(short));
        combatant_sta = malloc(1 * sizeof(short));
        combatant_hp = malloc(1 * sizeof(short));
        dmg = malloc(1 * sizeof(short));
        sta = malloc(1 *sizeof(short));
        hp = malloc(1 * sizeof(short));

        *dmg = decode_dmg(stats[*id][0]);
        *sta = decode_sta(stats[*id][0]);
        *hp = decode_hp(stats[*id][0]);
        *combatant_dmg = decode_dmg(stats[*combatant_id][0]);
        *combatant_sta = decode_sta(stats[*combatant_id][0]);
        *combatant_hp = decode_hp(stats[*combatant_id][0]);
        if (*sta >= *combatant_sta) {
            while (1) {
                if (*dmg >= *combatant_hp) {
                    *sta += *combatant_sta;
                    kills[*id] = kills[*id] + 1;
                    stats[*combatant_id][0] = -1;
                    break;
                }
                *combatant_hp -= *dmg;
                if (*hp <= *combatant_dmg) {
                    *combatant_sta += *sta;
                    kills[*combatant_id] = kills[*combatant_id] + 1;
                    stats[*id][0] = -1;
                    break;
                }
                *hp -= *combatant_dmg;
            }
        } else {
            while (1) {
                if (*hp <= *combatant_dmg) {
                    *combatant_sta += *sta;
                    stats[*id][0] = -1;
                    kills[*combatant_id] = kills[*combatant_id] + 1;
                    break;
                }
                *hp -= *combatant_dmg;
                if (*dmg >= *combatant_hp) {
                    *sta += *combatant_sta;
                    stats[*combatant_id][0] = -1;
                    kills[*id] = kills[*id] + 1;
                    break;
                }
                *combatant_hp -= *dmg;
            }
        }
        if (stats[*id][0] == -1) {
            fprintf(out_file, "%s sent %s back home.\n", names[*combatant_id],
            names[*id]);
            stats[*combatant_id][0] = encode_sta(stats[*combatant_id][0],
                *combatant_sta);
            stats[*combatant_id][0] = encode_hp(stats[*combatant_id][0],
                *combatant_hp);
        }
        if (stats[*combatant_id][0] == -1) {
            fprintf(out_file, "%s sent %s back home.\n", names[*id],
            names[*combatant_id]);
            stats[*id][0] = encode_sta(stats[*id][0], *sta);
            stats[*id][0] = encode_hp(stats[*id][0], *hp);
        }
        free(combatant_dmg);
        free(combatant_sta);
        free(combatant_hp);
        free(dmg);
        free(sta);
        free(hp);
    }
    free(combatant_line);
    free(combatant_column);
    free(combatant_id);
}

void free_move(short *id, short *next_column, short *next_line, short *line,
    short *column, short *actual_height, short *next_height,
    short *alt_diff, short *sta, short *dmg) {
    free(id);
    free(next_column);
    free(next_line);
    free(line);
    free(column);
    free(actual_height);
    free(next_height);
    free(alt_diff);
    free(sta);
    free(dmg);
}

// Encodes back in the arrays the movement that a player did.
void encode_move(long long **stats, short *id, short *line,
short* column, short * sta, short *dmg) {
    stats[*id][0] = encode_line(stats[*id][0], *line);
    stats[*id][0] = encode_column(stats[*id][0], *column);
    stats[*id][0] = encode_sta(stats[*id][0], *sta);
    stats[*id][0] = encode_dmg(stats[*id][0], *dmg);
}

// Decodes data about a player.
void decode_player(long long **stats, short *id, short *line, short *column,
short *sta, short *dmg) {
    *line = decode_line(stats[*id][0]);
    *column = decode_column(stats[*id][0]);
    *sta = decode_sta(stats[*id][0]);
    *dmg = decode_dmg(stats[*id][0]);
}

/* Decides whether a player can go to another cell or it will
fall off the glacier. */
short move(char **names, long long **stats, int **map, short nr_players,
short radius, short *meltdowns, short *kills, FILE *in_file, FILE *out_file) {
    short *id, *next_line, *next_column, *line, *column,
    *actual_height, *next_height, *sta, *alt_diff, *dmg, *dead, deads;
    unsigned short i;
    char *moves;

    actual_height = malloc(1 * sizeof(short));
    next_height = malloc(1 * sizeof(short));
    id = calloc(1, sizeof(short)); dead = calloc(1, sizeof(short));
    line = malloc(1 * sizeof(short)); column = malloc(1 * sizeof(short));
    next_line = malloc(1 * sizeof(short));
    next_column = malloc(1 * sizeof(short));
    moves = calloc(100, sizeof(char)); alt_diff = malloc(1 * sizeof(short));
    sta = malloc(1 * sizeof(short)); dmg = malloc(1 * sizeof(short));
    fscanf(in_file, "%hd", id);
    fscanf(in_file, "%s", moves);
    moves = realloc(moves, (strlen(moves) + 1) * sizeof(char));
    decode_player(stats, id, line, column, sta, dmg);
    if (stats[*id][0] != (-1)) {
        for (i = 0; i < strlen(moves); i++) {
            if (stats[*id][0] != -1) {
                if (moves[i] == 'R') {
                    *next_line = *line;
                    *next_column = *column + 1;
                    moving(map, actual_height, line, column,
                    next_height, next_line, next_column,
                    alt_diff, sta, dmg, radius, meltdowns);
                }
                if (moves[i] == 'L') {
                    *next_line = *line;
                    *next_column = *column - 1;
                    moving(map, actual_height, line, column,
                    next_height, next_line, next_column,
                    alt_diff, sta, dmg, radius, meltdowns);
                }
                if (moves[i] == 'U') {
                    *next_line = *line - 1;
                    *next_column = *column;
                    moving(map, actual_height, line, column,
                    next_height, next_line, next_column,
                    alt_diff, sta, dmg, radius, meltdowns);
                }
                if (moves[i] == 'D') {
                    *next_line = *line + 1;
                    *next_column = *column;
                    moving(map, actual_height, line, column,
                    next_height, next_line, next_column,
                    alt_diff, sta, dmg, radius, meltdowns);
                }
                *line = *next_line;
                *column = *next_column;
                if (is_glacier(*line, *column, radius,
                radius + *meltdowns) == 0) {
                    fprintf(out_file, "%s fell off the glacier.\n", names[*id]);
                    stats[*id][0] = -1;
                    *dead += 1;
                } else {
                    encode_move(stats, id, line, column, sta, dmg);
                    combat(stats, names, nr_players, id,
                    line, column, dead, kills, out_file);
                    *sta = decode_sta(stats[*id][0]);
                }
            }
        }
    }
    free(moves);
    free_move(id, next_column, next_line, line, column, actual_height,
        next_height, alt_diff, sta, dmg);
    deads = *dead;
    free(dead);
    return deads;
}

short decode_snowstorm_dmg(int k) {
    k = k >> 24;
    return k & 255;
}

short decode_snowstorm_radius(int k) {
    k = k << 8;
    k = k >> 24;
    return k & 255;
}

short decode_snowstorm_column(int k) {
    k = k << 16;
    k = k >> 24;
    return k & 255;
}

short decode_snowstorm_line(int k) {
    k = k << 24;
    k = k >> 24;
    return k & 255;
}

short is_in_snowstorm(short *r, short *x, short *y,
short *line, short *column) {
    if ((*x - *line) * (*x - *line) +
    (*y - *column) * (*y- *column) <= *r * *r) return 1;
    return 0;
}

short snowstorm(char **names, long long **stats, short nr_players,
FILE *in_file, FILE *out_file) {
    int k;
    short *dmg, *r, *y, *x, i, *line, *column, *hp, dead = 0;

    dmg = calloc(1, sizeof(short));
    r = calloc(1, sizeof(short));
    y = calloc(1, sizeof(short));
    x = calloc(1, sizeof(short));
    line = calloc(1, sizeof(short));
    column = calloc(1, sizeof(short));

    fscanf(in_file, "%d", &k);
    *dmg = decode_snowstorm_dmg(k);
    *r = decode_snowstorm_radius(k);
    *y = decode_snowstorm_column(k);
    *x = decode_snowstorm_line(k);
    for (i = 0; i < nr_players; i++) {
        if (stats[i][0] != -1) {
            *line = decode_line(stats[i][0]);
            *column = decode_column(stats[i][0]);
            if (is_in_snowstorm(r, x, y, line, column)) {
                hp = malloc(1 * sizeof(short));
                *hp = decode_hp(stats[i][0]);
                if (*hp <= *dmg) {
                    stats[i][0] = -1;
                    fprintf(out_file, "%s was hit by snowstorm.\n", names[i]);
                    dead += 1;
                } else {
                    stats[i][0] = encode_hp(stats[i][0], *hp - *dmg);
                }
                free(hp);
            }
        }
    }

    free(dmg);
    free(r);
    free(y);
    free(x);
    free(line);
    free(column);
    return dead;
}

short meltdown(char **names, long long **stats, short nr_players,
short radius, short *meltdowns, FILE *in_file, FILE *out_file) {
    short *bonus, i, *line, *column, dead = 0;
    line = malloc(1 * sizeof(short));
    column = malloc(1 * sizeof(short));
    bonus = malloc(1 * sizeof(short));
    fscanf(in_file, "%hu", bonus);
    for (i = 0; i < nr_players; i++) {
        if (stats[i][0] != -1) {
            *line = decode_line(stats[i][0]);
            *column = decode_column(stats[i][0]);
            stats[i][0] = encode_sta(stats[i][0],
            decode_sta(stats[i][0]) + *bonus);
            if (distance(*line, *column, radius + *meltdowns) > radius) {
                stats[i][0] = -1;
                fprintf(out_file, "%s got wet because of global warming.\n",
                names[i]);
                dead += 1;
            }
        }
    }
    free(line);
    free(column);
    free(bonus);
    return dead;
}

short who_wins(long long **stats, short nr_players) {
    short i;
    for (i = 0; i < nr_players; i++) {
        if (stats[i][0] != -1) return i;
    }
    return 0;
}

/* Function to print the scoreboard by making a copy of the list of names
and their IDs and sorting them out. */
void print_scoreboard(long long **stats, char **names,
short *kills, short nr_players, FILE *out_file) {
    short i, *copy_id, *id_aux;
    unsigned short j;
    char **copy_names, *name_aux;
    copy_id = calloc(nr_players, sizeof(short));
    for (i = 0; i < nr_players; i++) {
        copy_id[i] = i;
    }
    id_aux = calloc(1, sizeof(short));
    copy_names = (char**)calloc(nr_players, sizeof(char*));
    for (i = 0; i < nr_players; i++) {
        copy_names[i] = (char*)calloc(30, sizeof(char));
        for (j = 0; j <= strlen(names[i]); j++) {
            copy_names[i][j] = names[i][j];
        }
    }
    for (i = 0; i < nr_players - 1; i++) {
        for (j = i + 1; j < nr_players; j++) {
            if (stats[copy_id[j]][0] != -1 && stats[copy_id[i]][0] == -1) {
                name_aux = copy_names[i];
                copy_names[i] = copy_names[j];
                copy_names[j] = name_aux;
                *id_aux = copy_id[i];
                copy_id[i] = copy_id[j];
                copy_id[j] = *id_aux;
            }
        }
    }
    for (i = 0; i < nr_players - 1; i++) {
        for (j = i + 1; j < nr_players; j++) {
            if (kills[copy_id[j]] > kills[copy_id[i]]) {
                if ((stats[copy_id[i]][0] != -1 && stats[copy_id[j]][0] != -1)
                ||(stats[copy_id[i]][0] == -1 && stats[copy_id[j]][0] == -1)) {
                    name_aux = copy_names[i];
                    copy_names[i] = copy_names[j];
                    copy_names[j] = name_aux;
                    *id_aux = copy_id[i];
                    copy_id[i] = copy_id[j];
                    copy_id[j] = *id_aux;
                }
            }
        }
    }
    for (i = 0; i < nr_players - 1; i++) {
        for (j = i + 1; j < nr_players; j++) {
            if (strcmp(copy_names[i], copy_names[j]) > 0) {
                if ((stats[copy_id[i]][0] != -1 && stats[copy_id[j]][0] != -1)
                ||(stats[copy_id[i]][0] == -1 && stats[copy_id[j]][0] == -1)) {
                    if (kills[copy_id[i]] == kills[copy_id[j]]) {
                        name_aux = copy_names[i];
                        copy_names[i] = copy_names[j];
                        copy_names[j] = name_aux;
                        *id_aux = copy_id[i];
                        copy_id[i] = copy_id[j];
                        copy_id[j] = *id_aux;
                    }
                }
            }
        }
    }
    fprintf(out_file, "SCOREBOARD:\n");
    for (i = 0 ; i < nr_players; i++) {
        fprintf(out_file, "%s\t", copy_names[i]);
        if (stats[copy_id[i]][0] != -1) {
            fprintf(out_file, "DRY");
        } else {
            fprintf(out_file, "WET");
        }
        fprintf(out_file, "\t%hd\n", kills[copy_id[i]]);
    }
    for (i = 0; i < nr_players; i++) {
        free(copy_names[i]);
    }
    free(copy_id);
    free(copy_names);
    free(id_aux);
}

void start_game(char **names, long long **stats, int **map,
short nr_players, short radius, FILE *in_file, FILE *out_file) {
    char *command;
    short *meltdowns, *deads;
    short *kills;

    kills = calloc(nr_players, sizeof(short));
    deads = calloc(1, sizeof(short));
    command = calloc(18, sizeof(char));
    meltdowns = calloc(1, sizeof(short));
    *deads += read_players(names, stats, map, nr_players,
        radius, in_file, out_file);

    while (!feof(in_file)) {
        if (*deads == nr_players - 1) {
            fprintf(out_file, "%s has won.\n",
            names[who_wins(stats, nr_players)]);
            break;
        }
        fscanf(in_file, "%s", command);
        if (strcmp(command, "MOVE") == 0) {
            *deads += move(names, stats, map, nr_players, radius,
            meltdowns, kills, in_file, out_file);
            }
        if (strcmp(command, "SNOWSTORM") == 0) {
            *deads += snowstorm(names, stats, nr_players, in_file, out_file);
        }
        if (strcmp(command, "MELTDOWN") == 0) {
            radius--;
            *meltdowns = *meltdowns + 1;
            *deads += meltdown(names, stats, nr_players, radius, meltdowns,
            in_file, out_file);
        }
        if (strcmp(command, "PRINT_SCOREBOARD") == 0) {
            print_scoreboard(stats, names, kills, nr_players, out_file);
        }
    }
    free(command);
    free(meltdowns);
    free(deads);
    free(kills);
}

void PLAY_SNOWFIGHT() {
    char **names;
    short *radius, i, *nr_players;
    int **map;
    long long **stats;
    FILE *in_file = fopen("snowfight.in", "r");
    FILE *out_file = fopen("snowfight.out", "w");
    radius = malloc(1 * sizeof(short));
    nr_players = malloc(1 * sizeof(short));
    fscanf(in_file, "%hu %hu", radius, nr_players);

    map = malloc(1 * sizeof(int));
    map = alloc_map(map, *radius);
    read_map(*radius, map, in_file);
    stats = malloc(1);
    stats = alloc_stats(stats, *nr_players);
    names = malloc(1);
    names = alloc_names(names, *nr_players);

    start_game(names, stats, map, *nr_players, *radius, in_file, out_file);
    for (i = 0; i <= *radius * 2; i++) {
        free(map[i]);
    }
    free(map);
    for (i = 0; i < *nr_players; i++) {
        free(stats[i]);
        free(names[i]);
    }
    free(stats);
    free(names);
    free(radius);
    free(nr_players);
    fclose(in_file);
    fclose(out_file);
}
int main() {
    PLAY_SNOWFIGHT();
    return 0;
}
