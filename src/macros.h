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

// EEPROM ID
#define EEPROM_SCORES_ID 0x8C

// Screens
#define SPLASH 0
#define HIGH_SCORES 1
#define LEVEL 2

// Selections
#define START_SELECTED 0
#define HIGH_SCORES_SELECTED 1

// Frame counts
#define FRAMES_PER_FADE 3
#define FRAMES_PER_GRACE 120

// General macros
#define FRAME_TIME 0.0166666
#define MAX_SHOTS 4
#define TEXT_LINGER 3
#define CAMERA_WIDTH 28
#define CAMERA_HEIGHT 27
#define LEVEL_WIDTH 60
#define LEVEL_HEIGHT 27

// Sound Effects
#define SFX_NAVIGATE TriggerNote(PCM_CHANNEL,PATCH_NAVIGATE,23,0xff)

#endif
