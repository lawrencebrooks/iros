/*
 * Program: Iris
 *
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
#include <uzebox.h>
#include <avr/pgmspace.h>
#include <string.h>
#include "data/tiles.h"
#include "data/sprites.h"
#include "data/levels.h"
#include "data/patches.h"
#include "types.h"
#include "strings.h"
#include "utils.h"
#include "macros.h"

/* Prototypes */
void load_eeprom(struct EepromBlockStruct* block);
void save_eeprom(struct EepromBlockStruct* block);
void fade_through();
void clear_sprites();
void level_transition(u8 index);
void load_splash();
void exit_game();

// Globals
Game game;
struct EepromBlockStruct scores;

char* idle_anim[1];
char* jump_anim[1];
char* prone_anim[1];
char* run_anim[4];

void init_game_state()
{
	game.camera_x = 0;
	game.camera_y = 0;
	game.current_level = 0;
	game.current_screen = 0;
	game.paused = 0;
	game.selection = 0;
	game.player.active_shots = 0;
	game.player.flags = 0;
	game.player.grace_frame = 0;
	game.player.shared.gravity = GRAVITY;
	game.player.shared.vx = 0;
	game.player.shared.vy = 0;
	
	game.player.idle.anim_count = 1;
	game.player.idle.current_anim = 0;
	game.player.idle.frame_count = 0;
	game.player.idle.frames_per_anim = 1;
	game.player.idle.looped = 0;
	game.player.idle.reversing = 0;
	game.player.idle.anims = idle_anim;
	game.player.idle.anims[0] = (char*) map_hero_idle;
	
	game.player.jump.anim_count = 1;
	game.player.jump.current_anim = 0;
	game.player.jump.frame_count = 0;
	game.player.jump.frames_per_anim = 1;
	game.player.jump.looped = 0;
	game.player.jump.reversing = 0;
	game.player.jump.anims = jump_anim;
	game.player.jump.anims[0] = (char*) map_hero_jump;
	
	game.player.prone.anim_count = 1;
	game.player.prone.current_anim = 0;
	game.player.prone.frame_count = 0;
	game.player.prone.frames_per_anim = 1;
	game.player.prone.looped = 0;
	game.player.prone.reversing = 0;
	game.player.prone.anims = prone_anim;
	game.player.prone.anims[0] = (char*) map_hero_prone;
	
	game.player.run.anim_count = 4;
	game.player.run.current_anim = 0;
	game.player.run.frame_count = 0;
	game.player.run.frames_per_anim = FRAMES_PER_RUN_CYCLE;
	game.player.run.looped = 0;
	game.player.run.reversing = 0;
	game.player.run.anims = run_anim;
	game.player.run.anims[0] = (char*) map_hero_step_0;
	game.player.run.anims[1] = (char*) map_hero_step_1;
	game.player.run.anims[2] = (char*) map_hero_step_2;
	game.player.run.anims[3] = (char*) map_hero_step_3;
}

void load_eeprom(struct EepromBlockStruct* block)
/*
 * Load from EEPROM
 *  EEPROM Statuses
 *  0x00 = Success
 *  0x01 = EEPROM_ERROR_INVALID_BLOCK
 *  0x02 = EEPROM_ERROR_FULL
 *  0x03 = EEPROM_ERROR_BLOCK_NOT_FOUND
 *  0x04 = EEPROM_ERROR_NOT_FORMATTED
 */
{
	u8 status = 0;
	status = EepromReadBlock(block->id, block);
	if (status == 0x03)
	{
		status = EepromWriteBlock(block);
	}
}

void save_eeprom(struct EepromBlockStruct* block)
/*
 * Save to EEPROM
 */
{
	EepromWriteBlock(block);
}

void fade_through()
{
	FadeOut(FRAMES_PER_FADE, true);
	ClearVram();
	FadeIn(FRAMES_PER_FADE, false);
}

void render_level_tile(u8 level_tile, u8 x, u8 y)
{
	switch (level_tile)
	{
		case L_ICE_SKY : DrawMap2(x, y, map_ice_sky); break;
		case L_ICE_MIDDLE : DrawMap2(x, y, map_ice_middle); break;
		case L_ICE_LEFT : DrawMap2(x, y, map_ice_left); break;
		case L_ICE_RIGHT : DrawMap2(x, y, map_ice_right); break;
		case L_ICE_BOTTOM : DrawMap2(x, y, map_ice_bottom); break;
		case L_ICE_FAR : DrawMap2(x, y, map_ice_far); break;
		case L_ICE_HORIZON : DrawMap2(x, y, map_ice_horizon); break;
	}
}

void clear_overlay(u8 overlayHeight)
{
	for(u8 x = 0; x < CAMERA_WIDTH; x++)
	{
		for(u8 y = 0; y < overlayHeight; y++)
		{
			SetTile(x, VRAM_TILES_V+y, 0);
		}
	}
}

#if RLE == 0

u8 get_camera_x(u8 level_index)
{
	return pgm_read_byte(&level_data[level_index*LEVEL_WIDTH*LEVEL_HEIGHT + level_index*4 + 0]);
}

u8 get_camera_y(u8 level_index)
{
	return pgm_read_byte(&level_data[level_index*LEVEL_WIDTH*LEVEL_HEIGHT + level_index*4 + 1]);
}

u8 get_hero_spawn_x(u8 level_index)
{
	return pgm_read_byte(&level_data[level_index*LEVEL_WIDTH*LEVEL_HEIGHT + level_index*4 + 2]);
}

u8 get_hero_spawn_y(u8 level_index)
{
	return pgm_read_byte(&level_data[level_index*LEVEL_WIDTH*LEVEL_HEIGHT + level_index*4 + 3]);
}

u8 get_level_tile(u8 level_index, u8 x, u8 y)
{
	return pgm_read_byte(&level_data[level_index*LEVEL_WIDTH*LEVEL_HEIGHT + level_index*4 + 4 + (y*LEVEL_WIDTH+x)]);
}

#else

#endif

void load_level(u8 index)
{
	u8 level_tile;
	
	Screen.scrollX = 0;
	Screen.scrollY = 0;
	Screen.scrollHeight = 30;
	Screen.overlayTileTable = tiles_data;
	Screen.overlayHeight = 2;
	clear_overlay(2);
	game.camera_x = get_camera_x(index);
	game.camera_y = get_camera_y(index);
	
	for (u8 x = game.camera_x; x < game.camera_x + CAMERA_WIDTH; x++)
	{
		for (u8 y = game.camera_y; y < game.camera_y + CAMERA_HEIGHT; y++)
		{
			level_tile = get_level_tile(index, x, y);
			render_level_tile(level_tile, x - game.camera_x, y - game.camera_y);
		}
	}
	game.player.shared.x = get_hero_spawn_x(index)*8;
	game.player.shared.y = get_hero_spawn_y(index)*8;
	LBMapSprite(0, LBGetNextFrame(&game.player.idle), 0);
}

void level_transition(u8 index)
{
	FadeOut(FRAMES_PER_FADE, true);
	ClearVram();
	clear_sprites();
	Print(8, 12, (char*) strLevels+index*16);
	FadeIn(1, true);
	LBWaitSeconds(TEXT_LINGER);
	FadeOut(1, true);
	ClearVram();
	FadeIn(FRAMES_PER_FADE, false);
	load_level(index);
}

void update_level()
{
	LBMoveSprite(0, game.player.shared.x, game.player.shared.y, 2, 3);
	LBRotateSprites(6);
}

void clear_sprites()
{
	for(char i = 0; i < MAX_EXTENDED_SPRITES; i++)
	{
		LBMoveSprite(i, OFF_SCREEN, 0, 1, 1);
	}
	LBRotateSprites(MAX_SPRITES);
}

void save_score()
{
	save_eeprom(&scores);
}

void exit_game()
{
    save_score();
	fade_through();
	SetSpriteVisibility(true);
	init_game_state();
	load_splash();
}

void load_splash()
{
	game.current_screen = SPLASH;
	game.selection = START_SELECTED;
	clear_sprites();
	Print(8, 15, (char*) str1Player);
	Print(8, 16, (char*) strHighscores);
	Print(6, 21, (char*) strSelectHandle);
	Print(4, 26, (char*) strCopyright);
	DrawMap2(6, 5, (const char*) map_splash);
	LBMapSprite(0, map_right_arrow, 0);
	LBMoveSprite(0, 7*8, 15*8, 1, 1);
	LBRotateSprites(1);
}

char select_pressed(JoyPadState* p)
{
	return (p->pressed & BTN_A) || (p->pressed & BTN_START);
}

void update_splash()
{
	if (game.joypadState.pressed & BTN_DOWN && game.selection == START_SELECTED)
	{
		game.selection = HIGH_SCORES_SELECTED;
		LBMoveSprite(0, 7*8, 16*8, 1, 1);
	}
	else if (game.joypadState.pressed & BTN_UP && game.selection == HIGH_SCORES_SELECTED)
	{
		game.selection = START_SELECTED;
		LBMoveSprite(0, 7*8, 15*8, 1, 1);
	}
	else if (select_pressed(&game.joypadState) && game.selection == START_SELECTED)
	{
		game.current_screen = LEVEL;
		level_transition(0);
		return;
	}
	LBRotateSprites(1);
}

int main()
{
	// Initialize
	InitMusicPlayer(my_patches);
	ClearVram();
	SetMasterVolume(0xff);
	SetTileTable(tiles_data);
	SetSpritesTileTable(sprites_data);
	SetFontTilesIndex(TILES_DATA_SIZE);
	FadeIn(FRAMES_PER_FADE, false);
	init_game_state();
	load_splash();
	while (1)
	{
		WaitVsync(1);
		LBGetJoyPadState(&game.joypadState, 0);
		switch (game.current_screen)
		{
			case SPLASH:
				update_splash();
				break;
			case LEVEL:
				update_level();
				break;
			default:
				break;
		}
	}
}
