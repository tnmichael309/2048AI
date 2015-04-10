//#include "stdafx.h"
#include "bitboard.h"

void board::init(){
	m_board = 0;
	insertNewPiece();
	insertNewPiece();
}

void board::init(board_t pos){
	m_board = pos;
}

int board::getNewPiece(){
	
	int r = configure::uRNG.rand()%10;
	if(r == 0) return 2;
	else return 1;
};

int board::getEmptyPos(){

	int empty = 0;
	for(int i =0; i < 64; i+=4){
		if(((m_board >> i) & 0xf ) == 0) empty ++;
	}
	int iStart = configure::uRNG.rand()%empty;
	cout << iStart << endl;
	bool existEmpty = false;
	
	for(int i = 0; i < 16; i++){
		board_t b = m_board;
		if(((m_board>>(4*(15-i)))&0xf) == 0){
			if(iStart == 0){
				return i;
			}
			iStart --;
			existEmpty = true;
		}else if(i == 15 && existEmpty == false){
			return -1;
		}
		
	}

	return -1;
}

void board::insertNewPiece(){
	int pos = getEmptyPos();
	if(pos == -1){
		cout << "Wrong with getting new empty position" << endl;
		while(1);
	}else{
		board_t num = getNewPiece();
		
		num = num << 4*(15-pos);
		board_t mask = 0x000f;
		mask = mask << 4*(15-pos);
		//cout << pos << endl;
		//cout << hex << num << endl;
		//cout << hex << mask << endl; 
		mask = ~mask;
		//cout << hex << mask << endl; 
		m_board = (m_board&mask) | num;
	}
};

bool board::isEnd(){
	
	for(int i =0; i < 4; i++){
		row_t r = operation::getRow(m_board, i);
		row_t c = operation::getCol(m_board, i);
		for(int j =0; j < 3; j++){
			if(((r>>4*j)&0xf) == ((r>>4*(j+1))&0xf) || ((r>>4*j)&0xf) == 0){
				return false;
			} 
			if(((c>>4*j)&0xf) == ((c>>4*(j+1))&0xf) || ((c>>4*j)&0xf) == 0){
				return false;
			} 
		} 
		if(((r>>12)&0xf) == 0) return false;
		if(((c>>12)&0xf) == 0) return false;
	}
	

	
	return true;
	
}

bool board::isFull(){
	for(int i = 0; i < 16; i ++){
		if(((m_board >> 4*i) & 0xf) == 0) return false;
	}

	return true;
};

// 0 : up
// 1 : right
// 2 : down 
// 3 : left
int board::canMove(int direct){
	
	row_t row[4];
	int score = 0;
	bool notMove = true;
	switch(direct){
		case 0:
			for(int i =0; i < 4; i++){
				row[i] = operation::reverseRow(operation::getCol(m_board,i));
				if(configure::rightInfo[row[i]].score == -1);
				else{
					row_t result = configure::rightInfo[row[i]].r;
					score += configure::rightInfo[row[i]].score;
					row[i] = result;
					notMove = false;
				}
				//score += moveRight(row[i]);	
				row[i] = operation::reverseRow(row[i]);		
			}
			if(notMove) return -1;
			else{
				m_board = operation::setCols(row);
				return score; 
			}
			break;
		case 1:
			for(int i =0; i < 4; i++){
				row[i] = operation::getRow(m_board,i);
				if(configure::rightInfo[row[i]].score == -1);
				else{
					row_t result = configure::rightInfo[row[i]].r;
					score += configure::rightInfo[row[i]].score;
					row[i] = result;
					notMove = false;
				}
				//score += moveRight(row[i]);				
			}
			if(notMove) return -1;
			else{
				m_board = operation::setRows(row);
				return score; 
			}
			break;
		case 2:
			for(int i =0; i < 4; i++){
				row[i] = operation::getCol(m_board,i);
				if(configure::rightInfo[row[i]].score == -1);
				else{
					row_t result = configure::rightInfo[row[i]].r;
					score += configure::rightInfo[row[i]].score;
					row[i] = result;
					notMove = false;
				}
				//score += moveRight(row[i]);		
			}
			if(notMove) return -1;
			else{
				m_board = operation::setCols(row);
				return score; 
			}
			break;
		case 3:
			for(int i =0; i < 4; i++){
				row[i] = operation::reverseRow(operation::getRow(m_board,i));
				if(configure::rightInfo[row[i]].score == -1);
				else{
					row_t result = configure::rightInfo[row[i]].r;
					score += configure::rightInfo[row[i]].score;
					row[i] = result;
					notMove = false;
				}
				row[i] = operation::reverseRow(row[i]);	
			}
			if(notMove) return -1;
			else{
				m_board = operation::setRows(row);
				return score; 
			}
			break;
		default:
			cout << "Wrong direction!" << endl;
			break;
	}	
	
	return score;
};

void board::showBoard(){
	for(int i = 0; i < 4; i++){
		row_t r = operation::getRow(m_board,i);
		for(int j = 3; j >= 0; j--){
			cout << configure::tile_score[((r>>4*j)&0xf)] << "\t";
		}
		cout << endl;
	}
	cout << endl;
}

void board::showBoard(int positionOf1st32k,int positionOf2nd32k,int positionOf64k){
	
	for(int i = 0; i < 4; i++){
		row_t r = operation::getRow(m_board,i);
		for(int j = 3; j >= 0; j--){
			if((i*4+3-j) ==  positionOf1st32k || (i*4+3-j) == positionOf2nd32k) cout << 32768 << "\t";
			else if((i*4+3-j) ==  positionOf64k) cout << 65536 << "\t";
			else cout << configure::tile_score[((r>>4*j)&0xf)] << "\t";
		}
		cout << endl;
	}
	cout << endl;
}

board_t board::getCurrentPosition(){
	return m_board;
}