#include <stdio.h>
#include <stdlib.h>
#include "detCase.h"

// class detCase
//////////////////
detCase::detCase(){
	pSubTest = 0;
}
void detCase::SetSubTestNum(int Num){
	pSubTest = Num;
	printf(",%i", Num);
}

