/* 
 * Drag[en]gine Game Engine
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

#include "deSkinMapped.h"
#include "../../common/exceptions.h"



// Class deSkinMapped
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinMapped::deSkinMapped ( const char *name ) :
pName( name ),
pInputType( eitTime ),
pInputLower( 0.0f ),
pInputUpper( 1.0f ),
pInputClamped( false ),
pOutputLower( 0.0f ),
pOutputUpper( 1.0f ),
pRenderableComponent( ercRed ){
}

deSkinMapped::~deSkinMapped(){
}



// Management
///////////////

void deSkinMapped::SetInputType( eInputTypes inputType ){
	pInputType = inputType;
}

void deSkinMapped::SetInputLower( float lower ){
	pInputLower = lower;
}

void deSkinMapped::SetInputUpper( float upper ){
	pInputUpper = upper;
}

void deSkinMapped::SetInputClamped( bool inputClamped ){
	pInputClamped = inputClamped;
}

void deSkinMapped::SetOutputLower( float lower ){
	pOutputLower = lower;
}

void deSkinMapped::SetOutputUpper( float upper ){
	pOutputUpper = upper;
}

void deSkinMapped::SetBone( const char *bone ){
	pBone = bone;
}

void deSkinMapped::SetRenderable( const char *renderable ){
	pRenderable = renderable;
}

void deSkinMapped::SetRenderableComponent( eRenderableComponent component ){
	pRenderableComponent = component;
}



deSkinMapped &deSkinMapped::operator=( const deSkinMapped &mapped ){
	pCurve = mapped.pCurve;
	pInputType = mapped.pInputType;
	pInputLower = mapped.pInputLower;
	pInputUpper = mapped.pInputUpper;
	pInputClamped = mapped.pInputClamped;
	pOutputLower = mapped.pOutputLower;
	pOutputUpper = mapped.pOutputUpper;
	pBone = mapped.pBone;
	pRenderable = mapped.pRenderable;
	pRenderableComponent = mapped.pRenderableComponent;
	return *this;
}
