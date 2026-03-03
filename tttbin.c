/*
    Made by:
        Grimmnoir

    Sources:
        https://www.geeksforgeeks.org/dsa/what-is-bitmasking/
        https://youtu.be/SLgZhpDsrfc?si=DZsNLHGgJXRineAX
        
    Compiler:
        https://www.programiz.com/cpp-programming/online-compiler/
*/

#include <stdio.h>

#define mask(n) (1U << n)

typedef int Board;


static const int win[9] = {
    0b000000111, // row 0
    0b000111000, // row 1
    0b111000000, // row 2
    0b001001001, // col 0
    0b010010010, // col 1
    0b100100100, // col 2
    0b100010001, // diag lt
    0b001010100  // diag rt
};

void printBoard(Board x, Board o) {
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            int i = mask(r * 3 + c); 
            if (x & i) printf("\033[31mX \033[0m");
            else if (o & i) printf("\033[34mO \033[0m");
            else printf("_ ");
        }
        printf("\n");
    }
    printf("\n");
}

int checkWin(Board b) {
    for (int i = 0; i < 8; i++)
        if ((b & win[i]) == win[i]) return 1;
    return 0;
}

int miniMax(Board x, Board o, int ai_turn, int depth) {
    if (checkWin(x)) return -10 + depth;
    if (checkWin(o)) return 10 - depth;
    if ((x | o) == 0x1FF) return 0;
    
    int best = ai_turn ? -1000 : 1000;
    for (int i = 0; i < 9; i++) {
        int m = mask(i);
        if (!((x | o) & m)) {
            if (ai_turn) {
                Board new_x = x | m;
                int val = miniMax(new_x, o, 0, depth + 1);
                if (val > best) best = val;
            } else {
                Board new_o = o | m;
                int val = miniMax(x, new_o, 1, depth + 1);
                if (val < best) best = val;
            }
        }
    }
    return best;
}

int findBestMove(Board x, Board o) {
    int bestVal = -1000;
    int bestMove = -1;
    
    for (int i = 0; i < 9; i++) {
        int m = mask(i);          
        if (!((x | o) & m)) {     
            Board new_x = x | m;  
            if (checkWin(new_x)) return i;

            Board new_o = o | m;
            if (checkWin(new_o)) { bestMove = i; continue;}

            int moveVal = miniMax(new_x, o, 0, 0);
            if (moveVal > bestVal) {
                bestVal = moveVal;
                bestMove = i;   
            }
        }
    }

    return bestMove;
}

int main() {
    Board x = 0;
    Board o = 0;
    int turn = 0;
    int move;

    printf("TTTBin\n");
    for(;;) {
        printBoard(x, o);

        if (checkWin(x)) { printf("You win.\n"); break; }
        if (checkWin(o)) { printf("You lose.\n"); break; }
        if ((x | o) == 0x1FF) { printf("Draw.\n"); break;}

        if (turn == 0) {
            printf("Your move (0..8): ");
            scanf("%i", &move);
            if ((x | o) & mask(move)) {
                printf("Invalid move, try again.\n");
                continue;
            }
            x |= mask(move);
        } else {
            printf("AI move:\n");
            move = findBestMove(x, o);
            o |= mask(move);
        }
        turn ^= 1;
    }
}
