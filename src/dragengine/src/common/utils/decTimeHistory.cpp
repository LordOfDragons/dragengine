/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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
#include <string.h>
#include <stdlib.h>

#include "decTimeHistory.h"
#include "../exceptions.h"
#include "../../logger/deLogger.h"



// Class decTimeHistory
/////////////////////////

// Constructor, destructor
////////////////////////////

decTimeHistory::decTimeHistory( int initialSize, int cutExtremeValues ) :
pEntries( NULL ),
pSorted( NULL ),
pSize( 0 ),
pHead( 0 ),
pTail( 0 ),
pCount( 0 ),
pCutExtremeValues( 0 ),
pHasMetrics ( false ),
pMinimum( 0.0f ),
pMaximum( 0.0f ),
pAverage( 0.0f )
{
	SetSize( initialSize );
	SetCutExtremeValues( cutExtremeValues );
}

decTimeHistory::~decTimeHistory(){
	if( pSorted ){
		delete [] pSorted;
	}
	if( pEntries ){
		delete [] pEntries;
	}
}



// Management
///////////////

void decTimeHistory::SetSize( int size ){
	if( size == pSize ){
		return;
	}
	
	if( size < 1 ){
		DETHROW_INFO( deeInvalidParam, "size < 1" );
	}
	
	pHead = 0;
	pTail = 0;
	pCount = 0;
	
	if( pSorted ){
		delete [] pSorted;
		pSorted = NULL;
	}
	if( pEntries ){
		delete [] pEntries;
		pEntries = NULL;
	}
	pSize = 0;
	
	pEntries = new float[ size ];
	pSorted = new float[ size ];
	pSize = size;
}

float decTimeHistory::GetAt( int position ) const{
	if( position < 0 ){
		DETHROW_INFO( deeInvalidParam, "position < 0" );
	}
	if( position >= pCount ){
		DETHROW_INFO( deeInvalidParam, "position >= count" );
	}
	
	return pEntries[ ( pHead + position ) % pSize ];
}

void decTimeHistory::Add( float time ){
	pEntries[ pTail ] = time;
	
	pTail++;
	if( pTail == pSize ){
		pTail = 0;
	}
	
	if( pCount < pSize ){
		pCount++;
		
	}else{
		pHead = pTail;
	}
	
	pCalculateMetrics();
}

void decTimeHistory::Clear(){
	pHead = 0;
	pTail = 0;
	pCount = 0;
	
	pCalculateMetrics();
}



void decTimeHistory::SetCutExtremeValues( int cutExtremeValues ){
	if( cutExtremeValues == pCutExtremeValues ){
		return;
	}
	if( cutExtremeValues < 0 ){
		DETHROW_INFO( deeInvalidParam, "cutExtremeValues < 0" );
	}
	
	pCutExtremeValues = cutExtremeValues;
	pCalculateMetrics();
}



decString decTimeHistory::DebugPrint() const{
	if( pCount <= pCutExtremeValues * 2 ){
		return decString( "no metrics" );
	}
	
	decString buffer( "times=[" );
	const int intTime = ( int )( pSorted[ pCutExtremeValues ] * 1e4f );
	buffer.AppendFormat( "%2d.%1d", intTime / 10, intTime % 10 );
	
	int i;
	for( i=pCutExtremeValues+1; i<pCount-pCutExtremeValues; i++ ){
		const int intTime2 = ( int )( pSorted[ i ] * 1e4f );
		buffer.AppendFormat( " %2d.%1d", intTime2 / 10, intTime2 % 10 );
	}
	
	if( pCutExtremeValues == 0 ){
		buffer.Append( "] cut=[" );
		
	}else{
		const int intTimeCut = ( int )( pSorted[ 0 ] * 1e4f );
		buffer.AppendFormat( "] cut=[%2d.%1d", intTimeCut / 10, intTimeCut % 10 );
		
		for( i=1; i<pCutExtremeValues; i++ ){
			const int intTimeCut2 = ( int )( pSorted[ i ] * 1e4f );
			buffer.AppendFormat( " %2d.%1d", intTimeCut2 / 10, intTimeCut2 % 10 );
		}
		
		for( i=pCount-pCutExtremeValues; i<pCount; i++ ){
			const int intTimeCut2 = ( int )( pSorted[ i ] * 1e4f );
			buffer.AppendFormat( " %2d.%1d", intTimeCut2 / 10, intTimeCut2 % 10 );
		}
	}
	
	const int intTimeMin = ( int )( pMinimum * 1e4f );
	const int intTimeMax = ( int )( pMaximum * 1e4f );
	const int intTimeAvg = ( int )( pAverage * 1e4f );
	buffer.AppendFormat( "] min=%2d.%1d max=%2d.%1d avg=%2d.%1d",
		intTimeMin / 10, intTimeMin % 10,
		intTimeMax / 10, intTimeMax % 10,
		intTimeAvg / 10, intTimeAvg % 10);
	return buffer;
}



// Private Functions
//////////////////////

void decTimeHistory::pCalculateMetrics(){
	if( pCount == 0 ){
		// no entries
		pHasMetrics = false;
		pMinimum = 0.0f;
		pMaximum = 0.0f;
		pAverage = 0.0f;
		return;
	}
	
	// sort entries
	pSorted[ 0 ] = pEntries[ pHead ];
	int next = pHead + 1;
	if( next == pSize ){
		next = 0;
	}
	
	int i = 1;
	while( next != pTail ){
		pSorted[ i++ ] = pEntries[ next++ ];
		if( next == pSize ){
			next = 0;
		}
	}
	
	for( i=1; i<pCount; i++ ){
		if( pSorted[ i ] < pSorted[ i - 1 ] ){
			const float swap = pSorted[ i - 1 ];
			pSorted[ i - 1 ] = pSorted[ i ];
			pSorted[ i ] = swap;
			if( i > 1 ){
				i -= 2;
			}
		}
	}
	
	// store minimum, maximum and average cutting extreme values
	if( pCount <= pCutExtremeValues * 2 ){
		// no entries
		pHasMetrics = false;
		pMinimum = 0.0f;
		pMaximum = 0.0f;
		pAverage = 0.0f;
		return;
	}
	
	pMinimum = pSorted[ pCutExtremeValues ];
	pMaximum = pSorted[ pCount - pCutExtremeValues ];
	
	pAverage = pSorted[ pCount / 2 ]; // median, fine enough
	/*
	pAverage = pSorted[ pCutExtremeValues ];
	const int last = pCount - pCutExtremeValues;
	for( i=pCutExtremeValues+1; i<last; i++ ){
		pAverage += pSorted[ i ];
	}
	pAverage /= ( float )( pCount - pCutExtremeValues * 2 );
	*/
	
	pHasMetrics = true;
}
