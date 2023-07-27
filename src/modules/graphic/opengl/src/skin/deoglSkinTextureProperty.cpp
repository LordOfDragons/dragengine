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

#include "deoglSkinTextureProperty.h"
#include "dynamic/deoglRDynamicSkin.h"
#include "dynamic/renderables/render/deoglRDSRenderable.h"
#include "state/deoglSkinState.h"
#include "state/deoglSkinStateBone.h"
#include "state/deoglSkinStateRenderable.h"
#include "state/deoglSkinStateCalculated.h"

#include <dragengine/common/exceptions.h>



// Class deoglSkinTextureProperty
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinTextureProperty::deoglSkinTextureProperty() :
pRenderable( -1 ),
pCalculatedProperty( -1 ),
pBone( -1 ){
}

deoglSkinTextureProperty::~deoglSkinTextureProperty(){
}



// Management
///////////////

void deoglSkinTextureProperty::SetRenderable( int index ){
	DEASSERT_TRUE( index >= -1 )
	pRenderable = index;
}

void deoglSkinTextureProperty::SetCalculatedProperty( int index ){
	DEASSERT_TRUE( index >= -1 )
	pCalculatedProperty = index;
}

void deoglSkinTextureProperty::SetBone( int index ){
	DEASSERT_TRUE( index >= -1 )
	pBone = index;
}

bool deoglSkinTextureProperty::IsDynamic() const{
	return pRenderable != -1 || pCalculatedProperty != -1;
}



float deoglSkinTextureProperty::ResolveAsFloat( const deoglSkinState *skinState,
const deoglRDynamicSkin *dynamicSkin, float defaultValue ) const{
	if( ! skinState ){
		return defaultValue;
	}
	
	if( dynamicSkin && pRenderable >= 0 && pRenderable < skinState->GetRenderableCount() ){
		const deoglSkinStateRenderable &stateRenderable = *skinState->GetRenderableAt( pRenderable );
		if( stateRenderable.GetHostRenderable() != -1 ){
			return dynamicSkin->GetRenderableAt( stateRenderable.GetHostRenderable() )
				->GetRenderValue( defaultValue );
		}
	}
	
	if( pCalculatedProperty >= 0 && pCalculatedProperty < skinState->GetCalculatedPropertyCount() ){
		return skinState->GetCalculatedPropertyAt( pCalculatedProperty ).GetColor().r;
	}
	
	return defaultValue;
}

bool deoglSkinTextureProperty::ResolveAsBool( const deoglSkinState *skinState,
const deoglRDynamicSkin *dynamicSkin, bool defaultValue ) const{
	if( ! skinState ){
		return defaultValue;
	}
	
	if( dynamicSkin && pRenderable >= 0 && pRenderable < skinState->GetRenderableCount() ){
		const deoglSkinStateRenderable &stateRenderable = *skinState->GetRenderableAt( pRenderable );
		if( stateRenderable.GetHostRenderable() != -1 ){
			return dynamicSkin->GetRenderableAt( stateRenderable.GetHostRenderable() )
				->GetRenderValue( defaultValue ? 1.0f : 0.0f ) > 0.5f;
		}
	}
	
	if( pCalculatedProperty >= 0 && pCalculatedProperty < skinState->GetCalculatedPropertyCount() ){
		return skinState->GetCalculatedPropertyAt( pCalculatedProperty ).GetColor().r > 0.5f ? 1.0f : 0.0f;
	}
	
	return defaultValue;
}

decColor deoglSkinTextureProperty::ResolveColor( const deoglSkinState *skinState,
const deoglRDynamicSkin *dynamicSkin, const decColor &defaultValue ) const{
	if( ! skinState ){
		return defaultValue;
	}
	
	if( dynamicSkin && pRenderable >= 0 && pRenderable < skinState->GetRenderableCount() ){
		const deoglSkinStateRenderable &stateRenderable = *skinState->GetRenderableAt( pRenderable );
		if( stateRenderable.GetHostRenderable() != -1 ){
			return dynamicSkin->GetRenderableAt( stateRenderable.GetHostRenderable() )
				->GetRenderColor( defaultValue );
		}
	}
	
	if( pCalculatedProperty >= 0 && pCalculatedProperty < skinState->GetCalculatedPropertyCount() ){
		return skinState->GetCalculatedPropertyAt( pCalculatedProperty ).GetColor();
	}
	
	return defaultValue;
}

decVector2 deoglSkinTextureProperty::ResolveVector2( const deoglSkinState *skinState,
const deoglRDynamicSkin *dynamicSkin, const decVector2 &defaultValue ) const{
	if( ! skinState ){
		return defaultValue;
	}
	
	if( dynamicSkin && pRenderable >= 0 && pRenderable < skinState->GetRenderableCount() ){
		const deoglSkinStateRenderable &stateRenderable = *skinState->GetRenderableAt( pRenderable );
		if( stateRenderable.GetHostRenderable() != -1 ){
			const decColor color( dynamicSkin->GetRenderableAt( stateRenderable.GetHostRenderable() )
				->GetRenderColor( decColor( defaultValue.x, defaultValue.y, 0.0f ) ) );
			return decVector2( color.r, color.g );
		}
	}
	
	if( pCalculatedProperty >= 0 && pCalculatedProperty < skinState->GetCalculatedPropertyCount() ){
		const decColor &color = skinState->GetCalculatedPropertyAt( pCalculatedProperty ).GetColor();
		return decVector2( color.r, color.g );
	}
	
	return defaultValue;
}

decVector deoglSkinTextureProperty::ResolveVector( const deoglSkinState *skinState,
const deoglRDynamicSkin *dynamicSkin, const decVector &defaultValue ) const{
	if( ! skinState ){
		return defaultValue;
	}
	
	if( dynamicSkin && pRenderable >= 0 && pRenderable < skinState->GetRenderableCount() ){
		const deoglSkinStateRenderable &stateRenderable = *skinState->GetRenderableAt( pRenderable );
		if( stateRenderable.GetHostRenderable() != -1 ){
			const decColor color( dynamicSkin->GetRenderableAt( stateRenderable.GetHostRenderable() )
				->GetRenderColor( decColor( defaultValue.x, defaultValue.y, defaultValue.z ) ) );
			return decVector( color.r, color.g, color.b );
		}
	}
	
	if( pCalculatedProperty >= 0 && pCalculatedProperty < skinState->GetCalculatedPropertyCount() ){
		const decColor &color = skinState->GetCalculatedPropertyAt( pCalculatedProperty ).GetColor();
		return decVector( color.r, color.g, color.b );
	}
	
	return defaultValue;
}

decMatrix deoglSkinTextureProperty::ResolveMatrix( const deoglSkinState *skinState,
const decMatrix &defaultValue ) const{
	if( skinState && pBone >= 0 && pBone < skinState->GetBoneCount() ){
		return skinState->GetBoneAt( pRenderable ).GetBoneMatrix();
	}
	return defaultValue;
}
