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
#ifndef PATCHES_H
#define PATCHES_H

#define PATCH_NAVIGATE 2
#define PATCH_PLAYER_SHOOT 3
#define PATCH_HIT 4
#define PATCH_PLAYER_EXPLODE 5
#define PATCH_ENEMY_EXPLODE 6


// Lead
const char instrument[] PROGMEM ={ 
	0,PC_WAVE,4,
	0,PC_ENV_SPEED,-35,
	0,PATCH_END
};

const char navigate[] PROGMEM ={
	0,PC_WAVE,4,
	0,PC_ENV_VOL,0x70,
	0,PC_ENV_SPEED,-40,
	0,PC_PITCH,60,
	1,PC_NOTE_CUT,0,
	0,PATCH_END
};

const char pshoot[] PROGMEM ={
	0,PC_WAVE,3,
	0,PC_ENV_VOL,127,
	0,PC_PITCH,80,
	1,PC_NOTE_DOWN,6,
	1,PC_NOTE_DOWN,6,
	1,PC_NOTE_DOWN,6,
	1,PC_ENV_VOL,0,
	3,PC_ENV_VOL,127,
	0,PC_PITCH,80,
	1,PC_NOTE_DOWN,6,
	1,PC_NOTE_DOWN,6,
	1,PC_NOTE_DOWN,6,
	1,PC_ENV_VOL,0,
	3,PC_PITCH,80,
	0,PC_ENV_VOL,32,
	1,PC_NOTE_DOWN,6,
	1,PC_NOTE_DOWN,6,
	1,PC_NOTE_DOWN,6,
	1,PC_NOTE_DOWN,6,
	1,PC_ENV_VOL,0,
	3,PC_PITCH,80,
	0,PC_ENV_VOL,15,
	1,PC_NOTE_DOWN,6,
	1,PC_NOTE_DOWN,6,
	1,PC_NOTE_DOWN,6,
	1,PC_NOTE_DOWN,6,
	1,PC_ENV_VOL,0,
	1,PC_NOTE_CUT,0,
	0,PATCH_END 
};

const char explode[] PROGMEM ={
	0,PC_ENV_VOL,127,
	0,PC_WAVE,3,
	0,PC_ENV_SPEED,-1, 
	0,PC_PITCH,65,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	//1,PC_NOTE_DOWN,4,
	3,PC_ENV_VOL,100,
	0,PC_PITCH,60,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	//1,PC_NOTE_DOWN,4, 
	3,PC_ENV_VOL,80,
	0,PC_PITCH,55,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	//1,PC_NOTE_DOWN,4,
	3,PC_ENV_VOL,50,
	0,PC_PITCH,50,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	//1,PC_NOTE_DOWN,4,
	3,PC_ENV_VOL,40,
	0,PC_PITCH,45,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	//1,PC_NOTE_DOWN,4,
	3,PC_ENV_VOL,20,
	0,PC_PITCH,40,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	1,PC_NOTE_DOWN,4,
	//1,PC_NOTE_DOWN,4,
	1,PC_NOTE_CUT,0,
	0,PATCH_END 
};


const struct PatchStruct my_patches[] PROGMEM = {
  {0,NULL,instrument,0,0},
  {0,NULL,instrument,0,0},
  {0,NULL,navigate,0,0},
  {0,NULL,pshoot,0,0},
  {0,NULL,navigate,0,0},
  {0,NULL,explode,0,0},
  {0,NULL,explode,0,0},
};


#endif
