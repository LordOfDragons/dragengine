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
#include "reCreateRigShape.h"
#include "../rig/shape/reRigShapeBox.h"
#include "../rig/shape/reRigShapeSphere.h"
#include "../rig/shape/reRigShapeCapsule.h"
#include "../rig/shape/reRigShapeCylinder.h"
#include "../rig/shape/reRigShapeHull.h"
#include "dragengine/deEngine.h"
#include "dragengine/common/shape/decShape.h"
#include "dragengine/common/shape/decShapeBox.h"
#include "dragengine/common/shape/decShapeSphere.h"
#include "dragengine/common/shape/decShapeCylinder.h"
#include "dragengine/common/shape/decShapeCapsule.h"
#include <dragengine/common/shape/decShapeHull.h>
#include "dragengine/common/exceptions.h"



// Class reCreateRigShape
///////////////////////////

// Constructor, destructor
////////////////////////////

reCreateRigShape::reCreateRigShape( deEngine *engine ){
	if( ! engine ) DETHROW( deeInvalidParam );
	
	pEngine = engine;
	pRigShape = NULL;
}

reCreateRigShape::~reCreateRigShape(){
	if( pRigShape ) pRigShape->FreeReference();
}



// Management
///////////////

void reCreateRigShape::Reset(){
	if( pRigShape ) pRigShape->FreeReference();
	pRigShape = NULL;
}



// Visiting
/////////////

void reCreateRigShape::VisitShape( decShape &shape ){
	Reset();
}

void reCreateRigShape::VisitShapeSphere( decShapeSphere &sphere ){
	reRigShapeSphere *rigSphere = NULL;
	
	Reset();
	
	rigSphere = new reRigShapeSphere( pEngine );
	if( ! rigSphere ) DETHROW( deeOutOfMemory );
	
	pRigShape = rigSphere;
	
	rigSphere->SetPosition( sphere.GetPosition() );
	rigSphere->SetRadius( sphere.GetRadius() );
}

void reCreateRigShape::VisitShapeBox( decShapeBox &box ){
	reRigShapeBox *rigBox = NULL;
	
	Reset();
	
	rigBox = new reRigShapeBox( pEngine );
	if( ! rigBox ) DETHROW( deeOutOfMemory );
	
	pRigShape = rigBox;
	
	rigBox->SetPosition( box.GetPosition() );
	rigBox->SetOrientation( decMatrix::CreateFromQuaternion( box.GetOrientation() ).GetEulerAngles() * RAD2DEG );
	rigBox->SetHalfExtends( box.GetHalfExtends() );
}

void reCreateRigShape::VisitShapeCylinder( decShapeCylinder &cylinder ){
	reRigShapeCylinder *rigCylinder = NULL;
	
	Reset();
	
	rigCylinder = new reRigShapeCylinder( pEngine );
	if( ! rigCylinder ) DETHROW( deeOutOfMemory );
	
	pRigShape = rigCylinder;
	
	rigCylinder->SetPosition( cylinder.GetPosition() );
	rigCylinder->SetOrientation( decMatrix::CreateFromQuaternion( cylinder.GetOrientation() ).GetEulerAngles() * RAD2DEG );
	rigCylinder->SetHalfHeight( cylinder.GetHalfHeight() );
	rigCylinder->SetTopRadius( cylinder.GetTopRadius() );
	rigCylinder->SetBottomRadius( cylinder.GetBottomRadius() );
}

void reCreateRigShape::VisitShapeCapsule( decShapeCapsule &capsule ){
	reRigShapeCapsule *rigCapsule = NULL;
	
	Reset();
	
	rigCapsule = new reRigShapeCapsule( pEngine );
	if( ! rigCapsule ) DETHROW( deeOutOfMemory );
	
	pRigShape = rigCapsule;
	
	rigCapsule->SetPosition( capsule.GetPosition() );
	rigCapsule->SetOrientation( decMatrix::CreateFromQuaternion( capsule.GetOrientation() ).GetEulerAngles() * RAD2DEG );
	rigCapsule->SetHalfHeight( capsule.GetHalfHeight() );
	rigCapsule->SetTopRadius( capsule.GetTopRadius() );
	rigCapsule->SetBottomRadius( capsule.GetBottomRadius() );
}

void reCreateRigShape::VisitShapeHull( decShapeHull &hull ){
	Reset();
	
	reRigShapeHull * const rigHull = new reRigShapeHull( pEngine );
	pRigShape = rigHull;
	
	rigHull->SetPosition( hull.GetPosition() );
	rigHull->SetOrientation( decMatrix::CreateFromQuaternion( hull.GetOrientation() ).GetEulerAngles() * RAD2DEG );
	const int count = hull.GetPointCount();
	int i;
	for( i=0; i<count; i++ ){
		rigHull->AddPoint( hull.GetPointAt( i ) );
	}
}
