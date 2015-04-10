#ifndef _CONFIG_H
#define _CONFIG_H

#include "rng.h"
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define COMMON_RATIO 0.25

typedef uint16_t row_t;
typedef uint64_t board_t;


class moveRightInfo{
public:
	row_t r;
	int score;
};

class configure{
public:

	static double* COMMON_RATIOS;
	static int* tile_score;
	static double* ct_score;
	static double** line_score;
	static double* maxScoreOfSum;
	static double* minScoreOfSum;
	static int* rowSum;
	static int* log2val;
	static moveRightInfo* rightInfo;
	static uniform_RNG uRNG;
	/*
	double COMMON_RATIOS[16];
	int tile_score[16];
	double ct_score[16*16];
	double line_score[65536][4];
	double maxScoreOfSum[65536];
	double minScoreOfSum[65536];
	int rowSum[0xffff];
	int log2val[65536];
	moveRightInfo rightInfo[65536]; */
};
/*extern configure config;*/

class GameSetting{
public:
	static void init(){
		init_ratio_table();
		init_tile_score();
		init_ct_score();
		init_score_table();
		init_log2();
		init_right_info();
		init_MinMaxSum();
		init_rowSum();
	}
	
	static void init_random_generator(){
		uniform_RNG uRNG;
		configure::uRNG = uRNG;
		configure::uRNG.srand(time(NULL));
		srand(time(NULL));
	}
	
	static void init_ratio_table(){
		
		configure::COMMON_RATIOS[0] = 1;
		for(int i = 1; i < 16; i++){
			configure::COMMON_RATIOS[i] = configure::COMMON_RATIOS[i-1] * COMMON_RATIO;
		}
	}

	static void init_tile_score(){
		configure::tile_score[0] = 0;
		configure::tile_score[1] = 2;
		for(int i = 2; i < 16; i++){
			configure::tile_score[i] = configure::tile_score[i-1] * 2;
		}
	}

	static void init_ct_score(){
		for(int i = 0; i < 16; i++){
			for(int j = 0; j < 16; j ++){
				configure::ct_score[i*16+j] = configure::COMMON_RATIOS[i]*configure::tile_score[j];
			}
		}
	}

	static void init_score_table(){

		
		configure::line_score = new double*[65536];
		for(int i = 0; i < 65536; i++){
			configure::line_score[i] = new double[4];
		}
		
		int mask = 0xf;
		for(int num = 0; num < 65536; num++){

			int line_num[4];
			for(int i = 0; i < 4; i++){
				line_num[i] = ((num>>(4*i)) & mask);
			}

			configure::line_score[num][0]	=	configure::tile_score[line_num[0]]		*	configure::COMMON_RATIOS[0]
						+	configure::tile_score[line_num[1]]		*	configure::COMMON_RATIOS[1]
						+	configure::tile_score[line_num[2]]		*	configure::COMMON_RATIOS[2]
						+	configure::tile_score[line_num[3]]		*	configure::COMMON_RATIOS[3];
			configure::line_score[num][1]	=	configure::tile_score[line_num[3]]		*	configure::COMMON_RATIOS[4]
						+	configure::tile_score[line_num[2]]		*	configure::COMMON_RATIOS[5]
						+	configure::tile_score[line_num[1]]		*	configure::COMMON_RATIOS[6]
						+	configure::tile_score[line_num[0]]		*	configure::COMMON_RATIOS[7];
			configure::line_score[num][2]	=	configure::tile_score[line_num[0]]		*	configure::COMMON_RATIOS[8]
						+	configure::tile_score[line_num[1]]		*	configure::COMMON_RATIOS[9]
						+	configure::tile_score[line_num[2]]		*	configure::COMMON_RATIOS[10]
						+	configure::tile_score[line_num[3]]		*	configure::COMMON_RATIOS[11];
			configure::line_score[num][3]	=	configure::tile_score[line_num[3]]		*	configure::COMMON_RATIOS[12]
						+	configure::tile_score[line_num[2]]		*	configure::COMMON_RATIOS[13]
						+	configure::tile_score[line_num[1]]		*	configure::COMMON_RATIOS[14]
						+	configure::tile_score[line_num[0]]		*	configure::COMMON_RATIOS[15];
		}

	}

	static void init_log2(){
		int num = 1;
		for(int i = 0; i < 65536; i++){
			configure::log2val[i] = 0;
		}
		for(int i = 0; i < 16; i++){
			configure::log2val[num] = i;
			num *= 2;
		}
	}
	
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
	} 
	
	static void init_right_info(){
		for(int i = 0; i < 65536; i++){
			row_t num = i;
			int score = moveRight(num);
			configure::rightInfo[i].r = num;
			configure::rightInfo[i].score = score;
		}
	}

	static void init_MinMaxSum(){
		configure::maxScoreOfSum[0] = 0;
		configure::minScoreOfSum[0] = 0;
		for(int num = 1; num < 32768; num++){
			int finalSum = num * 2;
			int maxVal = 32768;
			board_t maxBoard = 0;
			board_t minBoard = 0;
			int pow = 15;
			int count = 0;
			while(1){
				int val = finalSum / maxVal;
				if(val > 0){
					board_t addVal = pow;
					configure::maxScoreOfSum[num*2] += configure::tile_score[pow] * configure::COMMON_RATIOS[count];
					configure::minScoreOfSum[num*2] += configure::tile_score[pow] * configure::COMMON_RATIOS[15-count];
					//maxBoard = maxBoard | (addVal << 4*count);
					//minBoard = minBoard | (addVal << 4*(15-count));
					finalSum -= maxVal;
					count ++;
					if(finalSum == 0) break;
				}
				maxVal /= 2;
				pow --;
			}
			
		}
		//cout << maxScoreOfSum[4096] << endl;
		//cout << minScoreOfSum[4096] << endl;
		//system("pause");
	}

	static void init_rowSum(){
		for(int r = 0; r < 65536; r++){
			int sum = 0;
			for(int i =0; i < 16; i+=4){
				int val = (r >> i) & 0xf;	
				configure::rowSum[r] += configure::tile_score[val];
			}
		}
	}
};

#endif