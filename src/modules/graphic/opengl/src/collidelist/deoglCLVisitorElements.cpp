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
