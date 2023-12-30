#include<iostream>
#include<cstdlib>
#include "random.h"


int randomNum(int start, int end) {
	int randomNumber;
	srand((unsigned)time(NULL));
	int random = rand();
	randomNumber = start + (random % end);
	return randomNumber;
};