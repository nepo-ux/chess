#include "server.h"
#include "def.h"


int compare_addresses(SockAddrIn* a, SockAddrIn* b) {
    return a->sin_addr.s_addr == b->sin_addr.s_addr && a->sin_port == b->sin_port;
}

int server() {
    WSADATA wsa;
    SOCKET server_socket;
    SockAddrIn server_addr;
    SockAddrIn client_addr[2];
    int addr_len = sizeof(SockAddrIn);
    char buffer[BUFFER_SIZE];
    char chess_buffer[CHESS_MOVE_BUFFER_SIZE];
    int draw_offer_pending = -1;
    int draw_waiting_reply = 0; //1 - server waiting agree/decline

    int draw_by_rules50 = 0;
    int draw_by_rules3Rep = 0;

    AllInit();

    _BOARD board[1];
    _BOARD tmp_board[1];
    ParseFen(START_FEN, board);


    //char *board_str = PrintBoardToString(board);

    //printf("%s", board_str);


    printf("[SERVER] Starting...\n");
    WSAStartup(MAKEWORD(2, 2), &wsa);

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("192.168.219.130"); //INADDR_ANY
    bind(server_socket, (SockAddr*)&server_addr, sizeof(server_addr));

    printf("[SERVER] Waiting for players...\n");
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &server_addr.sin_addr, ip_str, INET_ADDRSTRLEN);
    printf("Server running on: %s:%d\n", ip_str, ntohs(server_addr.sin_port));
    recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (SockAddr*)&client_addr[0], &addr_len);
    sendto(server_socket, "WHITE", 5, 0, (SockAddr*)&client_addr[0], addr_len);

    recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (SockAddr*)&client_addr[1], &addr_len);
    sendto(server_socket, "BLACK", 5, 0, (SockAddr*)&client_addr[1], addr_len);

    printf("[SERVER] Both players connected. Game starts!\n");
    printf("WHITE: %s:%d\n", inet_ntoa(client_addr[0].sin_addr), ntohs(client_addr[0].sin_port));
    printf("BLACK: %s:%d\n", inet_ntoa(client_addr[1].sin_addr), ntohs(client_addr[1].sin_port));


    char *board_str = PrintBoardToString(board);

    if (!board_str) {
        printf("[SERVER] Failed to generate board string.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    for (int i = 0; i < 2; ++i) {
        sendto(server_socket, board_str, strlen(board_str) + 1, 0, (SockAddr*)&client_addr[i], addr_len);
    }

    while (1) {
        SockAddrIn sender_addr;
        addr_len = sizeof(SockAddrIn);

        memset(chess_buffer, 0, CHESS_MOVE_BUFFER_SIZE * sizeof(char));

        int bytes_received = recvfrom(server_socket, chess_buffer, CHESS_MOVE_BUFFER_SIZE, 0, (SockAddr*)&sender_addr, &addr_len);
        if (bytes_received <= 0) continue;

        int sender_index = -1;
        for (int i = 0; i < 2; ++i) {
            if (compare_addresses(&sender_addr, &client_addr[i])) {
                sender_index = i;
                break;
            }
        }

        if (sender_index == -1) {
            printf("[SERVER] Unknown sender. Ignoring.\n");
            continue;
        }
        printf("RECEIVED FROM CLIENT: [%s]\n", chess_buffer);

        if(draw_by_rules3Rep) {

            if(strcmp(chess_buffer, "yes") == 0) {
                sendto(server_socket, "Game over with draw by three repetition rule.", 46, 0,
                            (SockAddr*)&client_addr[0], addr_len);
                sendto(server_socket, "Game over with draw by three repetition rule.", 46, 0,
                            (SockAddr*)&client_addr[1], addr_len);
                printf("DRAW BY 3 REPETITION. END\n");
                break;
            }

            if(strcmp(chess_buffer, "no") == 0) {
                draw_by_rules3Rep = 0;
                printf("DRAW BY 3REP CANCELED\n");

                board->side ^= 1;

                char* board_str = PrintBoardToString(board);
                if (!board_str) {
                    printf("[SERVER] Failed to generate board after decline.\n");
                    break;
                }
                for (int i = 0; i < 2; ++i) {
                    sendto(server_socket, board_str, strlen(board_str) + 1, 0,
                            (SockAddr*)&client_addr[i], addr_len);
                    }
                free(board_str);
                continue;
            }
            sendto(server_socket, "Please type 'yes' or 'no'.", 33, 0,
                            (SockAddr*)&sender_addr, addr_len);
            continue;
         }

         if(draw_by_rules50) {

            if(strcmp(chess_buffer, "yes") == 0) {
                sendto(server_socket, "Game over with draw by 50 moves rule.", 38, 0,
                            (SockAddr*)&client_addr[0], addr_len);
                sendto(server_socket, "Game over with draw by 50 moves rule.", 38, 0,
                            (SockAddr*)&client_addr[1], addr_len);
                printf("DRAW BY 50 MOVES. END\n");
                break;
            }

            if(strcmp(chess_buffer, "no") == 0) {
                draw_by_rules3Rep = 0;
                printf("DRAW BY 50 MOVES CANCELED\n");

                board->side ^= 1;

                char* board_str = PrintBoardToString(board);
                if (!board_str) {
                    printf("[SERVER] Failed to generate board after decline.\n");
                    break;
                }
                for (int i = 0; i < 2; ++i) {
                    sendto(server_socket, board_str, strlen(board_str) + 1, 0,
                            (SockAddr*)&client_addr[i], addr_len);
                    }
                free(board_str);
                continue;
            }
            sendto(server_socket, "Please type 'yes' or 'no'.", 33, 0,
                            (SockAddr*)&sender_addr, addr_len);
            continue;
         }


         if(draw_waiting_reply) {

            if (strcmp(chess_buffer, "agree") == 0) {
                sendto(server_socket, "Draw agreed. Game over.", 25, 0,
                            (SockAddr*)&client_addr[0], addr_len);
                sendto(server_socket, "Draw agreed. Game over.", 25, 0,
                            (SockAddr*)&client_addr[1], addr_len);
                printf("DRAW ACCEPTED. END\n");
                break;
            }

             if (strcmp(chess_buffer, "decline") == 0) {
                sendto(server_socket, "Draw declined. Continue playing.", 34, 0,
                            (SockAddr*)&client_addr[0], addr_len);
                sendto(server_socket, "Draw declined. Continue playing.", 34, 0,
                            (SockAddr*)&client_addr[1], addr_len);
                draw_offer_pending = -1;
                draw_waiting_reply = 0;
                printf("DRAW DECLINED. CONTINUE\n");

                char* board_str = PrintBoardToString(board);
                if (!board_str) {
                    printf("[SERVER] Failed to generate board after decline.\n");
                    break;
                }
                for (int i = 0; i < 2; ++i) {
                    sendto(server_socket, board_str, strlen(board_str) + 1, 0,
                            (SockAddr*)&client_addr[i], addr_len);
                    }
                free(board_str);
                continue;
            }
            sendto(server_socket, "Please type 'agree' or 'decline'.", 33, 0,
                            (SockAddr*)&sender_addr, addr_len);
            continue;
        }

        if (sender_index != board->side) {
            sendto(server_socket, "NOT YOUR TURN", 14, 0, (SockAddr*)&sender_addr, addr_len);
            continue;
        }

        addBuff(chess_buffer);

        printf("[SERVER] Move from %s: [%s]\n", board->side == WHITE ? "WHITE" : "BLACK", chess_buffer);

        if(strncmp(chess_buffer, "draw", 4) == 0 && draw_offer_pending == -1) {
            printf("some player offered draw\n");
            draw_offer_pending = sender_index;
            draw_waiting_reply = 1;
            sendto(server_socket, "Your opponent offered draw. (agree/decline)", 44, 0,
                    (SockAddr*)&client_addr[sender_index^1], addr_len);
            continue;
        }
        ///////
        memcpy(tmp_board, board, sizeof(_BOARD));

        if (!MakeMove(tmp_board, chess_buffer)) {
            sendto(server_socket, "INVALID_MOVE", 13, 0, (SockAddr*)&sender_addr, addr_len);
            continue;
        }
        if (!IsKingInCheck(tmp_board)) {
            sendto(server_socket, "INVALID_MOVE YOUR KING IS UNDER ATTACK", 39, 0, (SockAddr*)&sender_addr, addr_len);
            continue;
        }
        memcpy(board, tmp_board, sizeof(_BOARD));

        board->posKey = GeneratePosKey(board);
        SavePositionToHistory(board);

        tmp_board->side ^= 1;

        if(!isLegalMoveAvailable(tmp_board) && !IsKingInCheck(tmp_board)) { //ÌÀÒ
            if(tmp_board->side == WHITE) strcpy(buffer, "Black wins the game!");
            if(tmp_board->side == BLACK) strcpy(buffer, "White wins the game!");

            for (int i = 0; i < 2; ++i) {
                sendto(server_socket, buffer, strlen(buffer) + 1, 0, (SockAddr*)&client_addr[i], addr_len);
            }

            break;
        }
        if(!isLegalMoveAvailable(tmp_board)) { // ÏÀÒ
            strcpy(buffer, "Stalemate!!! That`s draw");
            for (int i = 0; i < 2; ++i) {
                    sendto(server_socket, buffer, 25, 0,
                            (SockAddr*)&client_addr[i], addr_len);
                    }
            break;
        }


        //////

        if(CountRepetitions(board)) {
            draw_by_rules3Rep = 1;
            sendto(server_socket, "You can draw this game by 3rep rule (yes/no)", 45, 0, (SockAddr*)&sender_addr, addr_len);
            continue;
        }

        if(drawByFiftyMove(board)) {
            draw_by_rules50 = 1;
            sendto(server_socket, "You can draw this game by 50move rule (yes/no)", 47, 0, (SockAddr*)&sender_addr, addr_len);
            continue;
        }

        board->side = (board->side == WHITE) ? BLACK : WHITE;

        board_str = PrintBoardToString(board);
        if (!board_str) {
            printf("[SERVER] ERROR: board_str is NULL\n");
            break;
        }

        for (int i = 0; i < 2; ++i) {
            sendto(server_socket, board_str, strlen(board_str) + 1, 0, (SockAddr*)&client_addr[i], addr_len);
        }
        free(board_str);
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}
