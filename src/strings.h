/*
 * All printable strings
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
#ifndef LB_STRINGS_H
#define LB_STRINGS_H


// String indexes
const unsigned char strCopyright [] PROGMEM="2016 LAWRENCE BROOKS";
const unsigned char str1Player [] PROGMEM="START";
const unsigned char strHighscores [] PROGMEM="HIGH SCORES";
const unsigned char strSelectHandle [] PROGMEM="SELECT - A/START";
const unsigned char strScore [] PROGMEM="SCORE: ";
const unsigned char strShield [] PROGMEM="SHIELD: ";
const unsigned char strLives [] PROGMEM="LIVES: ";
const unsigned char strTime [] PROGMEM="TIME: ";
const unsigned char strExit [] PROGMEM="EXIT - X";
const unsigned char strPaused [] PROGMEM="PAUSED";
const unsigned char strLevels [] PROGMEM="   GLACIES   ";

// Default scores
const unsigned char default_scores[] PROGMEM = {
    // 1. UZE 0
    // 2. LJB 0
    // 3. AAA 0
    // 4. BBB 0
    // 5. CCC 0
    // 6. DDD 0
	
    0x55, 0x5a, 0x45, 0x00, 0x00,
    0x4c, 0x4a, 0x42, 0x00, 0x00,
    0x41, 0x41, 0x41, 0x00, 0x00,
    0x42, 0x42, 0x42, 0x00, 0x00,
    0x43, 0x43, 0x43, 0x00, 0x00,
    0x44, 0x44, 0x44, 0x00, 0x00,
};

#endif
