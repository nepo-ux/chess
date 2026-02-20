#include "stdio.h"
#include "stdlib.h"
#include "def.h"
#include <ctype.h>
#include "string.h"

void addBuff(char *move) {
    if(move[4] != 'q' && move[4] != 'r' && move[4] != 'b' && move[4] != 'n') move[4] = ' ';
}

int numberOfFigures(const _BOARD *pos) {
    int index;
    int num = 0;
    for(index = 0; index < BRD_SQ_NUM; ++index) {
        if(pos->pieces[index] != EMPTY && pos->pieces[index] != OFFBOARD) ++num;
    }
    return num;
}

int drawByFiftyMove(const _BOARD *pos) { //достигли правила 50 ходов.
    if(pos->fifty_move == FIFTY_MOVE_RULE) return 1;
    return 0;
}

int KingPosition(const _BOARD *pos, int color) {
    int index;
    int piece;
    for(index = 0; index < BRD_SQ_NUM; ++index) {
        piece = pos->pieces[index];
        if(piece == wK && color == WHITE) return index;
        if(piece == bK && color == BLACK) return index;
    }
    printf("cant find KingPos\n");
    return -1;
}


void PawnPromotion(_BOARD *pos, const char *move, int from_sq, int to_sq) {

    int piece = pos->pieces[from_sq];
    int color = PieceColor(piece);

    int promoted_piece;

    switch(move[4]) {
    case 'q': promoted_piece = (color == WHITE) ? wQ : bQ; break;
    case 'r': promoted_piece = (color == WHITE) ? wR : bR; break;
    case 'b': promoted_piece = (color == WHITE) ? wB : bB; break;
    case 'n': promoted_piece = (color == WHITE) ? wN : bN; break;
    default:
        printf("Something went wrong in PawnPromotion\n");
        break;
    }

    pos->pieces[to_sq] = promoted_piece;
    pos->pieces[from_sq] = EMPTY;
}

int MakeMove(_BOARD *pos, const char *move) {

    //printf("[%c]\n", move[4]);

    int move_behave = IsMoveValid(pos, move);

    int from_file = move[0] - 'a';
    int from_rank = move[1] - '1';

    int to_file = move[2] - 'a';
    int to_rank = move[3] - '1';

    int from_sq = FR2SQ(from_file, from_rank);
    int to_sq = FR2SQ(to_file, to_rank);

    int piece = pos->pieces[from_sq];

    int color;
    int delta_file;

    int before_num_of_pieces = numberOfFigures(pos); // дл€ проверки правила 50-ти ходов

    printf("\nmove behave %d\n", move_behave);


    switch(move_behave) {
    case 0:
        printf("Invalid move\n");
        return 0;
    case 1: //просто ход или съели
        //съели ладью
        if(pos->pieces[to_sq] == wR && to_sq == H1) pos->castlePerm = pos->castlePerm & 0xE;
        if(pos->pieces[to_sq] == wR && to_sq == A1) pos->castlePerm = pos->castlePerm & 0xD;
        if(pos->pieces[to_sq] == bR && to_sq == H8) pos->castlePerm = pos->castlePerm & 0xB;
        if(pos->pieces[to_sq] == bR && to_sq == A8) pos->castlePerm = pos->castlePerm & 0x7;

        //ничего необычного
        pos->pieces[to_sq] = pos->pieces[from_sq];
        pos->pieces[from_sq] = EMPTY;

        pos->KingSq[pos->side] = KingPosition(pos, pos->side);
        break;
    case 2: // ситуаци€ когда по€вл€етс€ EnPass
        pos->pieces[to_sq] = pos->pieces[from_sq];
        pos->pieces[from_sq] = EMPTY;
        pos->enPassaunt = (pos->side == WHITE) ? from_sq + 10 : from_sq - 10;
        break;
    case 3: //¬з€тие на проходе
        pos->pieces[to_sq] = pos->pieces[from_sq];
        pos->pieces[from_sq] = EMPTY;

        if(pos->side == WHITE) {
            pos->pieces[to_sq - 10] = EMPTY;
        }
        if(pos->side == BLACK) {
            pos->pieces[to_sq + 10] = EMPTY;
        }
        break;
    case 4: //рокировка
        color = pos->side;
        delta_file = FilesBRD[to_sq] - FilesBRD[from_sq];

        pos->pieces[to_sq] = pos->pieces[from_sq];
        pos->pieces[from_sq] = EMPTY;
        pos->KingSq[color] = KingPosition(pos, pos->side);

        if(color == WHITE && delta_file == 2) {
            pos->pieces[to_sq - 1] = pos->pieces[to_sq + 1];
            pos->pieces[to_sq + 1] = EMPTY;
            pos->castlePerm = pos->castlePerm & 0xC;
        }
        if(color == WHITE && delta_file == -2) {
            pos->pieces[from_sq - 1] = pos->pieces[to_sq - 2];
            pos->pieces[to_sq - 2] = EMPTY;
            pos->castlePerm = pos->castlePerm & 0xC;
        }
        if(color == BLACK && delta_file == 2) {
            pos->pieces[to_sq - 1] = pos->pieces[to_sq + 1];
            pos->pieces[to_sq + 1] = EMPTY;
            pos->castlePerm = pos->castlePerm & 0x3;
        }
        if(color == BLACK && delta_file == -2) {
            pos->pieces[from_sq - 1] = pos->pieces[to_sq - 2];
            pos->pieces[to_sq - 2] = EMPTY;
            pos->castlePerm = pos->castlePerm & 0x3;
        }
        break;
    case 5: //трансформаци€ пешки
        PawnPromotion(pos, move, from_sq, to_sq);
        break;

    default:
        printf("Something went wrong in MakeMove\n");
        return 0;
    }

    int after_num_of_pieces = numberOfFigures(pos); // проверка правила 50-ходов
    if((piece != wP && piece != bP) ^ (after_num_of_pieces != before_num_of_pieces)) {
            if(pos->side == BLACK) pos->fifty_move++;
        }
    else pos->fifty_move = 0;

    if(move_behave != 2) pos->enPassaunt = NO_SQ; // ≈нѕассан пропадает на следующем ходу

    return 1;
}




int ParseFen(char *fen, _BOARD *pos) {

    int rank = RANK_8;
    int file = FILE_A;
    int piece = 0;
    int count = 0;
    int i = 0;
    int sq64 = 0;
    int sq120 = 0;

    if (fen == NULL || pos == NULL) {
        printf("FEN or pos is NULL\n");
        return -1;
    }
    ResetBoard(pos);

    while ((rank >= RANK_1) && *fen != ' ') {
        count = 1;

        if(isdigit(*fen)) {
            piece = EMPTY;
            count = *fen - '0';
        } else {

            switch (*fen) {
                case 'p': piece = bP; break;
                case 'r': piece = bR; break;
                case 'n': piece = bN; break;
                case 'b': piece = bB; break;
                case 'k': piece = bK; break;
                case 'q': piece = bQ; break;
                case 'P': piece = wP; break;
                case 'R': piece = wR; break;
                case 'N': piece = wN; break;
                case 'B': piece = wB; break;
                case 'K': piece = wK; break;
                case 'Q': piece = wQ; break;

                case '/':
                    if (file != 8) {
                        printf("FEN ERROR: not enough squares in rank %d\n", 8 - rank);
                        return -1;
                    }
                    rank--;
                    file = FILE_A;
                    fen++;
                    continue;
                case ' ':
                    if (file != 8) {
                        printf("FEN ERROR: not enough squares in rank %d (end of position)\n", 8 - rank);
                        return -1;
                    }
                    rank--;
                    file = FILE_A;
                    fen++;
                    break;

                default:
                    printf("FEN ERROR CHAR\n");
                    return -1;
            }
        }

        for(i = 0; i < count; ++i) {
            if (file >= FILE_A && file <= FILE_H && rank >= RANK_1 && rank <= RANK_8) {
                sq64 = rank * 8 + file;
                sq120 = SQ120(sq64);
                if(piece != EMPTY) {
                    pos->pieces[sq120] = piece;
                }
            }
            file++;
        }
        if (*fen != ' ') fen++;
    }

    fen++;

    if(*fen != 'w' && *fen != 'b') {
        printf("%c\n%c\n%c\n", fen[0], fen[1], fen[2]);
        printf("FEN ERROR INVALID SIDE\n");
        return -1;
    }

    pos->side = (*fen == 'w') ? WHITE : BLACK;
    fen += 2;

    while (*fen && *fen != ' ') {
        switch (*fen) {
            case 'K': pos->castlePerm |= WKCA; break;
            case 'Q': pos->castlePerm |= WQCA; break;
            case 'k': pos->castlePerm |= BKCA; break;
            case 'q': pos->castlePerm |= BQCA; break;
            case '-': break;
            default: printf("FEN ERROR: Invalid castling character\n"); return -1;
        }
        fen++;
    }
    fen++;

    if(*fen != '-') {
        if (fen[0] >= 'a' && fen[0] <= 'h' && fen[1] >= '1' && fen[1] <= '8') {
            file = fen[0] - 'a';
            rank = fen[1] - '1';
            pos->enPassaunt = FR2SQ(file, rank);
            fen += 3;
        } else {
            printf("FEN ERROR: Invalid en passant square\n");
            return -1;
        }
    } else {
        fen += 2;
        pos->enPassaunt = NO_SQ;
        //pos->fifty_move
    }

    pos->KingSq[WHITE] = KingPosition(pos, WHITE);
    pos->KingSq[BLACK] = KingPosition(pos, BLACK);

    return 0;
}

void ResetBoard(_BOARD *pos) {

    int index = 0;

    for(index = 0; index < BRD_SQ_NUM; ++index) {
        pos->pieces[index] = OFFBOARD;
    }

    for(index = 0; index < 64; ++index) {
        pos->pieces[SQ120(index)] = EMPTY;
    }


    pos->KingSq[WHITE] = NO_SQ;
    pos->KingSq[BLACK] = NO_SQ;

    pos->side = BOTH;
    pos->enPassaunt = NO_SQ;
    pos->fifty_move = 0;

    pos->castlePerm = 0;
    pos->hisPly = 0;
}

void PrintBoard(const _BOARD *pos) {
    int sq, file, rank, piece;

    printf("          Game board\n\n");

    for(rank = RANK_8; rank >= RANK_1; rank--) {
        printf("%d ", rank + 1);
        for(file = FILE_A; file <= FILE_H; ++file) {
            sq = FR2SQ(file,rank);
            piece = pos->pieces[sq];
            printf("%3c", PieceChar[piece]);
        }
        printf("\n");
    }

    printf("\n  ");
    for(file = FILE_A; file <= FILE_H; ++file) {
        printf("%3c", 'a'+file);
    }
    printf("\n\n");
    printf("side: %c\n", SideChar[pos->side]);
    printf("enPass120: %d\n", pos->enPassaunt);
    printf("castle: %c%c%c%c\n",
                                pos->castlePerm & WKCA ? 'K' : '-',
                                pos->castlePerm & WQCA ? 'Q' : '-',
                                pos->castlePerm & BKCA ? 'k' : '-',
                                pos->castlePerm & BQCA ? 'q' : '-'
                                );
    printf("Number of moves without pawn moves and capturing pieces: %d\n", pos->fifty_move);
}



char* PrintBoardToString(const _BOARD *pos) {
    char *buffer = (char*)malloc(1028);
    if (!buffer) return NULL;
    memset(buffer, 0, 1028 * sizeof(char));

    size_t dyn_size = 1028 * sizeof(char);

    int offset = 0;
    //сторона
    offset += snprintf(buffer + offset, dyn_size - offset, "SIDE:%s", pos->side == WHITE ? "WHITE" : "BLACK");
    // заголовок
    offset += snprintf(buffer + offset, dyn_size - offset, "     Game board\n\n");

    // генераци€ доски
    for (int rank = 7; rank >= 0; rank--) {
        offset += snprintf(buffer + offset, dyn_size - offset, "%d ", rank + 1);
        for (int file = 0; file < 8; file++) {
            int sq = FR2SQ(file, rank);
            int piece = pos->pieces[sq];

            offset += snprintf(buffer + offset, dyn_size - offset, "%3c", PieceChar[piece]);
        }
        offset += snprintf(buffer + offset, dyn_size - offset, "\n");
    }

    // буквы файлов
    offset += snprintf(buffer + offset, dyn_size - offset, "\n  ");
    for (int file = 0; file < 8; file++) {
        offset += snprintf(buffer + offset, dyn_size - offset, "%3c", 'a' + file);
    }

    return buffer;
}
