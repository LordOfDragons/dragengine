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

#include "deoalSoundRayInteraction.h"
#include "../../audiothread/deoalAudioThread.h"
#include "../../audiothread/deoalATLogger.h"
#include "../../component/deoalAComponentTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoalSoundRayInteraction
///////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalSoundRayInteraction::deoalSoundRayInteraction() :
pDistance( 1.0f ),
pRayEnds( false ),
pAbsorptionLow( 0.0f ),
pAbsorptionMedium( 0.0f ),
pAbsorptionHigh( 0.0f ),
pTransmissionLow( 1.0f ),
pTransmissionMedium( 1.0f ),
pTransmissionHigh( 1.0f ),
pReflectedLow( 0.0f ),
pReflectedMedium( 0.0f ),
pReflectedHigh( 0.0f ){
}

deoalSoundRayInteraction::deoalSoundRayInteraction( const deoalSoundRayInteraction &element ) :
pDistance( element.pDistance ),
pPoint( element.pPoint ),
pNormal( element.pNormal ),
pRayEnds( element.pRayEnds ),
pAbsorptionLow( element.pAbsorptionLow ),
pAbsorptionMedium( element.pAbsorptionMedium ),
pAbsorptionHigh( element.pAbsorptionHigh ),
pTransmissionLow( element.pTransmissionLow ),
pTransmissionMedium( element.pTransmissionMedium ),
pTransmissionHigh( element.pTransmissionHigh ),
pReflectedLow( element.pReflectedLow ),
pReflectedMedium( element.pReflectedMedium ),
pReflectedHigh( element.pReflectedHigh ){
}



// Manegement
/////////////

void deoalSoundRayInteraction::SetDistance( float distance ){
	pDistance = distance;
}

void deoalSoundRayInteraction::SetPoint( const decDVector &point ){
	pPoint = point;
}

void deoalSoundRayInteraction::SetNormal( const decDVector &normal ){
	pNormal = normal;
}

void deoalSoundRayInteraction::SetRayEnds( bool rayEnds ){
	pRayEnds = rayEnds;
}

void deoalSoundRayInteraction::SetParameters( const deoalAComponentTexture &texture, float distance ){
	pAbsorptionLow = texture.GetAbsorptionLow();
	pAbsorptionMedium = texture.GetAbsorptionMedium();
	pAbsorptionHigh = texture.GetAbsorptionHigh();
	
	const float nonAbsLow = 1.0f - pAbsorptionLow;
	const float nonAbsMedium = 1.0f - pAbsorptionMedium;
	const float nonAbsHigh = 1.0f - pAbsorptionHigh;
	
	pTransmissionLow = decMath::linearStep( distance, 0.0f, texture.GetTransmissionLow(), nonAbsLow, 0.0f );
	pTransmissionMedium = decMath::linearStep( distance, 0.0f, texture.GetTransmissionMedium(), nonAbsMedium, 0.0f );
	pTransmissionHigh = decMath::linearStep( distance, 0.0f, texture.GetTransmissionHigh(), nonAbsHigh, 0.0f );
	
	pReflectedLow = nonAbsLow - pTransmissionLow;
	pReflectedMedium = nonAbsMedium - pTransmissionMedium;
	pReflectedHigh = nonAbsHigh - pTransmissionHigh;
	
	pRayEnds = pTransmissionLow < FLOAT_SAFE_EPSILON && pTransmissionMedium < FLOAT_SAFE_EPSILON
		&& pTransmissionHigh < FLOAT_SAFE_EPSILON;
}

void deoalSoundRayInteraction::SetParameters( const deoalAComponentTexture &texture ){
	pAbsorptionLow = texture.GetAbsorptionLow();
	pAbsorptionMedium = texture.GetAbsorptionMedium();
	pAbsorptionHigh = texture.GetAbsorptionHigh();
	
	pTransmissionLow = 0.0f;
	pTransmissionMedium = 0.0f;
	pTransmissionHigh = 0.0f;
	
	pReflectedLow = 1.0f - pAbsorptionLow;
	pReflectedMedium = 1.0f - pAbsorptionMedium;
	pReflectedHigh = 1.0f - pAbsorptionHigh;
	
	pRayEnds = true;
}



void deoalSoundRayInteraction::DebugPrint( deoalAudioThread &audioThread, const char *prefix ){
	audioThread.GetLogger().LogInfoFormat(
		"%sSRI: distance=%.3f point=(%.3f,%.3f,%.3f) normal(%.3f,%.3f,%.3f) "
		"rayEnds=%d abs(%.3f,%.3f,%.3f) trans(%.3f,%.3f,%.3f) refl(%.3f,%.3f,%.3f)", prefix,
		pDistance, pPoint.x, pPoint.y, pPoint.z, pNormal.x, pNormal.y, pNormal.z, pRayEnds,
		pAbsorptionLow, pAbsorptionMedium, pAbsorptionHigh,
		pTransmissionLow, pTransmissionMedium, pTransmissionHigh,
		pReflectedLow, pReflectedMedium, pReflectedHigh );
}



deoalSoundRayInteraction &deoalSoundRayInteraction::operator=( const deoalSoundRayInteraction &element ){
	pDistance = element.pDistance;
	pPoint = element.pPoint;
	pNormal = element.pNormal;
	pRayEnds = element.pRayEnds;
	pAbsorptionLow = element.pAbsorptionLow;
	pAbsorptionMedium = element.pAbsorptionMedium;
	pAbsorptionHigh = element.pAbsorptionHigh;
	pTransmissionLow = element.pTransmissionLow;
	pTransmissionMedium = element.pTransmissionMedium;
	pTransmissionHigh = element.pTransmissionHigh;
	pReflectedLow = element.pReflectedLow;
	pReflectedMedium = element.pReflectedMedium;
	pReflectedHigh = element.pReflectedHigh;
	return *this;
}
