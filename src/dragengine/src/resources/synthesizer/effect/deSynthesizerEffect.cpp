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

#include "deSynthesizerEffect.h"
#include "deSynthesizerEffectVisitor.h"
#include "../deSynthesizer.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deSynthesizerEffect
//////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerEffect::deSynthesizerEffect() :
pSynthesizer( NULL ),
pStrength( 1.0f ),
pEnabled( true ){
}

deSynthesizerEffect::~deSynthesizerEffect(){
}



// Management
///////////////

void deSynthesizerEffect::SetSynthesizer( deSynthesizer *synthesizer ){
	pSynthesizer = synthesizer;
}

void deSynthesizerEffect::SetEnabled( bool enabled ){
	pEnabled = enabled;
}

void deSynthesizerEffect::SetStrength( float strength ){
	pStrength = strength;
}



// Visiting
/////////////

void deSynthesizerEffect::Visit( deSynthesizerEffectVisitor &visitor ){
	visitor.VisitEffect( *this );
}
