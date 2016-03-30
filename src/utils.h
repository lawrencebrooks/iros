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
#ifndef LB_UTILS_H
#define LB_UTILS_H

extern struct SpriteStruct sprites[];
extern ScreenType Screen;

struct SpriteStruct extendedSprites[MAX_EXTENDED_SPRITES];

typedef struct sJoyPadState {
	unsigned int pressed;
	unsigned int released;
	unsigned int held;
	unsigned char held_cycles;
} JoyPadState;

typedef struct sAnimation {
	u8 current_anim;		// Current animation
	u8 anim_count;			// Total animation count
	u8 frames_per_anim;		// Animation lifetime in amount of render frames
	u8 frame_count;			// Render frame counter
	u8 looped;
	u8 reversing;
	char** anims;
} Animation;

char* LBGetNextFrame(Animation* anim)
{
	anim->looped = 0;
	anim->frame_count += 1;
	if (anim->frame_count < anim->frames_per_anim)
	{
		return anim->anims[anim->current_anim];
	}
	anim->frame_count = 0;
	anim->current_anim = (anim->current_anim + 1) % anim->anim_count;
	if (anim->current_anim == 0) anim->looped = 1;
	return anim->anims[anim->current_anim];
}

char* LBGetNextFrameReverse(Animation* anim)
{
	anim->looped = 0;
	anim->frame_count += 1;
	if (anim->frame_count < anim->frames_per_anim)
	{
		return anim->anims[anim->current_anim];
	}
	anim->frame_count = 0;
	if (anim->reversing)
	{
		anim->current_anim = anim->current_anim - 1;
		if (anim->current_anim == 0xff)
		{
			anim->current_anim = 0;
			anim->looped = 1;
			anim->reversing = 0;
		}
	}
	else
	{
		anim->current_anim = (anim->current_anim + 1) % anim->anim_count;
		if (anim->current_anim == 0) 
		{
			anim->reversing = 1;
			anim->current_anim = anim->anim_count - 1;
		}
	}
	return anim->anims[anim->current_anim];
}

void LBGetJoyPadState(JoyPadState* p, unsigned char index)
/*
 * Get the current joy pad button state for index controller
 */
{
	static unsigned int p_prev[2] = {0,0};

	p->held = ReadJoypad(index);

	// Count held cycles
	if (p->held == 0 || p->held != p_prev[index])
	    p->held_cycles = 0;
	else
	    p->held_cycles += 1;

	p->pressed = p->held & (p->held ^ p_prev[index]);
	p->released = p_prev[index] & (p->held ^ p_prev[index]);
	p_prev[index] = p->held;
}

void LBCopyChars(u8* dst, u8 *src, u8 count)
/*
 * Custom string copy
 */
{
	for (u8 i = 0; i < count; i++)
	{
		dst[i] = src[i];
	}
}

void LBPrintStr(u8 x, u8 y, u8* txt, u8 count)
{
	for (u8 i = 0; i < count; i++)
	{
		PrintChar(x+i, y, txt[i]);
	}
}

u8 LBCollides(u8 x1, u8 y1, u8 width1, u8 height1,
						 u8 x2, u8 y2, u8 width2, u8 height2)
{
	if (y1 + height1 <= y2) return 0;
	if (y1 >= y2 + height2) return 0;
	if (x1 + width1 <= x2) return 0;
	if (x1 >= x2 + width2) return 0;
	return 1;
}

int LBRandom(unsigned int from, unsigned int to)
/*
 * Return a random number between 'from' and 'to'.
 */
{
	static unsigned int random_seed = 0;
	if (random_seed == 0) random_seed = GetTrueRandomSeed();
	
	static u8 shift_count = 0;
	unsigned int shifted = random_seed >> shift_count;
	unsigned int delta = to - from;

	shift_count++;
	if (shift_count >= 16) shift_count = 0;

	return from + ((delta + shifted) % delta);
}

void LBWaitSeconds(u8 seconds)
{
	for(u8 i = 0; i < seconds; i++)
	{
		WaitUs(65535);
	}
}

void LBMapSprite(u8 startSprite,const char *map,u8 spriteFlags)
{
	unsigned char mapWidth=pgm_read_byte(&(map[0]));
	unsigned char mapHeight=pgm_read_byte(&(map[1]));
	s8 x,y,dx,dy,t; 

	if(spriteFlags & SPRITE_FLIP_X)
	{
		x=(mapWidth-1);
		dx=-1;
	}
	else
	{
		x=0;
		dx=1;
	}

	if(spriteFlags & SPRITE_FLIP_Y)
	{
		y=(mapHeight-1);
		dy=-1;
	}
	else
	{
		y=0;
		dy=1;
	}

	for(u8 cy=0;cy<mapHeight;cy++)
	{
		for(u8 cx=0;cx<mapWidth;cx++)
		{
			t=pgm_read_byte(&(map[(y*mapWidth)+x+2])); 
			extendedSprites[startSprite].tileIndex=t;
			extendedSprites[startSprite++].flags=spriteFlags;
			x+=dx;
		}
		y+=dy;
		x=(spriteFlags & SPRITE_FLIP_X)?(mapWidth-1):0;
	}
}

void LBMoveSprite(u8 startSprite,u8 x,u8 y,u8 width,u8 height)
{
	for(u8 dy=0;dy<height;dy++)
	{
		for(u8 dx=0;dx<width;dx++)
		{
		
			extendedSprites[startSprite].x=x+(TILE_WIDTH*dx);
		
			#if SCROLLING == 1
				if((Screen.scrollHeight<32) && (y+(TILE_HEIGHT*dy))>(Screen.scrollHeight*TILE_HEIGHT))
				{
					u8 tmp=(y+(TILE_HEIGHT*dy))-(Screen.scrollHeight*TILE_HEIGHT);
					extendedSprites[startSprite].y=tmp;
				}
				else
				{
					extendedSprites[startSprite].y=y+(TILE_HEIGHT*dy);
				}
			#else
				if((VRAM_TILES_V<32) && (y+(TILE_HEIGHT*dy))>(VRAM_TILES_V*TILE_HEIGHT))
				{
					u8 tmp=(y+(TILE_HEIGHT*dy))-(VRAM_TILES_V*TILE_HEIGHT);
					extendedSprites[startSprite].y=tmp;
				}
				else
				{
					extendedSprites[startSprite].y=y+(TILE_HEIGHT*dy);
				}
			#endif

			startSprite++;
		}
	}	
}

void LBRotateSprites(u8 mappedSpriteCount)
{
	static u8 swapped = 0;
	u8 counter = 0;
	
	if (swapped)
	{
		while (counter < MAX_SPRITES && mappedSpriteCount > 0)
		{
			sprites[counter] = extendedSprites[--mappedSpriteCount];
			counter++;
		}
	}
	else
	{
		while (counter < MAX_SPRITES && counter < mappedSpriteCount)
		{
			sprites[counter] = extendedSprites[counter];
			counter++;
		}
	}
	swapped = swapped & 1;
}

#endif
