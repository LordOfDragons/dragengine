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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ceClipboardDataTopic.h"
#include "../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceClipboardDataTopic
///////////////////////////////

const char * const ceClipboardDataTopic::TYPE_NAME = "topic";

// Constructor, destructor
////////////////////////////

ceClipboardDataTopic::ceClipboardDataTopic( const ceConversationTopicList &topics ) :
igdeClipboardData( TYPE_NAME )
{
	const int count = topics.GetCount();
	ceConversationTopic *topic = NULL;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			topic = new ceConversationTopic( *topics.GetAt( i ) );
			pTopics.Add( topic );
			topic->FreeReference();
			topic = NULL;
		}
		
	}catch( const deException & ){
		if( topic ){
			topic->FreeReference();
		}
		throw;
	}
}

ceClipboardDataTopic::~ceClipboardDataTopic(){
	pTopics.RemoveAll();
}
