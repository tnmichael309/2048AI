/*
#ifndef _TUPLENETWORK_H
#define _TUPLENETWORK_H

#include "bitboard.h"
#include "tuple.h"
#include "tuples.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
using namespace std;

class tupleNetwork{
public:
	tupleNetwork(){
		tuples.push_back(new fourOuterTuple);
		tuples.push_back(new fourInnerTuple);
		tuples.push_back(new SixOuterTuple);
		tuples.push_back(new SixMiddleTuple);
		tuples.push_back(new SixInnerTuple);
		tuples.push_back(new LeftAxeLeftMostTuple);
		tuples.push_back(new LeftAxeMiddleTuple);
		tuples.push_back(new LeftAxeRightMostTuple);
		tuples.push_back(new RightAxeLeftMostTuple);
		tuples.push_back(new RightAxeMiddleTuple);
		tuples.push_back(new RightAxeRightMostTuple);
		tuples.push_back(new MaxFeatureTileTuple);
	}
	~tupleNetwork(){;}
	double getValue(board_t boardStatus){
		board_t b[4];
		row_t rows[4];
		row_t reverseRows[4];
		row_t oRows[4];
		row_t oReverseRows[4];
	
		for(int i =0; i < 4; i++){
			rows[i] = operation::getRow(boardStatus,i);
			oRows[3-i] = rows[i];
			reverseRows[i] = operation::reverseRow(rows[i]);
			oReverseRows[3-i] = reverseRows[i];
		}

	
		b[0] = boardStatus;
		b[1] = operation::setRows(reverseRows);
		b[2] = operation::setRows(oRows);
		b[3] = operation::setRows(oReverseRows);
		b[4] = operation::setCols(rows);
		b[5] = operation::setCols(reverseRows);
		b[6] = operation::setCols(oRows);
		b[7] = operation::setCols(oReverseRows);
	
	
		double totalSum = 0.0;
		for(int i = 0; i < 8; i++){
			for(int j = 0; j < tuples.size(); j++)
				totalSum += tuples[j]->getValue(b[i]);
		}
		
		return  totalSum;
	};
	void updateValue(board_t boardStatus, double delta){
		board_t b[4];
		row_t rows[4];
		row_t reverseRows[4];
		row_t oRows[4];
		row_t oReverseRows[4];
	
		for(int i =0; i < 4; i++){
			rows[i] = operation::getRow(boardStatus,i);
			oRows[3-i] = rows[i];
			reverseRows[i] = operation::reverseRow(rows[i]);
			oReverseRows[3-i] = reverseRows[i];
		}
	
	
		b[0] = boardStatus;
		b[1] = operation::setRows(reverseRows);
		b[2] = operation::setRows(oRows);
		b[3] = operation::setRows(oReverseRows);
		b[4] = operation::setCols(rows);
		b[5] = operation::setCols(reverseRows);
		b[6] = operation::setCols(oRows);
		b[7] = operation::setCols(oReverseRows);

		for(int i = 0; i < 8; i++){
			for(int j = 0; j < tuples.size(); j++)
				tuples[j]->updateValue(b[i],delta);
		}
	};
	void saveResultToFile(int mode){
		
		fstream out;
		
		if(mode == 0)out.open("trainingResult(Binary)_before8192.txt", ios::out | ios::binary);
		if(mode == 1)out.open("trainingResult(Binary)_8192Appear.txt", ios::out | ios::binary);
		if(mode == 2)out.open("trainingResult(Binary)_16384Appear.txt", ios::out | ios::binary);
		if(mode == 3)out.open("trainingResult(Binary)_16384_8192Appear.txt", ios::out | ios::binary);
		if(mode == 4)out.open("trainingResult(Binary)_16384_8192_4096Appear.txt", ios::out | ios::binary);
		if(mode == 5)out.open("trainingResult(Binary)_32768Appear.txt", ios::out | ios::binary);
		if(mode == 6)out.open("trainingResult(Binary)_32768_8192Appear.txt", ios::out | ios::binary);
		if(mode == 7)out.open("trainingResult(Binary)_32768_8192_4096Appear.txt", ios::out | ios::binary);
		
		
		for(int i = 0; i < tuples.size(); i++){
			out.write(reinterpret_cast<char*>(tuples[i]->getTupleDataPointer()), (tuples[i]->getSizeOfTuple())*sizeof(double));
		}	
		out.close();
	};

	void getResultFromFile(int mode){
		
		fstream in;
			
		if(mode == 0)in.open("trainingResult(Binary)_before8192.txt", ios::in | ios::binary);
		if(mode == 1)in.open("trainingResult(Binary)_8192Appear.txt", ios::in | ios::binary);
		if(mode == 2)in.open("trainingResult(Binary)_16384Appear.txt", ios::in | ios::binary);
		if(mode == 3)in.open("trainingResult(Binary)_16384_8192Appear.txt", ios::in | ios::binary);
		if(mode == 4)in.open("trainingResult(Binary)_16384_8192_4096Appear.txt", ios::in | ios::binary);
		if(mode == 5)in.open("trainingResult(Binary)_32768Appear.txt", ios::in | ios::binary);
		if(mode == 6)in.open("trainingResult(Binary)_32768_8192Appear.txt", ios::in | ios::binary);
		if(mode == 7)in.open("trainingResult(Binary)_32768_8192_4096Appear.txt", ios::in | ios::binary);
		
		for(int i = 0; i < tuples.size(); i++){
			in.read(reinterpret_cast<char*>(tuples[i]->getTupleDataPointer()), (tuples[i]->getSizeOfTuple())*sizeof(double));
		}	
		in.close();
	};
private:
	vector<tuple*> tuples;
};

#endif
*/
/* 
#ifndef _TUPLENETWORK_H
#define _TUPLENETWORK_H

#include "bitboard.h"
#include "operation.h"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
using namespace std;

class tupleNetwork{
public:
	tupleNetwork(){
		fourTuple = new double*[2];
		sixTuple = new double*[3];
		littleAxe = new double*[6];
		maxTileFeature = new double[1024];
		emptyTiles = new double[16];
		availableMergeMove = new double[9];
		//averageValueOfBoard = new double[];
		for(int i = 0; i < 2; i++){
			fourTuple[i] = new double[0xffff+1];
			memset(fourTuple[i],0,sizeof(double)*(0xffff+1));
		}
		for(int i = 0; i < 3; i++){
			sixTuple[i] = new double[0xffffff+1];
			memset(sixTuple[i],0,sizeof(double)*(0xffffff+1));
			}
		for(int i = 0; i < 6; i++){
			littleAxe[i] = new double[0xffffff+1];
			memset(littleAxe[i],0,sizeof(double)*(0xffffff+1));
		}
		memset(maxTileFeature,0,sizeof(double)*1024);
		memset(emptyTiles,0,sizeof(double)*16);
		memset(availableMergeMove,0,sizeof(double)*9);
		
		
	}
	~tupleNetwork(){
		for(int i = 0; i < 2; i++){
			delete[] fourTuple[i];
		}
		for(int i = 0; i < 3; i++){
			delete[] sixTuple[i];
		}
		for(int i = 0; i < 6; i++){
			delete[] littleAxe[i];
		}
		delete[] fourTuple;
		delete[] sixTuple;
		delete[] littleAxe;
		delete[] maxTileFeature;
		delete[] emptyTiles;
		delete[] availableMergeMove;
	};
	unsigned long long getFourTupleIndex(board_t b, int num){
		if(num!=0 && num!=1) while(1);
		row_t r = operation::getCol(b,3-num);
		return r;
	};
	unsigned long long getSixTupleIndex(board_t b, int num){
		if(num!=0 && num!=1 && num!= 2) while(1);
		
		board_t r1 = operation::getCol(b,num);
		board_t r2 = operation::reverseRow(operation::getCol(b,num+1));
		board_t finalIndex = (r1&0xfff0) << 8 | (r2&0xfff);
		
		return finalIndex;
	};
	unsigned long long getLittleAxeIndex(board_t b, int num){
		if( num < 0 || num >= 6) while(1);
		
		board_t r1 = operation::getCol(b,num);
		board_t r2 = operation::getCol(b,num+1);
		board_t finalIndex;
		if(num < 3) finalIndex = (r1&0xffff) << 8 | (r2&0xff);
		else finalIndex = (r1&0xff) << 16 | (r2&0xffff);

		return finalIndex;
	};
	unsigned long long getMaxFeatureIndex(board_t b){
		unsigned long long index = 0;
			
		for(int i = 0; i < 64; i+= 4){
			int val = (b >> i)&0xf;
			int shift = val - 11;
			if(val >= 11) index += 1 << 2*shift;
		}
		return index;
	};
	unsigned long long getEmptyIndex(board_t b){
		unsigned long long index = 0;
			
		for(int i = 0; i < 64; i+= 4){
			int val = (b >> i)&0xf;
			if(val == 0) index ++;
		}
		return index;
	};
	unsigned long long getAvailableMergeMoveIndex(board_t b){
		unsigned long long index = 0;
			
		for(int i = 0; i < 4; i++){
			row_t r = operation::getRow(b, i);
			row_t c = operation::getCol(b, i);
			for(int j =0; j < 3; j++){
				if(((r>>4*j)&0xf) == ((r>>4*(j+1))&0xf)) index++;
				if(((c>>4*j)&0xf) == ((c>>4*(j+1))&0xf)) index++;
			}
		}
		return index;
	};
	double getValue(board_t boardStatus){
		board_t b[8];
		row_t rows[4];
		row_t reverseRows[4];
		row_t oRows[4];
		row_t oReverseRows[4];
	
		for(int i = 0; i < 4; i++){
			rows[i] = operation::getRow(boardStatus,i);
			oRows[3-i] = rows[i];
			reverseRows[i] = operation::reverseRow(rows[i]);
			oReverseRows[3-i] = reverseRows[i];
		}
	/*
	
		b[0] = boardStatus;
		b[1] = operation::setCols(reverseRows);
		b[2] = operation::setCols(oRows);
		b[3] = operation::setRows(oReverseRows);
	
	
		b[0] = boardStatus;
		b[1] = operation::setRows(reverseRows);
		b[2] = operation::setRows(oRows);
		b[3] = operation::setRows(oReverseRows);
		b[4] = operation::setCols(rows);
		b[5] = operation::setCols(reverseRows);
		b[6] = operation::setCols(oRows);
		b[7] = operation::setCols(oReverseRows);
	
		
		double totalSum = 0.0;
		for(int j = 0; j < 8; j++){
			for(int i = 0; i < 2; i++){
				totalSum += fourTuple[i][getFourTupleIndex(b[j],i)];
			}
			for(int i = 0; i < 3; i++){
				totalSum += sixTuple[i][getSixTupleIndex(b[j],i)];
			}
			for(int i = 0; i < 6; i++){
				totalSum += littleAxe[i][ getLittleAxeIndex(b[j],i)];
			}
		}
		totalSum += maxTileFeature[getMaxFeatureIndex(b[0])];
		totalSum += emptyTiles[getEmptyIndex(b[0])];
		totalSum += availableMergeMove[getAvailableMergeMoveIndex(b[0])];
		
		return  totalSum;
	};
	void updateValue(board_t boardStatus, double delta){
		board_t b[8];
		row_t rows[4];
		row_t reverseRows[4];
		row_t oRows[4];
		row_t oReverseRows[4];
	
		for(int i =0; i < 4; i++){
			rows[i] = operation::getRow(boardStatus,i);
			oRows[3-i] = rows[i];
			reverseRows[i] = operation::reverseRow(rows[i]);
			oReverseRows[3-i] = reverseRows[i];
		}
	/*
		b[0] = boardStatus;
		b[1] = operation::setCols(reverseRows);
		b[2] = operation::setCols(oRows);
		b[3] = operation::setRows(oReverseRows);
	
	
		b[0] = boardStatus;
		b[1] = operation::setRows(reverseRows);
		b[2] = operation::setRows(oRows);
		b[3] = operation::setRows(oReverseRows);
		b[4] = operation::setCols(rows);
		b[5] = operation::setCols(reverseRows);
		b[6] = operation::setCols(oRows);
		b[7] = operation::setCols(oReverseRows);
	
		for(int j = 0; j < 8; j++){
			for(int i = 0; i < 2; i++){
				fourTuple[i][getFourTupleIndex(b[j],i)] += delta;
			}
			for(int i = 0; i < 3; i++){
				sixTuple[i][getSixTupleIndex(b[j],i)] += delta;
			}
			for(int i = 0; i < 6; i++){
				littleAxe[i][getLittleAxeIndex(b[j],i)] += delta;
			}
		}
		maxTileFeature[getMaxFeatureIndex(b[0])] += delta;
		emptyTiles[getEmptyIndex(b[0])] += delta;
		availableMergeMove[getAvailableMergeMoveIndex(b[0])] += delta;
	};
	
	void saveResultToFile(int mode){
		
		fstream out;
		
		if(mode == 0)out.open("trainingResult(Binary)_before8192.txt", ios::out | ios::binary);
		if(mode == 1)out.open("trainingResult(Binary)_8192Appear.txt", ios::out | ios::binary);
		if(mode == 2)out.open("trainingResult(Binary)_16384Appear.txt", ios::out | ios::binary);
		if(mode == 3)out.open("trainingResult(Binary)_16384_8192Appear.txt", ios::out | ios::binary);
		if(mode == 4)out.open("trainingResult(Binary)_16384_8192_4096Appear.txt", ios::out | ios::binary);
		if(mode == 5)out.open("trainingResult(Binary)_32768Appear.txt", ios::out | ios::binary);
		if(mode == 6)out.open("trainingResult(Binary)_32768_8192Appear.txt", ios::out | ios::binary);
		if(mode == 7)out.open("trainingResult(Binary)_32768_8192_4096Appear.txt", ios::out | ios::binary);
		
		
		for(int i = 0; i < 2; i++){
			out.write(reinterpret_cast<char*>(fourTuple[i]), 65536*sizeof(double));
		}	
		for(int i = 0; i < 3; i++){
			out.write(reinterpret_cast<char*>(sixTuple[i]), 0x1000000*sizeof(double));
		}	
		for(int i = 0; i < 6; i++){
			out.write(reinterpret_cast<char*>(littleAxe[i]), 0x1000000*sizeof(double));
		}	
		out.write(reinterpret_cast<char*>(maxTileFeature), 1024*sizeof(double));
		out.write(reinterpret_cast<char*>(emptyTiles), 16*sizeof(double));
		out.write(reinterpret_cast<char*>(availableMergeMove), 9*sizeof(double));
		out.close();
	};

	void getResultFromFile(int mode){
		
		fstream in;
			
		if(mode == 0)in.open("trainingResult(Binary)_before8192.txt", ios::in | ios::binary);
		if(mode == 1)in.open("trainingResult(Binary)_8192Appear.txt", ios::in | ios::binary);
		if(mode == 2)in.open("trainingResult(Binary)_16384Appear.txt", ios::in | ios::binary);
		if(mode == 3)in.open("trainingResult(Binary)_16384_8192Appear.txt", ios::in | ios::binary);
		if(mode == 4)in.open("trainingResult(Binary)_16384_8192_4096Appear.txt", ios::in | ios::binary);
		if(mode == 5)in.open("trainingResult(Binary)_32768Appear.txt", ios::in | ios::binary);
		if(mode == 6)in.open("trainingResult(Binary)_32768_8192Appear.txt", ios::in | ios::binary);
		if(mode == 7)in.open("trainingResult(Binary)_32768_8192_4096Appear.txt", ios::in | ios::binary);
		
		for(int i = 0; i < 2; i++){
			in.read(reinterpret_cast<char*>(fourTuple[i]), 65536 * sizeof(double));
		}
		for(int i = 0; i < 3; i++){
			in.read(reinterpret_cast<char*>(sixTuple[i]), 0x1000000 * sizeof(double));
		}
		for(int i = 0; i < 6; i++){
			in.read(reinterpret_cast<char*>(littleAxe[i]), 0x1000000 * sizeof(double));
		}
		in.read(reinterpret_cast<char*>(maxTileFeature), 1024*sizeof(double));
		in.read(reinterpret_cast<char*>(emptyTiles), 16*sizeof(double));
		in.read(reinterpret_cast<char*>(availableMergeMove), 9*sizeof(double));
		in.close();
	};
//private:
	double** fourTuple;
	double** sixTuple;
	double** littleAxe;
	double* maxTileFeature;
	double* emptyTiles;
	double* availableMergeMove;
	//double* averageValueOfBoard;
}; */

#ifndef _TUPLENETWORK_H
#define _TUPLENETWORK_H

#include "bitboard.h"
#include "operation.h"
#include "feature.h"
#include "featureSet.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
using namespace std;

class tupleNetwork{
public:
	
	distinctTileCount distinctTile;
	mergeableTileCount mergeableTile;
	emptyTileCount emptyTile;
	layerTileCount layerTile;
	maxTileCount maxTile;
	
	axeTuple axe;
	recTangleTuple recTangle;
	combineMaxTileCount combineMax;

	tupleNetwork():featureSet(){

		rotateBoards = new board_t[4];
		isomorphicBoards = new board_t[8];
		
		
		
		// add features into tupleNetwork
		/*featureSet.push_back(&mergeableTile);
		featureSet.push_back(&distinctTile); // find useless? after print out content
		featureSet.push_back(&emptyTile);
		
		featureSet.push_back(&layerTile); // after 16+8+4+2k
		
		featureSet.push_back(&maxTile);
		//featureSet.push_back(&combineMax); // before 16+8+4k
		
		featureSet.push_back(&axe);
		featureSet.push_back(&recTangle);	*/
	}
	
	~tupleNetwork(){
		delete[] rotateBoards;
		delete[] isomorphicBoards;
	};
	
	void setFeatureSet(int featureSetSelector){
		
		featureSet.clear();
		
		if(featureSetSelector == 0){
			featureSet.push_back(&combineMax);
		}else if(featureSetSelector == 1){
			featureSet.push_back(&mergeableTile);
			featureSet.push_back(&distinctTile); // find useless? after print out content
			featureSet.push_back(&emptyTile);
			featureSet.push_back(&maxTile);
		}else if(featureSetSelector == 2){
			featureSet.push_back(&mergeableTile);
			featureSet.push_back(&distinctTile); // find useless? after print out content
			featureSet.push_back(&emptyTile);		
			featureSet.push_back(&layerTile); // after 16+8+4+2k		
			featureSet.push_back(&maxTile);
		}
		
		featureSet.push_back(&axe);
		featureSet.push_back(&recTangle);
	}
	double getValue(board_t boardStatus){
		setSymmetricBoards(boardStatus);
		double sum = 0;
		for(int i = 0; i < featureSet.size(); i++){
			sum += featureSet[i]->getScore(boardStatus);
		}
		
		return  sum;
	};
	void updateValue(board_t boardStatus, double delta){
		setSymmetricBoards(boardStatus);
		for(int i = 0; i < featureSet.size(); i++){
			featureSet[i]->updateScore(boardStatus, delta);
		}
	};
	
	void addFromOtherTuple(tupleNetwork& tN, bool sign){
		for(int i = 0; i < featureSet.size(); i++){
			featureSet[i]->addFromOtherFeature(*(tN.featureSet[i]), sign);
		}
	}

	void addSquaredValueFromOtherTuple(tupleNetwork& tN){
		for(int i = 0; i < featureSet.size(); i++){
			featureSet[i]->addSquaredValueFromOtherFeature(*(tN.featureSet[i]));
		}
	}

	void divideAllDataByN(const int& N){
		for(int i = 0; i < featureSet.size(); i++){
			featureSet[i]->divideAllDataByN(N);
		}
	}

	void saveResultToFile(const int& mode,string fileToOpen){
		
		fstream out;
		
		out.open(fileToOpen.c_str(), ios::out | ios::binary);
		
		for(int i = 0; i < featureSet.size(); i++){
			featureSet[i]->saveToFile(out);
		}
			
		out.close();
	};
	
	void saveResultToFile(const int& mode){
		
		fstream out;
		
		string fileToOpen = fileSelector(mode);
		out.open(fileToOpen.c_str(), ios::out | ios::binary);
		
		for(int i = 0; i < featureSet.size(); i++){
			featureSet[i]->saveToFile(out);
		}
			
		out.close();
	};
	
	void getResultFromFile(const int& mode, string fileToOpen){
		
		fstream in;
			
		in.open(fileToOpen.c_str(), ios::in | ios::binary);
		
		for(int i = 0; i < featureSet.size(); i++){
			featureSet[i]->loadFromFile(in);
		}
		in.close();
		/*
		for(int i = 3; i <= 6; i++){
			featureSet[i]->showContent();
		}*/
	};
	
	void getResultFromFile(const int& mode){
		
		fstream in;
			
		string fileToOpen = fileSelector(mode);
		in.open(fileToOpen.c_str(), ios::in | ios::binary);
		
		for(int i = 0; i < featureSet.size(); i++){
			featureSet[i]->loadFromFile(in);
		}
		in.close();
		/*
		for(int i = 3; i <= 6; i++){
			featureSet[i]->showContent();
		}*/
	};
	
	vector<feature*> featureSet;
private:
	string fileSelector(const int& mode){
		string fileToOpen = "";
		switch(mode){
			case 0:
				fileToOpen = "trainingResult(Binary)_before8192.txt";
				break;
			case 1:
				fileToOpen = "trainingResult(Binary)_8192Appear.txt";
				break;
			case 2:
				fileToOpen = "trainingResult(Binary)_16384Appear.txt";
				break;
			case 3:
				fileToOpen = "trainingResult(Binary)_16384_8192Appear.txt";
				break;
			case 4:
				fileToOpen = "trainingResult(Binary)_16384_8192_4096Appear.txt";
				break;
			case 5:
				fileToOpen = "trainingResult(Binary)_32768Appear.txt";
				break;
			case 6:
				fileToOpen = "trainingResult(Binary)_32768_8192Appear.txt";
				break;
			case 7:
				fileToOpen = "trainingResult(Binary)_32768_8192_4096Appear.txt";
				break;
			default:
				cout << "Wrong File to Open!" << endl;
		}
		return fileToOpen;
	}
	
	void setSymmetricBoards(const board_t& boardStatus){
		row_t rows[4];
		row_t reverseRows[4];
		row_t oRows[4];
		row_t oReverseRows[4];

		for(int i =0; i < 4; i++){
			rows[i] = operation::getRow(boardStatus,i);
			oRows[3-i] = rows[i];
			reverseRows[i] = operation::reverseRow(rows[i]);
			oReverseRows[3-i] = reverseRows[i];
		}
		
		rotateBoards[0] = boardStatus;
		rotateBoards[1] = operation::setCols(reverseRows);
		rotateBoards[2] = operation::setCols(oRows);
		rotateBoards[3] = operation::setRows(oReverseRows);
		
		isomorphicBoards[0] = boardStatus;
		isomorphicBoards[1] = operation::setRows(reverseRows);
		isomorphicBoards[2] = operation::setRows(oRows);
		isomorphicBoards[3] = operation::setRows(oReverseRows);
		isomorphicBoards[4] = operation::setCols(rows);
		isomorphicBoards[5] = operation::setCols(reverseRows);
		isomorphicBoards[6] = operation::setCols(oRows);
		isomorphicBoards[7] = operation::setCols(oReverseRows);
		
		for(int i = 0; i < featureSet.size(); i++){
				featureSet[i]->setSymmetricBoards(rotateBoards,isomorphicBoards);
		}
	};
	
	board_t* rotateBoards;
	board_t* isomorphicBoards;
};
#endif