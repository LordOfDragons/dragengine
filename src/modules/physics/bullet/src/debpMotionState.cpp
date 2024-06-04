/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
