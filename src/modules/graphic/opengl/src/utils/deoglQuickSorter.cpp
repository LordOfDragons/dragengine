/* 
 * Drag[en]gine OpenGL Graphic Module
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
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "deoglQuickSorter.h"

#include <dragengine/common/exceptions.h>



// Class deoglQuickSorter
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglQuickSorter::deoglQuickSorter(){
	pElements = NULL;
	pScores = NULL;
	pCount = 0;
	pSize = 0;
}

deoglQuickSorter::~deoglQuickSorter(){
	if( pScores ){
		delete [] pScores;
	}
	if( pElements ){
		delete [] pElements;
	}
}



// Management
///////////////

void *deoglQuickSorter::GetElementAt( int index ) const{
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pElements[ index ];
}

void deoglQuickSorter::RemoveAllElements(){
	pCount = 0;
}

void deoglQuickSorter::AddElement( void *element, float score ){
	if( ! element ){
		DETHROW( deeInvalidParam );
	}
	
	if( pCount == pSize ){
		const int newSize = pSize + 10;
		void ** const newElements = new void*[ newSize ];
		float * const newScore = new float[ newSize ];
		
		if( pElements ){
			memcpy( newElements, pElements, sizeof( void* ) * pCount );
			delete [] pElements;
		}
		pElements = newElements;
		
		if( pScores ){
			memcpy( newScore, pScores, sizeof( float ) * pCount );
			delete [] pScores;
		}
		pScores = newScore;
		
		pSize = newSize;
	}
	
	pElements[ pCount ] = element;
	pScores[ pCount ] = score;
	pCount++;
}

void deoglQuickSorter::SortAscending(){
	if( pCount > 1 ){
		pSortAscendingStep( 0, pCount - 1 );
	}
}

void deoglQuickSorter::SortDescending(){
	if( pCount > 1 ){
		pSortDescendingStep( 0, pCount - 1 );
	}
}



void deoglQuickSorter::TestImplementation(){
	const float score[ 10 ] = { 7.6f, 4.1f, 8.1f, 9.6f, 1.2f, 5.7f, 8.2f, 3.4f, 3.7f, 2.8f };
	const int order[ 10 ] =   {   7,   5,    8,    10,   1,    6,    9,    3,    4,    2   };
	deoglQuickSorter sorter;
	int i;
	
	// test ascending sort
	for( i=0; i<10; i++ ){
		sorter.AddElement( ( void* )( intptr_t )order[ i ], score[ i ] );
	}
	sorter.SortAscending();
	
	for( i=0; i<10; i++ ){
		if( ( int )( ( char* )sorter.GetElementAt( i ) - ( char* )0x1 ) != i ){
			printf( "result [" );
			for( i=0; i<10; i++ ){
				printf( " %p", sorter.GetElementAt( i ) );
			}
			printf( " ]\n" );
			DETHROW( deeInvalidAction );
		}
	}
	
	// test descending sort
	sorter.RemoveAllElements();
	for( i=0; i<10; i++ ){
		sorter.AddElement( ( void* )( intptr_t )order[ i ], score[ i ] );
	}
	sorter.SortDescending();
	
	for( i=0; i<10; i++ ){
		if( ( int )( ( char* )sorter.GetElementAt( i ) - ( char* )0x1 ) != ( 9 - i ) ){
			printf( "result [" );
			for( i=0; i<10; i++ ){
				printf( " %p", sorter.GetElementAt( i ) );
			}
			printf( " ]\n" );
			DETHROW( deeInvalidAction );
		}
	}
}



// Private Functions
// ///////////////////

void deoglQuickSorter::pSortAscendingStep( int left, int right ){
	void * const pivotElement = pElements[ left ];
	const float pivotScore = pScores[ left ];
	const int r_hold = right;
	const int l_hold = left;
	
	while( left < right ){
		while( ( pScores[ right ] >= pivotScore ) && ( left < right ) ){
			right--;
		}
		if( left != right ){
			pElements[ left ] = pElements[ right ];
			pScores[ left ] = pScores[ right ];
			left++;
		}
		while( ( pScores[ left ] <= pivotScore ) && ( left < right ) ){
			left++;
		}
		if( left != right ){
			pElements[ right ] = pElements[ left ];
			pScores[ right ] = pScores[ left];
			right--;
		}
	}
	
	pElements[ left ] = pivotElement;
	pScores[ left ] = pivotScore;
	
	if( l_hold < left ){
		pSortAscendingStep( l_hold, left - 1 );
	}
	
	if( r_hold > left ){
		pSortAscendingStep( left + 1, r_hold );
	}
}

void deoglQuickSorter::pSortDescendingStep( int left, int right ){
	void * const pivotElement = pElements[ left ];
	const float pivotScore = pScores[ left ];
	const int r_hold = right;
	const int l_hold = left;
	
	while( left < right ){
		while( ( pScores[ right ] <= pivotScore ) && ( left < right ) ){
			right--;
		}
		if( left != right ){
			pElements[ left ] = pElements[ right ];
			pScores[ left ] = pScores[ right ];
			left++;
		}
		while( ( pScores[ left ] >= pivotScore ) && ( left < right ) ){
			left++;
		}
		if( left != right ){
			pElements[ right ] = pElements[ left ];
			pScores[ right ] = pScores[ left];
			right--;
		}
	}
	
	pElements[ left ] = pivotElement;
	pScores[ left ] = pivotScore;
	
	if( l_hold < left ){
		pSortDescendingStep( l_hold, left - 1 );
	}
	
	if( r_hold > left ){
		pSortDescendingStep( left + 1, r_hold );
	}
}
