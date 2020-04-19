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

#include "ceTextBoxText.h"
#include "ceTextBoxTextList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceTextBoxText
////////////////////////

// Constructor, destructor
////////////////////////////

ceTextBoxTextList::ceTextBoxTextList(){
}

ceTextBoxTextList::~ceTextBoxTextList(){
}



// Management
///////////////

int ceTextBoxTextList::GetCount() const{
	return pTexts.GetCount();
}

ceTextBoxText *ceTextBoxTextList::GetAt( int index ) const{
	return ( ceTextBoxText* )pTexts.GetAt( index );
}

int ceTextBoxTextList::IndexOf( ceTextBoxText *text ) const{
	return pTexts.IndexOf( text );
}

bool ceTextBoxTextList::Has( ceTextBoxText *text ) const{
	return pTexts.Has( text );
}

void ceTextBoxTextList::Add( ceTextBoxText *text ){
	if( ! text ) DETHROW( deeInvalidParam );
	
	pTexts.Add( text );
}

void ceTextBoxTextList::InsertAt( ceTextBoxText *text, int index ){
	if( ! text ) DETHROW( deeInvalidParam );
	
	pTexts.Insert( text, index );
}

void ceTextBoxTextList::MoveTo( ceTextBoxText *text, int index ){
	pTexts.Move( text, index );
}

void ceTextBoxTextList::Remove( ceTextBoxText *text ){
	pTexts.Remove( text );
}

void ceTextBoxTextList::RemoveAll(){
	pTexts.RemoveAll();
}



ceTextBoxTextList &ceTextBoxTextList::operator=( const ceTextBoxTextList &list ){
	pTexts = list.pTexts;
	return *this;
}
