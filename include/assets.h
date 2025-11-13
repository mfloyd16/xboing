#ifndef XBOING_ASSETS_H
#define XBOING_ASSETS_H

#include <raylib.h>

/* Title textures */
extern Texture2D titleX, titleB, titleO, titleI, titleN, titleG;
extern Texture2D titleBig, titleSml;

/* UI textures */
extern Texture2D icon, mouse, question, text;
extern Texture2D larrow, rarrow, highscr, floppy;

/* Ball textures */
extern Texture2D ball1, ball2, ball3, ball4;
extern Texture2D killer, life;
extern Texture2D bbirth[8];

/* Block textures */
extern Texture2D blakblk, blakblkH, blueblk, bombblk;
extern Texture2D grnblk, purpblk, redblk, tanblk, yellblk;
extern Texture2D cntblk, cntblk1, cntblk2, cntblk3, cntblk4, cntblk5;
extern Texture2D stkyblk, hypspc, dynamite;
extern Texture2D roamer, roamerD, roamerL, roamerR, roamerU;
extern Texture2D death[5];

/* Bonus textures */
extern Texture2D bonus[4];
extern Texture2D x2bonus[4];
extern Texture2D x4bonus[4];
extern Texture2D clock, lotsammo, machgun, multibal;
extern Texture2D padexpn, padshrk, reverse, speed;
extern Texture2D walloff, xtrabal, xtrabal2;

/* Paddle textures */
extern Texture2D padhuge, padmed, padsml;

/* Eye dude textures */
extern Texture2D deveyes, deveyes1, deveyes2, deveyes3, deveyes4, deveyes5;
extern Texture2D guydead;
extern Texture2D guyl[6], guyr[6];

/* Gun textures */
extern Texture2D bullet, tink;

/* Star textures */
extern Texture2D stars[11];

/* Background textures */
extern Texture2D bgrnd, bgrnd2, bgrnd3, bgrnd4, bgrnd5;
extern Texture2D mnbgrnd, space;

/* Guide textures */
extern Texture2D guide, guide1, guide2, guide3, guide4, guide5, guide6;
extern Texture2D guide7, guide8, guide9, guide10, guide11;

/* Digit textures */
extern Texture2D digits[10];

/* Block explosion textures */
extern Texture2D exblue[3], exbomb[3], excnt[3], exdeath[4];
extern Texture2D exgren[3], expurp[3], exred[3], extan[3];
extern Texture2D exx2bs[3], exyell[3];

/* Presents textures */
extern Texture2D earth, flag, justin, kibell, presents;

/* API */
void Assets_Init(void);
void Assets_Cleanup(void);

#endif /* XBOING_ASSETS_H */
