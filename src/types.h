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
	float vy;
	float x;
	float y;
	s16 vx;
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
	char shield;
	Shot shot[MAX_ENEMY_SHOTS];
	Animation anim;
	Animation expl;
	
} Enemy;

typedef struct sPlayer {
	SpriteShared shared;
	u8 active_shots;
	u8 flags;
	u8 grace_frame;
	u8 direction;
	u8 width;
	u8 height;
	char shield;
	Shot shot[MAX_PLAYER_SHOTS];
	Animation idle;
	Animation run;
	Animation jump;
	Animation prone;
	Animation expl;
} Player;

typedef struct sGameState {
	u16 scroll_src_x;
	u16 scroll_src_y;
	u16 camera_x;
	u16 camera_y;
	u16 score;
	u16 time;
	u16 column_count;
	u8 lives;
	u8 current_screen;
	u8 current_level;
	u8 paused;
	u8 selection;
	u8 frame_counter;
	u8 scroll_x;
	u8 scroll_y;
	u8 scroll_dest_x;
	u8 scroll_dest_y;
	u8 active_enemies;
	u8 spawn_rate;
	Player player;
	Enemy enemies[MAX_ENEMIES];
	JoyPadState joypadState;
} Game;

#endif
