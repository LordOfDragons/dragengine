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

#include "deoglGI.h"
#include "deoglGIBVHLocal.h"
#include "deoglGIBVHDynamic.h"
#include "deoglGIInstance.h"
#include "deoglGIInstances.h"
#include "deoglGIState.h"
#include "../component/deoglRComponent.h"
#include "../component/deoglRComponentLOD.h"
#include "../decal/deoglRDecal.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../tbo/deoglDynamicTBOBlock.h"
#include "../tbo/deoglDynamicTBOFloat16.h"
#include "../tbo/deoglDynamicTBOFloat32.h"
#include "../tbo/deoglDynamicTBOUInt32.h"
#include "../tbo/deoglDynamicTBOShared.h"
#include "../utils/bvh/deoglBVH.h"
#include "../utils/bvh/deoglBVHNode.h"
#include "../utils/collision/deoglDCollisionBox.h"

#include <dragengine/common/exceptions.h>


// Class deoglGIInstance::cListenerLink
/////////////////////////////////////////

deoglGIInstance::cListenerLink::cListenerLink( deoglGIInstance& instance ) : instance( instance ){
}

bool deoglGIInstance::cListenerLink::LayerMaskMatchesNot( const decLayerMask& layerMask ) const{
	return instance.GetInstances().GetGIState().GetLayerMask().MatchesNot( layerMask );
}

void deoglGIInstance::cListenerLink::RemoveInstance() const{
	if( instance.GetComponent() ){
		instance.GetInstances().RemoveInstance( instance ); // does clear and removes listener
	}
}

void deoglGIInstance::cListenerLink::ChangeInstance( bool hard ) const{
	if( hard ){
		instance.SetHardChanged( true );
	}
	if( ! instance.GetChanged() ){
		instance.GetInstances().InstanceChanged( instance );
	}
	
	instance.SetChanged( true );
}

void deoglGIInstance::cListenerLink::Moved() const{
	ChangeInstance( true );
	instance.SetMoved( true );
}

void deoglGIInstance::cListenerLink::TUCChanged() const{
	ChangeInstance( false );
	instance.SetRecheckDynamic( true );
	instance.SetDirtyTUCs( true );
}

void deoglGIInstance::cListenerLink::DynamicChanged() const{
	ChangeInstance( false );
	instance.SetRecheckDynamic( true );
}


// Class deoglGIInstance::cComponentListener
//////////////////////////////////////////////

deoglGIInstance::cComponentListener::cComponentListener( deoglGIInstance &instance ) :
pLink( instance ){
}

void deoglGIInstance::cComponentListener::ComponentDestroyed( deoglRComponent& ){
	pLink.RemoveInstance();
}

void deoglGIInstance::cComponentListener::ParentWorldChanged( deoglRComponent& ){
	pLink.RemoveInstance();
}

void deoglGIInstance::cComponentListener::LayerMaskChanged( deoglRComponent &component ){
	if( component.GetLayerMask().IsNotEmpty() && pLink.LayerMaskMatchesNot( component.GetLayerMask() ) ){
		pLink.RemoveInstance();
	}
}

void deoglGIInstance::cComponentListener::BoundariesChanged( deoglRComponent& ){
// 		pInstance.GetInstances().GetGIState().GetRenderThread().GetLogger().LogInfoFormat(
// 			"GIInstance BoundariesChanged %s (dynamic %d)\n", c.GetModel()->GetFilename().GetString(), pInstance.GetDynamic());
	pLink.Moved();
}

void deoglGIInstance::cComponentListener::OcclusionMeshChanged( deoglRComponent& ){
// 	ChangeInstance();
}

void deoglGIInstance::cComponentListener::TexturesChanged( deoglRComponent& ){
// 		pInstance.GetInstances().GetGIState().GetRenderThread().GetLogger().LogInfoFormat(
// 			"GIInstance TexturesChanged %s\n", c.GetModel()->GetFilename().GetString());
	pLink.TUCChanged();
}

void deoglGIInstance::cComponentListener::RenderStaticChanged( deoglRComponent& ){
// 		pInstance.GetInstances().GetGIState().GetRenderThread().GetLogger().LogInfoFormat(
// 			"GIInstance RenderStaticChanged %s (dynamic %d)\n", c.GetModel()->GetFilename().GetString(), pInstance.GetDynamic());
	// is ChangeInstance(false) required?
	pLink.DynamicChanged();
}

void deoglGIInstance::cComponentListener::MovementHintChanged( deoglRComponent& ){
// 		pInstance.GetInstances().GetGIState().GetRenderThread().GetLogger().LogInfoFormat(
// 			"GIInstance MovementHintChanged %s (dynamic %d)\n", c.GetModel()->GetFilename().GetString(), pInstance.GetDynamic());
	pLink.DynamicChanged();
}

void deoglGIInstance::cComponentListener::VisibilityChanged( deoglRComponent &component ){
	if( ! component.GetVisible() ){
		pLink.RemoveInstance();
	}
}


// Class deoglGIInstance::cDecalListener
//////////////////////////////////////////

deoglGIInstance::cDecalListener::cDecalListener( deoglGIInstance &instance ) :
pLink( instance ){
}

void deoglGIInstance::cDecalListener::DecalDestroyed( deoglRDecal& ){
	pLink.RemoveInstance();
}

void deoglGIInstance::cDecalListener::GeometryChanged( deoglRDecal& ){
	pLink.Moved();
}

void deoglGIInstance::cDecalListener::TextureChanged( deoglRDecal& ){
	pLink.TUCChanged();
}

void deoglGIInstance::cDecalListener::TUCChanged( deoglRDecal& ){
	pLink.TUCChanged();
}


// Class deoglGIInstance::cDecalComponentListener
///////////////////////////////////////////////////

deoglGIInstance::cDecalComponentListener::cDecalComponentListener( deoglGIInstance &instance ) :
pLink( instance ){
}

void deoglGIInstance::cDecalComponentListener::BoundariesChanged( deoglRComponent& ){
	pLink.Moved();
}

void deoglGIInstance::cDecalComponentListener::RenderStaticChanged( deoglRComponent& ){
	// is ChangeInstance(false) required?
	pLink.DynamicChanged();
}

void deoglGIInstance::cDecalComponentListener::MovementHintChanged( deoglRComponent& ){
	pLink.DynamicChanged();
}



// Class deoglGIInstance
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglGIInstance::deoglGIInstance( deoglGIInstances &instances ) :
pInstances( instances ),
pComponent( NULL ),
pDecal( NULL ),
pGIBVHLocal( NULL ),
pGIBVHDynamic( NULL ),
pIndexNodes( 0 ),
pIndexFaces( 0 ),
pIndexVertices( 0 ),
pHasBVHNodes( false ),
pDirtyTUCs( true ),
pTBOMaterial( NULL ),
pTBOMaterial2( NULL ),
pDynamic( false ),
pChanged( false ),
pHardChanged( false ),
pMoved( false ),
pRecheckDynamic( false )
{
	deoglRenderThread &renderThread = instances.GetGIState().GetRenderThread();
	
	try{
		pTBOMaterial = new deoglDynamicTBOUInt32( renderThread, 4 );
		pTBOMaterial2 = new deoglDynamicTBOFloat16( renderThread, 4 );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglGIInstance::~deoglGIInstance(){
	pCleanUp();
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
	pHardChanged = false;
	pMoved = false;
	pRecheckDynamic = false;
	
	if( ! component ){
		return;
	}
	
	pMinExtend = component->GetMinimumExtend();
	pMaxExtend = component->GetMaximumExtend();
	
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
	
	pInstances.RegisterElement( component, this );
}

void deoglGIInstance::SetDecal( deoglRDecal *decal, bool dynamic ){
	if( decal == pDecal ){
		return;
	}
	
	Clear();
	
	pDecal = decal;
	pDynamic = dynamic;
	pChanged = false;
	pHardChanged = false;
	pMoved = false;
	pRecheckDynamic = false;
	pMinExtend.SetZero();
	pMaxExtend.SetZero();
	
	if( ! decal || ! decal->GetParentComponent() || dynamic ){
		return;
	}
	
	if( ! pDecalListener ){
		pDecalListener.TakeOver( new cDecalListener( *this ) );
	}
	decal->AddListener( ( deoglDecalListener* )( deObject* )pDecalListener );
	
	if( ! pDecalComponentListener ){
		pDecalComponentListener.TakeOver( new cDecalComponentListener( *this ) );
	}
	decal->GetParentComponent()->AddListener( ( deoglComponentListener* )( deObject* )pDecalComponentListener );
	
	if( dynamic ){
// 		decal.PrepareGIDynamicBVH();
// 		pGIBVHDynamic = decal.GetGIBVHDynamic();
// 		if( ! pGIBVHDynamic ){
// 			return;
// 		}
// 		
// 		pGIBVHDynamic->AddBlockUsage();
		
	}else{
		decal->PrepareGILocalBVH();
		pGIBVHLocal = decal->GetGIBVHLocal();
		if( ! pGIBVHLocal ){
			return;
		}
		
		pGIBVHLocal->AddBlockUsage();
		SetExtendsFromBVHLocal();
	}
	
	pInitParameters();
	pDirtyTUCs = true;
}

void deoglGIInstance::SetExtends( const decDVector &minExtend, const decDVector &maxExtend ){
	pMinExtend = minExtend;
	pMaxExtend = maxExtend;
}

void deoglGIInstance::SetExtendsFromBVHLocal(){
	pMinExtend.SetZero();
	pMaxExtend.SetZero();
	
	if( ! pGIBVHLocal || ! pGIBVHLocal->GetTBOVertex() ){
		return;
	}
	
	decDMatrix matrix;
	if( pComponent ){
		matrix = pComponent->GetMatrix();
		
	}else if( pDecal ){
		if( ! pDecal->GetParentComponent() ){
			return;
		}
		matrix = pDecal->GetParentComponent()->GetMatrix();
		
	}else{
		return;
	}
	
	const deoglDynamicTBOFloat32 &tboVertex = *pGIBVHLocal->GetTBOVertex();
	const int count = tboVertex.GetPixelCount();
	if( count == 0 ){
		return;
	}
	
	const float *data = tboVertex.GetDataFloat();
	pMinExtend.Set( data[ 0 ], data[ 1 ], data[ 2 ] );
	pMaxExtend = pMinExtend;
	
	int i;
	for( i=1, data+=4; i<count; i++, data+=4 ){
		const decDVector position( matrix.Transform( data[ 0 ], data[ 1 ], data[ 2 ] ) );
		pMinExtend.SetSmallest( position );
		pMaxExtend.SetLargest( position );
	}
}

void deoglGIInstance::UpdateBVHExtends(){
	if( pGIBVHDynamic ){
		pBVHMinExtend = pGIBVHDynamic->GetMinimumExtend();
		pBVHMaxExtend = pGIBVHDynamic->GetMaximumExtend();
	}
}

void deoglGIInstance::SetDynamic( bool dynamic ){
	if( dynamic == pDynamic ){
		return;
	}
	
	pDynamic = dynamic;
	pDirtyTUCs = true;
}

void deoglGIInstance::SetChanged( bool changed ){
	pChanged = changed;
}

void deoglGIInstance::SetHardChanged( bool changed ){
	pHardChanged = changed;
}

void deoglGIInstance::ClearChanged(){
	pChanged = false;
	pHardChanged = false;
}

void deoglGIInstance::SetMoved( bool moved ){
	pMoved = moved;
}

void deoglGIInstance::SetRecheckDynamic( bool recheckDynamic ){
	pRecheckDynamic = recheckDynamic;
}

bool deoglGIInstance::Empty() const{
	return pComponent == NULL && pDecal == NULL;
}

void deoglGIInstance::Clear(){
	DropBlockMaterial();
	RemoveAllTUCs();
	pDirtyTUCs = false;
	
	if( pGIBVHLocal ){
		pGIBVHLocal->RemoveBlockUsage();
		pGIBVHLocal = NULL;
	}
	
	if( pGIBVHDynamic ){
		pGIBVHDynamic->RemoveBlockUsage();
		pGIBVHDynamic = NULL;
	}
	
	pIndexNodes = 0;
	pIndexFaces = 0;
	pIndexVertices = 0;
	pBVHMinExtend.SetZero();
	pBVHMaxExtend.SetZero();
	pHasBVHNodes = false;
	
	pDynamic = false;
	pRecheckDynamic = false;
	
	if( pComponent ){
		pInstances.UnregisterElement( pComponent );
		
		pComponent->RemoveListener( ( deoglComponentListener* )( deObject* )pComponentListener );
		pComponent = NULL;
		
	}else if( pDecal ){
		pDecal->RemoveListener( ( deoglDecalListener* )( deObject* )pDecalListener );
		if( pDecal->GetParentComponent() ){
			pDecal->GetParentComponent()->RemoveListener( ( deoglComponentListener* )( deObject* )pDecalComponentListener );
		}
		pDecal = NULL;
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

deoglDynamicTBOBlock *deoglGIInstance::GetBlockMaterial(){
	if( ! pBlockMaterial ){
		pBlockMaterial.TakeOver( pInstances.GetGIState().GetRenderThread().GetGI().GetBVHShared()
			.GetSharedTBOMaterial()->AddBlock( pTBOMaterial, pTBOMaterial2 ) );
	}
	return ( deoglDynamicTBOBlock* )( deObject* )pBlockMaterial;
}

void deoglGIInstance::DropBlockMaterial(){
	if( pBlockMaterial ){
		( ( deoglDynamicTBOBlock* )( deObject* )pBlockMaterial )->Drop();
		pBlockMaterial = NULL;
	}
}



// Private Functions
//////////////////////

void deoglGIInstance::pCleanUp(){
	Clear();
	
	if( pTBOMaterial ){
		pTBOMaterial->FreeReference();
	}
	if( pTBOMaterial2 ){
		pTBOMaterial2->FreeReference();
	}
}

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
			pBVHMinExtend = rootNode->GetMinExtend();
			pBVHMaxExtend = rootNode->GetMaxExtend();
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
		pBVHMinExtend = pGIBVHDynamic->GetMinimumExtend();
		pBVHMaxExtend = pGIBVHDynamic->GetMaximumExtend();
	}
}
