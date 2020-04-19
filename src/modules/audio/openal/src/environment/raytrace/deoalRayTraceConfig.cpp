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

#include <stdlib.h>
#include <string.h>

#include "deoalRayTraceConfig.h"
#include "deoalIcoSphere.h"

#include <dragengine/common/exceptions.h>



// Class deoalRayTraceConfig
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalRayTraceConfig::deoalRayTraceConfig() :
pRayCount( 0 ),
pRayDirections( NULL ),
pRayUnitVolume( 0.0f ),
pRayUnitSurface( 0.0f ),
pOpeningAngle( 0.0f ){
}

deoalRayTraceConfig::deoalRayTraceConfig( const deoalRayTraceConfig &config ) :
pRayCount( 0 ),
pRayDirections( NULL ),
pRayUnitVolume( config.pRayUnitVolume ),
pRayUnitSurface( config.pRayUnitSurface ),
pOpeningAngle( config.pOpeningAngle )
{
	if( config.pRayCount == 0 ){
		return;
	}
	
	pRayDirections = new decVector[ config.pRayCount ];
	memcpy( pRayDirections, config.pRayDirections, sizeof( decVector ) * config.pRayCount );
	pRayCount = config.pRayCount;
}

deoalRayTraceConfig::~deoalRayTraceConfig(){
	if( pRayDirections ){
		delete [] pRayDirections;
	}
}



// Manegement
///////////////

void deoalRayTraceConfig::SetFromVertices( const decVector *vertices, int count, float openingAngle ){
	if( ! vertices || count < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pRayDirections ){
		delete [] pRayDirections;
		pRayDirections = NULL;
	}
	
	pRayDirections = new decVector[ count ];
	memcpy( pRayDirections, vertices, sizeof( decVector ) * count );
	pRayCount = count;
	
	// V = 4/3 * PI * r^3
	// A = 4 * PI * r^2
	// V and A distributed equally on all rays. r=1 for these values. during final
	// calculation the radius will factorized into them.
	pRayUnitSurface = 4.0f * PI / ( float )count;
	pRayUnitVolume = pRayUnitSurface / 3.0f;
	
	pOpeningAngle = openingAngle;
}

void deoalRayTraceConfig::Rotate( float rx, float ry, float rz ){
	const decMatrix matrix( decMatrix::CreateRotation( rx * DEG2RAD, ry * DEG2RAD, rz * DEG2RAD ) );
	int i;
	for( i=0; i<pRayCount; i++ ){
		pRayDirections[ i ] = matrix * pRayDirections[ i ];
	}
}

void deoalRayTraceConfig::SetRaysEquallySpaced( int rayCount ){
	if( rayCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pRayDirections ){
		delete [] pRayDirections;
		pRayDirections = NULL;
	}
	
	pRayCount = 0;
	pRayDirections = new decVector[ rayCount ];
	
	const double dLongitude = PI * ( 3.0 - sqrt( 5.0 ) );
	const double dZ = 2.0 / ( float )rayCount;
	double longitude = 0.0;
	double z = 1.0 - dZ * 0.5;
	
	for( pRayCount=0; pRayCount<rayCount; pRayCount++ ){
		const double radius = sqrt( 1.0 - z * z );
		pRayDirections[ pRayCount ].x = cos( longitude ) * radius;
		pRayDirections[ pRayCount ].y = sin( longitude ) * radius;
		pRayDirections[ pRayCount ].z = z;
		z -= dZ;
		longitude += dLongitude;
	}
	
	// V = 4/3 * PI * r^3
	// A = 4 * PI * r^2
	// V and A distributed equally on all rays. r=1 for these values. during final
	// calculation the radius will factorized into them.
	pRayUnitSurface = 4.0f * PI / ( float )rayCount;
	pRayUnitVolume = pRayUnitSurface / 3.0f;
	
	// gauss factor is tan(openingAngle) * 0.466
	pOpeningAngle = atanf( ( pRayDirections[ 2 ] - pRayDirections[ 1 ] ).Length() ) * 2.0f;
}

void deoalRayTraceConfig::SetFromIcoSphere( const deoalIcoSphere &icoSphere ){
	SetFromVertices( icoSphere.GetVertices(), icoSphere.GetVertexCount(), icoSphere.GetOpeningAngle() );
}
