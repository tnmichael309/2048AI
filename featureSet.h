#ifndef _FEATURESET_H
#define _FEATURESET_H

#include "feature.h"
#include <fstream>
#include <string.h>
#include <iostream>
using namespace std;

class lineTuple : public feature{
public:

	lineTuple(){
		fourTuple = new double*[2];
		for(int i = 0; i < 2; i++){
			fourTuple[i] = new double[0xffff+1];
			memset(fourTuple[i],0,sizeof(double)*(0xffff+1));
		}
	}
	
	virtual ~lineTuple(){
		for(int i = 0; i < 2; i++){
			delete[] fourTuple[i];
		}
		delete[] fourTuple;
	}
	
	board_t getIndex(board_t b, int num){
		if(num!=0 && num!=1) cout << "Wrong Input Number" << endl;
		
		row_t r = operation::getCol(b,3-num);
		return r;
	}
	
	board_t getSymmetricIndex(board_t b){
		row_t r = b & 0xffff;
		r = operation::reverseRow(r);
		return r;
	}
	
	void updateScore(board_t b, double delta){
		board_t* boards = getRotatedBoards();
		
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 2; j++){
				board_t index = getIndex(boards[i],j);
				board_t symmetricIndex = getSymmetricIndex(index);
			
				fourTuple[j][index] += delta;
				if(symmetricIndex == index);
				else{
					fourTuple[j][symmetricIndex] += delta;
				}
				
			}
		}
	}
	
	double getScore(board_t b){
		board_t* boards = getRotatedBoards();
		
		double sum = 0;
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 2; j++){
				board_t index = getIndex(boards[i],j);
				board_t symmetricIndex = getSymmetricIndex(index);
			
				sum += fourTuple[j][index];
				if(symmetricIndex == index);
				else{
					sum += fourTuple[j][symmetricIndex];
				}
				
			}
		}
		
		return sum;
	}
	
	void saveToFile(fstream& out){
		for(int i = 0; i < 2; i++){
			out.write(reinterpret_cast<char*>(fourTuple[i]), 65536*sizeof(double));
		}	
	}
	
	void loadFromFile(fstream& in){
		for(int i = 0; i < 2; i++){
			in.read(reinterpret_cast<char*>(fourTuple[i]), 65536 * sizeof(double));
		}	
	}
	
private:
	double** fourTuple;
};

class recTangleTuple : public feature{
public:

	recTangleTuple(){
		sixTuple = new double*[2];
		for(int i = 0; i < 2; i++){
			sixTuple[i] = new double[0xffffff+1];
			memset(sixTuple[i],0,sizeof(double)*(0xffffff+1));
		}
	}
	
	virtual ~recTangleTuple(){
		for(int i = 0; i < 2; i++){
			delete[] sixTuple[i];
		}
		delete[] sixTuple;
	}
	
	board_t getIndex(board_t b, int num){
		
		if(num!=0 && num!=1) cout << "Wrong Input Number" << endl;
		
		board_t r1 = operation::getCol(b,num);
		board_t r2 = operation::getCol(b,num+1)>>4;
		board_t finalIndex = (r1&0xfff0) << 8ull | r2;
		
		return finalIndex;
	}
	
	void updateScore(board_t b, double delta){
		board_t* boards = getRotatedBoards();
		//if(boards == NULL) cout << "WTF" << endl;
		
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 2; j++){
				board_t index1 = getIndex(boards[i],j);
				board_t index2 = getIndex(getMirrorSymmetricBoard(boards[i]),j);
				
				if(index1 == index2 && j == 1) sixTuple[j][index1] += delta;
				else{
					sixTuple[j][index1] += delta;
					sixTuple[j][index2] += delta;
				}
				
			}
		}
	}
	
	double getScore(board_t b){
		board_t* boards = getRotatedBoards();
		
		double sum = 0;
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 2; j++){
				board_t index1 = getIndex(boards[i],j);
				board_t index2 = getIndex(getMirrorSymmetricBoard(boards[i]),j);
				
				if(j == 1) sum += sixTuple[j][index1];
				else{
					sum += sixTuple[j][index1];
					sum += sixTuple[j][index2];
				}
				
			}
		}
		
		return sum;
	}
	
	double getTupleValue(int i, board_t j){
		return sixTuple[i][j];
	}

	void addFromOtherFeature(feature& ft, bool sign){
		if(sign){
			for(int i = 0; i < 2; i++){
				for(board_t j = 0; j <= 0xffffff; j++){
					sixTuple[i][j] += ft.getTupleValue(i,j); 
				}
			}
		}else{
			for(int i = 0; i < 2; i++){
				for(board_t j = 0; j <= 0xffffff; j++){
					sixTuple[i][j] -= ft.getTupleValue(i,j); 
				}
			}
		}
		
	}

	void addSquaredValueFromOtherFeature(feature& ft){
		for(int i = 0; i < 2; i++){
			for(board_t j = 0; j <= 0xffffff; j++){
				double val = ft.getTupleValue(i,j);
				/*cout << val << " : " << val*val << endl;
				system("pause");*/
				sixTuple[i][j] += val * val; 
			}
		}
	}

	void divideAllDataByN(const int& n){
		for(int i = 0; i < 2; i++){
			for(board_t j = 0; j <= 0xffffff; j++){
				sixTuple[i][j] /= n;
			}
		}
	}

	void saveToFile(fstream& out){
		for(int i = 0; i < 2; i++){
			out.write(reinterpret_cast<char*>(sixTuple[i]), (0xffffff+1)*sizeof(double));
		}	
	}
	
	void loadFromFile(fstream& in){
		for(int i = 0; i < 2; i++){
			in.read(reinterpret_cast<char*>(sixTuple[i]), (0xffffff+1) * sizeof(double));
		}	
	}
	

private:
	double** sixTuple;
};

class axeTuple : public feature{
public:

	axeTuple(){
		sixTuple = new double*[3];
		for(int i = 0; i < 3; i++){
			sixTuple[i] = new double[0xffffff+1];
			memset(sixTuple[i],0,sizeof(double)*(0xffffff+1));
		}
	}
	
	virtual ~axeTuple(){
		for(int i = 0; i < 3; i++){
			delete[] sixTuple[i];
		}
		delete[] sixTuple;
	}
	
	board_t getIndex(board_t b, int num){
		
		if(num!=0 && num!=1 && num!=2) cout << "Wrong Input Number" << endl;
		
		board_t r1 = operation::getCol(b,num);
		board_t r2 = operation::getCol(b,num+1);
		board_t finalIndex = (r1&0xffff) << 8 | (r2&0xff);
		
		return finalIndex;
	}
	
	void updateScore(board_t b, double delta){
		board_t* boards = getRotatedBoards();
		
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 3; j++){
				board_t index1 = getIndex(boards[i],j);
				board_t index2 = getIndex(getMirrorSymmetricBoard(boards[i]),j);
				
				sixTuple[j][index1] += delta;
				sixTuple[j][index2] += delta;			
			}
		}
	}
	
	double getScore(board_t b){
		board_t* boards = getRotatedBoards();
		
		double sum = 0;
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 3; j++){
				board_t index1 = getIndex(boards[i],j);
				board_t index2 = getIndex(getMirrorSymmetricBoard(boards[i]),j);
				
				sum += sixTuple[j][index1];
				sum += sixTuple[j][index2];
			}
		}
		
		return sum;
	}
	
	double getTupleValue(int i, board_t j){
		return sixTuple[i][j];
	}

	void addFromOtherFeature(feature& ft, bool sign){
		if(sign){
			for(int i = 0; i < 3; i++){
				for(board_t j = 0; j <= 0xffffff; j++){
					sixTuple[i][j] += ft.getTupleValue(i,j); 
				}
			}
		}else{
			for(int i = 0; i < 3; i++){
				for(board_t j = 0; j <= 0xffffff; j++){
					sixTuple[i][j] -= ft.getTupleValue(i,j); 
				}
			}
		}
	}

	void addSquaredValueFromOtherFeature(feature& ft){
		for(int i = 0; i < 3; i++){
			for(board_t j = 0; j <= 0xffffff; j++){
				double val = ft.getTupleValue(i,j);
				sixTuple[i][j] += val * val; 
			}
		}
	}

	void divideAllDataByN(const int& n){
		for(int i = 0; i < 3; i++){
			for(board_t j = 0; j <= 0xffffff; j++){
				sixTuple[i][j] /= n;
			}
		}
	}

	void saveToFile(fstream& out){
		for(int i = 0; i < 3; i++){
			out.write(reinterpret_cast<char*>(sixTuple[i]), (0xffffff+1)*sizeof(double));
		}	
	}
	
	void loadFromFile(fstream& in){
		for(int i = 0; i < 3; i++){
			in.read(reinterpret_cast<char*>(sixTuple[i]), (0xffffff+1) * sizeof(double));
		}	
	}
	
private:
	double** sixTuple;
};

class combineMaxTileCount : public feature{
public:
	combineMaxTileCount(){
		combineMaxTile = new double[0xffffff+1];
		memset(combineMaxTile,0,sizeof(double)*(0xffffff+1));
	}

	virtual ~combineMaxTileCount(){
		delete [] combineMaxTile;
	}

	board_t getIndex(board_t b, int num){
		unsigned long long index = 0;

		for(int i = 60; i >= 0; i-= 4){
			int val = (b >> i)&0xf;
			int shift = val - 11;
			if(val >= 11) index += 1 << (2*shift+14); // max tile
			if(val == 0) index += 1 << 10; // empty count
			if(((60-i)/4)%4 != 3){
				int right = (b >> (i-4)) & 0xf;
				if(val != 0 && val == right) index += 1 << 5; // merge count
				if(val - right == 1 || val - right == -1) index += 1; // layer
			}
			if(((60-i)/16)%4 != 3){
				int down = (b >> (i - 16)) & 0xf;
				if(val != 0 && val == down) index += 1 << 5; // merge count
				if(val - down == 1 || val - down == -1) index += 1; // layer
			}
		}
		return index;
	}

	void updateScore(board_t b, double delta){
		combineMaxTile[getIndex(b,0)] += delta;
	}

	double getScore(board_t b){
		return combineMaxTile[getIndex(b,0)];
	}

	double getTupleValue(int i, board_t j){
		return combineMaxTile[j];
	}

	void addFromOtherFeature(feature& ft, bool sign){
		if(sign){
			for(board_t j = 0; j <= 0xffffff; j++){
				combineMaxTile[j] += ft.getTupleValue(0,j); 
			}
		}else{
			for(board_t j = 0; j <= 0xffffff; j++){
				combineMaxTile[j] -= ft.getTupleValue(0,j); 
			}
		}
	}

	void addSquaredValueFromOtherFeature(feature& ft){
		for(board_t j = 0; j <= 0xffffff; j++){
			double val = ft.getTupleValue(0,j); 
			combineMaxTile[j] += val*val;
		}
	}

	void divideAllDataByN(const int& n){
		for(board_t j = 0; j <= 0xffffff; j++){
			combineMaxTile[j] /= n;
		}
	}

	void saveToFile(fstream& out){
		out.write(reinterpret_cast<char*>(combineMaxTile), (0xffffff+1)*sizeof(double));
	}

	void loadFromFile(fstream& in){
		in.read(reinterpret_cast<char*>(combineMaxTile), (0xffffff+1)*sizeof(double));	
	}

private:
	double* combineMaxTile;
};

class maxTileCount : public feature{
public:
	maxTileCount(){
		maxTile = new double[1024];
		memset(maxTile,0,sizeof(double)*1024);
	}
	
	virtual ~maxTileCount(){
		delete [] maxTile;
	}
	
	board_t getIndex(board_t b, int num){
		unsigned long long index = 0;
			
		for(int i = 0; i < 64; i+= 4){
			int val = (b >> i)&0xf;
			int shift = val - 11;
			if(val >= 11) index += 1 << 2*shift;
		}
		return index;
	}
	
	void updateScore(board_t b, double delta){
		maxTile[getIndex(b,0)] += delta;
	}
	
	double getScore(board_t b){
		return maxTile[getIndex(b,0)];
	}

	void saveToFile(fstream& out){
		out.write(reinterpret_cast<char*>(maxTile), 1024*sizeof(double));
	}
	
	void loadFromFile(fstream& in){
		in.read(reinterpret_cast<char*>(maxTile), 1024*sizeof(double));	
	}
	
	void showContent(){
		cout << "maxTile" << endl;
		for(int i = 0; i < 1024; i++){
			cout << i << "\t: "<< maxTile[i] << endl;
		}
	}
	
	
private:
	double* maxTile;
};

class emptyTileCount : public feature{
public:
	emptyTileCount(){
		emptyTile = new double[16];
		memset(emptyTile,0,sizeof(double)*16);
	}
	
	virtual ~emptyTileCount(){
		delete [] emptyTile;
	}
	
	board_t getIndex(board_t b, int num){
		unsigned long long index = 0;
			
		for(int i = 0; i < 64; i+= 4){
			int val = (b >> i)&0xf;
			if(val == 0) index ++;
		}
		return index;
	}
	
	void updateScore(board_t b, double delta){
		emptyTile[getIndex(b,0)] += delta;
	}
	
	double getScore(board_t b){
		return emptyTile[getIndex(b,0)];
	}

	void saveToFile(fstream& out){
		out.write(reinterpret_cast<char*>(emptyTile), 16*sizeof(double));
	}
	
	void loadFromFile(fstream& in){
		in.read(reinterpret_cast<char*>(emptyTile), 16*sizeof(double));	
	}
	
	void showContent(){
		cout << "emptyTile" << endl;
		for(int i = 0; i < 16; i++){
			cout << i << "\t: "<< emptyTile[i] << endl;
		}
	}
	
private:
	double* emptyTile;
};

class mergeableTileCount : public feature{
public:
	mergeableTileCount(){
		mergeableTile = new double[24];
		memset(mergeableTile,0,sizeof(double)*24);
	}
	
	virtual ~mergeableTileCount(){
		delete [] mergeableTile;
	}
	
	board_t getIndex(board_t b, int num){
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
	}
	
	void updateScore(board_t b, double delta){
		mergeableTile[getIndex(b,0)] += delta;
	}
	
	double getScore(board_t b){
		return mergeableTile[getIndex(b,0)];
	}

	void saveToFile(fstream& out){
		out.write(reinterpret_cast<char*>(mergeableTile), 24*sizeof(double));
	}
	
	void loadFromFile(fstream& in){
		in.read(reinterpret_cast<char*>(mergeableTile), 24*sizeof(double));	
	}
	void showContent(){
		cout << "mergeableTile" << endl;
		for(int i = 0; i < 24; i++){
			cout << i << "\t: " << mergeableTile[i] << endl;
		}
	}
private:
	double* mergeableTile;
};

class distinctTileCount : public feature{
public:
	distinctTileCount(){
		distinctTile = new double[16];
		memset(distinctTile,0,sizeof(double)*16);
	}
	
	virtual ~distinctTileCount(){
		delete [] distinctTile;
	}
	
	board_t getIndex(board_t b, int num){
		board_t bitset = 0;
		while (b) {
			bitset |= 1<<(b & 0xf);
			b >>= 4;
		}
	 
		// Don't count empty tiles.
		bitset >>= 1;
	 
		board_t count = 0;
		while (bitset) {
			bitset &= bitset - 1;
			count++;
		}
		if(!(count>=0 && count <= 15))
		{
			cout << count << endl;
			return 0;
		}
		return count;
	}
	
	void updateScore(board_t b, double delta){
		distinctTile[getIndex(b,0)] += delta;
	}
	
	double getScore(board_t b){
		return distinctTile[getIndex(b,0)];
	}

	void saveToFile(fstream& out){
		out.write(reinterpret_cast<char*>(distinctTile), 16*sizeof(double));
	}
	
	void loadFromFile(fstream& in){
		in.read(reinterpret_cast<char*>(distinctTile), 16*sizeof(double));	
	}
	
	void showContent(){
		cout << "distinctTile" << endl;
		for(int i = 0; i < 16; i++){
			cout << i << "\t: " << distinctTile[i] << endl;
		}
	}
private:
	double* distinctTile;
};

class layerTileCount : public feature{
public:
	layerTileCount(){
		layerTile = new double[24];
		memset(layerTile,0,sizeof(double)*24);
	}
	
	virtual ~layerTileCount(){
		delete [] layerTile;
	}
	
	board_t getIndex(board_t b, int num){
		unsigned long long index = 0;

		for(int i = 60; i >= 0; i-= 4){
			int val = (b >> i)&0xf;
		
			if(((60-i)/4)%4 != 3){
				int right = (b >> (i-4)) & 0xf;
				if(val - right == 1 || val - right == -1) index += 1; // layer
			}
			if(((60-i)/16)%4 != 3){
				int down = (b >> (i - 16)) & 0xf;
				if(val - down == 1 || val - down == -1) index += 1; // layer
			}
		}
		return index;
	}
	
	void updateScore(board_t b, double delta){
		layerTile[getIndex(b,0)] += delta;
	}
	
	double getScore(board_t b){
		return layerTile[getIndex(b,0)];
	}

	double getTupleValue(int i, board_t j){
		return layerTile[j];
	}

	void addFromOtherFeature(feature& ft, bool sign){
		if(sign){
			for(board_t j = 0; j < 24; j++){
				layerTile[j] += ft.getTupleValue(0,j); 
			}
		}else{
			for(board_t j = 0; j < 24; j++){
				layerTile[j] -= ft.getTupleValue(0,j); 
			}
		}
	}

	void addSquaredValueFromOtherFeature(feature& ft){
		for(board_t j = 0; j < 24; j++){
			double val = ft.getTupleValue(0,j); 
			layerTile[j] += val*val;
		}
	}

	void divideAllDataByN(const double& n){
		for(board_t j = 0; j < 24; j++){
			layerTile[j] /= n;
		}
	}
	
	void divideByOtherTuple(feature& ft){
		for(board_t j = 0; j < 24; j++){
			double val = ft.getTupleValue(0,j);
			if(val != 0)layerTile[j] /= val;
		}
	}
	
	void saveToFile(fstream& out){
		out.write(reinterpret_cast<char*>(layerTile), 24*sizeof(double));
	}
	
	void loadFromFile(fstream& in){
		in.read(reinterpret_cast<char*>(layerTile), 24*sizeof(double));	
		cout << "layerTile" << endl;
		for(board_t j = 0; j < 24; j++){
			cout << layerTile[j] << endl;
		}
	}
	void showContent(){
		cout << "layerTile" << endl;
		for(int i = 0; i < 24; i++){
			cout << i << "\t: " << layerTile[i] << endl;
		}
	}
	
private:
	double* layerTile;
};
#endif