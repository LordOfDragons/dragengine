/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
