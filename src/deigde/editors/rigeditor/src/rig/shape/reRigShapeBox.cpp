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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "reRigShapeBox.h"
#include "../reRig.h"
#include "../bone/reRigBone.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/exceptions.h>



// Class reRigShapeBox
////////////////////////

// Constructor, destructor
////////////////////////////

reRigShapeBox::reRigShapeBox( deEngine *engine ) : reRigShape( engine, estBox ){
	pHalfExtends.Set( 0.5f, 0.5f, 0.5f );
}

reRigShapeBox::~reRigShapeBox(){
}



// Management
///////////////

void reRigShapeBox::SetHalfExtends( const decVector &halfExtends ){
	if( ! halfExtends.IsEqualTo( pHalfExtends ) ){
		pHalfExtends = halfExtends;
		NotifyShapeChanged();
	}
}

reRigShape *reRigShapeBox::Duplicate() const{
	reRigShapeBox *shape = NULL;
	
	try{
		shape = new reRigShapeBox( GetEngine() );
		if( ! shape ) DETHROW( deeOutOfMemory );
		
		shape->SetPosition( GetPosition() );
		shape->SetOrientation( GetOrientation() );
		shape->SetHalfExtends( GetHalfExtends() );
		
	}catch( const deException & ){
		if( shape ) shape->FreeReference();
		throw;
	}
	
	return shape;
}

void reRigShapeBox::Scale( float scale ){
	SetPosition( GetPosition() * scale );
	pHalfExtends *= scale;
}

decShape *reRigShapeBox::CreateShape(){
	return new decShapeBox( pHalfExtends, GetPosition(), decQuaternion::CreateFromEuler( GetOrientation() * DEG2RAD ) );
}
