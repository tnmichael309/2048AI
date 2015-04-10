#ifndef _BITBOARD_H
#define _BITBOARD_H

#include "rng.h"
#include "operation.h"
#include <iostream>
#include <stdlib.h> 
#include <stdint.h>
#include <time.h>

using namespace std;


class board{
public:

	board(){
		m_board = 0;	
	};

	board(board_t pos){
		m_board = pos;	
	};

	~board(){;}

	// initialize the board with two random piece
	void init();

	// initialize the board with given starting condition
	void init(board_t b);


	// 1 for piece num 2
	// 2 for piece num 4
	int getNewPiece();

	// return a new vacant position
	// if no empty pos -> return invalidPos
	int getEmptyPos();

	//
	void insertNewPiece();

	//return true if cannot move anymore
	bool isEnd();

	//return true if it is full
	bool isFull();
	
	//return >= 0 value (eaten score retrieve) if it is a move, and move
	//else return -1
	int canMove(int direct);

	// show the board on the screan
	void showBoard();
	
	// show the board on the screan
	void showBoard(int positionOf1st32k,int positionOf2nd32k,int positionOf64k);
	
	// return the hashKey of the current board
	board_t getCurrentPosition();

	// board status
	board_t m_board;


};



#endif