/* 
 * Drag[en]gine Live Launcher
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

#include "dellGame.h"
#include "dellGameList.h"

#include <dragengine/common/exceptions.h>



// Class dellGameList
///////////////////////

// Constructors and Destructors
/////////////////////////////////

dellGameList::dellGameList(){
}

dellGameList::~dellGameList(){
	RemoveAll();
}



// Management
///////////////

int dellGameList::GetCount() const{
	return pGames.GetCount();
}

dellGame *dellGameList::GetAt( int index ) const{
	return ( dellGame* )pGames.GetAt( index );
}

dellGame *dellGameList::GetWithID( const decUuid &id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pGames.GetCount();
	dellGame *game;
	int i;
	
	for( i=0; i<count; i++ ){
		game = ( dellGame* )pGames.GetAt( i );
		
		if( game->GetIdentifier() == id ){
			return game;
		}
	}
	
	return NULL;
}

dellGameList dellGameList::GetWithAlias( const char *alias ) const{
	const int count = pGames.GetCount();
	dellGameList list;
	int i;
	
	for( i=0; i<count; i++ ){
		dellGame * const game = ( dellGame* )pGames.GetAt( i );
		if( game->GetAliasIdentifier().Equals( alias ) ){
			list.Add( game );
		}
	}
	
	return list;
}

bool dellGameList::Has( dellGame *game ) const{
	return pGames.Has( game );
}

bool dellGameList::HasWithID( const decUuid &id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pGames.GetCount();
	dellGame *game;
	int i;
	
	for( i=0; i<count; i++ ){
		game = ( dellGame* )pGames.GetAt( i );
		
		if( game->GetIdentifier() == id ){
			return true;
		}
	}
	
	return false;
}

int dellGameList::IndexOf( dellGame *game ) const{
	return pGames.IndexOf( game );
}

int dellGameList::IndexOfWithID( const decUuid &id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pGames.GetCount();
	dellGame *game;
	int i;
	
	for( i=0; i<count; i++ ){
		game = ( dellGame* )pGames.GetAt( i );
		
		if( game->GetIdentifier() == id ){
			return i;
		}
	}
	
	return -1;
}

void dellGameList::Add( dellGame *game ){
	if( ! game || HasWithID( game->GetIdentifier() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pGames.Add( game );
}

void dellGameList::Remove( dellGame *game ){
	const int index = IndexOf( game );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pGames.RemoveFrom( index );
}

void dellGameList::RemoveAll(){
	pGames.RemoveAll();
}
