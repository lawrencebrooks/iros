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

#ifndef LB_MACROS_H
#define LB_MACROS_H

// EEPROM ID's
#define EEPROM_HANDLES_ID 0x8A
#define EEPROM_TANK_RANK_ID 0x8B

// Screens
#define SPLASH 0
#define TANK_RANK 1
#define HANDLE_SELECT 2
#define LEVEL 3

// Selections
#define PVCPU 0
#define PVP 1
#define TR 2
#define CPUVCPU 3

// Frame counts
#define FRAMES_PER_FADE 3
#define FRAMES_PER_BANTER 90
#define FRAMES_PER_GRACE 120
#define FRAMES_PER_BLANK 20
#define FRAMES_PER_ANIM 5
#define FRAMES_PER_SCOPE 30
#define FRAMES_PER_SUB 60

// Handle select states
#define SELECTING 0
#define EDITING 1
#define CONFIRMED 2

// Movement
#define D_UP 0
#define D_RIGHT 1
#define D_DOWN 2
#define D_LEFT 3
#define MAX_SPEED 50
#define OVER_SPEED 65
#define WATER_SPEED 25

// Shot
#define BASIC_SHOT 0
#define ROCKET_SHOT 1
#define BASIC_SHOT_HIT_COUNT 1
#define ROCKET_SHOT_HIT_COUNT 5
#define SHOT_REBOUNDS 4
#define SHOT_SPEED 120
#define SHOT_OVER_SPEED 170
#define MAX_SHOTS 1
#define DISTANCE_TO_ARM 8
#define HIT_METAL 1
#define HIT_BRICK 2
#define HIT_ANGLE 4

// General macros
#define FRAME_TIME 0.0166666
#define TEXT_LINGER 4 // seconds
#define MAX_LEVEL_SCORE 10
#define TILE_ANIMATIONS_LENGTH 2
#define DEMO_WAIT 600
#define DEMO_LENGTH 1800
#define TANK_RANK_LENGTH 900
#define SCOPE_REVEAL 1000

// Boss macros
#define BOSS_FIGHT_SCOPE_LOADING 1
#define BOSS_FIGHT_SUB_LOADING 2
#define BOSS_FIGHT 4
#define BOSS_FIGHT_WON 8
#define BOSS_FIGHT_LOST 16
#define BOSS_FIGHT_SUB_SINKING 32
#define BOSS_FIGHT_PLAYER_LIVES 2
#define BOSS_TURRET_LIVES 5
#define BOSS_TURRET_SPEED 90
#define BOSS_TURRET_SHOT_SPEED 250
#define BOSS_TURRET_SHOT 2
#define BOSS_TURRET_SHOT_HIT_COUNT 1
#define BOSS_TURRET_1_LEFT_LIMIT 40
#define BOSS_TURRET_1_RIGHT_LIMIT 96
#define BOSS_TURRET_2_LEFT_LIMIT 136
#define BOSS_TURRET_2_RIGHT_LIMIT 192

// Flags
#define EXPLODING_FLAG 1
#define ROCKET_FLAG 2
#define OVER_SPEED_FLAG 4

// Sound Effects
#define SFX_BANTER if (!((player1.flags & EXPLODING_FLAG) || (player2.flags & EXPLODING_FLAG))) TriggerNote(PCM_CHANNEL,PATCH_BANTER,30,0x5f)
#define SFX_BRICK_EXPLODE if (!((player1.flags & EXPLODING_FLAG) || (player2.flags & EXPLODING_FLAG))) TriggerNote(PCM_CHANNEL,PATCH_BRICK_EXPLODE,23,0xdf)
#define SFX_CANNONBALL if (!((player1.flags & EXPLODING_FLAG) || (player2.flags & EXPLODING_FLAG))) TriggerNote(PCM_CHANNEL,PATCH_CANNONBALL,23,0xff)
#define SFX_ITEM if (!((player1.flags & EXPLODING_FLAG) || (player2.flags & EXPLODING_FLAG))) TriggerNote(PCM_CHANNEL,PATCH_ITEM,23,0xff)
#define SFX_METAL if (!((player1.flags & EXPLODING_FLAG) || (player2.flags & EXPLODING_FLAG))) TriggerNote(PCM_CHANNEL,PATCH_METAL,23,0xff)
#define SFX_NAVIGATE TriggerNote(PCM_CHANNEL,PATCH_NAVIGATE,23,0xff)
#define SFX_ROCKET if (!((player1.flags & EXPLODING_FLAG) || (player2.flags & EXPLODING_FLAG))) TriggerNote(PCM_CHANNEL,PATCH_ROCKET,23,0xff)
#define SFX_TANK_EXPLODE TriggerNote(PCM_CHANNEL,PATCH_TANK_EXPLODE,16,0xff)
#define SFX_ALARM TriggerFx(PATCH_ALARM,0xff,true);

// AI Macros
#define DEFAULT_FRAMES_PER_GOAL 180
#define DEFAULT_FRAMES_PER_SHOT 10
#define FRAMES_PER_DEADLOCK 320
#define AI_SPEED_FACTOR 1.4

#endif
