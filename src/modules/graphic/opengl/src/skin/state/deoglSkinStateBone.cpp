/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "deoglSkinStateBone.h"
#include "../deoglSkinBone.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/rig/deRig.h>



// Class deoglSkinStateBone
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinStateBone::deoglSkinStateBone() :
pSkinBone( nullptr ),
pBoneIndex( -1 ){
}

deoglSkinStateBone::~deoglSkinStateBone(){
}



// Management
///////////////

void deoglSkinStateBone::SetSkinBone( const deoglSkinBone *skinBone ){
	pSkinBone = skinBone;
}

void deoglSkinStateBone::MapBone( const deComponent &component ){
	pBoneIndex = -1;
	
	if( ! pSkinBone || ! component.GetRig() ){
		return;
	}
	
	const deRig &rig = *component.GetRig();
	const decString &name = pSkinBone->GetName();
	if( name.IsEmpty() ){
		return;
	}
	
	pBoneIndex = rig.IndexOfBoneNamed( name );
}

void deoglSkinStateBone::UpdateBone( const deComponent &component ){
	const int boneCount = component.GetBoneCount();
	if( pBoneIndex < 0 || pBoneIndex >= boneCount ){
		return;
	}
	
	pBoneMatrix = component.GetBoneAt( pBoneIndex ).GetMatrix();
}
