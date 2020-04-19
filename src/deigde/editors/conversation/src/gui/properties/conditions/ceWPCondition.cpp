/* 
 * Drag[en]gine IGDE Conversation Editor
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

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ceWPCondition.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/condition/ceConversationCondition.h"
#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../conversation/actor/ceConversationActor.h"

#include <deigde/gui/igdeComboBox.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceWPCondition
///////////////////////

// Constructor, destructor
////////////////////////////

ceWPCondition::ceWPCondition( ceWPTopic &parentPanel ) :
igdeContainerForm( parentPanel.GetEnvironment() ),
pParentPanel( parentPanel ){
}

ceWPCondition::~ceWPCondition(){
}



// Management
///////////////

void ceWPCondition::UpdateComboBoxWithActorIDList( igdeComboBox &combobox ){
	const ceConversation * const conversation = pParentPanel.GetConversation();
	const decString selection( combobox.GetText() );
	
	combobox.RemoveAllItems();
	
	if( conversation ){
		const ceConversationActorList &list = conversation->GetActorList();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			if( list.GetAt( i )->GetID().IsEmpty() ){
				continue;
			}
			
			const decString &id = list.GetAt( i )->GetID();
			if( ! combobox.HasItem( id ) ){
				combobox.AddItem( id );
			}
			
			const decString &aliasID = list.GetAt( i )->GetAliasID();
			if( ! aliasID.IsEmpty() && ! combobox.HasItem( aliasID ) ){
				combobox.AddItem( aliasID );
			}
		}
		
		combobox.SortItems();
	}
	
	combobox.SetText( selection );
}
