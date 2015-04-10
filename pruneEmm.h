#ifndef _PRUNEEMM_H
#define _PRUNEEMM_H

#include "bitboard.h"
#include "zhash.h"
#include "tupleNetwork.h"
#include "config.h"
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>

#define AI 1
#define RANDOM 2
#define IsDump 0
#define IS_ADV_EMM 0

using namespace std;


class moveScore{
public:
	int move;
	board_t afterMove;
	double score;
	int moveReward;
};

class node{
public:
	double score; // it's score 
	board_t b; // board status
	string info;
	int depth;
	int bestMove;
};

class EMM{
public:
	
	node* nodes;
	int currentPos;
	
	EMM(){
		iTupleSelect = 0;
		probability[0] = 0.9;
		probability[1] = 0.1;
		for(int i = 0; i < 3; i++){
			tN[i].setFeatureSet(0);
		}
		tN[3].setFeatureSet(1);
		tN[4].setFeatureSet(2);
		
		for(int i = 0; i < 5; i++){
			tN[i].getResultFromFile(i);	
		}
		
		nodes = new node[10000000];
		currentPos = 0;
		depthLimit = 4;
	}
	~EMM(){;}
	
	void reloadTuple(int num, string fileToOpen){
		tN[0].getResultFromFile(num, fileToOpen);
		transTable.clear();
	};
	
	
	double evaluate(board_t b){
		//b = getTransformedBoard(b);
		double result = tN[iTupleSelect].getValue(b);
		return result;
	};
		
	double expectiminmax(board_t hashKey, double alpha, int player, int depth, bool& maybeDead){
		
		board checkBoard(hashKey);
		if((depth == 0)){
			
			return evaluate(hashKey);
		}else{


			if(player == AI){

				bool isMove = false;
				double tempAlpha = alpha;
				int bestMove = -1;
				
				if(transTable.exist(hashKey,depth)){
					return transTable.retrieve(hashKey,depth);
				}
				
				{		
					vector<moveScore> v;
					for(int i = 0; i < 4; i++){
						checkBoard.init(hashKey);
						int moveReward = 0;
						if((moveReward = checkBoard.canMove(i))!=-1){
							moveScore m;
							m.afterMove = checkBoard.getCurrentPosition();
							m.move = i;
							m.score = evaluate(checkBoard.getCurrentPosition());
							m.moveReward = moveReward;
							v.push_back(m);
						}
					}	

					// can not move
					if(v.size() == 0){
						maybeDead = true;
						return (double)(-999999.0);
					}

				
					
					double tempAlphaForMaybeDead = alpha;
					int bestMoveForMaybeDead = -1;
					for(int i = 0; i < v.size(); i++){
						bool maybeDeadNextCall = false;
						double result = v[i].moveReward + expectiminmax(v[i].afterMove, tempAlpha, RANDOM, depth - 1, maybeDeadNextCall);
						
#if IS_ADV_EMM == 1
						if(result > tempAlpha && maybeDeadNextCall == false){
							tempAlpha = result;
							bestMove = v[i].move;
						}
						if(result > tempAlphaForMaybeDead){
							tempAlphaForMaybeDead = result;
							bestMoveForMaybeDead = v[i].move;
						}
#elif IS_ADV_EMM == 0
						if(result > tempAlpha){
							tempAlpha = result;
							bestMove = v[i].move;
						}
#endif

					}
#if IS_ADV_EMM == 1
					if(bestMove == -1) {
						tempAlpha = tempAlphaForMaybeDead;
						bestMove = bestMoveForMaybeDead;
						maybeDead = true;
					}
#endif
				}
				transTable.insert(hashKey,tempAlpha,depth,bestMove);	
				
				return tempAlpha;

			}else if(player == RANDOM){

				
				
				double newAlpha = 0;
				double accumulatedAlpha = 0;
				int empty = 0;
				int sum  = configure::rowSum[operation::getRow(checkBoard.m_board,0)] + configure::rowSum[operation::getRow(checkBoard.m_board,1)] + configure::rowSum[operation::getRow(checkBoard.m_board,2)] + configure::rowSum[operation::getRow(checkBoard.m_board,3)];

				for(int i =0; i < 64; i+=4){
						int val = (checkBoard.m_board >> i) & 0xf;
						if(val == 0){
							empty++;
						}
				}

				double maxScore[2];
				double minScore[2];
				for(int i = 2; i <= 4; i+=2){
					int finalSum = sum + i + (depth/2)*4;
					maxScore[i/2-1] = configure::maxScoreOfSum[finalSum];
					finalSum = sum + i + (depth/2)*2;
					minScore[i/2-1] = configure::minScoreOfSum[finalSum];
				}

				double maxSumOfLeftChild = 0;
				maxSumOfLeftChild += probability[0]* maxScore[0] + probability[1]* maxScore[1];

				// can be parallelized for depth >= 5
				double random_minimum = 0;
				for(int i =0; i < 64; i+=4){
						if(((checkBoard.m_board >> i) & 0xf ) == 0){
							board_t mask = 0xf;
							mask = mask << i;
							mask = ~mask;
							for(int k = 1; k <= 2; k++){
								board_t val = k;
								maxSumOfLeftChild -= (probability[k-1] / empty) * maxScore[k-1];
								newAlpha = (alpha - accumulatedAlpha - maxSumOfLeftChild) / (probability[k-1] /(double)empty);
								double tempAlpha = -1000000;
								
								bool maybeDeadNextCall = false;
								double result = expectiminmax((checkBoard.m_board & mask) | (val << i), tempAlpha, AI, depth - 1, maybeDeadNextCall);
								if(maybeDeadNextCall)
									maybeDead = true;
								if(accumulatedAlpha == 0 || result < random_minimum)
									random_minimum = result;
									
								accumulatedAlpha += result* probability[k-1] / empty;
								

							}
						}		
				}
#if IS_ADV_EMM == 1
				accumulatedAlpha = 0.95 * accumulatedAlpha + 0.05 * random_minimum;
#endif				
				
				return accumulatedAlpha;
			}

		}

	}

	void setSearchDepth(int depth){
		depthLimit = depth;
	}
	
	int findBestMove(board_t b){
		
		// tree search need to use original board
		// so that search about merge is correct
		board_t originalBoardForTreeSearch = b;
		
		// heurisitic should used trained tuple
		// hence need to transform board and select tuple by transformed board
		b = getTransformedBoard(b);
		originalBoardForTreeSearch = b;
		selectTuple(b);
		
		board initialBoard;
		initialBoard.init(originalBoardForTreeSearch);
		//initialBoard.showBoard();
		int bestMove = -1;
		
		clock_t startTime = clock();
		int startDepth = depthLimit;
		int endDepth = depthLimit;
		
		// iterative deepening
		while(startDepth <= endDepth){
		
			if(transTable.exist(originalBoardForTreeSearch,startDepth+1))
			{
				bestMove = transTable.getBestMove(originalBoardForTreeSearch,startDepth+1);
			}
			else
			{
				vector<moveScore> v;
				for(int i = 0; i < 4; i++){
					initialBoard.init(originalBoardForTreeSearch);
					int moveReward = 0;
					if((moveReward = initialBoard.canMove(i))!=-1){
						moveScore m;
						m.afterMove = initialBoard.getCurrentPosition();
						m.move = i;
						m.score = evaluate(initialBoard.getCurrentPosition());
						m.moveReward = moveReward;
						v.push_back(m);
					}
				}
				//sort(v.begin(),v.end(),moveScoreSort);
				double alpha = -1000000;
				double deadAlpha = -1000000;
				bestMove = -1;
				int deadBestMove = -1;
				//cout << v.size() << endl;
				for(int i = 0; i < v.size(); i++){
					bool maybeDead = false;
					double result = v[i].moveReward + expectiminmax(v[i].afterMove, alpha, RANDOM, startDepth, maybeDead);
					
					if(alpha < result/* && maybeDead == false*/){
						alpha = result;
						bestMove = v[i].move;
					}
					/*if(deadAlpha < result) {
						deadAlpha = result;
						deadBestMove = v[i].move;
					}*/
				}
				
				/*if(bestMove == -1) {
					alpha = deadAlpha;
					bestMove = deadBestMove;
				}*/
				
				transTable.insert(originalBoardForTreeSearch,alpha,startDepth+1,bestMove);
				
				
			}
			startDepth += 2;
		}
		

		return bestMove;
	};
	
	void dumpTree(){
		for(int i = 0; i < 10000000; i++){
			if(nodes[i].b == 0) break;
			for(int j = 0; j < nodes[i].depth; j++){
				cout << "--";
			}
			
			cout << fixed << nodes[i].info << "\t" << nodes[i].b << "\t:\t" << nodes[i].score ;
			if(nodes[i].info == "move" || nodes[i].info == "root") cout << " best move : " << nodes[i].bestMove;
			cout << endl;
		}
	}
	
private:

	board_t getTransformedBoard(const board_t& boardStatus){
		board_t b = boardStatus;
		
		//check if 32768 exists
		bool is32768exist = false;
		for(int i = 0; i < 16; i++){
			int val = (b>>i*4)&0xf;		
			if(val == 15){
				is32768exist = true;
				break;
			}
		}	
		if(!is32768exist) return b;
		
		// find existence of all kind of values on board
		bool isExist[15] = {false};
		for(int i = 0; i < 16; i++){
			int val = (b>>i*4)&0xf;		
			if(val != 15) isExist[val] = true;
		}
		
		// find cut point 
		int cutPoint = 0;
		for(int i = 14; i >= 1; i--){	// the smallest cut point should be 2
			if(!isExist[i]){
				cutPoint = i;
				break;
			} 
		}
		if(cutPoint == 0 || cutPoint <= 2) return b;
	
	
		// transform those points > cutPoint to a lower value 
		// ex. cut point is 4
		// 8 -> 4
		// 16 -> 8
		for(int i = 0; i < 16; i++){
			int val = (b>>i*4)&0xf;
			
			if(val > cutPoint){
				board_t mask = 0xf;
				mask = mask << i*4;
				mask = ~mask;
				
				board_t value = val - 1;
				value = value << i*4;
				b &= mask;
				b |= value;
			}
		}
		
		return b;
	}
	
	void selectTuple(const board_t& b){
		
		iTupleSelect = 0;
		
		board_t tempKey = b;	
	
		int count2kTile = 0;
		int count4kTile = 0;
		int count8kTile = 0;
		int count16kTile = 0;
		
		for(int i = 0; i < 16; i++){
			int val = tempKey&0xf;
			if(val == 11) count2kTile++;
			if(val == 12) count4kTile++;
			if(val == 13) count8kTile++;
			if(val == 14) count16kTile++;
			
			tempKey  = tempKey  >> 4;
		}
		
		if(count16kTile > 0 && count8kTile > 0 && count4kTile > 0 && count2kTile > 0) iTupleSelect = 4;
		else if(count16kTile > 0 && count8kTile > 0 && count4kTile > 1) iTupleSelect = 4;
		else if(count16kTile > 0 && count8kTile > 1) iTupleSelect = 4;
		else if(count16kTile > 1) iTupleSelect = 4;
		else if(count16kTile > 0 && count8kTile > 0 && count4kTile > 0) iTupleSelect = 3;
		else if(count16kTile > 0 && count8kTile > 0) iTupleSelect = 2;
		else if(count16kTile > 0) iTupleSelect = 1;
		else iTupleSelect = 0;
	}
	
	int getSearchDepth(board_t b){
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
		
		// count is now the number of distinct tiles on board
		if(count <= 4) return 2;
		else if(count <= 8) return 4;
		else if(count <= 10) return 6;
		else return 8;
	};
	
	int iTupleSelect;
	double probability[2];
	tupleNetwork tN[5];
	
	zHashTable transTable;
	int depthLimit;
};

#endif
