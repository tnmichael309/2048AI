#ifndef _RNG_H
#define _RNG_H

#include <iostream>
#include <ctime>
#include <time.h>
using namespace std;

class uniform_RNG{
public:
	uniform_RNG(){
		srand(time(NULL));
		m_z = 100;
		m_w = 100;
	}
	~uniform_RNG(){;}
	void srand(unsigned int seed){
		if(seed == 0){
			m_z = rand();
			m_w = rand();
		}else{
			m_z = seed;
			m_w = seed;
		}
	}
	unsigned int rand(){
		m_z = 36969 * (m_z & 65535) + (m_z >> 16);
		m_w = 18000 * (m_w & 65535) + (m_w >> 16);
		return (m_z << 16) + m_w;
	}
private:
	unsigned int m_z;
	unsigned int m_w;
};

#endif