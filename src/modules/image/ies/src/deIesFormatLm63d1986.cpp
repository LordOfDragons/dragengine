/* 
 * Drag[en]gine IES Photometric Image Module
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

#include "deIesFormatLm63d1986.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>


// Class deIesFormatLm63d1986
///////////////////////////////

// Constructor, destructor
////////////////////////////

deIesFormatLm63d1986::deIesFormatLm63d1986() :
pAngleFactorCount( 0 ),
pAngleFactors( NULL ),
pLampCount( 0 ),
pLumensPerLamp( 0.0f ),
pCandelaMultiplier( 0.0f ),
pVerticalAngleCount( 0 ),
pHorizontalAngleCount( 0 ),
pPhotometricType( eptTypeC ),
pUnitsType( eutFeet ),
pLumWidth( 0.0f ),
pLumHeight( 0.0f ),
pLumLength( 0.0f ),
pBallastFactor( 1.0f ),
pFutureBallastFactor( 1.0f ),
pFinalLumMultiplier( 1.0f ),
pVerticalAngles( NULL ),
pHorizontalAngles( NULL ),
pCandelaValues( NULL ){
}

deIesFormatLm63d1986::~deIesFormatLm63d1986(){
	if( pAngleFactors ){
		delete [] pAngleFactors;
	}
	if( pVerticalAngles ){
		delete [] pVerticalAngles;
	}
	if( pHorizontalAngles ){
		delete [] pHorizontalAngles;
	}
	if( pCandelaValues ){
		delete [] pCandelaValues;
	}
}


// Management
///////////////

bool deIesFormatLm63d1986::LoadHeader( decBaseFileReader &reader ){
	pReadLines( reader );
	
	if( ! pFindTilt() ){
		return false;
	}
	pReadTilt();
	pReadLampConfig();
	
	// we use equi-rectangular images here. the advantage is that if the light source is
	// a point light it uses the image as 3D image. if the light source is a spot or
	// projected light it uses the image as is. this makes it possible to cover all bases
	// with a single image format. furthermore equi-rectangular maps allow to use the
	// vertical and horizontal angle count directly making creating theimage easy
	pWidth = pHorizontalAngleCount;
	pHeight = pVerticalAngleCount;
	return true;
}

void deIesFormatLm63d1986::LoadFile( unsigned short *pixels ){
	pReadBallast();
	pCalcFinalLumMuliplier();
	pReadVerticalAngles();
	pReadHorizontalAngles();
	pReadCandelaValues();
	
	pNormalizeCandelaValues();
	pSanitizeCandelaValues();
	
	pSetPixels( pixels );
}



// Protected Functions
////////////////////////

void deIesFormatLm63d1986::pReadValues( decStringList &values, int count ){
	int next = 0;
	int i;
	
	values.RemoveAll();
	
	while( next < count ){
		const decStringList lineValues( pLines.GetAt( pCurLine++ ).Split( " \t\v" ) );
		const int lineValueCount = lineValues.GetCount();
		
		for( i=0; i<lineValueCount; i++ ){
			if( next == count ){
				DETHROW_INFO( deeInvalidFormat, "Too many values on line" );
			}
			values.Add( lineValues.GetAt( i ) );
			next++;
		}
	}
}

bool deIesFormatLm63d1986::pFindTilt(){
	const int lineCount = pLines.GetCount();
	while( pCurLine < lineCount ){
		if( pLines.GetAt( pCurLine ).BeginsWith( "TILT=" ) ){
			return true;
		}
		pCurLine++;
	}
	return pCurLine < lineCount;
}

void deIesFormatLm63d1986::pReadTilt(){
	pTilt = pLines.GetAt( pCurLine++ ).GetMiddle( 5 );
	if( pTilt == "NONE" ){
		return;
	}
	
	// <lamp-to-luminaire geometry>
	pCurLine++; // ignore
	
	// <# of pairs of angles and multiplying factors>
	pAngleFactorCount = pLines.GetAt( pCurLine++ ).ToInt();
	if( pAngleFactorCount > 0 ){
		pAngleFactors = new sAngleFactor[ pAngleFactorCount ];
	}
	
	// <angles>
	decStringList values;
	pReadValues( values, pAngleFactorCount );
	int i;
	for( i=0; i<pAngleFactorCount; i++ ){
		pAngleFactors[ i ].angle = values.GetAt( i ).GetTrimmed().ToFloat();
	}
	
	// <multiplying factors>
	pReadValues( values, pAngleFactorCount );
	for( i=0; i<pAngleFactorCount; i++ ){
		pAngleFactors[ i ].factor = values.GetAt( i ).GetTrimmed().ToFloat();
	}
}

void deIesFormatLm63d1986::pReadLampConfig(){
	decStringList values;
	pReadValues( values, 10 );
	
	pLampCount = values.GetAt( 0 ).GetTrimmed().ToInt();
	pLumensPerLamp = values.GetAt( 1 ).GetTrimmed().ToFloat();
	pCandelaMultiplier = values.GetAt( 2 ).GetTrimmed().ToFloat();
	
	pVerticalAngleCount = values.GetAt( 3 ).GetTrimmed().ToInt();
	pHorizontalAngleCount = values.GetAt( 4 ).GetTrimmed().ToInt();
	
	switch( values.GetAt( 5 ).GetTrimmed().ToInt() ){
	case 1:
		pPhotometricType = eptTypeC;
		break;
		
	case 2:
		pPhotometricType = eptTypeB;
		break;
		
	case 3:
		pPhotometricType = eptTypeA;
		break;
		
	default:
		DETHROW_INFO( deeInvalidFormat, "Invalid Photometric Type" );
	}
	
	switch( values.GetAt( 6 ).GetTrimmed().ToInt() ){
	case 1:
		pUnitsType = eutFeet;
		break;
		
	case 2:
		pUnitsType = eutMeters;
		break;
		
	default:
		DETHROW_INFO( deeInvalidFormat, "Invalid Units Type" );
	}
	
	pLumWidth = values.GetAt( 7 ).GetTrimmed().ToFloat();
	pLumLength = values.GetAt( 8 ).GetTrimmed().ToFloat();
	pLumHeight = values.GetAt( 9 ).GetTrimmed().ToFloat();
}

void deIesFormatLm63d1986::pReadBallast(){
	decStringList values;
	pReadValues( values, 3 );
	
	pBallastFactor = values.GetAt( 0 ).GetTrimmed().ToFloat();
	pFutureBallastFactor = values.GetAt( 1 ).GetTrimmed().ToFloat();
	// input watts = values.GetAt( 2 ).GetTrimmed().ToFloat();
}

void deIesFormatLm63d1986::pCalcFinalLumMuliplier(){
	//pFinalLumMultiplier = pCandelaMultiplier * pBallastFactor * pFutureBallastFactor;
	
	// we need to normalize the values later on so we can skip this part. this also avoids
	// strange huge scale factors causing problematic values later on
	pFinalLumMultiplier = 1.0f;
}

void deIesFormatLm63d1986::pReadVerticalAngles(){
	if( pVerticalAngleCount == 0 ){
		return;
	}
	
	pVerticalAngles = new float[ pVerticalAngleCount ];
	decStringList values;
	pReadValues( values, pVerticalAngleCount );
	const int count = values.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pVerticalAngles[ i ] = values.GetAt( i ).GetTrimmed().ToFloat();
	}
}

void deIesFormatLm63d1986::pReadHorizontalAngles(){
	if( pHorizontalAngleCount == 0 ){
		return;
	}
	
	pHorizontalAngles = new float[ pHorizontalAngleCount ];
	decStringList values;
	pReadValues( values, pHorizontalAngleCount );
	const int count = values.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pHorizontalAngles[ i ] = values.GetAt( i ).GetTrimmed().ToFloat();
	}
}

void deIesFormatLm63d1986::pReadCandelaValues(){
	if( pVerticalAngleCount == 0 || pHorizontalAngleCount == 0 ){
		return;
	}
	
	pCandelaValues = new float[ pVerticalAngleCount * pHorizontalAngleCount ];
	decStringList values;
	int i, j;
	
	for( i=0; i<pHorizontalAngleCount; i++ ){
		float * const rowCandelaValues = pCandelaValues + pVerticalAngleCount * i;
		pReadValues( values, pVerticalAngleCount );
		
		for( j=0; j<pVerticalAngleCount; j++ ){
			rowCandelaValues[ j ] = values.GetAt( j ).GetTrimmed().ToFloat(); // * pFinalLumMultiplier;
		}
	}
}

void deIesFormatLm63d1986::pNormalizeCandelaValues(){
	// we want to deliver the file as a light pattern image. this requires the maximum value
	// to be normalized to 1. this allows the user to choose his light intensity while this
	// file defining the pattern no matter where the file comes from
	const int count = pVerticalAngleCount * pHorizontalAngleCount;
	int i;
	
	float maxValue = 0.0f;
	for( i=0; i<count; i++ ){
		if( pCandelaValues[ i ] > maxValue ){
			maxValue = pCandelaValues[ i ];
		}
	}
	
	if( maxValue > FLOAT_SAFE_EPSILON ){
		const float factor = 1.0f / maxValue;
		for( i=0; i<count; i++ ){
			pCandelaValues[ i ] *= factor;
		}
		
	}else{
		// should never happen
		for( i=0; i<count; i++ ){
			pCandelaValues[ i ] = 1.0f;
		}
	}
}

void deIesFormatLm63d1986::pSanitizeCandelaValues(){
	// ensure values are in the range from 0 to 1. this avoids graphic modules getting into
	// troubles if values are slightly less than 0.
	const int count = pVerticalAngleCount * pHorizontalAngleCount;
	int i;
	for( i=0; i<count; i++ ){
		pCandelaValues[ i ] = decMath::clamp( pCandelaValues[ i ], 0.0f, 1.0f );
	}
}

void deIesFormatLm63d1986::pSetPixels( unsigned short *pixels ){
	/*
	sample into a cube map. cube maps can use HW support while equirect needs additional shader
	instructions. since we need to over-sample the image anyways we can directly go to cube map
	instead of equirect which would be simpler for direct copy.
	
	The ordering of the faces is the cube map are:
	- Layer 0 (z=0): Positive X Axis
	- Layer 1 (z=1): Negative X axis
	- Layer 2 (z=2): Positive Y Axis
	- Layer 3 (z=3): Negative Y Axis
	- Layer 4 (z=4): Positive Z Axis
	- Layer 5 (z=5): Negative Z Axis
	
	Each layer has to be oriented along the other two positive axes. Hence the positive x axis
	layer has to be oriented with the image x and y axis along the cube map y and z axis.
	
	files seem to have a maximum resolution of 2.5 degrees. for a cube map covering 90 degrees
	this requires a texture size of 32 pixels. using a size of 64 pixels gives 1.4 degrees
	resolution. using a size of 128 pixels gives 0.7 degrees resolution. a resolution of
	1.4 degrees (64 pixels size) seems enough. 32 pixels for tighter memory. could be a
	module parameter.
	
	---
	
	vertical angles:
		type c: range [0°..90°] or [90°..180°]
		type a/b: range [-90°..90°] or [0°..90°]
	
	horizontal angles:
		type c: begin 0°, last:
			0°: only one horizontal angle. light is fully symmetric
			90°: symmetric in each quadrant.
			180°: symmetric about the 0-180 degree plane.
			>180° && <360°: no symmetry
			
			(A luminaire that is bilaterally symmetric about the 90-270 degree photometric
			plane will have a first value of 90 degrees and a last value of 270 degrees.)
			
		type a/b:
			range [0°..90°]: laterally symmetric about a vertical reference plane.
			range [-90°..90°]: no symmetry
	
	---
	
	for efficient and save sampling use a struct sSampleParam{ int index, float blend }.
	for both vertical and horizontal create an array of sSampleParam with the cube map
	size as count. calculate for each sample point (for example vertical) the index of the
	first angle and the blend factor to the next higher index angle. since angles are
	sorted this can be calculated in a linear way stepping from angle to angle. then during
	sampling the arrays can be queried to find the x/y indices to sample as well as the
	blend weights
	*/
	if( pVerticalAngleCount == 0 || pHorizontalAngleCount == 0 ){
		return;
	}
	
	int i, j;
	
	for( i=0; i<pHorizontalAngleCount; i++ ){
		const float * const rowCandelaValues = pCandelaValues + pVerticalAngleCount * i;
		for( j=0; j<pVerticalAngleCount; j++ ){
			pixels[ pWidth * j + i ] = ( unsigned short )( rowCandelaValues[ j ] * 65535.0f );
		}
	}
}
