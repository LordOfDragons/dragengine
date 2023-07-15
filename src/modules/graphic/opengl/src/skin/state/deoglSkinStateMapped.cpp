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

#include "deoglSkinStateMapped.h"
#include "../deoglSkinMapped.h"
#include "../../component/deoglRComponent.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/rig/deRig.h>



// Class deoglSkinStateMapped
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinStateMapped::deoglSkinStateMapped() :
pValue( 0.0f ),
pMapped( nullptr ),
pComponent( nullptr ),
pBone( -1 ),
pBoneInputValue( 0.0f ){
}

deoglSkinStateMapped::~deoglSkinStateMapped(){
}



// Management
///////////////

void deoglSkinStateMapped::SetValue( float value ){
	pValue = value;
}

void deoglSkinStateMapped::SetMapped( const deoglSkinMapped *mapped ){
	pMapped = mapped;
}

void deoglSkinStateMapped::SetComponent( const deoglRComponent *component ){
	pComponent = component;
}

void deoglSkinStateMapped::MapBone( const deComponent &component ){
	pBone = -1;
	
	if( pMapped && component.GetRig() ){
		const deRig &rig = *component.GetRig();
		const decString &name = pMapped->GetMapped().GetBone();
		if( ! name.IsEmpty() ){
			pBone = rig.IndexOfBoneNamed( name );
		}
	}
}

void deoglSkinStateMapped::UpdateBone( const deComponent &component ){
	const int boneCount = component.GetBoneCount();
	if( pBone < 0 || pBone >= boneCount ){
		return;
	}
	
	const deComponentBone &cbone = component.GetBoneAt( pBone );
	
	switch( pMapped->GetMapped().GetInputType() ){
	case deSkinMapped::eitBonePositionX:
		pBoneInputValue = cbone.GetPosition().x;
		break;
		
	case deSkinMapped::eitBonePositionY:
		pBoneInputValue = cbone.GetPosition().y;
		break;
		
	case deSkinMapped::eitBonePositionZ:
		pBoneInputValue = cbone.GetPosition().z;
		break;
		
	case deSkinMapped::eitBoneRotationX:
		pBoneInputValue = cbone.GetRotation().GetEulerAngles().x / TWO_PI;
		break;
		
	case deSkinMapped::eitBoneRotationY:
		pBoneInputValue = cbone.GetRotation().GetEulerAngles().y / TWO_PI;
		break;
		
	case deSkinMapped::eitBoneRotationZ:
		pBoneInputValue = cbone.GetRotation().GetEulerAngles().z / TWO_PI;
		break;
		
	case deSkinMapped::eitBoneScaleX:
		pBoneInputValue = cbone.GetScale().x;
		break;
		
	case deSkinMapped::eitBoneScaleY:
		pBoneInputValue = cbone.GetScale().y;
		break;
		
	case deSkinMapped::eitBoneScaleZ:
		pBoneInputValue = cbone.GetScale().z;
		break;
		
	default:
		break;
	}
}

void deoglSkinStateMapped::Update( deoglSkinState &skinState ){
	if( pMapped ){
		pValue = pMapped->Calculate( skinState, *this );
		
	}else{
		pValue = 0.0f;
	}
}
