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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglSkyLayerTracker.h"

#include <dragengine/resources/sky/deSkyLayer.h>
#include <dragengine/common/exceptions.h>



// Inline Functions
/////////////////////

inline float squaredOrientationDifference( const decQuaternion &q1, const decQuaternion &q2 ){
	const decQuaternion q3 = q1.Conjugate() * q2;
	
	// diff-squared = x*x + y*y + z*z + (w-1)*(w-1)
	return q3.x * q3.x + q3.y * q3.y + q3.z * q3.z + q3.w * q3.w - q3.w * 2.0f + 1.0f;
}



// Constants
//////////////

const float deoglSkyLayerTracker::THRESHOLD_ONE_DEGREE_ORIENTATION = 7.615902e-5f;



// Class deoglSkyLayerTracker
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkyLayerTracker::deoglSkyLayerTracker() :
	pThresholdOrientation( 0.0f ),
pThresholdIntensity( 0.0f ),
pThresholdTransparency( 0.0f ),
pThresholdColor( 0.0f ),

pIntensity( 0.0f ),
pTransparency( 1.0f ),

pLightIntensity( 0.0f ),
pAmbientIntensity( 0.0f ){
}

deoglSkyLayerTracker::~deoglSkyLayerTracker(){
}



// Management
///////////////

void deoglSkyLayerTracker::SetThresholdOrientation( float threshold ){
	pThresholdOrientation = threshold;
}

void deoglSkyLayerTracker::SetThresholdIntensity( float threshold ){
	pThresholdIntensity = threshold;
}

void deoglSkyLayerTracker::SetThresholdTransparency( float threshold ){
	pThresholdTransparency = threshold;
}

void deoglSkyLayerTracker::SetThresholdColor( float threshold ){
	pThresholdColor = threshold;
}



bool deoglSkyLayerTracker::UpdateOrientation( const decQuaternion &orientation ) {
	if( squaredOrientationDifference( orientation, pOrientation ) <= pThresholdOrientation ){
		return false;
		
	}else{
		//printf( "sky: tracker update due to layer orientation\n" );
		pOrientation = orientation;
		return true;
	}
}

bool deoglSkyLayerTracker::UpdateColor( const decColor &color ){
	if( color.IsEqualTo( pColor, pThresholdColor ) ){
		return false;
		
	}else{
		//printf( "sky: tracker update due to layer color\n" );
		pColor = color;
		return true;
	}
}

bool deoglSkyLayerTracker::UpdateIntensity( float intensity ){
	if( fabsf( intensity - pIntensity ) < pThresholdIntensity ){
		return false;
		
	}else{
		//printf( "sky: tracker update due to layer intensity\n" );
		pIntensity = intensity;
		return true;
	}
}

bool deoglSkyLayerTracker::UpdateTransparency( float transparency ){
	if( fabsf( transparency - pTransparency ) < pThresholdTransparency ){
		return false;
		
	}else{
		//printf( "sky: tracker update due to layer transparency\n" );
		pTransparency = transparency;
		return true;
	}
}

bool deoglSkyLayerTracker::UpdateLightOrientation( const decQuaternion &orientation ){
	if( squaredOrientationDifference( orientation, pLightOrientation ) < pThresholdOrientation ){
		return false;
		
	}else{
		//printf( "sky: tracker update due to layer light orientation\n" );
		pLightOrientation = orientation;
		return true;
	}
}

bool deoglSkyLayerTracker::UpdateLightColor( const decColor &color ){
	if( color.IsEqualTo( pLightColor, pThresholdColor ) ){
		return false;
		
	}else{
		//printf( "sky: tracker update due to layer light color\n" );
		pLightColor = color;
		return true;
	}
}

bool deoglSkyLayerTracker::UpdateLightIntensity( float intensity ){
	if( fabsf( intensity - pLightIntensity ) < pThresholdIntensity ){
		return false;
		
	}else{
		//printf( "sky: tracker update due to layer light intensity\n" );
		pLightIntensity = intensity;
		return true;
	}
}

bool deoglSkyLayerTracker::UpdateAmbientIntensity( float intensity ){
	if( fabsf( intensity - pAmbientIntensity ) < pThresholdIntensity ){
		return false;
		
	}else{
		//printf( "sky: tracker update due to layer ambient intensity\n" );
		pAmbientIntensity = intensity;
		return true;
	}
}

