#include "def.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int PieceColor(int piece) {
    if(piece > 0 && piece <= 6) return WHITE;
    if(piece > 6 && piece <= 12) return BLACK;
    return -1;
}


int IsMoveValid(const _BOARD *pos, const char *buff) {
    char *move = malloc(CHESS_MOVE_BUFFER_SIZE * sizeof(char));
    memcpy(move, buff, CHESS_MOVE_BUFFER_SIZE * sizeof(char));
    printf("%s", move);
    if (strlen(move) < 4 || strlen(move) > 5) return 0;
    if (move[0] < 'a' || move[0] > 'h') return 0;
    if (move[1] < '1' || move[1] > '8') return 0;
    if (move[2] < 'a' || move[2] > 'h') return 0;
    if (move[3] < '1' || move[3] > '8') return 0;
    if (move[4] != 'q' && move[4] != 'r' && move[4] != 'b' && move[4] != 'n' && move[4] != ' ') return 0;

    int from_file = move[0] - 'a';
    int from_rank = move[1] - '1';
    int to_file = move[2] - 'a';
    int to_rank = move[3] - '1';
    int from_sq = FR2SQ(from_file, from_rank);
    int to_sq = FR2SQ(to_file, to_rank);

    // Фигура должна существовать и принадлежать текущему игроку
    int piece = pos->pieces[from_sq];
    int color = PieceColor(piece);
    if (piece == EMPTY || color != pos->side) {
            free(move);
            return 0;
    }

    //фигура не должна есть своих же
    int to_piece = pos->pieces[to_sq];
    int to_color = PieceColor(to_piece);
    if(color == to_color) {
            free(move);
            return 0;
    }

    //происходит ли трансформация пешки
    if((move[4] == 'q' || move[4] == 'r' || move[4] == 'b' || move[4] == 'n')
        && (to_rank == RANK_1 || to_rank == RANK_8) && IsPawnMoveValid(pos, from_sq, to_sq)) {
            free(move);
            return 5;
        }
    free(move);

    // Проверка правил для каждой фигуры
    switch (piece) {
        case wP: case bP: return IsPawnMoveValid(pos, from_sq, to_sq);
        case wN: case bN: return IsKnightMoveValid(from_sq, to_sq);
        case wB: case bB: return IsBishopMoveValid(pos, from_sq, to_sq);
        case wR: case bR: return IsRookMoveValid(pos, from_sq, to_sq);
        case wQ: case bQ: return IsQueenMoveValid(pos, from_sq, to_sq);
        case wK: case bK: return IsKingMoveValid(pos, from_sq, to_sq);
        default: return 0;
    }
}


int IsPawnMoveValid(const _BOARD *pos, int from_sq, int to_sq) { //получает sq120
    int color = pos->side;
    int forward = (color == WHITE) ? 10 : -10;
    int start_rank = (color == WHITE) ? RANK_2 : RANK_7;

    // Обычный ход на 1 клетку
    if (to_sq == from_sq + forward && pos->pieces[to_sq] == EMPTY) return 1;

    // Ход на 2 клетки с начальной позиции
    if (RanksBRD[from_sq] == start_rank &&
        to_sq == from_sq + 2 * forward &&
        pos->pieces[to_sq] == EMPTY &&
        pos->pieces[from_sq + forward] == EMPTY) return 2;

    // Взятие
    if ((to_sq == from_sq + forward + 1 || to_sq == from_sq + forward - 1) &&
        pos->pieces[to_sq] != EMPTY) return 1; //&& PieceColor(pos->pieces[to_sq]) != color

    // Взятие на проходе
    if((to_sq == from_sq + forward + 1 || to_sq == from_sq + forward - 1) && pos->enPassaunt == to_sq) return 3;

    return 0;
}

int IsKnightMoveValid(int from_sq, int to_sq) {
    int delta_file = FilesBRD[to_sq] - FilesBRD[from_sq];
    int delta_rank = RanksBRD[to_sq] - RanksBRD[from_sq];

    if  (  ((delta_file == -2) && (abs(delta_rank) == 1))
        || ((delta_file == -1) && (abs(delta_rank) == 2))
        || ((delta_file == 1)  && (abs(delta_rank) == 2))
        || ((delta_file == 2)  && (abs(delta_rank) == 1)) ) return 1;
    return 0;
}

int IsBishopMoveValid(const _BOARD *pos, int from_sq, int to_sq) {
    int delta_file = FilesBRD[to_sq] - FilesBRD[from_sq];
    int delta_rank = RanksBRD[to_sq] - RanksBRD[from_sq];
    if(abs(delta_file) != abs(delta_rank)) return 0;

    IsPathClear(pos, from_sq, to_sq);
}

int IsRookMoveValid(_BOARD *pos, int from_sq, int to_sq) {

    int delta_file = FilesBRD[to_sq] - FilesBRD[from_sq];
    int delta_rank = RanksBRD[to_sq] - RanksBRD[from_sq];
    if(abs(delta_file) != 0 && abs(delta_rank) != 0) return 0;

    if(from_sq == A1) pos->castlePerm = pos->castlePerm & 0xD;
    if(from_sq == H1) pos->castlePerm = pos->castlePerm & 0xE;
    if(from_sq == A8) pos->castlePerm = pos->castlePerm & 0x7;
    if(from_sq == H8) pos->castlePerm = pos->castlePerm & 0xB;

    IsPathClear(pos, from_sq, to_sq);
}

int IsQueenMoveValid(const _BOARD *pos, int from_sq, int to_sq) {
    IsPathClear(pos, from_sq, to_sq);
}

int IsKingMoveValid(_BOARD *pos, int from_sq, int to_sq) {
    int delta_file = FilesBRD[to_sq] - FilesBRD[from_sq];
    int delta_rank = RanksBRD[to_sq] - RanksBRD[from_sq];

    int color = pos->side;
    int enemy = color ^ 1;

    if(abs(delta_rank) > 1) return 0;

    if((abs(delta_file) == 1 || abs(delta_rank) == 1) && (pos->pieces[to_sq] == EMPTY ||
                                                          PieceColor(pos->pieces[to_sq] == enemy))) { //вообще еще съесть можно
        if(color == WHITE) pos->castlePerm = pos->castlePerm & 0xC;
        if(color == BLACK) pos->castlePerm = pos->castlePerm & 0x3;

        return 1;
    }

    int isCasle = 0;
    if(delta_file == 2 && pos->side == WHITE)  isCasle = (pos->castlePerm >> 0) & 0x1;
    if(delta_file == 2 && pos->side == BLACK)  isCasle = (pos->castlePerm >> 2) & 0x1;
    if(delta_file == -2 && pos->side == WHITE && pos->pieces[to_sq - 1] == EMPTY) isCasle = (pos->castlePerm >> 1) & 0x1;
    if(delta_file == -2 && pos->side == BLACK && pos->pieces[to_sq - 1] == EMPTY) isCasle = (pos->castlePerm >> 3) & 0x1;


    if(isCasle && IsPathClear(pos, from_sq, to_sq)) {
        return 4;
    };
}

int IsPathClear(const _BOARD *pos, int from_sq, int to_sq) {
    int delta_file = FilesBRD[to_sq] - FilesBRD[from_sq];
    int delta_rank = RanksBRD[to_sq] - RanksBRD[from_sq];
    int step_file = (delta_file != 0) ? delta_file / abs(delta_file) : 0;
    int step_rank = (delta_rank != 0) ? delta_rank / abs(delta_rank) : 0;

    int current_sq = from_sq;
    while (current_sq != to_sq) {
        current_sq += step_file + 10 * step_rank;
        if (current_sq != to_sq && pos->pieces[current_sq] != EMPTY) return 0;
    }
    return 1;
}
