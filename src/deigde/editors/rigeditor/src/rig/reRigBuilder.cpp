/* 
 * Drag[en]gine IGDE Rig Editor
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
		
	}catch( const deException &e ){
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
		
	}catch( const deException &e ){
		if( engConstraint ) delete engConstraint;
		if( shape ) delete shape;
		if( engBone ) delete engBone;
	}
	
	engBone->SetShapes( shapes );
	engBone->SetShapeProperties( shapeProperties );
}
