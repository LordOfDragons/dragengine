#include "deoglMath.h"

unsigned int oglPotOf(unsigned int value){
	// https://stackoverflow.com/a/1322548
	value--;
	
	// divide by 2^k for consecutive doublings of k up to 32, and then or the results.
	value |= value >> 1;
	value |= value >> 2;
	value |= value >> 4;
	value |= value >> 8;
	value |= value >> 16;
	
	// the result is a number of 1 bits equal to the number of bits in the original
	// number, plus 1. that's the next highest power of 2.
	return value + 1;
}
