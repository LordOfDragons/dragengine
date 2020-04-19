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

#include "deoglCLVisitorElements.h"
#include "deoglCollideList.h"
#include "../billboard/deoglRBillboard.h"
#include "../component/deoglRComponent.h"
#include "../light/deoglRLight.h"
#include "../particle/deoglRParticleEmitterInstance.h"
#include "../world/deoglWorldOctree.h"
#include "../utils/collision/deoglDCollisionVolume.h"
#include "../utils/collision/deoglDCollisionSphere.h"
#include "../utils/collision/deoglDCollisionBox.h"
#include "../utils/collision/deoglDCollisionDetection.h"

#include <dragengine/common/exceptions.h>



// Class deoglCLVisitorElements
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCLVisitorElements::deoglCLVisitorElements( deoglCollideList *collideList, deoglDCollisionVolume *volume ){
	if( ! collideList || ! volume ) DETHROW( deeInvalidParam );
	
	pCollideList = collideList;
	pVolume = volume;
	
	SetVisitBillboards( true );
	SetVisitComponents( true );
	SetVisitLights( true );
}



// Management
///////////////



// Visiting
/////////////

void deoglCLVisitorElements::VisitNode( deoglDOctree *node, int intersection ){
	const deoglWorldOctree &sonode = *( ( deoglWorldOctree* )node );
	deoglDCollisionBox box;
	int i;
	
	if( GetVisitParticleEmitters() ){
		const deoglParticleEmitterInstanceList &nodeList = sonode.GetParticleEmittersList();
		deoglParticleEmitterInstanceList &clList = pCollideList->GetParticleEmitterList();
		const int count = nodeList.GetCount();
		
		if( intersection == deoglDCollisionDetection::eirInside ){
			for( i=0; i<count; i++ ){
				clList.Add( nodeList.GetAt( i ) );
			}
			
		}else{
			for( i=0; i<count; i++ ){
				deoglRParticleEmitterInstance * const emitter = nodeList.GetAt( i );
				
				box.SetFromExtends( emitter->GetMinExtend(), emitter->GetMaxExtend() );
				if( pVolume->BoxHitsVolume( &box ) ){
					clList.Add( emitter );
				}
			}
		}
	}
	
	if( GetVisitComponents() ){
		const int count = sonode.GetComponentCount();
		deoglRComponent *component;
		
		if( intersection == deoglDCollisionDetection::eirInside ){
			for( i=0; i<count; i++ ){
				pCollideList->AddComponent( sonode.GetComponentAt( i ) );
			}
			
		}else{
			for( i=0; i<count; i++ ){
				component = sonode.GetComponentAt( i );
				
				box.SetFromExtends( component->GetMinimumExtend(), component->GetMaximumExtend() );
				if( pVolume->BoxHitsVolume( &box ) ){
					pCollideList->AddComponent( component );
				}
			}
		}
	}
	
	if( GetVisitBillboards() ){
		const int count = sonode.GetBillboardList().GetCount();
		
		if( intersection == deoglDCollisionDetection::eirInside ){
			for( i=0; i<count; i++ ){
				pCollideList->AddBillboard( sonode.GetBillboardList().GetAt( i ) );
			}
			
		}else{
			for( i=0; i<count; i++ ){
				deoglRBillboard * const billboard = sonode.GetBillboardList().GetAt( i );
				
				box.SetFromExtends( billboard->GetMinimumExtend(), billboard->GetMaximumExtend() );
				if( pVolume->BoxHitsVolume( &box ) ){
					pCollideList->AddBillboard( billboard );
				}
			}
		}
	}
	
	if( GetVisitLights() ){
		const int count = sonode.GetLightCount();
		deoglRLight *light;
		
		if( intersection == deoglDCollisionDetection::eirInside ){
			for( i=0; i<count; i++ ){
				pCollideList->AddLight( sonode.GetLightAt( i ) );
			}
			
		}else{
			for( i=0; i<count; i++ ){
				light = sonode.GetLightAt( i );
				
				if( light->GetCollisionVolume()->VolumeHitsVolume( pVolume ) ){
					pCollideList->AddLight( light );
				}
			}
		}
	}
}
