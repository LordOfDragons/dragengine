/* 
 * Drag[en]gine IGDE Skin Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "seMapped.h"
#include "../seSkin.h"
#include "../property/seProperty.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>



// Class seMapped
////////////////////

// Constructor, destructor
////////////////////////////

seMapped::seMapped( const char *name ) :
pSkin( nullptr ),
pName( name ),
pInputType( deSkinMapped::eitTime ),
pInputLower( 0.0f ),
pInputUpper( 1.0f ),
pInputClamped( false ),
pSelected( false ),
pActive( false )
{
	DEASSERT_FALSE( pName.IsEmpty() )
}

seMapped::seMapped( const seMapped &mapped ) :
pSkin( nullptr ),
pName( mapped.pName ),
pInputType( mapped.pInputType ),
pInputLower( mapped.pInputLower ),
pInputUpper( mapped.pInputUpper ),
pInputClamped( mapped.pInputClamped ),
pSelected( false ),
pActive( false ){
}

seMapped::~seMapped(){
}



// Management
///////////////

void seMapped::SetSkin( seSkin *skin ){
	pSkin = skin;
}



void seMapped::SetName( const char *name ){
	if( pName == name ){
		return;
	}
	
	pName = name;
	NotifyNameChanged();
}

void seMapped::SetCurve( const decCurveBezier &curve ){
	if( curve == pCurve ){
		return;
	}
	
	pCurve = curve;
	NotifyChanged();
}

void seMapped::SetInputType( deSkinMapped::eInputTypes inputType ){
	if( inputType == pInputType ){
		return;
	}
	
	pInputType = inputType;
	NotifyChanged();
}

void seMapped::SetInputLower( float lower ){
	if( fabsf( lower - pInputLower ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pInputLower = lower;
	NotifyChanged();
}

void seMapped::SetInputUpper( float upper ){
	if( fabsf( upper - pInputUpper ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pInputUpper = upper;
	NotifyChanged();
}

void seMapped::SetInputClamped( bool inputClamped ){
	if( inputClamped == pInputClamped ){
		return;
	}
	
	pInputClamped = inputClamped;
	NotifyChanged();
}

void seMapped::SetBone( const char *bone ){
	if( pBone == bone ){
		return;
	}
	
	pBone = bone;
	NotifyChanged();
}



void seMapped::SetSelected( bool selected ){
	pSelected = selected;
}

void seMapped::SetActive( bool active ){
	pActive = active;
}



void seMapped::NotifyChanged(){
	if( pSkin ){
		pSkin->NotifyMappedChanged( this );
	}
}

void seMapped::NotifyNameChanged(){
	if( pSkin ){
		pSkin->NotifyMappedNameChanged( this );
	}
}
