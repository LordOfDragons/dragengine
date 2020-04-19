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

#include "cePCBOption.h"
#include "cePCBOptionList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class cePCBOption
//////////////////////

// Constructor, destructor
////////////////////////////

cePCBOptionList::cePCBOptionList(){
}

cePCBOptionList::~cePCBOptionList(){
}



// Management
///////////////

int cePCBOptionList::GetCount() const{
	return pOptions.GetCount();
}

cePCBOption *cePCBOptionList::GetAt( int index ) const{
	return ( cePCBOption* )pOptions.GetAt( index );
}

int cePCBOptionList::IndexOf( cePCBOption *option ) const{
	return pOptions.IndexOf( option );
}

bool cePCBOptionList::Has( cePCBOption *option ) const{
	return pOptions.Has( option );
}

void cePCBOptionList::Add( cePCBOption *option ){
	if( ! option ) DETHROW( deeInvalidParam );
	
	pOptions.Add( option );
}

void cePCBOptionList::InsertAt( cePCBOption *option, int index ){
	if( ! option ) DETHROW( deeInvalidParam );
	
	pOptions.Insert( option, index );
}

void cePCBOptionList::MoveTo( cePCBOption *option, int index ){
	pOptions.Move( option, index );
}

void cePCBOptionList::Remove( cePCBOption *option ){
	pOptions.Remove( option );
}

void cePCBOptionList::RemoveAll(){
	pOptions.RemoveAll();
}



cePCBOptionList &cePCBOptionList::operator=( const cePCBOptionList &list ){
	pOptions = list.pOptions;
	return *this;
}
