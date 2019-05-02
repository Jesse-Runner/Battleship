all: client server

client: battle_client.cc battleship.h
	g++ -o battle_client battle_client.cc battleship.cc -lboost_system -lboost_thread -lpthread -lncurses
server: battle_server.cc
	g++ -o battle_server battle_server.cc -lboost_system -lboost_thread -lpthread
