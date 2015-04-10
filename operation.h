#ifndef _OPERATION_H
#define _OPERATION_H

#include "config.h"

class operation{
public:
	static row_t getRow(board_t b,int r){
		return (b>>(48-16*r) & 0xffff);
	}

	static row_t getCol(board_t b,int c){
		uint64_t colMask = (0xf000f000f000f000 >>  (c*4) );
		
		b = (b&colMask) << 4*c;
	/*	
		cout << hex << b << endl;
		cout << hex << getRow(b,0) << endl;
		cout << hex << getRow(b,1) << endl;
		cout << hex << getRow(b,2) << endl;
		cout << hex << getRow(b,3) << endl;
	*/	
		return getRow(b,0) | (getRow(b,1) >> 4) | (getRow(b,2) >> 8) | (getRow(b,3) >> 12);
	}

	static board_t setRow(board_t b, int r, row_t row){
		
		board_t tempRow = row;
		
		
		switch(r){
			case 0: 
				b = b&0x0000ffffffffffff | (tempRow << 48ULL); 
				break;
			case 1:
				b = b&0xffff0000ffffffff | (tempRow << 32ULL);
				break;
			case 2:
				b = b&0xffffffff0000ffff | (tempRow << 16ULL);
				break;
			case 3:
				b = b&0xffffffffffff0000 | tempRow;
				break;
			default:
				cout << "Invalid row number!" << endl;
		}
		return b;
	}

	static board_t setCol(board_t b,int c, row_t col){
		
		
		board_t tempCol = col;
		
		tempCol = (tempCol&0xf000) << 48ULL
		| (tempCol&0x0f00) << 36ULL 
		| (tempCol&0x00f0) << 24ULL 
		| (tempCol&0x000f) << 12ULL;
		tempCol = tempCol >> 4*c;
		
		cout << hex << tempCol << endl;
		
		switch(c){
			case 0: 
				b = (b&0x0fff0fff0fff0fff) | tempCol; 
				break;
			case 1:
				b = (b&0xf0fff0fff0fff0ff) | tempCol;
				break;
			case 2:
				b = (b&0xff0fff0fff0fff0f) | tempCol;
				break;
			case 3:
				b = (b&0xfff0fff0fff0fff0) | tempCol;
				break;
			default:
				cout << "Invalid col number!" << endl;
		}
		return b;
	}

	static board_t setRows(row_t* rows){
		board_t tempRows[4];
		board_t b = 0;
		for(int i =0; i < 4; i++){
			tempRows[i] = rows[i];
			tempRows[i] = tempRows[i] << (3-i)*16;
			b = b | tempRows[i];
		}
		return b;
	}

	static board_t setCols(row_t* cols){
		board_t tempCols[4];
		board_t b = 0;
		for(int i =0; i < 4; i++){
			tempCols[i] = cols[i];
			tempCols[i] = (tempCols[i]&0xf000) << 48ULL
					| (tempCols[i]&0x0f00) << 36ULL 
					| (tempCols[i]&0x00f0) << 24ULL 
					| (tempCols[i]&0x000f) << 12ULL;
			tempCols[i] = tempCols[i] >> 4*i;
			b = b | tempCols[i];
		}
		return b;
	}

	static row_t reverseRow(row_t r){
		//cout << hex << (r&0x0f00 >> 4) << endl;
		return (r&0xf000) >> 12 | (r&0x0f00) >> 4 | (r&0x00f0)<<4 | (r&0x000f) << 12;
	}
	/*
	static int moveRight(row_t& r){
		
		row_t rowMask = 0x000f;
		
		int curCheck = 0;
		int score = 0;
		bool isMove = false;
		
		for(int i = 1; i <= 3; i++){
			
			int checkNum = (r>>4*curCheck)&rowMask;
			int compareNum = (r >> 4*i)&rowMask;
			//cout << checkNum << ' ' << compareNum << endl;
			
			if(compareNum == 0){
				continue;
			}
			else if(checkNum == 0){
				r += compareNum << curCheck*4;
				row_t mask = rowMask << i*4;
				mask = ~mask;
				r = r&mask;
				isMove = true;
			}
			else if(checkNum == compareNum){
				r += (0x0001 << curCheck*4);
				
				row_t mask = rowMask << i*4;
				mask = ~mask;
				
				r = r&mask;
				curCheck++;
				
				score += configure::tile_score[checkNum+1];
				isMove = true;
			}
			else if(checkNum != compareNum){
				curCheck++;
				i = curCheck;
			}
			
			//cout << curCheck << endl;
			//cout << hex << r << endl;
		}
		
		if(isMove == false) return -1;
		
		return score;
	} */
};

#endif