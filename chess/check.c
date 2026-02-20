#include "def.h"
#include <stdio.h>


int IsKingPathClear(const _BOARD *pos) {
    int color = pos->side;
    int KingSq = pos->KingSq[color];
    int index;
    int piece;

    //сначала проверить пешки  И enemy Короля
    if(color == WHITE) {

        index = KingSq + 11;
        if(pos->pieces[index] == bP || pos->pieces[index] == bK) return index;

        index = KingSq + 9;
        if(pos->pieces[index] == bP || pos->pieces[index] == bK) return index;

        //проверка всех соседних клеток на наличие enemy короля, кроме тех двух что проверены с пешками
        if(pos->pieces[KingSq - 1]  == bK) return KingSq - 1;
        if(pos->pieces[KingSq - 11] == bK) return KingSq - 11;
        if(pos->pieces[KingSq - 10] == bK) return KingSq - 10;
        if(pos->pieces[KingSq - 9]  == bK) return KingSq - 9;
        if(pos->pieces[KingSq + 1]  == bK) return KingSq + 1;
        if(pos->pieces[KingSq + 10] == bK) return KingSq + 10;
    }
    if(color == BLACK) {

        index = KingSq - 11;
        if(pos->pieces[index] == wP || pos->pieces[index] == wK) return index;

        index = KingSq - 9;
        if(pos->pieces[index] == wP || pos->pieces[index] == wK) return index;


        if(pos->pieces[KingSq - 1]  == wK) return KingSq - 1;
        if(pos->pieces[KingSq + 11] == wK) return KingSq + 11;
        if(pos->pieces[KingSq - 10] == wK) return KingSq - 10;
        if(pos->pieces[KingSq + 9]  == wK) return KingSq + 9;
        if(pos->pieces[KingSq + 1]  == wK) return KingSq + 1;
        if(pos->pieces[KingSq + 10] == wK) return KingSq + 10;
    }

    for(index = KingSq + 10; pos->pieces[index] != OFFBOARD; index += 10) { //ладья ферзь
        piece = pos->pieces[index];
        if(piece == EMPTY) continue;

        //printf("1 loop\n");

        if(color == WHITE && (piece == bQ || piece == bR)) return index;
        if(color == BLACK && (piece == wQ || piece == wR)) return index;
        else break; //встретили свою/enemy(конь, пешка) фигуру
    }
    for(index = KingSq - 10; pos->pieces[index] != OFFBOARD; index -= 10) { //ладья ферзь
        piece = pos->pieces[index];
        if(piece == EMPTY) continue;

        //printf("2 loop\n");

        if(color == WHITE && (piece == bQ || piece == bR)) return index;
        if(color == BLACK && (piece == wQ || piece == wR)) return index;
        else break; //встретили свою фигуру
    }
    for(index = KingSq + 1; pos->pieces[index] != OFFBOARD; index++) { //ладья ферзь
        piece = pos->pieces[index];
        if(piece == EMPTY) continue;

        //printf("3 loop\n");

        if(color == WHITE && (piece == bQ || piece == bR)) return index;
        if(color == BLACK && (piece == wQ || piece == wR)) return index;
        else break; //встретили свою фигуру
    }
    for(index = KingSq - 1; pos->pieces[index] != OFFBOARD; index--) { //ладья ферзь
        piece = pos->pieces[index];
        if(piece == EMPTY) continue;

        //printf("4 loop\n");

        if(color == WHITE && (piece == bQ || piece == bR)) return index;
        if(color == BLACK && (piece == wQ || piece == wR)) return index;
        else break; //встретили свою фигуру
    }
    for(index = KingSq + 11; pos->pieces[index] != OFFBOARD; index += 11) { //слон ферзь
        piece = pos->pieces[index];
        if(piece == EMPTY) continue;

        //printf("5 loop\n");

        if(color == WHITE && (piece == bQ || piece == bB)) return index;
        if(color == BLACK && (piece == wQ || piece == wB)) return index;
        else break; //встретили свою фигуру
    }
    for(index = KingSq + 9; pos->pieces[index] != OFFBOARD; index += 9) { //слон ферзь
        piece = pos->pieces[index];
        if(piece == EMPTY) continue;

        //printf("6 loop\n");

        if(color == WHITE && (piece == bQ || piece == bB)) return index;
        if(color == BLACK && (piece == wQ || piece == wB)) return index;
        else break; //встретили свою фигуру
    }
    for(index = KingSq - 11; pos->pieces[index] != OFFBOARD; index -= 11) { //слон ферзь
        piece = pos->pieces[index];
        if(piece == EMPTY) continue;

        //printf("7 loop\n");

        if(color == WHITE && (piece == bQ || piece == bB)) return index;
        if(color == BLACK && (piece == wQ || piece == wB)) return index;
        else break; //встретили свою фигуру
    }
    for(index = KingSq - 9; pos->pieces[index] != OFFBOARD; index -= 9) { //слон ферзь
        piece = pos->pieces[index];
        if(piece == EMPTY) continue;

       // printf("8 loop\n");

        if(color == WHITE && (piece == bQ || piece == bB)) return index;
        if(color == BLACK && (piece == wQ || piece == wB)) return index;
        else break; //встретили свою фигуру
    }

    //проверка есть ли атакующий конь
    if(color == WHITE) {
        if(pos->pieces[KingSq + 12] == bN) return KingSq + 12;
        if(pos->pieces[KingSq + 21] == bN) return KingSq + 21;
        if(pos->pieces[KingSq + 19] == bN) return KingSq + 19;
        if(pos->pieces[KingSq + 8]  == bN) return KingSq + 8;

        if(pos->pieces[KingSq - 12] == bN) return KingSq - 12;
        if(pos->pieces[KingSq - 21] == bN) return KingSq - 21;
        if(pos->pieces[KingSq - 19] == bN) return KingSq - 19;
        if(pos->pieces[KingSq - 8]  == bN) return KingSq - 8;
    }
    if(color == BLACK) {
        if(pos->pieces[KingSq + 12] == wN) return KingSq + 12;
        if(pos->pieces[KingSq + 21] == wN) return KingSq + 21;
        if(pos->pieces[KingSq + 19] == wN) return KingSq + 19;
        if(pos->pieces[KingSq + 8]  == wN) return KingSq + 8;

        if(pos->pieces[KingSq - 12] == wN) return KingSq - 12;
        if(pos->pieces[KingSq - 21] == wN) return KingSq - 21;
        if(pos->pieces[KingSq - 19] == wN) return KingSq - 19;
        if(pos->pieces[KingSq - 8]  == wN) return KingSq - 8;
    }

    return -1; //шаха нет
}
/*
void EnemyDetected(const _BOARD *pos, int index) {
    int piece = pos->pieces[index];
    switch(piece) {
    case wB: printf("Wb Det\n");
    case wR: printf("Wr det\n");
    case wQ: printf("wq det\n");
    case wP: printf("wp det\n");

    case bB: printf("bb det\n");
    case bR: printf("br det\n");
    case bQ: printf("bq det\n");
    case bP: printf("bp det\n");

    }
} */

int IsKingInCheck(const _BOARD *tmp_board) {

    int check = IsKingPathClear(tmp_board);
    if(check == -1) return 1; //шаха нет

    return 0; //король под шахом
}
