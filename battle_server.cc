// Jesse Runner
// March 23rd, 2019
// battle_server.cc
// This is the server-side for our battleship game
// it receives boards and acts as a referee for the client
// until a winner is decided

#include <iostream>
#include <string>
#include <sstream>
#include <boost/asio.hpp>
using namespace std;
using boost::asio::ip::tcp;

// Function: string_Search(string check)
// Parameters: string check
// Description: This function will examine a vector for
// the pre-determined hit character 3 and add them to count
// its return value decides if the game is over
///////////////////////////////////////////////////
int string_Search(string check){

  int count = 0;
  for(int i = 0; i < 16; i++){
      if(check[i] == '3')
          count++;
  }
  return count;
}

// Function: battle_server()
// Parameters: void
// Description: This function is entirely responsible for
// the server side of our battleship game
///////////////////////////////////////////////////
void battle_server(){

  boost::asio::io_service my_service; // init our service

  tcp::acceptor acceptor(my_service, tcp::endpoint(tcp::v4(), 4700)); // define our server

  tcp::socket socket1(my_service); // enable a socket to connect
  acceptor.accept(socket1); // accept that socket

  boost::asio::streambuf buf1; // set up a stream buffer
  int n = boost::asio::read_until(socket1, buf1, '\n' ); // read into that buffer
  istream is(&buf1); // set up a stream to use for reading
  string answer; // declare a string to hold our read value
  getline(is, answer); // load first board value into string

  cout << answer << endl; // print string value to server

  tcp::socket socket2(my_service); // enable a second socket for connection
  acceptor.accept(socket2); // accept a second sockets connection

  boost::asio::streambuf buf2; // set up a second stream buffer
  int m = boost::asio::read_until(socket2, buf2, '\n' ); // read into that buffer
  istream is2(&buf2); // set up a second stream
  string answer2; // declare a string to hold our value for second board
  getline(is2, answer2); // read our board value into that string
  cout << answer2 << endl; // print string value to server

  int player1hit = 0; // integer value to hold amt of times player 1 has a hit
  int player2hit = 0; // integer value to hold amt of times player 2 has a hit

  string player1hitstr; // will hold the string converted value of player1hit
  string player2hitstr; // will hold the string converted value of player2hit

  string msg;
  string msg1;
  msg = answer; // player's board
  msg += "\n";
  msg1 = answer2; // opponent's board
  msg1 += "\n";
  string msg2 = "1\n"; // will be sent to player one so the client knows which user they are
  string msg3 = "2\n"; // will be sent to player two so the client knows which user they are

  // for player 2
  // first write is their board
  // second write is opponent board
  boost::asio::write( socket2, boost::asio::buffer(msg));
  boost::asio::write( socket2, boost::asio::buffer(msg1));
  boost::asio::write( socket2, boost::asio::buffer(msg3));
  cout << "Player two written" << endl;
  // for player 1
  // first write is their board
  // second write is opponent board
  boost::asio::write( socket1, boost::asio::buffer(msg1));
  boost::asio::write( socket1, boost::asio::buffer(msg));
  boost::asio::write( socket1, boost::asio::buffer(msg2));
  cout << "Player one written" << endl;

  string gameover = "End\n";

  while(player1hit != 3 && player2hit != 3){ // loop to allow game to play until
                                             // one player registers 3 hits
  n = boost::asio::read_until(socket1, buf1, '\n' );
  answer.clear();
  getline(is, answer);
  cout << answer << endl;

  m = boost::asio::read_until(socket2, buf2, '\n' );
  answer2.clear();
  getline(is2, answer2);
  cout << answer2 << endl;

  player1hit = string_Search(answer); // looks for amt of hits
  player2hit = string_Search(answer2);  // looks for amt of hits

  player1hitstr = to_string(player1hit); // converts val to string for while loop
  player2hitstr = to_string(player2hit); // converts val to string for while loop
  player1hitstr += "\n";
  player2hitstr += "\n";


  answer += "\n";
  answer2 += "\n";

  boost::asio::write( socket2, boost::asio::buffer(answer2));
  boost::asio::write( socket2, boost::asio::buffer(answer));

  boost::asio::write( socket1, boost::asio::buffer(answer));
  boost::asio::write( socket1, boost::asio::buffer(answer2));

  cout << "Player 1 hit count = " << player1hit << endl;
  cout << "Player 2 hit count = " << player2hit << endl;

  boost::asio::write( socket2, boost::asio::buffer(player1hitstr));
  boost::asio::write( socket2, boost::asio::buffer(player2hitstr));
  boost::asio::write( socket1, boost::asio::buffer(player1hitstr));
  boost::asio::write( socket1, boost::asio::buffer(player2hitstr));
}

cout << gameover;
string winner;

if(player1hit == 3 && player2hit == 3){ // condition met if game ties
    winner = "Player one and player two tie!\n" ;
    boost::asio::write( socket2, boost::asio::buffer(winner));
    boost::asio::write( socket1, boost::asio::buffer(winner));
    cout << winner;

}
if(player1hit == 3 && player2hit < player1hit){ // condition met if player1 wins
    winner = "Player one wins!\n";
    boost::asio::write( socket2, boost::asio::buffer(winner));
    boost::asio::write( socket1, boost::asio::buffer(winner));
    cout << winner;
}
if(player2hit == 3 && player1hit < player2hit){ // condition met if player2 wins
    winner = "Player two wins!\n";
    boost::asio::write( socket2, boost::asio::buffer(winner));
    boost::asio::write( socket1, boost::asio::buffer(winner));
    cout << winner;
}

}

int main(){
  battle_server();
  return 0;
}
