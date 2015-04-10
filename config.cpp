//#include "stdafx.h"
#include "config.h"

double* configure::COMMON_RATIOS = new double[16];
int* configure::tile_score = new int[16];;
double* configure::ct_score = new double[16*16];
double** configure::line_score = 0;
double* configure::maxScoreOfSum = new double[65536];
double* configure::minScoreOfSum = new double[65536];
int* configure::rowSum = new int[65536];
int* configure::log2val = new int[65536];
moveRightInfo* configure::rightInfo = new moveRightInfo[65536];
uniform_RNG configure::uRNG = uniform_RNG();