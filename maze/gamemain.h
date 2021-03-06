#ifndef __GAMEMAIN_H__
#define __GAMEMAIN_H__

#define GM_MAX_PLAYER			2
#define GAME_TIME				180

#define MAZE_SIZE				16
#define MAZE_BLOCK_SIZE			128.0f
#define MAZE_BLOCK_HEIGHT		160.0f
#define MAZE_STAR_NUM				5

#define GR_FAR_Z				(MAZE_BLOCK_SIZE * MAZE_SIZE * 4)
#define GR_VISIBLE_DIST			(GR_FAR_Z * GR_FAR_Z / 4)
#define GR_NEAR_Z				1
#define GR_VIEWANGLE			60

typedef struct MAINGAME MAINGAME;
typedef struct MAINGAMEUI MAINGAMEUI;
typedef struct CAMERA CAMERA;
typedef struct ITEM ITEM;
typedef struct MAZE MAZE;
typedef struct NODE NODE;
typedef struct PLAYER PLAYER;
typedef struct PORTAL PORTAL;
typedef struct SUNLIGHT SUNLIGHT;
typedef struct SKY SKY;
typedef struct STAR STAR;

#endif // !__GAMEMAIN_H__
