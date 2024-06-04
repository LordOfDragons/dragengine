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
#include <string.h>

#include "deoalAComponentBone.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponentBone.h>



// Class deoalAComponentBone
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoalAComponentBone::deoalAComponentBone() :
pIndex( 0 ),
pScale( 1.0f, 1.0f, 1.0f ),
pParent( NULL ){
}

deoalAComponentBone::~deoalAComponentBone(){
}



// Management
///////////////

void deoalAComponentBone::SetIndex( int index ){
	pIndex = index;
}

void deoalAComponentBone::SetName( const char *name ){
	pName = name;
}

void deoalAComponentBone::SetGeometry( const decVector &position,
const decQuaternion &rotation, const decVector &scale ){
	pPosition = position;
	pRotation = rotation;
	pScale = scale;
}

void deoalAComponentBone::SetGeometry( const deComponentBone &bone ){
	pPosition = bone.GetPosition();
	pRotation = bone.GetRotation();
	pScale = bone.GetScale();
}

void deoalAComponentBone::SetOriginalMatrix( const decMatrix &matrix ){
	pOriginalMatrix = matrix;
}

void deoalAComponentBone::SetRigInverseMatrix( const decMatrix &matrix ){
	pRigInverseMatrix = matrix;
}

void deoalAComponentBone::SetParent( deoalAComponentBone *bone ){
	pParent = bone;
}

void deoalAComponentBone::UpdateMatrix(){
	pMatrix = decMatrix::CreateWorld( pPosition, pRotation, pScale ).QuickMultiply( GetOriginalMatrix() );
	if( pParent ){
		pMatrix = pMatrix.QuickMultiply( pParent->pMatrix );
	}
	pWeightMatrix = pRigInverseMatrix.QuickMultiply( pMatrix );
}
