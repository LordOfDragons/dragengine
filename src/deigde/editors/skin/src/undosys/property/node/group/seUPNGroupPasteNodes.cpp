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
#include <string.h>
#include <stdlib.h>

#include "seUPNGroupPasteNodes.h"
#include "../../../../skin/property/seProperty.h"
#include "../../../../skin/property/node/sePropertyNodeGroup.h"
#include "../../../../clipboard/seClipboardDataPropertyNode.h"

#include <dragengine/common/exceptions.h>



// Class seUPNGroupPasteNodes
///////////////////////////////

// Constructor, destructor
////////////////////////////

seUPNGroupPasteNodes::seUPNGroupPasteNodes( sePropertyNodeGroup *group,
int layer, const seClipboardDataPropertyNode &clipboardData ) :
pGroup( NULL ),
pLayer( layer )
{
	if( ! group || ! group->GetProperty() ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = clipboardData.GetCount();
	if( count == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Node group paste nodes" );
	
	sePropertyNode *pasteNode = NULL;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			pasteNode = clipboardData.GetAt( i ).Copy();
			
			decPoint3 position( pasteNode->GetPosition() );
			position.z = layer;
			pasteNode->SetPosition( position );
			
			pNodes.Add( pasteNode );
			pasteNode->FreeReference();
			pasteNode = NULL;
		}
		
		pGroup = group;
		group->AddReference();
		
	}catch( const deException & ){
		if( pasteNode ){
			pasteNode->FreeReference();
		}
		pNodes.RemoveAll();
		throw;
	}
}

seUPNGroupPasteNodes::~seUPNGroupPasteNodes(){
	if( pGroup ){
		pGroup->FreeReference();
	}
}



// Management
///////////////

void seUPNGroupPasteNodes::Undo(){
	const int count = pNodes.GetCount();
	int i;
	
	seProperty * const property = pGroup->GetProperty();
	if( property && property->GetActiveNodeGroup() == pGroup ){
		property->GetNodeSelection().RemoveAll();
	}
	
	for( i=0; i<count; i++ ){
		pGroup->RemoveNode( ( sePropertyNode* )pNodes.GetAt( i ) );
	}
}

void seUPNGroupPasteNodes::Redo(){
	const int count = pNodes.GetCount();
	int i;
	
	seProperty * const property = pGroup->GetProperty();
	if( property && property->GetActiveNodeGroup() == pGroup ){
		property->GetNodeSelection().RemoveAll();
	}
	
	for( i=0; i<count; i++ ){
		pGroup->AddNode( ( sePropertyNode* )pNodes.GetAt( i ) );
	}
	
	if( property && property->GetActiveNodeGroup() == pGroup ){
		property->GetNodeSelection().SetSelected( pNodes );
	}
}
