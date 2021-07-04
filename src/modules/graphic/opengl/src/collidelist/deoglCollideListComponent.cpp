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
#include <string.h>

#include "deoglCollideListComponent.h"
#include "../component/deoglRComponent.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../occlusiontest/deoglOcclusionTest.h"
#include "../utils/deoglCubeHelper.h"

#include <dragengine/common/exceptions.h>



// Class deoglCollideListComponent
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCollideListComponent::deoglCollideListComponent() :
pComponent( NULL ),
pLODLevel( 0 ),
pComponentLOD( NULL ),
pCulled( false ),
pCascadeMask( 0 ),
pCubeFaceMask( 0x2f ),
pSpecialFlags( 0 ){
}

deoglCollideListComponent::~deoglCollideListComponent(){
}



// Management
///////////////

void deoglCollideListComponent::Clear(){
	pComponent = NULL;
	pLODLevel = 0;
	pComponentLOD = NULL;
	pCulled = false;
	pCascadeMask = 0;
	pCubeFaceMask = 0x2f;
	pSpecialFlags = 0;
}

void deoglCollideListComponent::SetComponent( deoglRComponent *component ){
	pComponent = component;
	pLODLevel = 0;
	pComponentLOD = &pComponent->GetLODAt( pLODLevel );
}

void deoglCollideListComponent::SetLODLevel( int lodLevel ){
	pLODLevel = lodLevel;
	pComponentLOD = &pComponent->GetLODAt( pLODLevel );
}

void deoglCollideListComponent::SetLODLevelMax(){
	pLODLevel = pComponent->GetLODCount() - 1;
	pComponentLOD = &pComponent->GetLODAt( pLODLevel );
}

void deoglCollideListComponent::SetCulled( bool culled ){
	pCulled = culled;
}

void deoglCollideListComponent::SetCascadeMask( int mask ){
	pCascadeMask = mask;
}

void deoglCollideListComponent::UpdateCubeFaceMask( const decDVector &cubePosition ){
	pCubeFaceMask = deoglCubeHelper::CalcFaceVisibility(
		pComponent->GetMinimumExtend() - cubePosition,
		pComponent->GetMaximumExtend() - cubePosition );
	
	// DEBUG
	/*
	const deoglDMatrix &m = pComponent.GetMatrix();
	pRenderThread.GetLogger().LogInfoFormat( "DEBUG: (%g,%g,%g) [%d, %d, %d, %d, %d, %d] {(%g,%g,%g), (%g,%g,%g)}",
		m.GetPosition().x, m.GetPosition().y, m.GetPosition().z, ( pCubeFaceMask & 0x1 ) == 0x1,
		( pCubeFaceMask & 0x2 ) == 0x2, ( pCubeFaceMask & 0x4 ) == 0x4, ( pCubeFaceMask & 0x8 ) == 0x8,
		( pCubeFaceMask & 0x10 ) == 0x10, ( pCubeFaceMask & 0x20 ) == 0x20,
		( pComponent->GetMinimumExtend() - cubePosition ).x, ( pComponent->GetMinimumExtend() - cubePosition ).y,
		( pComponent->GetMinimumExtend() - cubePosition ).z, ( pComponent->GetMaximumExtend() - cubePosition ).x,
		( pComponent->GetMaximumExtend() - cubePosition ).y, ( pComponent->GetMaximumExtend() - cubePosition ).z );
	*/
}

bool deoglCollideListComponent::GetCubeFaceMaskAt( int face ) const{
	if( face < 0 || face > 5 ){
		DETHROW( deeInvalidParam );
	}
	return ( pCubeFaceMask & ( 1 << face ) ) != 0;
}

void deoglCollideListComponent::SetSpecialFlags( int flags ){
	pSpecialFlags = flags;
}

void deoglCollideListComponent::StartOcclusionTest( deoglOcclusionTest &occlusionTest,
const decDVector &referencePosition ){
	// this check is wrong. components can have no skin assigned but having skins assigned
	// to individual textures. this check should instead check all textures if they have
	// a UseTexture which is not NULL. doing this though over and over again is bad.
	// requires adding a flag to check if the component has at least one visible texture
// 	if( ! pComponent->GetModel() || ! pComponent->GetSkin() ){
	if( ! pComponent->GetModel() ){
		OcclusionTestInvisible();
		return;
	}
	
	pCulled = false;
	occlusionTest.AddInputData(
		( pComponent->GetMinimumExtend() - referencePosition ).ToVector(),
		( pComponent->GetMaximumExtend() - referencePosition ).ToVector(), this );
}

void deoglCollideListComponent::OcclusionTestInvisible(){
	pCulled = true;
}
