// 2048-TD_Lambda.cpp : 定義主控台應用程式的進入點。
//

//#include "stdafx.h"



#include "GameAI.h"
#include <fstream>
#include <iostream>
using namespace std;


int main(int argc, char* argv[])
{
	/*GameAI_TDLearning gt;
	board_t b = 45617204192617598;
	board test(b);
	test.showBoard();*/
	/*
		0       0       1024    4
		2       0       512     2048
		0       8       256     4096
		4       16      128     16384
	*/
	
	
	//GameAI_TDLearning gt;
	//gt.training(7000*1000,4,true);
	//combineMaxTileCount c;
	//board b(0xab00240012342345);
	//b.showBoard();
	//cout <<  c.getIndex(0xab00240012342345,0) << endl;
	
	/*board_t b[8] = {
		(board_t)1414733481397452926,
		(board_t)1414733481112240766,
		(board_t)45601811029832830,
		(board_t)45600711786640510,
		(board_t)45601811298264190,
		(board_t)45600712055071870,
		(board_t)189716999105684606,
		(board_t)189715899862492286
	};
	
	gt.getScoresOfBoardsFromTuples(b,8);*/
	
	//gt.getScoresFromTuples(0xedcba98745600012);
	//gt.getScoresFromTuples(0xedcba98745670012);
	//gt.getScoresFromTuples(0xedcba98700000000);
	GameAI_PLAYING_EMM ge;
	/*board b(0xff21000000000000);
	cout << "reward " << b.canMove(3) << endl;
	b.showBoard();*/
	
	ge.setResultFileName(argv[1]);
	ge.setInitialSeed(atoi(argv[2]));
	ge.playNGames(1000); // 10189
	//ge.playGame();
	//ge.findPossibleBestMoves();
	/*GameAI_TDLearning gt;
	EMM emm;
	*/
	/*board_t b = 1414733481397452926;
	board test(b);
	test.showBoard();
	
	test.init(1414733481112240766);
	test.showBoard();*/
	/*
		2       8       1024    4
		4       16      512     2048
		2       4       256     4096
		0       0       128     16384

		2       8       1024    4
		4       16      512     2048
		0       2       256     4096
		0       4       128     16384
	*/
	
	/*
	
	board_t b = 45617204192617598;
	string fileToOpen = to_string(5) + ".tuples";
	emm.reloadTuple(0, fileToOpen);
	emm.findBestMove(b);
	emm.dumpTree();
*/
	system("pause");
	return 0;
}
