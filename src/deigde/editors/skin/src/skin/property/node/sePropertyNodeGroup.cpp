/* 
 * Drag[en]gine IGDE Skin Editor
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
sePropertyNode( entGroup, engine ),
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
