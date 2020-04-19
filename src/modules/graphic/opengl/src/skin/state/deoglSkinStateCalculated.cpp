/* 
 * Drag[en]gine OpenGL Graphic Module
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
#include <string.h>

#include "deoglSkinStateCalculated.h"
#include "../deoglSkinCalculatedProperty.h"
#include "../../component/deoglRComponent.h"
#include "../../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/rig/deRig.h>



// Class deoglSkinStateCalculated
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinStateCalculated::deoglSkinStateCalculated() :
pTexture( NULL ),
pProperty( NULL ),
pComponent( NULL )
{
	int i;
	for( i=0; i<4; i++ ){
		pBone[ i ] = -1;
		pBoneInputValue[ i ] = 0.0f;
	}
}

deoglSkinStateCalculated::~deoglSkinStateCalculated(){
	if( pTexture ){
		delete pTexture;
	}
}



// Management
///////////////

void deoglSkinStateCalculated::SetColor( const decColor &color ){
	pColor = color;
}

void deoglSkinStateCalculated::SetTexture( deoglTexture* texture ) {
	if( texture == pTexture ){
		return;
	}
	
	if( pTexture ){
		delete pTexture;
	}
	pTexture = texture;
}

void deoglSkinStateCalculated::SetProperty( deoglSkinCalculatedProperty *property ){
	pProperty = property;
}

void deoglSkinStateCalculated::SetComponent( const deoglRComponent *component ){
	pComponent = component;
}

int deoglSkinStateCalculated::GetBone( int component ) const{
	if( component < 0 || component > 3 ){
		DETHROW( deeInvalidParam );
	}
	return pBone[ component ];
}

float deoglSkinStateCalculated::GetBoneInputValue( int component ) const{
	if( component < 0 || component > 3 ){
		DETHROW( deeInvalidParam );
	}
	return pBoneInputValue[ component ];
}

void deoglSkinStateCalculated::MapBones( const deComponent &component ){
	int i;
	for( i=0; i<4; i++ ){
		pBone[ i ] = -1;
	}
	
	if( pProperty && component.GetRig() ){
		const deRig &rig = *component.GetRig();
		for( i=0; i<4; i++ ){
			const decString &name = pProperty->GetMappedComponent( i ).GetBone();
			if( ! name.IsEmpty() ){
				pBone[ i ] = rig.IndexOfBoneNamed( name );
			}
		}
	}
}

void deoglSkinStateCalculated::UpdateBones( const deComponent &component ){
	const int boneCount = component.GetBoneCount();
	int i;
	
	for( i=0; i<4; i++ ){
		if( pBone[ i ] < 0 || pBone[ i ] >= boneCount ){
			continue;
		}
		
		const deComponentBone &cbone = component.GetBoneAt( pBone[ i ] );
		
		switch( pProperty->GetMappedComponent( i ).GetInputType() ){
		case deSkinPropertyMapped::eitBonePositionX:
			pBoneInputValue[ i ] = cbone.GetPosition().x;
			break;
			
		case deSkinPropertyMapped::eitBonePositionY:
			pBoneInputValue[ i ] = cbone.GetPosition().y;
			break;
			
		case deSkinPropertyMapped::eitBonePositionZ:
			pBoneInputValue[ i ] = cbone.GetPosition().z;
			break;
			
		case deSkinPropertyMapped::eitBoneRotationX:
			pBoneInputValue[ i ] = cbone.GetRotation().GetEulerAngles().x / TWO_PI;
			break;
			
		case deSkinPropertyMapped::eitBoneRotationY:
			pBoneInputValue[ i ] = cbone.GetRotation().GetEulerAngles().y / TWO_PI;
			break;
			
		case deSkinPropertyMapped::eitBoneRotationZ:
			pBoneInputValue[ i ] = cbone.GetRotation().GetEulerAngles().z / TWO_PI;
			break;
			
		case deSkinPropertyMapped::eitBoneScaleX:
			pBoneInputValue[ i ] = cbone.GetScale().x;
			break;
			
		case deSkinPropertyMapped::eitBoneScaleY:
			pBoneInputValue[ i ] = cbone.GetScale().y;
			break;
			
		case deSkinPropertyMapped::eitBoneScaleZ:
			pBoneInputValue[ i ] = cbone.GetScale().z;
			break;
			
		default:
			break;
		}
	}
}

void deoglSkinStateCalculated::Update( deoglSkinState &skinState ){
	if( pProperty ){
		pColor = pProperty->Calculate( skinState, *this );
		
	}else{
		pColor.Set( 0.0f, 0.0f, 0.0f, 1.0f );
	}
// 	printf("SkinStateCalculated.Update: property=%p color=(%g,%g,%g,%g)\n",
// 		pProperty, pColor.r, pColor.g, pColor.b, pColor.a);
}
