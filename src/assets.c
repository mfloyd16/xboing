#include "../include/assets.h"
#include <stdio.h>

/* Define all textures (storage) */
Texture2D titleX, titleB, titleO, titleI, titleN, titleG;
Texture2D titleBig, titleSml;

Texture2D icon, mouse, question, text;
Texture2D larrow, rarrow, highscr, floppy;

Texture2D ball1, ball2, ball3, ball4;
Texture2D killer, life;
Texture2D bbirth[8];

Texture2D blakblk, blakblkH, blueblk, bombblk;
Texture2D grnblk, purpblk, redblk, tanblk, yellblk;
Texture2D cntblk, cntblk1, cntblk2, cntblk3, cntblk4, cntblk5;
Texture2D stkyblk, hypspc, dynamite;
Texture2D roamer, roamerD, roamerL, roamerR, roamerU;
Texture2D death[5];

Texture2D bonus[4];
Texture2D x2bonus[4];
Texture2D x4bonus[4];
Texture2D clock, lotsammo, machgun, multibal;
Texture2D padexpn, padshrk, reverse, speed;
Texture2D walloff, xtrabal, xtrabal2;

Texture2D padhuge, padmed, padsml;

Texture2D deveyes, deveyes1, deveyes2, deveyes3, deveyes4, deveyes5;
Texture2D guydead;
Texture2D guyl[6], guyr[6];

Texture2D bullet, tink;

Texture2D stars[11];

Texture2D bgrnd, bgrnd2, bgrnd3, bgrnd4, bgrnd5;
Texture2D mnbgrnd, space;

Texture2D guide, guide1, guide2, guide3, guide4, guide5, guide6;
Texture2D guide7, guide8, guide9, guide10, guide11;

Texture2D digits[10];

Texture2D exblue[3], exbomb[3], excnt[3], exdeath[4];
Texture2D exgren[3], expurp[3], exred[3], extan[3];
Texture2D exx2bs[3], exyell[3];

Texture2D earth, flag, justin, kibell, presents;

void Assets_Init(void)
{
    /* Load title textures */
    titleX = LoadTexture("resource/textures/presents/titleX.png");
    titleB = LoadTexture("resource/textures/presents/titleB.png");
    titleO = LoadTexture("resource/textures/presents/titleO.png");
    titleI = LoadTexture("resource/textures/presents/titleI.png");
    titleN = LoadTexture("resource/textures/presents/titleN.png");
    titleG = LoadTexture("resource/textures/presents/titleG.png");
    titleBig = LoadTexture("resource/textures/presents/titleBig.png");
    titleSml = LoadTexture("resource/textures/presents/titleSml.png");

    /* Load UI textures */
    icon = LoadTexture("resource/textures/icon.png");
    mouse = LoadTexture("resource/textures/mouse.png");
    question = LoadTexture("resource/textures/question.png");
    text = LoadTexture("resource/textures/text.png");
    larrow = LoadTexture("resource/textures/larrow.png");
    rarrow = LoadTexture("resource/textures/rarrow.png");
    highscr = LoadTexture("resource/textures/highscr.png");
    floppy = LoadTexture("resource/textures/floppy.png");

    /* Load ball textures */
    ball1 = LoadTexture("resource/textures/balls/ball1.png");
    ball2 = LoadTexture("resource/textures/balls/ball2.png");
    ball3 = LoadTexture("resource/textures/balls/ball3.png");
    ball4 = LoadTexture("resource/textures/balls/ball4.png");
    killer = LoadTexture("resource/textures/balls/killer.png");
    life = LoadTexture("resource/textures/balls/life.png");
    for (int i = 0; i < 8; i++) {
        char path[128];
        snprintf(path, sizeof(path), "resource/textures/balls/bbirth%d.png", i + 1);
        bbirth[i] = LoadTexture(path);
    }

    /* Load block textures */
    blakblk = LoadTexture("resource/textures/blocks/blakblk.png");
    blakblkH = LoadTexture("resource/textures/blocks/blakblkH.png");
    blueblk = LoadTexture("resource/textures/blocks/blueblk.png");
    bombblk = LoadTexture("resource/textures/blocks/bombblk.png");
    grnblk = LoadTexture("resource/textures/blocks/grnblk.png");
    purpblk = LoadTexture("resource/textures/blocks/purpblk.png");
    redblk = LoadTexture("resource/textures/blocks/redblk.png");
    tanblk = LoadTexture("resource/textures/blocks/tanblk.png");
    yellblk = LoadTexture("resource/textures/blocks/yellblk.png");
    cntblk = LoadTexture("resource/textures/blocks/cntblk.png");
    cntblk1 = LoadTexture("resource/textures/blocks/cntblk1.png");
    cntblk2 = LoadTexture("resource/textures/blocks/cntblk2.png");
    cntblk3 = LoadTexture("resource/textures/blocks/cntblk3.png");
    cntblk4 = LoadTexture("resource/textures/blocks/cntblk4.png");
    cntblk5 = LoadTexture("resource/textures/blocks/cntblk5.png");
    stkyblk = LoadTexture("resource/textures/blocks/stkyblk.png");
    hypspc = LoadTexture("resource/textures/blocks/hypspc.png");
    dynamite = LoadTexture("resource/textures/blocks/dynamite.png");
    roamer = LoadTexture("resource/textures/blocks/roamer.png");
    roamerD = LoadTexture("resource/textures/blocks/roamerD.png");
    roamerL = LoadTexture("resource/textures/blocks/roamerL.png");
    roamerR = LoadTexture("resource/textures/blocks/roamerR.png");
    roamerU = LoadTexture("resource/textures/blocks/roamerU.png");
    for (int i = 0; i < 5; i++) {
        char path[128];
        snprintf(path, sizeof(path), "resource/textures/blocks/death%d.png", i + 1);
        death[i] = LoadTexture(path);
    }

    /* Load bonus textures */
    for (int i = 0; i < 4; i++) {
        char path[128];
        snprintf(path, sizeof(path), "resource/textures/blocks/bonus%d.png", i + 1);
        bonus[i] = LoadTexture(path);
        snprintf(path, sizeof(path), "resource/textures/blocks/x2bonus%d.png", i + 1);
        x2bonus[i] = LoadTexture(path);
        snprintf(path, sizeof(path), "resource/textures/blocks/x4bonus%d.png", i + 1);
        x4bonus[i] = LoadTexture(path);
    }
    clock = LoadTexture("resource/textures/blocks/clock.png");
    lotsammo = LoadTexture("resource/textures/blocks/lotsammo.png");
    machgun = LoadTexture("resource/textures/blocks/machgun.png");
    multibal = LoadTexture("resource/textures/blocks/multibal.png");
    padexpn = LoadTexture("resource/textures/blocks/padexpn.png");
    padshrk = LoadTexture("resource/textures/blocks/padshrk.png");
    reverse = LoadTexture("resource/textures/blocks/reverse.png");
    speed = LoadTexture("resource/textures/blocks/speed.png");
    walloff = LoadTexture("resource/textures/blocks/walloff.png");
    xtrabal = LoadTexture("resource/textures/blocks/xtrabal.png");
    xtrabal2 = LoadTexture("resource/textures/blocks/xtrabal2.png");

    /* Load paddle textures */
    padhuge = LoadTexture("resource/textures/paddle/padhuge.png");
    padmed = LoadTexture("resource/textures/paddle/padmed.png");
    padsml = LoadTexture("resource/textures/paddle/padsml.png");

    /* Load eye dude textures */
    deveyes = LoadTexture("resource/textures/eyes/deveyes.png");
    deveyes1 = LoadTexture("resource/textures/eyes/deveyes1.png");
    deveyes2 = LoadTexture("resource/textures/eyes/deveyes2.png");
    deveyes3 = LoadTexture("resource/textures/eyes/deveyes3.png");
    deveyes4 = LoadTexture("resource/textures/eyes/deveyes4.png");
    deveyes5 = LoadTexture("resource/textures/eyes/deveyes5.png");
    guydead = LoadTexture("resource/textures/eyes/guydead.png");
    for (int i = 0; i < 6; i++) {
        char path[128];
        snprintf(path, sizeof(path), "resource/textures/eyes/guyl%d.png", i + 1);
        guyl[i] = LoadTexture(path);
        snprintf(path, sizeof(path), "resource/textures/eyes/guyr%d.png", i + 1);
        guyr[i] = LoadTexture(path);
    }

    /* Load gun textures */
    bullet = LoadTexture("resource/textures/guns/bullet.png");
    tink = LoadTexture("resource/textures/guns/tink.png");

    /* Load star textures */
    for (int i = 0; i < 11; i++) {
        char path[128];
        snprintf(path, sizeof(path), "resource/textures/stars/star%d.png", i + 1);
        stars[i] = LoadTexture(path);
    }

    /* Load background textures */
    bgrnd = LoadTexture("resource/textures/bgrnds/bgrnd.png");
    bgrnd2 = LoadTexture("resource/textures/bgrnds/bgrnd2.png");
    bgrnd3 = LoadTexture("resource/textures/bgrnds/bgrnd3.png");
    bgrnd4 = LoadTexture("resource/textures/bgrnds/bgrnd4.png");
    bgrnd5 = LoadTexture("resource/textures/bgrnds/bgrnd5.png");
    mnbgrnd = LoadTexture("resource/textures/bgrnds/mnbgrnd.png");
    space = LoadTexture("resource/textures/bgrnds/space.png");

    /* Load guide textures */
    guide = LoadTexture("resource/textures/guides/guide.png");
    guide1 = LoadTexture("resource/textures/guides/guide1.png");
    guide2 = LoadTexture("resource/textures/guides/guide2.png");
    guide3 = LoadTexture("resource/textures/guides/guide3.png");
    guide4 = LoadTexture("resource/textures/guides/guide4.png");
    guide5 = LoadTexture("resource/textures/guides/guide5.png");
    guide6 = LoadTexture("resource/textures/guides/guide6.png");
    guide7 = LoadTexture("resource/textures/guides/guide7.png");
    guide8 = LoadTexture("resource/textures/guides/guide8.png");
    guide9 = LoadTexture("resource/textures/guides/guide9.png");
    guide10 = LoadTexture("resource/textures/guides/guide10.png");
    guide11 = LoadTexture("resource/textures/guides/guide11.png");

    /* Load digit textures */
    for (int i = 0; i < 10; i++) {
        char path[128];
        snprintf(path, sizeof(path), "resource/textures/digits/digit%d.png", i);
        digits[i] = LoadTexture(path);
    }

    /* Load block explosion textures */
    for (int i = 0; i < 3; i++) {
        char path[128];
        snprintf(path, sizeof(path), "resource/textures/blockex/exblue%d.png", i + 1);
        exblue[i] = LoadTexture(path);
        snprintf(path, sizeof(path), "resource/textures/blockex/exbomb%d.png", i + 1);
        exbomb[i] = LoadTexture(path);
        snprintf(path, sizeof(path), "resource/textures/blockex/excnt%d.png", i + 1);
        excnt[i] = LoadTexture(path);
        snprintf(path, sizeof(path), "resource/textures/blockex/exgren%d.png", i + 1);
        exgren[i] = LoadTexture(path);
        snprintf(path, sizeof(path), "resource/textures/blockex/expurp%d.png", i + 1);
        expurp[i] = LoadTexture(path);
        snprintf(path, sizeof(path), "resource/textures/blockex/exred%d.png", i + 1);
        exred[i] = LoadTexture(path);
        snprintf(path, sizeof(path), "resource/textures/blockex/extan%d.png", i + 1);
        extan[i] = LoadTexture(path);
        snprintf(path, sizeof(path), "resource/textures/blockex/exx2bs%d.png", i + 1);
        exx2bs[i] = LoadTexture(path);
        snprintf(path, sizeof(path), "resource/textures/blockex/exyell%d.png", i + 1);
        exyell[i] = LoadTexture(path);
    }
    for (int i = 0; i < 4; i++) {
        char path[128];
        snprintf(path, sizeof(path), "resource/textures/blockex/exdeath%d.png", i + 1);
        exdeath[i] = LoadTexture(path);
    }

    /* Load presents textures */
    earth = LoadTexture("resource/textures/presents/earth.png");
    flag = LoadTexture("resource/textures/presents/flag.png");
    justin = LoadTexture("resource/textures/presents/justin.png");
    kibell = LoadTexture("resource/textures/presents/kibell.png");
    presents = LoadTexture("resource/textures/presents/presents.png");
}

void Assets_Cleanup(void)
{
    /* Unload title textures */
    UnloadTexture(titleX);
    UnloadTexture(titleB);
    UnloadTexture(titleO);
    UnloadTexture(titleI);
    UnloadTexture(titleN);
    UnloadTexture(titleG);
    UnloadTexture(titleBig);
    UnloadTexture(titleSml);

    /* Unload UI textures */
    UnloadTexture(icon);
    UnloadTexture(mouse);
    UnloadTexture(question);
    UnloadTexture(text);
    UnloadTexture(larrow);
    UnloadTexture(rarrow);
    UnloadTexture(highscr);
    UnloadTexture(floppy);

    /* Unload ball textures */
    UnloadTexture(ball1);
    UnloadTexture(ball2);
    UnloadTexture(ball3);
    UnloadTexture(ball4);
    UnloadTexture(killer);
    UnloadTexture(life);
    for (int i = 0; i < 8; i++) {
        UnloadTexture(bbirth[i]);
    }

    /* Unload block textures */
    UnloadTexture(blakblk);
    UnloadTexture(blakblkH);
    UnloadTexture(blueblk);
    UnloadTexture(bombblk);
    UnloadTexture(grnblk);
    UnloadTexture(purpblk);
    UnloadTexture(redblk);
    UnloadTexture(tanblk);
    UnloadTexture(yellblk);
    UnloadTexture(cntblk);
    UnloadTexture(cntblk1);
    UnloadTexture(cntblk2);
    UnloadTexture(cntblk3);
    UnloadTexture(cntblk4);
    UnloadTexture(cntblk5);
    UnloadTexture(stkyblk);
    UnloadTexture(hypspc);
    UnloadTexture(dynamite);
    UnloadTexture(roamer);
    UnloadTexture(roamerD);
    UnloadTexture(roamerL);
    UnloadTexture(roamerR);
    UnloadTexture(roamerU);
    for (int i = 0; i < 5; i++) {
        UnloadTexture(death[i]);
    }

    /* Unload bonus textures */
    for (int i = 0; i < 4; i++) {
        UnloadTexture(bonus[i]);
        UnloadTexture(x2bonus[i]);
        UnloadTexture(x4bonus[i]);
    }
    UnloadTexture(clock);
    UnloadTexture(lotsammo);
    UnloadTexture(machgun);
    UnloadTexture(multibal);
    UnloadTexture(padexpn);
    UnloadTexture(padshrk);
    UnloadTexture(reverse);
    UnloadTexture(speed);
    UnloadTexture(walloff);
    UnloadTexture(xtrabal);
    UnloadTexture(xtrabal2);

    /* Unload paddle textures */
    UnloadTexture(padhuge);
    UnloadTexture(padmed);
    UnloadTexture(padsml);

    /* Unload eye dude textures */
    UnloadTexture(deveyes);
    UnloadTexture(deveyes1);
    UnloadTexture(deveyes2);
    UnloadTexture(deveyes3);
    UnloadTexture(deveyes4);
    UnloadTexture(deveyes5);
    UnloadTexture(guydead);
    for (int i = 0; i < 6; i++) {
        UnloadTexture(guyl[i]);
        UnloadTexture(guyr[i]);
    }

    /* Unload gun textures */
    UnloadTexture(bullet);
    UnloadTexture(tink);

    /* Unload star textures */
    for (int i = 0; i < 11; i++) {
        UnloadTexture(stars[i]);
    }

    /* Unload background textures */
    UnloadTexture(bgrnd);
    UnloadTexture(bgrnd2);
    UnloadTexture(bgrnd3);
    UnloadTexture(bgrnd4);
    UnloadTexture(bgrnd5);
    UnloadTexture(mnbgrnd);
    UnloadTexture(space);

    /* Unload guide textures */
    UnloadTexture(guide);
    UnloadTexture(guide1);
    UnloadTexture(guide2);
    UnloadTexture(guide3);
    UnloadTexture(guide4);
    UnloadTexture(guide5);
    UnloadTexture(guide6);
    UnloadTexture(guide7);
    UnloadTexture(guide8);
    UnloadTexture(guide9);
    UnloadTexture(guide10);
    UnloadTexture(guide11);

    /* Unload digit textures */
    for (int i = 0; i < 10; i++) {
        UnloadTexture(digits[i]);
    }

    /* Unload block explosion textures */
    for (int i = 0; i < 3; i++) {
        UnloadTexture(exblue[i]);
        UnloadTexture(exbomb[i]);
        UnloadTexture(excnt[i]);
        UnloadTexture(exgren[i]);
        UnloadTexture(expurp[i]);
        UnloadTexture(exred[i]);
        UnloadTexture(extan[i]);
        UnloadTexture(exx2bs[i]);
        UnloadTexture(exyell[i]);
    }
    for (int i = 0; i < 4; i++) {
        UnloadTexture(exdeath[i]);
    }

    /* Unload presents textures */
    UnloadTexture(earth);
    UnloadTexture(flag);
    UnloadTexture(justin);
    UnloadTexture(kibell);
    UnloadTexture(presents);
}
