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

#include "deoglDynamicSkin.h"
#include "deoglDynamicSkinListener.h"
#include "deoglRDynamicSkin.h"
#include "renderables/deoglDSRenderable.h"
#include "renderables/deoglDSRenderableCamera.h"
#include "renderables/deoglDSRenderableCanvas.h"
#include "renderables/deoglDSRenderableColor.h"
#include "renderables/deoglDSRenderableImage.h"
#include "renderables/deoglDSRenderableValue.h"
#include "renderables/deoglDSRenderableVideoFrame.h"
#include "renderables/render/deoglRDSRenderable.h"
#include "../../deGraphicOpenGl.h"
#include "../../billboard/deoglBillboard.h"
#include "../../component/deoglComponent.h"
#include "../../component/deoglComponentTexture.h"
#include "../../decal/deoglDecal.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderable.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableVisitorIdentify.h>



// Class deoglDynamicSkin
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglDynamicSkin::deoglDynamicSkin( deGraphicOpenGl &ogl, const deDynamicSkin &dynamicSkin ) :
pOgl( ogl ),
pDynamicSkin( dynamicSkin ),
pRDynamicSkin( NULL ),
pDirtyRenderables( true )
{
	const int renderableCount = dynamicSkin.GetRenderableCount();
	int i;
	
	try{
		pRDynamicSkin = new deoglRDynamicSkin( ogl.GetRenderThread() );
		
		for( i=0; i<renderableCount; i++ ){
			RenderableAdded( i, dynamicSkin.GetRenderableAt( i ) );
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglDynamicSkin::~deoglDynamicSkin(){
	pCleanUp();
}



// Management
///////////////

int deoglDynamicSkin::GetRenderableCount() const{
	return pRenderables.GetCount();
}

deoglDSRenderable *deoglDynamicSkin::GetRenderableAt( int index ) const{
	return ( deoglDSRenderable* )pRenderables.GetAt( index );
}



void deoglDynamicSkin::SyncToRender(){
	const int count = pRenderables.GetCount();
	int i;
	
	if( pDirtyRenderables ){
		pRDynamicSkin->RemoveAllRenderables();
		
		for( i=0; i<count; i++ ){
			deoglDSRenderable &renderable = *( ( deoglDSRenderable* )pRenderables.GetAt( i ) );
			renderable.SyncToRender();
			pRDynamicSkin->AddRenderable( renderable.GetRRenderable() );
		}
		
		pDirtyRenderables = false;
	}
	
	for( i=0; i<count; i++ ){
		( ( deoglDSRenderable* )pRenderables.GetAt( i ) )->SyncToRender();
	}
}



// Listeners
//////////////

void deoglDynamicSkin::AddListener( deoglDynamicSkinListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void deoglDynamicSkin::RemoveListener( deoglDynamicSkinListener *listener ){
	pListeners.RemoveIfPresent( listener );
}

void deoglDynamicSkin::NotifyDestroyed(){
	const int count = pListeners.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( deoglDynamicSkinListener* )pListeners.GetAt( i ) )->DynamicSkinDestroyed();
	}
}

void deoglDynamicSkin::NotifyRenderablesChanged(){
	const int count = pListeners.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( deoglDynamicSkinListener* )pListeners.GetAt( i ) )->DynamicSkinRenderablesChanged();
	}
}

void deoglDynamicSkin::NotifyRenderableChanged( deoglDSRenderable &renderable ){
	const int count = pListeners.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( deoglDynamicSkinListener* )pListeners.GetAt( i ) )->DynamicSkinRenderableChanged( renderable );
	}
}

void deoglDynamicSkin::NotifyRenderableRequiresSync( deoglDSRenderable &renderable ){
	const int count = pListeners.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( deoglDynamicSkinListener* )pListeners.GetAt( i ) )->DynamicSkinRenderableRequiresSync( renderable );
	}
}



// Notifications
//////////////////

void deoglDynamicSkin::RenderableAdded( int, deDSRenderable *renderable ){
	if( ! renderable ){
		DETHROW( deeInvalidParam );
	}
	
	deDSRenderableVisitorIdentify identify;
	renderable->Visit( identify );
	
	switch( identify.GetType() ){
	case deDSRenderableVisitorIdentify::eptCamera:
		pRenderables.Add( new deoglDSRenderableCamera( *this, identify.CastToCamera() ) );
		break;
		
	case deDSRenderableVisitorIdentify::eptCanvas:
		pRenderables.Add( new deoglDSRenderableCanvas( *this, identify.CastToCanvas() ) );
		break;
		
	case deDSRenderableVisitorIdentify::eptColor:
		pRenderables.Add( new deoglDSRenderableColor( *this, identify.CastToColor() ) );
		break;
		
	case deDSRenderableVisitorIdentify::eptImage:
		pRenderables.Add( new deoglDSRenderableImage( *this, identify.CastToImage() ) );
		break;
		
	case deDSRenderableVisitorIdentify::eptValue:
		pRenderables.Add( new deoglDSRenderableValue( *this, identify.CastToValue() ) );
		break;
		
	case deDSRenderableVisitorIdentify::eptVideoFrame:
		pRenderables.Add( new deoglDSRenderableVideoFrame( *this, identify.CastToVideoFrame() ) );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
	
	pDirtyRenderables = true;
	NotifyRenderablesChanged();
}

void deoglDynamicSkin::RenderableChanged( int index, deDSRenderable *renderable ){
	( ( deoglDSRenderable* )pRenderables.GetAt( index ) )->RenderableChanged();
}

void deoglDynamicSkin::RenderableRemoved( int index, deDSRenderable* ){
	delete ( deoglDSRenderable* )pRenderables.GetAt( index );
	pRenderables.RemoveFrom( index );
	
	pDirtyRenderables = true;
	NotifyRenderablesChanged();
}

void deoglDynamicSkin::AllRenderablesRemoved(){
	const int count = pRenderables.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delete ( deoglDSRenderable* )pRenderables.GetAt( i );
	}
	pRenderables.RemoveAll();
	
	pDirtyRenderables = true;
	NotifyRenderablesChanged();
}



// Private Functions
//////////////////////

void deoglDynamicSkin::pCleanUp(){
	int i, count = pRenderables.GetCount();
	for( i=0; i<count; i++ ){
		delete ( deoglDSRenderable* )pRenderables.GetAt( i );
	}
	pRenderables.RemoveAll();
	
	if( pRDynamicSkin ){
		pRDynamicSkin->FreeReference();
	}
	
	// notify owners we are about to be deleted. required since owners hold only a weak pointer
	// to the dynamic skin and are notified only after switching to a new dynamic skin. in this
	// case they can not use the old pointer to remove themselves from the dynamic skin
	NotifyDestroyed();
}
