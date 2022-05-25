/* 
 * Drag[en]gine OpenXR
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#include "deVROpenXR.h"
#include "deoxrFaceTracker.h"
#include "deoxrSession.h"
#include "deoxrUtils.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class deoxrFaceTracker
///////////////////////////

deoxrFaceTracker::deoxrFaceTracker( deoxrSession &session ) :
pSession( session ),
pEyeTracker( XR_NULL_HANDLE ),
pLipTracker( XR_NULL_HANDLE ),
pEyeWeights( nullptr ),
pLipWeights( nullptr ),
pFaceExpressions( nullptr ),
pFaceExpressionCount( 0 ),
pMapEyeXrToDe( nullptr ),
pMapEyeXrToDeCount( 0 ),
pMapLipXrToDe( nullptr ),
pMapLipXrToDeCount( 0 )
{
	deoxrInstance &instance = session.GetSystem().GetInstance();
	
	try{
		// create input arrays to store data to be send to user
		pFaceExpressions = new float[ deInputDevice::FaceExpressionCount ];
		pFaceExpressionCount = deInputDevice::FaceExpressionCount;
		
		// create eye tracker
		if( session.GetSystem().GetSupportsFaceEyeTracking() ){
			XrFacialTrackerCreateInfoHTC createInfo;
			memset( &createInfo, 0, sizeof( createInfo ) );
			createInfo.type = XR_TYPE_FACIAL_TRACKER_CREATE_INFO_HTC;
			createInfo.facialTrackingType = XR_FACIAL_TRACKING_TYPE_EYE_DEFAULT_HTC;
			
			OXR_CHECK( instance.xrCreateFacialTrackerHTC( session.GetSession(), &createInfo, &pEyeTracker ) );
			
			// create arrays to store weights
			pEyeWeights = new float[ XR_FACIAL_EXPRESSION_EYE_COUNT_HTC ];
			memset( pEyeWeights, 0, sizeof( float ) * XR_FACIAL_EXPRESSION_EYE_COUNT_HTC );
			
			// initialize structures used to fetch weights
			memset( &pEyeExpressionInfo, 0, sizeof( pEyeExpressionInfo ) );
			pEyeExpressionInfo.type = XR_TYPE_FACIAL_EXPRESSIONS_HTC;
			pEyeExpressionInfo.expressionWeightings = pEyeWeights;
			pEyeExpressionInfo.expressionCount = XR_FACIAL_EXPRESSION_EYE_COUNT_HTC;
			
			// create mapping table.
			pMapEyeXrToDe = new sMapping[ 14 ];
			
			pSetEyeMapping( 0, deInputDevice::efeEyeLeftBlink, XR_EYE_EXPRESSION_LEFT_BLINK_HTC );
			pSetEyeMapping( 1, deInputDevice::efeEyeLeftWide, XR_EYE_EXPRESSION_LEFT_WIDE_HTC );
			pSetEyeMapping( 2, deInputDevice::efeEyeLeftSqueeze, XR_EYE_EXPRESSION_LEFT_SQUEEZE_HTC );
			pSetEyeMapping( 3, deInputDevice::efeEyeLeftDown, XR_EYE_EXPRESSION_LEFT_DOWN_HTC );
			pSetEyeMapping( 4, deInputDevice::efeEyeLeftUp, XR_EYE_EXPRESSION_LEFT_UP_HTC );
			pSetEyeMapping( 5, deInputDevice::efeEyeLeftIn, XR_EYE_EXPRESSION_LEFT_IN_HTC );
			pSetEyeMapping( 6, deInputDevice::efeEyeLeftOut, XR_EYE_EXPRESSION_LEFT_OUT_HTC );
			
			pSetEyeMapping( 7, deInputDevice::efeEyeRightBlink, XR_EYE_EXPRESSION_RIGHT_BLINK_HTC );
			pSetEyeMapping( 8, deInputDevice::efeEyeRightWide, XR_EYE_EXPRESSION_RIGHT_WIDE_HTC );
			pSetEyeMapping( 9, deInputDevice::efeEyeRightSqueeze, XR_EYE_EXPRESSION_RIGHT_SQUEEZE_HTC );
			pSetEyeMapping( 10, deInputDevice::efeEyeRightDown, XR_EYE_EXPRESSION_RIGHT_DOWN_HTC );
			pSetEyeMapping( 11, deInputDevice::efeEyeRightUp, XR_EYE_EXPRESSION_RIGHT_UP_HTC );
			pSetEyeMapping( 12, deInputDevice::efeEyeRightIn, XR_EYE_EXPRESSION_RIGHT_IN_HTC );
			pSetEyeMapping( 13, deInputDevice::efeEyeRightOut, XR_EYE_EXPRESSION_RIGHT_OUT_HTC );
			
			pMapEyeXrToDeCount = 14;
		}
		
		// create lip tracker
		if( session.GetSystem().GetSupportsFaceLipTracking() ){
			XrFacialTrackerCreateInfoHTC createInfo;
			memset( &createInfo, 0, sizeof( createInfo ) );
			createInfo.type = XR_TYPE_FACIAL_TRACKER_CREATE_INFO_HTC;
			createInfo.facialTrackingType = XR_FACIAL_TRACKING_TYPE_LIP_DEFAULT_HTC;
			
			OXR_CHECK( instance.xrCreateFacialTrackerHTC( session.GetSession(), &createInfo, &pLipTracker ) );
			
			// create arrays to store weights
			pLipWeights = new float[ XR_FACIAL_EXPRESSION_LIP_COUNT_HTC ];
			memset( pLipWeights, 0, sizeof( float ) * XR_FACIAL_EXPRESSION_LIP_COUNT_HTC );
			
			// initialize structures used to fetch weights
			memset( &pLipExpressionInfo, 0, sizeof( pLipExpressionInfo ) );
			pLipExpressionInfo.type = XR_TYPE_FACIAL_EXPRESSIONS_HTC;
			pLipExpressionInfo.expressionWeightings = pLipWeights;
			pLipExpressionInfo.expressionCount = XR_FACIAL_EXPRESSION_LIP_COUNT_HTC;
			
			// create mapping table.
			pMapLipXrToDe = new sMapping[ 33 ];
			
			pSetLipMapping( 0, deInputDevice::efeJawRight, XR_LIP_EXPRESSION_JAW_RIGHT_HTC );
			pSetLipMapping( 1, deInputDevice::efeJawLeft, XR_LIP_EXPRESSION_JAW_LEFT_HTC );
			pSetLipMapping( 2, deInputDevice::efeJawForward, XR_LIP_EXPRESSION_JAW_FORWARD_HTC );
			pSetLipMapping( 3, deInputDevice::efeJawOpen, XR_LIP_EXPRESSION_JAW_OPEN_HTC );
			
			pSetLipMapping( 4, deInputDevice::efeCheekPuffRight, XR_LIP_EXPRESSION_CHEEK_PUFF_RIGHT_HTC );
			pSetLipMapping( 5, deInputDevice::efeCheekPuffLeft, XR_LIP_EXPRESSION_CHEEK_PUFF_LEFT_HTC );
			pSetLipMapping( 6, deInputDevice::efeCheekSuck, XR_LIP_EXPRESSION_CHEEK_SUCK_HTC );
			
			pSetLipMapping( 7, deInputDevice::efeMouthApeShape, XR_LIP_EXPRESSION_MOUTH_APE_SHAPE_HTC );
			pSetLipMapping( 8, deInputDevice::efeMouthUpperRight, XR_LIP_EXPRESSION_MOUTH_UPPER_RIGHT_HTC );
			pSetLipMapping( 9, deInputDevice::efeMouthUpperLeft, XR_LIP_EXPRESSION_MOUTH_UPPER_LEFT_HTC );
			pSetLipMapping( 10, deInputDevice::efeMouthUpperUpRight, XR_LIP_EXPRESSION_MOUTH_UPPER_UPRIGHT_HTC );
			pSetLipMapping( 11, deInputDevice::efeMouthUpperUpLeft, XR_LIP_EXPRESSION_MOUTH_UPPER_UPLEFT_HTC );
			pSetLipMapping( 12, deInputDevice::efeMouthUpperOverturn, XR_LIP_EXPRESSION_MOUTH_UPPER_OVERTURN_HTC );
			pSetLipMapping( 13, deInputDevice::efeMouthUpperInside, XR_LIP_EXPRESSION_MOUTH_UPPER_INSIDE_HTC );
			pSetLipMapping( 14, deInputDevice::efeMouthLowerRight, XR_LIP_EXPRESSION_MOUTH_LOWER_RIGHT_HTC );
			pSetLipMapping( 15, deInputDevice::efeMouthLowerLeft, XR_LIP_EXPRESSION_MOUTH_LOWER_LEFT_HTC );
			pSetLipMapping( 16, deInputDevice::efeMouthLowerDownRight, XR_LIP_EXPRESSION_MOUTH_LOWER_DOWNRIGHT_HTC );
			pSetLipMapping( 17, deInputDevice::efeMouthLowerDownLeft, XR_LIP_EXPRESSION_MOUTH_LOWER_DOWNLEFT_HTC );
			pSetLipMapping( 18, deInputDevice::efeMouthLowerOverturn, XR_LIP_EXPRESSION_MOUTH_LOWER_OVERTURN_HTC );
			pSetLipMapping( 19, deInputDevice::efeMouthLowerInside, XR_LIP_EXPRESSION_MOUTH_LOWER_INSIDE_HTC );
			pSetLipMapping( 20, deInputDevice::efeMouthLowerOverlay, XR_LIP_EXPRESSION_MOUTH_LOWER_OVERLAY_HTC );
			pSetLipMapping( 21, deInputDevice::efeMouthPout, XR_LIP_EXPRESSION_MOUTH_POUT_HTC );
			pSetLipMapping( 22, deInputDevice::efeMouthSmileRight, XR_LIP_EXPRESSION_MOUTH_SMILE_RIGHT_HTC );
			pSetLipMapping( 23, deInputDevice::efeMouthSmileLeft, XR_LIP_EXPRESSION_MOUTH_SMILE_LEFT_HTC );
			pSetLipMapping( 24, deInputDevice::efeMouthSadRight, XR_LIP_EXPRESSION_MOUTH_SAD_RIGHT_HTC );
			pSetLipMapping( 25, deInputDevice::efeMouthSadLeft, XR_LIP_EXPRESSION_MOUTH_SAD_LEFT_HTC );
			
			pSetLipMapping( 26, deInputDevice::efeTongueLeft, XR_LIP_EXPRESSION_TONGUE_LEFT_HTC );
			pSetLipMapping( 27, deInputDevice::efeTongueRight, XR_LIP_EXPRESSION_TONGUE_RIGHT_HTC );
			pSetLipMapping( 28, deInputDevice::efeTongueUp, XR_LIP_EXPRESSION_TONGUE_UP_HTC );
			pSetLipMapping( 29, deInputDevice::efeTongueDown, XR_LIP_EXPRESSION_TONGUE_DOWN_HTC );
			pSetLipMapping( 30, deInputDevice::efeTongueRoll, XR_LIP_EXPRESSION_TONGUE_ROLL_HTC );
			pSetLipMapping( 31, deInputDevice::efeTongueLongStep1, XR_LIP_EXPRESSION_TONGUE_LONGSTEP1_HTC );
			pSetLipMapping( 32, deInputDevice::efeTongueLongStep2, XR_LIP_EXPRESSION_TONGUE_LONGSTEP2_HTC );
			
			pMapLipXrToDeCount = 33;
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoxrFaceTracker::~deoxrFaceTracker(){
	pCleanUp();
}



// Management
///////////////

void deoxrFaceTracker::Update() {
	const deoxrInstance &instance = pSession.GetSystem().GetInstance();
	const XrTime sampleTime = pSession.GetPredictedDisplayTime();
	
	if( pEyeTracker != XR_NULL_HANDLE ){
		pEyeExpressionInfo.sampleTime = sampleTime;
		
		if( XR_SUCCEEDED( instance.xrGetFacialExpressionsHTC( pEyeTracker, &pEyeExpressionInfo ) )
		&& pEyeExpressionInfo.isActive ){
			int i;
			for( i=0; i<pMapEyeXrToDeCount; i++ ){
				const sMapping &mapping = pMapEyeXrToDe[ i ];
				*mapping.faceExpression = *mapping.weight;
			}
		}
	}
	
	if( pLipTracker != XR_NULL_HANDLE ){
		pLipExpressionInfo.sampleTime = sampleTime;
		
		if( XR_SUCCEEDED( instance.xrGetFacialExpressionsHTC( pLipTracker, &pLipExpressionInfo ) )
		&& pLipExpressionInfo.isActive ){
			int i;
			for( i=0; i<pMapLipXrToDeCount; i++ ){
				const sMapping &mapping = pMapLipXrToDe[ i ];
				*mapping.faceExpression = *mapping.weight;
			}
		}
	}
}

float deoxrFaceTracker::GetFaceExpressionAt( int index ){
	if( index < 0 || index >= pFaceExpressionCount ){
		return 0.0f;
	}
	return pFaceExpressions[ index ];
}



// Private Functions
//////////////////////

void deoxrFaceTracker::pCleanUp(){
	if( pEyeTracker ){
		pSession.GetSystem().GetInstance().xrDestroyFacialTrackerHTC( pEyeTracker );
	}
	if( pLipTracker ){
		pSession.GetSystem().GetInstance().xrDestroyFacialTrackerHTC( pLipTracker );
	}
	
	if( pMapEyeXrToDe ){
		delete [] pMapEyeXrToDe;
	}
	if( pMapLipXrToDe ){
		delete [] pMapLipXrToDe;
	}
	
	if( pFaceExpressions ){
		delete [] pFaceExpressions;
	}
}

void deoxrFaceTracker::pSetEyeMapping( int index, deInputDevice::eFaceExpressions to, XrEyeExpressionHTC from ){
	pMapEyeXrToDe[ index ].weight = pEyeWeights + from;
	pMapEyeXrToDe[ index ].faceExpression = pFaceExpressions + to;
}

void deoxrFaceTracker::pSetLipMapping( int index, deInputDevice::eFaceExpressions to, XrLipExpressionHTC from ){
	pMapLipXrToDe[ index ].weight = pLipWeights + from;
	pMapLipXrToDe[ index ].faceExpression = pFaceExpressions + to;
}
