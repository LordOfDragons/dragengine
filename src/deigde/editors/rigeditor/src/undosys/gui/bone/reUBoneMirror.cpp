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

#include <stdio.h>
#include <stdlib.h>

#include "reUBoneMirror.h"
#include "../../../rig/reRig.h"
#include "../../../rig/bone/reRigBone.h"
#include "../../../rig/bone/reSelectionBones.h"
#include "../../../rig/shape/reRigShape.h"
#include "../../../rig/shape/reRigShapeList.h"
#include "../../../rig/shape/reRigShapeSphere.h"
#include "../../../rig/shape/reRigShapeBox.h"
#include "../../../rig/shape/reRigShapeCylinder.h"
#include "../../../rig/shape/reRigShapeCapsule.h"
#include "../../../rig/constraint/reRigConstraint.h"
#include "../../../rig/constraint/reRigConstraintList.h"
#include "../../../rig/constraint/reRigConstraintDof.h"

#include <dragengine/common/exceptions.h>



// Class reUBoneMirror
////////////////////////

// Constructor, destructor
////////////////////////////

reUBoneMirror::reUBoneMirror( reRig *rig ){
	const reSelectionBones &selection = *rig->GetSelectionBones();
	int b, boneCount = selection.GetBoneCount();
	int c, constraintCount;
	reRigBone *boneTarget;
	int s, shapeCount;
	
	pRig = NULL;
	
	pBones = NULL;
	pBoneCount = 0;
	
	SetShortInfo( "Mirror Bones" );
	
	try{
		if( boneCount > 0 ){
			// create bones array
			pBones = new sBone[ boneCount ];
			if( ! pBones ) DETHROW( deeOutOfMemory );
			
			// clear all entries just setting the bone
			while( pBoneCount < boneCount ){
				sBone &ubone = pBones[ pBoneCount ];
				
				ubone.boneSource = selection.GetBoneAt( pBoneCount );
				ubone.boneSource->AddReference();
				
				ubone.boneTarget = NULL;
				ubone.oldMass = 0.0f;
				ubone.oldDynamic = false;
				ubone.oldShapes = NULL;
				ubone.oldConstraints = NULL;
				
				pBoneCount++;
			}
			
			// search for the mirrored bone and store the parameters if found
			for( b=0; b<pBoneCount; b++ ){
				sBone &ubone = pBones[ b ];
				
				boneTarget = pGetBoneWithMirroredName( rig, ubone.boneSource );
				
				if( boneTarget ){
					boneTarget->AddReference();
					ubone.boneTarget = boneTarget;
					
					ubone.oldCMP = boneTarget->GetCentralMassPoint();
					ubone.oldMass = boneTarget->GetMass();
					ubone.oldDynamic = boneTarget->GetDynamic();
					
					ubone.oldShapes = new reRigShapeList;
					if( ! ubone.oldShapes ) DETHROW( deeOutOfMemory );
					
					shapeCount = boneTarget->GetShapeCount();
					for( s=0; s<shapeCount; s++ ){
						ubone.oldShapes->AddShape( boneTarget->GetShapeAt( s ) );
					}
					
					ubone.oldConstraints = new reRigConstraintList;
					if( ! ubone.oldConstraints ) DETHROW( deeOutOfMemory );
					
					constraintCount = boneTarget->GetConstraintCount();
					for( c=0; c<constraintCount; c++ ){
						ubone.oldConstraints->AddConstraint( boneTarget->GetConstraintAt( c ) );
					}
				}
			}
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	
	pRig = rig;
	rig->AddReference();
}

reUBoneMirror::~reUBoneMirror(){
	pCleanUp();
}



// Undo and Redo operations
/////////////////////////////

void reUBoneMirror::Undo(){
	int c, constraintCount;
	reRigBone *boneTarget;
	int s, shapeCount;
	int b;
	
	for( b=0; b<pBoneCount; b++ ){
		sBone &ubone = pBones[ b ];
		boneTarget = ubone.boneTarget;
		
		if( boneTarget ){
			boneTarget->SetCentralMassPoint( ubone.oldCMP );
			boneTarget->SetMass( ubone.oldMass );
			boneTarget->SetDynamic( ubone.oldDynamic );
			
			boneTarget->RemoveAllShapes();
			const reRigShapeList &oldShapes = *ubone.oldShapes;
			shapeCount = oldShapes.GetShapeCount();
			for( s=0; s<shapeCount; s++ ){
				boneTarget->AddShape( oldShapes.GetShapeAt( s ) );
			}
			
			boneTarget->RemoveAllConstraints();
			const reRigConstraintList &oldConstraints = *ubone.oldConstraints;
			constraintCount = oldConstraints.GetConstraintCount();
			for( c=0; c<constraintCount; c++ ){
				boneTarget->AddConstraint( oldConstraints.GetConstraintAt( c ) );
			}
		}
	}
}

void reUBoneMirror::Redo(){
	reRigConstraint *constraint = NULL;
	reRigShape *shape = NULL;
	reRigBone *boneMirrored;
	decMatrix matrixMirror;
	decMatrix matrixResult;
	decMatrix matrixLimit;
	int c, constraintCount;
	reRigBone *boneTarget;
	reRigBone *boneSource;
	int s, shapeCount;
	float exchange;
	float lowerOrg;
	float upperOrg;
	float lowerNew;
	float upperNew;
	int b;
	
	try{
		for( b=0; b<pBoneCount; b++ ){
			sBone &ubone = pBones[ b ];
			boneSource = ubone.boneSource;
			boneTarget = ubone.boneTarget;
			
			if( boneTarget ){
				matrixMirror = boneSource->GetMatrix() * decMatrix::CreateScale( -1.0f, 1.0f, 1.0f ) * boneTarget->GetInverseMatrix();
				
				// mirror bone properties
				const decVector &cmp = boneSource->GetCentralMassPoint();
				boneTarget->SetCentralMassPoint( decVector( -cmp.x, cmp.y, cmp.z ) );
				boneTarget->SetMass( boneSource->GetMass() );
				boneTarget->SetDynamic( boneSource->GetDynamic() );
				
				// mirror shapes. this is not correct yet and has to be fixed. the main problem
				// is that using mirroring the angles are incorrect in some situations.
				boneTarget->RemoveAllShapes();
				shapeCount = boneSource->GetShapeCount();
				for( s=0; s<shapeCount; s++ ){
					shape = boneSource->GetShapeAt( s )->Duplicate();
					
					matrixResult = decMatrix::CreateRT( shape->GetOrientation() * DEG2RAD, shape->GetPosition() ) * matrixMirror;
					shape->SetPosition( matrixResult.GetPosition() );
					shape->SetOrientation( matrixResult.GetEulerAngles() / DEG2RAD );
					
					boneTarget->AddShape( shape );
					shape->FreeReference();
					shape = NULL;
				}
				
				// mirror constraints
				boneTarget->RemoveAllConstraints();
				constraintCount = boneSource->GetConstraintCount();
				for( c=0; c<constraintCount; c++ ){
					constraint = boneSource->GetConstraintAt( c )->Duplicate();
					
					// constraint position and orientation
					matrixResult = decMatrix::CreateRT( constraint->GetOrientation() * DEG2RAD, constraint->GetPosition() ) * matrixMirror;
					constraint->SetPosition( matrixResult.GetPosition() );
					constraint->SetOrientation( matrixResult.GetEulerAngles() / DEG2RAD );
					
					// matrix to mirror limits
					matrixResult *= decMatrix::CreateRT( constraint->GetOrientation() * DEG2RAD, constraint->GetPosition() ).Invert();
					
					// dof linear x
					lowerOrg = constraint->GetDofLinearX().GetLowerLimit();
					upperOrg = constraint->GetDofLinearX().GetUpperLimit();
					
					if( upperOrg - lowerOrg < -1e-5f ){
						lowerNew = lowerOrg;
						upperNew = lowerOrg;
						
					}else{
						lowerNew = ( matrixResult * decVector( lowerOrg, 0.0f, 0.0f ) ).x;
						upperNew = ( matrixResult * decVector( upperOrg, 0.0f, 0.0f ) ).x;
						
						if( upperNew < lowerNew ){
							exchange = lowerNew;
							lowerNew = upperNew;
							upperNew = exchange;
						}
					}
					
					constraint->GetDofLinearX().SetLowerLimit( lowerNew );
					constraint->GetDofLinearX().SetUpperLimit( upperNew );
					
					// dof linear y
					lowerOrg = constraint->GetDofLinearY().GetLowerLimit();
					upperOrg = constraint->GetDofLinearY().GetUpperLimit();
					
					if( upperOrg - lowerOrg < -1e-5f ){
						lowerNew = lowerOrg;
						upperNew = lowerOrg;
						
					}else{
						lowerNew = ( matrixResult * decVector( 0.0f, lowerOrg, 0.0f ) ).x;
						upperNew = ( matrixResult * decVector( 0.0f, upperOrg, 0.0f ) ).x;
						
						if( upperNew < lowerNew ){
							exchange = lowerNew;
							lowerNew = upperNew;
							upperNew = exchange;
						}
					}
					
					constraint->GetDofLinearY().SetLowerLimit( lowerNew );
					constraint->GetDofLinearY().SetUpperLimit( upperNew );
					
					// dof linear z
					lowerOrg = constraint->GetDofLinearZ().GetLowerLimit();
					upperOrg = constraint->GetDofLinearZ().GetUpperLimit();
					
					if( upperOrg - lowerOrg < -1e-5f ){
						lowerNew = lowerOrg;
						upperNew = lowerOrg;
						
					}else{
						lowerNew = ( matrixResult * decVector( 0.0f, 0.0f, lowerOrg ) ).x;
						upperNew = ( matrixResult * decVector( 0.0f, 0.0f, upperOrg ) ).x;
						
						if( upperNew < lowerNew ){
							exchange = lowerNew;
							lowerNew = upperNew;
							upperNew = exchange;
						}
					}
					
					constraint->GetDofLinearZ().SetLowerLimit( lowerNew );
					constraint->GetDofLinearZ().SetUpperLimit( upperNew );
					
					// dof angular x
					lowerOrg = constraint->GetDofAngularX().GetLowerLimit();
					upperOrg = constraint->GetDofAngularX().GetUpperLimit();
					
					if( upperOrg - lowerOrg < -1e-5f ){
						lowerNew = lowerOrg;
						upperNew = lowerOrg;
						
					}else{
						lowerNew = ( decMatrix::CreateRotationX( lowerOrg * DEG2RAD ) * matrixResult ).GetEulerAngles().x / DEG2RAD;
						upperNew = ( decMatrix::CreateRotationX( upperOrg * DEG2RAD ) * matrixResult ).GetEulerAngles().x / DEG2RAD;
						
						if( upperNew < lowerNew ){
							exchange = lowerNew;
							lowerNew = upperNew;
							upperNew = exchange;
						}
					}
					
					constraint->GetDofAngularX().SetLowerLimit( lowerNew );
					constraint->GetDofAngularX().SetUpperLimit( upperNew );
					
					// dof angular y
					lowerOrg = constraint->GetDofAngularY().GetLowerLimit();
					upperOrg = constraint->GetDofAngularY().GetUpperLimit();
					
					if( upperOrg - lowerOrg < -1e-5f ){
						lowerNew = lowerOrg;
						upperNew = lowerOrg;
						
					}else{
						lowerNew = ( decMatrix::CreateRotationY( lowerOrg * DEG2RAD ) * matrixResult ).GetEulerAngles().y / DEG2RAD;
						upperNew = ( decMatrix::CreateRotationY( upperOrg * DEG2RAD ) * matrixResult ).GetEulerAngles().y / DEG2RAD;
						
						if( upperNew < lowerNew ){
							exchange = lowerNew;
							lowerNew = upperNew;
							upperNew = exchange;
						}
					}
					
					constraint->GetDofAngularY().SetLowerLimit( lowerNew );
					constraint->GetDofAngularY().SetUpperLimit( upperNew );
					
					// dof angular z
					lowerOrg = constraint->GetDofAngularZ().GetLowerLimit();
					upperOrg = constraint->GetDofAngularZ().GetUpperLimit();
					
					if( upperOrg - lowerOrg < -1e-5f ){
						lowerNew = lowerOrg;
						upperNew = lowerOrg;
						
					}else{
						lowerNew = ( decMatrix::CreateRotationZ( lowerOrg * DEG2RAD ) * matrixResult ).GetEulerAngles().z / DEG2RAD;
						upperNew = ( decMatrix::CreateRotationZ( upperOrg * DEG2RAD ) * matrixResult ).GetEulerAngles().z / DEG2RAD;
						
						if( upperNew < lowerNew ){
							exchange = lowerNew;
							lowerNew = upperNew;
							upperNew = exchange;
						}
					}
					
					constraint->GetDofAngularZ().SetLowerLimit( lowerNew );
					constraint->GetDofAngularZ().SetUpperLimit( upperNew );
					
					// mirror constraint bone
					boneMirrored = pGetBoneWithMirroredName( pRig, constraint->GetConstraintBone() );
					if( boneMirrored ){
						constraint->SetConstraintBone( boneMirrored );
					}
					
					// add the mirrored constraint
					boneTarget->AddConstraint( constraint );
					constraint->FreeReference();
					constraint = NULL;
				}
			}
		}
		
	}catch( const deException & ){
		if( shape ) shape->FreeReference();
		if( constraint ) constraint->FreeReference();
		throw;
	}
}



// Private Functions
//////////////////////

void reUBoneMirror::pCleanUp(){
	if( pBones ){
		while( pBoneCount > 0 ){
			pBoneCount--;
			sBone &ubone = pBones[ pBoneCount ];
			
			if( ubone.oldConstraints ) delete ubone.oldConstraints;
			if( ubone.oldShapes ) delete ubone.oldShapes;
			if( ubone.boneTarget ) ubone.boneTarget->FreeReference();
			if( ubone.boneSource ) ubone.boneSource->FreeReference();
		}
		
		delete [] pBones;
	}
	
	if( pRig ) pRig->FreeReference();
}



reRigBone *reUBoneMirror::pGetBoneWithMirroredName( reRig *rig, reRigBone *bone ) const{
	const decString &nameBone = bone->GetName();
	reRigBone *mirroredBone = NULL;
	decString nameMirrored;
	
	if( nameBone.MatchesPattern( "*.[l,L]" ) ){
		nameMirrored = nameBone;
		
		nameMirrored.SetAt( nameBone.GetLength() - 1, 'r' );
		mirroredBone = rig->GetBoneNamed( nameMirrored.GetString() );
		
		if( ! mirroredBone ){
			nameMirrored.SetAt( nameBone.GetLength() - 1, 'R' );
			mirroredBone = rig->GetBoneNamed( nameMirrored.GetString() );
		}
		
	}else if( nameBone.MatchesPattern( "*.[r,R]" ) ){
		nameMirrored = nameBone;
		
		nameMirrored.SetAt( nameBone.GetLength() - 1, 'l' );
		mirroredBone = rig->GetBoneNamed( nameMirrored.GetString() );
		
		if( ! mirroredBone ){
			nameMirrored.SetAt( nameBone.GetLength() - 1, 'L' );
			mirroredBone = rig->GetBoneNamed( nameMirrored.GetString() );
		}
	}
	
	return mirroredBone;
}
