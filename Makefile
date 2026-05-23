# Makefile for Distributed Chat App

CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall

SERVER = chat_server
SOURCE = server.cpp

all:
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(SERVER)

clean:
	rm -f $(SERVER)
