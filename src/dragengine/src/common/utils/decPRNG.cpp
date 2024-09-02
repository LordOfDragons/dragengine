/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "decPRNG.h"



// Class decPRNG
//////////////////

// Cconstructor, destructor
////////////////////////////

decPRNG::decPRNG(){
	pSeed = ( unsigned int )clock();
	pNext = pSeed;
}

decPRNG::decPRNG( unsigned int seed ){
	pSeed = seed;
	pNext = seed;
}

decPRNG::~decPRNG(){
}



// Management
///////////////

void decPRNG::Reset(){
	pNext = pSeed;
}

void decPRNG::Reset( unsigned int seed ){
	pSeed = seed;
	pNext = seed;
}



int decPRNG::RandomInt(){
	return ( int )pNextRandomValue();
}

int decPRNG::RandomInt( int lower, int upper ){
	if( upper > lower ){
		return lower + ( int )( ( ( double )pNextRandomValue() / ( double )DE_RANDOM_INT_UPPER ) * ( double )( upper - lower ) );
		
	}else{
		pNextRandomValue(); // we have to advance the generator even if the result is defined
		return lower;
	}
}

float decPRNG::RandomFloat(){
	return RandomFloat( 0.0f, 1.0f );
}

float decPRNG::RandomFloat( float lower, float upper ){
	if( upper > lower ){
		return lower + ( ( float )pNextRandomValue() / ( float )DE_RANDOM_INT_UPPER ) * ( upper - lower );
		
	}else{
		pNextRandomValue(); // we have to advance the generator even if the result is defined
		return lower;
	}
}

double decPRNG::RandomDouble(){
	return RandomDouble( 0.0, 1.0 );
}

double decPRNG::RandomDouble( double lower, double upper ){
	if( upper > lower ){
		return lower + ( ( double )pNextRandomValue() / ( double )DE_RANDOM_INT_UPPER ) * ( upper - lower );
		
	}else{
		pNextRandomValue(); // we have to advance the generator even if the result is defined
		return lower;
	}
}




// Private Functions
//////////////////////

unsigned int decPRNG::pNextRandomValue(){
	// from glibc randr implementation. using the older implementation since it is
	// simpler and there is no requirement to go for a complex pseudo random number
	// generator for typical game usage.
	unsigned int result;
	
	pNext *= 1103515245;
	pNext += 12345;
	result = ( pNext >> 16 ) % 2048;
	
	pNext *= 1103515245;
	pNext += 12345;
	result <<= 10;
	result ^= ( pNext >> 16 ) % 1024;
	
	pNext *= 1103515245;
	pNext += 12345;
	result <<= 10;
	result ^= ( pNext >> 16 ) % 1024;
	
	return result;
}
