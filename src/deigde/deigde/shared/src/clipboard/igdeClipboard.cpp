/* 
 * Drag[en]gine IGDE
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

#include "igdeClipboard.h"
#include "igdeClipboardData.h"

#include <dragengine/common/exceptions.h>



// Class igdeClipboard
////////////////////////

// Constructor, destructor
////////////////////////////

igdeClipboard::igdeClipboard(){
}

igdeClipboard::~igdeClipboard(){
}



// Management
///////////////

int igdeClipboard::GetCount() const{
	return pData.GetCount();
}

bool igdeClipboard::HasWithTypeName( const char *typeName ) const{
	const int count = pData.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( igdeClipboardData* )pData.GetAt( i ) )->GetTypeName() == typeName ){
			return true;
		}
	}
	
	return false;
}

igdeClipboardData *igdeClipboard::GetAt( int index ) const{
	return ( igdeClipboardData* )pData.GetAt( index );
}

igdeClipboardData *igdeClipboard::GetWithTypeName( const char *typeName ) const{
	const int count = pData.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		igdeClipboardData * const data = ( igdeClipboardData* )pData.GetAt( i );
		if( data->GetTypeName() == typeName ){
			return data;
		}
	}
	
	return NULL;
}

void igdeClipboard::Set( igdeClipboardData *data ){
	if( ! data ){
		DETHROW( deeInvalidParam );
	}
	
	Clear( data->GetTypeName() );
	pData.Add( data );
}

void igdeClipboard::Clear( const char *typeName ){
	igdeClipboardData * const data = GetWithTypeName( typeName );
	if( data ){
		pData.Remove( data );
	}
}

void igdeClipboard::ClearAll(){
	pData.RemoveAll();
}
