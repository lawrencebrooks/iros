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

void init_game_state()
{
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

#if RLE == 0

u8 get_camera_x(u8 level_index)
{
	return pgm_read_byte(&level_data[level_index*LEVEL_WIDTH*LEVEL_HEIGHT + level_index*4 + 0]);
}

u8 get_camera_y(u8 level_index)
{
	return pgm_read_byte(&level_data[level_index*LEVEL_WIDTH*LEVEL_HEIGHT + level_index*4 + 1]);
}

u8 get_level_tile(u8 level_index, u8 x, u8 y)
{
	return pgm_read_byte(&level_data[level_index*LEVEL_WIDTH*LEVEL_HEIGHT + level_index*4 + 4 + (y*LEVEL_WIDTH+x)]);
}
#else

#endif

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

void clear_overlay()
{
	for(u8 i = 0; i < CAMERA_WIDTH; i++)
	{
		SetTile(i, VRAM_TILES_V, 0);
	}
}

void load_level(u8 index)
{
	u8 level_tile;
	
	Screen.scrollX = 0;
	Screen.scrollY = 0;
	Screen.overlayTileTable = tiles_data;
	Screen.overlayHeight = 1;
	clear_overlay();
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
	//Print(0, VRAM_TILES_V, (char*) strCopyright);
	Print(8, 15, (char*) str1Player);
	Print(8, 16, (char*) strHighscores);
	Print(6, 21, (char*) strSelectHandle);
	Print(4, 26, (char*) strCopyright);
	DrawMap2(6, 5, (const char*) map_splash);
	LBMapSprite(0, map_right_arrow, 0);
	LBMoveSprite(0, 7*8, 15*8, 1, 1);
	LBRotateSprites(1);
}

void update_level()
{
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
