#include "../include/block.h"
#include "../include/assets.h"
#include <string.h>

BlockType Block_CharToType(char c)
{
    switch (c) {
        case 'r': return BLOCK_RED;
        case 'g': return BLOCK_GREEN;
        case 'b': return BLOCK_BLUE;
        case 't': return BLOCK_TAN;
        case 'p': return BLOCK_PURPLE;
        case 'y': return BLOCK_YELLOW;
        case 'w': return BLOCK_BLACK;
        case '0': return BLOCK_COUNTER_0;
        case '1': return BLOCK_COUNTER_1;
        case '2': return BLOCK_COUNTER_2;
        case '3': return BLOCK_COUNTER_3;
        case '4': return BLOCK_COUNTER_4;
        case '5': return BLOCK_COUNTER_5;
        case 'H': return BLOCK_HYPERSPACE;
        case 'B': return BLOCK_SPEED;
        case 'c': return BLOCK_MAXAMMO;
        case '+': return BLOCK_ROAMER;
        case 'X': return BLOCK_BOMB;
        case 'D': return BLOCK_DEATH;
        case 'L': return BLOCK_EXTRABALL;
        case 'M': return BLOCK_MACHINEGUN;
        case 'W': return BLOCK_WALLOFF;
        case '?': return BLOCK_RANDOM;
        case 'd': return BLOCK_DROP;
        case 'T': return BLOCK_TIMER;
        case 'm': return BLOCK_MULTIBALL;
        case 's': return BLOCK_STICKY;
        case 'R': return BLOCK_REVERSE;
        case '<': return BLOCK_PAD_SHRINK;
        case '>': return BLOCK_PAD_EXPAND;
        default:  return BLOCK_NONE;
    }
}

Texture2D Block_GetTexture(BlockType type)
{
    switch (type) {
        case BLOCK_RED:          return redblk;
        case BLOCK_GREEN:        return grnblk;
        case BLOCK_BLUE:         return blueblk;
        case BLOCK_TAN:          return tanblk;
        case BLOCK_PURPLE:       return purpblk;
        case BLOCK_YELLOW:       return yellblk;
        case BLOCK_BLACK:        return blakblk;
        case BLOCK_COUNTER_0:    return cntblk;
        case BLOCK_COUNTER_1:    return cntblk1;
        case BLOCK_COUNTER_2:    return cntblk2;
        case BLOCK_COUNTER_3:    return cntblk3;
        case BLOCK_COUNTER_4:    return cntblk4;
        case BLOCK_COUNTER_5:    return cntblk5;
        case BLOCK_HYPERSPACE:   return hypspc;
        case BLOCK_SPEED:        return speed;
        case BLOCK_MAXAMMO:      return lotsammo;
        case BLOCK_ROAMER:       return roamer;
        case BLOCK_BOMB:         return bombblk;
        case BLOCK_DEATH:        return death[0];
        case BLOCK_EXTRABALL:    return xtrabal;
        case BLOCK_MACHINEGUN:   return machgun;
        case BLOCK_WALLOFF:      return walloff;
        case BLOCK_TIMER:        return clock;
        case BLOCK_MULTIBALL:    return multibal;
        case BLOCK_STICKY:       return stkyblk;
        case BLOCK_REVERSE:      return reverse;
        case BLOCK_PAD_SHRINK:   return padshrk;
        case BLOCK_PAD_EXPAND:   return padexpn;
        case BLOCK_RANDOM:       return redblk;
        case BLOCK_DROP:         return grnblk;
        default:                 return redblk;
    }
}

void BlockGrid_Load(BlockGrid* grid, const Level* level, Rectangle bounds)
{
    if (!grid || !level) return;
    
    memset(grid, 0, sizeof(BlockGrid));
    
    int rows = Level_GetRows(level);
    int cols = Level_GetCols(level);
    
    /* Calculate cell dimensions (not block dimensions - blocks are centered in cells) */
    grid->cellWidth = (int)(bounds.width / cols);
    grid->cellHeight = (int)(bounds.height / (rows + 3)); /* +3 for paddle rows like src2 */
    
    const int BLOCK_WIDTH = 40;
    const int BLOCK_HEIGHT = 20;
    
    grid->count = 0;
    
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            char ch = Level_GetBlockAt(level, r, c);
            BlockType type = Block_CharToType(ch);
            
            if (type != BLOCK_NONE) {
                Block* blk = &grid->blocks[grid->count++];
                blk->type = type;
                blk->row = r;
                blk->col = c;
                blk->active = true;
                
                /* Calculate offset to center texture in cell - default for standard blocks */
                blk->offsetX = (grid->cellWidth - BLOCK_WIDTH) / 2;
                blk->offsetY = (grid->cellHeight - BLOCK_HEIGHT) / 2;
                
                /* Adjust offsets for non-standard block sizes based on src2 */
                Texture2D tex = Block_GetTexture(type);
                if (type == BLOCK_HYPERSPACE) {
                    blk->offsetX = (grid->cellWidth - 31) / 2;
                    blk->offsetY = (grid->cellHeight - 31) / 2;
                } else if (type == BLOCK_BLACK) {
                    blk->offsetX = (grid->cellWidth - 50) / 2;
                    blk->offsetY = (grid->cellHeight - 30) / 2;
                } else if (type == BLOCK_ROAMER) {
                    blk->offsetX = (grid->cellWidth - 25) / 2;
                    blk->offsetY = (grid->cellHeight - 27) / 2;
                } else if (type == BLOCK_BOMB || type == BLOCK_DEATH) {
                    blk->offsetX = (grid->cellWidth - 30) / 2;
                    blk->offsetY = (grid->cellHeight - 30) / 2;
                } else if (type == BLOCK_EXTRABALL) {
                    blk->offsetX = (grid->cellWidth - 30) / 2;
                    blk->offsetY = (grid->cellHeight - 19) / 2;
                } else if (type == BLOCK_MACHINEGUN) {
                    blk->offsetX = (grid->cellWidth - 35) / 2;
                    blk->offsetY = (grid->cellHeight - 15) / 2;
                } else if (type == BLOCK_WALLOFF) {
                    blk->offsetX = (grid->cellWidth - 27) / 2;
                    blk->offsetY = (grid->cellHeight - 23) / 2;
                } else if (type == BLOCK_TIMER) {
                    blk->offsetX = (grid->cellWidth - 21) / 2;
                    blk->offsetY = (grid->cellHeight - 21) / 2;
                } else if (type == BLOCK_STICKY) {
                    blk->offsetX = (grid->cellWidth - 32) / 2;
                    blk->offsetY = (grid->cellHeight - 27) / 2;
                } else if (type == BLOCK_REVERSE) {
                    blk->offsetX = (grid->cellWidth - 33) / 2;
                    blk->offsetY = (grid->cellHeight - 16) / 2;
                } else if (type == BLOCK_PAD_SHRINK || type == BLOCK_PAD_EXPAND) {
                    blk->offsetX = (grid->cellWidth - 40) / 2;
                    blk->offsetY = (grid->cellHeight - 15) / 2;
                }
            }
        }
    }
}

void BlockGrid_Draw(const BlockGrid* grid, Rectangle bounds)
{
    if (!grid) return;
    
    for (int i = 0; i < grid->count; i++) {
        const Block* blk = &grid->blocks[i];
        if (!blk->active) continue;
        
        Texture2D tex = Block_GetTexture(blk->type);
        
        /* Calculate position: cell position + offset to center texture */
        int x = (int)bounds.x + (blk->col * grid->cellWidth) + blk->offsetX;
        int y = (int)bounds.y + (blk->row * grid->cellHeight) + blk->offsetY;
        
        /* Draw texture at native size (no stretching) */
        DrawTexture(tex, x, y, WHITE);
    }
}

void BlockGrid_Free(BlockGrid* grid)
{
    if (!grid) return;
    /* Nothing to free currently, but keep for future extensions */
    grid->count = 0;
}
