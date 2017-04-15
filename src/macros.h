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

#ifndef LB_MACROS_H
#define LB_MACROS_H

// Debug Flags
#define DEBUG_MODE 1
#define DEBUG_LEVEL 0
#define DEBUG_LEVEL_BEGINNING 0
#define DEBUG_LEVEL_END 1
#define DEBUG_LEVEL_LOCATION DEBUG_LEVEL_END
#define DEBUG_GODMODE 1

// EEPROM ID
#define EEPROM_SCORES_ID 0x8C

// Screens
#define SPLASH 0
#define HIGH_SCORES 1
#define LEVEL 2
#define INTRO 4

// Selections
#define START_SELECTED 0
#define HIGH_SCORES_SELECTED 1

// Frame counts
#define FRAMES_PER_FADE 1
#define FRAMES_PER_GRACE 120

// General macros
#define FRAME_TIME 0.016666666666666666
#define MAX_PLAYER_SHOTS 3
#define TEXT_LINGER 3
#define CAMERA_WIDTH 28
#define CAMERA_HEIGHT 25
#define LEVEL_WIDTH 255
#define LEVEL_HEIGHT 52
#define OVERLAP_THRESHOLD 4
#define KILL_SCORE 10
#define EXPLOSION_FRAME_COUNT 5
#define CHARACTER_DELAY_US 6000
#define TALLY_DELAY 1500
#define CHALLENGE_LENGTH 35

// Player macros
#define FRAMES_PER_RUN_CYCLE 12
#define RUN_SPEED 60
#define SPACE_SHIP_SPEED 60
#define JUMP_SPEED 127
#define GRAVITY 200
#define MAX_SPEED 120
#define D_LEFT 0
#define D_RIGHT 1
#define LIVES 5
#define PLAYER_SHIELD 10
#define BOSS_SHIELD 30

// State flags
#define JUMPING 1
#define RUNNING 2
#define PRONE 4
#define IDLE 8
#define EXPLODING 16

// Shot macros
#define BASIC_SHOT 0
#define BASIC_ENEMY_SHOT 1
#define SHOT_SPEED 127
#define ENEMY_SHOT_SPEED 100
#define BASIC_ENEMY_SHOT_HITCOUNT 1
#define BSIC_SHOT_HITCOUNT 1

// Sprite slots
#define PLAYER_SLOT 0
#define PLAYER_SHOT_SLOT 6
#define ENEMY_SLOT 9
#define ENEMY_SHOT_SLOT 15
#define BOSS_SLOT 9
#define BOSS_SHOT_SLOT 15


// Enemy macros
#define MAX_ENEMIES 5
#define MAX_ENEMY_SHOTS 1
#define ENEMY_SPIDER 0
#define ENEMY_SHARK 1
#define BASE_SPAWN_RATE 10
#define SPIDER_IDLE_FRAMES 128
#define SPIDER_JUMP_SPEED 100
#define SPIDER_SIDE_SPEED 60
#define SHARK_SPEED 20
#define SPIDER_FRAMES_PER_IDLE 10
#define ENEMY_SPIDER_SHIELD 2
#define ENEMY_SHARK_SHIELD 4
#define ENEMY_SHOT_DELAY_FRAMES 60
#define ENEMY_SPAWN_CUTOFF 1480
#define BOSS_UPDATE_THRESHOLD 1784

// Player AI flags
#define AI_NOT_READY 0
#define AI_READY 1
#define AI_WALKING 2
#define AI_SHOOTING 4
#define AI_JUMPING 8
#define AI_PRONE 16
#define AI_DEFEATED 32

// Sound Effects
#define SFX_NAVIGATE TriggerFx(PATCH_NAVIGATE,0xff,true)
#define SFX_DIALOGUE TriggerFx(PATCH_NAVIGATE,0xff,true)
#define SFX_PLAYER_SHOOT TriggerNote(2,PATCH_PLAYER_SHOOT,0x23,0xff)
#define SFX_HIT TriggerFx(PATCH_HIT,0xff,true)
#define SFX_PLAYER_EXPLODE TriggerFx(PATCH_PLAYER_EXPLODE,0xff,true)
#define SFX_ENEMY_EXPLODE TriggerFx(PATCH_ENEMY_EXPLODE,0xff,true)

#endif
