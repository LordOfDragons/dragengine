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
#include <string.h>

#include "decCurveBezier3D.h"
#include "decCurveBezier3DEvaluator.h"
#include "decCurveDistanceMapping.h"

#include "../exceptions.h"



// Class decCurveDistanceMapping
//////////////////////////////////

// Constructor, destructor
////////////////////////////

decCurveDistanceMapping::decCurveDistanceMapping() :
pResolution( 0.1f ),
pLength( 0.0f ),
pLastSegment( 0 ),
pMappings( NULL ),
pMappingCount( 0 ){
}

decCurveDistanceMapping::decCurveDistanceMapping( const decCurveDistanceMapping &mapping ) :
pResolution( mapping.pResolution ),
pLength( mapping.pLength ),
pLastSegment( mapping.pLastSegment ),
pMappings( NULL ),
pMappingCount( 0 ){
	if( mapping.pMappingCount > 0 ){
		pMappings = new float[ mapping.pMappingCount ];
		memcpy( pMappings, mapping.pMappings, sizeof( float ) * mapping.pMappingCount );
		pMappingCount = mapping.pMappingCount;
	}
}

decCurveDistanceMapping::~decCurveDistanceMapping(){
	if( pMappings ){
		delete [] pMappings;
	}
}



// Management
///////////////

void decCurveDistanceMapping::Map( float distance, int &segment, float &blend ) const{
	if( distance <= 0.0f ){
		segment = 0;
		blend = 0.0f;
		
	}else if( distance >= pLength ){
		segment = pLastSegment;
		blend = 1.0f;
		
	}else{
		float intpart;
		const float frac = modff( distance / pResolution, &intpart );
		const int index = decMath::clamp( ( int )intpart, 0, pMappingCount - 1 );
		
		if( index < 0 ){
			segment = 0;
			blend = 0.0f;
			
		}else if( index < pMappingCount - 1 ){
			blend = modff( decMath::mix( pMappings[ index ], pMappings[ index + 1 ], frac ), &intpart );
			segment = ( int )intpart;
			
		}else{
			segment = pLastSegment;
			blend = 1.0f;
		}
	}
}

void decCurveDistanceMapping::Clear(){
	if( pMappings ){
		delete [] pMappings;
		pMappings = NULL;
		pMappingCount = 0;
	}
	pResolution = 0.1f;
	pLastSegment = 0;
}

void decCurveDistanceMapping::Init( const decCurveBezier3D &curve ){
	Init( curve, 0.01f, 10 );
}

void decCurveDistanceMapping::Init( const decCurveBezier3D &curve, float resolution, int segmentSamples ){
	if( segmentSamples < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	Clear();
	
	const int pointCount = curve.GetPointCount();
	pResolution = decMath::max( resolution, 0.001f );
	pLength = 0.0f;
	
	if( pointCount == 0 ){
		pLastSegment = 0;
		return;
	}
	
	pLastSegment = pointCount - 2;
	
	// estimate length of curve using sample count
	const float blendStep = 1.0f / ( float )segmentSamples;
	const decCurveBezier3DEvaluator evaluator( curve );
	float segmentLength;
	decVector lastPoint;
	decVector nextPoint;
	float blend;
	int i, j;
	
	lastPoint = evaluator.EvaluateAt( 0, 0.0f );
	
	for( i=0; i<=pLastSegment; i++ ){
		segmentLength = 0.0f;
		blend = blendStep;
		
		for( j=0; j<segmentSamples; j++ ){
			if( j < segmentSamples - 1 ){
				nextPoint = evaluator.EvaluateAt( i, blend );
				
			}else{
				nextPoint = evaluator.EvaluateAt( i + 1, 0.0f );
			}
			
			segmentLength += ( nextPoint - lastPoint ).Length();
			lastPoint = nextPoint;
			blend += blendStep;
		}
		
		pLength += segmentLength;
		//printf( "segment %i/%i length %f\n", i, pLastSegment, segmentLength );
	}
	
	//printf( "curve length %f\n", pLength );
	
	// create mappings
	const int mappingCount = ( int )ceil( pLength / pResolution ) + 1;
	//printf( "create mapping with %i entries\n", mappingCount );
	
	if( mappingCount < 2 ){
		return;
	}
	
	pMappings = new float[ mappingCount ];
	pMappings[ 0 ] = 0.0f; // start of curve is always the same. easier to evaluate if included
	
	float lastCurveValue = 0.0f;
	float nextCurveLength = 0.0f;
	float nextCurveValue;
	
	pMappingCount = 1;
	lastPoint = evaluator.EvaluateAt( 0, 0.0f );
	
	for( i=0; i<=pLastSegment; i++ ){
		blend = blendStep;
		
		for( j=0; j<segmentSamples; j++ ){
			if( j < segmentSamples - 1 ){
				nextPoint = evaluator.EvaluateAt( i, blend );
				nextCurveValue = ( float )i + blend;
				
			}else{
				nextPoint = evaluator.EvaluateAt( i + 1, 0.0f );
				nextCurveValue = ( float )( i + 1 );
			}
			
			nextCurveLength += ( nextPoint - lastPoint ).Length();
			//printf( "mapping segment %i sample %i nextCurveValue=%f\n", i, j, nextCurveLength );
			while( nextCurveLength >= pResolution ){
				if( pMappingCount > mappingCount - 1 ){
					DETHROW( deeInvalidParam );
				}
				
				// pMappingCount = mappingCount-1 is an edge case but accepted
				lastCurveValue = decMath::mix( lastCurveValue, nextCurveValue, pResolution / nextCurveLength );
				pMappings[ pMappingCount++ ] = lastCurveValue;
				nextCurveLength -= pResolution;
				//printf( "add mapping at %i/%i curveValue %f nextCurveLength=%f\n", pMappingCount-1, mappingCount-1, lastCurveValue, nextCurveLength );
			}
			
			lastPoint = nextPoint;
			blend += blendStep;
		}
	}
	
	if( pMappingCount == mappingCount - 1 ){
		// last mapping entry is a fictional one allowing to properly map the last segment. basically we have
		// now the remaining curve value (from last mapping point to end of curve) and the remaining curve
		// length (from last mapping point to end of curve). now the following holds:
		//   remainingCurveValue / mappingCurveValue = nextCurveLength / resolution
		// 
		// and thus:
		//   mappingCurveValue = remainingCurveValue * resolution / nextCurveLength
		if( nextCurveLength < 0.01f ){
			// too small next curve length. just map it to the end of the curve
			pMappings[ pMappingCount++ ] = ( float )( pLastSegment + 1 );
			
		}else{
			const float remainingCurveValue = decMath::max( ( float )( pLastSegment + 1 ) - pMappings[ mappingCount - 2 ], 0.0f );
			pMappings[ pMappingCount++ ] = pMappings[ mappingCount - 2 ] + remainingCurveValue * pResolution / nextCurveLength;
		}
		//printf( "last mapping %f\n", pMappings[ mappingCount - 1 ] );
	}
	
	if( pMappingCount != mappingCount ){
		DETHROW( deeInvalidParam );
	}
	
	/*
	for( i=0; i<pMappingCount; i++ ){
		printf( "mapping %3i: %f\n", i, pMappings[ i ] );
	}
	*/
}



// Operators
//////////////

decCurveDistanceMapping &decCurveDistanceMapping::operator=( const decCurveDistanceMapping &mapping ){
	Clear();
	
	pResolution = mapping.pResolution;
	pLength = mapping.pLength;
	pLastSegment = mapping.pLastSegment;
	
	if( mapping.pMappingCount > 0 ){
		pMappings = new float[ mapping.pMappingCount ];
		memcpy( pMappings, mapping.pMappings, sizeof( float ) * mapping.pMappingCount );
		pMappingCount = mapping.pMappingCount;
	}
	
	return *this;
}

bool decCurveDistanceMapping::operator==( const decCurveDistanceMapping &mapping ) const{
	if( fabsf( pResolution - mapping.pResolution ) > FLOAT_SAFE_EPSILON ){
		return false;
	}
	if( fabsf( pLength - mapping.pLength ) > FLOAT_SAFE_EPSILON ){
		return false;
	}
	if( pLastSegment != mapping.pLastSegment ){
		return false;
	}
	if( pMappingCount != mapping.pMappingCount ){
		return false;
	}
	
	int i;
	for( i=0; i<pMappingCount; i++ ){
		if( fabsf( pMappings[ i ] - mapping.pMappings[ i ] ) > FLOAT_SAFE_EPSILON ){
			return false;
		}
	}
	
	return true;
}
