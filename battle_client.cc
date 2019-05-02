// Jesse Runner
// March 23rd, 2019
// battle_client.cc
// This is the client-side for our battleship game
// it sends and receives boards to the server side
// until a winner is decided

#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include "battleship.h"
#include <ncurses.h>
using namespace std;
using boost::asio::ip::tcp;

// Function: convert_To_String(vector<vector<int> > board)
// Parameters: vector<vector<int> > board
// Description: This function will take a vector and turn it into
// a 16 char string
///////////////////////////////////////////////////
string convert_To_String(vector<vector<int> > board){
    stringstream newstring;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
              newstring << board[i][j];
        }
    }
    return newstring.str();
}
// Function: convert_To_Vector(string& convert, vector<vector<int> > board)
// Parameters: string& convert, vector<vector<int> > board
// Description: This function will take a 16 char string and turn it into
// a 2D vector
///////////////////////////////////////////////////
vector<vector<int> > convert_To_Vector(string& convert, vector<vector<int> > board){

    int x = 0;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(convert[x] == '0')
                board[i][j] = 0;
            if(convert[x] == '1')
                board[i][j] = 1;
            if(convert[x] == '2' )
                board[i][j] = 2;
            if(convert[x] == '3')
                board[i][j] = 3;
            x++;
        }
    }
    return board;
}



int main(int argc, char *argv[]) {

int portno = atoi(argv[2]); // set our port number to our second argument

boost::asio::io_service my_service; // set up our service

tcp::resolver resolver(my_service); // resolve it

tcp::socket socket(my_service); // create our socket

socket.connect(tcp::endpoint(boost::asio::ip::address::from_string(argv[1]),portno));

vector<vector<int> > board; // user's board
vector<vector<int> > opponent_Board; // opponent's board
int cur_row=0; // starting row value for our vector
int cur_col=0; // starting col value for our vector
bool firstmove = true; // bool var to tell us if we've made our first move

board = make_Move(board,opponent_Board,firstmove, firstmove); // make first move
opponent_Board = board; // just do this so you can reference indexes later when opponent_Board is set
//draw_top_matrix(board, 0, 0,0);
firstmove = false; // first move has ended

string msg;
msg = convert_To_String(board); // make our board into a sendable string
msg += "\n"; // add the newline so server knows when to stop reading
boost::asio::write(socket, boost::asio::buffer(msg)); // write our string

boost::asio::streambuf buf; // create a stream buffer

int n = boost::asio::read_until(socket, buf, "\n"); // read in our updated board
istream in(&buf); // create a stream to read from
string answer2; // string to hold our board
getline (in,answer2); // reading in our board

int m = boost::asio::read_until(socket, buf, "\n"); // read in opponent board
string answer ; // string to hold opponent board
getline (in,answer); // reading in opponent board
board = convert_To_Vector(answer2, board); // converting string into vector
opponent_Board = convert_To_Vector(answer, opponent_Board); // same as above

int a =  boost::asio::read_until(socket, buf, "\n"); // read in player #
string usernumber; // string to hold the usernumber
getline(in,usernumber); // read the value in to the string
int player_num = 0; // declare and initalize value to hold the string as an int
player_num = stoi(usernumber); // convert value to int

clear(); // clear the screen
refresh();
int o,p,r; // declares ints to be used for stream reading later
string player1hit, player2hit; // strings to tell us how many times a user ship is hit
string gameover = "3"; // game will end if three hits are registered
string winner; // string to hold the winning player
int did_move_hit_1 = 0; // will be used to check and see if player 1 hit
int did_move_hit_2 = 0; // will be used to check and see if player 2 hit
int player1hit_toint = 0; // will hold converted int value of player1hit
int player2hit_toint = 0; // will hold converted int value of player2hit
int player1hitmove = 0;
int player2hitmove = 0;
// this loop allows the players to go until someone wins or they both tie
while(player1hit != gameover && player2hit != gameover){
    msg.clear();
    if(player_num == 1)
      opponent_Board = make_Move(opponent_Board,board,firstmove,player1hitmove); //make your move
    if(player_num == 2)
        opponent_Board = make_Move(opponent_Board,board,firstmove,player2hitmove); //make your move
    msg = convert_To_String(opponent_Board); // make the board into a string
    msg += "\n";
    boost::asio::write(socket, boost::asio::buffer(msg)); // write to server

    answer.clear();
    answer2.clear();
    // your board first
    n = boost::asio::read_until(socket, buf, "\n"); // read in user board
    getline (in,answer); // load it into answer

    // opponents board second
    m = boost::asio::read_until(socket, buf, "\n"); // read in opp board
    getline (in,answer2); // load it into answer
    board = convert_To_Vector(answer, board); // convert back into a board
    opponent_Board = convert_To_Vector(answer2, opponent_Board); // same as above
    bool dummy = true;
    draw_top_matrix(board,0,0,0); // draw your board
    if(player_num == 1)
      draw_bot_matrix(opponent_Board,0,0,0,dummy); // draw opponents board
    if (player_num == 2)
        draw_bot_matrix(opponent_Board,0,0,0,dummy); // draw opponents board
    refresh(); // refresh screen so boards are drawn
    o = boost::asio::read_until(socket, buf, "\n"); // read in player1hit
    getline (in, player1hit); // load it into string

    p = boost::asio::read_until(socket, buf, "\n"); // read in player2hit
    getline (in, player2hit); // load it into string

    clear();
    refresh();
    player1hit_toint = stoi(player1hit);
    player2hit_toint = stoi(player2hit);

    if(player_num == 1){
        if(player1hit_toint != did_move_hit_1){
          //  player1hitmove = true;
            player1hitmove = 2;
            did_move_hit_1 = player1hit_toint;
        }
        else{
            //player1hitmove = false;
            player1hitmove = 1;
        }
    }
    else{
        if(player2hit_toint != did_move_hit_2){
            //player2hitmove = true;
            player2hitmove = 2;
            did_move_hit_2 = player2hit_toint;
        }
        else{
          //  player2hitmove = false;
              player2hitmove = 1;
        }
    }
  //  cout << " Bool value = " << player1hitmove;
}
    clear(); // clear the board
    refresh(); // refresh it so clear is reflected on screen
    r = boost::asio::read_until(socket, buf, "\n"); // read in winner
    getline(in, winner); // load it into string
    cout << winner; // print winner to the screen

}
