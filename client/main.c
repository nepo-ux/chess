#include "client.h"

int main() {
    WSADATA wsa;
    SOCKET client_socket;
    SockAddrIn server_addr;
    int addr_len = sizeof(SockAddrIn);
    int endgame = 0;

    char chess_buffer[CHESS_MOVE_BUFFER_SIZE];
    char agreement_buffer[DRAW_BUFFER];

    WSAStartup(MAKEWORD(2, 2), &wsa);

    client_socket = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("192.168.219.130");

    sendto(client_socket, "connect", 7, 0, (SockAddr*)&server_addr, addr_len);

    char color_msg[6] = {0};
    recvfrom(client_socket, color_msg, 6, 0, (SockAddr*)&server_addr, &addr_len);
    Color my_color = strcmp(color_msg, "WHITE") == 0 ? WHITE : BLACK;
    printf("[CLIENT] I am %s\n", my_color == WHITE ? "WHITE" : "BLACK");

    while (1) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(client_socket, &readfds);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 500000; // 0.5 сек

        int activity = select(0, &readfds, NULL, NULL, &timeout);
        if (activity == SOCKET_ERROR) continue;

        if(FD_ISSET(client_socket, &readfds)) {
            char received_board[BUFFER_SIZE];
            memset(received_board, 0, BUFFER_SIZE * sizeof(char));
            recvfrom(client_socket, received_board, BUFFER_SIZE, 0, (SockAddr*)&server_addr, &addr_len);

            if(strstr(received_board, "wins the game!")) {//кому-то мат поставили
                printf("%s\n", received_board);
                endgame = 1;
                //break;
            }
            if(strstr(received_board, "Stalemate") || strstr(received_board, "Game over")) { //ничья
                printf("%s\n", received_board);                                              //3rep/50moves/пат
                endgame = 1;
                //break;
            }

            if(strncmp(received_board, "Your opponent offered draw. (agree/decline)", 44) == 0 ||
               strncmp(received_board, "Please type 'agree' or 'decline'.", 34) == 0 ||
               strncmp(received_board, "Please type 'yes' or 'no'.", 26) == 0) {
                printf("%s\n", received_board);

                fflush(stdout);
                memset(agreement_buffer, 0, DRAW_BUFFER);
                fflush(stdin);
                printf("Your choose: ");
                fgets(agreement_buffer, DRAW_BUFFER, stdin);
                agreement_buffer[strcspn(agreement_buffer, "\n")] = 0;
                sendto(client_socket, agreement_buffer, strlen(agreement_buffer), 0, (SockAddr*)&server_addr, addr_len);
                continue;

            }

            if (strncmp(received_board, "INVALID_MOVE", 13) == 0 ||
                strncmp(received_board, "NOT YOUR TURN", 14) == 0 ||
                strncmp(received_board, "INVALID_MOVE YOUR KING IS UNDER ATTACK", 39) == 0) {
                printf("%s\n", received_board);

                printf("Try again: ");
                fflush(stdout);
                memset(chess_buffer, 0, CHESS_MOVE_BUFFER_SIZE);
                fflush(stdin);
                fgets(chess_buffer, CHESS_MOVE_BUFFER_SIZE, stdin);
                chess_buffer[strcspn(chess_buffer, "\n")] = 0;

                sendto(client_socket, chess_buffer, strlen(chess_buffer), 0, (SockAddr*)&server_addr, addr_len);
                continue; // снова ждём ответ
            }

            if(strncmp(received_board, "You can draw this game by", 25) == 0) {
                printf("%s\n", received_board);

                fflush(stdout);
                memset(agreement_buffer, 0, DRAW_BUFFER);
                fflush(stdin);
                printf("Do you want to finish with draw? ");
                fgets(agreement_buffer, DRAW_BUFFER, stdin);
                agreement_buffer[strcspn(agreement_buffer, "\n")] = 0;
                sendto(client_socket, agreement_buffer, strlen(agreement_buffer), 0, (SockAddr*)&server_addr, addr_len);
                continue;
            }

            if(endgame) printf("You can leave now\n");

            else { //продолжаем играть, если игра не закончена.
                system("cls"); // очистка консоли
                printf("%s\n", received_board); // вывод доски

                char* turn_white = strstr(received_board, "SIDE:WHITE");
                char* turn_black = strstr(received_board, "SIDE:BLACK");

                if ((my_color == WHITE && turn_white) || (my_color == BLACK && turn_black)) {

                    printf("Your move: ");

                    fflush(stdout); // чтобы точно вывелось
                    fflush(stdin);  // очистка потока ввода

                    memset(chess_buffer, 0, CHESS_MOVE_BUFFER_SIZE);

                    fgets(chess_buffer, CHESS_MOVE_BUFFER_SIZE, stdin);
                    printf("%s", chess_buffer);
                    chess_buffer[strcspn(chess_buffer, "\n")] = 0; // remove newline[\n]
                    sendto(client_socket, chess_buffer, strlen(chess_buffer), 0, (SockAddr*)&server_addr, addr_len);
                } else {
                    printf("Waiting for opponent...\n");
                }
            }
        }
    }

    closesocket(client_socket);
    WSACleanup();


    return 0;
}
