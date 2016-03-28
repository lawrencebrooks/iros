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

#define PATCH_NAVIGATE 0

const char ad_navigate[] PROGMEM ={
	0,PC_WAVE,2,
	0,PC_NOTE_DOWN,50, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_UP,1, 
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_DOWN,1,
	1,PC_NOTE_CUT,0,
	0,PATCH_END
};

const struct PatchStruct my_patches[] PROGMEM = {
  {0,0,ad_navigate,0,0},
};

#endif
