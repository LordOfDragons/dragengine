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

#include "deoglGIState.h"
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

deoglGIInstances::deoglGIInstances( deoglGIState &giState ) :
pGIState( giState  ),
pDynamicBoxes( NULL ),
pDynamicBoxCount( 0 ),
pDynamicBoxSize( 0 ){
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
	ref.TakeOver( new deoglGIInstance( pGIState.GetRenderThread() ) );
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



void deoglGIInstances::UpdateDynamicBoxes( const decDVector &offset, const decVector &enlarge ){
	pDynamicBoxCount = 0;
	
	const decVector halfEnlarge( enlarge * 0.5f );
	const int count = pInstances.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deoglGIInstance &instance = *( ( deoglGIInstance* )pInstances.GetAt( i ) );
		if( ! instance.GetDynamic() ){
			continue;
		}
		
		if( pDynamicBoxCount == pDynamicBoxSize ){
			const int newSize = pDynamicBoxCount * 3 / 2 + 1;
			sDynamicBox * const newArray = new sDynamicBox[ newSize ];
			if( pDynamicBoxes ){
				memcpy( newArray, pDynamicBoxes, sizeof( sDynamicBox ) * pDynamicBoxCount );
				delete [] pDynamicBoxes;
			}
			pDynamicBoxes = newArray;
			pDynamicBoxSize = newSize;
		}
		
		sDynamicBox &box = pDynamicBoxes[ pDynamicBoxCount++ ];
		box.minExtend = ( instance.GetMinimumExtend() + offset ).ToVector() - halfEnlarge;
		box.maxExtend = ( instance.GetMaximumExtend() + offset ).ToVector() + halfEnlarge;
	}
}

bool deoglGIInstances::DynamicBoxesContain( const decVector &point ) const{
	int i;
	for( i=0; i<pDynamicBoxCount; i++ ){
		if( point >= pDynamicBoxes[ i ].minExtend && point <= pDynamicBoxes[ i ].maxExtend ){
			return true;
		}
	}
	return false;
}

int deoglGIInstances::CountDynamicBoxesContaining( const decVector &point ) const{
	int i, count = 0;
	for( i=0; i<pDynamicBoxCount; i++ ){
		if( point >= pDynamicBoxes[ i ].minExtend && point <= pDynamicBoxes[ i ].maxExtend ){
			count++;
		}
	}
	return count;
}



void deoglGIInstances::Clear(){
	const int count = pInstances.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( deoglGIInstance* )pInstances.GetAt( i ) )->Clear();
	}
}

void deoglGIInstances::AnyChanged() const{
	const int count = pInstances.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglGIInstance &instance = *( ( deoglGIInstance* )pInstances.GetAt( i ) );
		if( ! instance.GetChanged() ){
			continue;
		}
		
		bool invalidate = ! instance.GetDynamic();
		
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
			
			invalidate |= instance.GetDynamic() != dynamic;
			
			if( invalidate ){
				pGIState.TouchDynamicArea( instance.GetMinimumExtend(), instance.GetMaximumExtend() );
			}
		}
		
		if( invalidate ){
// 				pGIState.GetRenderThread().GetLogger().LogInfoFormat("GIInstances.AnyChanged: %s",
// 					instance.GetComponent()?instance.GetComponent()->GetModel()->GetFilename().GetString():"-");
			pGIState.InvalidateArea( instance.GetMinimumExtend(), instance.GetMaximumExtend() );
		}
		
		if( instance.GetMoved() ){
			instance.SetMoved( false );
			
			if( instance.GetComponent() ){
				if( instance.GetDynamic() ){
					pGIState.TouchDynamicArea( instance.GetMinimumExtend(), instance.GetMaximumExtend() );
				}
				
				const decDVector &minExtend = instance.GetComponent()->GetMinimumExtend();
				const decDVector &maxExtend = instance.GetComponent()->GetMaximumExtend();
				instance.SetExtends( minExtend, maxExtend );
				
				if( instance.GetDynamic() ){
					pGIState.TouchDynamicArea( minExtend, maxExtend );
				}
				
				if( invalidate ){
// 						pGIState.GetRenderThread().GetLogger().LogInfoFormat("GIInstances.AnyChanged: Moved %s",
// 							instance.GetComponent()?instance.GetComponent()->GetModel()->GetFilename().GetString():"-");
					pGIState.InvalidateArea( minExtend, maxExtend );
				}
			}
		}
	}
}

void deoglGIInstances::ClearAllChanged(){
	const int count = pInstances.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglGIInstance* )pInstances.GetAt( i ) )->SetChanged( false );
	}
}

void deoglGIInstances::AddComponents( deoglCollideList &list ){
	const int count = list.GetComponentCount();
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
// 				pGIState.GetRenderThread().GetLogger().LogInfoFormat("GIInstances.AddComponent: %s",
// 					component.GetModel()->GetFilename().GetString());
			pGIState.InvalidateArea( component.GetMinimumExtend(), component.GetMaximumExtend() );
			
		}else{
// 				pGIState.GetRenderThread().GetLogger().LogInfoFormat("GIInstances.AddComponent: %s",
// 					component.GetModel()->GetFilename().GetString());
			pGIState.TouchDynamicArea( component.GetMinimumExtend(), component.GetMaximumExtend() );
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
}

void deoglGIInstances::RemoveComponents( deoglCollideList &list ){
	const int count = pInstances.GetCount();
	int i;
	
	MarkComponents( true );
	list.MarkComponents( false );
	
	for( i=0; i<count; i++ ){
		deoglGIInstance &instance = *( ( deoglGIInstance* )pInstances.GetAt( i ) );
		if( ! instance.GetComponent() ){
			if( instance.GetChanged() ){
				// component has been removed from game world
				instance.SetChanged( false );
				if( instance.GetDynamic() ){
					pGIState.TouchDynamicArea( instance.GetMinimumExtend(), instance.GetMaximumExtend() );
					
				}else{
// 						pGIState.GetRenderThread().GetLogger().LogInfoFormat("GIInstances.RemoveComponents: LeftWorld %d", i);
					pGIState.InvalidateArea( instance.GetMinimumExtend(), instance.GetMaximumExtend() );
				}
			}
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
		
		// either GI field moved and component is no longer inside the GI field or the component
		// moved out of the GI field. if GI field moved no invalidating is required. if component
		// moved invalidating is required
		if( instance.GetChanged() ){
			if( instance.GetDynamic() ){
				pGIState.TouchDynamicArea( instance.GetMinimumExtend(), instance.GetMaximumExtend() );
				
			}else{
// 				pGIState.GetRenderThread().GetLogger().LogInfoFormat("GIInstances.AnyChanged: LeftField %s",
// 					instance.GetComponent()->GetModel()->GetFilename().GetString());
				pGIState.InvalidateArea( instance.GetMinimumExtend(), instance.GetMaximumExtend() );
			}
		}
		
		instance.Clear();
	}
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
	deoglRTLogger &logger = pGIState.GetRenderThread().GetLogger();
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
	if( pDynamicBoxes ){
		delete [] pDynamicBoxes;
	}
}
