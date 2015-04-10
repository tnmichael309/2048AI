
//#include "stdafx.h"
#include "GameAI.h"

void GameAI_PLAYING_EMM::playGame(){
	
	configure::uRNG.srand(initialSeed);
	
	board initialBoard;
	initialBoard.init(); //0xe230320000202020
	initialBoard.showBoard();

	int round = 0;
	board_t currentKey = initialBoard.getCurrentPosition();
	
	int score = 0;
	clock_t totalTime = 0;
	bool aboutToHave64kTile = false;
	int positionOf64k = -1;
	int positionOf1st32k = -1;
	int positionOf2nd32k = -1;
	
	while(!initialBoard.isEnd()){
		if(round % 2 == 0){ // my AI's turn
			clock_t start = clock();
			board_t lastKey = currentKey;
			int bestMove = emm.findBestMove(currentKey);
			
			
			totalTime += clock() - start;
			initialBoard.init(currentKey);
			
			int reward = initialBoard.canMove(bestMove);
			currentKey = initialBoard.getCurrentPosition();
			
			if(aboutToHave64kTile){
				board_t newBoard = currentKey;
				bool is32kExist = false;
				
				int firstToChange = -1;
				int secondToChange = -1;
				for(int i = 0; i < 64; i+=4){
					if(((newBoard >> i)&0xf) == 0xf){
						is32kExist = true;
						positionOf64k = 15 - i/4;
						break;
					}
				}
				
				if(!is32kExist){
					for(int i = 0; i < 64; i+=4){
						if(positionOf64k == -1 && ((newBoard >> i)&0xf) == 0xe){
							if(firstToChange == -1) firstToChange = 15 - i/4;
							else if(secondToChange == -1) secondToChange = 15 - i/4;
						}
					}
					positionOf1st32k = firstToChange;
					positionOf2nd32k = secondToChange;
				}
				
				if(is32kExist && positionOf1st32k != -1 && positionOf2nd32k != -1){
					score += 65536 + reward;
					positionOf1st32k = -1;
					positionOf2nd32k = -1;
				}else{
					score += reward;
				}
			}else score += reward;
			
			
			
		}else{ // opponent's turn
			if(!initialBoard.isFull()){
				initialBoard.insertNewPiece();
				currentKey = initialBoard.getCurrentPosition();
			}
			
			//cin.get();
		}
		
		int num32kTile = 0;
		board_t tempBoard = currentKey;
		for(int i = 0; i < 16; i++){
			int val = tempBoard&0xf;
			if(val == 0xf) num32kTile++;
			if(num32kTile >= 2) break;
			tempBoard = tempBoard >> 4;
		}
		if(num32kTile >= 2) aboutToHave64kTile = true; //cin.get();//break;
	
		
		round++;
		
		//cout << "Current score: " << score << endl;
		if(!aboutToHave64kTile) ;//initialBoard.showBoard();
		else{
			if(positionOf64k == -1){
				for(int i = 0; i < 64; i+=4){
					if(((currentKey >> i)&0xf) == 0xf){
						// record the position of the 32k tiles
						if(positionOf1st32k == -1){
							positionOf1st32k = 15 - i/4;
						}else if(positionOf2nd32k == -1){
							positionOf2nd32k = 15 - i/4;
						}
						
						// transform original board 32k -> 16k
						board_t mask = 0xf;
						mask <<= i;
						mask = ~mask;
						currentKey &= mask;
						
						mask = 0xe;
						mask <<= i;
						currentKey |= mask;
						
					}
				}
				initialBoard.init(currentKey);
			}
			
			//cout << hex << currentKey << dec << endl;
			//initialBoard.showBoard(positionOf1st32k, positionOf2nd32k, positionOf64k);
			//cin.get();
		}
	}
	
	updateStatistic(currentKey);
	
	// find max file
	int maxTile = 0;
	for(int i = 0; i < 16; i++){
		int val = currentKey&0xf;
		if(val != 0 && configure::tile_score[val] > maxTile){
			maxTile = configure::tile_score[val];
		}
		currentKey = currentKey >> 4;
	}
	
	/*
	cout.precision(14);	
	cout << "Final Score: " << score  << endl;
	cout << "Max Tile: " << maxTile << endl;
	double time = (totalTime*1.0)/CLOCKS_PER_SEC;
	cout << "Move: " << round/2 << " time: " << time << endl;
	cout << "Move per sec: " << (round/2)/time << endl;
	*/
	
	if(isDumpResult){
		fstream out;
		string fileName = resultFileName;
		out.precision(10);
		out.open(fileName.c_str(), ios::out | ios::app);
		out << initialSeed << "\t" << score << "\t" << maxTile << endl;
		out.close();
	}
	
	initialSeed ++;
};

void GameAI_PLAYING_EMM::updateStatistic(board_t b){

	int tileStatus[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	int maxVal = 0;
	for(int i = 0; i < 16; i++){
		int val = b&0xf;
		if(val != 0 && val > maxVal){
			maxVal = val;
		}
		tileStatus[val] = 1;
		b = b >> 4;
	}
	
	int index = 0;
	if(maxVal == 11){
		statistic[0] ++;
		return;
	}else if(maxVal == 12){
		statistic[1] ++;
		return;
	}else if(maxVal == 13){
		statistic[2] ++;
		return;
	}else if(maxVal == 14){
		index = 3;
	}else if(maxVal == 15){
		index = 17;
	}else return;
	
	for(int i = maxVal - 1; i >= 1; i--){
		if(tileStatus[i] == 1) index++;
		else break;
	}

	statistic[index]++;
}

void GameAI_PLAYING_EMM::playNGames(int num){
	configure::uRNG.srand(initialSeed);
	unsigned int seed;
	vector<unsigned int> vRandSeed;
	for(int i = 0; i < num; i++){
		vRandSeed.push_back(configure::uRNG.rand());
	};
	for(int i = 0; i < num; i++){
		playGame();
	};
	
	printStatistic();
};
void GameAI_PLAYING_EMM::printStatistic(){
	
	int totalNumOfGamesInRecord = 0;
	
	for(int i = 0; i < 32; i++) totalNumOfGamesInRecord += statistic[i];
	
	fstream out;
	string fileName = resultFileName;
	out.precision(10);
	out.open(fileName.c_str(), ios::out | ios::app);
	
	out << "2048:\t" << ((double)totalNumOfGamesInRecord)/10.0 << "%"<< endl;
	totalNumOfGamesInRecord -= statistic[0];
	
	out << "4096:\t" << ((double)totalNumOfGamesInRecord)/10.0 << "%"<< endl;
	totalNumOfGamesInRecord -= statistic[1];
	
	out << "8192:\t" << ((double)totalNumOfGamesInRecord)/10.0 << "%"<< endl;
	totalNumOfGamesInRecord -= statistic[2];
	
	//cout << "start" << endl;
	
	for(int i = 0; i < 14; i++){
		//cout << i << endl;
		int number = 16384;
		for(int j = 14; j >= 14 - i; j--){
			if(j != 14) out << "+";
			out << number;
			number/=2;
		}
		out << ":\t" << ((double)totalNumOfGamesInRecord)/10.0 << "%"<< endl;
		totalNumOfGamesInRecord -= statistic[3+i];
	}
	
	//cout << "a" << endl;
	
	for(int i = 0; i < 15; i++){
		int number = 32768;
		for(int j = 15; j >= 15 - i; j--){
			if(j != 15) out << "+";
			out << number;
			number/=2;
		}
		out << ":\t" << ((double)totalNumOfGamesInRecord)/10.0 << "%"<< endl;
		totalNumOfGamesInRecord -= statistic[17+i];
	}
	
	//cout << "b" << endl;

};

void GameAI_PLAYING_EMM::findPossibleBestMoves(){
	
};

void GameAI_PLAYING_EMM::setResultFileName(string s){
	resultFileName = s;
}

void GameAI_PLAYING_EMM::setInitialSeed(unsigned int seed){
	initialSeed = seed;
}

void GameAI_PLAYING_EMM::setDumpable(const bool isDump){
	isDumpResult = isDump;
}

void GameAI_TDLearning::training(int numberOfGames, int selector, const bool isSave){
	if(selector == 0){
		t.playNGames(numberOfGames, isSave);
	}else{
		if(selector <= 4 && selector >= 1){
			t.playNGamesFromFile(selector, isSave);
		}else{
			cout << "Wrong Selector Number" << endl;
			return;
		}
	}
};

void GameAI_TDLearning::getScoresFromTuples(board_t b){
	t.getScoresFromTuples(b);
};

void GameAI_TDLearning::getScoresOfBoardsFromTuples(board_t* b, int numberOfBoards){
	t.getScoresOfBoardsFromTuples(b,numberOfBoards);
};

