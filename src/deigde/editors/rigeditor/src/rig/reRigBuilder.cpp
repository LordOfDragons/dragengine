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

#include "reRig.h"
#include "reRigBuilder.h"
#include "bone/reRigBone.h"
#include "shape/reRigShape.h"
#include "constraint/reRigConstraint.h"

#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/rig/deRigConstraint.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/exceptions.h>



// Class reRigBuilder
///////////////////////

// Constructor, destructor
////////////////////////////

reRigBuilder::reRigBuilder( reRig *rig ){
	if( ! rig ){
		DETHROW( deeInvalidParam );
	}
	pRig = rig;
}

reRigBuilder::~reRigBuilder(){
}



// Management
///////////////

void reRigBuilder::BuildRig( deRig *engRig ){
	if( ! engRig ) DETHROW( deeInvalidParam );
	int s, shapeCount = pRig->GetShapeCount();
	int b, boneCount = pRig->GetBoneCount();
	decShape *shape = NULL;
	decStringList shapeProperties;
	decShapeList shapes;
	
	// TODO: constraints are wrong if the rig changes while a pose different
	// than the rest pose is used. most probably some incorrect bone position
	// is used to initialize the rig. all works well when the bones are in
	// rest position and then are posed for simulation. just changes while
	// in pose mode cranks up the build rig. One solution would be to put
	// all bones in rest mode for building the rig and then placing them
	// back in pose mode afterwards.
	for( b=0; b<boneCount; b++ ){
		pRig->GetBoneAt( b )->SetPoseFromRest();
	}
	
	try{
		// parameters
		engRig->SetCentralMassPoint( pRig->GetCentralMassPoint() );
		engRig->SetModelCollision( pRig->GetModelCollision() );
		
		// add bones. we have to respect the order of the bones to create
		// a valid engine rig.
		for( b=0; b<boneCount; b++ ){
			BuildRigBone( engRig, pRig->GetBoneWithOrder( b ) );
		}
		
		// add shapes
		for( s=0; s<shapeCount; s++ ){
			shape = pRig->GetShapeAt( s )->CreateShape();
			
			shapes.Add( shape );
			shape = NULL;
			
			shapeProperties.Add( pRig->GetShapeAt( s )->GetProperty() );
		}
		
		// root bone
		if( pRig->GetRootBone() ){
			engRig->SetRootBone( pRig->GetRootBone()->GetOrder() );
			
		}else{
			engRig->SetRootBone( -1 );
		}
		
	}catch( const deException & ){
		if( shape ) delete shape;
	}
	
	engRig->SetShapes( shapes );
	engRig->SetShapeProperties( shapeProperties );
}

void reRigBuilder::BuildRigBone( deRig *engRig, reRigBone *rigBone ){
	if( ! engRig || ! rigBone ) DETHROW( deeInvalidParam );
	
	int c, constraintCount = rigBone->GetConstraintCount();
	int s, shapeCount = rigBone->GetShapeCount();
	deRigConstraint *engConstraint = NULL;
	deRigBone *engBone = NULL;
	reRigBone *parentBone;
	decShape *shape = NULL;
	decStringList shapeProperties;
	decShapeList shapes;
	
	try{
		// create engine bone
		engBone = new deRigBone( rigBone->GetName().GetString() );
		if( ! engBone ) DETHROW( deeOutOfMemory );
		
		// set options
		parentBone = rigBone->GetParentBone();
		if( parentBone ){
			engBone->SetParent( parentBone->GetOrder() );
			
		}else{
			engBone->SetParent( -1 );
		}
		
		engBone->SetPosition( rigBone->GetPosition() );
		engBone->SetRotation( rigBone->GetOrientation() * DEG2RAD );
		engBone->SetCentralMassPoint( rigBone->GetCentralMassPoint() );
		engBone->SetMatrices( rigBone->GetMatrix() );
		engBone->SetDynamic( rigBone->GetDynamic() );
		engBone->SetMass( rigBone->GetMass() );
		
		// ik limits
		engBone->SetIKLimits( rigBone->GetIKLimitsLower() * DEG2RAD, rigBone->GetIKLimitsUpper() * DEG2RAD );
		engBone->SetIKResistance( rigBone->GetIKResistance() );
		engBone->SetIKLockedX( rigBone->GetIKLockedX() );
		engBone->SetIKLockedY( rigBone->GetIKLockedY() );
		engBone->SetIKLockedZ( rigBone->GetIKLockedZ() );
		
		// add shapes
		for( s=0; s<shapeCount; s++ ){
			shape = rigBone->GetShapeAt( s )->CreateShape();
			
			shapes.Add( shape );
			shape = NULL;
			
			shapeProperties.Add( rigBone->GetShapeAt( s )->GetProperty() );
		}
		
		// add constraints
		for( c=0; c<constraintCount; c++ ){
			engConstraint = rigBone->GetConstraintAt( c )->BuildEngineRigConstraint();
			if( ! engConstraint ) DETHROW( deeOutOfMemory );
			
			engBone->AddConstraint( engConstraint );
			engConstraint = NULL;
		}
		
		// add to the rig
		engRig->AddBone( engBone );
		
	}catch( const deException & ){
		if( engConstraint ) delete engConstraint;
		if( shape ) delete shape;
		if( engBone ) delete engBone;
	}
	
	engBone->SetShapes( shapes );
	engBone->SetShapeProperties( shapeProperties );
}
