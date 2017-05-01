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
#include "data/patches.h"
#include "data/planetmusic.h"
#include "data/spacemusic.h"
#include "types.h"
#include "strings.h"
#include "utils.h"
#include "macros.h"

// Function Prototypes
void load_eeprom(struct EepromBlockStruct* block);
void save_eeprom(struct EepromBlockStruct* block);
void fade_through();
void clear_sprites(u8 from, u8 count);
void load_splash();
void load_high_scores();
void exit_game();
void tally_score(char* title, u16 bonus);
u8 handle_player_death(Player* player);
void planet_transition(u8 index, char scroll, char atmosphere_height, char dy, u8 oy);
void challenge();
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
char* shark_anim[1];
char* enemy_shot_anim[1];
char* expl_anim[3];
char* boss_idle_anim[1];
char* boss_jump_anim[1];
char* boss_run_anim[4];
char* boss_shot_anim[1];

u8 is_space()
{
	return (game.current_level_index % 2 != 0);
}

void init_default_high_scores()
{
	scores.id = EEPROM_SCORES_ID;
	for (u8 i = 0; i < 30; i++)
	{
		scores.data[i] = pgm_read_byte(&default_scores[i]);
	}
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

void init_boss_state()
{
	LBResetJoyPadState(&game.boss.controls);
	if (is_space())
	{
		game.boss.width = 3;
		game.boss.height = 1;
		game.boss.run.anim_count = 2;
		game.boss.run.frames_per_anim = EXPLOSION_FRAME_COUNT;
		game.boss.run.anims = boss_run_anim;
		game.boss.run.anims[0] = (char*) map_hero_ship_0;
		game.boss.run.anims[1] = (char*) map_hero_ship_1;
	}
	else
	{
		game.boss.width = 2;
		game.boss.height = 3;
		game.boss.run.anim_count = 4;
		game.boss.run.frames_per_anim = FRAMES_PER_RUN_CYCLE;
		game.boss.run.anims = boss_run_anim;
		game.boss.run.anims[0] = (char*) map_ahero_step_0;
		game.boss.run.anims[1] = (char*) map_ahero_step_1;
		game.boss.run.anims[2] = (char*) map_ahero_step_2;
		game.boss.run.anims[3] = (char*) map_ahero_step_3;
	}
	game.boss.active = 0;
	game.boss.direction = D_RIGHT;
	game.boss.flags = IDLE;
	game.boss.ai_flags = AI_NOT_READY;
	game.boss.shield = BOSS_SHIELD;
	game.boss.idle.anim_count = 1;
	game.boss.idle.frames_per_anim = 1;
	game.boss.idle.anims = boss_idle_anim;
	game.boss.idle.anims[0] = (char*) map_ahero_idle;
	
	game.boss.jump.anim_count = 1;
	game.boss.jump.frames_per_anim = 1;
	game.boss.jump.anims = boss_jump_anim;
	game.boss.jump.anims[0] = (char*) map_ahero_jump;
	
	game.boss.expl.anim_count = 3;
	game.boss.expl.frames_per_anim = EXPLOSION_FRAME_COUNT;
	game.boss.expl.anims = expl_anim;
	game.boss.expl.anims[0] = (char*) map_explosion_0;
	game.boss.expl.anims[1] = (char*) map_explosion_1;
	game.boss.expl.anims[2] = (char*) map_explosion_2;
	
	for (u8 i = 0; i < MAX_PLAYER_SHOTS; i++)
	{
		game.boss.shot[i].hit_count = 1;
		game.boss.shot[i].shot_type = BASIC_SHOT;
		game.boss.shot[i].anim.anim_count = 1;
		game.boss.shot[i].anim.frames_per_anim = 1;
		game.boss.shot[i].anim.anims = boss_shot_anim;
		game.boss.shot[i].anim.anims[0] = (char*) map_ahero_shot;
	}
}

void init_player_state()
{
	if (is_space())
	{
		game.player.width = 3;
		game.player.height = 1;
		game.player.run.anim_count = 2;
		game.player.run.frames_per_anim = EXPLOSION_FRAME_COUNT;
		game.player.run.anims = run_anim;
		game.player.run.anims[0] = (char*) map_hero_ship_0;
		game.player.run.anims[1] = (char*) map_hero_ship_1;
	}
	else
	{
		game.player.width = 2;
		game.player.height = 3;
		game.player.run.anim_count = 4;
		game.player.run.frames_per_anim = FRAMES_PER_RUN_CYCLE;
		game.player.run.anims = run_anim;
		game.player.run.anims[0] = (char*) map_hero_step_0;
		game.player.run.anims[1] = (char*) map_hero_step_1;
		game.player.run.anims[2] = (char*) map_hero_step_2;
		game.player.run.anims[3] = (char*) map_hero_step_3;
	}
	game.player.active = 1;
	game.player.direction = D_RIGHT;
	game.player.flags = IDLE;
	game.player.ai_flags = AI_NOT_READY;
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

void init_enemy_shot(u8 i, u16 x, u16 y)
{
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
	
	init_enemy_shot(i, x, y);
}

void init_enemy_shark(u8 i, u16 x, u16 y)
{
	game.enemies[i].active = 1;
	game.enemies[i].width = 1;
	game.enemies[i].height = 1;
	game.enemies[i].enemy_type = ENEMY_SHARK;
	game.enemies[i].frame_count = 0;
	game.enemies[i].shot_frame_count = 0;
	game.enemies[i].shield = ENEMY_SHARK_SHIELD;
	
	game.enemies[i].anim.anim_count = 1;
	game.enemies[i].anim.frames_per_anim = 1;
	game.enemies[i].anim.anims = shark_anim;
	game.enemies[i].anim.anims[0] = (char*) map_enemy_shark;
	game.enemies[i].shared.gravity = 0;
	game.enemies[i].shared.vx = SHARK_SPEED;
	game.enemies[i].shared.vy = 0;
	game.enemies[i].shared.x = x;
	game.enemies[i].shared.y = y;
	
	init_enemy_shot(i, x, y);
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
	clear_sprites(0, MAX_EXTENDED_SPRITES);
	LBRotateSprites();
	ClearVram();
	FadeIn(FRAMES_PER_FADE, false);
}

void render_level_tile(u8 level_tile, u8 x, u8 y)
{
	SetTile(x, y, level_tile);
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

u8 get_camera_x(u8 level_index)
{
	return 0;
}

u8 get_camera_y(u8 level_index)
{
	return 0;
}

u8 get_hero_spawn_x(u8 level_index)
{
	return 3;
}

u8 get_hero_spawn_y(u8 level_index)
{
	return 21;
}

u8 get_boss_spawn_x(u8 level_index)
{
	return 251;
}

u8 get_boss_spawn_y(u8 level_index)
{
	return 21;
}

u8 get_level_tile(char* level_data, u8 x, u8 y)
{
	u16 mega_tile_index = 0;
	u16 mega_tile_offset = 0;
	u8 map_x = 0;
	u8 map_y = 0;
	
	map_x = x / MAP_LEVEL_MEGA_TILES_MEGA_TILE_WIDTH;
	map_y = y / MAP_LEVEL_MEGA_TILES_MEGA_TILE_HEIGHT;
	mega_tile_offset = (y % MAP_LEVEL_MEGA_TILES_MEGA_TILE_HEIGHT)*MAP_LEVEL_MEGA_TILES_MEGA_TILE_WIDTH + (x % MAP_LEVEL_MEGA_TILES_MEGA_TILE_WIDTH);
	mega_tile_index = pgm_read_byte(2+(&level_data[map_y*game.raw_level_width + map_x]));
	mega_tile_index *= MAP_LEVEL_MEGA_TILES_MEGA_TILE_WIDTH*MAP_LEVEL_MEGA_TILES_MEGA_TILE_HEIGHT;
	return pgm_read_byte(&map_level_mega_tiles[mega_tile_index+mega_tile_offset]);
}

void render_camera_view()
{
	u8 level_tile;
	u8 camara_x_tile = game.camera_x/8;
	u8 camera_y_tile = game.camera_y/8;
	
	for (u16 x = camara_x_tile; x <= camara_x_tile + CAMERA_WIDTH; x++)
	{
		for (u16 y = camera_y_tile; y <= camera_y_tile + CAMERA_HEIGHT; y++)
		{
			level_tile = get_level_tile(game.current_level, x, y);
			render_level_tile(level_tile, (Screen.scrollX / 8 + x - camara_x_tile) % 32, (Screen.scrollY / 8 + y - camera_y_tile) % 30);
		}
	}
}

void spawn_enemy(u16 x, u16 y)
{
	if (game.camera_x < ENEMY_SPAWN_CUTOFF)
	{
		for (u8 i = 0; i < MAX_ENEMIES; i++)
		{
			if (!game.enemies[i].active)
			{
				for (u8 j = 0; j < MAX_ENEMY_SHOTS; j++)
				{
					if (!game.enemies[i].shot[j].active)
					{
						if (is_space())
						{
							init_enemy_shark(i, x, y);
						}
						else
						{
							init_enemy_spider(i, x, y);
						}
						game.active_enemies++;
						return;
					}
				}
			}
		}
	}
}

u8 solid_tile(u8 level_tile)
{
	if (level_tile >= 1 && level_tile <= 24)
	{
		return 1;
	}
	return 0;
}

u8 level_hazard(u8 level_tile)
{
	switch (level_tile)
	{
		case L_DESERT_SPIKE_DOWN : return 1;
		case L_DESERT_SPIKE_UP : return 1;
		case L_ICICLE_DOWN : return 1;
		case L_ICICLE_UP : return 1;
		case L_JUNGLE_FLOWER : return 1;
		case L_FIRE_LAVA : return 1;
		case L_JUNGLE_WATER : return 1;
	}
	return 0;
}

void append_tile_column()
{
	u8 enemy_spawned = 0;
	u8 level_tile;
	u8 rndom = LBRandom(0, CAMERA_HEIGHT-1)*8;

	game.column_count++;
    for (u8 y = 0; y <= CAMERA_HEIGHT; y++)
	{
		level_tile = get_level_tile(game.current_level, game.scroll_src_x, y + game.camera_y / 8);
		render_level_tile(level_tile, game.scroll_dest_x, (y + Screen.scrollY / 8) % 30);
		if (game.column_count % game.spawn_rate == game.spawn_rate - 1 && game.active_enemies < MAX_ENEMIES && !enemy_spawned)
		{
			if (is_space())
			{
				spawn_enemy(game.camera_x+CAMERA_WIDTH*8, rndom);
				enemy_spawned = 1;
			}
			else if (solid_tile(level_tile))
			{
				spawn_enemy(game.camera_x+CAMERA_WIDTH*8, (game.camera_y / 8 + y - 1)*8);
				enemy_spawned = 1;
			}
		}
    }
	
    game.scroll_src_x++;
	game.scroll_dest_x++;
    if(game.scroll_dest_x>=32)game.scroll_dest_x=0;
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

void map_ship()
{
	game.player.run.anim_count = 2;
	game.player.run.frames_per_anim = EXPLOSION_FRAME_COUNT;
	game.player.run.anims = run_anim;
	game.player.run.anims[0] = (char*) map_hero_ship_large_0;
	game.player.run.anims[1] = (char*) map_hero_ship_large_1;
}

void map_level_info(char* level)
{
	game.current_level = level;
	
	game.raw_level_width = pgm_read_byte(&level[0]);
	game.raw_level_height = pgm_read_byte(&level[1]);
	game.level_width = pgm_read_byte(&level[0])*MAP_LEVEL_MEGA_TILES_MEGA_TILE_WIDTH;
	game.level_height = pgm_read_byte(&level[1])*MAP_LEVEL_MEGA_TILES_MEGA_TILE_HEIGHT;
}

void load_level(u8 index, u8 drop_ship)
{	
	u8 ship_x = (get_hero_spawn_x(index) - 2) * 8;
	u8 ship_y = 0;
	char ship_increment = 3;
	
	Screen.scrollX = 0;
	Screen.scrollY = 0;
	Screen.scrollHeight = 29;
	Screen.overlayTileTable = tiles_data;
	Screen.overlayHeight = 3;
	clear_overlay(2);
	game.current_level_index = index;
	game.level_ended = 0;
	LBResetJoyPadState(&game.player.controls);
	LBResetJoyPadState(&game.boss.controls);
	switch (index)
	{
		case 0: map_level_info((char*) map_level_ice); break;
		case 1: map_level_info((char*) map_level_space); break;
		case 2: map_level_info((char*) map_level_fire); break;
		case 3: map_level_info((char*) map_level_space); break;
		case 4: map_level_info((char*) map_level_forest); break;
		case 5: map_level_info((char*) map_level_space); break;
		case 6: map_level_info((char*) map_level_city); break;
		case 7: map_level_info((char*) map_level_space); break;
		case 8: map_level_info((char*) map_level_desert); break;
		case 9: map_level_info((char*) map_level_space); break;
	}
	game.column_count = 0;
	game.camera_x = get_camera_x(index);
	game.camera_y = get_camera_y(index);
	game.player.shared.x = get_hero_spawn_x(index)*8;
	game.player.shared.y = get_hero_spawn_y(index)*8;
	game.boss.shared.x = get_boss_spawn_x(index)*8;
	game.boss.shared.y = get_boss_spawn_y(index)*8;
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
	LBPrint(0, VRAM_TILES_V-3, (char*) strShield);
	LBPrint(18, VRAM_TILES_V-3, (char*) strTime);
	LBPrint(0, VRAM_TILES_V-2, (char*) strLives);
	LBPrint(17, VRAM_TILES_V-2, (char*) strScore);
	LBPrint(17, VRAM_TILES_V-1, (char*) strTally);
	
	LBPrintByte(9, VRAM_TILES_V-3, game.player.shield ,true);
	LBPrintInt(27, VRAM_TILES_V-3, game.time ,true);
	LBPrintByte(9, VRAM_TILES_V-2, game.lives ,true);
	LBPrintInt(27, VRAM_TILES_V-2, game.level_score ,true);
	LBPrintInt(27, VRAM_TILES_V-1, game.score ,true);
	
	
	if (!is_space() && drop_ship)
	{
		map_ship();
		while (1)
		{
			WaitVsync(1);
			if (ship_y >= get_hero_spawn_y(index)*8)
			{
				LBMapSprite(0, map_hero_idle, 0);
				LBMoveSprite(0, get_hero_spawn_x(index)*8, get_hero_spawn_y(index)*8, 2, 3);
				ship_increment = -3;
			}
			LBMapSprite(6, LBGetNextFrame(&game.player.run), 0);
			LBMoveSprite(6, ship_x, ship_y, 4, 2);
			ship_y += ship_increment;
			if (ship_y == 0)
			{
				clear_sprites(6, 8);
				break;
			}
			LBRotateSprites();
		}
	}
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

u8 find_shot_slot(Player* player)
{
	for (u8 i = 0; i < MAX_PLAYER_SHOTS; i++)
	{
		if (!player->shot[i].active) return i;
	}
	return 0;
}

void update_shot(Player* player, u8 shot_slot)
{
	// Shot updates
	u8 idx;
	if (player->controls.pressed & BTN_A && player->active_shots < MAX_PLAYER_SHOTS)
	{
		SFX_PLAYER_SHOOT;
		idx = find_shot_slot(player);
		LBMapSprite(shot_slot+idx, LBGetNextFrame(&player->shot[idx].anim), 0);
		player->shot[idx].active = 1;
		player->shot[idx].shared.y = (u16) player->shared.y / 8 * 8;
		if (is_space())
		{
			player->shot[idx].shared.x = player->shared.x+player->width*8;
			player->shot[idx].shared.vx = SHOT_SPEED;
		}
		else
		{
			if (!(player->flags & PRONE))
			{
				player->shot[idx].shared.y += 8;
			}
			if (player->direction == D_RIGHT)
			{
				player->shot[idx].shared.x = player->shared.x+2*8;
				player->shot[idx].shared.vx = SHOT_SPEED;
			}
			else
			{
				player->shot[idx].shared.vx = -SHOT_SPEED;
				player->shot[idx].shared.x = player->shared.x-1*8;
			}
		}
		player->active_shots++;
	}
}

void animate_shot(Player* player, Player* other_player, u8 shot_slot)
{
	// Animate shots
	u8 slot = ENEMY_SLOT;
	for (u8 i = 0; i < MAX_PLAYER_SHOTS; i++)
	{
		if (player->shot[i].active)
		{
			player->shot[i].shared.x += player->shot[i].shared.vx*FRAME_TIME*2;
			player->shot[i].shared.y += player->shot[i].shared.vy*FRAME_TIME;
			LBMoveSprite(shot_slot+i, player->shot[i].shared.x - game.camera_x, player->shot[i].shared.y - game.camera_y, 1, 1);
			if (player->shot[i].shared.x < game.camera_x || player->shot[i].shared.x+8 > game.camera_x + CAMERA_WIDTH*8 ||
			    collision_detect_level(&player->shot[i].shared, 1, 1))
			{
				player->shot[i].active = 0;
				player->active_shots--;
				LBMoveSprite(shot_slot+i, OFF_SCREEN, 0, 1, 1);
			}
			else
			{
				slot = ENEMY_SLOT;
				for (u8 j = 0; j < MAX_ENEMIES; j++)
				{
					if (game.enemies[j].active &&
					    LBCollides(player->shot[i].shared.x-game.camera_x,player->shot[i].shared.y, 8, 8,
							game.enemies[j].shared.x-game.camera_x, game.enemies[j].shared.y, game.enemies[j].width*8, game.enemies[j].height*8
						)
					)
					{
						SFX_HIT;
						game.enemies[j].shield -= player->shot[i].hit_count;
						if (game.enemies[j].shield <= 0)
						{
							SFX_ENEMY_EXPLODE;
							game.enemies[j].flags = EXPLODING;
							game.enemies[j].active = 0;
							game.active_enemies--;
							game.level_score += KILL_SCORE;
							break;
						}
						player->shot[i].active = 0;
						player->active_shots--;
						LBMoveSprite(shot_slot+i, OFF_SCREEN, 0, 1, 1);
					}
					slot += game.enemies[j].width*game.enemies[j].height;
				}
				
				// Boss interaction
				if (!(other_player == &game.player && DEBUG_GODMODE) && other_player->active)
				{
					if (LBCollides(player->shot[i].shared.x-game.camera_x,player->shot[i].shared.y, 8, 8,
								other_player->shared.x-game.camera_x, other_player->shared.y, other_player->width*8, other_player->height*8
							)
						)
						{
							SFX_HIT;
							other_player->shield -= player->shot[i].hit_count;
							if (other_player->shield <= 0)
							{
								SFX_PLAYER_EXPLODE;
								other_player->shield = 0;
								other_player->flags = EXPLODING;
								if (other_player == &game.boss) game.level_score += BOSS_KILL_SCORE;
							}
							player->shot[i].active = 0;
							player->active_shots--;
							LBMoveSprite(shot_slot+i, OFF_SCREEN, 0, 1, 1);
						}
				}
			}
		}
	}
}

u8 handle_player_death(Player* player)
{
	if (player == &game.boss) {
		game.level_ended = 1;
		return 1;
	}
	game.lives--;
	clear_sprites(0, 6);
	LBRotateSprites();
	if (game.lives == 0)
	{
		tally_score((char*) strGameOver, 0);
		exit_game();
		return 0;
	}
	FadeOut(FRAMES_PER_FADE, true);
	ClearVram();
	clear_sprites(0, MAX_EXTENDED_SPRITES);
	LBRotateSprites();
	FadeIn(FRAMES_PER_FADE, false);
	init_player_state();
	init_boss_state();
	init_enemy_state();
	load_level(game.current_level_index, false);
	return 0;
}

u8 update_player(Player* player, u8 slot)
{
	
	if (player->flags & EXPLODING)
	{
		if (map_explosion(&player->flags, &player->expl, slot, player->width, player->height))
		{
			return handle_player_death(player);
		}
	}
	else if (is_space())
	{
		if (player->flags & END_OF_SPACE) {
			player->shared.vx = SPACE_SHIP_SPEED*2;
			player->shared.vy = 0;
		} else {
			player->shared.vy = 0;
			player->shared.vx = SPACE_SHIP_SPEED;
			
			if ((player->controls.held & BTN_DOWN) && (player->shared.y/8 + 1 < game.level_height))
			{
				player->shared.vy = SPACE_SHIP_SPEED;
			}
			if ((player->controls.held & BTN_RIGHT) && (player->shared.x + 24 < game.camera_x + CAMERA_WIDTH*8))
			{
				player->shared.vx = SPACE_SHIP_SPEED*2;
			}
			if ((player->controls.held & BTN_LEFT) && (player->shared.x > game.camera_x))
			{
				player->shared.vx = 0;
			}
			if ((player->controls.held & BTN_UP) && (player->shared.y > 0))
			{
				player->shared.vy = -SPACE_SHIP_SPEED;
			}
		}
		LBMapSprite(slot, LBGetNextFrame(&player->run), 0);
	}
	else if (player->flags & (IDLE|RUNNING))
	{
		if (!on_solid_ground(&player->shared, player->width, player->height))
		{
			player->shared.gravity = GRAVITY;
			player->flags = JUMPING;
			LBMapSprite(slot, LBGetNextFrame(&player->jump), extendedSprites[slot].flags);
		}
		else
		{
			if (player->controls.held & BTN_DOWN)
			{
				player->shared.y += 16;
				if(player->direction == D_RIGHT)
				{
					player->shared.x -= 8;
				}
				player->shared.vx = 0;
				player->flags = PRONE;
				player->width = 3;
				player->height = 1;
				clear_sprites(3, 3);
				LBMapSprite(slot, LBGetNextFrame(&player->prone), extendedSprites[slot].flags);
			}
			else if ((player->controls.held & BTN_RIGHT) && (player->shared.x/8 + 2 < game.level_width))
			{
				player->shared.vx = RUN_SPEED;
				player->flags = RUNNING;
				player->direction = D_RIGHT;
				LBMapSprite(slot, LBGetNextFrame(&player->run), 0);
			}
			else if ((player->controls.held & BTN_LEFT) && (player->shared.x > game.camera_x))
			{
				player->shared.vx = -RUN_SPEED;
				player->flags = RUNNING;
				player->direction = D_LEFT;
				LBMapSprite(slot, LBGetNextFrame(&player->run), SPRITE_FLIP_X);
			}
			else
			{
				player->flags = IDLE;
				player->shared.vx = 0;
				LBMapSprite(slot, LBGetNextFrame(&player->idle), extendedSprites[slot].flags);
			}
			if (player->controls.pressed & BTN_B && !(player->flags & PRONE))
			{
				player->shared.vy = -JUMP_SPEED;
				player->shared.gravity = GRAVITY;
				player->flags = JUMPING;
				player->width = 2;
				player->height = 3;
				LBMapSprite(slot, LBGetNextFrame(&player->jump), extendedSprites[slot].flags);
			}
		}
	}
	else if (player->flags & JUMPING)
	{
		if ((player->controls.held & BTN_RIGHT) && (player->shared.x/8 + 2 < game.level_width))
		{
			player->shared.vx = RUN_SPEED;
			player->direction = D_RIGHT;
			LBMapSprite(slot, LBGetNextFrame(&player->jump), 0);
		}
		else if ((player->controls.held & BTN_LEFT) && (player->shared.x > game.camera_x))
		{
			player->shared.vx = -RUN_SPEED;
			player->direction = D_LEFT;
			LBMapSprite(slot, LBGetNextFrame(&player->jump), SPRITE_FLIP_X);
		}
		else
		{
			player->shared.vx = 0;
			LBMapSprite(slot, LBGetNextFrame(&player->jump), extendedSprites[slot].flags);
		}
		
		if (player->shared.gravity == 0)
		{
			player->flags = IDLE;
		}
	}
	else if (player->flags & PRONE)
	{
		if (player->controls.pressed & BTN_UP)
		{
			player->flags = IDLE;	
			player->shared.y -= 16;
			if(player->direction == D_RIGHT)
			{
				player->shared.x += 8;
			}
			player->width = 2;
			player->height = 3;
		}
		else if (player->controls.pressed & BTN_B)
		{
			player->flags = JUMPING;
			player->shared.y -= 16;
			if(player->direction == D_RIGHT)
			{
				player->shared.x += 8;
			}
			player->shared.vy = -JUMP_SPEED;
			player->shared.gravity = GRAVITY;
			player->width = 2;
			player->height = 3;
			LBMapSprite(slot, LBGetNextFrame(&player->jump), extendedSprites[slot].flags);
		}
		else if (player->controls.pressed & BTN_LEFT)
		{
			player->direction = D_LEFT;
			LBMapSprite(slot, LBGetNextFrame(&player->prone), SPRITE_FLIP_X);
		}
		else if (player->controls.pressed & BTN_RIGHT)
		{
			player->direction = D_RIGHT;
			LBMapSprite(slot, LBGetNextFrame(&player->prone), 0);
		}
	}
	return 1;
}

u8 pixel_overlap(u8 s1, u8 s2, u8 w1, u8 w2)
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
	u8 x0_tile = s->x >> 3;
	u8 y0_tile = s->y >> 3;
	u8 x1_tile = (s->x+7) >> 3;
	u8 y1_tile = (s->y+7) >> 3;
	u8 lt1, lt2;
	u8 result = 0;
	
	// Top and Bottom
	if (s->vy != 0)
	{
		for (u8 x = 0; x < tile_width; x++)
		{
			if (s->vy < 0)
			{
				lt1 = get_level_tile(game.current_level, x0_tile + x, y0_tile);
				lt2 = get_level_tile(game.current_level, x1_tile + x, y0_tile);
				if ((solid_tile(lt1) && pixel_overlap(s->x-game.camera_x, x0_tile * 8, 8, 8) >= OVERLAP_THRESHOLD) ||
					(solid_tile(lt2) && pixel_overlap(s->x-game.camera_x, x1_tile * 8, 8, 8) >= OVERLAP_THRESHOLD))
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
				if ((solid_tile(lt1) && pixel_overlap(s->x-game.camera_x, x0_tile * 8, 8, 8) >= OVERLAP_THRESHOLD) || 
					(solid_tile(lt2) && pixel_overlap(s->x-game.camera_x, x1_tile * 8, 8, 8) >= OVERLAP_THRESHOLD))
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

void animate_player(Player* player, Player* other_player, u8 slot)
{
	if (player->flags & EXPLODING)
	{
		LBMoveSprite(slot, player->shared.x - game.camera_x, player->shared.y - game.camera_y, player->width, player->height);
	}
	else if (!DEBUG_GODMODE && other_player == &game.player && LBCollides(player->shared.x-game.camera_x,player->shared.y, player->width*8, player->height*8,
					other_player->shared.x-game.camera_x, other_player->shared.y, other_player->width*8, other_player->height*8
				) 
	)
	{
		SFX_PLAYER_EXPLODE;
		other_player->shield = 0;
		other_player->flags = EXPLODING;
	}
	else animate_sprite(&player->shared, slot, player->width, player->height);
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

void update_shark_enemy(Enemy* e, u8 slot)
{
	e->shared.vx = SHARK_SPEED;
	e->shared.vy = 0;
	LBMapSprite(slot, LBGetNextFrame(&e->anim), 0);
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
				case ENEMY_SHARK: update_shark_enemy(&game.enemies[i], slot); break;
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
#if DEBUG_GODMODE
#else
			else if (LBCollides(game.player.shared.x-game.camera_x,game.player.shared.y, game.player.width*8, game.player.height*8,
							game.enemies[i].shared.x-game.camera_x, game.enemies[i].shared.y, 8, 8
						) && !(game.player.flags & EXPLODING)
			)
			{
				SFX_PLAYER_EXPLODE;
				game.player.shield = 0;
				game.player.flags = EXPLODING;
				return;
			}
#endif
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
					else if (LBCollides(game.player.shared.x-game.camera_x,game.player.shared.y, game.player.width*8, game.player.height*8,
							game.enemies[i].shot[j].shared.x-game.camera_x, game.enemies[i].shot[j].shared.y, 8, 8
						)
					)
					{
						SFX_HIT;
#if DEBUG_GODMODE
#else
						game.player.shield -= game.enemies[i].shot[j].hit_count;
#endif
						if (game.player.shield <= 0)
						{
							SFX_PLAYER_EXPLODE;
							game.player.shield = 0;
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
	u8 ship_x = game.player.shared.x - game.camera_x;
	u8 ship_y = 0;
	char ship_increment = 3;
	
	game.frame_counter++;
	if (game.frame_counter == 60)
	{
		game.frame_counter = 0;
		if (game.time != 65535) game.time++;
	}
	
	if (is_space())
	{
		if (game.camera_x/8 + CAMERA_WIDTH < game.level_width)
		{
			move_camera_x();
		}
		else if (game.player.shared.x >= game.camera_x+CAMERA_WIDTH*8)
		{
			game.level_ended = 1;
			game.player.flags ^= END_OF_SPACE;
		} else {
			game.player.flags |= END_OF_SPACE;
		}
	}
	else if (game.player.shared.vx > 0 &&
	    (game.player.shared.x - game.camera_x) >= (CAMERA_WIDTH / 2 * 8) &&
		(game.camera_x/8 + CAMERA_WIDTH < game.level_width))
	{
		move_camera_x();
	}
	
	// Score
	LBPrintByte(9, VRAM_TILES_V-3, game.player.shield ,true);
	LBPrintInt(27, VRAM_TILES_V-3, game.time ,true);
	LBPrintByte(9, VRAM_TILES_V-2, game.lives ,true);
	LBPrintInt(27, VRAM_TILES_V-2, game.level_score ,true);
	LBPrintInt(27, VRAM_TILES_V-1, game.score ,true);
	
	if (game.level_ended && !is_space() && game.player.flags & (IDLE|RUNNING|PRONE))
	{
		LBMapSprite(PLAYER_SLOT, map_hero_idle, 0);
		if (game.player.flags & PRONE) {
			game.player.shared.y -= 16;
			game.player.width = 2;
			game.player.height = 3;
			LBMoveSprite(PLAYER_SLOT, game.player.shared.x-game.camera_x, game.player.shared.y, game.player.width, game.player.height);
		}
		clear_sprites(ENEMY_SLOT, 11);
		LBRotateSprites();
		map_ship();
		while (1)
		{
			WaitVsync(1);
			if (ship_y >= get_hero_spawn_y(game.current_level_index)*8)
			{
				clear_sprites(0, 6);
				LBRotateSprites();
				ship_increment = -3;
			}
			LBMapSprite(PLAYER_SHOT_SLOT, LBGetNextFrame(&game.player.run), 0);
			LBMoveSprite(PLAYER_SHOT_SLOT, ship_x, ship_y, 4, 2);
			ship_y += ship_increment;
			if (ship_y == 0)
			{
				clear_sprites(6, 8);
				LBRotateSprites();
				tally_score((char*) strLevelClear, 100);
				planet_transition(game.current_level_index+1, -1, 26, 3, 0);
				break;
			}
			LBRotateSprites();
		}
	}
	else if (game.level_ended && is_space())
	{
		tally_score((char*) strLevelClear, 100);
		if (game.current_level_index == 9) {
			exit_game();
		} else {
			planet_transition(game.current_level_index+1, 1, 28, -3, 199);
		}
	}
	
}

void clear_sprites(u8 from, u8 count)
{
	for(u8 i = from; i < from+count; i++)
	{
		LBMapSprite(i, map_none, 0);
		LBMoveSprite(i, OFF_SCREEN, 0, 1, 1);
	}
}

void hide_sprites(u8 from, u8 count)
{
	for(u8 i = from; i < from+count; i++)
	{
		LBMoveSprite(i, OFF_SCREEN, 0, 1, 1);
	}
}

void exit_game()
{
	clear_sprites(0, MAX_EXTENDED_SPRITES);
	LBRotateSprites();
	init_player_state();
	init_boss_state();
	init_enemy_state();
	StopSong();
	load_high_scores();
}

char select_pressed(JoyPadState* p)
{
	return (p->pressed & BTN_A) || (p->pressed & BTN_START);
}


void stream_text_middle(const char* dialogue, u8 y)
{
	u8 x, ln, c;
	
	while (pgm_read_byte(dialogue) != '#')
	{
		ln = strnlen_P(dialogue, 255);
		x = 14 - ln / 2;
		while ((c = pgm_read_byte(dialogue++)))
		{
			LBPrintChar(x++, y, c);
			LBGetJoyPadState(&game.player.controls, 0);
			if (!(game.player.controls.held & BTN_B)) WaitUs(CHARACTER_DELAY_US);
		}
		y++;
	}
}

void stream_text_anywhere(const char* dialogue, u8 x, u8 y)
{
	u8 ln, c;
	u8 cursor = x; 
	
	while (pgm_read_byte(dialogue) != '#')
	{
		ln = strnlen_P(dialogue, 255);
		while ((c = pgm_read_byte(dialogue++)))
		{
			LBPrintChar(cursor++, y, c);
			WaitUs(CHARACTER_DELAY_US);
		}
		y++;
		cursor = x;
	}
}

void map_particles()
{
	for (u8 i = 9; i < 20; i++)
	{
		LBMapSprite(i, map_particle, 0);
	}
}

void place_particles()
{
	for (u8 i = 9; i < 20; i++)
	{
		LBMoveSprite(i, LBRandom(0, 224), LBRandom(0, 200), 1, 1);
	}
}

void animate_particles(char dy, u8 oy)
{
	for (u8 i = 9; i < 20; i++)
	{
		LBMoveSprite(i, extendedSprites[i].x, extendedSprites[i].y+dy, 1, 1);
		if (extendedSprites[i].y > 200)
		{
			LBMoveSprite(i, LBRandom(0, 224), oy, 1, 1);
		}
	}
}

const char* get_emarald_map(u8 index)
{
	switch (index)
	{
		case 0: return map_emerald_0;
		case 2: return map_emerald_1;
		case 4: return map_emerald_2;
		case 6: return map_emerald_3;
		case 8: return map_emerald_4;
		default: return map_emerald_0;
	}
}

void music_transition()
{
	if (is_space())
	{
		StartSong(spacesong);
	}
	else if (game.current_level_index != 0)
	{
		StartSong(planetsong);
	}
}

void planet_transition(u8 index, char scroll, char atmosphere_height, char dy, u8 oy)
{
	u8 frame_counter = 0;
	
	fade_through();
	Screen.scrollX = 0;
	Screen.scrollY = 0;
	Screen.scrollHeight = 32;
	Screen.overlayHeight = 0;
	game.scroll_x = 0;
	game.scroll_y = 0;
	map_particles();
	map_ship();
	place_particles();
	DrawMap2(0, atmosphere_height, map_atmosphere);
	game.current_level_index = index;
	music_transition();
	if (!is_space())
	{
		LBMapSprite(8, get_emarald_map(index), 0);
		LBMoveSprite(8, 112, 48, 1, 1);
	}
	
	while (frame_counter < 255)
	{
		WaitVsync(1);
		LBMapSprite(0, LBGetNextFrame(&game.player.run), 0);
		LBMoveSprite(0, 104, 104, 4, 2);
		frame_counter++;
		if (frame_counter % 16 == 0) Screen.scrollY += scroll;
		animate_particles(dy, oy);
		LBRotateSprites();
	}
	game.current_screen = LEVEL;
	ClearVram();
	clear_sprites(0, MAX_EXTENDED_SPRITES);
	LBRotateSprites();
	fade_through();
	game.level_score = 0;
	load_level(index, true);
	init_player_state();
	init_boss_state();
	init_enemy_state();
}


void intro()
{
	fade_through();
	game.lives = LIVES;
	game.score = 0;
	game.level_score = 0;
	game.time = 0;
	StartSong(planetsong);
	LBMapSprite(0, map_emerald_0, 0);
	LBMoveSprite(0, 80, 48, 1, 1);
	LBMapSprite(1, map_emerald_1, 0);
	LBMoveSprite(1, 96, 48, 1, 1);
	LBMapSprite(2, map_emerald_2, 0);
	LBMoveSprite(2, 112, 48, 1, 1);
	LBMapSprite(3, map_emerald_3, 0);
	LBMoveSprite(3, 128, 48, 1, 1);
	LBMapSprite(4, map_emerald_4, 0);
	LBMoveSprite(4, 144, 48, 1, 1);
	LBRotateSprites();
	stream_text_middle((const char*) strIntro, 12);
	LBWaitSeconds(1);
	planet_transition(0, 1, 28, -3, 199);
}

void load_splash()
{
	fade_through();
	Screen.scrollX = 0;
	Screen.scrollY = 0;
	Screen.scrollHeight = 32;
	Screen.overlayHeight = 0;
	game.current_screen = SPLASH;
	game.selection = START_SELECTED;
	clear_sprites(0, MAX_EXTENDED_SPRITES);
	LBRotateSprites();
	LBPrint(8, 15, (char*) str1Player);
	LBPrint(8, 16, (char*) strHighScores);
	LBPrint(4, 21, (char*) strCopyright);
	LBPrint(3, 26, (char*) strMusic);
	DrawMap2(6, 5, map_splash);
	LBMapSprite(0, map_right_arrow, 0);
}

void update_splash()
{
	if (game.player.controls.pressed & BTN_DOWN && game.selection == START_SELECTED)
	{
		game.selection = HIGH_SCORES_SELECTED;
		SFX_NAVIGATE;
	}
	else if (game.player.controls.pressed & BTN_UP && game.selection == HIGH_SCORES_SELECTED)
	{
		game.selection = START_SELECTED;
		SFX_NAVIGATE;
	}
	else if (select_pressed(&game.player.controls) && game.selection == START_SELECTED)
	{
		SFX_NAVIGATE;
		intro();
		return;
	}
	else if (select_pressed(&game.player.controls) && game.selection == HIGH_SCORES_SELECTED)
	{
		SFX_NAVIGATE;
		load_high_scores();
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

void load_high_scores()
{
	u16 score = 0;
	u8 ypos = 7;
	
	game.current_screen = HIGH_SCORES;
	game.high_score_index = -1;
	fade_through();
	Screen.scrollX = 0;
	Screen.scrollY = 0;
	Screen.scrollHeight = 32;
	Screen.overlayHeight = 0;
	load_eeprom(&scores);
	
	LBPrint(9, 3, (char*) strHighScores);
	
	LBMapSprite(0, map_emerald_0, 0);
	LBMapSprite(1, map_emerald_1, 0);
	LBMoveSprite(0, 7*8, 3*8, 1, 1);
	LBMoveSprite(1, 21*8, 3*8, 1, 1);
	
	for (u8 i = 0; i < 30; i += 5)
	{
		LBPrintByte(9, ypos, i/5+1, false);
		LBPrintChar(10, ypos, ' ');
		LBPrintChar(11, ypos, scores.data[i]);
		LBPrintChar(12, ypos, scores.data[i+1]);
		LBPrintChar(13, ypos, scores.data[i+2]);
		LBPrintChar(14, ypos, ' ');
		memcpy(&score, &(scores.data[i+3]), 2);
		LBPrintInt(19, ypos, score, true);
		
		if (game.score > score && game.high_score_index == -1)
		{
			// Shift scores down
			for (u8 j = 25; j > i; j -= 5)
			{
				memcpy(&(scores.data[j]), &(scores.data[j-5]), 5);
			}
			scores.data[i] = 'A';
			scores.data[i+1] = 'A';
			scores.data[i+2] = 'A';
			memcpy(&(scores.data[i+3]), &game.score, 2);
			LBPrintChar(11, ypos, scores.data[i]);
			LBPrintChar(12, ypos, scores.data[i+1]);
			LBPrintChar(13, ypos, scores.data[i+2]);
			LBPrintInt(19, ypos, game.score, true);
			game.high_score_index = i;
			LBMapSprite(2, map_down_arrow, 0);
			LBMapSprite(3, map_up_arrow, 0);
			LBMoveSprite(2, 11*8, (ypos-1)*8, 1, 1);
			LBMoveSprite(3, 11*8, (ypos+1)*8, 1, 1);
		}
		
		ypos += 2;
	}
	
	LBPrint(9, 22, (char*) strMainMenu);
}

void update_high_scores()
{
	if (game.player.controls.pressed & BTN_X)
	{
		SFX_NAVIGATE;
		game.high_score_index = -1;
		game.score = 0;
		save_eeprom(&scores);
		load_splash();
	}
	
	if (game.high_score_index != -1)
	{
		if (game.player.controls.pressed & BTN_RIGHT && game.high_score_index % 5 != 2)
		{
			SFX_NAVIGATE;
			game.high_score_index++;
			LBMoveSprite(2, extendedSprites[2].x+8, extendedSprites[2].y, 1, 1);
			LBMoveSprite(3, extendedSprites[3].x+8, extendedSprites[3].y, 1, 1);
		}
		else if (game.player.controls.pressed & BTN_LEFT && game.high_score_index % 5 != 0)
		{
			SFX_NAVIGATE;
			game.high_score_index--;
			LBMoveSprite(2, extendedSprites[2].x-8, extendedSprites[2].y, 1, 1);
			LBMoveSprite(3, extendedSprites[3].x-8, extendedSprites[3].y, 1, 1);
		}
		else if (game.player.controls.pressed & BTN_UP)
		{
			SFX_NAVIGATE;
			scores.data[game.high_score_index] = scores.data[game.high_score_index]-1;
			if (scores.data[game.high_score_index] < 'A') scores.data[game.high_score_index] = 'A';
			LBPrintChar(11+(game.high_score_index % 5), 7+(game.high_score_index/5)*2, scores.data[game.high_score_index]);
		}
		else if (game.player.controls.pressed & BTN_DOWN)
		{
			SFX_NAVIGATE;
			scores.data[game.high_score_index] = scores.data[game.high_score_index]+1;
			if (scores.data[game.high_score_index] > 'Z') scores.data[game.high_score_index] = 'Z';
			LBPrintChar(11+(game.high_score_index % 5), 7+(game.high_score_index/5)*2, scores.data[game.high_score_index]);
		}
	}
}

void update_pause()
{
	u8 x = Screen.scrollX / 8;
	u8 y = Screen.scrollY / 8;
	
	if (game.player.controls.pressed & BTN_START)
	{
		DrawMap2((x+8)%32, (y+6)%30, map_canvas);
		LBPrint((x+13)%32, (y+9)%30, (char*) strPaused);
		LBPrint((x+10)%32, (y+12)%30, (char*) strExitGame);
		hide_sprites(0, MAX_EXTENDED_SPRITES);
		LBRotateSprites();
		while (1)
		{
			WaitVsync(1);
			LBGetJoyPadState(&game.player.controls, 0);
			if (game.player.controls.pressed & BTN_START)
			{
				render_camera_view();
				break;
			}
			else if (game.player.controls.pressed & BTN_X)
			{
				tally_score((char*)strGameOver, 0);
				exit_game();
				break;
			}
		}
	}
}

void tally_score(char* title, u16 bonus)
{
	u8 x = Screen.scrollX / 8;
	u8 y = Screen.scrollY / 8;
	u16 tally = game.score;
	u16 counter = 0;
	
	hide_sprites(0, MAX_EXTENDED_SPRITES);
	LBRotateSprites();
	
	StopSong();
	DrawMap2((x+8)%32, (y+6)%30, map_canvas);
	LBPrint((x+10)%32, (y+7)%30, title);
	LBPrint((x+9)%32, (y+9)%30, (char*) strTally);
	LBPrint((x+9)%32, (y+11)%30, (char*) strScore);
	LBPrint((x+9)%32, (y+12)%30, (char*) strTime);
	LBPrint((x+9)%32, (y+13)%30, (char*) strLevelBonus);
	
	LBPrintInt((x+20)%32, (y+9)%30, tally, true);
	LBPrintInt((x+20)%32, (y+11)%30, game.level_score, true);
	LBPrintInt((x+20)%32, (y+12)%30, game.time, true);
	LBPrintInt((x+20)%32, (y+13)%30, bonus, true);
	
	LBWaitSeconds(2);
	
	// Tally Score
	counter = game.level_score;
	while (counter > 0)
	{
		WaitVsync(1);
		SFX_HIT;
		tally += 1;
		counter -= 1;
		LBPrintInt((x+20)%32, (y+9)%30, tally, true);
		LBPrintInt((x+20)%32, (y+11)%30, counter, true);
		WaitUs(TALLY_DELAY);
	}
	
	// Tally Time
	counter = game.time;
	while (counter > 0)
	{
		WaitVsync(1);
		SFX_HIT;
		tally -= 1;
		if (tally == 0 || tally == 65535) tally = 0;
		counter -= 1;
		LBPrintInt((x+20)%32, (y+9)%30, tally, true);
		LBPrintInt((x+20)%32, (y+12)%30, counter, true);
		WaitUs(TALLY_DELAY);
	}
	
	// Tally Level Bonus
	counter = bonus;
	while (counter > 0)
	{
		WaitVsync(1);
		SFX_HIT;
		tally += 1;
		counter -= 1;
		LBPrintInt((x+20)%32, (y+9)%30, tally, true);
		LBPrintInt((x+20)%32, (y+13)%30, counter, true);
		WaitUs(TALLY_DELAY);
	}
	
	game.score = tally;
	game.time = 0;
	LBWaitSeconds(2);
}

void challenge()
{
	u8 x = Screen.scrollX / 8;
	u8 y = Screen.scrollY / 8;
	u16 counter = 0;
	char* challenge_pointer = (char*)strChallenge + (game.current_level_index/2)*CHALLENGE_LENGTH;
	DrawMap2((x+7)%32, (y+6)%30, map_dialog);
	stream_text_anywhere((const char*)challenge_pointer, (x+8)%32, (y+9)%30);
	LBWaitSeconds(3);
	game.boss.active = 1;
	render_camera_view();
}

void update_player_ai(Player* player) 
{
	 static s8 last_shield_value;
	 
	 if (player->ai_flags == AI_NOT_READY) {
		 player->controls.held = BTN_LEFT;
		 player->ai_flags = AI_READY;
	 }
	 else if (player->ai_flags & AI_READY) {
		 LBResetJoyPadState(&player->controls);
		 if (game.camera_x/8 + CAMERA_WIDTH >= game.level_width) {
			 challenge();
			 player->ai_flags = AI_WALKING;
			 player->controls.held = BTN_LEFT;
		 }
	 } else if (player->ai_flags & AI_WALKING) {
		 player->controls.pressed = 0;
		 if (game.frame_counter % 15 == 0) {
			 player->controls.pressed = BTN_A;
		 }
		 if (last_shield_value > player->shield) {
			 player->controls.pressed |= BTN_B;
		 }
		 if (player->shared.x <= game.camera_x) {
			 player->controls.held = BTN_RIGHT;
		 } else if (player->shared.x >= game.camera_x+(CAMERA_WIDTH-2)*8) {
			 player->controls.held = BTN_LEFT;
		 }
	 }
	 last_shield_value = player->shield;
}

#if DEBUG_MODE
void prepare_debugging() {
	fade_through();
	Screen.scrollX = 0;
	Screen.scrollY = 0;
	Screen.scrollHeight = 29;
	Screen.overlayHeight = 3;
	Screen.overlayTileTable = tiles_data;
	clear_overlay(2);
	game.current_screen = LEVEL;
	game.selection = START_SELECTED;
	game.lives = LIVES;
	game.score = 0;
	game.level_score = 0;
	game.time = 0;
	game.current_level_index = DEBUG_LEVEL;
	game.level_ended = 0;
	clear_sprites(0, MAX_EXTENDED_SPRITES);
	LBResetJoyPadState(&game.player.controls);
	LBResetJoyPadState(&game.boss.controls);
	switch (DEBUG_LEVEL)
	{
		case 0: map_level_info((char*) map_level_ice); break;
		case 1: map_level_info((char*) map_level_space); break;
		case 2: map_level_info((char*) map_level_fire); break;
		case 3: map_level_info((char*) map_level_space); break;
		case 4: map_level_info((char*) map_level_forest); break;
		case 5: map_level_info((char*) map_level_space); break;
		case 6: map_level_info((char*) map_level_city); break;
		case 7: map_level_info((char*) map_level_space); break;
		case 8: map_level_info((char*) map_level_desert); break;
		case 9: map_level_info((char*) map_level_space_final); break;
	}
	game.column_count = 0;
#if DEBUG_LEVEL_LOCATION == DEBUG_BEGGINING
	game.camera_x = get_camera_x(DEBUG_LEVEL)*8;
	game.camera_y = get_camera_y(DEBUG_LEVEL)*8;
	game.player.shared.x = get_hero_spawn_x(DEBUG_LEVEL)*8;
	game.player.shared.y = get_hero_spawn_y(DEBUG_LEVEL)*8;
	game.scroll_src_x = game.camera_x / 8 + CAMERA_WIDTH + 1;
    game.scroll_dest_x = game.camera_x / 8 + CAMERA_WIDTH + 1;
	game.scroll_src_y = game.camera_y / 8 + CAMERA_HEIGHT + 1;
    game.scroll_dest_y =  game.camera_y / 8 + CAMERA_HEIGHT + 1;
#else
	game.camera_x = 199*8;
	game.camera_y = 0*8;
	game.player.shared.x = 202*8;
	game.player.shared.y = 21*8;
	game.scroll_src_x = game.camera_x / 8 + CAMERA_WIDTH + 1;
    game.scroll_dest_x = 29;
	game.scroll_src_y = game.camera_y / 8 + CAMERA_HEIGHT + 1;
    game.scroll_dest_y =  26;
#endif
	game.boss.shared.x = get_boss_spawn_x(DEBUG_LEVEL)*8;
	game.boss.shared.y = get_boss_spawn_y(DEBUG_LEVEL)*8;
	game.scroll_x = 0;
	game.scroll_y = 0;
	game.spawn_rate = BASE_SPAWN_RATE;
	LBPrint(0, VRAM_TILES_V-3, (char*) strShield);
	LBPrint(18, VRAM_TILES_V-3, (char*) strTime);
	LBPrint(0, VRAM_TILES_V-2, (char*) strLives);
	LBPrint(17, VRAM_TILES_V-2, (char*) strScore);
	LBPrint(17, VRAM_TILES_V-1, (char*) strTally);
	
	LBPrintByte(9, VRAM_TILES_V-3, game.player.shield ,true);
	LBPrintInt(27, VRAM_TILES_V-3, game.time ,true);
	LBPrintByte(9, VRAM_TILES_V-2, game.lives ,true);
	LBPrintInt(27, VRAM_TILES_V-2, game.level_score ,true);
	LBPrintInt(27, VRAM_TILES_V-1, game.score ,true);
	
	init_player_state();
	init_boss_state();
	init_enemy_state();
	
	render_camera_view();
}
#endif

int main()
{
	// Initialize
	GetPrngNumber(GetTrueRandomSeed());
	InitMusicPlayer(my_patches);
	ClearVram();
	SetMasterVolume(0x05);
	SetTileTable(tiles_data);
	SetSpritesTileTable(sprites_data);
	LBSetFontTilesMap((char*) map_font);
	init_default_high_scores();
#if DEBUG_MODE
	prepare_debugging();
#else
	load_splash();
#endif
	while (1)
	{
		WaitVsync(1);
		LBGetJoyPadState(&game.player.controls, 0);
		if (game.current_screen == LEVEL)
		{
			update_level();
			if (update_player(&game.player, PLAYER_SLOT))
			{
				update_shot(&game.player, PLAYER_SHOT_SLOT);
				update_enemies();
				update_enemy_shots();
				animate_enemies();
				animate_enemy_shots();
				animate_player(&game.player, &game.boss, PLAYER_SLOT);
				animate_shot(&game.player, &game.boss, PLAYER_SHOT_SLOT);
				update_pause();
			}
			if (!is_space() && (game.camera_x >= BOSS_UPDATE_THRESHOLD) && update_player(&game.boss, BOSS_SLOT))
			{
				update_shot(&game.boss, BOSS_SHOT_SLOT);
				animate_player(&game.boss, &game.player, BOSS_SLOT);
				animate_shot(&game.boss, &game.player, BOSS_SHOT_SLOT);
				update_player_ai(&game.boss);
			}
		}
		else if (game.current_screen == SPLASH)
		{
			update_splash();
			
		}
		else if (game.current_screen == HIGH_SCORES)
		{
			update_high_scores();
			
		}
		LBRotateSprites();
	}
}
