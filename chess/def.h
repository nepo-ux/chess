#pragma once

#define BRD_SQ_NUM 120
#define MAXMOVES 1024
#define BUFFER_SIZE 1024
#define CHESS_MOVE_BUFFER_SIZE 8

#define FIFTY_MOVE_RULE 50

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

#define TEST_FEN "rnbqkbnr/ppppp1pp/8/8/5p2/1PNBQK2/PPPP1PPP/R4BNR w kq - 0 1"

#define CHECK_FEN "1n1qkb1r/P5pp/p7/1Q6/8/8/PP1P1PPP/RNBQKBNR b KQ - 0 1"
#define MATE_FEN "1kr5/p1p4P/1p6/2N2K2/8/8/6B1/8 w - - 0 1"
#define STALEMATE_FEN "7k/7P/2p5/1pP2K2/1P6/8/2B5/8 w - - 0 1"
#define PAWN_PROM_FEN "1n1qkb1r/P4ppp/8/8/8/6Q1/PP1P1PPP/RNBQKBNR w KQ - 0 1"



enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK, OFFBOARD };

enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE };
enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE };

enum { WHITE, BLACK, BOTH };

enum {
    A1 = 21, B1, C1, D1, E1, F1, G1, H1,
    A2 = 31, B2, C2, D2, E2, F2, G2, H2,
    A3 = 41, B3, C3, D3, E3, F3, G3, H3,
    A4 = 51, B4, C4, D4, E4, F4, G4, H4,
    A5 = 61, B5, C5, D5, E5, F5, G5, H5,
    A6 = 71, B6, C6, D6, E6, F6, G6, H6,
    A7 = 81, B7, C7, D7, E7, F7, G7, H7,
    A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ
    };

enum { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8}; //рокировка. 4 бита 0 0 0 0. Через короля(K), через королеву(Q)

typedef unsigned long long U64;

typedef struct BOARD {
    int pieces[BRD_SQ_NUM];

    int KingSq[2];

    int side;
    int enPassaunt;
    int fifty_move;

    int castlePerm;

    int hisPly;
    U64 history[MAXMOVES];
    U64 posKey;

} _BOARD;








//globals
extern int Sq120ToSq64[BRD_SQ_NUM];
extern int Sq64ToSq120[64];


extern char PieceChar[];
extern char SideChar[];
extern char RankChar[];
extern char FileChar[];


extern int FilesBRD[BRD_SQ_NUM];
extern int RanksBRD[BRD_SQ_NUM];

extern U64 PieceKeys[13][120];
extern U64 SideKey;
extern U64 CastleKey[16];


//macros
#define FR2SQ(f,r) ( (21 + (f) ) + ( (r) * 10 ) )
#define SQ64(sq120) (Sq120ToSq64[(sq120)])
#define SQ120(sq64) (Sq64ToSq120[(sq64)])


//function

void AllInit();

//board.c
void ResetBoard(_BOARD *pos);
int ParseFen(char *fen, _BOARD *pos);
void PrintBoard(const _BOARD *pos);
int MakeMove(_BOARD *pos, const char *move);
void addBuff(char *move);
int numberOfFigures(const _BOARD *pos);
char *PrintBoardToString(const _BOARD *pos);
int drawByFiftyMove(const _BOARD *pos);
int KingPosition(const _BOARD *pos, int color);


//validmove.c
PieceColor(int piece);
IsMoveValid(const _BOARD *pos, const char *move);

int IsPawnMoveValid(const _BOARD *pos, int from_sq, int to_sq);
int IsKnightMoveValid(int from_sq, int to_sq);
int IsBishopMoveValid(const _BOARD *pos, int from_sq, int to_sq);
int IsRookMoveValid(_BOARD *pos, int from_sq, int to_sq);
int IsQueenMoveValid(const _BOARD *pos, int from_sq, int to_sq);
int IsKingMoveValid(_BOARD *pos, int from_sq, int to_sq);
int IsPathClear(const _BOARD *pos, int from_sq, int to_sq);

//check.c
int IsKingInCheck(const _BOARD *pos);

//mate.c
int isLegalMoveAvailable(const _BOARD *pos);

//hashkeys.c
U64 Rand64();
U64 GeneratePosKey(const _BOARD *pos);
int CountRepetitions(const _BOARD *pos);
void SavePositionToHistory(_BOARD *pos);

int server();
