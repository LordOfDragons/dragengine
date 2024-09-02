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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sePropertyNodeGroup.h"
#include "../seProperty.h"
#include "../../seSkin.h"
#include "../../texture/seTexture.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>



// Class sePropertyNodeGroup
//////////////////////////////

// Constructor, destructor
////////////////////////////

sePropertyNodeGroup::sePropertyNodeGroup( deEngine &engine ) :
sePropertyNode( entGroup, engine, MappedCount ),
pProperty( NULL ),
pActiveGroup( false ){
}

sePropertyNodeGroup::sePropertyNodeGroup( const sePropertyNodeGroup &node ) :
sePropertyNode( node ),
pProperty( NULL ),
pActiveGroup( false )
{
	const int count = node.pNodes.GetCount();
	sePropertyNode *childNode = NULL;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			childNode = ( ( sePropertyNode* )node.pNodes.GetAt( i ) )->Copy();
			pNodes.Add( childNode );
			childNode->SetParent( this );
			childNode = NULL;
		}
		
	}catch( const deException & ){
		if( childNode ){
			childNode->FreeReference();
		}
		pNodes.RemoveAll();
		throw;
	}
}

sePropertyNodeGroup::~sePropertyNodeGroup(){
}



// Management
///////////////

seProperty *sePropertyNodeGroup::GetProperty() const{
	if( pProperty ){
		return pProperty;
		
	}else{
		return sePropertyNode::GetProperty();
	}
}

void sePropertyNodeGroup::SetProperty( seProperty *property ){
	if( property == pProperty ){
		return;
	}
	
	pProperty = property;
	UpdateResources();
}

void sePropertyNodeGroup::SetActiveGroup( bool active ){
	pActiveGroup = active;
}



int sePropertyNodeGroup::GetNodeCount() const{
	return pNodes.GetCount();
}

sePropertyNode *sePropertyNodeGroup::GetNodeAt( int index ) const{
	return ( sePropertyNode* )pNodes.GetAt( index );
}

bool sePropertyNodeGroup::HasNode( sePropertyNode *node ) const{
	return pNodes.Has( node );
}

int sePropertyNodeGroup::IndexOfNode( sePropertyNode *node ) const{
	return pNodes.IndexOf( node );
}

void sePropertyNodeGroup::AddNode( sePropertyNode *node ){
	if( ! node || node->GetProperty() ){
		DETHROW( deeInvalidParam );
	}
	
	pNodes.Add( node );
	node->SetParent( this );
	
	NotifyStructreChanged();
}

void sePropertyNodeGroup::InsertNode( int index, sePropertyNode *node ){
	if( ! node || node->GetProperty() ){
		DETHROW( deeInvalidParam );
	}
	
	pNodes.Insert( node, index );
	node->SetParent( this );
	
	NotifyStructreChanged();
}

void sePropertyNodeGroup::RemoveNode( sePropertyNode *node ){
	seProperty * const property = GetProperty();
	if( property ){
		if( property->GetNodeSelection().GetActive() == node ){
			property->GetNodeSelection().SetActive( NULL );
		}
	}
	
	if( ! pNodes.Has( node ) ){
		DETHROW( deeInvalidParam );
	}
	
	node->SetParent( NULL );
	pNodes.Remove( node );
	
	NotifyStructreChanged();
}

void sePropertyNodeGroup::RemoveAllNodes(){
	seProperty * const property = GetProperty();
	const int count = pNodes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		sePropertyNode * const node = ( sePropertyNode* )pNodes.GetAt( i );
		
		if( property && property->GetNodeSelection().GetActive() == node ){
			property->GetNodeSelection().SetActive( NULL );
		}
		
		node->SetParent( NULL );
	}
	
	pNodes.RemoveAll();
	
	NotifyStructreChanged();
}

void sePropertyNodeGroup::MoveNode( sePropertyNode *node, int index ){
	pNodes.Move( node, index );
	NotifyStructreChanged();
}



sePropertyNode *sePropertyNodeGroup::Copy() const{
	return new sePropertyNodeGroup( *this );
}

void sePropertyNodeGroup::UpdateResources(){
	sePropertyNode::UpdateResources();
	
	const int count = pNodes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( sePropertyNode* )pNodes.GetAt( i ) )->UpdateResources();
	}
}
