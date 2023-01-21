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

#include <stdio.h>
#include <stdlib.h>
#include "debpMotionState.h"
#include "LinearMath/btTransform.h"



// class debpMotionState
//////////////////////////

// constructor, destructor
////////////////////////////

debpMotionState::debpMotionState(){
	pScaling.Set( 1.0f, 1.0f, 1.0f );
}

debpMotionState::~debpMotionState(){
}



// management
///////////////

void debpMotionState::SetPosition( const decDVector &position ){
	pPosition = position;
}

void debpMotionState::SetScaling( const decVector &scaling ){
	pScaling = scaling;
}

void debpMotionState::SetOrientation( const decQuaternion &orientation ){
	pOrientation = orientation;
}



// bullet management
//////////////////////

void debpMotionState::getWorldTransform( btTransform &centerOfMassWorldTrans ) const{
	centerOfMassWorldTrans.setOrigin(
		btVector3( pPosition.x, pPosition.y, pPosition.z ) );
	
	centerOfMassWorldTrans.setRotation(
		btQuaternion( pOrientation.x, pOrientation.y, pOrientation.z, pOrientation.w ) );
}

void debpMotionState::setWorldTransform( const btTransform &centerOfMassWorldTrans ){
	const btVector3 &position = centerOfMassWorldTrans.getOrigin();
	const btQuaternion rotation( centerOfMassWorldTrans.getRotation() );
	
	pPosition.Set( position.getX(), position.getY(), position.getZ() );
	pOrientation.Set( rotation.getX(), rotation.getY(), rotation.getZ(), rotation.getW() );
}

bool debpMotionState::deactivationCallback( void *userPointer ){
	return true;
}
