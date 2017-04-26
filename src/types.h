/*
 * Copyright 2016 Lawrence Brooks
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

#ifndef LB_TYPES_H
#define LB_TYPES_H

#include "utils.h"
#include "macros.h"

typedef struct sSpriteShared {
	u16 x;
	u16 y;
	s8 vy;
	s8 vx;
	u8 gravity;
} SpriteShared;

typedef struct sShot {
	SpriteShared shared;
	u8 shot_type;
	u8 hit_count;
	u8 active;
	Animation anim;

} Shot;

typedef struct sEnemy {
	SpriteShared shared;
	u8 active;
	u8 flags;
	u8 direction;
	u8 width;
	u8 height;
	u8 enemy_type;
	u8 frame_count;
	u8 shot_frame_count;
	s8 shield;
	Shot shot[MAX_ENEMY_SHOTS];
	Animation anim;
	Animation expl;
	
} Enemy;

typedef struct sPlayer {
	SpriteShared shared;
	u8 active_shots;
	u8 flags;
	u8 ai_flags;
	u8 direction;
	u8 width;
	u8 height;
	u8 active;
	s8 shield;
	Shot shot[MAX_PLAYER_SHOTS];
	Animation idle;
	Animation run;
	Animation jump;
	Animation prone;
	Animation expl;
	JoyPadState controls;
} Player;

typedef struct sGameState {
	u16 camera_x;
	u16 score;
	u16 level_score;
	u16 time;
	u8 camera_y;
	u8 scroll_src_x;
	u8 scroll_src_y;
	u8 scroll_x;
	u8 scroll_y;
	u8 scroll_dest_x;
	u8 scroll_dest_y;
	u8 level_width;
	u8 level_height;
	u8 column_count;
	s8 high_score_index;
	u8 lives;
	u8 current_screen;
	u8 current_level_index;
	u8 raw_level_width;
	u8 raw_level_height;
	u8 paused;
	u8 selection;
	u8 frame_counter;
	u8 active_enemies;
	u8 spawn_rate;
	u8 level_ended;
	Player player;
	Player boss;
	Enemy enemies[MAX_ENEMIES];
	char* current_level;
} Game;

#endif
