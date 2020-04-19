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
#include "deSkinPropertyMapped.h"
#include "deSkinPropertyVisitor.h"
#include "../../../common/exceptions.h"



// Class deSkinPropertyMapped::cComponent
//////////////////////////////////////////////

deSkinPropertyMapped::cComponent::cComponent() :
pInputType( deSkinPropertyMapped::eitTime ),
pInputLower( 0.0f ),
pInputUpper( 1.0f ),
pInputClamped( false ){
}

deSkinPropertyMapped::cComponent::cComponent( const deSkinPropertyMapped::cComponent &component ) :
pCurve( component.pCurve ),
pInputType( component.pInputType ),
pInputLower( component.pInputLower ),
pInputUpper( component.pInputUpper ),
pInputClamped( component.pInputClamped ),
pBone( component.pBone ){
}

void deSkinPropertyMapped::cComponent::SetInputType( eInputTypes inputType ){
	pInputType = inputType;
}

void deSkinPropertyMapped::cComponent::SetInputLower( float lower ){
	pInputLower = lower;
}

void deSkinPropertyMapped::cComponent::SetInputUpper( float upper ){
	pInputUpper = upper;
}

void deSkinPropertyMapped::cComponent::SetInputClamped( bool inputClamped ){
	pInputClamped = inputClamped;
}

void deSkinPropertyMapped::cComponent::SetBone( const char *bone ){
	pBone = bone;
}

deSkinPropertyMapped::cComponent &deSkinPropertyMapped::cComponent::operator=(
const deSkinPropertyMapped::cComponent &component ){
	pCurve = component.pCurve;
	pInputType = component.pInputType;
	pInputLower = component.pInputLower;
	pInputUpper = component.pInputUpper;
	pInputClamped = component.pInputClamped;
	pBone = component.pBone;
	return *this;
}



// Class deSkinPropertyMapped
///////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyMapped::deSkinPropertyMapped( const char *type ) : deSkinProperty( type ){
}

deSkinPropertyMapped::~deSkinPropertyMapped(){
}



// Management
///////////////

deSkinPropertyMapped::cComponent &deSkinPropertyMapped::GetComponent( int component ){
	if( component < 0 || component > 3 ){
		DETHROW( deeInvalidParam );
	}
	return pComponents[ component ];
}

const deSkinPropertyMapped::cComponent &deSkinPropertyMapped::GetComponent( int component ) const{
	if( component < 0 || component > 3 ){
		DETHROW( deeInvalidParam );
	}
	return pComponents[ component ];
}



// Visiting
/////////////

void deSkinPropertyMapped::Visit( deSkinPropertyVisitor &visitor ){
	visitor.VisitMapped( *this );
}
