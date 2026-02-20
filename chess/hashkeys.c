#include "def.h"
#include <stdlib.h>



U64 PieceKeys[13][120];
U64 SideKey;
U64 CastleKey[16];



U64 Rand64() {
    return (((U64)rand() <<  0) & 0x000000000000FFFFULL) |
           (((U64)rand() << 16) & 0x00000000FFFF0000ULL) |
           (((U64)rand() << 32) & 0x0000FFFF00000000ULL) |
           (((U64)rand() << 48) & 0xFFFF000000000000ULL);
}

U64 GeneratePosKey(const _BOARD *pos) {
    U64 finalKey = 0;
    int sq = 0;
    int piece = EMPTY;

    for (sq = 0; sq < BRD_SQ_NUM; ++sq) {
        piece = pos->pieces[sq];
        if (piece != EMPTY && piece != OFFBOARD) {
            finalKey ^= PieceKeys[piece][sq];
        }
    }

    if (pos->side == WHITE) {
        finalKey ^= SideKey;
    }

    if (pos->enPassaunt != NO_SQ) {
        finalKey ^= PieceKeys[EMPTY][pos->enPassaunt];
    }

    finalKey ^= CastleKey[pos->castlePerm];

    return finalKey;
}

int CountRepetitions(const _BOARD *pos) {
    int reps = 0;
    for (int i = 0; i < pos->hisPly; ++i) {
        if (pos->history[i] == pos->posKey) {
            reps++;
            if (reps == 3) return 1; // позиция встретилась 3 раза
        }
    }
    return 0;
}

void SavePositionToHistory(_BOARD *pos) {
    if (pos->hisPly < MAXMOVES) {
        pos->history[pos->hisPly] = pos->posKey;
        pos->hisPly++;
    }
}

