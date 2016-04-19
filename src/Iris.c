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
#include "data/planetmusic.h"
#include "data/spacemusic.h"
#include "types.h"
#include "strings.h"
#include "utils.h"
#include "macros.h"

/* Prototypes */
void load_eeprom(struct EepromBlockStruct* block);
void save_eeprom(struct EepromBlockStruct* block);
void fade_through();
void clear_sprites(u8 from, u8 count);
void level_transition(u8 index);
void load_splash();
void exit_game();
void handle_player_death();
u8 collision_detect_level(SpriteShared* s, u8 tile_width, u8 tile_height);
u8 map_explosion(u8* flags, Animation* anim, u8 slot, u8 width, u8 height);

// Globals
Game game;
struct EepromBlockStruct scores;

char* idle_anim[1];
char* jump_anim[1];
char* prone_anim[1];
char* run_anim[4];
char* shot_anim[1];
char* spider_anim[2];
char* enemy_shot_anim[1];
char* expl_anim[3];

void init_game_state()
{
	game.lives = LIVES;
	game.score = 0;
	game.time = 0;
}

void init_enemy_state()
{
	for (u8 i = 0; i < MAX_ENEMIES; i++)
	{
		game.enemies[i].active = 0;
		for (u8 j = 0; j < MAX_ENEMY_SHOTS; j++)
		{
			game.enemies[i].shot[j].active = 0;
		}
	}
	game.active_enemies = 0;
}

void init_player_state()
{
	game.player.direction = D_RIGHT;
	game.player.flags = IDLE;
	game.player.width = 2;
	game.player.height = 3;
	game.player.shield = PLAYER_SHIELD;
	game.player.idle.anim_count = 1;
	game.player.idle.frames_per_anim = 1;
	game.player.idle.anims = idle_anim;
	game.player.idle.anims[0] = (char*) map_hero_idle;
	
	game.player.jump.anim_count = 1;
	game.player.jump.frames_per_anim = 1;
	game.player.jump.anims = jump_anim;
	game.player.jump.anims[0] = (char*) map_hero_jump;
	
	game.player.prone.anim_count = 1;
	game.player.prone.frames_per_anim = 1;
	game.player.prone.anims = prone_anim;
	game.player.prone.anims[0] = (char*) map_hero_prone;
	
	game.player.expl.anim_count = 3;
	game.player.expl.frames_per_anim = EXPLOSION_FRAME_COUNT;
	game.player.expl.anims = expl_anim;
	game.player.expl.anims[0] = (char*) map_explosion_0;
	game.player.expl.anims[1] = (char*) map_explosion_1;
	game.player.expl.anims[2] = (char*) map_explosion_2;
	
	game.player.run.anim_count = 4;
	game.player.run.frames_per_anim = FRAMES_PER_RUN_CYCLE;
	game.player.run.anims = run_anim;
	game.player.run.anims[0] = (char*) map_hero_step_0;
	game.player.run.anims[1] = (char*) map_hero_step_1;
	game.player.run.anims[2] = (char*) map_hero_step_2;
	game.player.run.anims[3] = (char*) map_hero_step_3;
	
	for (u8 i = 0; i < MAX_PLAYER_SHOTS; i++)
	{
		game.player.shot[i].hit_count = 1;
		game.player.shot[i].shot_type = BASIC_SHOT;
		game.player.shot[i].anim.anim_count = 1;
		game.player.shot[i].anim.frames_per_anim = 1;
		game.player.shot[i].anim.anims = shot_anim;
		game.player.shot[i].anim.anims[0] = (char*) map_hero_shot;
	}
}

void init_enemy_spider(u8 i, u16 x, u16 y)
{
	game.enemies[i].active = 1;
	game.enemies[i].direction = D_LEFT;
	game.enemies[i].flags = IDLE;
	game.enemies[i].width = 1;
	game.enemies[i].height = 1;
	game.enemies[i].enemy_type = ENEMY_SPIDER;
	game.enemies[i].frame_count = 0;
	game.enemies[i].shot_frame_count = 0;
	game.enemies[i].shield = ENEMY_SPIDER_SHIELD;
	
	game.enemies[i].anim.anim_count = 2;
	game.enemies[i].anim.frames_per_anim = SPIDER_FRAMES_PER_IDLE;
	game.enemies[i].anim.anims = spider_anim;
	game.enemies[i].anim.anims[0] = (char*) map_enemy_spider_0;
	game.enemies[i].anim.anims[1] = (char*) map_enemy_spider_1;
	game.enemies[i].shared.gravity = 0;
	game.enemies[i].shared.vx = 0;
	game.enemies[i].shared.vy = 0;
	game.enemies[i].shared.x = x;
	game.enemies[i].shared.y = y;
	
	game.enemies[i].expl.anim_count = 3;
	game.enemies[i].expl.frames_per_anim = EXPLOSION_FRAME_COUNT;
	game.enemies[i].expl.anims = expl_anim;
	game.enemies[i].expl.anims[0] = (char*) map_explosion_0;
	game.enemies[i].expl.anims[1] = (char*) map_explosion_1;
	game.enemies[i].expl.anims[2] = (char*) map_explosion_2;
		
	for (u8 j = 0; j < MAX_ENEMY_SHOTS; j++)
	{
		game.enemies[i].shot[j].hit_count = 1;
		game.enemies[i].shot[j].active = 0;
		game.enemies[i].shot[j].shot_type = BASIC_ENEMY_SHOT;
		game.enemies[i].shot[j].anim.anim_count = 1;
		game.enemies[i].shot[j].anim.frames_per_anim = 1;
		game.enemies[i].shot[j].anim.anims = enemy_shot_anim;
		game.enemies[i].shot[j].anim.anims[0] = (char*) map_enemy_shot;
		game.enemies[i].shot[j].shared.vx = 0;
		game.enemies[i].shot[j].shared.vy = 0;
		game.enemies[i].shot[j].shared.x = x;
		game.enemies[i].shot[j].shared.y = y;
	}
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
		case L_ICE_SKY : LBDrawTile(x, y, map_ice_sky); break;
		case L_ICE_MIDDLE : LBDrawTile(x, y, map_ice_middle); break;
		case L_ICE_LEFT : LBDrawTile(x, y, map_ice_left); break;
		case L_ICE_RIGHT : LBDrawTile(x, y, map_ice_right); break;
		case L_ICE_BOTTOM : LBDrawTile(x, y, map_ice_bottom); break;
		case L_ICE_FAR : LBDrawTile(x, y, map_ice_far); break;
		case L_ICE_HORIZON : LBDrawTile(x, y, map_ice_horizon); break;
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
u8 read_level_byte(u16 index)
{
	return pgm_read_byte(&level_data[index]);
}
#else
u8 read_level_byte(u16 index)
{
	// C Code
	
	u16 counter = index / 128 * 16;
	u16 cumulative_rlength = counter * 8;
	u8 rlength = 0;
	u8 value = 0;
	
	/*while (1)
	{
		rlength = pgm_read_byte(&level_data[counter]);
		value = pgm_read_byte(&level_data[counter+1]);
		cumulative_rlength += rlength;
		if (cumulative_rlength > index)
		{
			return value;
		}
		counter += 2;
	}
	return 0;*/
	
	// ASM Code
	index += 1;
	counter += (u16) &level_data;
	asm volatile ("" "\n\t"
	"while1:" "\n\t"
	"lpm  %[rln]   , %a[cntr]+" "\n\t"
	"lpm  %[vl]    , %a[cntr]+" "\n\t"
	"add  %A[crln], %[rln]" "\n\t"
	"adc  %B[crln], __zero_reg__" "\n\t"
	"cp   %A[crln], %A[idx]" "\n\t"
	"cpc  %B[crln], %B[idx]" "\n\t"
	"brsh while1done" "\n\t"
	"rjmp while1" "\n\t"
	"while1done:" "\n\t"
	: [idx]  "+r" (index),
	  [cntr] "+z" (counter),
	  [crln] "+r" (cumulative_rlength),
	  [rln]  "+r" (rlength),
	  [vl]   "=&r" (value));
	return value;
}

#endif

u8 get_camera_x(u8 level_index)
{
	return read_level_byte(level_index*LEVEL_WIDTH*LEVEL_HEIGHT + level_index*4 + 0);
}

u8 get_camera_y(u8 level_index)
{
	return read_level_byte(level_index*LEVEL_WIDTH*LEVEL_HEIGHT + level_index*4 + 1);
}

u8 get_hero_spawn_x(u8 level_index)
{
	return read_level_byte(level_index*LEVEL_WIDTH*LEVEL_HEIGHT + level_index*4 + 2);
}

u8 get_hero_spawn_y(u8 level_index)
{
	return read_level_byte(level_index*LEVEL_WIDTH*LEVEL_HEIGHT + level_index*4 + 3);
}

u8 get_level_tile(u8 level_index, u16 x, u16 y)
{
	return read_level_byte(level_index*LEVEL_WIDTH*LEVEL_HEIGHT + level_index*4 + 4 + (y*LEVEL_WIDTH+x));
}

void render_camera_view()
{
	u8 level_tile;
	u16 camara_x_tile = game.camera_x/8;
	u16 camera_y_tile = game.camera_y/8;
	
	for (u16 x = camara_x_tile; x <= camara_x_tile + CAMERA_WIDTH; x++)
	{
		for (u16 y = camera_y_tile; y <= camera_y_tile + CAMERA_HEIGHT; y++)
		{
			level_tile = get_level_tile(game.current_level, x, y);
			render_level_tile(level_tile, x - camara_x_tile, y - camera_y_tile);
		}
	}
}

void spawn_enemy(u16 x, u16 y)
{
	for (u8 i = 0; i < MAX_ENEMIES; i++)
	{
		if (!game.enemies[i].active)
		{
			for (u8 j = 0; j < MAX_ENEMY_SHOTS; j++)
			{
				if (!game.enemies[i].shot[j].active)
				{
					init_enemy_spider(i, x, y);
					game.active_enemies++;
					return;
				}
			}
		}
	}
}

char solid_tile(u8 level_tile)
{
	switch (level_tile)
	{
		case L_ICE_MIDDLE : return 1;
		case L_ICE_LEFT : return 1;
		case L_ICE_RIGHT : return 1;
		case L_ICE_BOTTOM : return 1;
	}
	return 0;
}

void append_tile_column()
{
	u8 enemy_spawned = 0;
	u8 level_tile;

	game.column_count++;
    for (u8 y = 0; y <= CAMERA_HEIGHT; y++)
	{
		level_tile = get_level_tile(game.current_level, game.scroll_src_x, y + game.camera_y / 8);
		render_level_tile(level_tile, game.scroll_dest_x, (y + Screen.scrollY / 8) % 30);
		if (game.column_count % game.spawn_rate == game.spawn_rate - 1 && game.active_enemies < MAX_ENEMIES && !enemy_spawned && solid_tile(level_tile))
		{
			spawn_enemy(game.camera_x+(CAMERA_WIDTH+1)*8, (game.camera_y / 8 + y - 1)*8);
			enemy_spawned = 1;
		}
    }
	
    game.scroll_src_x++;
	game.scroll_dest_x++;
    if(game.scroll_dest_x>=32)game.scroll_dest_x=0;
}

void append_tile_row()
{
	u8 level_tile;

    for (u8 x = 0; x <= CAMERA_WIDTH; x++)
	{
		level_tile = get_level_tile(game.current_level, x + game.camera_x / 8, game.scroll_src_y);
		render_level_tile(level_tile, (x + Screen.scrollX / 8) % 32, game.scroll_dest_y);
    }
	
    game.scroll_src_y++;
	game.scroll_dest_y++;
    if(game.scroll_dest_y>=30)game.scroll_dest_y=0;
}

void move_camera_x()
{	
	game.camera_x++;
	Scroll(1,0);
	if (++game.scroll_x == 8)
	{
		game.scroll_x = 0;
		append_tile_column();
	}
}

void move_camera_y()
{	
	game.camera_y+=2;
	game.scroll_y+=2;
	Scroll(0,2);
	if (game.scroll_y == 8)
	{
		game.scroll_y = 0;
		append_tile_row();
	}
}

void load_level(u8 index)
{	
	Screen.scrollX = 0;
	Screen.scrollY = 0;
	Screen.scrollHeight = 30;
	Screen.overlayTileTable = tiles_data;
	Screen.overlayHeight = 2;
	clear_overlay(2);
	game.current_level = index;
	game.column_count = 0;
	game.camera_x = get_camera_x(index);
	game.camera_y = get_camera_y(index);
	game.player.shared.x = get_hero_spawn_x(index)*8;
	game.player.shared.y = get_hero_spawn_y(index)*8;
	game.camera_x *= 8;
	game.camera_y *= 8;
	game.scroll_src_x = game.camera_x / 8 + CAMERA_WIDTH + 1;
    game.scroll_dest_x = game.camera_x / 8 + CAMERA_WIDTH + 1;
	game.scroll_src_y = game.camera_y / 8 + CAMERA_HEIGHT + 1;
    game.scroll_dest_y =  game.camera_y / 8 + CAMERA_HEIGHT + 1;
	game.scroll_x = 0;
	game.scroll_y = 0;
	game.spawn_rate = BASE_SPAWN_RATE;
	render_camera_view();
	Print(0, VRAM_TILES_V-1, (char*) strLives);
	Print(17, VRAM_TILES_V-1, (char*) strScore);
	Print(0, VRAM_TILES_V-2, (char*) strShield);
	Print(18, VRAM_TILES_V-2, (char*) strTime);
}

void level_transition(u8 index)
{
	FadeOut(FRAMES_PER_FADE, true);
	ClearVram();
	clear_sprites(0, MAX_EXTENDED_SPRITES);
	LBRotateSprites();
	init_player_state();
	init_enemy_state();
	Print(8, 12, (char*) strLevels+index*16);
	FadeIn(1, true);
	StartSong(planetsong);
	LBWaitSeconds(TEXT_LINGER);
	FadeOut(1, true);
	ClearVram();
	FadeIn(FRAMES_PER_FADE, false);
	load_level(index);
}

char on_solid_ground(SpriteShared* s, u8 tile_width, u8 tile_height)
{
	u8 lt1, lt2;
	
	// Bottom
	for (u8 x = 0; x < tile_width; x++)
	{
		lt1 = get_level_tile(game.current_level, s->x / 8 + x, (s->y + tile_height*8) / 8);
		lt2 = get_level_tile(game.current_level, (s->x+7) / 8 + x, (s->y + tile_height*8) / 8);
		if (solid_tile(lt1) || solid_tile(lt2))
		{
			return 1;
		}
	}
	return 0;
}

u8 find_shot_slot()
{
	for (u8 i = 0; i < MAX_PLAYER_SHOTS; i++)
	{
		if (!game.player.shot[i].active) return i;
	}
	return 0;
}

void update_shot()
{
	// Shot updates
	u8 idx;
	if (game.joypadState.pressed & BTN_A && game.player.active_shots < MAX_PLAYER_SHOTS)
	{
		SFX_PLAYER_SHOOT;
		idx = find_shot_slot();
		LBMapSprite(PLAYER_SHOT_SLOT+idx, LBGetNextFrame(&game.player.shot[idx].anim), 0);
		game.player.shot[idx].active = 1;
		game.player.shot[idx].shared.y = (u16) game.player.shared.y / 8 * 8;
		if (!(game.player.flags & PRONE))
		{
			game.player.shot[idx].shared.y += 8;
		}
		if (game.player.direction == D_RIGHT)
		{
			game.player.shot[idx].shared.x = game.player.shared.x+2*8;
			game.player.shot[idx].shared.vx = SHOT_SPEED;
		}
		else
		{
			game.player.shot[idx].shared.vx = -SHOT_SPEED;
			game.player.shot[idx].shared.x = game.player.shared.x-1*8;
		}
		game.player.active_shots++;
	}
}

void animate_shot()
{
	// Animate shots
	u8 slot = ENEMY_SLOT;
	for (u8 i = 0; i < MAX_PLAYER_SHOTS; i++)
	{
		if (game.player.shot[i].active)
		{
			game.player.shot[i].shared.x += game.player.shot[i].shared.vx*FRAME_TIME;
			game.player.shot[i].shared.y += game.player.shot[i].shared.vy*FRAME_TIME;
			LBMoveSprite(PLAYER_SHOT_SLOT+i, game.player.shot[i].shared.x - game.camera_x, game.player.shot[i].shared.y - game.camera_y, 1, 1);
			if (game.player.shot[i].shared.x < game.camera_x || game.player.shot[i].shared.x+8 > game.camera_x + CAMERA_WIDTH*8 ||
			    collision_detect_level(&game.player.shot[i].shared, 1, 1))
			{
				game.player.shot[i].active = 0;
				game.player.active_shots--;
				LBMoveSprite(PLAYER_SHOT_SLOT+i, OFF_SCREEN, 0, 1, 1);
			}
			else
			{
				slot = ENEMY_SLOT;
				for (u8 j = 0; j < MAX_ENEMIES; j++)
				{
					if (game.enemies[j].active &&
					    LBCollides(game.player.shot[i].shared.x,game.player.shot[i].shared.y, 8, 8,
							game.enemies[j].shared.x, game.enemies[j].shared.y, game.enemies[j].width*8, game.enemies[j].height*8
						)
					)
					{
						SFX_HIT;
						game.enemies[j].shield -= game.player.shot[i].hit_count;
						if (game.enemies[j].shield <= 0)
						{
							SFX_ENEMY_EXPLODE;
							game.enemies[j].flags = EXPLODING;
							game.enemies[j].active = 0;
							game.active_enemies--;
							game.score += KILL_SCORE;
							break;
						}
						game.player.shot[i].active = 0;
						game.player.active_shots--;
						LBMoveSprite(PLAYER_SHOT_SLOT+i, OFF_SCREEN, 0, 1, 1);
					}
					slot += game.enemies[j].width*game.enemies[j].height;
				}
			}
		}
	}
}

void handle_player_death()
{
	game.lives--;
	clear_sprites(0, 6);
	LBRotateSprites();
	if (game.lives == 0)
	{
		exit_game();
		return;
	}
	FadeOut(FRAMES_PER_FADE, true);
	ClearVram();
	clear_sprites(0, MAX_EXTENDED_SPRITES);
	LBRotateSprites();
	init_player_state();
	init_enemy_state();
	FadeIn(FRAMES_PER_FADE, false);
	load_level(game.current_level);
}

void update_player()
{
	
	if (game.player.flags & EXPLODING)
	{
		if (map_explosion(&game.player.flags, &game.player.expl, PLAYER_SLOT, game.player.width, game.player.height))
		{
			handle_player_death();
		}
	}
	else if (game.player.flags & (IDLE|RUNNING))
	{
		if (!on_solid_ground(&game.player.shared, game.player.width, game.player.height))
		{
			game.player.shared.gravity = GRAVITY;
			game.player.flags = JUMPING;
			LBMapSprite(PLAYER_SLOT, LBGetNextFrame(&game.player.jump), extendedSprites[PLAYER_SLOT].flags);
		}
		else
		{
			if (game.joypadState.held & BTN_DOWN)
			{
				game.player.shared.y += 16;
				if(game.player.direction == D_RIGHT)
				{
					game.player.shared.x -= 8;
				}
				game.player.shared.vx = 0;
				game.player.flags = PRONE;
				game.player.width = 3;
				game.player.height = 1;
				clear_sprites(3, 3);
				LBMapSprite(PLAYER_SLOT, LBGetNextFrame(&game.player.prone), extendedSprites[PLAYER_SLOT].flags);
			}
			else if ((game.joypadState.held & BTN_RIGHT) && (game.player.shared.x/8 + 2 < LEVEL_WIDTH))
			{
				game.player.shared.vx = RUN_SPEED;
				game.player.flags = RUNNING;
				game.player.direction = D_RIGHT;
				LBMapSprite(PLAYER_SLOT, LBGetNextFrame(&game.player.run), 0);
			}
			else if ((game.joypadState.held & BTN_LEFT) && (game.player.shared.x > game.camera_x))
			{
				game.player.shared.vx = -RUN_SPEED;
				game.player.flags = RUNNING;
				game.player.direction = D_LEFT;
				LBMapSprite(PLAYER_SLOT, LBGetNextFrame(&game.player.run), SPRITE_FLIP_X);
			}
			else
			{
				game.player.flags = IDLE;
				game.player.shared.vx = 0;
				LBMapSprite(PLAYER_SLOT, LBGetNextFrame(&game.player.idle), extendedSprites[PLAYER_SLOT].flags);
			}
			if (game.joypadState.pressed & BTN_B && !(game.player.flags & PRONE))
			{
				game.player.shared.vy = -JUMP_SPEED;
				game.player.shared.gravity = GRAVITY;
				game.player.flags = JUMPING;
				game.player.width = 2;
				game.player.height = 3;
				LBMapSprite(PLAYER_SLOT, LBGetNextFrame(&game.player.jump), extendedSprites[PLAYER_SLOT].flags);
			}
		}
	}
	else if (game.player.flags & JUMPING)
	{
		if ((game.joypadState.held & BTN_RIGHT) && (game.player.shared.x/8 + 2 < LEVEL_WIDTH))
		{
			game.player.shared.vx = RUN_SPEED;
			game.player.direction = D_RIGHT;
			LBMapSprite(PLAYER_SLOT, LBGetNextFrame(&game.player.jump), 0);
		}
		else if ((game.joypadState.held & BTN_LEFT) && (game.player.shared.x > game.camera_x))
		{
			game.player.shared.vx = -RUN_SPEED;
			game.player.direction = D_LEFT;
			LBMapSprite(PLAYER_SLOT, LBGetNextFrame(&game.player.jump), SPRITE_FLIP_X);
		}
		else
		{
			game.player.shared.vx = 0;
			LBMapSprite(PLAYER_SLOT, LBGetNextFrame(&game.player.jump), extendedSprites[PLAYER_SLOT].flags);
		}
		
		if (game.player.shared.gravity == 0)
		{
			game.player.flags = IDLE;
		}
	}
	else if (game.player.flags & PRONE)
	{
		if (game.joypadState.pressed & BTN_UP)
		{
			game.player.flags = IDLE;	
			game.player.shared.y -= 16;
			if(game.player.direction == D_RIGHT)
			{
				game.player.shared.x += 8;
			}
			game.player.width = 2;
			game.player.height = 3;
		}
		else if (game.joypadState.pressed & BTN_B)
		{
			game.player.flags = JUMPING;
			game.player.shared.y -= 16;
			if(game.player.direction == D_RIGHT)
			{
				game.player.shared.x += 8;
			}
			game.player.shared.vy = -JUMP_SPEED;
			game.player.shared.gravity = GRAVITY;
			game.player.width = 2;
			game.player.height = 3;
			LBMapSprite(PLAYER_SLOT, LBGetNextFrame(&game.player.jump), extendedSprites[PLAYER_SLOT].flags);
		}
		else if (game.joypadState.pressed & BTN_LEFT)
		{
			game.player.direction = D_LEFT;
			LBMapSprite(PLAYER_SLOT, LBGetNextFrame(&game.player.prone), SPRITE_FLIP_X);
		}
		else if (game.joypadState.pressed & BTN_RIGHT)
		{
			game.player.direction = D_RIGHT;
			LBMapSprite(PLAYER_SLOT, LBGetNextFrame(&game.player.prone), 0);
		}
	}
}

u8 pixel_overlap(u16 s1, u16 s2, u8 w1, u8 w2)
{
	u8 result = w1;
	
	if (s2 > s1)
	{
		result -= s2 - s1;
	}
	s1 = s1 + w1;
	s2 = s2 + w2;
	if (s2 < s1)
	{
		result -= s1 - s2;
	}
	return result;
}

u8 collision_detect_level(SpriteShared* s, u8 tile_width, u8 tile_height)
{
	u16 x0_tile = s->x / 8;
	u16 y0_tile = s->y / 8;
	u16 x1_tile = (s->x+7) / 8;
	u16 y1_tile = (s->y+7) / 8;
	u8 lt1, lt2;
	u8 result = 0;
	
	// Only if moving
	if (s->vx == 0 && s->vy == 0) return result;
	
	// Top and Bottom
	if (s->vy != 0)
	{
		for (u8 x = 0; x < tile_width; x++)
		{
			if (s->vy < 0)
			{
				lt1 = get_level_tile(game.current_level, x0_tile + x, y0_tile);
				lt2 = get_level_tile(game.current_level, x1_tile + x, y0_tile);
				if ((solid_tile(lt1) && pixel_overlap(s->x, x0_tile * 8, 8, 8) >= OVERLAP_THRESHOLD) ||
					(solid_tile(lt2) && pixel_overlap(s->x, x1_tile * 8, 8, 8) >= OVERLAP_THRESHOLD))
				{
					s->vy =  0;
					s->y = (y0_tile + 1) * 8;
					result = 1;
				}
			}
			if (s->vy > 0)
			{
				lt1 = get_level_tile(game.current_level, x0_tile + x, (s->y + tile_height*8 - 1) / 8);
				lt2 = get_level_tile(game.current_level, x1_tile + x, (s->y + tile_height*8 - 1) / 8);
				if ((solid_tile(lt1) && pixel_overlap(s->x, x0_tile * 8, 8, 8) >= OVERLAP_THRESHOLD) || 
					(solid_tile(lt2) && pixel_overlap(s->x, x1_tile * 8, 8, 8) >= OVERLAP_THRESHOLD))
				{
					s->vy =  0;
					s->gravity = 0;
					s->y = y0_tile * 8;
					result = 1;
				}
			}
		}
	}
	
	// Left and Right
	if (s->vx != 0)
	{
		y0_tile = s->y / 8;
		y1_tile = (s->y+7) / 8;
		for (u8 y = 0; y < tile_height; y++)
		{
			if (s->vx < 0)
			{
				lt1 = get_level_tile(game.current_level, x0_tile, y0_tile + y);
				lt2 = get_level_tile(game.current_level, x0_tile, y1_tile + y);
				if ((solid_tile(lt1) && pixel_overlap(s->y, y0_tile * 8, 8, 8) >= OVERLAP_THRESHOLD) ||
					(solid_tile(lt2) && pixel_overlap(s->y, y1_tile * 8, 8, 8) >= OVERLAP_THRESHOLD))
				{
					s->vx =  0;
					s->x = (x0_tile + 1) * 8;
					result = 1;
				}
			}
			
			if (s->vx > 0)
			{
				lt1 = get_level_tile(game.current_level, (s->x + tile_width * 8 - 1) / 8, y0_tile + y);
				lt2 = get_level_tile(game.current_level, (s->x + tile_width * 8 - 1) / 8, y1_tile + y);
				if ((solid_tile(lt1) && pixel_overlap(s->y, y0_tile * 8, 8, 8) >= OVERLAP_THRESHOLD) ||
					(solid_tile(lt2) && pixel_overlap(s->y, y1_tile * 8, 8, 8) >= OVERLAP_THRESHOLD))
				{
					s->vx =  0;
					s->x = x0_tile * 8;
					result = 1;
				}
			}
		}
	}
	return result;
}

u8 map_explosion(u8* flags, Animation* anim, u8 slot, u8 width, u8 height)
{
	char* frame;
	frame = LBGetNextFrame(anim);
	for (u8 i = 0; i < width*height; i++)
	{
		LBMapSprite(slot+i, frame, 0);
	}
	if (anim->looped)
	{
		*flags ^= EXPLODING;
		LBMoveSprite(slot, OFF_SCREEN, 0, width, height);
	}
	return anim->looped;
}

void animate_sprite(SpriteShared* s, u8 slot, u8 width, u8 height)
{
	s->vy = s->vy + s->gravity*FRAME_TIME;
	if (s->vy > MAX_SPEED) s->vy = MAX_SPEED;
	s->x += s->vx*FRAME_TIME;
	s->y += s->vy*FRAME_TIME;
	collision_detect_level(s, width, height);
	LBMoveSprite(slot, s->x - game.camera_x, s->y - game.camera_y, width, height);
}

void animate_player()
{
	if (game.player.flags & EXPLODING)
	{
		LBMoveSprite(PLAYER_SLOT, game.player.shared.x - game.camera_x, game.player.shared.y - game.camera_y, game.player.width, game.player.height);
	}
	else animate_sprite(&game.player.shared, PLAYER_SLOT, game.player.width, game.player.height);
}

void update_spider_enemy(Enemy* e, u8 slot)
{
	if (e->flags & IDLE && e->frame_count == SPIDER_IDLE_FRAMES)
	{
		e->flags = JUMPING;
		e->shared.vy = -SPIDER_JUMP_SPEED;
		e->shared.gravity = GRAVITY;
		e->shared.vx = SPIDER_SIDE_SPEED;
		if (e->direction == D_LEFT)
		{
			e->shared.vx = -SPIDER_SIDE_SPEED;
		}
		LBMapSprite(slot, map_enemy_spider_1, 0);
	}
	else if (e->flags & JUMPING && e->shared.gravity == 0)
	{
		e->flags = IDLE;
		e->shared.vx = 0;
		e->frame_count = 0;
		if (e->direction == D_LEFT)
		{
			e->direction = D_RIGHT;
		}
		else
		{
			e->direction = D_LEFT;
		}
	}
	else if (e->flags & IDLE)
	{
		LBMapSprite(slot, LBGetNextFrame(&e->anim), 0);
	}
	e->frame_count++;
}

void update_basic_shot(Shot* s, u8 slot)
{
	LBMapSprite(slot, LBGetNextFrame(&s->anim), 0);
}

void update_enemies()
{
	u8 slot = ENEMY_SLOT;
	
	for (u8 i = 0; i < MAX_ENEMIES; i++)
	{
		if (game.enemies[i].flags & EXPLODING)
		{
			map_explosion(&game.enemies[i].flags, &game.enemies[i].expl, slot, game.enemies[i].width, game.enemies[i].height);
		}
		else if (game.enemies[i].active)
		{
			switch (game.enemies[i].enemy_type)
			{
				case ENEMY_SPIDER: update_spider_enemy(&game.enemies[i], slot); break;
			}
		}
		slot += game.enemies[i].width * game.enemies[i].height;
	}
}

void update_enemy_shots()
{
	u8 slot = ENEMY_SHOT_SLOT;
	
	for (u8 i = 0; i < MAX_ENEMIES; i++)
	{
		if (game.enemies[i].active && game.enemies[i].shot_frame_count >= ENEMY_SHOT_DELAY_FRAMES)
		{
			game.enemies[i].shot_frame_count = 0;	
			for (u8 j = 0; j < MAX_ENEMY_SHOTS; j++)
			{
				if (game.enemies[i].shot[j].shot_type == BASIC_ENEMY_SHOT)
				{
					if (!game.enemies[i].shot[j].active)
					{
						game.enemies[i].shot[j].active = 1;
						game.enemies[i].shot[j].shared.x = game.enemies[i].shared.x;
						game.enemies[i].shot[j].shared.y = ((u16 ) game.enemies[i].shared.y / 8) * 8;
						game.enemies[i].shot[j].shared.vx = ENEMY_SHOT_SPEED;
						if (game.player.shared.x <= game.enemies[i].shared.x)
						{
							game.enemies[i].shot[j].shared.vx = -ENEMY_SHOT_SPEED;
						}
					}
					update_basic_shot(&game.enemies[i].shot[j], slot);
				}
				slot += 1;
			}
		}
		else if (game.enemies[i].active)
		{
			game.enemies[i].shot_frame_count++;
			slot += MAX_ENEMY_SHOTS;
		}
		else
		{
			slot += MAX_ENEMY_SHOTS;
		}
	}
}

void animate_enemies()
{
	u8 slot = ENEMY_SLOT;
	
	for (u8 i = 0; i < MAX_ENEMIES; i++)
	{
		if (game.enemies[i].active && !(game.enemies[i].flags & EXPLODING))
		{
			if (game.enemies[i].shared.x < game.camera_x || game.enemies[i].shared.y < game.camera_y || game.enemies[i].shared.y > game.camera_y + CAMERA_HEIGHT*8)
			{
				game.enemies[i].active = 0;
				game.active_enemies--;
				LBMoveSprite(slot, OFF_SCREEN, 0, game.enemies[i].width, game.enemies[i].height);
			}
			else if (LBCollides(game.player.shared.x,game.player.shared.y, game.player.width*8, game.player.height*8,
							game.enemies[i].shared.x, game.enemies[i].shared.y, 8, 8
						) && !(game.player.flags & EXPLODING)
			)
			{
				SFX_PLAYER_EXPLODE;
				game.player.shield = 0;
				game.player.flags = EXPLODING;
				return;
			}
			else
			{
				animate_sprite(&game.enemies[i].shared, slot, game.enemies[i].width, game.enemies[i].height);
			}
		}
		else if (game.enemies[i].flags & EXPLODING)
		{
			LBMoveSprite(slot, game.enemies[i].shared.x - game.camera_x, game.enemies[i].shared.y - game.camera_y, game.enemies[i].width, game.enemies[i].height);
		}
		slot +=  game.enemies[i].width * game.enemies[i].height;
	}
}

void animate_enemy_shots()
{
	u8 slot = ENEMY_SHOT_SLOT;
	
	for (u8 i = 0; i < MAX_ENEMIES; i++)
	{
		for (u8 j = 0; j < MAX_ENEMY_SHOTS; j++)
		{
			if (game.enemies[i].shot[j].shot_type == BASIC_ENEMY_SHOT)
			{
				if (game.enemies[i].shot[j].active)
				{
					if (game.enemies[i].shot[j].shared.x < game.camera_x || game.enemies[i].shot[j].shared.x > game.camera_x + CAMERA_WIDTH*8 ||
					   collision_detect_level(&game.enemies[i].shot[j].shared, 1, 1))
					{
						game.enemies[i].shot[j].active = 0;
						LBMoveSprite(slot, OFF_SCREEN, 0, 1, 1);
					}
					else if (LBCollides(game.player.shared.x,game.player.shared.y, game.player.width*8, game.player.height*8,
							game.enemies[i].shot[j].shared.x, game.enemies[i].shot[j].shared.y, 8, 8
						)
					)
					{
						SFX_HIT;
						game.player.shield -= game.enemies[i].shot[j].hit_count;
						if (game.player.shield <= 0)
						{
							SFX_PLAYER_EXPLODE;
							game.player.flags = EXPLODING;
						}
						game.enemies[i].shot[j].active = 0;
						LBMoveSprite(slot, OFF_SCREEN, 0, 1, 1);
					}
					else
					{
						game.enemies[i].shot[j].shared.x += game.enemies[i].shot[j].shared.vx*FRAME_TIME;
						LBMoveSprite(slot, game.enemies[i].shot[j].shared.x - game.camera_x, game.enemies[i].shot[j].shared.y - game.camera_y, 1, 1);
					}
				}
			}
			slot += 1;
		}
	}
}

void update_level()
{
	game.frame_counter++;
	if (game.frame_counter == 60)
	{
		game.frame_counter = 0;
		if (game.time != 65535) game.time++;
	}
	
	if (game.player.shared.vx > 0 &&
	    (game.player.shared.x - game.camera_x) >= (CAMERA_WIDTH / 2 * 8) &&
		(game.camera_x/8 + CAMERA_WIDTH < LEVEL_WIDTH))
	{
		move_camera_x();
	}
	
	if (game.player.shared.vy > 0 &&
	    (game.player.shared.y - game.camera_y + 24) >= ((CAMERA_HEIGHT - 2) * 8) &&
		(game.camera_y/8 + CAMERA_HEIGHT < LEVEL_HEIGHT))
	{
		move_camera_y();
	}
	
	// Score
	PrintByte(9, VRAM_TILES_V-1, game.lives ,true);
	PrintInt(27, VRAM_TILES_V-1, game.score ,true);
	PrintByte(9, VRAM_TILES_V-2, game.player.shield ,true);
	PrintInt(27, VRAM_TILES_V-2, game.time ,true);
	
}

void clear_sprites(u8 from, u8 count)
{
	for(u8 i = from; i < from+count; i++)
	{
		LBMapSprite(i, map_none, 0);
		LBMoveSprite(i, OFF_SCREEN, 0, 1, 1);
	}
}

void save_score()
{
	save_eeprom(&scores);
}

void exit_game()
{
    save_score();
	fade_through();
	clear_sprites(0, MAX_EXTENDED_SPRITES);
	LBRotateSprites();
	init_game_state();
	init_player_state();
	init_enemy_state();
	StopSong();
	load_splash();
}

void load_splash()
{
	Screen.scrollX = 0;
	Screen.scrollY = 0;
	Screen.scrollHeight = 32;
	Screen.overlayHeight = 0;
	game.current_screen = SPLASH;
	game.selection = START_SELECTED;
	clear_sprites(0, MAX_EXTENDED_SPRITES);
	LBRotateSprites();
	Print(8, 15, (char*) str1Player);
	Print(8, 16, (char*) strHighscores);
	Print(6, 21, (char*) strSelectHandle);
	Print(4, 26, (char*) strCopyright);
	DrawMap2(6, 5, (const char*) map_splash);
	LBMapSprite(0, map_right_arrow, 0);
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
		SFX_NAVIGATE;
	}
	else if (game.joypadState.pressed & BTN_UP && game.selection == HIGH_SCORES_SELECTED)
	{
		game.selection = START_SELECTED;
		SFX_NAVIGATE;
	}
	else if (select_pressed(&game.joypadState) && game.selection == START_SELECTED)
	{
		game.current_screen = LEVEL;
		SFX_NAVIGATE;
		level_transition(0);
		return;
	}
	
	if (game.selection == START_SELECTED)
	{
		LBMoveSprite(0, 7*8, 15*8, 1, 1);
	}
	else
	{
		LBMoveSprite(0, 7*8, 16*8, 1, 1);
	}
}

int main()
{
	// Initialize
	InitMusicPlayer(my_patches);
	ClearVram();
	SetMasterVolume(0x05);
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
				LBRotateSprites();
				break;
			case LEVEL:
				update_level();
				update_player();
				update_shot();
				update_enemies();
				update_enemy_shots();
				animate_player();
				animate_shot();
				animate_enemies();
				animate_enemy_shots();
				LBRotateSprites();
				break;
			default:
				break;
		}
	}
}
