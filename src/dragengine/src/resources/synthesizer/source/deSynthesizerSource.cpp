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

#include "deSynthesizerSource.h"
#include "deSynthesizerSourceVisitor.h"
#include "../deSynthesizer.h"
#include "../effect/deSynthesizerEffect.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deSynthesizerSource
//////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerSource::deSynthesizerSource() :
pSynthesizer( NULL ),
pMixMode( emmAdd ),
pBlendFactor( 1.0f ),
pEnabled( true ),
pMinVolume( 1.0f ),
pMaxVolume( 1.0f ),
pMinPanning( 0.0f ),
pMaxPanning( 0.0f ){
}

deSynthesizerSource::~deSynthesizerSource(){
}



// Management
///////////////

void deSynthesizerSource::SetSynthesizer( deSynthesizer *synthesizer ){
	pSynthesizer = synthesizer;
}

void deSynthesizerSource::SetEnabled( bool enabled ){
	pEnabled = enabled;
}

void deSynthesizerSource::SetMixMode( eMixModes mode ){
	if( mode < emmBlend || mode > emmAdd ){
		DETHROW( deeInvalidParam );
	}
	pMixMode = mode;
}

void deSynthesizerSource::SetBlendFactor( float factor ){
	pBlendFactor = factor;
}

void deSynthesizerSource::SetMinVolume( float volume ){
	pMinVolume = volume;
}

void deSynthesizerSource::SetMaxVolume( float volume ){
	pMaxVolume = volume;
}

void deSynthesizerSource::SetMinPanning( float panning ){
	pMinPanning = panning;
}

void deSynthesizerSource::SetMaxPanning( float panning ){
	pMaxPanning = panning;
}



// Effects
////////////

int deSynthesizerSource::GetEffectCount() const{
	return pEffects.GetCount();
}

deSynthesizerEffect *deSynthesizerSource::GetEffectAt( int index ) const{
	return ( deSynthesizerEffect* )pEffects.GetAt( index );
}

void deSynthesizerSource::AddEffect( deSynthesizerEffect *effect ){
	if( ! effect ){
		DETHROW( deeInvalidParam );
	}
	pEffects.Add( effect );
}

void deSynthesizerSource::RemoveEffect( deSynthesizerEffect *effect ){
	pEffects.Remove( effect );
}

void deSynthesizerSource::RemoveAllEffects(){
	pEffects.RemoveAll();
}



// Visiting
/////////////

void deSynthesizerSource::Visit( deSynthesizerSourceVisitor &visitor ){
	visitor.VisitSource( *this );
}
