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
	return deoglSkinStateConstructedNode::Ref::NewWith(*this);
}
