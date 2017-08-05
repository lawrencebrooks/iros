/*
 * All printable strings
 *
 * Copyright 2017 Lawrence Brooks
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
const unsigned char strCopyright [] PROGMEM="2017 LAWRENCE BROOKS";
const unsigned char str1Player [] PROGMEM="START";
const unsigned char strHighScores [] PROGMEM="HIGH SCORES";
const unsigned char strSelectHandle [] PROGMEM="";
const unsigned char strScore [] PROGMEM="SCORE ";
const unsigned char strShield [] PROGMEM="SHIELD ";
const unsigned char strLives [] PROGMEM="LIVES ";
const unsigned char strTime [] PROGMEM="TIME ";
const unsigned char strMainMenu [] PROGMEM="X MAIN MENU";
const unsigned char strExitGame [] PROGMEM="X EXIT GAME";
const unsigned char strPaused [] PROGMEM="PAUSE";
const unsigned char strLevelClear [] PROGMEM="LEVEL CLEAR";
const unsigned char strLevelBonus [] PROGMEM="BONUS";
const unsigned char strGameOver [] PROGMEM="GAME OVER";
const unsigned char strTally [] PROGMEM="TALLY ";

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

const unsigned char strIntro [] PROGMEM="THE 5 KNIGHTS OF IROS\0"
"HAVE STOLEN YOUR POWER UPS\0"
"AND FLEW TO THEIR HOMEWORLDS\0\0"
"\0\0"
"JERKS\0\0"
"\0\0"
"DESTROY THEM AND\0"
"TAKE BACK YOUR SWAG\0#";

const unsigned char strChallenge [] PROGMEM="   PREPARE\0  TO BE ICED\0#          "
"     CUTE\0PINK IS SO YOU\0#         "
" I LIKE TREES\0 AND BEATINGS\0#      "
"    MY GUN\0  YOUR FACE\0#           "
" YEAH BUT ITS\0  A DRY HEAT\0#       ";

#endif
