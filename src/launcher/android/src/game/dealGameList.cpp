/* 
 * Drag[en]gine Android Launcher
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

#include "dealGame.h"
#include "dealGameList.h"

#include "../common/exceptions.h"



// Class dealGameList
///////////////////////

// Constructors and Destructors
/////////////////////////////////

dealGameList::dealGameList(){
}

dealGameList::~dealGameList(){
	RemoveAll();
}



// Management
///////////////

int dealGameList::GetCount() const{
	return pGames.GetCount();
}

dealGame *dealGameList::GetAt( int index ) const{
	return ( dealGame* )pGames.GetAt( index );
}

dealGame *dealGameList::GetWithID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pGames.GetCount();
	dealGame *game;
	int i;
	
	for( i=0; i<count; i++ ){
		game = ( dealGame* )pGames.GetAt( i );
		
		if( game->GetIdentifier().Equals( id ) ){
			return game;
		}
	}
	
	return NULL;
}

bool dealGameList::Has( dealGame *game ) const{
	return pGames.Has( game );
}

bool dealGameList::HasWithID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pGames.GetCount();
	dealGame *game;
	int i;
	
	for( i=0; i<count; i++ ){
		game = ( dealGame* )pGames.GetAt( i );
		
		if( game->GetIdentifier().Equals( id ) ){
			return true;
		}
	}
	
	return false;
}

int dealGameList::IndexOf( dealGame *game ) const{
	return pGames.IndexOf( game );
}

int dealGameList::IndexOfWithID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pGames.GetCount();
	dealGame *game;
	int i;
	
	for( i=0; i<count; i++ ){
		game = ( dealGame* )pGames.GetAt( i );
		
		if( game->GetIdentifier().Equals( id ) ){
			return i;
		}
	}
	
	return -1;
}

void dealGameList::Add( dealGame *game ){
	if( ! game || HasWithID( game->GetIdentifier().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pGames.Add( game );
}

void dealGameList::Remove( dealGame *game ){
	const int index = IndexOf( game );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pGames.RemoveFrom( index );
}

void dealGameList::RemoveAll(){
	pGames.RemoveAll();
}
