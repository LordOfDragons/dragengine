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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cePlaybackCommand.h"
#include "cePlaybackCommandList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class cePlaybackCommand
////////////////////////////

// Constructor, destructor
////////////////////////////

cePlaybackCommandList::cePlaybackCommandList(){
}

cePlaybackCommandList::cePlaybackCommandList( const cePlaybackCommandList &list ){
	*this = list;
}

cePlaybackCommandList::~cePlaybackCommandList(){
}



// Management
///////////////

int cePlaybackCommandList::GetCount() const{
	return pCommands.GetCount();
}

cePlaybackCommand *cePlaybackCommandList::GetAt( int index ) const{
	return ( cePlaybackCommand* )pCommands.GetAt( index );
}

cePlaybackCommand *cePlaybackCommandList::GetWith( const char *command ) const{
	const int count = pCommands.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		cePlaybackCommand * const entry = ( cePlaybackCommand* )pCommands.GetAt( i );
		
		if( entry->GetCommand().Equals( command ) ){
			return entry;
		}
	}
	
	return NULL;
}

int cePlaybackCommandList::IndexOf( cePlaybackCommand *command ) const{
	return pCommands.IndexOf( command );
}

int cePlaybackCommandList::IndexWith( const char *command ) const{
	const int count = pCommands.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const cePlaybackCommand &entry = *( ( cePlaybackCommand* )pCommands.GetAt( i ) );
		
		if( entry.GetCommand().Equals( command ) ){
			return i;
		}
	}
	
	return -1;
}

bool cePlaybackCommandList::Has( cePlaybackCommand *command ) const{
	return pCommands.Has( command );
}

bool cePlaybackCommandList::HasWith( const char *command ) const{
	return IndexWith( command ) != -1;
}

void cePlaybackCommandList::Add( cePlaybackCommand *command ){
	if( ! command || Has( command ) ){
		DETHROW( deeInvalidParam );
	}
	
	pCommands.Add( command );
}

void cePlaybackCommandList::Remove( cePlaybackCommand *command ){
	pCommands.Remove( command );
}

void cePlaybackCommandList::RemoveAll(){
	pCommands.RemoveAll();
}



cePlaybackCommandList &cePlaybackCommandList::operator=( const cePlaybackCommandList &list ){
	pCommands = list.pCommands;
	return *this;
}
