/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deoglGIInstance.h"
#include "deoglGIInstances.h"
#include "../renderthread/deoglRenderThread.h"
#include "../collidelist/deoglCollideList.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../component/deoglRComponent.h"
#include "../component/deoglRComponentLOD.h"
#include "../model/deoglRModel.h"
#include "../model/deoglModelLOD.h"
#include "../occlusiontest/mesh/deoglROcclusionMesh.h"
#include "../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>


// Class deoglGIInstances
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglGIInstances::deoglGIInstances( deoglRenderThread &renderThread ) :
pRenderThread( renderThread  ){
}

deoglGIInstances::~deoglGIInstances(){
	pCleanUp();
}



// Management
///////////////

bool deoglGIInstances::IsComponentStatic( const deoglRComponent &component ){
	// weight check catches more cases than GetRenderStatic() or GetRenderMode() can.
	// static textures is checked to avoid cached rays with changing textures
	return component.GetRenderStatic()
		&& component.GetStaticTextures()
		&& component.GetMovementHint() == deComponent::emhStationary
		&& component.GetLODAt( -1 ).GetModelLODRef().GetWeightsCount() == 0;
}

bool deoglGIInstances::IsOcclusionMeshStatic( const deoglRComponent &component ){
	return component.GetRenderStatic()
		&& component.GetMovementHint() == deComponent::emhStationary
		&& ! component.GetDynamicOcclusionMesh();
}

deoglGIInstance &deoglGIInstances::GetInstanceAt( int slot ) const{
	return *( ( deoglGIInstance* )pInstances.GetAt( slot ) );
}

deoglGIInstance &deoglGIInstances::AddInstance(){
	deObjectReference ref;
	ref.TakeOver( new deoglGIInstance( pRenderThread ) );
	pInstances.Add( ref );
	
	return ( deoglGIInstance& )( deObject& )ref;
}

deoglGIInstance &deoglGIInstances::NextFreeSlot(){
	const int count = pInstances.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglGIInstance &instance = *( ( deoglGIInstance* )pInstances.GetAt( i ) );
		if( instance.Empty() ){
			return instance;
		}
	}
	
	return AddInstance();
}

bool deoglGIInstances::AnyChanged() const{
	const int count = pInstances.GetCount();
	bool changed = false;
	int i;
	
	for( i=0; i<count; i++ ){
		deoglGIInstance &instance = *( ( deoglGIInstance* )pInstances.GetAt( i ) );
		if( ! instance.GetChanged() ){
			continue;
		}
		
		changed |= ! instance.GetDynamic();
		
		if( instance.GetRecheckDynamic() ){
			instance.SetRecheckDynamic( false );
			
			const bool dynamic = instance.GetDynamic();
			
			if( instance.GetComponent() ){
				instance.SetDynamic( ! IsComponentStatic( *instance.GetComponent() ) );
				
			}else if( instance.GetOcclusionMesh() ){
				instance.SetDynamic( ! IsOcclusionMeshStatic( *instance.GetOcclusionMesh() ) );
				
			}else{
				instance.SetDynamic( false );
			}
			
			changed |= instance.GetDynamic() != dynamic;
		}
	}
	
	return changed;
}

bool deoglGIInstances::AnyComponentChanged() const{
	const int count = pInstances.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deoglGIInstance &instance = *( ( deoglGIInstance* )pInstances.GetAt( i ) );
		if( instance.GetComponent() && instance.GetChanged() && ! instance.GetDynamic() ){
			return true;
		}
	}
	
	return false;
}

bool deoglGIInstances::AnyOcclusionMeshChanged() const{
	const int count = pInstances.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deoglGIInstance &instance = *( ( deoglGIInstance* )pInstances.GetAt( i ) );
		if( instance.GetOcclusionMesh() && instance.GetChanged() && ! instance.GetDynamic() ){
			return true;
		}
	}
	
	return false;
}

void deoglGIInstances::ClearAllChanged(){
	const int count = pInstances.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglGIInstance* )pInstances.GetAt( i ) )->SetChanged( false );
	}
}

bool deoglGIInstances::AddComponents( deoglCollideList &list ){
	const int count = list.GetComponentCount();
	bool anyAdded = false;
	int i;
	
	list.MarkComponents( true );
	MarkComponents( false );
	
	for( i=0; i<count; i++ ){
		deoglRComponent &component = *list.GetComponentAt( i )->GetComponent();
		if( ! component.GetMarked() ){
			continue;
		}
		
		const bool isStatic = IsComponentStatic( component );
		NextFreeSlot().SetComponent( &component, ! isStatic );
		if( isStatic ){
			anyAdded = true;
		}
		
// 		{ // debug
// 			int j, index = -1;
// 			for( j=0; j<pInstances.GetCount(); j++ ){
// 				if( ( ( deoglGIInstance* )pInstances.GetAt( j ) )->GetComponent() == &component ){
// 					index = j;
// 					break;
// 				}
// 			}
// 			const decDVector p( component.GetMatrix().GetPosition() );
// 			pRenderThread.GetLogger().LogInfoFormat( "GIInstances: AddComponent: %d (%g,%g,%g) component=%s [%d]",
// 				index, p.x, p.y, p.z, component.GetModel() ? component.GetModel()->GetFilename().GetString() : "-", isStatic );
// 		}
	}
	
	return anyAdded;
}

bool deoglGIInstances::RemoveComponents( deoglCollideList &list ){
	const int count = pInstances.GetCount();
	bool anyRemoved = false;
	int i;
	
	MarkComponents( true );
	list.MarkComponents( false );
	
	for( i=0; i<count; i++ ){
		deoglGIInstance &instance = *( ( deoglGIInstance* )pInstances.GetAt( i ) );
		if( ! instance.GetComponent() ){
			continue;
		}
		if( ! instance.GetComponent()->GetMarked() ){
			continue;
		}
		
// 		{ // debug
// 			const decDVector p( instance.GetComponent()->GetMatrix().GetPosition() );
// 			pRenderThread.GetLogger().LogInfoFormat( "GIInstances: RemoveComponent: %d (%g,%g,%g) component=%s",
// 				i, p.x, p.y, p.z, instance.GetComponent()->GetModel()
// 					? instance.GetComponent()->GetModel()->GetFilename().GetString() : "-" );
// 		}
		
		instance.Clear();
		if( ! instance.GetDynamic() ){
			anyRemoved = true;
		}
	}
	
	return anyRemoved;
}

bool deoglGIInstances::AddOcclusionMeshes( deoglCollideList &list ){
	const int count = list.GetComponentCount();
	bool anyAdded = false;
	int i;
	
	list.MarkComponents( true );
	MarkOcclusionMeshes( false );
	
	for( i=0; i<count; i++ ){
		deoglRComponent &component = *list.GetComponentAt( i )->GetComponent();
		if( component.GetMarked() ){
			const bool isStatic = IsOcclusionMeshStatic( component );
			NextFreeSlot().SetOcclusionMesh( &component, ! isStatic );
			if( isStatic ){
				anyAdded = true;
			}
			
// 			{ // debug
// 				int j, index = -1;
// 				for( j=0; j<pInstances.GetCount(); j++ ){
// 					if( ( ( deoglGIInstance* )pInstances.GetAt( j ) )->GetOcclusionMesh() == &component ){
// 						index = j;
// 						break;
// 					}
// 				}
// 				const decDVector p( component.GetMatrix().GetPosition() );
// 				pRenderThread.GetLogger().LogInfoFormat( "GIInstances: AddOcclusionMesh: %d (%g,%g,%g) occmesh=%s",
// 					index, p.x, p.y, p.z, component.GetOcclusionMesh() ? component.GetOcclusionMesh()->GetFilename().GetString() : "-" );
// 			}
		}
	}
	
	return anyAdded;
}

bool deoglGIInstances::RemoveOcclusionMeshes( deoglCollideList &list ){
	const int count = pInstances.GetCount();
	bool anyRemoved = false;
	int i;
	
	MarkOcclusionMeshes( true );
	list.MarkComponents( false );
	
	for( i=0; i<count; i++ ){
		deoglGIInstance &instance = *( ( deoglGIInstance* )pInstances.GetAt( i ) );
		if( instance.GetOcclusionMesh() && instance.GetOcclusionMesh()->GetMarked() ){
// 			{ // debug
// 				const decDVector p( instance.GetOcclusionMesh()->GetMatrix().GetPosition() );
// 				pRenderThread.GetLogger().LogInfoFormat( "GIInstances: RemoveOcclusionMesh: %d (%g,%g,%g) occmesh=%s",
// 					i, p.x, p.y, p.z, instance.GetOcclusionMesh()->GetOcclusionMesh()
// 						? instance.GetOcclusionMesh()->GetOcclusionMesh()->GetFilename().GetString() : "-" );
// 			}
			
			instance.Clear();
			if( ! instance.GetDynamic() ){
				anyRemoved = true;
			}
		}
	}
	
	return anyRemoved;
}

void deoglGIInstances::MarkAll( bool marked ){
	const int count = pInstances.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglGIInstance &instance = *( ( deoglGIInstance* )pInstances.GetAt( i ) );
		if( instance.GetComponent() ){
			instance.GetComponent()->SetMarked( marked );
			
		}else if( instance.GetOcclusionMesh() ){
			instance.GetOcclusionMesh()->SetMarked( marked );
		}
	}
}

void deoglGIInstances::MarkComponents( bool marked ){
	const int count = pInstances.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglGIInstance &instance = *( ( deoglGIInstance* )pInstances.GetAt( i ) );
		if( instance.GetComponent() ){
			instance.GetComponent()->SetMarked( marked );
		}
	}
}

void deoglGIInstances::MarkOcclusionMeshes( bool marked ){
	const int count = pInstances.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglGIInstance &instance = *( ( deoglGIInstance* )pInstances.GetAt( i ) );
		if( instance.GetOcclusionMesh() ){
			instance.GetOcclusionMesh()->SetMarked( marked );
		}
	}
}

void deoglGIInstances::DebugPrint(){
	deoglRTLogger &logger = pRenderThread.GetLogger();
	const int count = pInstances.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		const deoglGIInstance &instance = *( ( deoglGIInstance* )pInstances.GetAt( i ) );
		if( instance.GetComponent() ){
			const decDVector p( instance.GetComponent()->GetMatrix().GetPosition() );
				logger.LogInfoFormat( "%d: component (%g,%g,%g) %s",
					i, p.x, p.y, p.z, instance.GetComponent()->GetModel()
						? instance.GetComponent()->GetModel()->GetFilename().GetString() : "-" );
				
		}else if( instance.GetOcclusionMesh() ){
			const decDVector p( instance.GetOcclusionMesh()->GetMatrix().GetPosition() );
				logger.LogInfoFormat( "%d: occlusion mesh (%g,%g,%g) %s",
					i, p.x, p.y, p.z, instance.GetOcclusionMesh()->GetOcclusionMesh()
						? instance.GetOcclusionMesh()->GetOcclusionMesh()->GetFilename().GetString() : "-" );

		}
	}
}



// Private Functions
//////////////////////

void deoglGIInstances::pCleanUp(){
}
