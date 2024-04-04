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

int forward_dot_count(int row, int col, ChessGame *game){
    int count = 0;
    for(int i = col; i<8; i++){
        if((game->chessboard[row][i])=='.'){
            count = count + 1;
        }
        else{
            break;
        }
    }
    return count;
}

void chessboard_to_fen(char fen[], ChessGame *game) {
    int g = 0;
    for(int i = 0; i<8; i++,g++){
        for(int j = 0; j<8; j++,g++){
            if((game->chessboard[i][j]) == '.'){
                int count = forward_dot_count(i, j, game);
                fen[g] = count + '0';
                g = g + 1;
                j = j + count;
            }
            if(j>=8){
                break;
            }
            fen[g] = game->chessboard[i][j];
        }
        fen[g] = '/';
    }
    g = g - 1;
    fen[g] = ' ';
    g = g + 1;
    if((game->currentPlayer) == 0){
        fen[g] = 'w';
    }
    if((game->currentPlayer) == 1){
        fen[g] = 'b';
    }
    g = g + 1;
    fen[g] = '\0';
}

bool is_valid_pawn_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    //check out of board
    if(out_of_board(src_row, src_col, dest_row, dest_col)){
        return false;
    }
    //for black
    if(piece == 'p'){
        //sw diag
        if((dest_row == src_row + 1) && (dest_col == src_col - 1)){
            if(game->chessboard[dest_row][dest_col] == '.'){
                return false;
            }
            return true;
        }
        //se diag
        if((dest_row == src_row + 1) && (dest_col == src_col + 1)){
            if(game->chessboard[dest_row][dest_col] == '.'){
                return false;
            }
            return true;
        }
        //s find
        if((dest_row == src_row + 1) && (dest_col == src_col)){
            if((game->chessboard[dest_row][dest_col]) != '.'){
                return false;
            }
            else{
                return true;
            }
        }
        if((dest_row == src_row + 2) && (dest_col == src_col)){
            if((game->chessboard[dest_row][dest_col]) != '.'){
                return false;
            }
            else  if((game->chessboard[src_row+1][src_col]) != '.'){
                return false;
            }
            else{
                return true;
            }
        }
        return false;
    }

    //for white
    else if(piece == 'P'){
        //nw
        if((dest_row == src_row - 1) && (dest_col == src_col - 1)){
            if(game->chessboard[dest_row][dest_col] == '.'){
                return false;
            }
            return true;
        }
        //ne
        if((dest_row == src_row - 1) && (dest_col == src_col + 1)){
            if(game->chessboard[dest_row][dest_col] == '.'){
                return false;
            }
            return true;
        }
        //n find
        if((dest_row == src_row - 1) && (dest_col == src_col)){
            if((game->chessboard[dest_row][dest_col]) != '.'){
                return false;
            }
            else{
                return true;
            }
        }
        if((dest_row == src_row - 2) && (dest_col == src_col)){
            if((game->chessboard[dest_row][dest_col]) != '.'){
                return false;
            }
            else  if((game->chessboard[src_row-1][src_col]) != '.'){
                return false;
            }
            else{
                return true;
            }
        }
        return false;
    }

    else{
        return false;
    }
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
    //check out of board dest
    if(out_of_board(src_row, src_col, dest_row, dest_col)){
        return false;
    }

    //8 cases
    //nl
    if((dest_row == src_row - 2) && (dest_col == src_col - 1)){
        return true;
    }
    //nr
    else if((dest_row == src_row - 2) && (dest_col == src_col + 1)){
        return true;
    }
    //wd
    else if((dest_row == src_row + 1) && (dest_col == src_col - 2)){
        return true;
    }
    //wu
    else if((dest_row == src_row - 1) && (dest_col == src_col - 2)){
        return true;
    }
    //sl
    else if((dest_row == src_row + 2) && (dest_col == src_col - 1)){
        return true;
    }
    //sr
    else if((dest_row == src_row + 2) && (dest_col == src_col + 1)){
        return true;
    }
    //eu
    else if((dest_row == src_row - 1) && (dest_col == src_col + 2)){
        return true;
    }
    //ed
    else if((dest_row == src_row + 1) && (dest_col == src_col + 2)){
        return true;
    }
    else{
        return false;
    }
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
    if((is_valid_rook_move(src_row, src_col, dest_row, dest_col, game)) || (is_valid_bishop_move(src_row, src_col, dest_row, dest_col, game))){
        return true;
    }
    return false;
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
    int g = 0;
    for(int i = 0; i<8; i++,g++){//rows in game
        for(int j = 0; j<8; j++,g++){//column in game
            if((fen[g] >= '1') && (fen[g] <= '8')){
                int k = fen[g] - '0';
                while(k>0){
                    game->chessboard[i][j] = '.';
                    k = k - 1;
                    j = j + 1;
                }
                j = j - 1;
                continue;
            }
            game->chessboard[i][j] = fen[g];
        }
    }
    if(fen[g] == 'b'){
        game->currentPlayer = 1;
    }
    if(fen[g] == 'w'){
        game->currentPlayer = 0;
    }
}

int parse_move(const char *move, ChessMove *parsed_move) {
    //PARSE_MOVE_INVALID_FORMAT
    if(((int)strlen(move) != 4) && ((int)strlen(move) != 5)){
        return PARSE_MOVE_INVALID_FORMAT;
    }
    if((move[0] < 'a') || (move[0] > 'h')){
        return PARSE_MOVE_INVALID_FORMAT;
    }
    if((move[2] < 'a') || (move[2] > 'h')){
        return PARSE_MOVE_INVALID_FORMAT;
    }

    //PARSE_OUT_OF_BOUNDS
    if((move[1] < '1') || (move[1] > '8')){
        return PARSE_MOVE_OUT_OF_BOUNDS;
    }
    if((move[3] < '1') || (move[3] > '8')){
        return PARSE_MOVE_OUT_OF_BOUNDS;
    }

    //PARSE_MOVE_INVALID_DESTINATION
    if((int)strlen(move) == 5){
        if((move[3] != '1') && (move[3] != '8')){
            return PARSE_MOVE_INVALID_DESTINATION;
        }
    }

    //PARSE_MOVE_INVALID_PROMOTION
    if((int)strlen(move) == 5){
        if((move[4] != 'q') && (move[4] != 'r') && (move[4] != 'b') && (move[4] != 'n')){
            return PARSE_MOVE_INVALID_PROMOTION;
        }
    }

    //placing
    parsed_move->startSquare[0] = move[0];
    parsed_move->startSquare[1] = move[1];
    parsed_move->startSquare[2] = '\0';

    parsed_move->endSquare[0] = move[2];
    parsed_move->endSquare[1] = move[3];
    if((int)strlen(move)==5){
        parsed_move->endSquare[2] = move[4];
        parsed_move->endSquare[3] = '\0';
    }
    else{
        parsed_move->endSquare[2] = '\0';
    }
    return 0;
}

int make_move(ChessGame *game, ChessMove *move, bool is_client, bool validate_move) {

    
     if (validate_move) {
        // error checks go here
        //MOVING_OUT_OF_TURN
        if(game->currentPlayer == 0){
            if(is_client == false){
                return MOVE_OUT_OF_TURN;
            }
        }
        if(game->currentPlayer == 1){
            if(is_client == true){
                return MOVE_OUT_OF_TURN;
            }
        }
        //MOVE_NOTHING
        // a=0, b=1, c=2, d=3, e=4, f=5, g=6, h=7
        // 8 - (move[1] - '0')
        if((game->chessboard[8 - (move->startSquare[1] - '0')][(move->startSquare[0])-'a'])=='.'){
            return MOVE_NOTHING;
        }
        //MOVE_WRONG_COLOR
        if(is_client == true){
            if(islower((game->chessboard[8 - (move->startSquare[1] - '0')][(move->startSquare[0])-'a']))){
                return MOVE_WRONG_COLOR;
            }
        }
        if(is_client == false){
            if(isupper((game->chessboard[8 - (move->startSquare[1] - '0')][(move->startSquare[0])-'a']))){
                return MOVE_WRONG_COLOR;
            }
        }
        //MOVE_SUS
        if(is_client == true){
            if(isupper((game->chessboard[8 - (move->endSquare[1] - '0')][(move->endSquare[0])-'a']))){
                return MOVE_SUS;
            }
        }
        if(is_client == false){
            if(islower((game->chessboard[8 - (move->endSquare[1] - '0')][(move->endSquare[0])-'a']))){
                return MOVE_SUS;
            }
        }
        //MOVE_NOT_A_PAWN
        if((int)strlen(move->endSquare) == 3){
            if((game->chessboard[8 - (move->startSquare[1] - '0')][(move->startSquare[0])-'a']) != 'P'){
                if((game->chessboard[8 - (move->startSquare[1] - '0')][(move->startSquare[0])-'a']) != 'p'){
                    return MOVE_NOT_A_PAWN;
                }
            }
        }
        //MOVE_MISSING_PROMOTION
        if((int)strlen(move->endSquare) == 2){
            if(((move->endSquare[1] - '0') == 1) || ((move->endSquare[1] - '0') == 8)){
                if((game->chessboard[8 - (move->endSquare[1] - '0')][(move->endSquare[0])-'a']) == 'P'){
                    return MOVE_MISSING_PROMOTION;
                }
                if((game->chessboard[8 - (move->endSquare[1] - '0')][(move->endSquare[0])-'a']) == 'p'){
                    return MOVE_MISSING_PROMOTION;
                }
            }
        }
        //MOVE_WRONG
        int src_row = 8 - (move->startSquare[1] - '0');
        int src_col = (move->startSquare[0])-'a';
        int dest_row = 8 - (move->endSquare[1] - '0');
        int dest_col = (move->endSquare[0])-'a';
        char piece = (game->chessboard[8 - (move->startSquare[1] - '0')][(move->startSquare[0])-'a']); 
        if(is_valid_move(piece, src_row, src_col, dest_row, dest_col, game) == false){
            return MOVE_WRONG;
        }
    }
    //if 4 length
    if((int)strlen(move->endSquare) == 2){
        //Capture 
        int src_row = 8 - (move->startSquare[1] - '0');
        int src_col = (move->startSquare[0])-'a';
        int dest_row = 8 - (move->endSquare[1] - '0');
        int dest_col = (move->endSquare[0])-'a';
        char temp;
        if(game->chessboard[dest_row][dest_col] != '.'){
            temp = game->chessboard[dest_row][dest_col];
            //putting in capturedPieces
            game->capturedPieces[game->capturedCount] = temp;
            //updating capturedCOunt
            game->capturedCount = game->capturedCount + 1;
            //updating dest piece
            game->chessboard[dest_row][dest_col] = game->chessboard[src_row][src_col];
            //updating src piece
            game->chessboard[src_row][src_col] = '.';
            //updating moveCount
            game->moveCount = game->moveCount + 1;
            //updating currentPlayer
            if(is_client == true){
                game->currentPlayer = 1;
            }   
            else{
                game->currentPlayer = 0;
            }
        }
        //No capture
        else{   
            //updating dest piece
            game->chessboard[dest_row][dest_col] = game->chessboard[src_row][src_col];
            //updating src piece
            game->chessboard[src_row][src_col] = '.';
            //updating moveCount
            game->moveCount = game->moveCount + 1;
            //updating currentPlayer
            if(is_client == true){
                game->currentPlayer = 1;
            }   
            else{
                game->currentPlayer = 0;
            }
        }

    }
    //if length is 5
    else{
        //Capture
        int src_row = 8 - (move->startSquare[1] - '0');
        int src_col = (move->startSquare[0])-'a';
        int dest_row = 8 - (move->endSquare[1] - '0');
        int dest_col = (move->endSquare[0])-'a';
        char temp;
        if(game->chessboard[dest_row][dest_col] != '.'){
            temp = game->chessboard[dest_row][dest_col];
            //putting in capturedPieces
            game->capturedPieces[game->capturedCount] = temp;
            //updating capturedCOunt
            game->capturedCount = game->capturedCount + 1;
            //updating dest piece
            game->chessboard[dest_row][dest_col] = game->chessboard[src_row][src_col];
            //updatind src piece
            game->chessboard[src_row][src_col] = '.';
            //updating moveCount
            game->moveCount = game->moveCount + 1;
            //updating currentPlayer
            if(is_client == true){
                game->currentPlayer = 1;
            }   
            else{
                game->currentPlayer = 0;
            }
            //PROMOTION
            //for white
            if(is_client == true){
                char promotion = move->endSquare[2];
                if(game->chessboard[dest_row][dest_col] == 'P'){
                    promotion = toupper(promotion);
                    game->chessboard[dest_row][dest_col] = promotion;
                }
            }
            //for black
            else{
                char promotion = move->endSquare[2];
                if(game->chessboard[dest_row][dest_col] == 'p'){
                    promotion = tolower(promotion);
                    game->chessboard[dest_row][dest_col] = promotion;
                }
            }
        }
        //no capture
        else{
            //updating dest piece
            game->chessboard[dest_row][dest_col] = game->chessboard[src_row][src_col];
            //updating src piece
            game->chessboard[src_row][src_col] = '.';
            //updating moveCount
            game->moveCount = game->moveCount + 1;
            //updating currentPlayer
            if(is_client == true){
                game->currentPlayer = 1;
            }   
            else{
                game->currentPlayer = 0;
            }
            //PROMOTION
            //for white
            if(is_client == true){
                char promotion = move->endSquare[2];
                if(game->chessboard[dest_row][dest_col] == 'P'){
                    promotion = toupper(promotion);
                    game->chessboard[dest_row][dest_col] = promotion;
                }
            }
            //for black
            else{
                char promotion = move->endSquare[2];
                if(game->chessboard[dest_row][dest_col] == 'p'){
                    promotion = tolower(promotion);
                    game->chessboard[dest_row][dest_col] = promotion;
                }
            }
        }
    }
    return 0;
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
    //checking username
    for(int i = 0; i<(int)strlen(username); i++){
        if(username[i] == ' '){
            return -1;
        }
    }
    if((int)strlen(username) == 0){
        return -1;
    }
    char fen[BUFFER_SIZE];
    chessboard_to_fen(fen, game);
    FILE *file = fopen(db_filename, "a");
    fprintf(file, "%s:", username);
    fprintf(file, "%s\n", fen);
    fclose(file);
    return 0;
}

int load_game(ChessGame *game, const char *username, const char *db_filename, int save_number) {
    FILE *file = fopen(db_filename, "r");
    if(file == NULL){
        return -1;
    }
    char c;
    char temp_arr[BUFFER_SIZE];
    int line_number = 1, i = 0, j = 0;
    fseek(file, 0, SEEK_SET);
    while((c=fgetc(file))!=EOF){
        if(c=='\n'){
            line_number = line_number + 1;
            continue;
        }
        if(c==username[0]){
            i=1;
            while((c=fgetc(file))==username[i++]){
                if(i == ((int)strlen(username))){
                    c = fgetc(file);
                    if(c==':'){
                        if(line_number==save_number){
                            c = fgetc(file);
                            j = 0;
                            temp_arr[j] = c;
                            j = j + 1;
                            while((c=fgetc(file))!='\n'){
                                temp_arr[j] = c;
                                j = j + 1;
                            }
                            fen_to_chessboard(temp_arr, game);
                            return 0;
                        }
                    }
                }
            }
        }
    }
    return -1;
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
    else if((src_row > 7 || src_col > 7)){
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
    //check out of board
    if((out_of_board(src_row, src_col, dest_row, dest_col))){
        return false;
    }
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
    //check out of board
    if((out_of_board(src_row, src_col, dest_row, dest_col))){
        return false;
    }

    if(((abs(dest_row-src_row))!=abs(dest_col-src_col))){
        return false;
    }


    //4 cases
    //nw
    if((dest_row < src_row) && (dest_col < src_col)){
        for(int i = src_row-1, j = src_col-1; i>dest_row && j>dest_col; i--, j--){
            if(game->chessboard[i][j] != '.'){
                //if any word interrupts
                return false;
            }
        }
        return true;
    }
    //sw
    else if((dest_row > src_row) && (dest_col < src_col)){
        for(int i = src_row+1, j = src_col-1; i<dest_row && j>dest_col; i++, j--){
            if(game->chessboard[i][j] != '.'){
                //if any word interrupts
                return false;
            }
        }
        return true;
    }
    //ne
    else if((dest_row < src_row) && (dest_col > src_col)){
        for(int i = src_row-1, j = src_col+1; i>dest_row && j<dest_col; i--, j++){
            if(game->chessboard[i][j] != '.'){
                //if any word interrupts
                return false;
            }
        }
        return true;
    }
    //se
    else if((dest_row > src_row) && (dest_col > src_col)){
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
