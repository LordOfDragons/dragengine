/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "delGame.h"
#include "delGameList.h"

#include <dragengine/common/exceptions.h>



// Class delGameList
///////////////////////

// Constructors and Destructors
/////////////////////////////////

delGameList::delGameList(){
}

delGameList::~delGameList(){
	RemoveAll();
}



// Management
///////////////

int delGameList::GetCount() const{
	return pGames.GetCount();
}

delGame *delGameList::GetAt( int index ) const{
	return ( delGame* )pGames.GetAt( index );
}

delGame *delGameList::GetWithID( const decUuid &id ) const{
	if( ! id ){
		DETHROW_INFO( deeInvalidParam, "id is 0 uuid" );
	}
	
	const int count = pGames.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delGame * const game = ( delGame* )pGames.GetAt( i );
		if( game->GetIdentifier() == id ){
			return game;
		}
	}
	
	return NULL;
}

delGameList delGameList::GetWithAlias( const char *alias ) const{
	const int count = pGames.GetCount();
	delGameList list;
	int i;
	
	for( i=0; i<count; i++ ){
		delGame * const game = ( delGame* )pGames.GetAt( i );
		if( game->GetAliasIdentifier().Equals( alias ) ){
			list.Add( game );
		}
	}
	
	return list;
}

bool delGameList::Has( delGame *game ) const{
	return pGames.Has( game );
}

bool delGameList::HasWithID( const decUuid &id ) const{
	if( ! id ){
		DETHROW_INFO( deeInvalidParam, "id is 0 uuid" );
	}
	
	const int count = pGames.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const delGame * const game = ( delGame* )pGames.GetAt( i );
		if( game->GetIdentifier() == id ){
			return true;
		}
	}
	
	return false;
}

int delGameList::IndexOf( delGame *game ) const{
	return pGames.IndexOf( game );
}

int delGameList::IndexOfWithID( const decUuid &id ) const{
	if( ! id ){
		DETHROW_INFO( deeInvalidParam, "id is 0 uuid" );
	}
	
	const int count = pGames.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const delGame * const game = ( delGame* )pGames.GetAt( i );
		if( game->GetIdentifier() == id ){
			return i;
		}
	}
	
	return -1;
}

void delGameList::Add( delGame *game ){
	if( ! game ){
		DETHROW_INFO( deeNullPointer, "game" );
	}
	if( HasWithID( game->GetIdentifier() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pGames.Add( game );
}

void delGameList::Remove( delGame *game ){
	const int index = IndexOf( game );
	if( index == -1 ){
		DETHROW_INFO( deeInvalidParam, "game is absent" );
	}
	
	pGames.RemoveFrom( index );
}

void delGameList::RemoveAll(){
	pGames.RemoveAll();
}
