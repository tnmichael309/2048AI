#ifndef _FEATURE_H
#define _FEATURE_H

#include <fstream>
#include <iostream>
using namespace std;

class feature{
public:
	feature(){;}
	~feature(){;}
	virtual board_t getIndex(board_t b, int num)  = 0;
	virtual void updateScore(board_t b, double delta) = 0;
	virtual double getScore(board_t b) = 0;
	virtual void saveToFile(fstream& out) = 0;
	virtual void loadFromFile(fstream& in) = 0;
	virtual double getTupleValue(int i, board_t j){
		return  0;
	}

	// sign: true-> add, false->minus
	virtual void addFromOtherFeature(feature& ft, bool sign){
		return;
	}
	virtual void addSquaredValueFromOtherFeature(feature& ft){
		return;
	}
	virtual void divideAllDataByN(const int& n){
		return;
	}

	void setSymmetricBoards(board_t* rotateSymmetry, board_t* isomorphic){
		rotateBoards = rotateSymmetry;
		isomorphicBoards = isomorphic;
	};
	virtual void showContent(){
		cout << "I'm superclass ... not children" << endl;
	};
protected:
	board_t* getRotatedBoards(){
		return rotateBoards;
	};
	board_t* getIsomorphicBoards(){
		return isomorphicBoards;
	};
	board_t  getMirrorSymmetricBoard(board_t boardStatus){
		row_t rows[4];
		row_t reverseRows[4];
		
		for(int i =0; i < 4; i++){
			rows[i] = operation::getRow(boardStatus,i);
			reverseRows[i] = operation::reverseRow(rows[i]);
		}
		
		board_t b = operation::setRows(reverseRows); 
		
		return b;
	};
private:
	board_t* rotateBoards;
	board_t* isomorphicBoards;
};


#endif