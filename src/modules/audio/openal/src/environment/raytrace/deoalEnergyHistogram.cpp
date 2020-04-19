/* 
 * Drag[en]gine OpenAL Audio Module
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
#include <string.h>

#include "deoalEnergyHistogram.h"
#include "deoalImpulseResponse.h"

#include <dragengine/common/exceptions.h>



// Class deoalEnergyHistogram
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalEnergyHistogram::deoalEnergyHistogram() :
pSlotCount( 0 ),
pBandCount( 1 ),
pTime( 0.0f ),
pSlotTime( 0.0f ),
pEntries( NULL ),
pEntryCount( 0 ),
pInvSlotTime( 0.0f ),
pHalfSlotSize( 0.0f ){
}

deoalEnergyHistogram::deoalEnergyHistogram( int slotCount, int bandCount, float time ) :
pSlotCount( slotCount ),
pBandCount( bandCount ),
pTime( decMath::max( time, 0.0f ) ),
pEntries( NULL ),
pEntryCount( bandCount * slotCount ),
pInvSlotTime( 0.0f ),
pHalfSlotSize( 0.0f )
{
	if( slotCount < 0 || bandCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( slotCount == 0 ){
		return;
	}
	
	pEntries = new float[ pEntryCount ];
	memset( pEntries, 0, sizeof( float ) * pEntryCount );
	pSlotTime = time / ( float )slotCount;
	pInvSlotTime = 1.0f / pSlotTime;
	pHalfSlotSize = pSlotTime * 0.5f;
}

deoalEnergyHistogram::deoalEnergyHistogram( const deoalEnergyHistogram &histogram ) :
pSlotCount( histogram.pSlotCount ),
pBandCount( histogram.pBandCount ),
pTime( histogram.pTime ),
pSlotTime( histogram.pSlotTime ),
pEntries( new float[ histogram.pEntryCount ] ),
pEntryCount( histogram.pEntryCount ),
pInvSlotTime( histogram.pInvSlotTime ),
pHalfSlotSize( histogram.pHalfSlotSize )
{
	memcpy( pEntries, histogram.pEntries, sizeof( float ) * pEntryCount );
}



// Manegement
/////////////

void deoalEnergyHistogram::SetParameters( int slotCount, int bandCount, float time ){
	time = decMath::max( time, 0.0f );
	
	if( slotCount == pSlotCount && bandCount == pBandCount
	&& fabsf( time - pTime ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	if( slotCount < 0 || bandCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	const int entryCount = bandCount * slotCount;
	float *entries = NULL;
	if( entryCount > 0 ){
		entries = new float[ entryCount ];
	}
	
	delete [] pEntries;
	
	pSlotCount = slotCount;
	pBandCount = bandCount;
	pEntries = entries;
	pEntryCount = entryCount;
	pTime = time;
	
	if( slotCount > 0 ){
		pSlotTime = time / ( float )slotCount;
		pInvSlotTime = 1.0f / pSlotTime;
		
	}else{
		pSlotTime = 0.0f;
		pInvSlotTime = 0.0f;
	}
	
	pHalfSlotSize = pSlotTime * 0.5f;
}

void deoalEnergyHistogram::Clear(){
	memset( pEntries, 0, sizeof( float ) * pEntryCount );
}

void deoalEnergyHistogram::Add( float time, const float *energies ){
	const int index = decMath::max( ( int )( ( time - pHalfSlotSize ) * pInvSlotTime ), 0 );
	if( index > pSlotCount - 1 ){
		return;
	}
	
	float *slot = pEntries + index;
	int i;
	
	for( i=0; i<pBandCount; i++ ){
		slot[ pSlotCount * i ] += energies[ i ];
	}
}
void deoalEnergyHistogram::Add( const deoalEnergyHistogram &histogram ){
	if( histogram.pSlotCount != pSlotCount || histogram.pBandCount != pBandCount ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=0; i<pEntryCount; i++ ){
		pEntries[ i ] += histogram.pEntries[ i ];
	}
}

void deoalEnergyHistogram::Add( const deoalImpulseResponse &impulseResponse ){
	if( pBandCount != 3 ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = impulseResponse.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deoalImpulseResponse::sImpulse &impulse = impulseResponse.GetAt( i );
		const float energies[ 3 ] = { impulse.low, impulse.medium, impulse.high };
		Add( impulse.time, energies );
	}
}

void deoalEnergyHistogram::AddMax( float time, const float *energies ){
	const int index = decMath::max( ( int )( ( time - pHalfSlotSize ) * pInvSlotTime ), 0 );
	if( index > pSlotCount - 1 ){
		return;
	}
	
	float *slot = pEntries + index;
	int i;
	
	for( i=0; i<pBandCount; i++ ){
		if( energies[ i ] > slot[ pSlotCount * i ] ){
			slot[ pSlotCount * i ] = energies[ i ];
		}
	}
}

void deoalEnergyHistogram::AddMax( const deoalEnergyHistogram &histogram ){
	if( histogram.pSlotCount != pSlotCount || histogram.pBandCount != pBandCount ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=0; i<pEntryCount; i++ ){
		if( histogram.pEntries[ i ] > pEntries[ i ] ){
			pEntries[ i ] = histogram.pEntries[ i ];
		}
	}
}

void deoalEnergyHistogram::BlendAdd( const deoalEnergyHistogram &histogram ){
	if( histogram.pSlotCount != pSlotCount || histogram.pBandCount != pBandCount ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=0; i<pEntryCount; i++ ){
		pEntries[ i ] += histogram.pEntries[ i ];
	}
}

void deoalEnergyHistogram::BlendAddScaled( const deoalEnergyHistogram &histogram, float factor ){
	if( histogram.pSlotCount != pSlotCount || histogram.pBandCount != pBandCount ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=0; i<pEntryCount; i++ ){
		pEntries[ i ] += histogram.pEntries[ i ] * factor;
	}
}

void deoalEnergyHistogram::Integrate( float *integrations ) const{
	if( ! integrations ){
		DETHROW( deeInvalidParam );
	}
	
	int i, j;
	for( i=0; i<pBandCount; i++ ){
		const float * const entries = pEntries + pSlotCount * i;
		float &integration = integrations[ i ];
		integration = 0.0f;
		
		for( j=0; j<pSlotCount; j++ ){
			integration += entries[ j ] * pSlotTime;
		}
	}
}



// Operators
//////////////

deoalEnergyHistogram &deoalEnergyHistogram::operator=( const deoalEnergyHistogram &histogram ){
	SetParameters( histogram.pSlotCount, histogram.pBandCount, histogram.pTime );
	memcpy( pEntries, histogram.pEntries, sizeof( float ) * pEntryCount );
	return *this;
}

deoalEnergyHistogram &deoalEnergyHistogram::operator*=( float factor ){
	int i;
	for( i=0; i<pEntryCount; i++ ){
		pEntries[ i ] *= factor;
	}
	return *this;
}
