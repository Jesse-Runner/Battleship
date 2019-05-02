//***************************************************************************
// File: battle_example.cc
//
// Author: David W. Juedes
// Date: March 4th, 2019
//
// The following example shows how to use the curses
// library to display a 4x4 boards for "Mini-Battleship"
//
//
//
// The program exists when the F10 key is hit.
//
//***************************************************************************
#include <ncurses.h>
#include <iostream>
#include <vector>
#include "battleship.h"

using namespace std;
// Function: draw_top_matrix
// Parameters: vector<vector<int> > &board,int cur_row, int cur_col, int offset
// Description: Prints the user board starting at window pos 0,0
///////////////////////////////////////////////////////////////////////////////
void draw_top_matrix(vector<vector<int> > &board,int cur_row, int cur_col, int offset) {

  move(0,0);
  printw("Your Board:");

  for (int j=0;j<4;j++) {
      move(1,2*j);
      printw("+-");
    }
    move(1,2*4);
    printw("+");
  for (int i=0;i<4;i++) {
    for (int j=0;j<4;j++) {
        move(1 + 2*i+1,2*j);
        printw("|");
        move(1+ 2*i+1,2*j+1);
        if(board[i][j] == 3)
          printw("H");
        else{
          if (board[i][j] == 0) {
	           printw("X");
           }
        }

        if(board[i][j] == 2)
          printw("M");

         else {
	          printw("*");
         }
    }
    move(1 + 2*i+1,2*4);
    printw("|");
    for (int j=0;j<4;j++) {
      move(1 + 2*i+2,2*j);
      printw("+-");
    }
    move(1+ 2*i+2,2*4);
    printw("+");
  }
  move(1 + 2*cur_row+1,2*cur_col+1 + offset);
}
// Function: draw_bot_matrix
// Parameters: vector<vector<int> > &board,int cur_row, int cur_col, int offset
// Description: Prints the opponent board starting at window pos 10,0
///////////////////////////////////////////////////////////////////////////////
void draw_bot_matrix(vector<vector<int> > &board,int cur_row, int cur_col, int offset, int hit) {

  move(10,0);
  if(hit == 0)
    printw("Make your first move against your opponent!");
  if(hit == 2){
    printw("Your last move hit!");
  }
  if(hit == 1){
    printw("Your last move missed!");
  }


  move(11,0); // was 10
  printw("Opponent's Board:");
    for (int j=0;j<4;j++) {
        move(12,2*j);
        printw("+-");
      }
      move(12,2*4);
      printw("+");

    for (int i = 0;i<4;i++) {
      for (int j=0;j<4;j++) {
          move(2*i+13,2*j);
          printw("|");
          move(2*i+13,2*j+1);
          if(board[i][j] == 3){
            printw("H");
          }
          if(board[i][j] == 2)
            printw("M");

           else {
  	          printw("*");
           }
      }
      move(2*i+13,2*4);
      printw("|");
      for (int j=0;j<4;j++) {
        move(2*i+14,2*j);
        printw("+-");
      }
      move(2*i+14,2*4);
      printw("+");
    }

    move(2*cur_row+13,2*cur_col+1 + offset);
}



// Function: revert_Board
// Parameters: vector<vector<int> > board
// Description: sets all values of vector back to 0
///////////////////////////////////////////////////////////////////////////////
vector<vector<int> > revert_Board(vector<vector<int> > board){
    for(int i = 0; i < board.size(); i++){
        for(int j = 0; j < board.size(); j++)
            board[i][j] = 1;
    }
    return board;
}
// Function: move_StartPos
// Parameters: vector<vector<int> > &board,int &rightpress, int &cur_row, int &cur_col
// Description: allows user to rotate starting position
// on the board
///////////////////////////////////////////////////////////////////////////////
vector<vector<int> > move_StartPos(vector<vector<int> > &board, int &rightpress, int &cur_row, int &cur_col){

  if(rightpress == 0){

    if( cur_row + 1 > 3 || cur_row + 2 > 3){
        rightpress++;
    }
    else{
      board[cur_row][cur_col] = 0;
      board[(cur_row + 1) % 4][(cur_col) % 4] = 0;
      board[(cur_row + 2) % 4][(cur_col) % 4] = 0;
      return board;
    }
  }
  if(rightpress == 1){ // works
      if(cur_row + 1 > 3 || cur_row + 2 > 3 || cur_col + 1 > 3 || cur_col + 2 > 3){
          rightpress++;
      }
      else{
        board[cur_row][cur_col] = 0;
        board[(cur_row + 1) % 4][(cur_col + 1) % 4] = 0;
        board[(cur_row + 2) % 4][(cur_col + 2) % 4] = 0;
        return board;
      }
  }
  if(rightpress == 2){
    if(cur_col + 1 > 3 || cur_col + 2 > 3){
        rightpress++;
    }
    else{
        board[cur_row][cur_col] = 0;
        board[(cur_row) % 4][(cur_col + 1) % 4] = 0;
        board[(cur_row) % 4][(cur_col + 2) % 4] = 0;
        return board;
    }
  }
  if(rightpress == 3){ // works
    if(cur_row - 1 < 0 || cur_row - 2 < 0 || cur_col + 1 > 3 || cur_col + 2 > 3){
        rightpress++;
    }
    else{
        if(cur_row - 1 < 0 || cur_row - 2 < 0){
          if(cur_row - 1 >= 0){
            board[cur_row][cur_col] = 0;
            board[(cur_row - 1) % 4][(cur_col + 1) % 4] = 0;
            board[(4 - cur_row) % 4][(cur_col + 2) % 4] = 0;
            return board;
          }
          else{
            board[cur_row][cur_col] = 0;
            board[(4 - cur_row - 1 ) % 4][(cur_col + 1) % 4] = 0;
            board[(4 - cur_row - 2) % 4][(cur_col + 2) % 4] = 0;
            return board;
          }
        }
        else{
            board[cur_row][cur_col] = 0;
            board[(cur_row - 1) % 4][(cur_col + 1) % 4] = 0;
            board[(cur_row - 2) % 4][(cur_col + 2) % 4] = 0;
            return board;
        }
    }
  }
  if(rightpress == 4){
    if(cur_row - 1 < 0 || cur_row - 2 < 0){
        rightpress++;
    }
    else{
      if(cur_row - 1 < 0 || cur_row - 2 < 0){
          if(cur_row - 1 >= 0){
              board[cur_row][cur_col] = 0;
              board[(cur_row - 1) % 4][cur_col % 4] = 0;
              board[(4-cur_row) % 4][cur_col % 4] = 0;
              return board;
          }
          else{
            board[cur_row][cur_col] = 0;
            board[(4 - cur_row - 1) % 4][cur_col % 4] = 0;
            board[(4 - cur_row - 2) % 4][cur_col % 4] = 0;
            return board;
          }
      }
      else{
        board[cur_row][cur_col] = 0;
        board[(cur_row - 1) % 4][(cur_col) % 4] = 0;
        board[(cur_row - 2) % 4][(cur_col) % 4] = 0;
        return board;
      }
    }
  }
  if(rightpress == 5){
    if(cur_row - 1 < 0 || cur_row - 2 < 0 || cur_col - 1 < 0 || cur_col - 2 < 0){
        rightpress++;
    }
    else{
      board[cur_row][cur_col] = 0;
      board[(4 + cur_row - 1) % 4][(4 + cur_col - 1) % 4] = 0;
      board[(4 + cur_row - 2) % 4][(4 + cur_col - 2) % 4] = 0;
      return board;
    }
  }
  if(rightpress == 6){
    if(cur_col - 1 < 0 || cur_col - 2 < 0){
        rightpress++;
    }
    else{
        board[cur_row][cur_col] = 0;
        board[(cur_row) % 4][(4 + cur_col - 1) % 4] = 0;
        board[(cur_row) % 4][(4 + cur_col - 2) % 4] = 0;
        return board;
    }

  }
  if(rightpress == 7){
    if(cur_row + 1 > 3 || cur_row + 2 > 3 || cur_col - 1 < 0 || cur_col - 2 < 0){
        rightpress = -1;
    }
    else{
        board[cur_row][cur_col] = 0;
        board[(cur_row + 1) % 4][(4 + cur_col - 1) % 4] = 0;
        board[(cur_row + 2) % 4][(4 + cur_col - 2) % 4] = 0;
        return board;
    }
  }
  return board;
}

// Function: first_Move
// Parameters: vector<vector<int> > &board,int &cur_row, int &cur_col
// Description: allows user to move to a starting position of their choice
// on the board
///////////////////////////////////////////////////////////////////////////////
vector<vector<int> > first_Move(vector<vector<int>> &board, int &cur_row, int &cur_col){

    int value;
    int rightpress = 0;
    int leftpress = 0;
    board = move_StartPos(board, rightpress, cur_row, cur_col);
    //board[cur_row][cur_col] = 0;
    //board[(cur_row + 1)% 4][cur_col] = 0;
    //board[(cur_row + 2) % 4][cur_col] = 0;
    draw_top_matrix(board,cur_row,cur_col,0);

    while((value = getch())!= 27){
      switch(value){
          case ' ':
            return board;

          case KEY_RIGHT:
            //if(rightpress + 1 == 8)
                //rightpress = -1;
            rightpress++;
            board = revert_Board(board);  // revert board so old move is erased
            board = move_StartPos(board,rightpress,cur_row,cur_col);
            draw_top_matrix(board,cur_row,cur_col,0);
            // Redraw the screen.
            refresh();
            break;
          case KEY_LEFT:

            if(rightpress - 1 == -1){
                rightpress = 8;
            }

            rightpress--;

            board = revert_Board(board);
            board = move_StartPos(board, rightpress, cur_row, cur_col);
            draw_top_matrix(board,cur_row,cur_col,0);
            // Redraw the screen.
            refresh();
            break;
      }
    }
    board = revert_Board(board);
    board = make_Move(board, board,true, 0);
    return board;
}
// Function: next_Move
// Parameters: vector<vector<int> > &board, int &cur_row, int &cur_col
// Description: allows user to continue playing the game after the
// first move by using keyboard to move around the board and sel move
///////////////////////////////////////////////////////////////////////////////
vector<vector<int> > next_Move(vector<vector<int>> &board, int &cur_row, int &cur_col){
      if(board[cur_row][cur_col] == 0)
          board[cur_row][cur_col] = 3;
      if(board[cur_row][cur_col] == 1)
          board[cur_row][cur_col] = 2;

          return board;
}
// Function: move_StartPos
// Parameters: vector<vector<int> > board,vector<vector<int> > oppboard, int &cur_row, int &cur_col
// Description: Primary function for move making, it controls and sends the user
// to the necessary function to make a move depending on the status of the game
///////////////////////////////////////////////////////////////////////////////
vector<vector<int>> make_Move(vector<vector<int>> board, vector<vector<int>> oppboard,bool firstmove, int hit) {
  int rows;
  int cols;
  int cur_row = 0;
  int cur_col = 0;
  int ch;

  for (int i=0;i<4;i++) {
    vector<int> t;
    for (int j=0;j<4;j++) {
      t.push_back(1);
    }
    board.push_back(t);
  }

  // Screen initialization

  initscr();
  // Clear the screen

  clear();

  // Get the size of the window!
  getmaxyx(stdscr,rows,cols);


  cbreak();  // Pass all characters to this program!

  keypad(stdscr, TRUE); // Grab the special keys, arrow keys, etc.

  // Paint the row and column markers.
  //paint_markers(rows,cols,10,0,0);
  // Redraw the screen.
  refresh();

  if(firstmove == true)
  draw_top_matrix(board,0,0,0);
  else{
    draw_top_matrix(board,0,0,0);
    draw_bot_matrix(oppboard,0,0,0, hit);
  }
  // I/O Loop....
  // Stop when the q Key is hit.
  //


  while ((ch = getch())!='q') {
    switch (ch) {
      case ' ':
        if(firstmove == true){
          board = first_Move(board,cur_row,cur_col);
          firstmove = false;
          return board;
        }
        else{
          clear();
          refresh();
          board = next_Move(oppboard, cur_row, cur_col);
          return board;
      }
      break;
      case KEY_RIGHT:
      if(firstmove == true){
        cur_col++;
        cur_col%=4;
        draw_top_matrix(board,cur_row,cur_col,0);
        // Redraw the screen.
        refresh();
        break;
      }
      else{
        cur_col++;
        cur_col%=4;
        draw_top_matrix(board,cur_row,cur_col,0);
        draw_bot_matrix(oppboard,cur_row,cur_col, 0, hit);
        // Redraw the screen.
        refresh();
        break;

      }
      case KEY_LEFT:
        if(firstmove == true ){
          cur_col--;
          cur_col = (4+cur_col)%4;
          draw_top_matrix(board,cur_row,cur_col,0);
          // Redraw the screen.
          refresh();
          break;
        }
        else{
          cur_col--;
          cur_col = (4+cur_col)%4;
          draw_top_matrix(board,cur_row,cur_col,0);
          draw_bot_matrix(oppboard, cur_row, cur_col, 0, hit);
          // Redraw the screen.
          refresh();
          break;


        }

      case KEY_UP:
        if(firstmove == true){
        cur_row--;
        cur_row=(4+cur_row) % 4;
        draw_top_matrix(board,cur_row,cur_col,0);

        //      paint_markers(rows,cols,10,cur_row,cur_col);
        // Redraw the screen.
        refresh();
        break;
       }
       else{
         cur_row--;
         cur_row=(4+cur_row) % 4;
         draw_top_matrix(board,cur_row,cur_col,0);
         draw_bot_matrix(oppboard, cur_row, cur_col, 0,hit);
         //      paint_markers(rows,cols,10,cur_row,cur_col);
         // Redraw the screen.
         refresh();
         break;
       }
      case KEY_DOWN:
        if(firstmove == true){
        cur_row++;
        cur_row%=4;
        draw_top_matrix(board,cur_row,cur_col,0);
              //paint_markers(rows,cols,10,cur_row,cur_col);
        // Redraw the screen.
        refresh();
        break;
        }
        else{
          cur_row++;
          cur_row%=4;
          draw_top_matrix(board,cur_row,cur_col,0);
          draw_bot_matrix(oppboard,cur_row, cur_col, 0, hit);
                //paint_markers(rows,cols,10,cur_row,cur_col);
          // Redraw the screen.
          refresh();
          break;
        }
      }

    }
    exit (EXIT_FAILURE);
    endwin();
  }
