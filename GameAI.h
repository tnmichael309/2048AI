#ifndef _GAMEAI_H
#define _GAMEAI_H

#include "pruneEmm.h"
#include "TDLearning.h"
#include "config.h"
#include <string>


class GameAI_PLAYING_EMM{
public:		
	GameAI_PLAYING_EMM(){		
		// always called before do something
		// initialize all the game settings
		GameSetting::init();
		initialSeed = 517;
		resultFileName = "result_playingTD+EMM.txt";
		isDumpResult = true;
		for(int i = 0; i < 32; i++) statistic[i] = 0;
	}
	~GameAI_PLAYING_EMM(){;}
	
	void playGame();
	void updateStatistic(board_t b);
	void playNGames(int numberOfGames);
	void printStatistic();
	void findPossibleBestMoves();
	void setResultFileName(string s);
	void setInitialSeed(unsigned int seed);
	void setDumpable(const bool isDump);
private:
	EMM emm;
	string resultFileName;
	unsigned int initialSeed;
	bool isDumpResult;
	int statistic[32];
};

class GameAI_TDLearning{
public:	
	GameAI_TDLearning(){
		// always called before do something
		// initialize all the game settings
		GameSetting::init();
	}
	~GameAI_TDLearning(){;}
	
	void training(int numberOfGames, int selector, const bool isSave);
	void getScoresFromTuples(board_t b);
	void getScoresOfBoardsFromTuples(board_t* b, int numberOfBoards);
private:
	TD t;
};


#endif