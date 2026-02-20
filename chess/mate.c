#include "def.h"
#include <string.h>


int isLegalMoveAvailable(const _BOARD *pos) {

    int index;
    int side = pos->side;
    int piece;

    int enemy = side ^ 1; //можно еще енеми съесть вообще

    _BOARD tmp_pos[1];

    for(index = 0; index < BRD_SQ_NUM; ++index) {

        memcpy(tmp_pos, pos, sizeof(_BOARD));

        piece = tmp_pos->pieces[index];
        if(piece == OFFBOARD || piece == EMPTY) continue;

        int color = PieceColor(piece);
        if(side != color) continue;

        if(piece == wQ || piece == bQ) { //провер€ет горизонтали дл€ queen

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            for(int i = 10; tmp_pos->pieces[index + i] == EMPTY || PieceColor(tmp_pos->pieces[index + i]) == enemy; i += 10) {
                tmp_pos->pieces[index + i] = (piece == wQ) ? wQ : bQ;
                tmp_pos->pieces[index + i - 10] = EMPTY; //заметает каждую клетку
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            for(int i = -10; tmp_pos->pieces[index + i] == EMPTY || PieceColor(tmp_pos->pieces[index + i]) == enemy; i -= 10) {
                tmp_pos->pieces[index + i] = (piece == wQ) ? wQ : bQ;
                tmp_pos->pieces[index + i + 10] = EMPTY; //заметает каждую клетку
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            for(int i = 1; tmp_pos->pieces[index + i] == EMPTY || PieceColor(tmp_pos->pieces[index + i]) == enemy; i += 1) {
                tmp_pos->pieces[index + i] = (piece == wQ) ? wQ : bQ;
                tmp_pos->pieces[index + i - 1] = EMPTY; //заметает каждую клетку
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            for(int i = -1; tmp_pos->pieces[index + i] == EMPTY || PieceColor(tmp_pos->pieces[index + i]) == enemy; i -= 1) {
                tmp_pos->pieces[index + i] = (piece == wQ) ? wQ : bQ;
                tmp_pos->pieces[index + i + 1] = EMPTY; //заметает каждую клетку
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));
                                                    //ѕ–ќ¬≈–я≈“ ƒ»ј√ќЌјЋ» ƒЋя QUEEN
            for(int i = 9; tmp_pos->pieces[index + i] == EMPTY || PieceColor(tmp_pos->pieces[index + i]) == enemy; i += 9) {
                tmp_pos->pieces[index + i] = (piece == wQ) ? wQ : bQ;
                tmp_pos->pieces[index + i - 9] = EMPTY; //заметает каждую клетку
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            for(int i = -9; tmp_pos->pieces[index + i] == EMPTY || PieceColor(tmp_pos->pieces[index + i]) == enemy; i -= 9) {
                tmp_pos->pieces[index + i] = (piece == wQ) ? wQ : bQ;
                tmp_pos->pieces[index + i + 9] = EMPTY; //заметает каждую клетку
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            for(int i = 11; tmp_pos->pieces[index + i] == EMPTY || PieceColor(tmp_pos->pieces[index + i]) == enemy; i += 11) {
                tmp_pos->pieces[index + i] = (piece == wQ) ? wQ : bQ;
                tmp_pos->pieces[index + i - 11] = EMPTY; //заметает каждую клетку
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            for(int i = -11; tmp_pos->pieces[index + i] == EMPTY || PieceColor(tmp_pos->pieces[index + i]) == enemy; i -= 11) {
                tmp_pos->pieces[index + i] = (piece == wQ) ? wQ : bQ;
                tmp_pos->pieces[index + i + 11] = EMPTY; //заметает каждую клетку
                if(IsKingInCheck(tmp_pos)) return 1;
            }
        }

        memcpy(tmp_pos, pos, sizeof(_BOARD));

        if(piece == wB || piece == bB) {//провер€ет диагонали дл€ bishop

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            for(int i = 9; tmp_pos->pieces[index + i] == EMPTY || PieceColor(tmp_pos->pieces[index + i]) == enemy; i += 9) {
                tmp_pos->pieces[index + i] = (piece == wB) ? wB : bB;
                tmp_pos->pieces[index + i - 9] = EMPTY; //заметает каждую клетку
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            for(int i = -9; tmp_pos->pieces[index + i] == EMPTY || PieceColor(tmp_pos->pieces[index + i]) == enemy; i -= 9) {
                tmp_pos->pieces[index + i] = (piece == wB) ? wB : bB;
                tmp_pos->pieces[index + i + 9] = EMPTY; //заметает каждую клетку
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            for(int i = 11; tmp_pos->pieces[index + i] == EMPTY || PieceColor(tmp_pos->pieces[index + i]) == enemy; i += 11) {
                tmp_pos->pieces[index + i] = (piece == wB) ? wB : bB;
                tmp_pos->pieces[index + i - 11] = EMPTY; //заметает каждую клетку
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            for(int i = -11; tmp_pos->pieces[index + i] == EMPTY || PieceColor(tmp_pos->pieces[index + i]) == enemy; i -= 11) {
                tmp_pos->pieces[index + i] = (piece == wB) ? wB : bB;
                tmp_pos->pieces[index + i + 11] = EMPTY; //заметает каждую клетку
                if(IsKingInCheck(tmp_pos)) return 1;
            }
        }

        memcpy(tmp_pos, pos, sizeof(_BOARD));

        if(piece == wR || piece == bR) { //√ќ–»«ќЌ“јЋ» ƒЋя ROOK

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            for(int i = 10; tmp_pos->pieces[index + i] == EMPTY || PieceColor(tmp_pos->pieces[index + i]) == enemy; i += 10) {
                tmp_pos->pieces[index + i] = (piece == wR) ? wR : bR;
                tmp_pos->pieces[index + i - 10] = EMPTY; //заметает каждую клетку
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            for(int i = -10; tmp_pos->pieces[index + i] == EMPTY || PieceColor(tmp_pos->pieces[index + i]) == enemy; i -= 10) {
                tmp_pos->pieces[index + i] = (piece == wR) ? wR : bR;
                tmp_pos->pieces[index + i + 10] = EMPTY; //заметает каждую клетку
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            for(int i = 1; tmp_pos->pieces[index + i] == EMPTY || PieceColor(tmp_pos->pieces[index + i]) == enemy; i += 1) {
                tmp_pos->pieces[index + i] = (piece == wR) ? wR : bR;
                tmp_pos->pieces[index + i - 1] = EMPTY; //заметает каждую клетку
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            for(int i = -1; tmp_pos->pieces[index + i] == EMPTY || PieceColor(tmp_pos->pieces[index + i]) == enemy; i -= 1) {
                tmp_pos->pieces[index + i] = (piece == wR) ? wR : bR;
                tmp_pos->pieces[index + i + 1] = EMPTY; //заметает каждую клетку
                if(IsKingInCheck(tmp_pos)) return 1;
            }
        }

        memcpy(tmp_pos, pos, sizeof(_BOARD));

        if(piece == wN || piece == bN) {

            if(tmp_pos->pieces[index + 19] == EMPTY || PieceColor(tmp_pos->pieces[index + 19]) == enemy) {
                tmp_pos->pieces[index + 19] = (piece == wN) ? wN : bN;
                tmp_pos->pieces[index] = EMPTY;
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            if(tmp_pos->pieces[index + 21] == EMPTY || PieceColor(tmp_pos->pieces[index + 21]) == enemy) {
                tmp_pos->pieces[index + 21] = (piece == wN) ? wN : bN;
                tmp_pos->pieces[index] = EMPTY;
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            if(tmp_pos->pieces[index + 12] == EMPTY || PieceColor(tmp_pos->pieces[index + 12]) == enemy) {
                tmp_pos->pieces[index + 12] = (piece == wN) ? wN : bN;
                tmp_pos->pieces[index] = EMPTY;
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            if(tmp_pos->pieces[index + 8] == EMPTY || PieceColor(tmp_pos->pieces[index + 8]) == enemy) {
                tmp_pos->pieces[index + 8] = (piece == wN) ? wN : bN;
                tmp_pos->pieces[index] = EMPTY;
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            if(tmp_pos->pieces[index - 12] == EMPTY || PieceColor(tmp_pos->pieces[index - 12]) == enemy) {
                tmp_pos->pieces[index - 12] = (piece == wN) ? wN : bN;
                tmp_pos->pieces[index] = EMPTY;
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            if(tmp_pos->pieces[index - 8] == EMPTY || PieceColor(tmp_pos->pieces[index - 8]) == enemy) {
                tmp_pos->pieces[index - 8] = (piece == wN) ? wN : bN;
                tmp_pos->pieces[index] = EMPTY;
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            if(tmp_pos->pieces[index - 19] == EMPTY || PieceColor(tmp_pos->pieces[index - 19]) == enemy) {
                tmp_pos->pieces[index - 19] = (piece == wN) ? wN : bN;
                tmp_pos->pieces[index] = EMPTY;
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            if(tmp_pos->pieces[index - 21] == EMPTY || PieceColor(tmp_pos->pieces[index - 21]) == enemy) {
                tmp_pos->pieces[index - 21] = (piece == wN) ? wN : bN;
                tmp_pos->pieces[index] = EMPTY;
                if(IsKingInCheck(tmp_pos)) return 1;
            }

        }

        memcpy(tmp_pos, pos, sizeof(_BOARD));

        if(piece == wP) {

            if(tmp_pos->pieces[index + 10] == EMPTY) {
                tmp_pos->pieces[index + 10] = wP;
                tmp_pos->pieces[index] = EMPTY;
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            if(index >= A2 && index <= H2) {
                tmp_pos->pieces[index + 20] = wP;
                tmp_pos->pieces[index] = EMPTY;
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            if(PieceColor(tmp_pos->pieces[index + 9]) == enemy || tmp_pos->enPassaunt == index + 9) {
                tmp_pos->pieces[index + 9] = wP;
                tmp_pos->pieces[index] = EMPTY;
                if(IsKingInCheck(tmp_pos)) return 1;
            }
            memcpy(tmp_pos, pos, sizeof(_BOARD));
            if(PieceColor(tmp_pos->pieces[index + 11]) == enemy || tmp_pos->enPassaunt == index + 11) {
                tmp_pos->pieces[index + 11] = wP;
                tmp_pos->pieces[index] = EMPTY;
                if(IsKingInCheck(tmp_pos)) return 1;
            }
        }

        memcpy(tmp_pos, pos, sizeof(_BOARD));

        if(piece == bP) {

            if(tmp_pos->pieces[index - 10] == EMPTY) {
                tmp_pos->pieces[index - 10] = bP;
                tmp_pos->pieces[index] = EMPTY;
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            if(index >= A7 && index <= H7) {
                tmp_pos->pieces[index - 20] = bP;
                tmp_pos->pieces[index] = EMPTY;
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            if(PieceColor(tmp_pos->pieces[index - 9]) == enemy || tmp_pos->enPassaunt == index - 9) {
                tmp_pos->pieces[index - 9] = bP;
                tmp_pos->pieces[index] = EMPTY;
                if(IsKingInCheck(tmp_pos)) return 1;
            }
            memcpy(tmp_pos, pos, sizeof(_BOARD));
            if(PieceColor(tmp_pos->pieces[index - 11]) == enemy || tmp_pos->enPassaunt == index - 11) {
                tmp_pos->pieces[index - 11] = bP;
                tmp_pos->pieces[index] = EMPTY;
                if(IsKingInCheck(tmp_pos)) return 1;
            }
        }

        memcpy(tmp_pos, pos, sizeof(_BOARD));

        if(piece == wK || piece == bK) {

            if(tmp_pos->pieces[index + 1] == EMPTY || PieceColor(tmp_pos->pieces[index + 1]) == enemy) {
                tmp_pos->pieces[index + 1] = (piece == wK) ? wK : bK;
                tmp_pos->pieces[index] = EMPTY;

                tmp_pos->KingSq[color] = KingPosition(tmp_pos, color);

                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            if(tmp_pos->pieces[index - 1] == EMPTY || PieceColor(tmp_pos->pieces[index - 1]) == enemy) {
                tmp_pos->pieces[index - 1] = (piece == wK) ? wK : bK;
                tmp_pos->pieces[index] = EMPTY;

                tmp_pos->KingSq[color] = KingPosition(tmp_pos, color);

                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            if(tmp_pos->pieces[index + 10] == EMPTY || PieceColor(tmp_pos->pieces[index + 10]) == enemy) {
                tmp_pos->pieces[index + 10] = (piece == wK) ? wK : bK;
                tmp_pos->pieces[index] = EMPTY;

                tmp_pos->KingSq[color] = KingPosition(tmp_pos, color);

                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            if(tmp_pos->pieces[index - 10] == EMPTY || PieceColor(tmp_pos->pieces[index - 10]) == enemy) {
                tmp_pos->pieces[index - 10] = (piece == wK) ? wK : bK;
                tmp_pos->pieces[index] = EMPTY;

                tmp_pos->KingSq[color] = KingPosition(tmp_pos, color);

                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            if(tmp_pos->pieces[index - 11] == EMPTY || PieceColor(tmp_pos->pieces[index - 11]) == enemy) {
                tmp_pos->pieces[index - 11] = (piece == wK) ? wK : bK;
                tmp_pos->pieces[index] = EMPTY;

                tmp_pos->KingSq[color] = KingPosition(tmp_pos, color);

                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            if(tmp_pos->pieces[index - 9] == EMPTY || PieceColor(tmp_pos->pieces[index - 9]) == enemy) {
                tmp_pos->pieces[index - 9] = (piece == wK) ? wK : bK;
                tmp_pos->pieces[index] = EMPTY;

                tmp_pos->KingSq[color] = KingPosition(tmp_pos, color);

                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            if(tmp_pos->pieces[index + 11] == EMPTY || PieceColor(tmp_pos->pieces[index + 11]) == enemy) {
                tmp_pos->pieces[index + 11] = (piece == wK) ? wK : bK;
                tmp_pos->pieces[index] = EMPTY;

                tmp_pos->KingSq[color] = KingPosition(tmp_pos, color);

                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            if(tmp_pos->pieces[index + 9] == EMPTY || PieceColor(tmp_pos->pieces[index + 9]) == enemy) {
                tmp_pos->pieces[index + 9] = (piece == wK) ? wK : bK;
                tmp_pos->pieces[index] = EMPTY;

                tmp_pos->KingSq[color] = KingPosition(tmp_pos, color);

                if(IsKingInCheck(tmp_pos)) return 1;
            }
        }

        memcpy(tmp_pos, pos, sizeof(_BOARD));

        if(piece == wK) {
            if(((tmp_pos->castlePerm >> 0) & 0x1) && IsPathClear(tmp_pos, index, index + 2)) {
                tmp_pos->pieces[index + 2] = wK;
                tmp_pos->pieces[index] = EMPTY;
                tmp_pos->KingSq[WHITE] = KingPosition(tmp_pos, WHITE);

                tmp_pos->pieces[index + 1] = wR;
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            if(((tmp_pos->castlePerm >> 1) & 0x1) && IsPathClear(tmp_pos, index, index - 2)) {
                tmp_pos->pieces[index - 2] = wK;
                tmp_pos->pieces[index] = EMPTY;
                tmp_pos->KingSq[WHITE] = KingPosition(tmp_pos, WHITE);

                tmp_pos->pieces[index - 1] = wR;
                if(IsKingInCheck(tmp_pos)) return 1;
            }
        }

        if(piece == bK) {
            if(((tmp_pos->castlePerm >> 2) & 0x1) && IsPathClear(tmp_pos, index, index + 2)) {
                tmp_pos->pieces[index + 2] = bK;
                tmp_pos->pieces[index] = EMPTY;
                tmp_pos->KingSq[BLACK] = KingPosition(tmp_pos, BLACK);

                tmp_pos->pieces[index + 1] = bR;
                if(IsKingInCheck(tmp_pos)) return 1;
            }

            memcpy(tmp_pos, pos, sizeof(_BOARD));

            if(((tmp_pos->castlePerm >> 3) & 0x1) && IsPathClear(tmp_pos, index, index - 2)) {
                tmp_pos->pieces[index - 2] = bK;
                tmp_pos->pieces[index] = EMPTY;
                tmp_pos->KingSq[BLACK] = KingPosition(tmp_pos, BLACK);

                tmp_pos->pieces[index - 1] = bR;
                if(IsKingInCheck(tmp_pos)) return 1;
            }
        }
    }

    return 0;
}
