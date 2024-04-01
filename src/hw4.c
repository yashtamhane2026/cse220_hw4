#include "hw4.h"

bool out_of_board(int src_row, int src_col, int dest_row, int dest_col);
bool no_interrupt_plus(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game);
bool no_interrupt_diagonal(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game);

void initialize_game(ChessGame *game) {
    //black others
    game->chessboard[0][0] = 'r';
    game->chessboard[0][1] = 'n';
    game->chessboard[0][2] = 'b';
    game->chessboard[0][3] = 'q';
    game->chessboard[0][4] = 'k';
    game->chessboard[0][5] = 'b';
    game->chessboard[0][6] = 'n';
    game->chessboard[0][7] = 'r';

    //white others
    game->chessboard[7][0] = 'R';
    game->chessboard[7][1] = 'N';
    game->chessboard[7][2] = 'B';
    game->chessboard[7][3] = 'Q';
    game->chessboard[7][4] = 'K';
    game->chessboard[7][5] = 'B';
    game->chessboard[7][6] = 'N';
    game->chessboard[7][7] = 'R';

    //black p
    for(int i = 0; i<8; i++){
        game->chessboard[1][i] = 'p';
    }

    //white p
    for(int i = 0; i<8; i++){
        game->chessboard[6][i] = 'P';
    }

    //all dots
    for(int i = 2; i<6; i++){
        for(int j = 0; j<8; j++){
            game->chessboard[i][j] = '.';
        }
    }

    game->moveCount = 0;
    game->capturedCount = 0;
    game->currentPlayer = 0; //0 for White and 1 for black
}

void chessboard_to_fen(char fen[], ChessGame *game) {
    (void)fen;
    (void)game;
}

bool is_valid_pawn_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    (void)piece;
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return false;
    // //check out of board
    // if(out_of_board(src_row, src_col, dest_row, dest_col)){
    //     return false;
    // }
    // //for black
    // if(piece == 'p'){
    //     //sw diag
    //     if((dest_row == src_row + 1) && (dest_col == src_col - 1)){
    //         if(game->chessboard[dest_row][dest_col] == '.'){
    //             return false;
    //         }
    //         return true;
    //     }
    //     //se diag
    //     if((dest_row == src_row + 1) && (dest_col == src_col + 1)){
    //         if(game->chessboard[dest_row][dest_col] == '.'){
    //             return false;
    //         }
    //         return true;
    //     }
    //     //s find
    //     if((dest_row == src_row + 1) && (dest_col == src_col)){
    //         if((game->chessboard[dest_row][dest_col]) != '.'){
    //             return false;
    //         }
    //         else{
    //             return true;
    //         }
    //     }
    //     if((dest_row == src_row + 2) && (dest_col == src_col)){
    //         if((game->chessboard[dest_row][dest_col]) != '.'){
    //             return false;
    //         }
    //         else  if((game->chessboard[src_row+1][src_col]) != '.'){
    //             return false;
    //         }
    //         else{
    //             return true;
    //         }
    //     }
    //     return false;
    // }

    // //for white
    // else if(piece == 'P'){
    //     //nw
    //     if((dest_row == src_row - 1) && (dest_col == src_col - 1)){
    //         if(game->chessboard[dest_row][dest_col] == '.'){
    //             return false;
    //         }
    //         return true;
    //     }
    //     //ne
    //     if((dest_row == src_row - 1) && (dest_col == src_col + 1)){
    //         if(game->chessboard[dest_row][dest_col] == '.'){
    //             return false;
    //         }
    //         return true;
    //     }
    //     //n find
    //     if((dest_row == src_row - 1) && (dest_col == src_col)){
    //         if((game->chessboard[dest_row][dest_col]) != '.'){
    //             return false;
    //         }
    //         else{
    //             return true;
    //         }
    //     }
    //     if((dest_row == src_row - 2) && (dest_col == src_col)){
    //         if((game->chessboard[dest_row][dest_col]) != '.'){
    //             return false;
    //         }
    //         else  if((game->chessboard[src_row-1][src_col]) != '.'){
    //             return false;
    //         }
    //         else{
    //             return true;
    //         }
    //     }
    //     return false;
    // }

    // else{
    //     return false;
    // }
}

bool is_valid_rook_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    //check whether it goes out of board
    if(out_of_board(src_row, src_col, dest_row, dest_col)){
        return false;
    }
    //check whether any piece interrupts (news)
    if(no_interrupt_plus(src_row, src_col, dest_row, dest_col, game)){
        return true;
    }
    return false;
}

bool is_valid_knight_move(int src_row, int src_col, int dest_row, int dest_col) {
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    return false;
    // //check out of board dest
    // if(out_of_board(src_row, src_col, dest_row, dest_col)){
    //     return false;
    // }

    // //8 cases
    // //nl
    // if((dest_row == src_row - 2) && (dest_col == src_col - 1)){
    //     return true;
    // }
    // //nr
    // else if((dest_row == src_row - 2) && (dest_col == src_col + 1)){
    //     return true;
    // }
    // //wd
    // else if((dest_row == src_row + 1) && (dest_col == src_col - 2)){
    //     return true;
    // }
    // //wu
    // else if((dest_row == src_row - 1) && (dest_col == src_col - 2)){
    //     return true;
    // }
    // //sl
    // else if((dest_row == src_row + 2) && (dest_col == src_col - 1)){
    //     return true;
    // }
    // //sr
    // else if((dest_row == src_row + 2) && (dest_col == src_col + 1)){
    //     return true;
    // }
    // //eu
    // else if((dest_row == src_row - 1) && (dest_col == src_col + 2)){
    //     return true;
    // }
    // //ed
    // else if((dest_row == src_row + 1) && (dest_col == src_col + 2)){
    //     return true;
    // }
    // else{
    //     return false;
    // }
}

bool is_valid_bishop_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    //check whether it goes out of board
    if(out_of_board(src_row, src_col, dest_row, dest_col)){
        return false;
    }
    //check whether any piece interrupts (nw, ne, se, sw)
    if(no_interrupt_diagonal(src_row, src_col, dest_row, dest_col, game)){
        return true;
    }
    return false;
}

bool is_valid_queen_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    //check out of board
    if(out_of_board(src_row, src_col, dest_row, dest_col)){
        return false;
    }
    //check if piece interrupts (news, ne, nw, se, sw)
    if((src_row == dest_row) || (src_col == dest_col)){
        if(no_interrupt_plus(src_row, src_col, dest_row, dest_col, game)){
            return true;
        }
        return false;
    }
    else{
        if(no_interrupt_diagonal(src_row, src_col, dest_row, dest_col, game)){
            return true;
        }
        return false;
    }
}

bool is_valid_king_move(int src_row, int src_col, int dest_row, int dest_col) {
    //check whether it goes out of board
    if(out_of_board(src_row, src_col, dest_row, dest_col)){
        return false;
    }
    //8 cases of moving
    //nw
    if((dest_row == src_row - 1) && (dest_col == src_col - 1)){
        return true;
    }
    //n
    else if((dest_row == src_row -1 ) && (dest_col == src_col)){
        return true;
    }
    //ne
    else if((dest_row == src_row - 1) && (dest_col == src_col + 1)){
        return true;
    }
    //w
    else if((dest_row == src_row) && (dest_col == src_col - 1)){
        return true;
    }
    //e
    else if((dest_row == src_row) && (dest_col == src_col + 1)){
        return true;
    }
    //sw
    else if((dest_row == src_row + 1) && (dest_col == src_col - 1)){
        return true;
    }
    //s
    else if((dest_row == src_row + 1) && (dest_col == src_col)){
        return true;
    }
    //se
    else if((dest_row == src_row + 1) && (dest_col == src_col + 1)){
        return true;
    }
    else{
        return false;
    }
    return false;
}

bool is_valid_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    //check whether it goes out of board
    if(out_of_board(src_row, src_col, dest_row, dest_col)){
        return false;
    }
    if(tolower(piece) == 'k'){
        return is_valid_king_move(src_row, src_col, dest_row, dest_col);
    }
    else if(tolower(piece) == 'r'){
        return is_valid_rook_move(src_row, src_col, dest_row, dest_col, game);
    }
    else if(tolower(piece) == 'b'){
        return is_valid_bishop_move(src_row, src_col, dest_row, dest_col, game);
    }
    else if(tolower(piece) == 'q'){
        return is_valid_queen_move(src_row, src_col, dest_row, dest_col, game);
    }
    else if(tolower(piece) == 'n'){
        return is_valid_knight_move(src_row, src_col, dest_row, dest_col);
    }
    else if(tolower(piece) == 'p'){
        return is_valid_pawn_move(piece, src_row, src_col, dest_row, dest_col, game);
    }
    else{
        return false;
    }
}

void fen_to_chessboard(const char *fen, ChessGame *game) {
    (void)fen;
    (void)game;
}

int parse_move(const char *move, ChessMove *parsed_move) {
    (void)move;
    (void)parsed_move;
    return -999;
}

int make_move(ChessGame *game, ChessMove *move, bool is_client, bool validate_move) {
    (void)game;
    (void)move;
    (void)is_client;
    (void)validate_move;
    return -999;
}

int send_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    (void)game;
    (void)message;
    (void)socketfd;
    (void)is_client;
    return -999;
}

int receive_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    (void)game;
    (void)message;
    (void)socketfd;
    (void)is_client;
    return -999;
}

int save_game(ChessGame *game, const char *username, const char *db_filename) {
    (void)game;
    (void)username;
    (void)db_filename;
    return -999;
}

int load_game(ChessGame *game, const char *username, const char *db_filename, int save_number) {
    (void)game;
    (void)username;
    (void)db_filename;
    (void)save_number;
    return -999;
}

void display_chessboard(ChessGame *game) {
    printf("\nChessboard:\n");
    printf("  a b c d e f g h\n");
    for (int i = 0; i < 8; i++) {
        printf("%d ", 8 - i);
        for (int j = 0; j < 8; j++) {
            printf("%c ", game->chessboard[i][j]);
        }
        printf("%d\n", 8 - i);
    }
    printf("  a b c d e f g h\n");
}

//Check whether dest is out of board
bool out_of_board(int src_row, int src_col, int dest_row, int dest_col){
    if((dest_row > 7) || (dest_col > 7)){
        return true;
    }
    else if((dest_row < 0 || dest_col < 0)){
        return true;
    }
    else if((src_row < 0 || src_col < 0)){
        return true;
    }
    else if((src_row < 0 || src_col < 0)){
        return true;
    }
    else{
        return false;
    }
    return false;
}

//up_down, right_left piece interrupt checking
bool no_interrupt_plus(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game){
    //4 cases
    //n
    if((dest_row<src_row) && (dest_col == src_col)){
        for(int i = src_row-1; i>dest_row; i--){
            if(game->chessboard[i][dest_col] != '.'){
                //if any word interrupts
                return false;
            }
        }
        return true;
    }
    //w
    else if((dest_row == src_row) && (dest_col < src_col)){
        for(int i = src_col-1; i>dest_col; i--){
            if(game->chessboard[dest_row][i] != '.'){
                //if any word interrupts
                return false;
            }
        }
        return true;
    }
    //s
    else if((dest_row>src_row) && (dest_col == src_col)){
        for(int i = src_row+1; i<dest_row; i++){
            if(game->chessboard[i][dest_col] != '.'){
                //if any word interrupts
                return false;
            }
        }
        return true;
    }
    //e
    else if((dest_row == src_row) && (dest_col > src_col)){
        for(int i = src_col+1; i<dest_col; i++){
            if(game->chessboard[dest_row][i] != '.'){
                //if any word interrupts
                return false;
            }
        }
        return true;
    }
    else{
        return false;
    }
}

//diagonal interrupt checking
bool no_interrupt_diagonal(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game){
    //4 cases
    //nw
    if((dest_row <= src_row - 1) && (dest_col <= src_col - 1)){
        for(int i = src_row-1, j = src_col-1; i>dest_row && j>dest_col; i--, j--){
            if(game->chessboard[i][j] != '.'){
                //if any word interrupts
                return false;
            }
        }
        return true;
    }
    //sw
    else if((dest_row >= src_row + 1) && (dest_col <= src_col - 1)){
        for(int i = src_row+1, j = src_col-1; i<dest_row && j>dest_col; i++, j--){
            if(game->chessboard[i][j] != '.'){
                //if any word interrupts
                return false;
            }
        }
        return true;
    }
    //ne
    else if((dest_row <= src_row - 1) && (dest_col >= src_col + 1)){
        for(int i = src_row-1, j = src_col+1; i>dest_row && j<dest_col; i--, j++){
            if(game->chessboard[i][j] != '.'){
                //if any word interrupts
                return false;
            }
        }
        return true;
    }
    //se
    else if((dest_row >= src_row + 1) && (dest_col >= src_col + 1)){
        for(int i = src_row+1, j = src_col+1; i<dest_row && j<dest_col; i++, j++){
            if(game->chessboard[i][j] != '.'){
                //if any word interrupts
                return false;
            }
        }
        return true;
    }
    else{
        return false;
    }
}
