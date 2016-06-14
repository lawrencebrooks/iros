/*
 * Transformation file: tiles.xml
 * Source image: ../art/tiles.png
 * Tile width: 8px
 * Tile height: 8px
 * Output format: (null)
 */
#define L_ICE_LEFT 1
#define L_ICE_MIDDLE 2
#define L_ICE_RIGHT 3
#define L_ICE_BOTTOM 4
#define L_ICICLE_DOWN 9
#define L_ICICLE_UP 10
#define L_FIRE_BOTTOM 12
#define L_FIRE_TOP 13
#define L_FIRE_LAVA 14
#define L_JUNGLE_BOTTOM 15
#define L_JUNGLE_TOP 16
#define L_JUNGLE_FLOWER 17
#define L_JUNGLE_BRANCH 18
#define L_CITY_BOTTOM 19
#define L_CITY_TOP 20
#define L_CITY_SPARK 21
#define L_DESERT_BOTTOM 23
#define L_DESERT_TOP 22
#define L_DESERT_SPIKE_UP 25
#define L_DESERT_SPIKE_DOWN 24

#define MAP_LEVEL_MEGA_TILES_WIDTH 255
#define MAP_LEVEL_MEGA_TILES_HEIGHT 25
const char map_level_ice[] PROGMEM ={255,25
,0x0,0x1,0x1,0x1,0x2,0x3,0x4,0x1,0x5,0x1,0x1,0x1,0x1,0x1,0x6,0x3,0x7,0x1,0x1,0x1
,0x1,0x8,0x9,0x1,0x1,0xa,0xb,0x1,0xc,0x9,0x1,0x1,0xd,0x1,0xe,0xf,0x3,0x10,0x1,0x1
,0x1,0x1,0x1,0x11,0x1,0x12,0x3,0x13,0x14,0x15,0x1,0x16,0x1,0x1,0x1,0x17,0x3,0x4,0x1,0x1
,0x1,0x1,0x1,0x1,0x1,0x6,0x3,0x7,0x1,0x1,0x1,0x18,0x19,0x19,0x19,0x19,0x1a,0x1b,0x1c,0x19
,0x19,0x1d,0x1e,0x1e,0x1e,0x1e,0x1f,0x3,0x20,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x21,0x22,0x23,0x1e
,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e
,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x24
,0x25,0x1e,0x1e,0x1e,0x26,0x1e,0x1e,0x1e,0x1e,0x1e,0x27,0x1e,0x1e,0x1e,0x28,0x29,0x1e,0x1e,0x1e,0x1e
,0x2a,0x1e,0x1e,0x1e,0x2b,0x2c,0x1e,0x1e,0x1e,0x2d,0x2e,0x2f,0x1e,0x30,0x1e,0x31,0x32,0x1e,0x1e,0x1e
,0x33,0x34,0x1e,0x1e,0x35,0x36,0x1e,0x37,0x38,0x39,0x1e,0x3a,0x3b,0x1e,0x3c,0x3d,0x3e,0x3f,0x40,0x3
,0x3e,0x1e,0x41,0x1e,0x42,0x43,0x44,0x45,0x46,0x3,0x44,0x1e,0x47,0x1e,0x48,0x49,0x4a,0x4b,0x4c,0x3
,0x4a,0x1e,0x4d,0x1e,0x4e,0x4f,0x50,0x51,0x1e,0x52,0x50,0x1e,0x53,0x1e,0x54,0x55,0x3,0x56,0x57,0x58
,0x3,0x56,0x59,0x59,0x5a,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3};

#define MAP_LEVEL_MEGA_TILES_WIDTH 32
#define MAP_LEVEL_MEGA_TILES_HEIGHT 25
const char map_level_space[] PROGMEM ={32,25
,0x5b,0x5c,0x5d,0x5e,0x5f,0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e
,0x5e,0x6f,0x70,0x71,0x72,0x73,0x74,0x75,0x67,0x76,0x77,0x78};

#define MAP_LEVEL_MEGA_TILES_WIDTH 255
#define MAP_LEVEL_MEGA_TILES_HEIGHT 25
const char map_level_fire[] PROGMEM ={255,25
,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79
,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79
,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79
,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x79,0x7a,0x7b,0x7b,0x7b,0x7b,0x7b,0x7b,0x7b,0x7b
,0x7b,0x7c,0x7d,0x7e,0x7e,0x7f,0x7e,0x80,0x7e,0x7e,0x7e,0x7e,0x81,0x82,0x83,0x84,0x7e,0x85,0x86,0x7e
,0x7e,0x87,0x88,0x7e,0x89,0x7e,0x7e,0x7e,0x7e,0x8a,0x7e,0x8b,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e
,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x8c,0x8d,0x7e,0x8e,0x7e
,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x8f,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x90,0x91
,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x92,0x93,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x94,0x95
,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x96,0x7e,0x7e,0x97,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x98,0x7e,0x99
,0x7e,0x8f,0x9a,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x9b,0x9c,0x90,0x9d,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x9e
,0x9f,0x92,0xa0,0xa1,0x7e,0x7e,0x7e,0x7e,0x7e,0xa2,0xa3,0x94,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xa9,0xaa
,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb2,0xb3,0xb2,0xb4,0xb2,0xb2,0xb2};

#define MAP_LEVEL_MEGA_TILES_WIDTH 255
#define MAP_LEVEL_MEGA_TILES_HEIGHT 25
const char map_level_forest[] PROGMEM ={255,25
,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5
,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5
,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5
,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb5,0xb6,0xb7,0xb7,0xb7,0xb7,0xb7,0xb7,0xb7,0xb7
,0xb7,0xb8,0xb9,0xba,0xb9,0xbb,0xbc,0xbd,0xba,0xbe,0xbf,0xc0,0xc1,0xc2,0xc1,0xc3,0xc4,0xc5,0xc2,0xc2
,0xc6,0xc0,0xc7,0xc8,0xc7,0xc9,0xca,0xcb,0xc8,0xcc,0xcd,0xc0,0xce,0xcf,0xce,0xd0,0xd1,0xd2,0xd3,0xd3
,0xd4,0xd5,0xc0,0xd6,0xc0,0xd7,0xd8,0xd9,0xda,0xda,0xd2,0xc2,0xc0,0xdb,0xd5,0xdb,0xdc,0xdd,0xde,0xd5
,0xdf,0xe0,0xc0,0xc1,0xc2,0xc1,0xc3,0xc4,0xc5,0xc2,0xc2,0xc6,0xc0,0xe1,0xe2,0xe1,0xe3,0xe4,0xe5,0xe2
,0xe2,0xe6,0xe7,0xce,0xe8,0xce,0xd0,0xd1,0xd2,0xd3,0xd3,0xd4,0xd5,0xc0,0xd6,0xc0,0xd7,0xd8,0xd9,0xda
,0xda,0xd2,0xc2,0xc0,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,0xf0,0xc8,0xc0,0xc1,0xc2,0xc1,0xc3,0xc4,0xc5
,0xc2,0xc2,0xc6,0xc0,0xf1,0xe0,0xf1,0xf2,0xf3,0xd4,0xe0,0xe0,0xf4,0xc0,0xce,0xd3,0xce,0xd0,0xd1,0xd2
,0xd3,0xd3,0xd4,0xd5,0xc0,0xd6,0xc0,0xd7,0xd8,0xd9,0xda,0xda,0xd2,0xc2,0xf5,0xf6,0xf7,0xf8,0xf6,0xf6
,0xf6,0xf6,0xf6,0xf6,0xf9,0xfa,0xfb,0xfc,0xfa,0xfa,0xfa,0xfa,0xfa,0xfa,0xfa};

#define MAP_LEVEL_MEGA_TILES_WIDTH 255
#define MAP_LEVEL_MEGA_TILES_HEIGHT 25
const char map_level_city[] PROGMEM ={255,25
,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfe,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd
,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd
,0xfd,0xff,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd
,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,