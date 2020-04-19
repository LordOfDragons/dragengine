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

#include "ceCAIfElseCase.h"
#include "ceCAIfElseCaseList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceCAIfElseCase
/////////////////////////

// Constructor, destructor
////////////////////////////

ceCAIfElseCaseList::ceCAIfElseCaseList(){
}

ceCAIfElseCaseList::~ceCAIfElseCaseList(){
}



// Management
///////////////

int ceCAIfElseCaseList::GetCount() const{
	return pCases.GetCount();
}

ceCAIfElseCase *ceCAIfElseCaseList::GetAt( int index ) const{
	return ( ceCAIfElseCase* )pCases.GetAt( index );
}

int ceCAIfElseCaseList::IndexOf( ceCAIfElseCase *ifcase ) const{
	return pCases.IndexOf( ifcase );
}

bool ceCAIfElseCaseList::Has( ceCAIfElseCase *ifcase ) const{
	return pCases.Has( ifcase );
}

void ceCAIfElseCaseList::Add( ceCAIfElseCase *ifcase ){
	if( ! ifcase ){
		DETHROW( deeInvalidParam );
	}
	
	pCases.Add( ifcase );
}

void ceCAIfElseCaseList::InsertAt( ceCAIfElseCase *ifcase, int index ){
	if( ! ifcase ){
		DETHROW( deeInvalidParam );
	}
	
	pCases.Insert( ifcase, index );
}

void ceCAIfElseCaseList::MoveTo( ceCAIfElseCase *ifcase, int index ){
	pCases.Move( ifcase, index );
}

void ceCAIfElseCaseList::Remove( ceCAIfElseCase *ifcase ){
	pCases.Remove( ifcase );
}

void ceCAIfElseCaseList::RemoveAll(){
	pCases.RemoveAll();
}



ceCAIfElseCaseList &ceCAIfElseCaseList::operator=( const ceCAIfElseCaseList &list ){
	pCases = list.pCases;
	return *this;
}
