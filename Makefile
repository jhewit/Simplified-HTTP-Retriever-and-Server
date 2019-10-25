# Author: Johnathan Hewit
# Makefile for HW2

all: Server Retriever

Server: Server.cpp
		g++ Server.cpp -o Server -lpthread

Retriever: Retriever.cpp
		g++ Retriever.cpp -o Retriever

clean:
		rm Server Retriever
