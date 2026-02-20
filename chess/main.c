#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include "def.h"


int main()
{
    //server();

    AllInit();

    _BOARD board[1];

    ParseFen(START_FEN, board);
    PrintBoard(board);

    int mate;
    int subDraw = 0;
    char *buff = (char*) malloc(5 * sizeof(char));


    _BOARD tmp_board[1];

    while(1) {
        memcpy(tmp_board, board, sizeof(_BOARD));
        mate = isLegalMoveAvailable(board);

        if(isLegalMoveAvailable(board)) {
            printf("There is legal move for [%s]- %d\n", (board->side == WHITE) ? "WHITE" : "BLACK", mate);

        }
        if((!isLegalMoveAvailable(board) && !IsKingInCheck(board))) {
            printf("THE WINNER IS %s\n", (board->side == WHITE) ? "BLACK": "WHITE");
            break;
        }

        else if(!isLegalMoveAvailable(board)) {
            printf("Stalemate!!! That`s draw\n");
            break;
        }


        printf("Your move: ");
        scanf("%s", buff);

        if(strcmp(buff, "draw") == 0) {
            printf("The %s offered draw. (Agree - 1, Decline - 0): ", (board->side == WHITE) ? "WHITE" : "BLACK");
            scanf("%d", &subDraw);
            if(subDraw) {
                printf("game over with draw by agreement\n");
                break;
            }
            else continue;
        }

        addBuff(buff);

        if (!MakeMove(tmp_board, buff)) {
            printf("INVALID MOVE\n");
            continue;
        }
        if (!IsKingInCheck(tmp_board)) {
            printf("INVALID_MOVE YOUR KING IS UNDER ATTACK\n");
            continue;
        }
        memcpy(board, tmp_board, sizeof(_BOARD));

        board->posKey = GeneratePosKey(board);
        SavePositionToHistory(board);

        if(CountRepetitions(board)) {
            printf("You can submit draw by 3rep rule (Y - 1/ N - 0): ");
            scanf("%d", &subDraw);
            if(subDraw) {
                printf("game end with draw by 3rep rule\n");
                break;
            }
        }
        if(drawByFiftyMove(board)) {
            printf("You can submit draw by 50 moves rule (Y - 1/N - 0): ");
            scanf("%d", &subDraw);
            if(subDraw) {
                printf("game end with draw by 50 moves rule\n");
                break;
            }
        }


        /*for(int i = 0; i <= board->hisPly; ++i) {
            printf("[%llX] , ", board->history[i]);
        }*/

        board->side = (board->side == WHITE) ? BLACK : WHITE;

        system("clear");
        PrintBoard(board);
    }


    return 0;
}
