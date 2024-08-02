//
// Author: Jessie Zhou
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lab8part1.h"

void initBoard(char board[][26], int n) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if ((i == n / 2 - 1 && j == n / 2 - 1) || (i == n / 2 && j == n / 2)) {
        board[i][j] = 'W';
      }

      else if ((i == n / 2 && j == n / 2 - 1) ||
               (i == n / 2 - 1 && j == n / 2)) {
        board[i][j] = 'B';
      }

      else {
        board[i][j] = 'U';
      }
    }
  }
}

void printBoard(char board[][26], int n) {
  char row = 'a';
  printf("  ");
  for (int i = 0; i < n; i++) {
    printf("%c", row++);
  }
  printf("\n");
  char col = 'a';
  for (int i = 0; i < n; i++) {
    printf("%c ", col++);
    for (int j = 0; j < n; j++) {
      printf("%c", board[i][j]);
    }
    printf("\n");
  }
}

bool positionInBounds(int row, int col, int n) {
  return (row >= 0 && row < n && col >= 0 && col < n);
}

bool checkLegalInDirection(char board[][26], int n, int row, int col, char currentTurn, int i, int j) {
  int deltaRow = i;
  int deltaCol = j;

  int contRow = row + deltaRow;
  int contCol = col + deltaCol;

  if (positionInBounds(contRow, contCol, n) == false) {
    return false;
  }
  if (board[contRow][contCol] == currentTurn) {
    return false;
  }

  while (positionInBounds(contRow, contCol, n)) {
    if (board[contRow][contCol] == currentTurn) {
      return true;
    }
    if (board[contRow][contCol] == 'U') {
      return false;
    }
    if (contRow == n || contCol == n) {
      return false;
    }
    contRow += deltaRow;
    contCol += deltaCol;
  }
  return false;
}

bool moveIsValid(char board[][26], int n, int row, int col, char currentTurn) {
  if (board[row][col] == 'B' || board[row][col] == 'W') {
    return false;
  }

  else {
    for (int i = -1; i < 2; ++i) {
      for (int j = -1; j < 2; ++j) {
        if (!(j == 0 && i == 0)) {
          if (checkLegalInDirection(board, n, row, col, currentTurn, i, j)) {
            return true;
          }
        }
      }
    }
    return false;
  }
}

int calculateAvailableMoves(char board[][26], int n, char currentTurn) {  //original output is void
  int numAvailableMoves = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (moveIsValid(board, n, i, j, currentTurn)) {
        //printf("%c%c\n", i + 'a', j + 'a');
        ++numAvailableMoves;
      }
    }
  }
  return numAvailableMoves;
}

int checkValidAndFlip(char board[][26], int n, int row, int col, char currentTurn, bool flip) {
  int cnt = 0;
  if (moveIsValid(board, n, row, col, currentTurn)) {
    //printf("Valid move.\n");
    if (flip) {
      board[row][col] = currentTurn;
    }
    for (int i = -1; i < 2; i++) {
      for (int j = -1; j < 2; j++) {
        if (checkLegalInDirection(board, n, row, col, currentTurn, i, j)) {
          int r = row + i;
          int c = col + j;
          while (positionInBounds(r, c, n) && board[r][c] != currentTurn) {
            if (flip) {
              board[r][c] = currentTurn;
            }
            r += i;
            c += j;
            cnt++;
          }
        }
      }
    }
  } else {
    //printf("Invalid move.\n");
  }
  return cnt;
}

int testTheMoveScore(char board[][26], int n, int row, int col, char computerPlayer) {
  return checkValidAndFlip(board, n, row, col, computerPlayer, false);
}


void computerFlips(char board[][26], int n, char computerColour) {
  int bRow = -1;
  int bCol = -1;
  int mostTurns = -1;
  int turns;
  
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if(moveIsValid(board, n, i, j, computerColour)) {
        
        
        turns = testTheMoveScore(board, n, i, j, computerColour);
        
        if (turns > mostTurns) {
          mostTurns = turns;
          bRow = i;
          bCol = j;
        }
      }
    }
  }


  if(mostTurns > -1) {
    printf("Computer places %c at %c%c. \n", computerColour, 'a' + bRow, 'a' + bCol);
    checkValidAndFlip(board, n, bRow, bCol, computerColour, true);
  }
  else {
    printf("%c player has no valid move. \n", computerColour);
  }
}

bool checkGameFinished (char board[][26], int n) {
  bool isGameCompleted = false;
  bool isBoardFilled = true;
  bool movesAvaliable = false;
  int blackTotal = 0; 
  int whiteTotal = 0;

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (board[i][j] != 'B' && board[i][j] != 'W' ) {
        isBoardFilled = false;
      }
      else if (board[i][j] == 'W') {
        ++whiteTotal;
      }
      else {
        ++blackTotal;
      }
    }
  }

  for (int i = 0; ((i < n) && (!moveIsValid)); ++i) { // we need to ensure that every place on the board is taken or no other valids can exist
    for (int j = 0; ((j < n) && (!moveIsValid)); ++j) {
      if (moveIsValid(board, n, i, j, 'W') || moveIsValid(board, n, i, j, 'B')) {
        movesAvaliable = true;
      }
    }
  }

  if (isBoardFilled && !movesAvaliable) { // this will run if both are true and therefore we are determining which colour is the winer
    if (whiteTotal == blackTotal) {
      printf("Draw!\n");
    }
    else if (whiteTotal > blackTotal) {
      printf("W player wins.\n");
    }
    else {
      printf("B player wins. \n");
    }
    isGameCompleted = true;
    return isGameCompleted;
  }
  return isGameCompleted;
}


int main(void) {
  int boardSize = 0;
  char computerPlayer;
  char currentTurn = 'B';
  char winner;
  char oppositeTurn = 'U';

  printf("Enter the board dimension: ");
  scanf(" %d", &boardSize);
  printf("Computer plays (B/W): ");
  scanf(" %c", &computerPlayer);


  char boardPlace[26][26];
  initBoard(boardPlace, boardSize);
  char row, col;
  bool gameOn = true;

  printBoard(boardPlace, boardSize);
  
  while (gameOn) {
    //printBoard(boardPlace, boardSize);
    if (currentTurn == 'W') {
      oppositeTurn = 'B';

    }
    else if (currentTurn == 'B') {
      oppositeTurn = 'W';
    }

    if (calculateAvailableMoves(boardPlace, boardSize, currentTurn) == 0 && calculateAvailableMoves(boardPlace, boardSize,oppositeTurn) != 0) {
      printf("%c player has no valid move. \n", currentTurn);
    
      if (currentTurn == 'W') {
        currentTurn = 'B';
      } 
      else {
        currentTurn =  'W';
      }
      continue;
    }
  

    if ((computerPlayer == 'W' && currentTurn == 'W') || (computerPlayer == 'B' && currentTurn == 'B')) {
      computerFlips(boardPlace, boardSize, computerPlayer);

    }
    else {
      printf("Enter move for colour %c (RowCol): ", currentTurn);
      scanf(" %c%c",&row, &col);
       if (!checkValidAndFlip(boardPlace, boardSize, row - 'a', col - 'a', currentTurn, true)) {
        printf("Invalid move.\n");
        if (currentTurn == 'W') {
          winner = 'B';
        } 
        else {
          winner = 'W';
        }
        printf("%c player wins.", winner);
        gameOn = false;
        break;
      }
    }
    printBoard(boardPlace, boardSize);
    gameOn = !checkGameFinished(boardPlace, boardSize);
    
    if(currentTurn == 'W'){
      currentTurn = 'B';
    }
    else if (currentTurn == 'B') {
      currentTurn = 'W';
    }
    
  }

  return EXIT_SUCCESS;
}
