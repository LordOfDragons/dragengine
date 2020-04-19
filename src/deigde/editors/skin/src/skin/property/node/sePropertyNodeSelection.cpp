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

#include "sePropertyNode.h"
#include "sePropertyNodeSelection.h"
#include "../seProperty.h"
#include "../../texture/seTexture.h"
#include "../../seSkin.h"

#include <dragengine/common/exceptions.h>



// Class sePropertyNodeSelection
//////////////////////////////////

// Constructor, destructor
////////////////////////////

sePropertyNodeSelection::sePropertyNodeSelection( seProperty &property ) :
pProperty( property ),
pActive( NULL ){
}

sePropertyNodeSelection::~sePropertyNodeSelection(){
}



// Management
///////////////

void sePropertyNodeSelection::Add( sePropertyNode *node ){
	if( ! node ){
		DETHROW( deeInvalidParam );
	}
	
	if( pSelection.Has( node ) ){
		return;
	}
	
	node->SetSelected( true );
	pSelection.Add( node );
	NotifyNodeSelectionChanged();
	
	if( ! pActive ){
		SetActive( node );
	}
}

void sePropertyNodeSelection::Remove( sePropertyNode *node ){
	if( ! node ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pSelection.Has( node ) ){
		return;
	}
	
	if( pActive == node ){
		if( pSelection.GetCount() > 1 ){
			const int index = pSelection.IndexOf( node );
			if( index == pSelection.GetCount() - 1 ){
				SetActive( pSelection.GetAt( index - 1 ) );
				
			}else{
				SetActive( pSelection.GetAt( index + 1 ) );
			}
			
		}else{
			SetActive( NULL );
		}
	}
	
	node->SetSelected( false );
	pSelection.Remove( node );
	NotifyNodeSelectionChanged();
}

void sePropertyNodeSelection::RemoveAll(){
	if( pSelection.GetCount() == 0 ){
		return;
	}
	
	SetActive( NULL );
	
	const int count = pSelection.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pSelection.GetAt( i )->SetSelected( false );
	}
	pSelection.RemoveAll();
	NotifyNodeSelectionChanged();
}

void sePropertyNodeSelection::SetSelected( const sePropertyNodeList &list ){
	const int count = list.GetCount();
	if( count == pSelection.GetCount() ){
		int i;
		for( i=0; i<count; i++ ){
			if( ! pSelection.Has( list.GetAt( i ) ) ){
				break;
			}
		}
		
		if( i == count ){
			return; // same nodes no matter the order
		}
	}
	
	if( ! list.Has( pActive ) ){
		SetActive( NULL );
	}
	
	const int clearCount = pSelection.GetCount();
	int i;
	for( i=0; i<clearCount; i++ ){
		pSelection.GetAt( i )->SetSelected( false );
	}
	
	pSelection = list;
	
	for( i=0; i<count; i++ ){
		pSelection.GetAt( i )->SetSelected( true );
	}
	
	NotifyNodeSelectionChanged();
	
	if( pSelection.GetCount() > 0 ){
		SetActive( pSelection.GetAt( 0 ) );
	}
}



bool sePropertyNodeSelection::HasActive() const{
	return pActive != NULL;
}

void sePropertyNodeSelection::SetActive( sePropertyNode *node ){
	if( node == pActive ){
		return;
	}
	
	if( pActive ){
		pActive->SetActive( false );
		pActive->FreeReference();
	}
	
	pActive = node;
	
	if( node ){
		node->AddReference();
		node->SetActive( true );
	}
	
	NotifyActiveNodeChanged();
}



void sePropertyNodeSelection::NotifyNodeSelectionChanged() const{
	if( ! pProperty.GetTexture() ){
		return;
	}
	
	seTexture * const texture = pProperty.GetTexture();
	if( ! texture->GetSkin() ){
		return;
	}
	
	texture->GetSkin()->NotifyPropertyNodeSelectionChanged( texture, &pProperty );
}

void sePropertyNodeSelection::NotifyActiveNodeChanged() const{
	if( ! pProperty.GetTexture() ){
		return;
	}
	
	seTexture * const texture = pProperty.GetTexture();
	if( ! texture->GetSkin() ){
		return;
	}
	
	texture->GetSkin()->NotifyPropertyActiveNodeChanged( texture, &pProperty );
}
