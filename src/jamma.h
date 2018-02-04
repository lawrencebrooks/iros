/*
 * Program: Iros
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
 
#ifndef LB_JAMMA_H
#define LB_JAMMA_H

u8 credits;
u8 coin_count;
u8 coin_credit_rel;
u8 demo_sound_off;
u16 p2_prev_state;

#define CC_1_1 1
#define CC_1_2 2
#define CC_1_3 3
#define CC_2_1 4
#define CC_3_2 5
#define CC_3_4 6
#define CC_4_3 7
#define CC_FRE 8


void extract_dip_switches(u8 switches) {
	coin_credit_rel = switches << 4;
    coin_credit_rel = (coin_credit_rel >> 5) + 1;
	demo_sound_off = switches << 3;
    demo_sound_off = switches >> 7;
}

void coin_inserted() {
	TriggerNote(2,PATCH_PLAYER_SHOOT,0x23,0xff);
    if (coin_credit_rel == CC_FRE || credits == 255) {
        credits = 255;
        return;
    }
    coin_count += 1;
    if (coin_credit_rel < 4) {
        coin_count = 0;
        credits += coin_credit_rel;
    } else if (coin_count == 2 && coin_credit_rel == CC_2_1) {
        coin_count = 0;
        credits += 1;
    } else if (coin_count == 3 && coin_credit_rel == CC_3_2) {
        coin_count = 0;
        credits += 2;
    } else if (coin_count == 3 && coin_credit_rel == CC_3_4) {
        coin_count = 0;
        credits += 4;
    } else if (coin_count == 4 && coin_credit_rel == CC_4_3) {
        coin_count = 0;
        credits += 3;
    }
}

u8 credits_available() {
	if (coin_credit_rel == CC_FRE) return 255;
    return credits;
}

u8 no_demo_sound() {
	return demo_sound_off;
}

void acquire_credit() {
    if (coin_credit_rel != CC_FRE) credits--;
}

void handle_coin_insert() {
    if ((ReadJoypad(1) & BTN_SL) && !(p2_prev_state & BTN_SL)) {
        coin_inserted();
    }
    if ((ReadJoypad(1) & BTN_SR) && !(p2_prev_state & BTN_SR)) {
        coin_inserted();
    }
    p2_prev_state = ReadJoypad(1);
}

#endif
