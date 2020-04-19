/* 
 * Drag[en]gine Bullet Physics Module
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

#include "debpFFVortex.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define BP_FFV_TWOPI	( PI * 2.0f )



// Class debpFFVortex
///////////////////////

// Constructor, destructor
////////////////////////////

debpFFVortex::debpFFVortex(){
	pView.Set( 0.0f, 0.0f, 1.0f );
	pSize = 1.0f;
	
	pFullVelocity = 5.0f;
	pCurrentVelocity = 0.0f;
	
	pTimeToLive = 10.0f;
	pLivedTime = 0.0f;
}

debpFFVortex::~debpFFVortex(){
}



// Management
///////////////

void debpFFVortex::SetPosition( const decVector &position ){
	pPosition = position;
}

void debpFFVortex::SetView( const decVector &view ){
	pView = view;
}

void debpFFVortex::SetSize( float size ){
	if( size < 0.001f ) DETHROW( deeInvalidParam );
	
	pSize = size;
}



void debpFFVortex::SetFullVelocity( float velocity ){
	pFullVelocity = velocity;
}

void debpFFVortex::SetCurrentVelocity( float velocity ){
	pCurrentVelocity = velocity;
}



void debpFFVortex::SetTimeToLive( float time ){
	if( time < 0.01f ) DETHROW( deeInvalidParam );
	
	pTimeToLive = time;
}

void debpFFVortex::SetLivedTime( float time ){
	if( time < 0.0f ){
		pLivedTime = 0.0f;
		
	}else{
		pLivedTime = time;
	}
}

void debpFFVortex::IncreaseLivedTime( float time ){
	pLivedTime += time;
	/*
	if( pLivedTime < 0.0f ){
		pLivedTime = 0.0f;
	}
	*/
}



void debpFFVortex::Update(){
	pCurrentVelocity = pFullVelocity * ( 1.0f - cosf( BP_FFV_TWOPI * pLivedTime / pTimeToLive ) * 0.5f );
}
