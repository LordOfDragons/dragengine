/* 
 * Drag[en]gine GUI Launcher
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

#include "deglGame.h"
#include "deglGameList.h"

#include <dragengine/common/exceptions.h>



// Class deglGameList
///////////////////////

// Constructors and Destructors
/////////////////////////////////

deglGameList::deglGameList(){
}

deglGameList::~deglGameList(){
	RemoveAll();
}



// Management
///////////////

int deglGameList::GetCount() const{
	return pGames.GetCount();
}

deglGame *deglGameList::GetAt( int index ) const{
	return ( deglGame* )pGames.GetAt( index );
}

deglGame *deglGameList::GetWithID( const decUuid &id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pGames.GetCount();
	deglGame *game;
	int i;
	
	for( i=0; i<count; i++ ){
		game = ( deglGame* )pGames.GetAt( i );
		
		if( game->GetIdentifier() == id ){
			return game;
		}
	}
	
	return NULL;
}

deglGameList deglGameList::GetWithAlias( const char *alias ) const{
	const int count = pGames.GetCount();
	deglGameList list;
	int i;
	
	for( i=0; i<count; i++ ){
		deglGame * const game = ( deglGame* )pGames.GetAt( i );
		if( game->GetAliasIdentifier().Equals( alias ) ){
			list.Add( game );
		}
	}
	
	return list;
}

bool deglGameList::Has( deglGame *game ) const{
	return pGames.Has( game );
}

bool deglGameList::HasWithID( const decUuid &id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pGames.GetCount();
	deglGame *game;
	int i;
	
	for( i=0; i<count; i++ ){
		game = ( deglGame* )pGames.GetAt( i );
		
		if( game->GetIdentifier() == id ){
			return true;
		}
	}
	
	return false;
}

int deglGameList::IndexOf( deglGame *game ) const{
	return pGames.IndexOf( game );
}

int deglGameList::IndexOfWithID( const decUuid &id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pGames.GetCount();
	deglGame *game;
	int i;
	
	for( i=0; i<count; i++ ){
		game = ( deglGame* )pGames.GetAt( i );
		
		if( game->GetIdentifier() == id ){
			return i;
		}
	}
	
	return -1;
}

void deglGameList::Add( deglGame *game ){
	if( ! game || HasWithID( game->GetIdentifier() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pGames.Add( game );
}

void deglGameList::Remove( deglGame *game ){
	const int index = IndexOf( game );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pGames.RemoveFrom( index );
}

void deglGameList::RemoveAll(){
	pGames.RemoveAll();
}
