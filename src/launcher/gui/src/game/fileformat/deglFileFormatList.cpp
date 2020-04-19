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

#include "deglFileFormat.h"
#include "deglFileFormatList.h"

#include <dragengine/common/exceptions.h>



// Class deglFileFormatList
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deglFileFormatList::deglFileFormatList(){
}

deglFileFormatList::~deglFileFormatList(){
	RemoveAllFormats();
}



// Management
///////////////

int deglFileFormatList::GetFormatCount() const{
	return pFormats.GetCount();
}

deglFileFormat *deglFileFormatList::GetFormatAt( int index ) const{
	return ( deglFileFormat* )pFormats.GetAt( index );
}

bool deglFileFormatList::HasFormat( deglFileFormat *format ) const{
	return pFormats.Has( format );
}

int deglFileFormatList::IndexOfFormat( deglFileFormat *format ) const{
	return pFormats.IndexOf( format );
}

void deglFileFormatList::AddFormat( deglFileFormat *format ){
	if( ! format ) DETHROW( deeInvalidParam );
	
	pFormats.Add( format );
}

void deglFileFormatList::RemoveFormat( deglFileFormat *format ){
	int index = IndexOfFormat( format );
	
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	pFormats.RemoveFrom( index );
}

void deglFileFormatList::RemoveAllFormats(){
	pFormats.RemoveAll();
}
