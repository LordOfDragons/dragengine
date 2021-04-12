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
#include "deoglGIBVHLocal.h"
#include "deoglGIBVHDynamic.h"
#include "../component/deoglRComponent.h"
#include "../component/deoglRComponentLOD.h"
#include "../model/deoglModelLOD.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../tbo/deoglDynamicTBOBlock.h"
#include "../utils/bvh/deoglBVH.h"
#include "../utils/bvh/deoglBVHNode.h"

#include <dragengine/common/exceptions.h>


// Class deoglGIInstance::cComponentListener
//////////////////////////////////////////////

deoglGIInstance::cComponentListener::cComponentListener( deoglGIInstance &instance ) :
pInstance( instance ){
}

void deoglGIInstance::cComponentListener::ComponentDestroyed( deoglRComponent& ){
	pInstance.Clear();
	pInstance.SetChanged( true );
}

void deoglGIInstance::cComponentListener::ParentWorldChanged( deoglRComponent& ){
	pInstance.Clear();
	pInstance.SetChanged( true );
}

void deoglGIInstance::cComponentListener::LayerMaskChanged( deoglRComponent& ){
	pInstance.Clear();
	pInstance.SetChanged( true );
}

void deoglGIInstance::cComponentListener::BoundariesChanged( deoglRComponent& ){
	pInstance.SetChanged( true );
}

void deoglGIInstance::cComponentListener::OcclusionMeshChanged( deoglRComponent& ){
	pInstance.SetChanged( true );
}

void deoglGIInstance::cComponentListener::TexturesChanged( deoglRComponent& ){
	pInstance.SetChanged( true );
	pInstance.SetDirtyTUCs( true );
}



// Class deoglGIInstance
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglGIInstance::deoglGIInstance() :
pComponent( NULL ),
pOcclusionMesh( NULL ),
pGIBVHLocal( NULL ),
pGIBVHDynamic( NULL ),
pIndexNodes( 0 ),
pIndexFaces( 0 ),
pIndexVertices( 0 ),
pHasBVHNodes( false ),
pDirtyTUCs( true ),
pDynamic( false ),
pChanged( false ){
}

deoglGIInstance::~deoglGIInstance(){
	Clear();
}



// Management
///////////////

void deoglGIInstance::SetComponent( deoglRComponent *component, bool dynamic ){
	if( component == pComponent ){
		return;
	}
	
	Clear();
	
	pComponent = component;
	pDynamic = dynamic;
	pChanged = false;
	
	if( ! component ){
		return;
	}
	
	if( ! pComponentListener ){
		pComponentListener.TakeOver( new cComponentListener( *this ) );
	}
	component->AddListener( ( deoglComponentListener* )( deObject* )pComponentListener );
	
	deoglRComponentLOD &lod = component->GetLODAt( -1 );
	
	if( dynamic ){
		lod.PrepareGIDynamicBVH();
		pGIBVHDynamic = lod.GetGIBVHDynamic();
		if( ! pGIBVHDynamic ){
			return;
		}
		
		pGIBVHDynamic->AddBlockUsage();
		
	}else{
		deoglModelLOD * const modelLOD = lod.GetModelLOD();
		if( ! modelLOD ){
			return;
		}
		
		modelLOD->PrepareGILocalBVH();
		pGIBVHLocal = modelLOD->GetGIBVHLocal();
		if( ! pGIBVHLocal ){
			return;
		}
		
		pGIBVHLocal->AddBlockUsage();
	}
	
	pInitParameters();
	pDirtyTUCs = true;
}

void deoglGIInstance::SetOcclusionMesh( deoglRComponent *occlusionMesh, bool dynamic ){
	if( occlusionMesh == pOcclusionMesh ){
		return;
	}
	
	Clear();
	
	pOcclusionMesh = occlusionMesh;
	pDynamic = dynamic;
	pChanged = false;
	
	if( occlusionMesh ){
		if( ! pComponentListener ){
			pComponentListener.TakeOver( new cComponentListener( *this ) );
		}
		occlusionMesh->AddListener( ( deoglComponentListener* )( deObject* )pComponentListener );
	}
}

void deoglGIInstance::UpdateExtends(){
	if( pGIBVHDynamic ){
		pMinExtend = pGIBVHDynamic->GetMinimumExtend();
		pMaxExtend = pGIBVHDynamic->GetMaximumExtend();
	}
}

void deoglGIInstance::SetChanged( bool changed ){
	pChanged = changed;
}

void deoglGIInstance::Clear(){
	RemoveAllTUCs();
	pDirtyTUCs = false;
	
	pGIBVHLocal = NULL;
	pGIBVHDynamic = NULL;
	
	pIndexNodes = 0;
	pIndexFaces = 0;
	pIndexVertices = 0;
	pMinExtend.SetZero();
	pMaxExtend.SetZero();
	pHasBVHNodes = false;
	
	pDynamic = false;
	
	if( pComponent ){
		deoglRComponentLOD &lod = pComponent->GetLODAt( -1 );
		
		const deoglModelLOD * const modelLOD = lod.GetModelLOD();
		if( modelLOD ){
			deoglGIBVHLocal * const bvhLocal = modelLOD->GetGIBVHLocal();
			if( bvhLocal ){
				bvhLocal->RemoveBlockUsage();
			}
		}
		
		deoglGIBVHDynamic * const bvhDynamic = lod.GetGIBVHDynamic();
		if( bvhDynamic ){
			bvhDynamic->RemoveBlockUsage();
		}
		
		pComponent->RemoveListener( ( deoglComponentListener* )( deObject* )pComponentListener );
		pComponent = NULL;
	}
	
	if( pOcclusionMesh ){
		pOcclusionMesh->RemoveListener( ( deoglComponentListener* )( deObject* )pComponentListener );
		pOcclusionMesh = NULL;
	}
}



int deoglGIInstance::GetTUCCount() const{
	return pTUCs.GetCount();
}

deoglTexUnitsConfig *deoglGIInstance::GetTUCAt( int index ) const{
	return ( deoglTexUnitsConfig* )pTUCs.GetAt( index );
}

void deoglGIInstance::RemoveAllTUCs(){
	const int count = pTUCs.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglTexUnitsConfig * const tuc = ( deoglTexUnitsConfig* )pTUCs.GetAt( i );
		if( tuc ){
			tuc->RemoveMaterialUsage();
			tuc->RemoveUsage();
		}
	}
	
	pTUCs.RemoveAll();
}

void deoglGIInstance::AddTUC( deoglTexUnitsConfig *tuc ){
	pTUCs.Add( tuc );
	
	if( tuc ){
		tuc->AddUsage();
		tuc->AddMaterialUsage();
	}
}

void deoglGIInstance::SetDirtyTUCs( bool dirty ){
	pDirtyTUCs = dirty;
}



// Private Functions
//////////////////////

void deoglGIInstance::pInitParameters(){
	if( pGIBVHLocal ){
		const deoglDynamicTBOBlock * const blockNode = pGIBVHLocal->GetBlockNode();
		if( blockNode ){
			pIndexNodes = blockNode->GetOffset();
		}
		
		const deoglDynamicTBOBlock * const blockFace = pGIBVHLocal->GetBlockFace();
		if( blockFace ){
			pIndexFaces = blockFace->GetOffset();
		}
		
		const deoglDynamicTBOBlock * const blockVertex = pGIBVHLocal->GetBlockVertex();
		if( blockVertex ){
			pIndexVertices = blockVertex->GetOffset();
		}
		
		const deoglBVHNode * const rootNode = pGIBVHLocal->GetBVH().GetRootNode();
		if( rootNode ){
			pHasBVHNodes = true;
			pMinExtend = rootNode->GetMinExtend();
			pMaxExtend = rootNode->GetMaxExtend();
		}
		
	}else if( pGIBVHDynamic ){
		const deoglDynamicTBOBlock * const blockNode = pGIBVHDynamic->GetBlockNode();
		if( blockNode ){
			pIndexNodes = blockNode->GetOffset();
		}
		
		const deoglDynamicTBOBlock * const blockFace = pGIBVHDynamic->GetGIBVHLocal().GetBlockFace();
		if( blockFace ){
			pIndexFaces = blockFace->GetOffset();
		}
		
		const deoglDynamicTBOBlock * const blockVertex = pGIBVHDynamic->GetBlockVertex();
		if( blockVertex ){
			pIndexVertices = blockVertex->GetOffset();
		}
		
		pHasBVHNodes = pGIBVHDynamic->GetGIBVHLocal().GetBVH().GetRootNode() != NULL;
		pMinExtend = pGIBVHDynamic->GetMinimumExtend();
		pMaxExtend = pGIBVHDynamic->GetMaximumExtend();
	}
}
