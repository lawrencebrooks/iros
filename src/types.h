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
	u8 direction;
	u8 speed;
	u8 recoiled;
	float x;
	float y;
} SpriteShared;

typedef struct sShot {
	SpriteShared shared;
	u8 shot_type;
	u8 hit_count;
	u8 active;
	u8 distance;
	Animation anim;

} Shot;

typedef struct sPlayer {
	SpriteShared shared;
	u8 score;
	u8 active_shots;
	u8 flags;
	u8 grace_frame;
	u8 max_speed;
	Shot shot[MAX_SHOTS];
	Animation idle;
	Animation run_anim;
	Animation jump_anim;
	Animation prone_anim;
	Animation expl_anim;
} Player;

typedef struct sGameState {
	u8 current_screen;
	u8 current_level;
	u8 paused;
	u8 selection;
	u8 camera_x;
	u8 camera_y;
	Player player;
	JoyPadState joypadState;
} Game;

#endif
