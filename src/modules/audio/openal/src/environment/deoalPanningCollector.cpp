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

#include <string.h>

#include "deoalPanningCollector.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"

#include <dragengine/common/exceptions.h>



// Class deoalPanningCollector
////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalPanningCollector::deoalPanningCollector( int resolution ) :
pResolution( resolution ),
pPixels( NULL ),
pPixelCount( resolution * resolution * 6 ),
pPixelsPerFace( resolution * resolution ),
pPixelsXPos( NULL ),
pPixelsXNeg( NULL ),
pPixelsYPos( NULL ),
pPixelsYNeg( NULL ),
pPixelsZPos( NULL ),
pPixelsZNeg( NULL )
{
	if( resolution < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	pPixels = new float[ pPixelCount ];
	
	pPixelsXPos = pPixels;
	pPixelsXNeg = pPixels + pPixelsPerFace;
	pPixelsYPos = pPixels + pPixelsPerFace * 2;
	pPixelsYNeg = pPixels + pPixelsPerFace * 3;
	pPixelsZPos = pPixels + pPixelsPerFace * 4;
	pPixelsZNeg = pPixels + pPixelsPerFace * 5;
	
	Clear();
}

deoalPanningCollector::deoalPanningCollector( const deoalPanningCollector &collector ) :
pResolution( collector.pResolution ),
pPixels( NULL ),
pPixelCount( collector.pPixelCount ),
pPixelsPerFace( collector.pPixelsPerFace ),
pPixelsXPos( NULL ),
pPixelsXNeg( NULL ),
pPixelsYPos( NULL ),
pPixelsYNeg( NULL ),
pPixelsZPos( NULL ),
pPixelsZNeg( NULL )
{
	pPixels = new float[ pPixelCount ];
	
	pPixelsXPos = pPixels;
	pPixelsXNeg = pPixels + pPixelsPerFace;
	pPixelsYPos = pPixels + pPixelsPerFace * 2;
	pPixelsYNeg = pPixels + pPixelsPerFace * 3;
	pPixelsZPos = pPixels + pPixelsPerFace * 4;
	pPixelsZNeg = pPixels + pPixelsPerFace * 5;
	
	*this = collector;
}

deoalPanningCollector::~deoalPanningCollector(){
	if( pPixels ){
		delete [] pPixels;
	}
}



// Manegement
///////////////

void deoalPanningCollector::Clear(){
	memset( pPixels, 0, sizeof( float ) * ( pPixelsPerFace * 6 ) );
}



void deoalPanningCollector::Add( const decVector &, float ){
	DETHROW( deeInvalidAction );
}



decVector deoalPanningCollector::CalcPanningVector() const{
	DETHROW( deeInvalidAction );
}



void deoalPanningCollector::DebugPrint( deoalAudioThread &audioThread ) const{
	const char * const faceNames[ 6 ] = {
		"Positive X-Axis", "Negative X-Axis", "Positive Y-Axis",
		"Negative Y-Axis", "Positive Z-Axis", "Negative Z-Axis" };
	deoalATLogger &logger = audioThread.GetLogger();
	decString text;
	int i, j, k;
	
	logger.LogInfoFormat( "Panning Collector: Resolution %d", pResolution );
	for( i=0; i<6; i++ ){
		const float * const facePixels = pPixels + pPixelsPerFace * i;
		logger.LogInfoFormat( "- Face %s", faceNames[ i ] );
		for( j=0; j<pResolution; j++ ){
			const float * const rowPixels = facePixels + pResolution * j;;
			text.Format( "  - Row %02d: [", j );
			for( k=0; k<pResolution; k++ ){
				if( k > 0 ){
					text += ", ";
				}
				text.AppendFormat( "%.3f", rowPixels[ k ] );
			}
			text += "]";
			logger.LogInfo( text );
		}
	}
}



// Operators
//////////////

deoalPanningCollector &deoalPanningCollector::operator=( const deoalPanningCollector &collector ){
	if( collector.pResolution != pResolution ){
		DETHROW( deeInvalidParam );
	}
	memcpy( pPixels, collector.pPixels, sizeof( float ) * pPixelCount );
	return *this;
}

deoalPanningCollector &deoalPanningCollector::operator+=( const deoalPanningCollector &collector ){
	if( collector.pResolution != pResolution ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=0; i<pPixelCount; i++ ){
		if( collector.pPixels[ i ] > pPixels[ i ] ){
			pPixels[ i ] = collector.pPixels[ i ];
		}
	}
	
	return *this;
}
