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

#include "deSynthesizerSource.h"
#include "deSynthesizerSourceChain.h"
#include "deSynthesizerSourceGroup.h"
#include "deSynthesizerSourceSound.h"
#include "deSynthesizerSourceSynthesizer.h"
#include "deSynthesizerSourceVisitorIdentify.h"
#include "deSynthesizerSourceWave.h"

#include "../../../common/exceptions.h"



// Class deSynthesizerSourceVisitorIdentify
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerSourceVisitorIdentify::deSynthesizerSourceVisitorIdentify() :
pSource( NULL ),
pType( estUnknown ){
}

deSynthesizerSourceVisitorIdentify::~deSynthesizerSourceVisitorIdentify(){
}



// Management
///////////////

deSynthesizerSourceSound &deSynthesizerSourceVisitorIdentify::CastToSound() const{
	if( pType != estSound ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deSynthesizerSourceSound* )pSource );
}

deSynthesizerSourceWave &deSynthesizerSourceVisitorIdentify::CastToWave() const{
	if( pType != estWave ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deSynthesizerSourceWave* )pSource );
}

deSynthesizerSourceChain &deSynthesizerSourceVisitorIdentify::CastToChain() const{
	if( pType != estChain ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deSynthesizerSourceChain* )pSource );
}

deSynthesizerSourceSynthesizer &deSynthesizerSourceVisitorIdentify::CastToSynthesizer() const{
	if( pType != estSynthesizer ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deSynthesizerSourceSynthesizer* )pSource );
}

deSynthesizerSourceGroup &deSynthesizerSourceVisitorIdentify::CastToGroup() const{
	if( pType != estGroup ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deSynthesizerSourceGroup* )pSource );
}

void deSynthesizerSourceVisitorIdentify::Reset(){
	pSource = NULL;
	pType = estUnknown;
}



// Visiting
/////////////

void deSynthesizerSourceVisitorIdentify::VisitSource( deSynthesizerSource &source ){
	pSource = &source;
	pType = estUnknown;
}

void deSynthesizerSourceVisitorIdentify::VisitSound( deSynthesizerSourceSound &source ){
	pSource = &source;
	pType = estSound;
}

void deSynthesizerSourceVisitorIdentify::VisitWave( deSynthesizerSourceWave &source ){
	pSource = &source;
	pType = estWave;
}

void deSynthesizerSourceVisitorIdentify::VisitChain( deSynthesizerSourceChain &source ){
	pSource = &source;
	pType = estChain;
}

void deSynthesizerSourceVisitorIdentify::VisitSynthesizer( deSynthesizerSourceSynthesizer &source ){
	pSource = &source;
	pType = estSynthesizer;
}

void deSynthesizerSourceVisitorIdentify::VisitGroup( deSynthesizerSourceGroup &source ){
	pSource = &source;
	pType = estGroup;
}
