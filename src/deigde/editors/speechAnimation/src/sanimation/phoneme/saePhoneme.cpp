/* 
 * Drag[en]gine IGDE Speech Animation Editor
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "saePhoneme.h"
#include "../saeSAnimation.h"

#include <dragengine/common/exceptions.h>



// Class saePhoneme
/////////////////////

// Constructor, destructor
////////////////////////////

saePhoneme::saePhoneme() :
pSAnimation( nullptr ),
pEngController( -1 ),
pIPA( 0 ),
pMoveName( "viseme" ),
pLength( 0.2f ),
pActive( false ){
}

saePhoneme::saePhoneme( int ipa ) :
pSAnimation( nullptr ),
pEngController( -1 ),
pIPA( ipa ),
pMoveName( "viseme" ),
pLength( 0.2f ),
pActive( false )
{
	DEASSERT_TRUE( ipa >= 0 )
}

saePhoneme::~saePhoneme(){
	SetSAnimation( nullptr );
}



// Management
///////////////

void saePhoneme::SetSAnimation( saeSAnimation *sanimation ){
	pSAnimation = sanimation;
}

void saePhoneme::SetEngineController( int controller ){
	pEngController = controller;
}

void saePhoneme::SetIPA( int ipa ){
	if( ipa == pIPA ){
		return;
	}
	
	pIPA = ipa;
	
	if( pSAnimation ){
		pSAnimation->NotifyPhonemeChanged( this );
	}
}

void saePhoneme::SetLength( float length ){
	if( fabsf( length - pLength ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pLength = length;
	
	if( pSAnimation ){
		pSAnimation->NotifyPhonemeChanged( this );
	}
}

void saePhoneme::SetSampleText( const char *sampleText ){
	if( pSampleText == sampleText ){
		return;
	}
	
	pSampleText = sampleText;
	
	if( pSAnimation ){
		pSAnimation->NotifyPhonemeChanged( this );
	}
}

void saePhoneme::SetMoveName( const char *name ){
	if( pMoveName == name ){
		return;
	}
	
	pMoveName = name;
	
	if( pSAnimation ){
		pSAnimation->NotifyPhonemeChanged( this );
	}
}

void saePhoneme::SetVertexPositionSet( const char *name ){
	if( pVertexPositionSet == name ){
		return;
	}
	
	pVertexPositionSet = name;
	
	if( pSAnimation ){
		pSAnimation->NotifyPhonemeChanged( this );
	}
}

void saePhoneme::SetActive( bool active ){
	pActive = active;
}
