/*
 * Copyright 2015 Lawrence Brooks
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LB_LEVELS_H
#define LB_LEVELS_H

#define LEVEL_COUNT 11

#define L_EMPTY 0
#define L_BRICK 1
#define L_METAL 2
#define L_TL 3
#define L_TR 4
#define L_BL 5
#define L_BR 6
#define L_SPEED 7
#define L_EXPLODE 8
#define L_ROCKET 9
#define L_SCOPE 44
#define L_FENCE 55
#define L_TURF 66
#define L_WATER 77
#define L_P1_SPAWN 88
#define L_P2_SPAWN 99

const unsigned char level_data [] PROGMEM={
};

#endif
