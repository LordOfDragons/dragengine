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

#include "deoglRLumimeter.h"

#include "../light/deoglRLight.h"
#include "../renderthread/deoglRenderThread.h"
#include "../world/deoglRWorld.h"
#include "../world/deoglWorldOctree.h"

#include <dragengine/common/exceptions.h>



// Class deoglRLumimeter
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglRLumimeter::deoglRLumimeter( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

pParentWorld( NULL ),
pOctreeNode( NULL ),
pDirtyOctree( true ),

pLuminance( 0.0f ),
pDirtyMeasurements( true ),

pWorldMarkedRemove( false ){
	LEAK_CHECK_CREATE( renderThread, Lumimeter );
}

deoglRLumimeter::~deoglRLumimeter(){
	LEAK_CHECK_FREE( pRenderThread, Lumimeter );
	RemoveAllLights();
}



// Management
///////////////

void deoglRLumimeter::MarkDirty(){
	pDirtyMeasurements = true;
}



void deoglRLumimeter::SetParentWorld( deoglRWorld *parentWorld ){
	if( parentWorld == pParentWorld ){
		return;
	}
	
	pParentWorld = parentWorld;
	
	if( pOctreeNode ){
		pOctreeNode->RemoveLumimeter( this );
		pOctreeNode = NULL;
	}
}

void deoglRLumimeter::SetOctreeNode( deoglWorldOctree *octreeNode ){
	pOctreeNode = octreeNode;
}

void deoglRLumimeter::UpdateOctreeNode(){
	if( pParentWorld ){
		pParentWorld->GetOctree().InsertLumimeterIntoTree( this );
	}
}



void deoglRLumimeter::SetPosition( const decDVector &position ){
	pPosition = position;
}



void deoglRLumimeter::UpdateMeasurements(){
	// find all potential lights. this is not the fastest way but a good
	// way to test functionality.
	int i;
	
	RemoveAllLights();
	
	if( pParentWorld ){
		const int lightCount = pParentWorld->GetLightCount();
		
		for( i=0; i<lightCount; i++ ){
			deoglRLight * const light = pParentWorld->GetLightAt( i );
			
			const float cutOffDist = light->GetRange();
			const decVector difference = ( light->GetMatrix().GetPosition() - pPosition ).ToVector();
			
			if( difference * difference <= cutOffDist * cutOffDist ){
				AddLight( light );
			}
		}
	}
	
	// calculate lumimance and color for the lights in our list
	pLuminance = 0.0f;
	pColor.SetZero();
	
	const int lightCount = pLights.GetCount();
	for( i=0; i<lightCount; i++ ){
		const deoglRLight &light = *pLights.GetAt( i );
		
		const float distance = ( float )( ( light.GetMatrix().GetPosition() - pPosition ).Length() );
		
		const float attenuation = decMath::max( ( light.GetDampingCoefficient() /
			( 1.0f + light.GetAttenuationCoefficient() * distance * distance ) ) +
				light.GetDampingThreshold(), 0.0f );
		
		const float intensity = light.GetIntensity() * attenuation;
		pLuminance += intensity;
		pColor += light.GetColor() * intensity;
	}
	
	// debug, get rid of lights
	RemoveAllLights();
	
	// no more dirty
	pDirtyMeasurements = false;
}



void deoglRLumimeter::PrepareQuickDispose(){
	pParentWorld = NULL;
	pOctreeNode = NULL;
}



// Lights
///////////

int deoglRLumimeter::GetLightCount() const{
	return pLights.GetCount();
}

deoglRLight *deoglRLumimeter::GetLightAt( int index ) const{
	return pLights.GetAt( index );
}

void deoglRLumimeter::AddLight( deoglRLight *light ){
	if( ! light ){
		DETHROW( deeInvalidParam );
	}
	
	pLights.Add( light );
}

void deoglRLumimeter::RemoveLight( deoglRLight *light ){
	pLights.Remove( light );
}

void deoglRLumimeter::RemoveAllLights(){
	pLights.RemoveAll();
}



// Render world usage
///////////////////////

void deoglRLumimeter::SetWorldMarkedRemove( bool marked ){
	pWorldMarkedRemove = marked;
}
