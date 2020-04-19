/* 
 * Drag[en]gine Console Launcher
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

#include "declGame.h"
#include "declGameList.h"

#include <dragengine/common/exceptions.h>



// Class declGameList
///////////////////////

// Constructors and Destructors
/////////////////////////////////

declGameList::declGameList(){
}

declGameList::~declGameList(){
	RemoveAll();
}



// Management
///////////////

int declGameList::GetCount() const{
	return pGames.GetCount();
}

declGame *declGameList::GetAt( int index ) const{
	return ( declGame* )pGames.GetAt( index );
}

declGame *declGameList::GetWithID( const decUuid &id ) const{
	const int count = pGames.GetCount();
	declGame *game;
	int i;
	
	for( i=0; i<count; i++ ){
		game = ( declGame* )pGames.GetAt( i );
		
		if( game->GetIdentifier() == id ){
			return game;
		}
	}
	
	return NULL;
}

declGameList declGameList::GetWithAlias( const char *alias ) const{
	const int count = pGames.GetCount();
	declGameList list;
	int i;
	
	for( i=0; i<count; i++ ){
		declGame * const game = ( declGame* )pGames.GetAt( i );
		if( game->GetAliasIdentifier().Equals( alias ) ){
			list.Add( game );
		}
	}
	
	return list;
}

bool declGameList::Has( declGame *game ) const{
	return pGames.Has( game );
}

bool declGameList::HasWithID( const decUuid &id ) const{
	const int count = pGames.GetCount();
	declGame *game;
	int i;
	
	for( i=0; i<count; i++ ){
		game = ( declGame* )pGames.GetAt( i );
		if( game->GetIdentifier() == id ){
			return true;
		}
	}
	
	return false;
}

int declGameList::IndexOf( declGame *game ) const{
	return pGames.IndexOf( game );
}

int declGameList::IndexOfWithID( const decUuid &id ) const{
	const int count = pGames.GetCount();
	declGame *game;
	int i;
	
	for( i=0; i<count; i++ ){
		game = ( declGame* )pGames.GetAt( i );
		if( game->GetIdentifier() == id ){
			return i;
		}
	}
	
	return -1;
}

void declGameList::Add( declGame *game ){
	if( ! game || HasWithID( game->GetIdentifier() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pGames.Add( game );
}

void declGameList::Remove( declGame *game ){
	const int index = IndexOf( game );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pGames.RemoveFrom( index );
}

void declGameList::RemoveAll(){
	pGames.RemoveAll();
}
