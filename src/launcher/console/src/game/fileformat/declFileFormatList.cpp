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

#include "declFileFormat.h"
#include "declFileFormatList.h"

#include <dragengine/common/exceptions.h>



// Class declFileFormatList
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

declFileFormatList::declFileFormatList(){
}

declFileFormatList::~declFileFormatList(){
	RemoveAll();
}



// Management
///////////////

int declFileFormatList::GetCount() const{
	return pFormats.GetCount();
}

declFileFormat *declFileFormatList::GetAt( int index ) const{
	return ( declFileFormat* )pFormats.GetAt( index );
}

bool declFileFormatList::Has( declFileFormat *format ) const{
	return pFormats.Has( format );
}

int declFileFormatList::IndexOf( declFileFormat *format ) const{
	return pFormats.IndexOf( format );
}

void declFileFormatList::Add( declFileFormat *format ){
	if( ! format ){
		DETHROW( deeInvalidParam );
	}
	
	pFormats.Add( format );
}

void declFileFormatList::Remove( declFileFormat *format ){
	int index = IndexOf( format );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pFormats.RemoveFrom( index );
}

void declFileFormatList::RemoveAll(){
	pFormats.RemoveAll();
}
