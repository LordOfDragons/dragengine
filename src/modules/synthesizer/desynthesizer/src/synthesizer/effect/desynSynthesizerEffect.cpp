/* 
 * Drag[en]gine Synthesizer Module
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

#include "desynSynthesizerEffect.h"
#include "../desynSynthesizer.h"
#include "../desynSynthesizerInstance.h"
#include "../source/desynSynthesizerSource.h"
#include "../../deDESynthesizer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffect.h>



// Class desynSynthesizerEffect
/////////////////////////////////

// Constructor, destructor
////////////////////////////

desynSynthesizerEffect::desynSynthesizerEffect( const desynSynthesizer &synthesizer,
int firstLink, desynSynthesizerSource &source, const deSynthesizerEffect &effect ) :
pSynthesizer( synthesizer ),
pSource( source ),
pDisabled( true ),
pStateDataOffset( 0 ),

pTargetStrength( synthesizer, firstLink, effect.GetTargetStrength() ),

pPreviousEffect( NULL ){
}

desynSynthesizerEffect::~desynSynthesizerEffect(){
}



// Management
///////////////

deDESynthesizer &desynSynthesizerEffect::GetModule() const{
	return pSynthesizer.GetModule();
}

void desynSynthesizerEffect::SetDisabled( bool disabled ){
	pDisabled = disabled;
}

void desynSynthesizerEffect::SetStateDataOffset( int offset ){
	pStateDataOffset = decMath::max( offset, 0 );
}



void desynSynthesizerEffect::SetPreviousEffect( desynSynthesizerEffect *effect ){
	pPreviousEffect = effect;
}



float desynSynthesizerEffect::GetStrength( const desynSynthesizerInstance &instance, int sample ) const{
	return pTargetStrength.GetValue( instance, sample, 1.0f );
}



int desynSynthesizerEffect::StateDataSize( int offset ){
	pStateDataOffset = offset;
	return 0;
}

void desynSynthesizerEffect::InitStateData( char *stateData ){
}

void desynSynthesizerEffect::CleanUpStateData( char *stateData ){
}



void desynSynthesizerEffect::GenerateSound( const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor ){
	if( pPreviousEffect ){
		pPreviousEffect->GenerateSound( instance, stateData, buffer, samples, curveOffset, curveFactor );
		
	}else{
		pSource.GenerateSourceSound( instance, stateData, buffer, samples, curveOffset, curveFactor );
	}
}

void desynSynthesizerEffect::SkipSound( const desynSynthesizerInstance &instance,
char *stateData, int samples, float curveOffset, float curveFactor ){
	if( pPreviousEffect ){
		pPreviousEffect->SkipSound( instance, stateData, samples, curveOffset, curveFactor );
		
	}else{
		pSource.SkipSourceSound( instance, stateData, samples, curveOffset, curveFactor );
	}
}
