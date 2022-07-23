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

#include "deoglBillboard.h"
#include "deoglRBillboard.h"
#include "../deGraphicOpenGl.h"
#include "../skin/deoglSkin.h"
#include "../skin/dynamic/deoglDynamicSkin.h"
#include "../skin/state/deoglSkinStateController.h"
#include "../world/deoglWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/billboard/deBillboard.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>



// Class deoglBillboard
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglBillboard::deoglBillboard( deGraphicOpenGl &ogl, const deBillboard &billboard ) :
pOgl( ogl ),
pBillboard( billboard ),
pRBillboard( NULL ),

pParentWorld( NULL ),
pSkinStateController( NULL ),
pDynamicSkin( NULL ),

pAccumUpdate( 0.0f ),

pDirtyGeometry( true ),
pDirtyOctreeNode( true ),
pDirtyExtends( true ),
pDirtyLayerMask( true ),
pDirtySkinStateController( true ),
pDirtyCullSphere( true ),
pDirtyRenderEnvMap( true ),
pDirtySkin( true ),
pDirtyDynamicSkin( true ),
pDirtyRenderableMapping( true ),
pDirtySkinStateCalculatedProperties( true ),
pSkinStatePrepareRenderables( true ),

pDynamicSkinRenderablesChanged( true ),
pDynamicSkinRequiresSync( true ),
pRequiresUpdateEverySync( false ),

pLLSyncWorld( this )
{
	try{
		pRBillboard = new deoglRBillboard( ogl.GetRenderThread() );
		pSkinStateController = new deoglSkinStateController;
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglBillboard::~deoglBillboard(){
	pCleanUp();
}



// Management
///////////////

void deoglBillboard::SyncToRender(){
	pSyncSkin();
	pSyncDynamicSkin();
	
	if( pDirtyGeometry ){
		pRBillboard->SetPosition( pBillboard.GetPosition() );
		pRBillboard->SetAxis( pBillboard.GetAxis() );
		pRBillboard->SetSize( pBillboard.GetSize() );
		pRBillboard->SetOffset( pBillboard.GetOffset() );
		pRBillboard->SetLocked( pBillboard.GetLocked() );
		pRBillboard->SetSpherical( pBillboard.GetSpherical() );
		pRBillboard->SetSizeFixedToScreen( pBillboard.GetSizeFixedToScreen() );
		pRBillboard->MarkParamBlocksDirty();
		pDirtyGeometry = false;
	}
	if( pDirtyLayerMask ){
		pRBillboard->SetLayerMask( pBillboard.GetLayerMask() );
		pDirtyLayerMask = false;
	}
	if( pDirtyCullSphere ){
		pRBillboard->SetDirtyCulling();
		pDirtyCullSphere = false;
	}
	if( pDirtyRenderEnvMap ){
		pRBillboard->InvalidateRenderEnvMap();
		pDirtyRenderEnvMap = false;
	}
	if( pDirtyRenderableMapping ){
		pRBillboard->UpdateRenderableMapping();
		pDirtyRenderableMapping = false;
	}
	
	// sync skin state controller
	if( pDirtySkinStateController ){
		pSkinStateController->Init( *pRBillboard->GetSkinState(), pRBillboard->GetSkin(), pParentWorld );
		pDirtySkinStateController = false;
	}
	
	// accumulated times
	const float skinUpdate = pAccumUpdate;
	
	pSkinStateController->AdvanceTime( skinUpdate );
	pSkinStateController->SyncToRender();
	
	pRBillboard->UpdateSkin( skinUpdate );
	
	pAccumUpdate = 0.0f;
	pCheckRequiresUpdateEverySync();
	
	// sync calculated skin state properties. has to come after pSkinStateController->SyncToRender()
	// and pRBillboard->UpdateSkin()
	if( pDirtySkinStateCalculatedProperties ){
		pRBillboard->InitSkinStateCalculatedProperties();
		pDirtySkinStateCalculatedProperties = false;
	}
	if( pSkinStatePrepareRenderables ){
		pRBillboard->DirtyPrepareSkinStateRenderables();
		pSkinStatePrepareRenderables = false;
	}
	
	pRBillboard->UpdateSkinStateCalculatedProperties(); // has to be done better. only some need this
	
	// octree, extends and matrices. order is important
	if( pDirtyExtends ){
		pRBillboard->UpdateExtends( pBillboard );
		pDirtyExtends = false;
	}
	if( pDirtyOctreeNode ){
		pRBillboard->SetVisible( pBillboard.GetVisible() );
		pRBillboard->UpdateOctreeNode();
		pDirtyOctreeNode = false;
	}
}

void deoglBillboard::Update( float elapsed ){
	pAccumUpdate += elapsed;
	
	if( pRequiresUpdateEverySync ){
		pRequiresSync();
	}
}



void deoglBillboard::SetParentWorld( deoglWorld *parentWorld ){
	if( parentWorld == pParentWorld ){
		return;
	}
	
	if( pSkinStateController ){
		pSkinStateController->Clear();
	}
	
	pParentWorld = parentWorld;
	
	pDirtyOctreeNode = true;
	pDirtySkinStateController = true;
	pDirtyRenderEnvMap = true;
}



// Dynamic skin listener
//////////////////////////

void deoglBillboard::DynamicSkinDestroyed(){
	pDynamicSkin = NULL;
}

void deoglBillboard::DynamicSkinRenderablesChanged(){
	pDynamicSkinRenderablesChanged = true;
	pDynamicSkinRequiresSync = true;
	pDirtyRenderableMapping = true;
// 	pDirtyStaticTexture = true;
	pSkinStatePrepareRenderables = true;
// 	pNotifyTexturesChanged = true;
// 	pDirtySolid = true;
	
	pRequiresSync();
}

void deoglBillboard::DynamicSkinRenderableChanged( deoglDSRenderable& ){
	pDynamicSkinRenderablesChanged = true;
	pDynamicSkinRequiresSync = true;
// 	pDirtyStaticTexture = true;
	pSkinStatePrepareRenderables = true;
// 	pNotifyTexturesChanged = true;
// 	pDirtySolid = true;
	
	pRequiresSync();
}

void deoglBillboard::DynamicSkinRenderableRequiresSync( deoglDSRenderable& ){
	pDynamicSkinRequiresSync = true;
	pSkinStatePrepareRenderables = true;
	
	pRequiresSync();
}



// Notifications
//////////////////

void deoglBillboard::PositionChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	pDirtyExtends = true;
	pDirtyCullSphere = true;
	pDirtyRenderEnvMap = true;
	
	pRequiresSync();
}

void deoglBillboard::AxisChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	pDirtyExtends = true;
	pDirtyCullSphere = true;
	pDirtyRenderEnvMap = true;
	
	pRequiresSync();
}

void deoglBillboard::SizeChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	pDirtyExtends = true;
	pDirtyCullSphere = true;
	pDirtyRenderEnvMap = true;
	
	pRequiresSync();
}

void deoglBillboard::OffsetChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	pDirtyExtends = true;
	pDirtyCullSphere = true;
	pDirtyRenderEnvMap = true;
	
	pRequiresSync();
}

void deoglBillboard::SkinChanged(){
	pDirtySkin = true;
	pDirtySkinStateController = true;
	pDirtySkinStateCalculatedProperties = true;
	pDirtyRenderableMapping = true;
	pSkinStatePrepareRenderables = true;
	
	pRequiresSync();
}

void deoglBillboard::DynamicSkinChanged(){
	if( pDynamicSkin ){
		pDynamicSkin->RemoveListener( this );
	}
	
	if( pBillboard.GetDynamicSkin() ){
		pDynamicSkin = ( deoglDynamicSkin* )pBillboard.GetDynamicSkin()->GetPeerGraphic();
		pDynamicSkin->AddListener( this );
		
	}else{
		pDynamicSkin = NULL;
	}
	
	pDirtyDynamicSkin = true;
	pDynamicSkinRequiresSync = true;
	pDynamicSkinRenderablesChanged = true;
	pDirtyRenderableMapping = true;
	pSkinStatePrepareRenderables = true;
	
	pRequiresSync();
}

void deoglBillboard::ParametersChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	pDirtyExtends = true;
	pDirtyCullSphere = true;
	pDirtyRenderEnvMap = true;
	
	pRequiresSync();
}

void deoglBillboard::VisibilityChanged(){
	pDirtyOctreeNode = true;
	pDirtyExtends = true;
	
	pRequiresSync();
}

void deoglBillboard::LayerMaskChanged(){
	pDirtyLayerMask = true;
	
	pRequiresSync();
}



// Private Functions
//////////////////////

void deoglBillboard::pCleanUp(){
	if( pSkinStateController ){
		delete pSkinStateController;
	}
	if( pRBillboard ){
		pRBillboard->FreeReference();
	}
	if( pDynamicSkin ){
		pDynamicSkin->RemoveListener( this );
	}
}



void deoglBillboard::pSyncSkin(){
	if( ! pDirtySkin ){
		return;
	}
	
	if( pBillboard.GetSkin() ){
		pRBillboard->SetSkin( ( ( deoglSkin* )pBillboard.GetSkin()->GetPeerGraphic() )->GetRSkin() );
		
	}else{
		pRBillboard->SetSkin( NULL );
	}
	
	pDirtySkin = false;
}

void deoglBillboard::pSyncDynamicSkin(){
	if( pDirtyDynamicSkin ){
		if( pDynamicSkin ){
			pRBillboard->SetDynamicSkin( pDynamicSkin->GetRDynamicSkin() );
			
		}else{
			pRBillboard->SetDynamicSkin( NULL );
		}
		
		pDirtyDynamicSkin = false;
	}
	
	if( pDynamicSkinRenderablesChanged ){
		pDynamicSkinRenderablesChanged = false;
		pRBillboard->DynamicSkinRenderablesChanged();
	}
	
	if( pDynamicSkinRequiresSync ){
		pDynamicSkinRequiresSync = false;
		if( pDynamicSkin ){
			pDynamicSkin->SyncToRender();
		}
	}
}

void deoglBillboard::pCheckRequiresUpdateEverySync(){
	if( pSkinStateController->RequiresSyncEveryFrameUpdate() ){
		pRequiresUpdateEverySync = true;
		if( pSkinStateController->RequiresPrepareRenderables() ){
			pRBillboard->DirtyPrepareSkinStateRenderables();
		}
	}
}



void deoglBillboard::pRequiresSync(){
	if( ! pLLSyncWorld.GetList() && pParentWorld ){
		pParentWorld->AddSyncBillboard( this );
	}
}
