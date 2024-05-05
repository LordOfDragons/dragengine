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

#include "reCLSelect.h"
#include "reCLHitListEntry.h"
#include "../rig/reRig.h"
#include "../rig/bone/reRigBone.h"
#include "../rig/bone/reSelectionBones.h"
#include "../rig/push/reRigPush.h"
#include "../rig/push/reSelectionPushes.h"
#include "../rig/shape/reRigShape.h"
#include "../rig/shape/reSelectionShapes.h"
#include "../rig/constraint/reRigConstraint.h"
#include "../rig/constraint/reSelectionConstraints.h"
#include "../rigeditor.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderVisitorIdentify.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/common/exceptions.h>



// Class reCLSelect
/////////////////////

// Constructor, destructor
////////////////////////////

reCLSelect::reCLSelect( reRig *rig ){
	if( ! rig ){
		DETHROW( deeInvalidParam );
	}
	
	pRig = rig;
	
	pToggleSelection = false;
	pCanSelectBones = false;
	pCanSelectShapes = false;
	pCanSelectConstraints = false;
	pCanSelectPushes = false;
}

reCLSelect::~reCLSelect(){
}



// Management
///////////////

void reCLSelect::SetToggleSelection( bool toggle ){
	pToggleSelection = toggle;
}

void reCLSelect::SetCanSelectBones( bool canSelect ){
	pCanSelectBones = canSelect;
}

void reCLSelect::SetCanSelectShapes( bool canSelect ){
	pCanSelectShapes = canSelect;
}

void reCLSelect::SetCanSelectConstraints( bool canSelect ){
	pCanSelectConstraints = canSelect;
}

void reCLSelect::SetCanSelectPushes( bool canSelect ){
	pCanSelectPushes = canSelect;
}

void reCLSelect::Reset(){
	pHitList.RemoveAllEntries();
}

void reCLSelect::RunAction(){
	pHitList.SortByDistance();
	
	if( pHitList.GetEntryCount() > 0 ){
		reCLHitListEntry *entry = pHitList.GetEntryAt( 0 );
		reSelectionConstraints *selectionConstraints = pRig->GetSelectionConstraints();
		reSelectionShapes *selectionShapes = pRig->GetSelectionShapes();
		reSelectionPushes *selectionPushes = pRig->GetSelectionPushes();
		reSelectionBones *selectionBones = pRig->GetSelectionBones();
		reRigConstraint *constraint = entry->GetConstraint();
		reRigShape *shape = entry->GetShape();
		reRigBone *bone = entry->GetBone();
		reRigPush *push = entry->GetPush();
		
		// toggle selection state of the hit element
		if( pToggleSelection ){
			if( constraint ){
				if( constraint->GetSelected() ){
					selectionConstraints->RemoveConstraint( constraint );
					
				}else{
					selectionConstraints->AddConstraint( constraint );
				}
				
			}else if( push ){
				if( push->GetSelected() ){
					selectionPushes->RemovePush( push );
					
				}else{
					selectionPushes->AddPush( push );
				}
				
			}else if( shape ){
				if( shape->GetSelected() ){
					selectionShapes->RemoveShape( shape );
					
				}else{
					selectionShapes->AddShape( shape );
				}
				
			}else if( bone ){
				if( bone->GetSelected() ){
					selectionBones->RemoveBone( bone );
					
				}else{
					selectionBones->AddBone( bone );
				}
			}
			
		// switch to the hit element
		}else{
			if( constraint ){
				selectionConstraints->RemoveAllConstraints();
				selectionConstraints->AddConstraint( constraint );
				
			}else if( push ){
				selectionPushes->RemoveAllPushes();
				selectionPushes->AddPush( push );
				
			}else if( shape ){
				selectionShapes->RemoveAllShapes();
				selectionShapes->AddShape( shape );
				
			}else if( bone ){
				selectionBones->RemoveAllBones();
				selectionBones->AddBone( bone );
			}
		}
	}
}



// Notifications
//////////////////

void reCLSelect::CollisionResponse( deCollider *owner, deCollisionInfo *info ){
	reCLHitListEntry *entry = NULL;
	
	if( info->IsCollider() ){
		deCollider *collider = info->GetCollider();
		reRigConstraint *constraint = NULL;
		reRigShape *shape = NULL;
		reRigBone *bone = NULL;
		reRigPush *push = NULL;
		deColliderVisitorIdentify identify;
		
		// add hit object
		collider->Visit( identify );
		if( identify.IsVolume() ){
			deColliderVolume *colliderVolume = &identify.CastToVolume();
			
			//if( pCanSelectBones ){
				bone = pGetBoneFromCollider( colliderVolume );
				
				if( bone ){
					try{
						entry = new reCLHitListEntry;
						if( ! entry ) DETHROW( deeOutOfMemory );
						
						entry->SetBone( bone );
						entry->SetDistance( info->GetDistance() );
						entry->SetNormal( info->GetNormal() );
						
						pHitList.AddEntry( entry );
						entry = NULL;
						
					}catch( const deException & ){
						if( entry ) delete entry;
						throw;
					}
				}
			//}
			
			//if( pCanSelectShapes ){
				shape = pGetShapeFromCollider( colliderVolume );
				
				if( shape ){
					try{
						entry = new reCLHitListEntry;
						if( ! entry ) DETHROW( deeOutOfMemory );
						
						entry->SetShape( shape );
						entry->SetDistance( info->GetDistance() );
						entry->SetNormal( info->GetNormal() );
						
						pHitList.AddEntry( entry );
						entry = NULL;
						
					}catch( const deException & ){
						if( entry ) delete entry;
						throw;
					}
				}
			//}
			
			//if( pCanSelectPushes ){
				push = pGetPushFromCollider( colliderVolume );
				
				if( push ){
					try{
						entry = new reCLHitListEntry;
						if( ! entry ) DETHROW( deeOutOfMemory );
						entry->SetPush( push );
						entry->SetDistance( info->GetDistance() );
						entry->SetNormal( info->GetNormal() );
						
						pHitList.AddEntry( entry );
						entry = NULL;
						
					}catch( const deException & ){
						if( entry ) delete entry;
						throw;
					}
				}
			//}
			
			//if( pCanSelectConstraints ){
				constraint = pGetConstraintFromCollider( colliderVolume );
				
				if( constraint ){
					try{
						entry = new reCLHitListEntry;
						if( ! entry ) DETHROW( deeOutOfMemory );
						entry->SetConstraint( constraint );
						entry->SetDistance( info->GetDistance() );
						entry->SetNormal( info->GetNormal() );
						
						pHitList.AddEntry( entry );
						entry = NULL;
						
					}catch( const deException & ){
						if( entry ) delete entry;
						throw;
					}
				}
			//}
		}
	}
}

bool reCLSelect::CanHitCollider( deCollider *owner, deCollider *collider ){
	deColliderVisitorIdentify identify;
	
	collider->Visit( identify );
	if( identify.IsVolume() ){
		deColliderVolume *colliderVolume = &identify.CastToVolume();
		
		if( pCanSelectBones && pGetBoneFromCollider( colliderVolume ) ){
			return true;
		}
		
		if( pCanSelectShapes && pGetShapeFromCollider( colliderVolume ) ){
			return true;
		}
		
		if( pCanSelectConstraints && pGetConstraintFromCollider( colliderVolume ) ){
			return true;
		}
		
		if( pCanSelectPushes && pGetPushFromCollider( colliderVolume ) ){
			return true;
		}
	}
	
	return false;
}

void reCLSelect::ColliderChanged( deCollider *owner ){
}



// Private Functions
//////////////////////

reRigBone *reCLSelect::pGetBoneFromCollider( deColliderVolume *collider ) const{
	return pRig->GetBoneWith( collider );
}

reRigShape *reCLSelect::pGetShapeFromCollider( deColliderVolume *collider ) const{
	int b, boneCount = pRig->GetBoneCount();
	reRigShape *shape = NULL;
	
	// check if a rig shape belongs to this collider
	shape = pRig->GetShapeWith( collider );
	if( shape ) return shape;
	
	// check if a bone shape belongs to this collider
	for( b=0; b<boneCount; b++ ){
		shape = pRig->GetBoneAt( b )->GetShapeWith( collider );
		if( shape ) return shape;
	}
	
	// otherwise no shape belongs to this collider
	return NULL;
}

reRigConstraint *reCLSelect::pGetConstraintFromCollider( deColliderVolume *collider ) const{
	int b, boneCount = pRig->GetBoneCount();
	reRigConstraint *constraint = NULL;
	
	// check if a rig constraint belongs to this collider
	constraint = pRig->GetConstraintWith( collider );
	if( constraint ) return constraint;
	
	// check if a bone constraint belongs to this collider
	for( b=0; b<boneCount; b++ ){
		constraint = pRig->GetBoneAt( b )->GetConstraintWith( collider );
		if( constraint ) return constraint;
	}
	
	// otherwise no constraint belongs to this collider
	return NULL;
}

reRigPush *reCLSelect::pGetPushFromCollider( deColliderVolume *collider ) const{
	return pRig->GetPushWith( collider );
}
