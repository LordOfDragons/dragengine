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

void deIesFormatLm63d1986::sSamplePoint::Set( int index ){
	index1 = index;
	index2 = index;
	blend1 = 1.0f;
	blend2 = 0.0f;
}

void deIesFormatLm63d1986::sSamplePoint::Set( int index, float blend ){
	index1 = index;
	index2 = index + 1;
	blend1 = 1.0f - blend;
	blend2 = blend;
}

// Constructor, destructor
////////////////////////////

deIesFormatLm63d1986::deIesFormatLm63d1986() :
pImageType( eitEquirectangular ),
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
pCandelaValues( NULL ),
pSmallestVerticalStep( 0.0f ),
pSmallestHorizontalStep( 0.0f ),
pVerticalResolution( 1 ),
pHorizontalResolution( 1 ),
pVerticalSamplePoints( NULL ),
pHorizontalSamplePoints( NULL ){
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
	if( pVerticalSamplePoints ){
		delete [] pVerticalSamplePoints;
	}
	if( pHorizontalSamplePoints ){
		delete [] pHorizontalSamplePoints;
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
	
	// reading angles is required to figure out the appropriate image size
	pReadBallast();
	pCalcFinalLumMuliplier();
	pReadVerticalAngles();
	pReadHorizontalAngles();
	
	// about resolutions. angles have no defined placement and in particular the spacing
	// across angles in files tends to vary. to capture this the angles are up-sampled
	// into images of larger size to store the denser areas while blending the large ones.
	// theoretically the sampling density could be derived from the smallest spacing in
	// the data set but this works only for equirectangular images and even there it is
	// not sure if the spacing is not offset in a bad way. for this reason up-sampling is
	// the better solution even if potentially less accurace for high density files.
	// 
	// the image size to use can be calculated from the desired angular resolution. in
	// examined files the sampling density is between 1.25 - 5 degrees. the following
	// table shows the angular resolution for image resolutions of 360, 180 and 90 degrees:
	// - 11.25 degrees: 32 (16) (8) pixels
	// - 5.62 degrees: 64 (32) (16) pixels
	// - 2.81 degrees: 128 (64) (32) pixels
	// - 1.41 degrees: 256 (128) (64) pixels
	// - 0.70 degrees: 512 (256) (128) pixels
	// - 0.35 degrees: 1024 (512) (256) pixels
	// 
	// for equirectangular image the size 360x180 degrees is required:
	// - 11.25 degrees: 32 x 16 pixels
	// - 5.62 degrees: 64 x 32 pixels
	// - 2.81 degrees: 128 x 64 pixels
	// - 1.41 degrees: 256 x 128 pixels
	// - 0.70 degrees: 512 x 256 pixels
	// - 0.35 degrees: 1024 x 512 pixels
	// 
	// for cube map image the size 90x90 degrees is required:
	// - 11.25 degrees: 8 x 8 pixels
	// - 5.62 degrees: 16 x 16 pixels
	// - 2.81 degrees: 32 x 32 pixels
	// - 1.41 degrees: 64 x 64 pixels
	// - 0.70 degrees: 128 x 128 pixels
	// - 0.35 degrees: 256 x 256 pixels
	// 
	// 2.81 degrees would be enough to capture the files examined. this requires the sizes:
	// - equirectangular: 128 x 64 pixels
	// - cube map: 32 x 32 pixels
	// 
	// 1.41 degrees is more fine grained. this requires the sizes:
	// - equirect: 256 x 128 pixels
	// - cube map: 64 x 64 pixels
	// 
	// for the time being 1.41 degrees is used. this yields cube maps of size 64x64 which
	// should be still good enough for GPU to cache which should result in best performance.
	// 
	// the angular resolution stores the pixel count for 360 degrees. for equirectangular
	// the same resolution as the image width is used since the values are mostly one-to-one
	// translated. for cubemap images though the resolution has to be increased since cubemap
	// pixels are not evenly spaced. cranking up the resolution helps
	pFindSmallestSteps();
	
	const float overSampleFactor = 2.0f; // 2.0f
	
	pVerticalResolution = decMath::min( ( int )( 180.0f / ( pSmallestVerticalStep / overSampleFactor ) + 0.5f ), 1024 );
	pHorizontalResolution = decMath::min( ( int )( 360.0f / ( pSmallestHorizontalStep / overSampleFactor ) + 0.5f ), 2048 );
	
	// equirectangular
	pImageType = eitEquirectangular;
	pWidth = pHorizontalResolution;
	pHeight = pVerticalResolution;
	pDepth = 1;
	
	// cubemap
	/*
	pImageType = eitCubeMap;
	pWidth = decMath::max( pVerticalResolution / 2, pHorizontalResolution / 4, 32 );
	pHeight = pWidth;
	pDepth = 6;
	*/
	
	return true;
}

void deIesFormatLm63d1986::LoadFile( unsigned short *pixels ){
	pReadCandelaValues();
	
	pNormalizeCandelaValues();
	pSanitizeCandelaValues();
	
	pGammaCorrectCandelaValues(); // see comments in function why this is done here
	
	pCreateSamplePoints();
	
	switch( pImageType ){
	case eitEquirectangular:
		pSetPixelsEquirect( pixels );
		break;
		
	case eitCubeMap:
		pSetPixelsCubemap( pixels );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}



// Protected Functions
////////////////////////

void deIesFormatLm63d1986::pReadValues( decStringList &values, int count ){
	int next = 0;
	int i;
	
	values.RemoveAll();
	
	while( next < count ){
		// according to the reference documentation values are separated by spaces. in one file
		// though commas are used instead. accepting this here too although not correct
		const decStringList lineValues( pLines.GetAt( pCurLine++ ).Split( " \t\v," ) );
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

void deIesFormatLm63d1986::pFindSmallestSteps(){
	int i;
	
	pSmallestVerticalStep = 180.0f;
	for( i=1; i<pVerticalAngleCount; i++ ){
		const float difference = pVerticalAngles[ i ] - pVerticalAngles[ i - 1 ];
		if( difference < pSmallestVerticalStep ){
			pSmallestVerticalStep = difference;
		}
	}
	
	pSmallestHorizontalStep = 360.0f;
	for( i=1; i<pHorizontalAngleCount; i++ ){
		const float difference = pHorizontalAngles[ i ] - pHorizontalAngles[ i - 1 ];
		if( difference < pSmallestHorizontalStep ){
			pSmallestHorizontalStep = difference;
		}
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

void deIesFormatLm63d1986::pGammaCorrectCandelaValues(){
	// images loaded into the game engine are typically gamma corrected with 2.2 . IES profiles
	// though are gamma 1. there are now two possible solutions. either the user has to specify
	// a "color.gamma" texture property of value 1 or this module gamma-corrects the values.
	// forcing this on the user is not a good idea since this prevents IES profiles to be used
	// as drop-in replacement for regular images. furthermore extra work is required by the
	// used to specify gamma=1 everywhere
	const int count = pVerticalAngleCount * pHorizontalAngleCount;
	const float gamma = 1.0f / 2.2f;
	int i;
	for( i=0; i<count; i++ ){
		pCandelaValues[ i ] = powf( pCandelaValues[ i ], gamma );
	}
}

void deIesFormatLm63d1986::pCreateSamplePoints(){
	if( pVerticalAngleCount == 0 || pHorizontalAngleCount == 0 ){
		return;
	}
	
	const int horizontalCount360 = pHorizontalResolution;
	const int horizontalCount180 = pHorizontalResolution / 2;
	const int horizontalCount90 = pHorizontalResolution / 4;
	
	// horizontal angles
	pHorizontalSamplePoints = new sSamplePoint[ horizontalCount360 ];
	
	if( pPhotometricType == eptTypeC ){
		const float lastAngle = pHorizontalAngles[ pHorizontalAngleCount - 1 ];
		const float firstAngle = pHorizontalAngles[ 0 ];
		
		if( pIsAngle( firstAngle, 0.0f ) ){
			if( pIsAngle( lastAngle, 0.0f ) ){
				// 0°: only one horizontal angle. light is fully symmetric
				pFillSamples( 0, pHorizontalSamplePoints, horizontalCount360 );
				
			}else if( pIsAngle( lastAngle, 90.0f ) ){
				// 90°: symmetric in each quadrant.
				pSample( pHorizontalAngles, pHorizontalAngleCount,
					pHorizontalSamplePoints, horizontalCount90 );
				pMirrorSamples( pHorizontalSamplePoints,
					pHorizontalSamplePoints + horizontalCount90, horizontalCount90 );
				pMirrorSamples( pHorizontalSamplePoints,
					pHorizontalSamplePoints + horizontalCount180, horizontalCount180 );
				
			}else if( pIsAngle( lastAngle, 180.0f ) ){
				// 180°: symmetric about the 0-180 degree plane.
				pSample( pHorizontalAngles, pHorizontalAngleCount,
					pHorizontalSamplePoints, horizontalCount180 );
				pMirrorSamples( pHorizontalSamplePoints,
					pHorizontalSamplePoints + horizontalCount180, horizontalCount180 );
				
			}else{
				// >180° && <360°: no symmetry
				pSample( pHorizontalAngles, pHorizontalAngleCount,
					pHorizontalSamplePoints, horizontalCount360 );
			}
			
		}else if( pIsAngle( firstAngle, 90.0f ) ){
			if( pIsAngle( lastAngle, 270.0f ) ){
				// A luminaire that is bilaterally symmetric about the 90-270 degree photometric
				// plane will have a first value of 90 degrees and a last value of 270 degrees.
				pSample( pHorizontalAngles, pHorizontalAngleCount,
					pHorizontalSamplePoints + horizontalCount90, horizontalCount180 );
				pMirrorSamples( pHorizontalSamplePoints + horizontalCount90,
					pHorizontalSamplePoints, horizontalCount90 );
				pMirrorSamples( pHorizontalSamplePoints + horizontalCount180,
					pHorizontalSamplePoints + horizontalCount180 + horizontalCount90, horizontalCount90 );
				
			}else{
				DETHROW_INFO( deeInvalidFileFormat, "invalid end angle" );
			}
			
		}else{
			DETHROW_INFO( deeInvalidFileFormat, "invalid start angle" );
		}
		
	}else{
		const float lastAngle = pHorizontalAngles[ pHorizontalAngleCount - 1 ];
		const float firstAngle = pHorizontalAngles[ 0 ];
		
		if( pIsAngle( lastAngle, 90.0f ) ){
			if( pIsAngle( firstAngle, 0.0f ) ){
				// range [0°..90°]: laterally symmetric about a vertical reference plane.
				pSample( pHorizontalAngles, pHorizontalAngleCount,
					pHorizontalSamplePoints, horizontalCount90 );
				pMirrorSamples( pHorizontalSamplePoints,
					pHorizontalSamplePoints + horizontalCount180 + horizontalCount90, horizontalCount90 );
				pFillSamples( pHorizontalAngleCount - 1,
					pHorizontalSamplePoints + horizontalCount90, horizontalCount180 );
				
			}else if( pIsAngle( firstAngle, -90.0f ) ){
				// range [-90°..90°]: no symmetry
				pSample( pHorizontalAngles, pHorizontalAngleCount,
					pHorizontalSamplePoints + horizontalCount90, horizontalCount180 );
				pCopySamples( pHorizontalSamplePoints + horizontalCount90,
					pHorizontalSamplePoints + horizontalCount180 + horizontalCount90, horizontalCount90 );
				pCopySamples( pHorizontalSamplePoints + horizontalCount180,
					pHorizontalSamplePoints, horizontalCount90 );
				pFillSamples( 0, pHorizontalSamplePoints + horizontalCount180, horizontalCount90 );
				pFillSamples( pHorizontalAngleCount - 1,
					pHorizontalSamplePoints + horizontalCount90, horizontalCount90 );
				
			}else{
				DETHROW_INFO( deeInvalidFileFormat, "invalid start angle" );
			}
			
		}else{
			DETHROW_INFO( deeInvalidFileFormat, "invalid end angle" );
		}
	}
	
	// vertical angles
	const int verticalCount180 = pVerticalResolution;
	const int verticalCount90 = pVerticalResolution / 2;
	
	pVerticalSamplePoints = new sSamplePoint[ verticalCount180 ];
	
	if( pPhotometricType == eptTypeC ){
		const float lastAngle = pVerticalAngles[ pVerticalAngleCount - 1 ];
		const float firstAngle = pVerticalAngles[ 0 ];
		
		if( pIsAngle( firstAngle, 0.0f ) && pIsAngle( lastAngle, 90.0f ) ){
			// range [0°..90°]
			pSample( pVerticalAngles, pVerticalAngleCount, pVerticalSamplePoints, verticalCount90 );
			pFillSamples( pVerticalAngleCount - 1,
				pVerticalSamplePoints + verticalCount90, verticalCount90 );
			
		}else if( pIsAngle( firstAngle, 0.0f ) && pIsAngle( lastAngle, 180.0f ) ){
			// range [0°..180°]
			pSample( pVerticalAngles, pVerticalAngleCount, pVerticalSamplePoints, verticalCount180 );
			
		}else if( pIsAngle( firstAngle, 0.0f ) && pIsAngle( lastAngle, 180.0f ) ){
			// range [90°..90°]
			pFillSamples( 0, pVerticalSamplePoints, verticalCount180 );
			
		}else if( pIsAngle( firstAngle, 90.0f ) && pIsAngle( lastAngle, 180.0f ) ){
			// range [90°..180°]
			pFillSamples( 0, pVerticalSamplePoints, verticalCount90 );
			pSample( pVerticalAngles, pVerticalAngleCount,
				pVerticalSamplePoints + verticalCount90, verticalCount90 );
			
		}else{
			DETHROW_INFO( deeInvalidFileFormat, "invalid angle range" );
		}
		
	}else{
		const float lastAngle = pVerticalAngles[ pVerticalAngleCount - 1 ];
		const float firstAngle = pVerticalAngles[ 0 ];
		
		if( pIsAngle( lastAngle, 90.0f ) ){
			if( pIsAngle( firstAngle, -90.0f ) ){
				// range [-90°..90°]
				pSample( pVerticalAngles, pVerticalAngleCount, pVerticalSamplePoints, verticalCount180 );
				
			}else if( pIsAngle( firstAngle, 0.0f ) ){
				// range [0°..90°]
				pSample( pVerticalAngles, pVerticalAngleCount, pVerticalSamplePoints, verticalCount90 );
				pFillSamples( pVerticalAngleCount - 1,
					pVerticalSamplePoints + verticalCount90, verticalCount90 );
				
			}else{
				DETHROW_INFO( deeInvalidFileFormat, "invalid first angle" );
			}
			
		}else{
			DETHROW_INFO( deeInvalidFileFormat, "invalid last angle" );
		}
	}
}

bool deIesFormatLm63d1986::pIsAngle( float angle, float requiredAngle ){
	return fabsf( angle - requiredAngle ) < 0.1f;
}

void deIesFormatLm63d1986::pFillSamples( int index, sSamplePoint *samples, int sampleCount ){
	int i;
	for( i=0; i<sampleCount; i++ ){
		samples[ i ].Set( index );
	}
}

void deIesFormatLm63d1986::pSample( const float *angles, int angleCount, sSamplePoint *samples, int sampleCount ){
	if( sampleCount == 1 ){
		pFillSamples( 0, samples, 1 );
		return;
	}
	
	const int lastIndex = angleCount - 1;
	const float firstAngle = angles[ 0 ];
	const float angleStep = ( angles[ lastIndex ] - firstAngle ) / ( sampleCount - 1 );
	int i, prev = 0;
	
	samples[ 0 ].Set( 0 );
	
	for( i=1; i<sampleCount; i++ ){
		const float angle = firstAngle + angleStep * i;
		
		while( prev < lastIndex && angle >= angles[ prev + 1 ] ){
			prev++;
		}
		
		if( prev < lastIndex ){
			samples[ i ].Set( prev, ( angle - angles[ prev ] ) / ( angles[ prev + 1 ] - angles[ prev ] ) );
			
		}else{
			samples[ i ].Set( lastIndex );
		}
	}
}

void deIesFormatLm63d1986::pMirrorSamples( const sSamplePoint *samplesFrom, sSamplePoint *samplesTo, int sampleCount ){
	const int lastIndex = sampleCount - 1;
	int i;
	for( i=0; i<sampleCount; i++ ){
		samplesTo[ i ] = samplesFrom[ lastIndex - i ];
	}
}

void deIesFormatLm63d1986::pCopySamples( const sSamplePoint *samplesFrom, sSamplePoint *samplesTo, int sampleCount ){
	int i;
	for( i=0; i<sampleCount; i++ ){
		samplesTo[ i ] = samplesFrom[ i ];
	}
}

void deIesFormatLm63d1986::pSetPixelsEquirect( unsigned short *pixels ){
	// NOTE candela values are column-major while pixels are row-major
	// NOTE vertical angle 0 degrees is bottom (aka negative Z axis)
	unsigned short *nextPixel = pixels;
	int x, y;
	
	for( y=0; y<pHeight; y++ ){
		const sSamplePoint &sampleVertical = pVerticalSamplePoints[ pHeight - 1 - y ];
		const float * const baseCandelaValues1 = pCandelaValues + sampleVertical.index1;
		const float * const baseCandelaValues2 = pCandelaValues + sampleVertical.index2;
		
		for( x=0; x<pWidth; x++ ){
			const sSamplePoint &sampleHorizontal = pHorizontalSamplePoints[ x ];
			const int offset1 = pVerticalAngleCount * sampleHorizontal.index1;
			const int offset2 = pVerticalAngleCount * sampleHorizontal.index2;
			const float value1 = baseCandelaValues1[ offset1 ] * sampleHorizontal.blend1
				+ baseCandelaValues1[ offset2 ] * sampleHorizontal.blend2;
			const float value2 = baseCandelaValues2[ offset1 ] * sampleHorizontal.blend1
				+ baseCandelaValues2[ offset2 ] * sampleHorizontal.blend2;
			const float value = value1 * sampleVertical.blend1 + value2 * sampleVertical.blend2;
			
			*( nextPixel++ ) = ( unsigned short )( value * 65535.0f );
		}
	}
}

void deIesFormatLm63d1986::pSetPixelsCubemap( unsigned short *pixels ){
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
	*/
	
	const int strideFace = pWidth * pWidth;
	decMatrix matrix;
	int i;
	
	for( i=0; i<6; i++ ){
		pGetMatrixForFace( matrix, i );
		pSetPixelsCubemapFace( pixels + strideFace * i, matrix );
	}
}

void deIesFormatLm63d1986::pSetPixelsCubemapFace( unsigned short *pixels, const decMatrix &matrix ){
	// NOTE - candela values are column-major while pixels are row-major
	// NOTE vertical angle 0 degrees is bottom (aka negative Z axis)
	const float factorSampleAngleHorizontal = ( float )pHorizontalResolution / 360.0f;
	const float factorSampleAngleVertical = ( float )pVerticalResolution / 180.0f;
	const int lastSampleIndexHorizontal = pHorizontalResolution - 1;
	const int lastSampleIndexVertical = pVerticalResolution - 1;
	const int size = pWidth; // same as pHeight
	const float faceOffset = 0.5f - 0.5f * size;
	unsigned short *nextPixel = pixels;
	decVector vector( 0.0f, 0.0f, 0.5f * size );
	int x, y;
	
	for( y=0; y<size; y++ ){
		vector.y = faceOffset + y;
		for( x=0; x<size; x++ ){
			vector.x = faceOffset + x;
			const decVector tvec( matrix.TransformNormal( vector ) );
			
			const float angleVertical = -atan2f( tvec.y, decVector2( tvec.x, tvec.z ).Length() ) * RAD2DEG + 90.0f;
			const float angleHorizontal = atan2f( tvec.x, tvec.z ) * RAD2DEG + 180.0f;
			
			const sSamplePoint &sampleVertical = pVerticalSamplePoints[ decMath::clamp(
				( int )( factorSampleAngleVertical * angleVertical ), 0, lastSampleIndexVertical ) ];
			const sSamplePoint &sampleHorizontal = pHorizontalSamplePoints[ decMath::clamp(
				( int )( factorSampleAngleHorizontal * angleHorizontal ), 0, lastSampleIndexHorizontal ) ];
			
			const float verticalAngle1 = pVerticalAngles[ sampleVertical.index1 ];
			const float verticalAngle2 = pVerticalAngles[ sampleVertical.index2 ];
			const float verticalBlend2 = decMath::linearStep( angleVertical, verticalAngle1, verticalAngle2 );
			const float verticalBlend1 = 1.0f - verticalBlend2;
			
			const float horizontalAngle1 = pHorizontalAngles[ sampleHorizontal.index1 ];
			const float horizontalAngle2 = pHorizontalAngles[ sampleHorizontal.index2 ];
			const float horizontalBlend2 = decMath::linearStep( angleHorizontal, horizontalAngle1, horizontalAngle2 );
			const float horizontalBlend1 = 1.0f - horizontalBlend2;
			
			const float * const baseCandelaValues1 = pCandelaValues + sampleVertical.index1;
			const float * const baseCandelaValues2 = pCandelaValues + sampleVertical.index2;
			const int offset1 = pVerticalAngleCount * sampleHorizontal.index1;
			const int offset2 = pVerticalAngleCount * sampleHorizontal.index2;
			const float value1 = baseCandelaValues1[ offset1 ] * /*sampleHorizontal.blend1*/ horizontalBlend1
				+ baseCandelaValues1[ offset2 ] * /*sampleHorizontal.blend2*/ horizontalBlend2;
			const float value2 = baseCandelaValues2[ offset1 ] * /*sampleHorizontal.blend1*/ horizontalBlend1
				+ baseCandelaValues2[ offset2 ] * /*sampleHorizontal.blend2*/ horizontalBlend2;
			const float value = value1 * /*sampleVertical.blend1*/ verticalBlend1
				+ value2 * /*sampleVertical.blend2*/ verticalBlend2;
			
			*( nextPixel++ ) = ( unsigned short )( value * 65535.0f );
		}
	}
}

void deIesFormatLm63d1986::pGetMatrixForFace( decMatrix &matrix, int face ){
	//matrix.SetCamera( decVector(), decVector( 1.0f, 0.0f, 0.0f ), decVector( 0.0f, 1.0f, 0.0f ) );
	// right = up % view;
	// [ right.x | right.y | right.z | -(position * right) ]
	// [    up.x |    up.y |    up.z |    -(position * up) ]
	// [  view.x |  view.y |  view.z |  -(position * view) ]
	// [    0    |    0    |    0    |          1          ]
	
	switch( face ){
	case 0: // positive x
		//matrix.SetCamera( decVector(), decVector( 1.0f, 0.0f, 0.0f ), decVector( 0.0f, 1.0f, 0.0f ) );
		matrix.a11 =  0.0f; matrix.a12 =  0.0f; matrix.a13 = -1.0f;
		matrix.a21 =  0.0f; matrix.a22 =  1.0f; matrix.a23 =  0.0f;
		matrix.a31 =  1.0f; matrix.a32 =  0.0f; matrix.a33 =  0.0f;
		break;
		
	case 1: // negative x
		//matrix.SetCamera( decVector(), decVector( -1.0f, 0.0f, 0.0f ), decVector( 0.0f, 1.0f, 0.0f ) );
		matrix.a11 =  0.0f; matrix.a12 =  0.0f; matrix.a13 =  1.0f;
		matrix.a21 =  0.0f; matrix.a22 =  1.0f; matrix.a23 =  0.0f;
		matrix.a31 = -1.0f; matrix.a32 =  0.0f; matrix.a33 =  0.0f;
		break;
		
	case 2: // positive y
		//matrix.SetCamera( decVector(), decVector( 0.0f, 1.0f, 0.0f ), decVector( 0.0f, 0.0f, -1.0f ) );
		matrix.a11 =  1.0f; matrix.a12 =  0.0f; matrix.a13 =  0.0f;
		matrix.a21 =  0.0f; matrix.a22 =  0.0f; matrix.a23 = -1.0f;
		matrix.a31 =  0.0f; matrix.a32 =  1.0f; matrix.a33 =  0.0f;
		break;
		
	case 3: // negative y
		//matrix.SetCamera( decVector(), decVector( 0.0f, -1.0f, 0.0f ), decVector( 0.0f, 0.0f, 1.0f ) );
		matrix.a11 =  1.0f; matrix.a12 =  0.0f; matrix.a13 =  0.0f;
		matrix.a21 =  0.0f; matrix.a22 =  0.0f; matrix.a23 =  1.0f;
		matrix.a31 =  0.0f; matrix.a32 = -1.0f; matrix.a33 =  0.0f;
		break;
		
	case 4: // positive z
		//matrix.SetCamera( decVector(), decVector( 0.0f, 0.0f, 1.0f ), decVector( 0.0f, 1.0f, 0.0f ) );
		matrix.a11 =  1.0f; matrix.a12 =  0.0f; matrix.a13 =  0.0f;
		matrix.a21 =  0.0f; matrix.a22 =  1.0f; matrix.a23 =  0.0f;
		matrix.a31 =  0.0f; matrix.a32 =  0.0f; matrix.a33 =  1.0f;
		break;
		
	case 5: // negative z
		//matrix.SetCamera( decVector(), decVector( 0.0f, 0.0f, -1.0f ), decVector( 0.0f, 1.0f, 0.0f ) );
		matrix.a11 = -1.0f; matrix.a12 =  0.0f; matrix.a13 =  0.0f;
		matrix.a21 =  0.0f; matrix.a22 =  1.0f; matrix.a23 =  0.0f;
		matrix.a31 =  0.0f; matrix.a32 =  0.0f; matrix.a33 = -1.0f;
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}
