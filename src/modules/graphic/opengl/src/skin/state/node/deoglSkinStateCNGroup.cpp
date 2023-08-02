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

#include "deoglSkinStateCNGroup.h"
#include "../../../rendering/deoglRenderCanvasContext.h"
#include "../../../rendering/deoglRenderConstructed.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTRenderers.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>


// Class deoglSkinStateCNGroup
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinStateCNGroup::deoglSkinStateCNGroup( deSkinPropertyNodeGroup &node ) :
deoglSkinStateConstructedNode( node, etGroup )
{
	const int count = node.GetNodeCount();
	int i;
	for( i=0; i<count; i++ ){
		pNodes.Add( CreateNode( *node.GetNodeAt( i ) ) );
	}
}

deoglSkinStateCNGroup::deoglSkinStateCNGroup( const deoglSkinStateCNGroup &node ) :
deoglSkinStateConstructedNode( node )
{
	const int count = node.pNodes.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pNodes.Add( ( ( deoglSkinStateConstructedNode* )node.pNodes.GetAt( i ) )->Copy() );
	}
}

deoglSkinStateCNGroup::~deoglSkinStateCNGroup(){
}



// Management
///////////////

int deoglSkinStateCNGroup::GetNodeCount() const{
	return pNodes.GetCount();
}

deoglSkinStateConstructedNode *deoglSkinStateCNGroup::GetNodeAt( int index ) const{
	return ( deoglSkinStateConstructedNode* )pNodes.GetAt( index );
}



void deoglSkinStateCNGroup::Update( deoglSkinState &state ){
	const int count = pNodes.GetCount();
	if( count == 0 ){
		return;
	}
	
	deoglSkinStateConstructedNode::Update( state );
	
	int i;
	for( i=0; i<count; i++ ){
		( ( deoglSkinStateConstructedNode* )pNodes.GetAt( i ) )->Update( state );
	}
}

void deoglSkinStateCNGroup::PrepareForRender( deoglSkinState &state ){
	const int count = pNodes.GetCount();
	if( count == 0 ){
		return;
	}
	
	deoglSkinStateConstructedNode::PrepareForRender( state );
	
	int i;
	for( i=0; i<count; i++ ){
		( ( deoglSkinStateConstructedNode* )pNodes.GetAt( i ) )->PrepareForRender( state );
	}
}

void deoglSkinStateCNGroup::Render( deoglSkinState &state, const deoglRenderCanvasContext &context ){
	const int count = pNodes.GetCount();
	if( count == 0 ){
		return;
	}
	
	const deoglRenderCanvasContext childContext( context, *this );
	if( childContext.IsZeroClip() ){
		return;
	}
	
	int i;
	for( i=0; i<count; i++ ){
		( ( deoglSkinStateConstructedNode* )pNodes.GetAt( i ) )->Render( state, childContext );
	}
}

deoglSkinStateConstructedNode::Ref deoglSkinStateCNGroup::Copy() const{
	return deoglSkinStateConstructedNode::Ref::New( new deoglSkinStateCNGroup( *this ) );
}
