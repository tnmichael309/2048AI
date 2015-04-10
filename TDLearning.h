#ifndef _TDLEARNING_H
#define _TDLEARNING_H

#include "bitboard.h"
#include "tupleNetwork.h"
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <math.h>
#include <limits>
using namespace std;

#define SELF_PLAYING_MODE 0
#define TEACHING_MODE 1
#define LEARNING_ENABLED 1



class state{
public:
	board_t moveBoard;
	board_t addedBoard;
};

class bestMoveNode{
public:
	int bestMove;
	int reward;
	board_t moveBoard;
};

class TD{
public:
	TD(){
		tN = new tupleNetwork[5];
		for(int i = 0; i < 16; i++){
			maxRepeatNum[i] = 0;
		}
		meanTuple.getResultFromFile(0,"MeanTuple");
		deviationTuple.getResultFromFile(0,"VariationTuple");
		for(int i = 0; i < 5; i++){
			tN[i].getResultFromFile(i);
		}
		LEARNING_RATE = 0.00025;
		configure::uRNG.srand(356038);
		boards.open("16+8+4+2k.boards", ios::binary | ios::in | ios::out | ios::app);
		scores.open("16+8+4+2k.scores", ios::binary | ios::in | ios::out | ios::app);
		boardsToSave.open("16+8+4+2k.boards", ios::binary | ios::in | ios::out | ios::app);
		scoresToSave.open("16+8+4+2k.scores", ios::binary | ios::in | ios::out | ios::app);
	};
	
	~TD(){;};
	
	void getScoresFromTuples(board_t b){
		
	};
	
	void getScoresOfBoardsFromTuples(board_t* b, int numberOfBoards){
		
		
		
	};
	
	
	double evaluate(board_t b, int dir, bool& bCanMove){
		board checkBoard(b);
		int result = checkBoard.canMove(dir);
		if(result == -1) return 0;
		else {
			
			bCanMove = true;
			double answer = result + tN[currentTN].getValue(checkBoard.getCurrentPosition());
			return answer;
		}
	};
	int findBestMove(board_t aBoard){
		int nextDir = -1;
		double maxScore = -1000000.0;
	
		// td 0.5

		board b(aBoard);
		//b.showBoard();

		bool isFirst = true;
		for(int i = 0; i < 4;i++){
			bool bCanMove = false;
			double result = evaluate(aBoard,i, bCanMove);
			
			if(bCanMove && isFirst){
				nextDir = i;
				maxScore = result;
				isFirst = false;
			}
			if(bCanMove && result > maxScore){
				nextDir = i;
				maxScore = result;
			}
		}
		
		
		return nextDir;
	};

	
	// for 1 step lambda
	void learnEvaluation(board_t oBoard, int dir, int reward, board_t mBoard, board_t aBoard){
		
		int nextDir = -1;
		double maxScore = -1000000.0;
	
		// td 0.5
		for(int i = 0; i < 4;i++){
			bool bCanMove = false;
			double result = evaluate(aBoard,i, bCanMove);
			if(bCanMove && result > maxScore){
				nextDir = i;
				maxScore = result;
			}
		}

		// td 1.5
		//nextDir = findBestMove(aBoard);
		
		board checkBoard(aBoard);
		int nextReward = 0;
		if(nextDir != -1){
			nextReward = checkBoard.canMove(nextDir);
			tN[currentTN].updateValue(mBoard,LEARNING_RATE*((double)nextReward+tN[currentTN].getValue(checkBoard.getCurrentPosition())-tN[currentTN].getValue(mBoard)));
		}else{
			tN[currentTN].updateValue(mBoard,LEARNING_RATE*((double)nextReward+ 0 -tN[currentTN].getValue(mBoard)));
		}
	};


	// for 2 step lambda
	void learnEvaluation(board_t oBoard, int dir, int reward, board_t mBoard, board_t aBoard, board_t mBoard2, board_t aBoard2){
		
		double firstScore = 0;
		double secondScore = 0;
		bool isEnd = false;
		int nextDir = findBestMove(aBoard);
		
		board checkBoard(aBoard);
		int nextReward = 0;
		if(nextDir != -1) nextReward = checkBoard.canMove(nextDir);
		else isEnd = true;
		firstScore = (1-lambda)*(nextReward + tN[currentTN].getValue(checkBoard.getCurrentPosition()));

		if(!isEnd){
			nextDir = findBestMove(aBoard2);
			checkBoard.init(aBoard2);
			nextReward = 0;
			if(nextDir != -1) nextReward = checkBoard.canMove(nextDir);
			else;
			secondScore = lambda*(nextReward + tN[currentTN].getValue(checkBoard.getCurrentPosition()));
		}else;
		
		//cout << firstScore << "\t" << secondScore << endl;
		
		tN[currentTN].updateValue(mBoard,LEARNING_RATE*(firstScore+secondScore-tN[currentTN].getValue(mBoard)));
	};

	// update currentTN's boardChains
	void updateEvaluation(){
		/*
		for(int i = boardChains[currentTN].size()-1; i >= 0; i--){
				learnEvaluation(0, 0, 0, boardChains[currentTN][i].moveBoard , boardChains[currentTN][i].addedBoard);
		}*/
		
		vector<bestMoveNode> v;
		
		for(int i = 0; i < boardChains[currentTN].size(); i++){
			int nextDir = findBestMove(boardChains[currentTN][i].addedBoard);
			board checkBoard(boardChains[currentTN][i].addedBoard);
			int nextReward = 0;
			if(nextDir != -1) nextReward = checkBoard.canMove(nextDir);
			bestMoveNode b;
			b.bestMove = nextDir; 
			b.reward = nextReward;
			b.moveBoard = checkBoard.getCurrentPosition();
			v.push_back(b);
		}
		
		lambda = 0.5;
		for(int i = boardChains[currentTN].size()-1; i >= 0; i--){
			
			int size = 5;
			double score = 0;
			int totalReward = 0;
			for(int j = 0; j < size && (i+j) < boardChains[currentTN].size(); j++){
				
				double weight = 0;
				if(j != size - 1){
					weight = (1-lambda)*pow(lambda, j*1.0);
				}else{
					weight = pow(lambda, j*1.0);
				}
				
				totalReward += v[i].reward;
				score += weight * (totalReward + tN[currentTN].getValue(v[i].moveBoard));
			}
		
			tN[currentTN].updateValue(boardChains[currentTN][i].moveBoard,LEARNING_RATE*(score-tN[currentTN].getValue(boardChains[currentTN][i].moveBoard)));
		
		}
	}

	double playGame(){
		
		double score = 0.0;
		board checkBoard;
		checkBoard.init();
		currentTN = 0;
		
		

		bool isFirst8192 = true;
		bool isFirst16384 = true;
		bool isFirst8192And16384 = true;
		
		while(!checkBoard.isEnd()){
			int nextDir = -1;
			double maxScore = -100000000.0;
			
			/*	
			for(int i = 0; i < 4;i++){
				bool bCanMove = false;
				double result = evaluate(checkBoard.getCurrentPosition(),i, bCanMove);

				if(bCanMove && (result > maxScore)){
					nextDir = i;
					maxScore = result;
				}
			}*/
			nextDir = findBestMove(checkBoard.getCurrentPosition());

			board_t oBoard = checkBoard.getCurrentPosition();
			int reward = checkBoard.canMove(nextDir);
			board_t mBoard = checkBoard.getCurrentPosition();
			if(checkBoard.isFull()) cout << "Why Full?" << endl;
			else checkBoard.insertNewPiece();
			board_t aBoard = checkBoard.getCurrentPosition();

			if(LEARNING_ENABLED == 1){
				state s;
				s.moveBoard = mBoard;
				s.addedBoard = aBoard;
				boardChains[0].push_back(s);
			}

			score += reward;
			
			board_t b = aBoard;
			int maxTile = 0;
			
			for(int i = 0; i < 64; i+=4){
				int val = (b>>i)&0xf;
				if(configure::tile_score[val] > maxTile){
					maxTile = configure::tile_score[val];
				}			
			}
			
			bool is8192Appear = false;
			bool is16384Appear = false;
			
			for(int i = 0; i < 64; i+=4){
				int val = (b>>i)&0xf;
				
				if(configure::tile_score[val] == 8192) is8192Appear = true;
				if(configure::tile_score[val] == 16384) is16384Appear = true;
			}
				
			if(is16384Appear && isFirst16384){
				
				boardsToSave.write((char*)&aBoard,sizeof(board_t));
				scoresToSave.write((char*)&score,sizeof(double));
				
				numberOfBoardsCollected++;
				isFirst16384 = false;
				break;
			}
		}

		

		if(boardChains[0].size() != 0){
			updateEvaluation();
			boardChains[0].clear();
		}

		board_t b = checkBoard.getCurrentPosition();
		globalMaxTile= 0;
		for(int i = 0; i < 64; i+=4){
			int val = (b>>i)&0xf;
			if(configure::tile_score[val] > globalMaxTile){
				globalMaxTile = configure::tile_score[val];
			}
		}
		if(globalMaxTile >= 2048) winNum++;
		
		//meanTuple.addFromOtherTuple(tN[0], true);
		//deviationTuple.addSquaredValueFromOtherTuple(tN[0]);

		return score;
	};


	void playNGamesFromFile(int num, bool isUpdate){
		
		currentTN = num;
		tN[currentTN].getResultFromFile(currentTN);
		
		
		double* fileScores = new double[100000];
		board_t* fileBoards = new board_t[100000];
		scores.read(reinterpret_cast<char*>(fileScores), 100000 * sizeof(double));
		boards.read(reinterpret_cast<char*>(fileBoards), 100000 * sizeof(board_t));
		
		int iGameCount = 1;
		double avgScore = 0.0;
		int maxScore = 0.0;
		int tempMaxTile = 0;
		int maxCount = 0;
		int tile_num[5] = {0,0,0,0,0};
		numberOfBoardsCollected = 0;
		double totalScore = 0;
		
	/*	for(int i = 0; i < 100000; i++){
			double score = fileScores[i%100000];
			board_t startBoard = fileBoards[i%100000];
			board checkBoard;
			checkBoard.init(startBoard);
			checkBoard.showBoard();
		}
		
		return;*/
		
		for(int i = 0 ; i < 7000*1000 /*&& numberOfBoardsCollected <= 100000*/; i++){
			
			//cout << "Start Game" << endl;
			double score = fileScores[i%100000];
			board_t startBoard = fileBoards[i%100000];
			
			//cout << "Score: " << score << "\tBoard: " << startBoard << endl;
			
			board checkBoard;
			checkBoard.init(startBoard);
			
			
			bool isFirstMeet = true;
			bool isFirstMeet16384 = true;
			bool isFirstMeet16384_8192 = true;
			bool isFirstMeet16384_8192_4096 = true;
			bool isFirstMeet16384_8192_4096_2048 = true;
			bool isFirstMeet32768 = true;
			
			while(1){
				int nextDir = -1;
				double maxScore = -1000000.0;
				
				for(int i = 0; i < 4;i++){
					bool bCanMove = false;
					double result = evaluate(checkBoard.getCurrentPosition(),i, bCanMove);

					if(bCanMove && result > maxScore){
						nextDir = i;
						maxScore = result;
					}
				}
				
				if(nextDir == -1){
					break;
				}

				board_t oBoard = checkBoard.getCurrentPosition();
				int reward = checkBoard.canMove(nextDir);
				board_t mBoard = checkBoard.getCurrentPosition();
				if(checkBoard.isFull()) cout << "Why Full?" << endl;
				else checkBoard.insertNewPiece();
				board_t aBoard = checkBoard.getCurrentPosition();

				
				if(LEARNING_ENABLED == 1){
					state s;
					s.moveBoard = mBoard;
					s.addedBoard = aBoard;
					boardChains[currentTN].push_back(s);
				}

				score += reward;
			
				board_t b = aBoard;
				int maxTile = 0;
				bool is2048Appear = false;
				bool is4096Appear = false;
				bool is8192Appear = false;
				bool is16384Appear = false;
				bool is32768Appear = false;
				
				for(int i = 0; i < 64; i+=4){
					int val = (b>>i)&0xf;
					
					if(configure::tile_score[val] == 2048) is2048Appear = true;
					if(configure::tile_score[val] == 4096) is4096Appear = true;
					if(configure::tile_score[val] == 8192) is8192Appear = true;
					if(configure::tile_score[val] == 16384) is16384Appear = true;
					if(configure::tile_score[val] == 32768) is32768Appear = true;
				}
				
				if(is4096Appear && is8192Appear && is16384Appear && isFirstMeet16384_8192_4096){
					isFirstMeet16384_8192_4096 = false;
					tile_num[2] ++;
				}else if(is2048Appear && is4096Appear && is8192Appear && is16384Appear && isFirstMeet16384_8192_4096_2048){
					isFirstMeet16384_8192_4096_2048 = false;
					tile_num[4] ++;
				}else if(is8192Appear && is16384Appear && isFirstMeet16384_8192){
					isFirstMeet16384_8192 = false;
					tile_num[1] ++;
					
					
				}else if(is16384Appear && isFirstMeet16384){
					isFirstMeet16384 = false;
					tile_num[0] ++;
				}else if(is32768Appear && isFirstMeet32768){
					isFirstMeet32768 = false;
					tile_num[3] ++;
				}else;
				
				/*
				if(is2048Appear && is4096Appear && is8192Appear && is16384Appear){
				
					boardsToSave.write((char*)&aBoard,sizeof(board_t));
					scoresToSave.write((char*)&score,sizeof(double));
					//checkBoard.showBoard();
					//cin.get();
					numberOfBoardsCollected++;
					
					break;
				}*/
				
			}
			
			
			if(boardChains[currentTN].size() != 0){
				updateEvaluation();
				boardChains[currentTN].clear();
			}
			//checkBoard.showBoard();
			//cin.get();
			board_t b = checkBoard.getCurrentPosition();
			for(int i = 0; i < 64; i+=4){
				int val = (b>>i)&0xf;
				if(configure::tile_score[val] > tempMaxTile){
					tempMaxTile = configure::tile_score[val];
					//cout << tempMaxTile << endl;
					maxCount = 1;
				}else if(configure::tile_score[val] == tempMaxTile){
					maxCount++;
				}
				
				//cout << val << " : " << configure::tile_score[val] << endl;
			}
			//cout  << endl;
			if(score > maxScore) maxScore = score;
			avgScore += score;
			//totalScore += score;
			
			if(iGameCount % 1000 == 0){
				//cout << "Boards Collected:" << numberOfBoardsCollected << endl;
				cout << "Round: " << iGameCount << "\tAvg. score: " << avgScore/1000 
				<< "\tMax Score: " << maxScore 
				<< "\tMax Tile: " << tempMaxTile
				<< "\t#: " << maxCount
				<< "\t#of 16K: " << tile_num[0]
				<< "\t#of 16+8K: " << tile_num[1]
				<< "\t#of 16+8+4K: " << tile_num[2]
				<< "\t#of 16+8+4+2K: " << tile_num[4]
				<< "\t#of 32K: " << tile_num[3] << endl;
				
				avgScore = 0.0;
				maxScore = 0.0;
				maxCount = 0;
				tempMaxTile = 0;
				for(int i = 0; i < 5; i++) tile_num[i] = 0;
			}
			if(iGameCount % 10000 == 0 && isUpdate) tN[currentTN].saveResultToFile(currentTN);
			
			iGameCount++;
		}
		//cout << "Tuple:\t" << num << "\t:\t" << totalScore/100000 << endl;
		//in.close();
	};
	

	void playNGames(int num, bool isUpdate){
		//cout << isUpdate << endl;
		
		currentTN = 0;
		
		fstream out;
		out.open("b16k.out", ios::out | ios::app);

		double maxScore = 0.0;
		double avgScore = 0.0;
		int localMaxTile = 0;
		int maxCount = 0;
		int tupleCollectCount = 0;
		winNum = 0;
		numOf8192And16384 = 0;
		numberOfBoardsCollected = 0;
		
		//cout.precision(std::numeric_limits<double>::digits10 + 100);
				
		//board_t b = 0xedcbabcd00000000;
		tN[0].getResultFromFile(0);
		//cout << tN[0].getValue(b) << endl;
		
		for(int i = 1; i <= num && numberOfBoardsCollected <= 100000; i++){
			
			double result = playGame();
			/*cout << result << endl;
			system("pause");*/
			avgScore += result;
			

			if(result > maxScore) maxScore = result;
			if(globalMaxTile > localMaxTile){
				localMaxTile = globalMaxTile;
				maxCount = 1;
			}
			else if(globalMaxTile == localMaxTile) maxCount++;
			
			if(i%1000 == 0){
				out << "Boards Collected: " << numberOfBoardsCollected << "\t";
				//out << "Tuples Collected:" << tupleCollectCount << "\t";
				out << "Round: " << i << "\tAvgScore: " << avgScore/1000 
				<< "\tMax Score: " << maxScore 
				<< "\tWin Rate: " << (double)winNum*1.0/1000 
				<< "\tMax Tile: " << localMaxTile 
				<< "\t#: " << maxCount 
				<< "\t#of 16384+8192: " << numOf8192And16384 << endl;
				avgScore = 0;
				winNum = 0;
				maxScore = 0;
				localMaxTile = 0;
				maxCount = 0;
				numOf8192And16384 = 0;
				
			}
			if(i%10000 == 0 && isUpdate) {
				tN[0].saveResultToFile(0);		
			}
			/*
			if(i%100 == 0){
				
				string fileToOpen = to_string(tupleCollectCount) + ".tuples";
				tupleCollectCount++;
				
				tN[0].saveResultToFile(0,fileToOpen);
			}*/
		}
		/*
		meanTuple.divideAllDataByN(num); // E(x)
		deviationTuple.divideAllDataByN(num); // E(x^2)
		tupleNetwork tempTuple;
		tempTuple.addSquaredValueFromOtherTuple(meanTuple); // E(x)^2
		deviationTuple.addFromOtherTuple(tempTuple, false); // E(x^2) - E(x)^2
		
		meanTuple.saveResultToFile(0,"MeanTuple");
		deviationTuple.saveResultToFile(0,"VariationTuple");*/
	};

private:
	double LEARNING_RATE;
	tupleNetwork* tN;
	tupleNetwork deviationTuple;
	tupleNetwork meanTuple;

	int currentTN;
	vector<state> boardChains[5];
	int winNum;
	int numOf8192And16384;
	int globalMaxTile;
	int maxRepeatNum[16];
	double lambda;
	int numberOfBoardsCollected;
	fstream boards;
	fstream scores;
	fstream boardsToSave;
	fstream scoresToSave;
};


#endif
