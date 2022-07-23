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

#include "delFileFormat.h"
#include "delFileFormatList.h"

#include <dragengine/common/exceptions.h>



// Class delFileFormatList
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delFileFormatList::delFileFormatList(){
}

delFileFormatList::~delFileFormatList(){
	RemoveAll();
}



// Management
///////////////

int delFileFormatList::GetCount() const{
	return pFormats.GetCount();
}

delFileFormat *delFileFormatList::GetAt( int index ) const{
	return ( delFileFormat* )pFormats.GetAt( index );
}

bool delFileFormatList::Has( delFileFormat *format ) const{
	return pFormats.Has( format );
}

int delFileFormatList::IndexOf( delFileFormat *format ) const{
	return pFormats.IndexOf( format );
}

void delFileFormatList::Add( delFileFormat *format ){
	if( ! format ){
		DETHROW_INFO( deeNullPointer, "format" );
	}
	
	pFormats.Add( format );
}

void delFileFormatList::Remove( delFileFormat *format ){
	const int index = IndexOf ( format );
	if( index == -1 ){
		DETHROW_INFO( deeInvalidParam, "format is absent" );
	}
	
	pFormats.RemoveFrom( index );
}

void delFileFormatList::RemoveAll(){
	pFormats.RemoveAll();
}
