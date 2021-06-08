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

deoglGIInstance &deoglGIInstances::GetInstanceAt( int slot ) const{
	return *( ( deoglGIInstance* )pInstances.GetAt( slot ) );
}

deoglGIInstance *deoglGIInstances::GetInstanceWithComponent( deoglRComponent *component ) const{
	if( ! component ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pInstances.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglGIInstance * const instance = ( deoglGIInstance* )pInstances.GetAt( i );
		if( instance->GetComponent() == component ){
			return instance;
		}
	}
	
	return NULL;
}

deoglGIInstance &deoglGIInstances::AddInstance(){
	deObjectReference ref;
	ref.TakeOver( new deoglGIInstance( *this ) );
	pInstances.Add( ref );
	
	return ( deoglGIInstance& )( deObject& )ref;
}

deoglGIInstance &deoglGIInstances::NextFreeSlot(){
	if( pEmptyInstances.GetCount() == 0 ){
		return AddInstance();
	}
	
	const int index = pEmptyInstances.GetCount() - 1;
	deoglGIInstance * const instance = ( deoglGIInstance* )pEmptyInstances.GetAt( index );
	pEmptyInstances.RemoveFrom( index );
	return *instance;
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
			         sBox * const newArray = new sBox[ newSize ];
			if( pDynamicBoxes ){
				memcpy( newArray, pDynamicBoxes, sizeof( sBox ) * pDynamicBoxCount );
				delete [] pDynamicBoxes;
			}
			pDynamicBoxes = newArray;
			pDynamicBoxSize = newSize;
		}
		
		      sBox &box = pDynamicBoxes[ pDynamicBoxCount++ ];
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
	pEmptyInstances.RemoveAll();
	
	const int count = pInstances.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		deoglGIInstance * const instance = ( deoglGIInstance* )pInstances.GetAt( i );
		instance->Clear();
		pEmptyInstances.Add( instance );
	}
	
	pChangedInstances.RemoveAll();
}

void deoglGIInstances::ApplyChanges(){
	const int count = pChangedInstances.GetCount();
	if( count == 0 ){
		return;
	}
	
	int i;
	for( i=0; i<count; i++ ){
		deoglGIInstance &instance = *( ( deoglGIInstance* )pChangedInstances.GetAt( i ) );
		
		bool invalidate = ! instance.GetDynamic();
		
		if( instance.GetRecheckDynamic() ){
			instance.SetRecheckDynamic( false );
			
			const bool dynamic = instance.GetDynamic();
			
			if( instance.GetComponent() ){
				instance.SetDynamic( ! IsComponentStatic( *instance.GetComponent() ) );
				
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
		
		instance.SetChanged( false );
	}
	
	pChangedInstances.RemoveAll();
}

// #define DO_LOG_ADD_REMOVE 1

void deoglGIInstances::AddComponent( deoglRComponent *component, bool invalidate ){
	const bool isStatic = IsComponentStatic( *component );
	NextFreeSlot().SetComponent( component, ! isStatic );
	
	if( invalidate ){
		if( isStatic ){
			#ifdef DO_LOG_ADD_REMOVE
				pGIState.GetRenderThread().GetLogger().LogInfoFormat("GIInstances.AddComponent: %s",
					component->GetModel()->GetFilename().GetString());
			#endif
			pGIState.InvalidateArea( component->GetMinimumExtend(), component->GetMaximumExtend() );
				// WARNING InvalidateArea becomes expensive if called multiple times.
				//         unfortunately we can not collect all boxes into an enclosing box
				//         since then moving diagonally can invalidate lots of probes inside
				//         the area which should not be touched. not sure how to solve this.
				//         maybe a small octree for probe extends?
			
		}else{
			#ifdef DO_LOG_ADD_REMOVE
				pGIState.GetRenderThread().GetLogger().LogInfoFormat("GIInstances.AddComponent: %s",
					component->GetModel()->GetFilename().GetString());
			#endif
			pGIState.TouchDynamicArea( component->GetMinimumExtend(), component->GetMaximumExtend() );
		}
	}
	
	#ifdef DO_LOG_ADD_REMOVE
		{
		int j, index = -1;
		for( j=0; j<pInstances.GetCount(); j++ ){
			if( ( ( deoglGIInstance* )pInstances.GetAt( j ) )->GetComponent() == component ){
				index = j;
				break;
			}
		}
		const decDVector p( component->GetMatrix().GetPosition() );
		pGIState.GetRenderThread().GetLogger().LogInfoFormat( "GIInstances: AddComponent: %d (%g,%g,%g) component=%s [%d]",
			index, p.x, p.y, p.z, component->GetModel() ? component->GetModel()->GetFilename().GetString() : "-", isStatic );
		}
	#endif
}

void deoglGIInstances::AddComponents( const deoglCollideList &list, bool invalidate ){
	const int count = list.GetComponentCount();
	int i;
	for( i=0; i<count; i++ ){
		AddComponent( list.GetComponentAt( i )->GetComponent(), invalidate );
	}
}

void deoglGIInstances::RemoveComponent( deoglRComponent *component ){
	deoglGIInstance * const instance = GetInstanceWithComponent( component );
	if( instance ){
		RemoveInstance( *instance );
	}
}

void deoglGIInstances::RemoveComponents( const deoglCollideList &list ){
	if( list.GetComponentCount() == 0 ){
		return;
	}
	
	const int count = pInstances.GetCount();
	int i;
	
	MarkComponents( true );
	list.MarkComponents( false );
	
	for( i=0; i<count; i++ ){
		deoglGIInstance &instance = *( ( deoglGIInstance* )pInstances.GetAt( i ) );
		
		if( instance.GetComponent() ){
			if( instance.GetComponent()->GetMarked() ){
				continue;
			}
			
			#ifdef DO_LOG_ADD_REMOVE
				{
				const decDVector p( instance.GetComponent()->GetMatrix().GetPosition() );
				pGIState.GetRenderThread().GetLogger().LogInfoFormat( "GIInstances.RemoveComponents: %d (%g,%g,%g) component=%s",
					i, p.x, p.y, p.z, instance.GetComponent()->GetModel()
						? instance.GetComponent()->GetModel()->GetFilename().GetString() : "-" );
				}
			#endif
			
			// GI field moved and component is no longer inside the GI field.
			// no invalidating is required
			
		}else{
			continue;
		}
		
		instance.Clear();
		pEmptyInstances.Add( &instance );
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



void deoglGIInstances::RemoveInstance( deoglGIInstance &instance ){
	if( instance.GetDynamic() ){
		pGIState.TouchDynamicArea( instance.GetMinimumExtend(), instance.GetMaximumExtend() );
		
	}else{
		#ifdef DO_LOG_ADD_REMOVE
			pGIState.GetRenderThread().GetLogger().LogInfoFormat("GIInstances.RemoveInstance: %p", &instance);
		#endif
		pGIState.InvalidateArea( instance.GetMinimumExtend(), instance.GetMaximumExtend() );
	}
	
	instance.Clear();
	pEmptyInstances.Add( &instance );
}

void deoglGIInstances::InstanceChanged( deoglGIInstance &instance ){
	pChangedInstances.Add( &instance );
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
