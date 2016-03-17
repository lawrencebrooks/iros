/*
 * All printable strings
 *
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
#ifndef LB_STRINGS_H
#define LB_STRINGS_H


// String indexes
const unsigned char strCopyright [] PROGMEM="2015 LAWRENCE BROOKS";
const unsigned char str1Player [] PROGMEM="PLAYER VS CPU";
const unsigned char str2Player [] PROGMEM="PLAYER VS PLAYER";
const unsigned char strHighscores [] PROGMEM="TANK RANK";
const unsigned char strHandlesTitle [] PROGMEM="EDIT HANDLE";
const unsigned char strUnderline [] PROGMEM="___________";
const unsigned char strVertSep [] PROGMEM=";";
const unsigned char strPlayer1 [] PROGMEM="P1";
const unsigned char strPlayer2 [] PROGMEM="P2";
const unsigned char strSelectHandle [] PROGMEM="SELECT - A/START";
const unsigned char strConfirmHandle [] PROGMEM="CONFIRM - A/START";
const unsigned char strCancelHandle [] PROGMEM="CANCEL - X";
const unsigned char strReset [] PROGMEM="RESET DEFAULTS - SL (5S)";
const unsigned char strChangeHandle [] PROGMEM="EDIT - UP/DOWN";
const unsigned char strOwns [] PROGMEM="OWNS";
const unsigned char strBy [] PROGMEM="BY";
const unsigned char strFinalScore [] PROGMEM="FINAL SCORE";
const unsigned char banter_map [] PROGMEM="SUCH FAIL     \0"
"LOL           \0"
"BROKEN BUTTON?\0"
"TRY AT LEAST  \0"
"TOO EASY      \0"
"YAWN          \0"
"NOOB          \0"
"THAT'S IT?    \0"
"NEED HELP?    \0"
"PFFT          ";
const unsigned char strBanterClear [] PROGMEM="              ";
const unsigned char strScore [] PROGMEM="SCORE:";
const unsigned char strTotal [] PROGMEM="TOTAL:";
const unsigned char strExit [] PROGMEM="BAIL - X";
const unsigned char strPaused [] PROGMEM="PAUSED";
const unsigned char strLevels [] PROGMEM="   BOOT CAMP   \0"
" THE BRICKYARD \0"
"   VELOCITY    \0"
"   THE MAZE    \0"
"   RICHOCHET   \0"
" THE HONEY POT \0"
"  FOCAL POINT  \0"
"IT'S NOT FAIR! \0"
"  MUCH BETTER  \0"
"   THE CANAL   \0"
"  THE HARBOUR  ";

// Default handles and scores
const unsigned char default_handles[] PROGMEM = {
    // 1. UZE
    // 2. LJB
    // 3. AAA
    // 4. BBB
    // 5. CCC
    // 6. DDD
    // 7. EEE
    // 8. FFF
    // 9. GGG
    // 10. CPU
    0x55, 0x5a, 0x45,
    0x4c, 0x4a, 0x42,
    0x41, 0x41, 0x41,
    0x42, 0x42, 0x42,
    0x43, 0x43, 0x43,
    0x44, 0x44, 0x44,
    0x45, 0x45, 0x45,
    0x46, 0x46, 0x46,
    0x47, 0x47, 0x47,
    0x43, 0x50, 0x55
};

const unsigned char default_scores[] PROGMEM = {
    // 1. owns 2. by 20 - 13
    // 2. owns 3. by 20 - 14
    // 3. owns 4. by 20 - 15
    // 4. owns 5. by 20 - 16
    // 5. owns 6. by 20 - 17
    // 6. owns 7. by 20 - 18
    0, 1, 20, 13, 0,
    1, 2, 20, 14, 0,
    2, 3, 20, 15, 0,
    3, 4, 20, 16, 0,
    4, 5, 20, 17, 0,
    5, 6, 20, 18, 0
};

#endif
