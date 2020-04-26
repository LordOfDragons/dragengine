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

#include "reUBoneMassFromVolume.h"
#include "../../../rig/reRig.h"
#include "../../../rig/bone/reRigBone.h"
#include "../../../rig/bone/reSelectionBones.h"
#include "../../../rig/shape/reRigShape.h"
#include "../../../rig/shape/reRigShapeSphere.h"
#include "../../../rig/shape/reRigShapeBox.h"
#include "../../../rig/shape/reRigShapeCylinder.h"
#include "../../../rig/shape/reRigShapeCapsule.h"
#include "../../../rig/shape/reRigShapeHull.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>



// Class reUBoneMassFromVolume
////////////////////////////////

// Constructor, destructor
////////////////////////////

reUBoneMassFromVolume::reUBoneMassFromVolume( reRig *rig, const decObjectOrderedSet &bones, float density ){
	if( ! rig || bones.GetCount() == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( density < 0.0f ){
		density = 0.0f;
	}
	
	const int boneCount = bones.GetCount();
	int i;
	
	pRig = NULL;
	
	pBones = NULL;
	pBoneCount = 0;
	
	SetShortInfo( "Bone mass from volume" );
	
	try{
		pBones = new sBone[ boneCount ];
		
		for( i=0; i<boneCount; i++ ){
			pBones[ i ].bone = ( reRigBone* )bones.GetAt( i );
			pBones[ i ].bone->AddReference();
			pBones[ i ].oldMass = pBones[ i ].bone->GetMass();
			pBones[ i ].newMass = pCalcVolume( *pBones[ i ].bone ) * density;
		}
		
		pBoneCount = boneCount;
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	
	pRig = rig;
	rig->AddReference();
}

reUBoneMassFromVolume::~reUBoneMassFromVolume(){
	pCleanUp();
}



// Undo and Redo operations
/////////////////////////////

void reUBoneMassFromVolume::Undo(){
	int i;
	
	for( i=0; i<pBoneCount; i++ ){
		if( pBones[ i ].bone ){
			pBones[ i ].bone->SetMass( pBones[ i ].oldMass );
		}
	}
}

void reUBoneMassFromVolume::Redo(){
	int i;
	
	for( i=0; i<pBoneCount; i++ ){
		if( pBones[ i ].bone ){
			pBones[ i ].bone->SetMass( pBones[ i ].newMass );
		}
	}
}



// Private Functions
//////////////////////

void reUBoneMassFromVolume::pCleanUp(){
	if( pBones ){
		while( pBoneCount > 0 ){
			pBoneCount--;
			if( pBones[ pBoneCount ].bone ){
				pBones[ pBoneCount ].bone->FreeReference();
			}
		}
		
		delete [] pBones;
	}
	
	if( pRig ){
		pRig->FreeReference();
	}
}

float reUBoneMassFromVolume::pCalcVolume( const reRigBone &bone ) const{
	const int count = bone.GetShapeCount();
	float volume = 0.0f;
	int i;
	
	for( i=0; i<count; i++ ){
		const reRigShape &shape = *bone.GetShapeAt( i );
		const reRigShape::eShapeTypes type = shape.GetShapeType();
		
		switch( type ){
		case reRigShape::estSphere:{
			const reRigShapeSphere &sphere = ( reRigShapeSphere& )shape;
			const float radius = sphere.GetRadius();
			volume += PI * radius * radius * radius * 4.0f / 3.0f;
			}break;
			
		case reRigShape::estBox:{
			const reRigShapeBox &box = *( ( reRigShapeBox* )&shape );
			const decVector extends = box.GetHalfExtends() * 2.0f;
			volume += extends.x * extends.y * extends.z;
			}break;
			
		case reRigShape::estCylinder:{
			const reRigShapeCylinder &cylinder = ( reRigShapeCylinder& )shape;
			const float height = cylinder.GetHalfHeight() * 2.0f;
			const float radiusBottom = cylinder.GetBottomRadius();
			const float radiusTop = cylinder.GetTopRadius();
			const float radius = ( radiusTop + radiusBottom ) * 0.5f;
			volume += PI * radius * radius * height;
			}break;
			
		case reRigShape::estCapsule:{
			// NOTE this solution is not fully correct. the spheres at the top and bottom are
			//      calculate as if the top and bottom radius are equal. correctly though due
			//      to the different radi the spheres are slightly moved in or out of the
			//      cylinder. this can be achieved by calculating the tilt angle of the base
			//      cylinder and using this as the angle where the sphere is truely cut.
			const reRigShapeCapsule &capsule = ( reRigShapeCapsule& )shape;
			const float height = capsule.GetHalfHeight() * 2.0f;
			const float radiusBottom = capsule.GetBottomRadius();
			const float radiusTop = capsule.GetTopRadius();
			const float radius = ( radiusTop + radiusBottom ) * 0.5f;
			volume += PI * radius * radius * height; // base cylinder
			volume += PI * radiusTop * radiusTop * radiusTop * 2.0f / 3.0f; // top half-sphere
			volume += PI * radiusBottom * radiusBottom * radiusBottom * 2.0f / 3.0f; // bottom half-sphere
			}break;
			
		case reRigShape::estHull:{
			// this calculation is a crude approximation for the time being
			const reRigShapeHull &hull = ( reRigShapeHull& )shape;
			const int count = hull.GetPointCount();
			decVector minExtend, maxExtend;
			int i;
			
			for( i=0; i<count; i++ ){
				const decVector &point = hull.GetPointAt( i );
				if( i == 0 ){
					minExtend = maxExtend = point;
					
				}else{
					minExtend.SetSmallest( point );
					maxExtend.SetLargest( point );
				}
			}
			
			const decVector extends( maxExtend - minExtend );
			volume += extends.x * extends.y * extends.z;
			}break;
			
		default:
			break;
		}
	}
	
	return volume;
}
