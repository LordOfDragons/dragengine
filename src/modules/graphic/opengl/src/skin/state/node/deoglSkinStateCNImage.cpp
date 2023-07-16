/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "deoglSkinStateCNImage.h"
#include "../deoglSkinState.h"
#include "../../../texture/deoglImage.h"
#include "../../../rendering/deoglRenderCanvasContext.h"
#include "../../../rendering/deoglRenderConstructed.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTRenderers.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeImage.h>


// Class deoglSkinStateCNImage
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinStateCNImage::deoglSkinStateCNImage( deSkinPropertyNodeImage &node ) :
deoglSkinStateConstructedNode( node, etImage ),
pSyncImage( node.GetImage() ),
pImage( pSyncImage && pSyncImage->GetPeerGraphic()
	? ( ( deoglImage* )pSyncImage->GetPeerGraphic() )->GetRImage() : nullptr ),
pRepeat( node.GetRepeat() ),
pDirtyTCTransform( true ){
}

deoglSkinStateCNImage::deoglSkinStateCNImage( const deoglSkinStateCNImage &node ) :
deoglSkinStateConstructedNode( node ),
pSyncImage( node.pSyncImage ),
pImage( node.pImage ),
pRepeat( node.pRepeat ),
pTCTransform( node.pTCTransform ),
pDirtyTCTransform( true ){
}

deoglSkinStateCNImage::~deoglSkinStateCNImage(){
}



// Management
///////////////

void deoglSkinStateCNImage::Update( deoglSkinState &state ){
	if( ! pImage ){
		return;
	}
	
	deoglSkinStateConstructedNode::Update( state );
	
	pUpdateTCTransform();
	
	deoglImage * const peer = ( deoglImage* )pSyncImage->GetPeerGraphic();
	if( peer ){
		peer->SyncToRender();
	}
}

void deoglSkinStateCNImage::PrepareForRender( deoglSkinState &state ){
	if( ! pImage ){
		return;
	}
	
	deoglSkinStateConstructedNode::PrepareForRender( state );
	
	pImage->PrepareForRender();
}

void deoglSkinStateCNImage::Render( deoglSkinState &state, const deoglRenderCanvasContext &context ){
	if( ! pImage ){
		return;
	}
	
	deoglSkinStateConstructedNode::Render( state, context );
	
	deoglRenderCanvasContext imageContext( context, *this );
	imageContext.SetTCClampMinimum( pTCClampMin );
	imageContext.SetTCClampMaximum( pTCClampMax );
	state.GetRenderThread().GetRenderers().GetConstructed().DrawNodeImage( imageContext, *this );
}

deoglSkinStateConstructedNode::Ref deoglSkinStateCNImage::Copy() const{
	return deoglSkinStateConstructedNode::Ref::New( new deoglSkinStateCNImage( *this ) );
}



// Private Functions
//////////////////////

void deoglSkinStateCNImage::pUpdateTCTransform(){
	if( ! pDirtyTCTransform ){
		return;
	}
	
	pDirtyTCTransform = false;
	
	const decVector2 halfPixel( 0.5f / ( float )pImage->GetWidth(), 0.5f / ( float )pImage->GetHeight() );
	
	const float repeatScaleU = ( float )pRepeat.x;
	const float repeatScaleV = ( float )pRepeat.y;
	
	pTCTransform = decTexMatrix2::CreateScale( repeatScaleU, repeatScaleV );
	pTCClampMin = halfPixel;
	pTCClampMax = decVector2( repeatScaleU, repeatScaleV ) - halfPixel;
}
