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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoccmWeightSet.h"

#include <dragengine/common/exceptions.h>



// Class deoccmWeightSet
/////////////////////////

// Constructor, destructor
////////////////////////////

deoccmWeightSet::deoccmWeightSet(){
	pWeights = NULL;
	pCount = 0;
	pGroupedIndex = -1;
}

deoccmWeightSet::~deoccmWeightSet(){
	if( pWeights ){
		delete [] pWeights;
	}
}



// Management
///////////////

int deoccmWeightSet::GetBoneAt( int index ) const{
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pWeights[ index ].bone;
}

float deoccmWeightSet::GetWeightAt( int index ) const{
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pWeights[ index ].weight;
}

float deoccmWeightSet::GetWeightFor( int bone ) const{
	int i;
	
	for( i=0; i<pCount; i++ ){
		if( pWeights[ i ].bone == bone ){
			return pWeights[ i ].weight;
		}
	}
	
	return 0;
}

void deoccmWeightSet::Set( int bone, float weight ){
	sWeight *newArray;
	int i, j;
	
	if( weight == 0 ){
		for( i=0; i<pCount; i++ ){
			if( pWeights[ i ].bone == bone ){
				for( j=i+1; j<pCount; j++ ){
					pWeights[ j - 1 ] = pWeights[ j ];
				}
				pCount--;
				return;
			}
		}
		
	}else{
		for( i=0; i<pCount; i++ ){
			if( pWeights[ i ].bone == bone ){
				pWeights[ i ].weight = weight;
				return;
			}
		}
		
		newArray = new sWeight[ pCount + 1 ];
		if( pWeights ){
			for( i=0; i<pCount; i++ ){
				newArray[ i ] = pWeights[ i ];
			}
			delete [] pWeights;
		}
		
		pWeights = newArray;
		pWeights[ pCount ].bone = bone;
		pWeights[ pCount ].weight = weight;
		pCount++;
	}
}

void deoccmWeightSet::SetGroupedIndex( int index ){
	pGroupedIndex = index;
}

void deoccmWeightSet::RemoveAll(){
	if( pWeights ){
		delete [] pWeights;
		pWeights = NULL;
		pCount = 0;
	}
}

void deoccmWeightSet::Normalize(){
	if( pCount > 0 ){
		float sum = 0, factor;
		int i;
		
		for( i=0; i<pCount; i++ ){
			sum += pWeights[ i ].weight;
		}
		
		if( sum > 1e-5f ){
			factor = 1.0f / sum;
			
			for( i=0; i<pCount; i++){
				pWeights[ i ].weight *= factor;
			}
		}
	}
}

bool deoccmWeightSet::Equals( const deoccmWeightSet &weight ) const{
	int i;
	
	if( pCount != weight.pCount ){
		return false;
	}
	
	for( i=0; i<pCount; i++ ){
		if( fabs( pWeights[ i ].weight - weight.GetWeightFor( pWeights[ i ].bone ) ) > 0.001f ){
			return false;
		}
	}
	
	return true;
}
