/* 
 * Drag[en]gine Occlusion Mesh Module
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
